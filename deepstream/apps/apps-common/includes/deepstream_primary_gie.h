#ifndef __NVGSTDS_PRIMARY_GIE_H__
#define __NVGSTDS_PRIMARY_GIE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "deepstream_gie.h"

typedef struct {
	GstElement *bin;
	GstElement *queue;
	GstElement *nvvidconv;
	GstElement *primary_gie;
} NvDsPrimaryGieBin;

// @ref CONFIG_GROUP_PRIMARY_GIE
gboolean create_primary_gie_bin(NvDsGieConfig *config, NvDsPrimaryGieBin *bin);

#ifdef __cplusplus
}
#endif

#endif
