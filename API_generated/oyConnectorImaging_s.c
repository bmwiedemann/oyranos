/** @file oyConnectorImaging_s.c

   [Template file inheritance graph]
   +-> oyConnectorImaging_s.template.c
   |
   +-> oyConnector_s.template.c
   |
   +-> Base_s.c
   |
   +-- oyStruct_s.template.c

 *  Oyranos is an open source Colour Management System
 *
 *  @par Copyright:
 *            2004-2012 (C) Kai-Uwe Behrmann
 *
 *  @author   Kai-Uwe Behrmann <ku.b@gmx.de>
 *  @par License:
 *            new BSD - see: http://www.opensource.org/licenses/bsd-license.php
 *  @date     2012/10/08
 */


  
#include "oyConnectorImaging_s.h"
#include "oyObject_s.h"
#include "oyranos_object_internal.h"


#include "oyConnectorImaging_s_.h"

#ifdef oyCheckCType__m
#undef oyCheckCType__m
#endif
#define oyCheckCType__m(a,b) ;
  


/** Function oyConnectorImaging_New
 *  @memberof oyConnectorImaging_s
 *  @brief   allocate a new ConnectorImaging object
 */
OYAPI oyConnectorImaging_s * OYEXPORT
  oyConnectorImaging_New( oyObject_s object )
{
  oyObject_s s = object;
  oyConnectorImaging_s_ * connectorimaging = 0;

  if(s)
    oyCheckType__m( oyOBJECT_OBJECT_S, return 0 )

  connectorimaging = oyConnectorImaging_New_( s );

  return (oyConnectorImaging_s*) connectorimaging;
}

/** Function oyConnectorImaging_Copy
 *  @memberof oyConnectorImaging_s
 *  @brief   copy or reference a ConnectorImaging object
 *
 *  The function is for copying and for referencing. The reference is the most
 *  often used way, which saves resourcs and time.
 *
 *  @param[in]     connectorimaging                 ConnectorImaging struct object
 *  @param         object              NULL - means reference,
 *                                     the optional object triffers a real copy
 */
OYAPI oyConnectorImaging_s* OYEXPORT
  oyConnectorImaging_Copy( oyConnectorImaging_s *connectorimaging, oyObject_s object )
{
  oyConnectorImaging_s_ * s = (oyConnectorImaging_s_*) connectorimaging;

  if(s)
    oyCheckCType__m( oyOBJECT_CONNECTOR_IMAGING_S, return 0 )

  s = oyConnectorImaging_Copy_( s, object );

  return (oyConnectorImaging_s*) s;
}
 
/** Function oyConnectorImaging_Release
 *  @memberof oyConnectorImaging_s
 *  @brief   release and possibly deallocate a oyConnectorImaging_s object
 *
 *  @param[in,out] connectorimaging                 ConnectorImaging struct object
 */
OYAPI int OYEXPORT
  oyConnectorImaging_Release( oyConnectorImaging_s **connectorimaging )
{
  oyConnectorImaging_s_ * s = 0;

  if(!connectorimaging || !*connectorimaging)
    return 0;

  s = (oyConnectorImaging_s_*) *connectorimaging;

  oyCheckCType__m( oyOBJECT_CONNECTOR_IMAGING_S, return 1 )

  *connectorimaging = 0;

  return oyConnectorImaging_Release_( &s );
}



/* Include "ConnectorImaging.public_methods_definitions.c" { */
/** Function  oyConnectorImaging_GetDataTypes
 *  @memberof oyConnectorImaging_s
 *  @brief    Get capabilities of oyConnectorImaging_s
 *
 *  @version  Oyranos: 0.5.0
 *  @date     2012/09/06
 *  @since    2012/09/06 (Oyranos: 0.5.0)
 */
OYAPI int  OYEXPORT
                 oyConnectorImaging_GetDataTypes (
                                       oyConnectorImaging_s  * c,
                                       const oyDATATYPE_e   ** data_types,
                                       int                   * data_types_n )
{
  oyConnectorImaging_s_ * s = (oyConnectorImaging_s_*)c;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_CONNECTOR_IMAGING_S, return 1 )

  if(data_types)
    *data_types = s->data_types;
  if(data_types_n)
    *data_types_n = s->data_types_n;

  return 0;
}
/** Function  oyConnectorImaging_GetChannelTypes
 *  @memberof oyConnectorImaging_s
 *  @brief    Get capabilities of oyConnectorImaging_s
 *
 *  @version  Oyranos: 0.5.0
 *  @date     2012/09/06
 *  @since    2012/09/06 (Oyranos: 0.5.0)
 */
OYAPI int  OYEXPORT
                 oyConnectorImaging_GetChannelTypes (
                                       oyConnectorImaging_s  * c,
                                       const oyCHANNELTYPE_e** channel_types,
                                       int                   * channel_types_n )
{
  oyConnectorImaging_s_ * s = (oyConnectorImaging_s_*)c;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_CONNECTOR_IMAGING_S, return 1 )

  if(channel_types)
    *channel_types = s->channel_types;
  if(channel_types_n)
    *channel_types_n = s->channel_types_n;

  return 0;
}

/** Function  oyConnectorImaging_GetDataTypes
 *  @memberof oyConnectorImaging_s
 *  @brief    Get capabilities of oyConnectorImaging_s
 *
 *  @version  Oyranos: 0.5.0
 *  @date     2012/09/06
 *  @since    2012/09/06 (Oyranos: 0.5.0)
 */
OYAPI int  OYEXPORT
                 oyConnectorImaging_GetCapability (
                                       oyConnectorImaging_s    * c,
                                       oyCONNECTOR_IMAGING_CAP_e type )
{
  oyConnectorImaging_s_ * s = (oyConnectorImaging_s_*)c;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_CONNECTOR_IMAGING_S, return 0 )

  switch(type)
  {
    case oyCONNECTOR_IMAGING_CAP_IS_PLUG: return s->is_plug;
    case oyCONNECTOR_IMAGING_CAP_MAX_COLOUR_OFFSET: return s->max_colour_offset;
    case oyCONNECTOR_IMAGING_CAP_MIN_CHANNELS_COUNT: return s->min_channels_count;
    case oyCONNECTOR_IMAGING_CAP_MAX_CHANNELS_COUNT: return s->max_channels_count;
    case oyCONNECTOR_IMAGING_CAP_MIN_COLOUR_COUNT: return s->min_colour_count;
    case oyCONNECTOR_IMAGING_CAP_MAX_COLOUR_COUNT: return s->max_colour_count;
    case oyCONNECTOR_IMAGING_CAP_CAN_PLANAR: return s->can_planar;
    case oyCONNECTOR_IMAGING_CAP_CAN_INTERWOVEN: return s->can_interwoven;
    case oyCONNECTOR_IMAGING_CAP_CAN_SWAP: return s->can_swap;
    case oyCONNECTOR_IMAGING_CAP_CAN_SWAP_BYTES: return s->can_swap_bytes;
    case oyCONNECTOR_IMAGING_CAP_CAN_REVERT: return s->can_revert;
    case oyCONNECTOR_IMAGING_CAP_CAN_PREMULTIPLIED_ALPHA: return s->can_premultiplied_alpha;
    case oyCONNECTOR_IMAGING_CAP_CAN_NONPREMULTIPLIED_ALPHA: return s->can_nonpremultiplied_alpha;
    case oyCONNECTOR_IMAGING_CAP_CAN_SUBPIXEL: return s->can_subpixel;
    case oyCONNECTOR_IMAGING_CAP_ID: return s->id;
    case oyCONNECTOR_IMAGING_CAP_IS_MANDATORY: return s->is_mandatory;
    default: return 0; 
  }
  return 0;
}

/* } Include "ConnectorImaging.public_methods_definitions.c" */
