#include <gst/gst.h>
#include <iostream>

static void pad_added_handler(GstElement *src, GstPad *new_pad, gpointer data)
{
    GstElement *conv = GST_ELEMENT(data);

    GstPad *sink_pad = gst_element_get_static_pad(conv, "sink");

    if (gst_pad_is_linked(sink_pad)) {
        gst_object_unref(sink_pad);
        return;
    }

    if (gst_pad_link(new_pad, sink_pad) != GST_PAD_LINK_OK)
        std::cerr << "Failed to link decodebin -> nvvidconv" << std::endl;

    gst_object_unref(sink_pad);
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    GstElement *pipeline = gst_pipeline_new("pipeline");
    GstElement *src      = gst_element_factory_make("filesrc", "src");
    GstElement *decode   = gst_element_factory_make("decodebin", "decode");
    GstElement *conv     = gst_element_factory_make("nvvidconv", "conv");
    GstElement *sink     = gst_element_factory_make("nveglglessink", "sink");

    if (!pipeline || !src || !decode || !conv || !sink) {
        std::cerr << "Failed to create elements." << std::endl;
        return -1;
    }

    g_object_set(src,
                 "location", "/home/great/video.mp4",
                 NULL);

    gst_bin_add_many(GST_BIN(pipeline),
                     src, decode, conv, sink,
                     NULL);

    if (!gst_element_link(src, decode)) {
        std::cerr << "filesrc -> decodebin failed." << std::endl;
        return -1;
    }

    if (!gst_element_link(conv, sink)) {
        std::cerr << "nvvidconv -> sink failed." << std::endl;
        return -1;
    }

    g_signal_connect(decode,
                     "pad-added",
                     G_CALLBACK(pad_added_handler),
                     conv);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    GstBus *bus = gst_element_get_bus(pipeline);
    gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    gst_object_unref(bus);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
