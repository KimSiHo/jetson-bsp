#ifndef __NVGSTDS_TRACKER_H__
#define __NVGSTDS_TRACKER_H__

#include "nvds_tracker_meta.h"
#include <gst/gst.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	gboolean enable;
	gint width;
	gint height;
	guint gpu_id;
	guint tracking_surf_type;
	gchar *ll_config_file;
	gchar *ll_lib_file;
	guint tracking_surface_type;
	gboolean display_tracking_id;
	guint tracking_id_reset_mode;
	gboolean input_tensor_meta;
	guint input_tensor_gie_id;
	guint compute_hw;
	guint user_meta_pool_size;
	gchar *sub_batches;
	gint sub_batch_err_recovery_trial_cnt;
} NvDsTrackerConfig;

typedef struct {
	GstElement *bin;
	GstElement *tracker;
} NvDsTrackerBin;

typedef uint64_t NvDsTrackerStreamId;

// @ref CONFIG_GROUP_TRACKER
gboolean create_tracking_bin(NvDsTrackerConfig *config, NvDsTrackerBin *bin);

#ifdef __cplusplus
}
#endif

#endif
