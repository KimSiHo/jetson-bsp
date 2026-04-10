#ifndef DEEPSTREAM_PROXIMITY_H
#define DEEPSTREAM_PROXIMITY_H

#include <stdio.h>

#include <gst/gst.h>

#include "deepstream_app_version.h"
#include "deepstream_common.h"
#include "deepstream_config.h"
#include "deepstream_dsexample.h"
#include "deepstream_image_save.h"
#include "deepstream_osd.h"
#include "deepstream_perf.h"
#include "deepstream_preprocess.h"
#include "deepstream_primary_gie.h"
#include "deepstream_secondary_gie.h"
#include "deepstream_secondary_preprocess.h"
#include "deepstream_sinks.h"
#include "deepstream_sources.h"
#include "deepstream_streammux.h"
#include "deepstream_tiled_display.h"
#include "deepstream_tracker.h"
#include "gst-nvdscommonconfig.h"
#include "gst-nvdscustommessage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AppCtx AppCtx;

typedef void (*bbox_generated_callback)(AppCtx *appCtx, GstBuffer *buf, NvDsBatchMeta *batch_meta, guint index);
typedef gboolean (*overlay_graphics_callback)(AppCtx *appCtx, GstBuffer *buf, NvDsBatchMeta *batch_meta, guint index);

typedef struct {
	guint index;
	gulong all_bbox_buffer_probe_id;
	gulong primary_bbox_buffer_probe_id;
	gulong fps_buffer_probe_id;
	GstElement *bin;
	GstElement *tee;
	GstElement *msg_conv;
	NvDsPreProcessBin preprocess_bin;
	NvDsPrimaryGieBin primary_gie_bin;
	NvDsOSDBin osd_bin;
	NvDsSecondaryGieBin secondary_gie_bin;
	NvDsSecondaryPreProcessBin secondary_preprocess_bin;
	NvDsTrackerBin tracker_bin;
	NvDsSinkBin sink_bin;
	NvDsSinkBin demux_sink_bin;
	NvDsDsExampleBin dsexample_bin;
	AppCtx *appCtx;
} NvDsInstanceBin;

typedef struct {
	gulong primary_bbox_buffer_probe_id;
	guint bus_id;
	GstElement *pipeline;
	NvDsSrcParentBin multi_src_bin;
	NvDsInstanceBin instance_bins[MAX_SOURCE_BINS];
	NvDsInstanceBin demux_instance_bins[MAX_SOURCE_BINS];
	NvDsInstanceBin common_elements;
	GstElement *tiler_tee;
	NvDsTiledDisplayBin tiled_display_bin;
	GstElement *demuxer;
	NvDsDsExampleBin dsexample_bin;
	AppCtx *appCtx;
} NvDsPipeline;

typedef struct {
	gboolean enable_perf_measurement;
	gint file_loop;
	gint pipeline_recreate_sec;
	gboolean source_list_enabled;
	guint total_num_sources;
	guint num_source_sub_bins;
	guint num_secondary_gie_sub_bins;
	guint num_secondary_preprocess_sub_bins;
	guint num_sink_sub_bins;
	guint num_message_consumers;
	guint perf_measurement_interval_sec;
	guint sgie_batch_size;
	gboolean extract_sei_type5_data;
	gchar *sei_uuid;
	gboolean low_latency_mode;
	gchar *bbox_dir_path;
	gchar *kitti_track_dir_path;
	gchar *reid_track_dir_path;
	gchar *terminated_track_output_path;
	gchar *shadow_track_output_path;
	gchar **uri_list;
	gchar **sensor_id_list;
	gchar **sensor_name_list;

	NvDsSourceConfig multi_source_config[MAX_SOURCE_BINS];
	NvDsStreammuxConfig streammux_config;
	NvDsOSDConfig osd_config;
	NvDsPreProcessConfig preprocess_config;
	NvDsPreProcessConfig secondary_preprocess_sub_bin_config[MAX_SECONDARY_PREPROCESS_BINS];
	NvDsGieConfig primary_gie_config;
	NvDsTrackerConfig tracker_config;
	NvDsGieConfig secondary_gie_sub_bin_config[MAX_SECONDARY_GIE_BINS];
	NvDsSinkSubBinConfig sink_bin_sub_bin_config[MAX_SINK_BINS];
	NvDsTiledDisplayConfig tiled_display_config;
	NvDsDsExampleConfig dsexample_config;
	NvDsSinkMsgConvBrokerConfig msg_conv_config;
	NvDsImageSave image_save_config;

	/** To support nvmultiurisrcbin */
	gboolean use_nvmultiurisrcbin;
	gboolean stream_name_display;
	guint max_batch_size;
	gchar *http_ip;
	gchar *http_port;
	gboolean source_attr_all_parsed;
	NvDsSourceConfig source_attr_all_config;

	/** To set Global GPU ID for all the componenents at once if needed
	 * This will be used in case gpu_id prop is not set for a component
	 * if gpu_id prop is set for a component, global_gpu_id will be overridden by it */
	gint global_gpu_id;
} NvDsConfig;

typedef struct {
	gulong frame_num;
} NvDsInstanceData;

struct _AppCtx {
	gboolean version;
	gboolean cintr;
	gboolean show_bbox_text;
	gboolean seeking;
	gboolean quit;
	gint person_class_id;
	gint car_class_id;
	gint return_value;
	guint index;
	gint active_source_index;
	GMutex app_lock;
	GCond app_cond;
	NvDsPipeline pipeline;
	NvDsConfig config;
	NvDsConfig override_config;
	NvDsInstanceData instance_data[MAX_SOURCE_BINS];
	NvDsAppPerfStructInt perf_struct;
	bbox_generated_callback bbox_generated_post_analytics_cb;
	bbox_generated_callback all_bbox_generated_cb;
	overlay_graphics_callback overlay_graphics_cb;
	NvDsFrameLatencyInfo *latency_info;
	GMutex latency_lock;
	GThread *ota_handler_thread;
	guint ota_inotify_fd;
	guint ota_watch_desc;
	/** Hash table to save NvDsSensorInfo
	 * obtained with REST API stream/add, remove operations
	 * The key is souce_id */
	GHashTable *sensorInfoHash;
	gboolean eos_received;
};

/**
 * @param bbox_generated_post_analytics_cb [IN]
 * This callback shall be triggered after analytics. (PGIE, Tracker or the last SGIE appearing * in the pipeline)
 */
gboolean create_pipeline(AppCtx *appCtx, bbox_generated_callback bbox_generated_post_analytics_cb,
                         bbox_generated_callback all_bbox_generated_cb, perf_callback perf_cb,
                         overlay_graphics_callback overlay_graphics_cb);
gboolean pause_pipeline(AppCtx *appCtx);
gboolean resume_pipeline(AppCtx *appCtx);
gboolean seek_pipeline(AppCtx *appCtx, glong milliseconds, gboolean seek_is_relative);
void destroy_pipeline(AppCtx *appCtx);
void restart_pipeline(AppCtx *appCtx);

void toggle_show_bbox_text(AppCtx *appCtx);
gboolean parse_config_file(NvDsConfig *config, gchar *cfg_file_path);
NvDsSensorInfo *get_sensor_info(AppCtx *appCtx, guint source_id);

#ifdef __cplusplus
}
#endif

#endif /* DEEPSTREAM_PROXIMITY_H */
