#include "gsthello.h"

G_DEFINE_TYPE(GstHello, gst_hello, GST_TYPE_ELEMENT)

static GstStateChangeReturn
gst_hello_change_state(GstElement *element,
                       GstStateChange transition)
{
    if (transition == GST_STATE_CHANGE_NULL_TO_READY)
        g_print("Hello World!\n");

    return GST_ELEMENT_CLASS(gst_hello_parent_class)
        ->change_state(element, transition);
}

static void
gst_hello_class_init(GstHelloClass *klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS(klass);

    gst_element_class_set_static_metadata(
        element_class,
        "Hello Element",
        "Generic",
        "Print Hello World",
        "kim");

    element_class->change_state = gst_hello_change_state;
}

static void
gst_hello_init(GstHello *self)
{
}
