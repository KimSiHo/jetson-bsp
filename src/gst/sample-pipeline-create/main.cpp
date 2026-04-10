#include <gst/gst.h>
#include <iostream>

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    GstElement *pipeline = gst_pipeline_new("pipeline");
    GstElement *source   = gst_element_factory_make("videotestsrc", "source");
    GstElement *sink     = gst_element_factory_make("autovideosink", "sink");

    if (!pipeline || !source || !sink)
    {
        std::cerr << "Failed to create GStreamer elements." << std::endl;
        return -1;
    }

    gst_bin_add_many(GST_BIN(pipeline), source, sink, NULL);

    if (!gst_element_link(source, sink))
    {
        std::cerr << "Failed to link elements." << std::endl;
        gst_object_unref(pipeline);
        return -1;
    }

    std::cout << "Pipeline created successfully." << std::endl;

    gst_object_unref(pipeline);

    return 0;
}