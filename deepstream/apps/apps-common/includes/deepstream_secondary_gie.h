#ifndef __NVGSTDS_SECONDARY_GIE_H__
#define __NVGSTDS_SECONDARY_GIE_H__

#include "deepstream_gie.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	GstElement *queue;
	GstElement *secondary_gie;
	GstElement *tee;
	GstElement *sink;
	gboolean create;
	guint num_children;
	gint parent_index;
} NvDsSecondaryGieBinSubBin;

typedef struct {
	GstElement *bin;
	GstElement *tee;
	GstElement *queue;
	gulong wait_for_sgie_process_buf_probe_id;
	gboolean stop;
	gboolean flush;
	NvDsSecondaryGieBinSubBin sub_bins[MAX_SECONDARY_GIE_BINS];
	GMutex wait_lock;
	GCond wait_cond;
} NvDsSecondaryGieBin;

// @ref CONFIG_GROUP_SECONDARY_GIE
gboolean create_secondary_gie_bin(guint num_secondary_gie, guint primary_gie_unique_id, NvDsGieConfig *config_array,
                                  NvDsSecondaryGieBin *bin);
void destroy_secondary_gie_bin(NvDsSecondaryGieBin *bin);

#ifdef __cplusplus
}
#endif

#endif
