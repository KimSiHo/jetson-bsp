#ifndef __NVGSTDS_TILED_DISPLAY_H__
#define __NVGSTDS_TILED_DISPLAY_H__

#include "nvll_osd_struct.h"
#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	GstElement *bin;
	GstElement *queue;
	GstElement *tiler;
} NvDsTiledDisplayBin;

typedef enum {
	NV_DS_TILED_DISPLAY_DISABLE = 0,
	NV_DS_TILED_DISPLAY_ENABLE = 1,
	/** When user sets tiler group enable=2,
	 * all sinks with the key: link-only-to-demux=1
	 * shall be linked to demuxer's src_[source_id] pad
	 * where source_id is the key set in this
	 * corresponding [sink] group
	 */
	NV_DS_TILED_DISPLAY_ENABLE_WITH_PARALLEL_DEMUX = 2
} NvDsTiledDisplayEnable;

typedef struct {
	NvDsTiledDisplayEnable enable;
	guint rows;
	guint columns;
	guint width;
	guint height;
	guint gpu_id;
	guint nvbuf_memory_type;
	/**Compute Scaling HW to use
	 * Applicable only for Jetson; x86 uses GPU by default
	 * (0): Default          - Default, GPU for Tesla, VIC for Jetson
	 * (1): GPU              - GPU
	 * (2): VIC              - VIC
	 *  */
	guint compute_hw;
	guint buffer_pool_size;
	guint square_seq_grid;
} NvDsTiledDisplayConfig;

// @ref CONFIG_GROUP_TILED_DISPLAY
gboolean create_tiled_display_bin(NvDsTiledDisplayConfig *config, NvDsTiledDisplayBin *bin);

#ifdef __cplusplus
}
#endif

#endif
