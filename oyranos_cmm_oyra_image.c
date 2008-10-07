/** @file oyranos_cmm_oyra_image.c
 *
 *  Oyranos is an open source Colour Management System 
 *
 *  Copyright (C) 2008  Kai-Uwe Behrmann
 *
 */

/**
 *  @brief    backends for Oyranos
 *  @internal
 *  @author   Kai-Uwe Behrmann <ku.b@gmx.de>
 *  @par License:\n new BSD <http://www.opensource.org/licenses/bsd-license.php>
 *  @since    2008/01/02
 */

#include "config.h"
#include "oyranos_alpha.h"
#include "oyranos_cmm.h"
#include "oyranos_cmm_oyra.h"
#include "oyranos_helper.h"
#include "oyranos_icc.h"
#include "oyranos_i18n.h"
#include "oyranos_io.h"
#include "oyranos_definitions.h"
#include "oyranos_texts.h"
#include <iconv.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** @func    oyraFilter_ImageOutputPPMCanHandle
 *  @brief   inform about image handling capabilities
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/10/07 (Oyranos: 0.1.8)
 *  @date    2008/10/07
 */
int    oyraFilter_ImageOutputPPMCanHandle (
                                       oyCMMQUERY_e      type,
                                       uint32_t          value )
{
  int ret = -1;
  return ret;
}

oyOptions_s* oyraFilter_ImageOutputPPMValidateOptions
                                     ( oyFilter_s        * filter,
                                       oyOptions_s       * validate,
                                       int                 statical,
                                       uint32_t          * result )
{
  uint32_t error = !filter;
  oyDATATYPE_e data_type = 0;

  if(!error)
    error = filter->type_ != oyOBJECT_FILTER_S;

  if(!error)
    if(filter->image_ && filter->image_->layout_)
    {
      data_type = oyToDataType_m( filter->image_->layout_[0] );
      if(!(data_type == oyUINT8 ||
           data_type == oyUINT16 ||
           data_type == oyFLOAT ||
           data_type == oyDOUBLE
                                   ))
        error = 1;
    }

  *result = error;

  return 0;
}

/** @func    oyraFilterPlug_ImageOutputPPMRun
 *  @brief   implement oyCMMFilter_GetNext_f()
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/10/07 (Oyranos: 0.1.8)
 *  @date    2008/10/07
 */
int      oyraFilterPlug_ImageOutputPPMRun (
                                       oyFilterPlug_s    * requestor_plug,
                                       oyPixelAccess_s   * ticket,
                                       oyArray2d_s      ** pixel )
{
  oyFilterSocket_s * socket = requestor_plug->remote_socket_;
  oyFilter_s * filter = 0;
  int result = 0;

  filter = socket->node->filter;

  /* to reuse the requestor_plug is a exception for the starting request */
  result = filter->api4_->oyCMMFilterPlug_Run( requestor_plug, ticket, pixel );

  return result;
}

oyDATATYPE_e oyra_image_ppm_data_types[5] = {oyUINT8, oyUINT16,
                                             oyFLOAT, oyDOUBLE, 0};

oyConnector_s oyra_imageOutputPPM_connector = {
  oyOBJECT_CONNECTOR_S,0,0,0,
  {oyOBJECT_NAME_S, 0,0,0, "Img", "Image", "Image PPM Plug"},
  oyCONNECTOR_IMAGE, /* connector_type */
  1, /* is_plug == oyFilterPlug_s */
  oyra_image_ppm_data_types,
  4, /* data_types_n; elements in data_types array */
  -1, /* max_colour_offset */
  1, /* min_channels_count; */
  4, /* max_channels_count; */
  1, /* min_colour_count; */
  4, /* max_colour_count; */
  0, /* can_planar; can read separated channels */
  1, /* can_interwoven; can read continuous channels */
  0, /* can_swap; can swap colour channels (BGR)*/
  0, /* can_swap_bytes; non host byte order */
  0, /* can_revert; revert 1 -> 0 and 0 -> 1 */
  1, /* can_premultiplied_alpha; */
  1, /* can_nonpremultiplied_alpha; */
  0, /* can_subpixel; understand subpixel order */
  0, /* oyCHANNELTYPE_e    * channel_types; */
  0, /* count in channel_types */
  1, /* id; relative to oyFilter_s, e.g. 1 */
  0  /* is_mandatory; mandatory flag */
};
oyConnector_s * oyra_imageOutputPPM_connectors[2] = 
             { &oyra_imageOutputPPM_connector, 0 };



/** @func    oyraFilter_ImageRootCanHandle
 *  @brief   inform about image handling capabilities
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/10 (Oyranos: 0.1.8)
 *  @date    2008/07/10
 */
int    oyraFilter_ImageRootCanHandle ( oyCMMQUERY_e      type,
                                       uint32_t          value )
{
  int ret = -1;

  switch(type)
  {
    case oyQUERY_OYRANOS_COMPATIBILITY:
         ret = OYRANOS_VERSION; break;
    case oyQUERY_PIXELLAYOUT_CHANNELCOUNT:
         ret = 65535;
         break;
    case oyQUERY_PIXELLAYOUT_DATATYPE:
         switch(value) {
         case oyUINT8:
         case oyUINT16:
         case oyUINT32:
         case oyFLOAT:
         case oyDOUBLE:
              ret = 1; break;
         case oyHALF:
         default:
              ret = 0; break;
         }
         break;
    case oyQUERY_PIXELLAYOUT_SWAP_COLOURCHANNELS:
         ret = 1;
         break;
    case oyQUERY_PIXELLAYOUT_PLANAR:
         ret = 1;
         break;
    case oyQUERY_HDR:
         ret = 1;
         break;
    default: break;
  }

  return ret;
}

oyOptions_s* oyraFilter_ImageRootValidateOptions
                                     ( oyFilter_s        * filter,
                                       oyOptions_s       * validate,
                                       int                 statical,
                                       uint32_t          * result )
{
  uint32_t error = !filter;
  oyDATATYPE_e data_type = 0;

  if(!error)
    error = filter->type_ != oyOBJECT_FILTER_S;

  if(!error)
    if(filter->image_ && filter->image_->layout_)
    {
      data_type = oyToDataType_m( filter->image_->layout_[0] );
      if(!(data_type == oyUINT8 ||
           data_type == oyUINT16 ||
           data_type == oyUINT32 ||
           data_type == oyHALF ||
           data_type == oyFLOAT ||
           data_type == oyDOUBLE
                                   ))
        error = 1;
    }

  *result = error;

  return 0;
}

/** @func    oyraFilter_ImageRootContextToMem
 *  @brief   implement oyCMMFilter_ContextToMem_f()
 *
 *  Serialise into a Oyranos specific ICC profile containers "Info" tag.
 *  We do not have any binary context to include.
 *  Thus oyFilter_TextToInfo() is fine.
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/17 (Oyranos: 0.1.8)
 *  @date    2008/07/18
 */
oyPointer  oyraFilter_ImageRootContextToMem (
                                       oyFilter_s        * filter,
                                       size_t            * size,
                                       oyCMMptr_s        * oy,
                                       oyAlloc_f           allocateFunc )
{
  return oyFilter_TextToInfo( filter, size, allocateFunc );
}

/** @func    oyraFilterPlug_ImageRootRun
 *  @brief   implement oyCMMFilter_GetNext_f()
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/10 (Oyranos: 0.1.8)
 *  @date    2008/10/05
 */
int      oyraFilterPlug_ImageRootRun ( oyFilterPlug_s    * requestor_plug,
                                       oyPixelAccess_s   * ticket,
                                       oyArray2d_s      ** pixel )
{
  int x = 0, y = 0, n = 0;
  int result = 0, error = 0;
  int is_allocated = 0;
  oyPointer * ptr = 0;
  oyFilterSocket_s * socket = requestor_plug->remote_socket_;
  oyImage_s * image = socket->node->filter->image_;

  x = ticket->start_xy[0];
  y = ticket->start_xy[1];

  result = oyPixelAccess_CalculateNextStartPixel( ticket, requestor_plug);

  if(result != 0)
    return result;

  if(x < image->width &&
     y < image->height &&
     ticket->pixels_n)
  {
    n = ticket->pixels_n;
    if(n == 1)
      ptr = image->getPoint( image, x, y, 0, &is_allocated );

    result = !ptr;

  } else {

    error = oyImage_FillArray( image, 0, 1, pixel, 0 );

  }

  return result;
}

oyDATATYPE_e oyra_image_data_types[7] = {oyUINT8, oyUINT16, oyUINT32,
                                         oyHALF, oyFLOAT, oyDOUBLE, 0};

oyConnector_s oyra_imageRoot_connector = {
  oyOBJECT_CONNECTOR_S,0,0,0,
  {oyOBJECT_NAME_S, 0,0,0, "Img", "Image", "Image Socket"},
  oyCONNECTOR_IMAGE, /* connector_type */
  0, /* is_plug == oyFilterPlug_s */
  oyra_image_data_types, /* data_types */
  6, /* data_types_n; elements in data_types array */
  -1, /* max_colour_offset */
  1, /* min_channels_count; */
  255, /* max_channels_count; */
  1, /* min_colour_count; */
  255, /* max_colour_count; */
  0, /* can_planar; can read separated channels */
  1, /* can_interwoven; can read continuous channels */
  0, /* can_swap; can swap colour channels (BGR)*/
  0, /* can_swap_bytes; non host byte order */
  0, /* can_revert; revert 1 -> 0 and 0 -> 1 */
  1, /* can_premultiplied_alpha; */
  1, /* can_nonpremultiplied_alpha; */
  0, /* can_subpixel; understand subpixel order */
  0, /* oyCHANNELTYPE_e    * channel_types; */
  0, /* count in channel_types */
  1, /* id; relative to oyFilter_s, e.g. 1 */
  0  /* is_mandatory; mandatory flag */
};
oyConnector_s * oyra_imageRoot_connectors[2] = {&oyra_imageRoot_connector,0};

oyConnector_s oyra_imageOutput_connector = {
  oyOBJECT_CONNECTOR_S,0,0,0,
  {oyOBJECT_NAME_S, 0,0,0, "Img", "Image", "Image Plug"},
  oyCONNECTOR_IMAGE, /* connector_type */
  1, /* is_plug == oyFilterPlug_s */
  oyra_image_data_types,
  6, /* data_types_n; elements in data_types array */
  -1, /* max_colour_offset */
  1, /* min_channels_count; */
  255, /* max_channels_count; */
  1, /* min_colour_count; */
  255, /* max_colour_count; */
  0, /* can_planar; can read separated channels */
  1, /* can_interwoven; can read continuous channels */
  0, /* can_swap; can swap colour channels (BGR)*/
  0, /* can_swap_bytes; non host byte order */
  0, /* can_revert; revert 1 -> 0 and 0 -> 1 */
  1, /* can_premultiplied_alpha; */
  1, /* can_nonpremultiplied_alpha; */
  0, /* can_subpixel; understand subpixel order */
  0, /* oyCHANNELTYPE_e    * channel_types; */
  0, /* count in channel_types */
  1, /* id; relative to oyFilter_s, e.g. 1 */
  0  /* is_mandatory; mandatory flag */
};
oyConnector_s* oyra_imageOutput_connectors[2] = {&oyra_imageOutput_connector,0};

uint32_t oyra_image_connectors_max[2] = {1,0};

/** @func    oyraFilter_ImageOutputRun
 *  @brief   implement oyCMMFilter_GetNext_f()
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/19 (Oyranos: 0.1.8)
 *  @date    2008/10/03
 */
int      oyraFilterPlug_ImageOutputRun(oyFilterPlug_s    * requestor_plug,
                                       oyPixelAccess_s   * ticket,
                                       oyArray2d_s      ** pixel )
{
  oyFilterSocket_s * socket = requestor_plug->remote_socket_;
  oyFilter_s * filter = 0;
  int result = 0;

  filter = socket->node->filter;

  /* to reuse the requestor_plug is a exception for the starting request */
  result = filter->api4_->oyCMMFilterPlug_Run( requestor_plug, ticket, pixel );

  return result;
}



/** @instance oyra_api4
 *  @brief    oyra oyCMMapi4_s implementation
 *
 *  a filter writing a PPM image
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/10/07 (Oyranos: 0.1.8)
 *  @date    2008/10/07
 */
oyCMMapi4_s   oyra_api4_image_output_ppm = {

  oyOBJECT_CMM_API4_S,
  0,0,0,
  0,
  
  oyraCMMInit,
  oyraCMMMessageFuncSet,
  oyraFilter_ImageOutputPPMCanHandle,

  "org.oyranos.image.image.output_ppm",

  {0,0,1},

  oyraFilter_ImageOutputPPMValidateOptions,
  oyraWidgetEvent,

  0,
  0,
  oyraFilter_ImageRootContextToMem,
  0,
  oyraFilterPlug_ImageOutputPPMRun,

  {oyOBJECT_NAME_S, 0,0,0, "image_out_ppm", "Image[out_ppm]", "Output PPM Image Filter Object"},
  "Image/Simple Image[out_ppm]", /* category */
  0,   /* options */
  0,   /* opts_ui_ */

  oyra_imageOutputPPM_connectors,   /* plugs */
  1,   /* plugs_n */
  0,   /* plugs_last_add */
  0,   /* sockets */
  0,   /* sockets_n */
  0    /* sockets_last_add */
};

/** @instance oyra_api4
 *  @brief    oyra oyCMMapi4_s implementation
 *
 *  a filter providing a target image
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/19 (Oyranos: 0.1.8)
 *  @date    2008/07/19
 */
oyCMMapi4_s   oyra_api4_image_output = {

  oyOBJECT_CMM_API4_S,
  0,0,0,
  0,
  
  oyraCMMInit,
  oyraCMMMessageFuncSet,
  oyraFilter_ImageRootCanHandle,

  "org.oyranos.image.image.output",

  {0,0,1},

  oyraFilter_ImageRootValidateOptions,
  oyraWidgetEvent,

  0,
  0,
  oyraFilter_ImageRootContextToMem,
  0,
  oyraFilterPlug_ImageOutputRun,

  {oyOBJECT_NAME_S, 0,0,0, "image_out", "Image[out]", "Output Image Filter Object"},
  "Image/Simple Image[out]", /* category */
  0,   /* options */
  0,   /* opts_ui_ */

  oyra_imageOutput_connectors,   /* plugs */
  1,   /* plugs_n */
  0,   /* plugs_last_add */
  0,   /* sockets */
  0,   /* sockets_n */
  0    /* sockets_last_add */
};

/** @instance oyra_api4
 *  @brief    oyra oyCMMapi4_s implementation
 *
 *  a filter providing a source image
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/02/08 (Oyranos: 0.1.8)
 *  @date    2008/06/26
 */
oyCMMapi4_s   oyra_api4_image_root = {

  oyOBJECT_CMM_API4_S,
  0,0,0,
  (oyCMMapi_s*) & oyra_api4_image_output,
  
  oyraCMMInit,
  oyraCMMMessageFuncSet,
  oyraFilter_ImageRootCanHandle,

  "org.oyranos.image.image.root",

  {0,0,1},

  oyraFilter_ImageRootValidateOptions,
  oyraWidgetEvent,

  0,
  0,
  oyraFilter_ImageRootContextToMem,
  0,
  oyraFilterPlug_ImageRootRun,

  {oyOBJECT_NAME_S, 0,0,0, "image", "Image", "Image Filter Object"},
  "Image/Simple Image[in]", /* category */
  0,   /* options */
  0,   /* opts_ui_ */

  0,   /* plugs */
  0,   /* plugs_n */
  0,   /* plugs_last_add */
  oyra_imageRoot_connectors,   /* sockets */
  1,   /* sockets_n */
  0    /* sockets_last_add */
};


