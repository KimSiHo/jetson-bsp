#include <gst/gst.h>
#include <glib.h>

#include "gstnvdsmeta.h"
#include "nvdsmeta.h"

#include <iostream>

typedef struct {
    GstElement *convert_before_mux;
    GstElement *capsfilter;
    GstElement *streammux;
} AppCtx;

static GstPadProbeReturn osd_sink_pad_buffer_probe(
    GstPad *pad,
    GstPadProbeInfo *info,
    gpointer user_data)
{
    GstBuffer *buf = (GstBuffer *)info->data;
    if (!buf)
        return GST_PAD_PROBE_OK;

    NvDsBatchMeta *batch_meta = gst_buffer_get_nvds_batch_meta(buf);
    if (!batch_meta)
        return GST_PAD_PROBE_OK;

    for (NvDsMetaList *l_frame = batch_meta->frame_meta_list;
         l_frame != NULL;
         l_frame = l_frame->next) {

        NvDsFrameMeta *frame_meta = (NvDsFrameMeta *)l_frame->data;
        int obj_count = 0;

        for (NvDsMetaList *l_obj = frame_meta->obj_meta_list;
             l_obj != NULL;
             l_obj = l_obj->next) {

            NvDsObjectMeta *obj_meta = (NvDsObjectMeta *)l_obj->data;
            obj_count++;

            std::cout
                << "frame=" << frame_meta->frame_num
                << " class_id=" << obj_meta->class_id
                << " label=" << obj_meta->obj_label
                << " conf=" << obj_meta->confidence
                << " bbox=("
                << obj_meta->rect_params.left << ","
                << obj_meta->rect_params.top << ","
                << obj_meta->rect_params.width << ","
                << obj_meta->rect_params.height << ")"
                << std::endl;
        }

        if (obj_count > 0) {
            std::cout << "frame=" << frame_meta->frame_num
                      << " objects=" << obj_count << std::endl;
        }
    }

    return GST_PAD_PROBE_OK;
}

static void decodebin_pad_added(GstElement *decodebin, GstPad *new_pad, gpointer data)
{
    AppCtx *ctx = (AppCtx *)data;

    GstCaps *caps = gst_pad_get_current_caps(new_pad);
    if (!caps)
        caps = gst_pad_query_caps(new_pad, NULL);

    const gchar *name = gst_structure_get_name(gst_caps_get_structure(caps, 0));

    if (!g_str_has_prefix(name, "video/")) {
        gst_caps_unref(caps);
        return;
    }

    std::cout << "decodebin pad-added: " << name << std::endl;

    GstPad *conv_sink_pad =
        gst_element_get_static_pad(ctx->convert_before_mux, "sink");

    if (gst_pad_is_linked(conv_sink_pad)) {
        gst_object_unref(conv_sink_pad);
        gst_caps_unref(caps);
        return;
    }

    if (gst_pad_link(new_pad, conv_sink_pad) != GST_PAD_LINK_OK) {
        std::cerr << "Failed to link decodebin -> nvvideoconvert" << std::endl;
    } else {
        std::cout << "Linked decodebin -> nvvideoconvert" << std::endl;
    }

    gst_object_unref(conv_sink_pad);
    gst_caps_unref(caps);
}

static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = (GMainLoop *)data;

    switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
        std::cout << "EOS" << std::endl;
        g_main_loop_quit(loop);
        break;

    case GST_MESSAGE_ERROR: {
        GError *err = NULL;
        gchar *debug = NULL;

        gst_message_parse_error(msg, &err, &debug);

        std::cerr << "ERROR: " << err->message << std::endl;
        if (debug)
            std::cerr << "DEBUG: " << debug << std::endl;

        g_error_free(err);
        g_free(debug);

        g_main_loop_quit(loop);
        break;
    }

    default:
        break;
    }

    return TRUE;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <video.mp4> <nvinfer_config.txt>" << std::endl;
        return -1;
    }

    const char *video_path = argv[1];
    const char *infer_config = argv[2];

    gst_init(&argc, &argv);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    GstElement *pipeline = gst_pipeline_new("deepstream-pipeline");

    GstElement *filesrc = gst_element_factory_make("filesrc", "filesrc");
    GstElement *decodebin = gst_element_factory_make("decodebin", "decodebin");

    GstElement *convert_before_mux =
        gst_element_factory_make("nvvideoconvert", "convert-before-mux");

    GstElement *capsfilter =
        gst_element_factory_make("capsfilter", "capsfilter");

    GstElement *streammux =
        gst_element_factory_make("nvstreammux", "streammux");

    GstElement *pgie =
        gst_element_factory_make("nvinfer", "primary-nvinference-engine");

    GstElement *convert_after_infer =
        gst_element_factory_make("nvvideoconvert", "convert-after-infer");

    GstElement *osd =
        gst_element_factory_make("nvdsosd", "onscreendisplay");

    GstElement *sink =
        gst_element_factory_make("nveglglessink", "sink");

    if (!pipeline || !filesrc || !decodebin || !convert_before_mux ||
        !capsfilter || !streammux || !pgie || !convert_after_infer ||
        !osd || !sink) {
        std::cerr << "Failed to create one or more elements." << std::endl;
        return -1;
    }

    g_object_set(G_OBJECT(filesrc), "location", video_path, NULL);

    GstCaps *caps = gst_caps_from_string("video/x-raw(memory:NVMM), format=NV12");
    g_object_set(G_OBJECT(capsfilter), "caps", caps, NULL);
    gst_caps_unref(caps);

    g_object_set(G_OBJECT(streammux),
                 "batch-size", 1,
                 "width", 1920,
                 "height", 1080,
                 "batched-push-timeout", 40000,
                 "live-source", 0,
                 NULL);

    g_object_set(G_OBJECT(pgie),
                 "config-file-path", infer_config,
                 NULL);

    gst_bin_add_many(GST_BIN(pipeline),
                     filesrc,
                     decodebin,
                     convert_before_mux,
                     capsfilter,
                     streammux,
                     pgie,
                     convert_after_infer,
                     osd,
                     sink,
                     NULL);

    if (!gst_element_link(filesrc, decodebin)) {
        std::cerr << "Failed to link filesrc -> decodebin" << std::endl;
        return -1;
    }

    if (!gst_element_link(convert_before_mux, capsfilter)) {
        std::cerr << "Failed to link nvvideoconvert -> capsfilter" << std::endl;
        return -1;
    }

    GstPad *mux_sink_pad = gst_element_get_request_pad(streammux, "sink_0");
    GstPad *caps_src_pad = gst_element_get_static_pad(capsfilter, "src");

    if (!mux_sink_pad || !caps_src_pad) {
        std::cerr << "Failed to get streammux/caps pads" << std::endl;
        return -1;
    }

    if (gst_pad_link(caps_src_pad, mux_sink_pad) != GST_PAD_LINK_OK) {
        std::cerr << "Failed to link capsfilter -> nvstreammux" << std::endl;
        return -1;
    }

    gst_object_unref(caps_src_pad);
    gst_object_unref(mux_sink_pad);

    if (!gst_element_link_many(streammux,
                               pgie,
                               convert_after_infer,
                               osd,
                               sink,
                               NULL)) {
        std::cerr << "Failed to link streammux -> nvinfer -> osd -> sink" << std::endl;
        return -1;
    }

    AppCtx ctx;
    ctx.convert_before_mux = convert_before_mux;
    ctx.capsfilter = capsfilter;
    ctx.streammux = streammux;

    g_signal_connect(decodebin,
                     "pad-added",
                     G_CALLBACK(decodebin_pad_added),
                     &ctx);

    GstPad *osd_sink_pad = gst_element_get_static_pad(osd, "sink");
    if (osd_sink_pad) {
        gst_pad_add_probe(osd_sink_pad,
                          GST_PAD_PROBE_TYPE_BUFFER,
                          osd_sink_pad_buffer_probe,
                          NULL,
                          NULL);
        gst_object_unref(osd_sink_pad);
    }

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    std::cout << "Starting pipeline..." << std::endl;

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_main_loop_run(loop);

    std::cout << "Stopping pipeline..." << std::endl;

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    g_main_loop_unref(loop);

    return 0;
}