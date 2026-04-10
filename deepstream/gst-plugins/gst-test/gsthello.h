#pragma once

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_HELLO (gst_hello_get_type())

typedef struct _GstHello
{
    GstElement parent;
} GstHello;

typedef struct _GstHelloClass
{
    GstElementClass parent_class;
} GstHelloClass;

GType gst_hello_get_type(void);

G_END_DECLS
