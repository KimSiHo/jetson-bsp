#ifndef __NVGSTDS_SECONDARY_PREPROCESS_H__
#define __NVGSTDS_SECONDARY_PREPROCESS_H__

#include "deepstream_preprocess.h"
#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	GstElement *queue;
	GstElement *secondary_preprocess;
	GstElement *tee;
	GstElement *sink;
	gboolean create;
	guint num_children;
	gint parent_index;
} NvDsSecondaryPreProcessBinSubBin;

typedef struct {
	GstElement *bin;
	GstElement *tee;
	GstElement *queue;
	gulong wait_for_secondary_preprocess_process_buf_probe_id;
	gboolean stop;
	gboolean flush;
	NvDsSecondaryPreProcessBinSubBin sub_bins[MAX_SECONDARY_GIE_BINS];
	GMutex wait_lock;
	GCond wait_cond;
} NvDsSecondaryPreProcessBin;

// @ref CONFIG_GROUP_SECONDARY_PREPROCESS
gboolean create_secondary_preprocess_bin(guint num_secondary_preprocess, guint primary_gie_unique_id,
                                         NvDsPreProcessConfig *config_array, NvDsSecondaryPreProcessBin *bin);
void destroy_secondary_preprocess_bin(NvDsSecondaryPreProcessBin *bin);

#ifdef __cplusplus
}
#endif

#endif