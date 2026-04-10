#include <gst/gst.h>

#include "gsthello.h"

#ifndef PACKAGE
#define PACKAGE "gst-test"
#endif

static gboolean
plugin_init(GstPlugin *plugin)
{
    return gst_element_register(
        plugin,
        "hello",
        GST_RANK_NONE,
        GST_TYPE_HELLO);
}

GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    test,
    "Test Plugin",
    plugin_init,
    "1.0",
    "LGPL",
    PACKAGE,
    "https://example.com")
