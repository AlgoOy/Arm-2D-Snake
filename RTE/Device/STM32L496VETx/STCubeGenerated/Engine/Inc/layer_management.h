#ifndef __LAYER_MANAGEMENT_H
#define __LAYER_MANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtthread.h"
#include "arm_2d_helper.h"

#define Screen_Width 240
#define Screen_Height 240

typedef struct cell_t {
	__arm_2d_color_t tColor;
	rt_uint8_t chOpacity;
	rt_uint8_t blsDirty;
} cell_t;

typedef struct layer_t {
	rt_uint16_t hwXCount;
	rt_uint16_t hwYCount;
	cell_t *ptCells;
} layer_t;

void GameEngineEntry(void *param);
void register_layer(layer_t *layer);
void apply_for_refresh (void);

#ifdef __cplusplus
}
#endif

#endif /* __LAYER_MANAGEMENT_H */