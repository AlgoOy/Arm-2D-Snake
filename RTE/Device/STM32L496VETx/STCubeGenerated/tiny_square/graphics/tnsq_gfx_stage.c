/*
 * Copyright (c) 2023, AlgoOy
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author     Notes
 * 2023-07-08     AlgoOy     the first version
 */

#include "arm_2d.h"

#define __TNSQ_GFX_STAGE_IMPLEMENT__
#define __TNSQ_GFX_LAYER_IMPLEMENT__
#include "__tnsq_gfx_common.h"

#include "arm_2d_helper.h"
#include "arm_extra_controls.h"

#include <stdlib.h>
#include <string.h>

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_IAR__
#   pragma diag_suppress=Pa089,Pe188,Pe177,Pe174
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#   pragma GCC diagnostic ignored "-Wunused-value"
#endif

#undef this
#define this (*ptThis)

static void _tnsq_gfx_on_stage_depose(arm_2d_scene_t *ptScene)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    this.use_as__arm_2d_scene_t.ptPlayer = NULL;

    if (this.blsUserAllocated == RT_FALSE) {
        // todo:judge layer free
        free(ptThis);
    }
}

static void _tnsq_gfx_on_stage_background_start(arm_2d_scene_t *ptScene)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}

static void _tnsq_gfx_on_stage_background_complete(arm_2d_scene_t *ptScene)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}


static void _tnsq_gfx_on_stage_frame_start(arm_2d_scene_t *ptScene)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}

static void _tnsq_gfx_on_stage_frame_complete(arm_2d_scene_t *ptScene)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}

static void _tnsq_gfx_before_stage_switching_out(arm_2d_scene_t *ptScene)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}

static IMPL_PFB_ON_DRAW(_tnsq_gfx_pfb_draw_stage_background_handler)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)pTarget;
    ARM_2D_UNUSED(ptTile);
    ARM_2D_UNUSED(bIsNewFrame);
    /*-----------------------draw back ground begin-----------------------*/



    /*-----------------------draw back ground end  -----------------------*/
    arm_2d_op_wait_async(NULL);

    return arm_fsm_rt_cpl;
}

static IMPL_PFB_ON_DRAW(_tnsq_gfx_pfb_draw_stage_handler)
{
    tnsq_gfx_stage_t *ptThis = (tnsq_gfx_stage_t *)pTarget;
    ARM_2D_UNUSED(ptTile);
    ARM_2D_UNUSED(bIsNewFrame);
    
    arm_2d_canvas(ptTile, __top_canvas) {
    /*-----------------------draw the foreground begin-----------------------*/



    /*-----------------------draw the foreground end  -----------------------*/
    }
    arm_2d_op_wait_async(NULL);

    return arm_fsm_rt_cpl;
}

static void _tnsq_gfx_register_stage_to_disp_adapter(tnsq_gfx_stage_t *ptThis)
{
    assert(ptThis != NULL);
    
    arm_2d_scene_player_append_scenes(this.tStageCFG.ptDispAdapter.ptPlayer, &this.use_as__arm_2d_scene_t, 1);
}


ARM_NONNULL(1) tnsq_gfx_stage_t *__tnsq_gfx_stage_init(tnsq_gfx_stage_cfg_t *ptCFG, tnsq_gfx_stage_t *ptThis)
{
    assert(ptCFG != NULL);
    
    rt_bool_t blsUserAllocated = RT_FALSE;
    
    if (ptThis == NULL)
    {
        ptThis = (tnsq_gfx_stage_t *)malloc(sizeof(tnsq_gfx_stage_t));
        if (ptThis == NULL)
        {
            return NULL;
        }
    }
    else
    {
        blsUserAllocated = RT_TRUE;
    }

    memset(ptThis, 0, sizeof(tnsq_gfx_stage_t));

    *ptThis = (tnsq_gfx_stage_t){
        .use_as__arm_2d_scene_t = {
            .fnBackground   = &_tnsq_gfx_pfb_draw_stage_background_handler,
            .fnScene        = &_tnsq_gfx_pfb_draw_stage_handler,
            //.ptDirtyRegion  = (arm_2d_region_list_item_t *)s_tDirtyRegions,

            .fnOnBGStart    = &_tnsq_gfx_on_stage_background_start,
            .fnOnBGComplete = &_tnsq_gfx_on_stage_background_complete,
            .fnOnFrameStart = &_tnsq_gfx_on_stage_frame_start,
            .fnBeforeSwitchOut = &_tnsq_gfx_before_stage_switching_out,
            .fnOnFrameCPL   = &_tnsq_gfx_on_stage_frame_complete,
            .fnDepose       = &_tnsq_gfx_on_stage_depose,
        },
        .blsUserAllocated = blsUserAllocated,
        .tStageCFG = *ptCFG,
        .ptLayersList = NULL,
    };
    
    tnsq_gfx_register_disp_adapter_to_crtl(tnsq_gfx_get_ctrl(), &ptCFG->ptDispAdapter);

    _tnsq_gfx_register_stage_to_disp_adapter(ptThis);
    
    return ptThis;
}

ARM_NONNULL(1, 2) void tnsq_gfx_register_layer_to_stage(tnsq_gfx_stage_t *ptThis, tnsq_gfx_layer_t *ptLayer)
{
    assert(ptThis != NULL);
    assert(ptLayer != NULL);
    
    tnsq_gfx_layer_t *ptLayerListPtr = this.ptLayersList;
    
    if (ptLayerListPtr == NULL)
    {
        this.ptLayersList = ptLayer;
    }
    else
    {
        while (ptLayerListPtr->ptNext != NULL)
        {
            ptLayerListPtr = ptLayerListPtr->ptNext;
        }
        ptLayerListPtr->ptNext = ptLayer;
    }
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
