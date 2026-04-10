#ifndef __NVGSTDS_PREPROCESS_H__
#define __NVGSTDS_PREPROCESS_H__

#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	gboolean enable;
	/*gie id on which preprocessing is to be done*/
	gint operate_on_gie_id;
	gboolean is_operate_on_gie_id_set;
	/** config file path having properties for preprocess */
	gchar *config_file_path;
} NvDsPreProcessConfig;

typedef struct {
	GstElement *bin;
	GstElement *queue;
	GstElement *preprocess;
} NvDsPreProcessBin;

// @ref CONFIG_GROUP_PREPROCESS
gboolean create_preprocess_bin(NvDsPreProcessConfig *config, NvDsPreProcessBin *bin);

#ifdef __cplusplus
}
#endif

#endif