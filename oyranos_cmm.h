/** @file oyranos_cmm.h
 *
 *  Oyranos is an open source Colour Management System 
 * 
 *  Copyright (C) 2004-2008  Kai-Uwe Behrmann
 *
 *  @brief external CMM API
 *  @internal
 *  @author   Kai-Uwe Behrmann <ku.b@gmx.de>

 *  @par License:
 *  new BSD - see: <http://www.opensource.org/licenses/bsd-license.php>
 *  @since    2007/11/12
 */


#ifndef OYRANOS_CMM_H
#define OYRANOS_CMM_H

#include "oyranos.h"
#include "oyranos_alpha.h"

#ifdef __cplusplus
extern "C" {
namespace oyranos
{
#endif /* __cplusplus */

/** @brief CMM capabilities query enum
 *
 *  @since: 0.1.8
 */
typedef enum {
  oyQUERY_OYRANOS_COMPATIBILITY,       /*!< provides the Oyranos version and expects the CMM compiled or compatibility Oyranos version back */
  oyQUERY_PIXELLAYOUT_DATATYPE,        /*!< takes a oyDATATYPE_e arg as value, return boolean */
  oyQUERY_PIXELLAYOUT_CHANNELCOUNT,    /**< maximmal channel count */
  oyQUERY_PIXELLAYOUT_SWAP_COLOURCHANNELS,
  oyQUERY_PIXELLAYOUT_COLOURCHANNEL_OFFSET, /**< How many other channels can be in front of the colour channels? */
  oyQUERY_PIXELLAYOUT_PLANAR,          /**< Can the plug-in handle a separat pixel layout? value 0 - on input; value 1 on output */
  oyQUERY_PIXELLAYOUT_FLAVOUR,         /**< Can the plug-in handle min is white? */
  oyQUERY_HDR,                         /*!< are the data types real unclipped HDR? value a oyDATATYPE_e (oyHALF...) */
  oyQUERY_PROFILE_FORMAT = 20,         /*!< value 1 == ICC */
  oyQUERY_PROFILE_TAG_TYPE_READ,       /**< value a icTagTypeSignature (ICC) */
  oyQUERY_PROFILE_TAG_TYPE_WRITE,      /**< value a icTagTypeSignature (ICC) */
  oyQUERY_MAX
} oyCMMQUERY_e;

typedef int      (*oyCMMCanHandle_f) ( oyCMMQUERY_e        type,
                                       uint32_t            value );

typedef int      (*oyCMMInit_f)      ( void );

#define oyCMM_PROFILE "oyPR"
#define oyCMM_COLOUR_CONVERSION "oyCC"

/** @brief CMM pointer
 *
 *  The oyCMMptr_s is used internally and for CMM's.
 *  Memory management is done by Oyranos' oyAllocateFunc_ and oyDeallocateFunc_.
 *
 *  @version Oyranos: 0.1.8
 *  @since   2007/11/00 (Oyranos: 0.1.8)
 *  @date    2008/07/02
 */
typedef struct {
  oyOBJECT_e           type;           /*!< internal struct type oyOBJECT_CMM_POINTER_S */
  oyStruct_Copy_f      copy;           /**< copy function */
  oyStruct_Release_f   release;        /**< release function */
  oyPointer        dummy;              /**< keep to zero */
  char                 cmm[5];         /*!< the CMM */
  char                 func_name[32];  /*!< optional the CMM's function name */
  oyPointer            ptr;            /*!< a CMM's data pointer */
  char                 resource[5];    /**< the resource type, e.g. oyCMM_PROFILE, oyCMM_COLOUR_CONVERSION */
  oyStruct_release_f   ptrRelease;     /*!< CMM's deallocation function */
  int                  ref;            /**< Oyranos reference counter */
} oyCMMptr_s;

typedef int      (*oyCMMProfile_Open_f)( oyPointer         block,
                                       size_t              size,
                                       oyCMMptr_s        * oy );

typedef int      (*oyCMMColourConversion_Create_f) (
                                       oyCMMptr_s       ** cmm_profile_array,
                                       int                 profiles_n,
                                       uint32_t            pixel_layout_in,
                                       uint32_t            pixel_layout_out,
                                       int                 intent,
                                       int                 proofing_intent,
                                       uint32_t            flags,
                                       oyCMMptr_s        * oy );
typedef int      (*oyCMMColourConversion_FromMem_f) (
                                       oyPointer           mem,
                                       size_t              size,
                                       oyPixel_t           oy_pixel_layout_in,
                                       oyPixel_t           oy_pixel_layout_out,
                                       icColorSpaceSignature colour_space_in,
                                       icColorSpaceSignature colour_space_out,
                                       int                 intent,
                                       oyCMMptr_s        * oy );
typedef oyPointer(*oyCMMColourConversion_ToMem_f) (
                                       oyCMMptr_s        * oy,
                                       size_t            * size,
                                       oyAlloc_f           allocateFunc );

typedef void     (*oyCMMProgress_f)  ( int                 ID,
                                       double              progress );

typedef int      (*oyCMMColourConversion_Run_f)(
                                       oyCMMptr_s        * cmm_transform,
                                       oyPointer           in_data,
                                       oyPointer           out_data,
                                       size_t              count,
                                       oyCMMProgress_f     progress );


typedef icSignature (*oyCMMProfile_GetSignature_f) (
                                       oyCMMptr_s        * cmm_ptr,
                                       int                 pcs);


/*oyPointer          oyCMMallocateFunc ( size_t              size );
void               oyCMMdeallocateFunc(oyPointer           mem );*/

typedef int      (*oyCMMMessageFuncSet_f)( oyMessage_f     message_func );

typedef oyCMMInfo_s* (*oyCMMInfo_Get_f) (void);

typedef enum {
  oyWIDGET_OK,
  oyWIDGET_CORRUPTED,
  oyWIDGET_REDRAW,
  oyWIDGET_HIDE,
  oyWIDGET_SHOW,
  oyWIDGET_ACTIVATE,
  oyWIDGET_DEACTIVATE,
  oyWIDGET_UNDEFINED
} oyWIDGET_EVENT_e;

/** @typedef oyFilter_ValidateOptions_t
 *  @brief   a function to check and validate options
 *
 *  @param[in]     filter              the filter
 *  @param[in]     validate            to validate
 *  @param[in]     statical            convert to a statical version
 *  @param[out]    ret                 0 if nothing changed otherwise >=1
 *  @return                            corrected options or zero
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/01/02 (Oyranos: 0.1.8)
 *  @date    2008/01/02
 */
typedef oyOptions_s * (*oyFilter_ValidateOptions_f)
                                     ( oyFilter_s        * filter,
                                       oyOptions_s       * validate,
                                       int                 statical,
                                       uint32_t          * result );
typedef const char* (*oyWidgetGet_f) ( uint32_t          * result );
typedef oyWIDGET_EVENT_e   (*oyWidgetEvent_f)
                                     ( oyOptions_s       * options,
                                       oyWIDGET_EVENT_e    type,
                                       oyStruct_s        * event );



/** @brief the generic part if a API to implement and set by a CMM
 *
 *  @since Oyranos: version 0.1.8 2007/12/12
 *  @date  12 december 2007 (API 0.1.8)
 */
struct oyCMMapi_s {
  oyOBJECT_e       type;               /**< struct type oyOBJECT_CMM_API_S */
  oyPointer        dummya;             /**< keep to zero */
  oyPointer        dummyb;             /**< keep to zero */
  oyPointer        dummyc;             /**< keep to zero */
  oyCMMapi_s     * next;

  oyCMMInit_f      oyCMMInit;
  oyCMMMessageFuncSet_f oyCMMMessageFuncSet;
  oyCMMCanHandle_f oyCMMCanHandle;
};


/** @brief the API 1 to implement and set by a CMM
 *
 *  @since Oyranos: version 0.1.8 2007/12/05
 *  @date  21 december 2007 (API 0.1.8)
 */
typedef struct {
  oyOBJECT_e       type;               /**< struct type oyOBJECT_CMM_API1_S */
  oyPointer        dummya;             /**< keep to zero */
  oyPointer        dummyb;             /**< keep to zero */
  oyPointer        dummyc;             /**< keep to zero */
  oyCMMapi_s     * next;

  oyCMMInit_f      oyCMMInit;
  oyCMMMessageFuncSet_f oyCMMMessageFuncSet;
  oyCMMCanHandle_f oyCMMCanHandle;

  oyCMMProfile_Open_f oyCMMProfile_Open;
  oyCMMColourConversion_Create_f oyCMMColourConversion_Create;
  oyCMMColourConversion_FromMem_f oyCMMColourConversion_FromMem;
  oyCMMColourConversion_ToMem_f oyCMMColourConversion_ToMem;
  oyCMMColourConversion_Run_f oyCMMColourConversion_Run;
} oyCMMapi1_s;


typedef int   (*oyGetMonitorInfo_f)  ( const char        * display,
                                       char             ** manufacturer,
                                       char             ** model,
                                       char             ** serial,
                                       oyAlloc_f           allocate_func);
typedef int   (*oyGetScreenFromPosition_f) (
                                       const char        * display_name,
                                       int                 x,
                                       int                 y );
typedef char* (*oyGetDisplayNameFromPosition_f) (
                                       const char        * display_name,
                                       int                 x,
                                       int                 y,
                                       oyAlloc_f           allocate_func);
typedef char* (*oyGetMonitorProfile_f)(const char        * display,
                                       size_t            * size,
                                       oyAlloc_f           allocate_func);
typedef char* (*oyGetMonitorProfileName_f) (
                                       const char        * display,
                                       oyAlloc_f           allocate_func);
typedef int   (*oySetMonitorProfile_f)(const char        * display_name,
                                       const char        * profil_name );
typedef int   (*oyActivateMonitorProfiles_f) (
                                       const char        * display_name);


/** @struct oyCMMapi2_s
 *  @brief the API 2 to implement and set to provide windowing support
 *
 *  @since Oyranos: version 0.1.8
 *  @date  10 december 2007 (API 0.1.8)
 */
typedef struct {
  oyOBJECT_e       type;               /**< struct type oyOBJECT_CMM_API2_S */
  oyPointer        dummya;             /**< keep to zero */
  oyPointer        dummyb;             /**< keep to zero */
  oyPointer        dummyc;             /**< keep to zero */
  oyCMMapi_s     * next;

  oyCMMInit_f      oyCMMInit;
  oyCMMMessageFuncSet_f oyCMMMessageFuncSet;
  oyCMMCanHandle_f oyCMMCanHandle;

  oyGetMonitorInfo_f oyGetMonitorInfo;
  oyGetScreenFromPosition_f oyGetScreenFromPosition;

  oyGetDisplayNameFromPosition_f oyGetDisplayNameFromPosition;
  oyGetMonitorProfile_f oyGetMonitorProfile;
  oyGetMonitorProfileName_f oyGetMonitorProfileName;

  oySetMonitorProfile_f oySetMonitorProfile;
  oyActivateMonitorProfiles_f oyActivateMonitorProfiles;

} oyCMMapi2_s;


typedef oyStructList_s *    (*oyCMMProfileTag_GetValues_f) (
                                       oyProfileTag_s    * tag );
typedef int                 (*oyCMMProfileTag_Create_f) ( 
                                       oyProfileTag_s    * tag,
                                       oyStructList_s    * list,
                                       icTagTypeSignature  tag_type,
                                       uint32_t            version );

/** @struct oyCMMapi3_s
 *  @brief the API 3 to implement and set to provide low level ICC profile
 *         support
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/01/02 (Oyranos: 0.1.8)
 *  @date    2008/01/02
 */
typedef struct {
  oyOBJECT_e       type;               /**< struct type oyOBJECT_CMM_API3_S */
  oyPointer        dummya;             /**< keep to zero */
  oyPointer        dummyb;             /**< keep to zero */
  oyPointer        dummyc;             /**< keep to zero */
  oyCMMapi_s     * next;

  oyCMMInit_f      oyCMMInit;
  oyCMMMessageFuncSet_f oyCMMMessageFuncSet;
  oyCMMCanHandle_f oyCMMCanHandle;

  oyCMMProfileTag_GetValues_f oyCMMProfileTag_GetValues;
  oyCMMProfileTag_Create_f oyCMMProfileTag_Create;
} oyCMMapi3_s;


/** @type    oyCMMFilterNode_CreateContext_f
 *  @brief   create a basic filter context from root image filter node
 *
 *  @param[in,out] node                access to the complete filter node struct, most important to handle is the oyOptions_s filter->options member
 *  @param[in]     cmm_profile_array   the CMM resources cached in Oyranos, e.g. oyCMM_PROFILE
 *  @param[in]     profiles_n          number of cmm_profile_array elements
 *  @param[out]    oy                  the CMM resource to cache in Oyranos, e.g. oyCMM_COLOUR_CONVERSION
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/06/24 (Oyranos: 0.1.8)
 *  @date    2008/07/02
 */
typedef int      (*oyCMMFilter_CreateContext_f) (
                                       oyFilter_s        * filter,
                                       oyCMMptr_s       ** cmm_profile_array,
                                       int                 profiles_n,
                                       oyCMMptr_s        * oy );
/** @type    oyCMMFilter_ContextFromMem_f
 *  @brief   create a basic filter context from a memory blob
 *
 *  This function complements the oyCMMFilter_ContextToMem_t() function.
 *
 *  @param[in,out] filter              access to the complete filter struct, most important to handle is the options and image members
 *  @param[in]     mem                 the CMM memory blob
 *  @param[in]     size                size in mem
 *  @param[out]    oy                  the CMM resource to cache in Oyranos, e.g. oyCMM_COLOUR_CONVERSION
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/02 (Oyranos: 0.1.8)
 *  @date    2008/07/02
 */
typedef int      (*oyCMMFilter_ContextFromMem_f) (
                                       oyFilter_s        * filter,
                                       oyPointer           mem,
                                       size_t              size,
                                       oyCMMptr_s        * oy );

/** @type    oyCMMFilter_ContextToMem_f
 *  @brief   dump a CMM filter context into a memory blob
 *
 *  The goal is to have a data blob for later reusing. It is as well used for
 *  exchange and analysis. A oyFILTER_TYPE_COLOUR filter should fill the data
 *  blob with a device link style profile for easy forwarding and reuseable
 *  on disk caching.
 *  This function complements the oyCMMFilter_ContextFromMem_t() function.
 *
 *  @param[in,out] filter              access to the complete filter struct, most important to handle is the options and image members
 *  @param[out]    size                size in return 
 *  @param[out]    oy                  the CMM resource to cache in Oyranos, e.g. oyCMM_COLOUR_CONVERSION
 *  @param         allocateFunc        memory allocator for the returned data
 *  @return                            the CMM memory blob, preferedly ICC
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/02 (Oyranos: 0.1.8)
 *  @date    2008/07/02
 */
typedef oyPointer(*oyCMMFilter_ContextToMem_f) (
                                       oyFilter_s        * filter,
                                       size_t            * size,
                                       oyCMMptr_s        * oy,
                                       oyAlloc_f           allocateFunc );


/** @type    oyCMMFilterPlug_Run_f
 *  @brief   get a pixel or channel from the previous filter
 *
 *  You have to call oyCMMFilter_CreateContext_t or oyCMMFilter_ContextFromMem_t first.
 *  The API provides flexible pixel access and cache configuration by the
 *  passed oyPixelAccess_s object. The filters internal precalculated data
 *  are passed by the filter object.
 *
 *  @verbatim
    while (err == 0) {
      memcpy( buf[x * n++], oyCMMFilterSocket_GetNext( filter_socket, pixel_access, &err ), x );
    } @endverbatim
 *
 *  @param[in]     connector           including the CMM's private data, connector is the requesting plug to obtain a handle for calling back
 *  @param[in]     pixel_access        processing order instructions
 *  @param[in,out] output              the data to place results into, its position is in start_xy relative to the previous mediator
 *  @return                            -1 end; 0 on success; error > 1
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/07/03 (Oyranos: 0.1.8)
 *  @date    2008/07/28
 */
typedef int (*oyCMMFilterPlug_Run_f)(
                                       oyFilterPlug_s    * connector,
                                       oyPixelAccess_s   * pixel_access,
                                       oyArray2d_s      ** output );

/** @struct oyCMMapi4_s
 *  @brief the API 4 to implement and set to provide Filter support
 *
 *  The registration member provides the means to later sucessfully select 
 *  the according filter. The string is separated into sections by a point'.'.
 *  The sections are separated by comma',' as needed. The sections are to be
 *  filled as folows:
 *  - top, e.g. "org"
 *  - vendor, e.g. "oyranos"
 *  - filter type, e.g. "colour" or "tonemap" or "image" or "generic" matching the filter_type member
 *  - filter name, e.g. "scale"
 *  - features, e.g. "no_interpolation,linear_interpolation,cubic_interpolation"
 *
 *  Different filters have to implement this struct each one per filter.
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/06/24 (Oyranos: 0.1.8)
 *  @date    2008/07/10
 */
struct  oyCMMapi4_s {
  oyOBJECT_e       type;               /**< struct type oyOBJECT_CMM_API4_S */
  oyPointer        dummya;             /**< keep to zero */
  oyPointer        dummyb;             /**< keep to zero */
  oyPointer        dummyc;             /**< keep to zero */
  oyCMMapi_s     * next;

  oyCMMInit_f      oyCMMInit;
  oyCMMMessageFuncSet_f oyCMMMessageFuncSet;
  oyCMMCanHandle_f oyCMMCanHandle;

  /** e.g. "org.oyranos.generic.scale.none,linear,cubic" */
  const char     * registration;

  /** 0: major - should be stable for the live time of a filter, \n
      1: minor - mark new features, \n
      2: patch version - correct errors */
  int              version[3];

  /** check options for validy and correct */
  oyFilter_ValidateOptions_f   oyFilter_ValidateOptions;
  oyWidgetEvent_f              oyWidget_Event;     /**< handle widget events */

  /** mandatory for "..colour" filters */
  oyCMMProfile_Open_f          oyCMMProfile_Open;
  /** mandatory for "..colour" filters */
  oyCMMFilter_CreateContext_f  oyCMMFilter_CreateContext;
  /** mandatory for "..colour" filters */
  oyCMMFilter_ContextToMem_f   oyCMMFilter_ContextToMem;
  /** mandatory for "..colour" filters */
  oyCMMFilter_ContextFromMem_f oyCMMFilter_ContextFromMem;
  /** mandatory for all filters; Special care has to taken for the
      oyPixelAccess_s argument to this function. */
  oyCMMFilterPlug_Run_f        oyCMMFilterPlug_Run;

  /** translatable, eg "scale" "image scaling" "..." */
  oyName_s         name;
  const char       category[256];      /**< menu structure */
  oyOptions_s    * options;            /**< default options */
  const char     * opts_ui;            /**< xml ui elements for filter options*/

  /** We have to tell about valid input and output connectors, by 
      passively providing enough informations. */
  oyConnector_s ** plugs;
  uint32_t         plugs_n;           /**< number of different plugs */ 
  /** additional allowed number for last input connector, e.g. typical 0 */
  uint32_t         plugs_last_add;
  oyConnector_s ** sockets;
  uint32_t         sockets_n;          /**< number of sockets */
  /** additional allowed number for last output connector, e.g. typical 0 */
  uint32_t         sockets_last_add;
};


#ifdef __cplusplus
} /* extern "C" */
} /* namespace oyranos */
#endif /* __cplusplus */

#endif /* OYRANOS_CMM_H */
