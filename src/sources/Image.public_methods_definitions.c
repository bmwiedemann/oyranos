#include "oyArray2d_s_.h"
#include "oyRectangle_s_.h"
#include "oyFilterNode_s.h"
#include "oyConversion_s.h"
#include "oyranos_image_internal.h"

/**
 *  Function oyImage_GetArray2dPointContinous
 *  @memberof oyImage_s
 *  @brief   Continous layout pixel accessor
 *
 *  Will be used by default.
 *
 *  @see oyImage_GetPoint_f()
 *
 *  @version Oyranos: 0.9.5
 *  @date    2013/07/21
 *  @since   2008/06/26 (Oyranos: 0.1.8)
 */
oyPointer oyImage_GetArray2dPointContinous (
                                         oyImage_s       * image,
                                         int               point_x,
                                         int               point_y,
                                         int               channel,
                                         int             * is_allocated )
{
  oyImage_s_ * image_ = oyImagePriv_m(image);
  oyArray2d_s_ * a = (oyArray2d_s_*) image_->pixel_data;
  unsigned char ** array2d = a->array2d;
  int pos;
  if(channel < 0)
    channel = 0;
  pos = (point_x * image_->layout_[oyCHANS]
             + image_->layout_[oyCHAN0+channel])
            * image_->layout_[oyDATA_SIZE];
  if(is_allocated) *is_allocated = 0;
  return &array2d[ point_y ][ pos ]; 
}

/** 
 *  Function oyImage_GetArray2dLineContinous
 *  @memberof oyImage_s
 *  @brief   Continous layout line accessor
 *
 *  Will be used by default.
 *
 *  @version Oyranos: 0.1.8
 *  @date    2008/08/21
 *  @since   2008/08/23 (Oyranos: 0.1.8)
 */
oyPointer oyImage_GetArray2dLineContinous (
                                         oyImage_s       * image,
                                         int               point_y,
                                         int             * height,
                                         int               channel,
                                         int             * is_allocated )
{
  oyArray2d_s_ * a = (oyArray2d_s_*) oyImagePriv_m(image)->pixel_data;
  unsigned char ** array2d = a->array2d;
  if(height) *height = 1;
  if(is_allocated) *is_allocated = 0;
  if(oy_debug && point_y >= a->height)
    WARNcc2_S(image, "point_y < a->height failed(%d/%d)", point_y, a->height)
  return &array2d[ point_y ][ 0 ]; 
}

/**  
 *  Function oyImage_SetArray2dPointContinous
 *  @memberof oyImage_s
 *  @brief   Continous layout pixel accessor
 *
 *  Will be used by default.
 *
 *  @see oyImage_SetPoint_f()
 *
 *  @version Oyranos: 0.9.5
 *  @date    2013/07/21
 *  @since   2009/02/22 (Oyranos: 0.1.10)
 */
int       oyImage_SetArray2dPointContinous (
                                         oyImage_s       * image,
                                         int               point_x,
                                         int               point_y,
                                         int               channel,
                                         oyPointer         data )
{
  oyImage_s_ * image_ = oyImagePriv_m(image);
  oyArray2d_s_ * a = (oyArray2d_s_*) image_->pixel_data;
  unsigned char ** array2d = a->array2d;
  int pos;
  oyDATATYPE_e data_type = oyToDataType_m( image_->layout_[oyLAYOUT] );
  int byteps = oyDataTypeGetSize( data_type );
  int channels = 1;

  if(channel < 0)
    channels = oyToChannels_m( image_->layout_[oyLAYOUT] );

  if(channel < 0)
    channel = 0;
  pos = (point_x * image_->layout_[oyCHANS]
             + image_->layout_[oyCHAN0+channel])
            * image_->layout_[oyDATA_SIZE];

  memcpy( &array2d[ point_y ][ pos ], data, byteps * channels );

  return 0;

}

/**  
 *  @internal
 *  Function oyImage_SetArray2dLineContinous
 *  @memberof oyImage_s
 *  @brief   Continous layout line accessor
 *
 *  Will be used by default.
 *
 *  @version Oyranos: 0.1.10
 *  @date    2009/02/22
 *  @since   2009/02/22 (Oyranos: 0.1.10)
 */
int       oyImage_SetArray2dLineContinous (
                                         oyImage_s       * image,
                                         int               point_x,
                                         int               point_y,
                                         int               pixel_n,
                                         int               channel,
                                         oyPointer         data )
{
  oyImage_s_ * image_ = oyImagePriv_m(image);
  oyArray2d_s_ * a = (oyArray2d_s_*) image_->pixel_data;
  unsigned char ** array2d = a->array2d;
  oyDATATYPE_e data_type = oyToDataType_m( image_->layout_[oyLAYOUT] );
  int byteps = oyDataTypeGetSize( data_type );
  int channels = 1;
  int offset = point_x;

  if(pixel_n < 0)
    pixel_n = image_->width - point_x;

  if(channel < 0)
  {
    channels = oyToChannels_m( image_->layout_[oyLAYOUT] );
    offset *= channels;
  }

  if(&array2d[ point_y ][ offset ] != data)
    memcpy( &array2d[ point_y ][ offset ], data, pixel_n * byteps * channels );

  return 0; 
}

/** 
 *  @internal
 *  Function  oyImage_GetArray2dPointPlanar
 *  @memberof oyImage_s
 *  @brief    Planar layout pixel accessor
 *
 *  Will be used by default.
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/06/26 (Oyranos: 0.1.8)
 *  @date    2008/08/24
 */
oyPointer oyImage_GetArray2dPointPlanar( oyImage_s       * image,
                                         int               point_x,
                                         int               point_y,
                                         int               channel,
                                         int             * is_allocated )
{
  WARNcc_S(image, "planar pixel access not implemented")
  return 0;
#if 0
  oyArray2d_s * a = (oyArray2d_s*) image->pixel_data;
  unsigned char ** array2d = a->array2d;
  if(is_allocated) *is_allocated = 0;
  return &array2d[ point_y ][ (point_x + image->layout_[oyCOFF]
                               * image->layout_[oyCHAN0+channel])
                              * image->layout_[oyDATA_SIZE]       ]; 
#endif
}

/** 
 *  @internal
 *  Function oyImage_GetArray2dLinePlanar
 *  @memberof oyImage_s
 *  @brief    Continus layout line accessor
 *
 *  We assume a channel after channel behaviour without line interweaving.
 *  Will be used by default.
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/08/23 (Oyranos: 0.1.8)
 *  @date    2008/08/24
 */
oyPointer oyImage_GetArray2dLinePlanar ( oyImage_s       * image,
                                         int               point_y,
                                         int             * height,
                                         int               channel,
                                         int             * is_allocated )
{
  if(height) *height = 1;
  WARNcc_S(image, "planar pixel access not implemented")
  return 0;
#if 0 /* SunC: warning: statement not reached */
  oyArray2d_s * a = (oyArray2d_s*) image->pixel_data;
  unsigned char ** array2d = a->array2d;
  if(is_allocated) *is_allocated = 0;
  /* it makes no sense to use more than one line */                   
  return &array2d[ 0 ][   image->width
                        * image->layout_[oyCOFF]
                        * image->layout_[oyCHAN0+channel]
                        * image->layout_[oyDATA_SIZE] ];
#endif
}


/** @brief   collect infos about a image
 *  @memberof oyImage_s
 *
 *  Create a image description and access object. The passed pixels pointer
 *  remains in the responsibility of the user. The image is a in memory blob.
 *
    @param[in]    width        image width
    @param[in]    height       image height
    @param[in]    pixels       pointer to the data buffer
    @param[in]    pixel_layout i.e. oyTYPE_123_16 for 16-bit RGB data
    @param[in]    profile      color space description
    @param[in]    object       the optional base
 *
 *  @version Oyranos: 0.1.8
 *  @since   2007/11/00 (Oyranos: 0.1.8)
 *  @date    2008/08/23
 */
oyImage_s *    oyImage_Create         ( int               width,
                                        int               height, 
                                        oyPointer         pixels,
                                        oyPixel_t         pixel_layout,
                                        oyProfile_s     * profile,
                                        oyObject_s        object)
{
  oyRectangle_s * display_rectangle = 0;
  /* ---- start of common object constructor ----- */
  oyOBJECT_e type = oyOBJECT_IMAGE_S;
# define STRUCT_TYPE oyImage_s_
  int error = 0;
  oyObject_s    s_obj = oyObject_NewFrom( object );
  STRUCT_TYPE * s = 0;
  
  if(s_obj)
    s = (STRUCT_TYPE*)s_obj->allocateFunc_(sizeof(STRUCT_TYPE));

  if(!s || !s_obj)
  {
    WARNc_S(_("MEM Error."));
    return NULL;
  }

  error = !memset( s, 0, sizeof(STRUCT_TYPE) );

  memcpy( s, &type, sizeof(oyOBJECT_e) );
  s->copy = (oyStruct_Copy_f) oyImage_Copy_x;
  s->release = (oyStruct_Release_f) oyImage_Release;

  s->oy_ = s_obj;

  error = !oyObject_SetParent( s_obj, type, (oyPointer)s );
# undef STRUCT_TYPE
  /* ---- end of common object constructor ------- */

  if(!profile)
  {
    WARNc_S("no profile obtained");

    oyImage_Release( (oyImage_s**)&s );
    return (oyImage_s*) s;
  }

  s->width = width;
  s->height = height;
  {
    int channels_n = oyToChannels_m(pixel_layout);
    oyArray2d_s * a = oyArray2d_Create( pixels,
                                        s->width * channels_n,
                                        s->height,
                                        oyToDataType_m(pixel_layout),
                                        s_obj );
    oyImage_SetData ( (oyImage_s*)s, (oyStruct_s**) &a, 0,0,0,0,0,0 );
  }
  s->profile_ = oyProfile_Copy( profile, 0 );
  if(s->width != 0.0)
    s->viewport = oyRectangle_NewWith( 0, 0, 1.0,
                                   (double)s->height/(double)s->width, s->oy_ );

  error = oyImage_CombinePixelLayout2Mask_ ( s, pixel_layout );

  if(s->pixel_data && s->layout_[oyCOFF] == 1)
    oyImage_SetData( (oyImage_s*)s, 0, oyImage_GetArray2dPointContinous,
                           oyImage_GetArray2dLineContinous, 0,
                           oyImage_SetArray2dPointContinous,
                           oyImage_SetArray2dLineContinous, 0 );
  else if(s->pixel_data)
    oyImage_SetData( (oyImage_s*)s, 0, oyImage_GetArray2dPointPlanar,
                           oyImage_GetArray2dLinePlanar, 0, 0,0,0 );

  if(error <= 0)
  {
    display_rectangle = oyRectangle_New( 0 );

    error = !display_rectangle;
    if(error <= 0)
      oyOptions_MoveInStruct( &s->tags,
                              "//imaging/output/display_rectangle",
                              (oyStruct_s**)&display_rectangle, OY_CREATE_NEW );
  }

  if(oy_debug_objects)
    oyObjectDebugMessage_( s?s->oy_:NULL, __func__, oyStructTypeToText(s->type_) );

  return (oyImage_s*) s;
}

/** @brief   collect infos about a image for showing one a display
 *  @memberof oyImage_s

    @param[in]     width               image width
    @param[in]     height              image height
    @param[in]     channels            pointer to the data buffer
    @param[in]     pixel_layout        i.e. oyTYPE_123_16 for 16-bit RGB data
    @param[in]     display_name        display name
    @param[in]     display_pos_x       left image position on display
    @param[in]     display_pos_y       top image position on display
    @param[in]     display_width       width to show in window
    @param[in]     display_height      height to show in window
    @param[in]     icc_profile_flags   profile selection flags, see oyProfile_FromFile()
    @param[in]     object              the optional base
 *
 *  @version  Oyranos: 0.9.6
 *  @date     2014/04/06
 *  @since    2007/10/00 (Oyranos: 0.1.8)
 */
oyImage_s *    oyImage_CreateForDisplay ( int              width,
                                       int                 height, 
                                       oyPointer           channels,
                                       oyPixel_t           pixel_layout,
                                       const char        * display_name,
                                       int                 display_pos_x,
                                       int                 display_pos_y,
                                       int                 display_width,
                                       int                 display_height,
                                       int                 icc_profile_flags,
                                       oyObject_s          object)
{
  oyProfile_s * p = oyProfile_FromStd(oyEDITING_XYZ, icc_profile_flags, 0);
  oyImage_s_ * s = (oyImage_s_*)oyImage_Create( width, height,
                                  channels, pixel_layout,
                                  p, object );
  int error = !s;
  oyRectangle_s * display_rectangle = 0;

  oyProfile_Release( &p );

  if(error <= 0)
  {
    if(!s->profile_)
      error = 1;

    if(error <= 0)
      error = oyImage_CombinePixelLayout2Mask_ ( s, pixel_layout );

    if(error <= 0)
    {
      display_rectangle = (oyRectangle_s*) oyOptions_GetType( s->tags, -1,
                                    "display_rectangle", oyOBJECT_RECTANGLE_S );
      oyRectangle_SetGeo( display_rectangle, display_pos_x, display_pos_y,
                                             display_width, display_height );
    }
    error = !display_rectangle;
    
    if(error <= 0 && display_name)
      error = oyOptions_SetFromText( &s->tags, "//imaging/output/display_name",
                                     display_name, OY_CREATE_NEW );

    if(error <= 0 && icc_profile_flags)
      oyOptions_SetFromInt( &s->tags, "///icc_profile_flags", icc_profile_flags, 0, OY_CREATE_NEW );

    if(error > 0)
    {
      oyImage_Release( (oyImage_s**)&s );
      WARNc1_S("Could not create image %d", oyObject_GetId( object ));
    }
  }

  return (oyImage_s*) s;
}


/** Function  oyImage_SetCritical
 *  @memberof oyImage_s
 *  @brief    Set a image
 *
 *  Set critical options and check consitency.
 *
 *  @param[in,out] image               the image
 *  @param[in]     pixel_layout        the new layout or pass 0
 *  @param[in]     profile             assign a new profile reinterprets color,
 *                                     optional
 *  @param[in]     tags                new tags, replace any old tags, optional
 *  @param[in]     width               new width or pass -1
 *  @param[in]     height              new height or pass -1
 *  @return                            error
 *
 *  @version  Oyranos: 0.5.0
 *  @date     2012/09/25
 *  @since    2007/12/19 (Oyranos: 0.1.8)
 */
int            oyImage_SetCritical   ( oyImage_s         * image,
                                       oyPixel_t           pixel_layout,
                                       oyProfile_s       * profile,
                                       oyOptions_s       * tags,
                                       int                 width,
                                       int                 height )
{
  oyImage_s_ * s = (oyImage_s_*)image;
  int error = !s;
  int need_new_array = 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 1 )

  if(oy_debug)
  {
    char * pl = oyPixelPrint( pixel_layout, oyAllocateFunc_ );
    const char * pt = oyProfile_GetText(profile,oyNAME_NAME),
               * tt = oyOptions_GetText( tags, oyNAME_NAME );
    DBGs6_S( image, "pixel_layout: %d=(\"%s\") profile = %s tags = %s\t%dx%d",
                 pixel_layout, pixel_layout?pl:oyNoEmptyString_m_( NULL ),
                 oyNoEmptyString_m_( pt ), oyNoEmptyString_m_( tt ),
                 width, height );
    oyFree_m_(pl);
  }

  if(profile)
  {
    oyProfile_Release( &s->profile_ );
    s->profile_ = oyProfile_Copy( profile, 0 );
  }

  if(tags)
  {
    oyOptions_Release( &s->tags );
    s->tags = oyOptions_Copy( tags, s->oy_ );
  }

  if(width >= 0)
    s->width = width;
  if(height >= 0)
    s->height = height;

  if(pixel_layout)
    error = oyImage_CombinePixelLayout2Mask_ ( s, pixel_layout );
  else
    /* update to new ID for possible new context hashing */
    error = oyImage_CombinePixelLayout2Mask_ ( s, s->layout_[oyLAYOUT] );

  /* Not shure whether it is a good idea to have automatic image data
     allocation here. Anyway this is intented as a fallback for empty images, 
     like a unspecified output image to be catched here. */
  if((!s->setLine || !s->getLine) &&
     (!s->setPoint || !s->getPoint) &&
     s->width && s->height)
  {
    need_new_array = 1;
    oyImage_SetData( (oyImage_s*)s,     NULL,
                           oyImage_GetArray2dPointContinous,
                           oyImage_GetArray2dLineContinous, 0,
                           oyImage_SetArray2dPointContinous,
                           oyImage_SetArray2dLineContinous, 0 );
  }

  if(width >= 0 || height >= 0 || need_new_array)
  {
    oyPixel_t pixel_layout = s->layout_[oyLAYOUT];
    oyPointer channels = 0;

    oyArray2d_s * a = oyArray2d_Create( channels,
                                        s->width * oyToChannels_m(pixel_layout),
                                        s->height,
                                        oyToDataType_m(pixel_layout),
                                        s->oy_ );

    oyImage_SetData( (oyImage_s*)s,    (oyStruct_s**) &a, 0,0,0, 0,0,0 );
  }

  return error;
}

/** Function oyImage_FillArray
 *  @memberof oyImage_s
 *  @brief   creata a array from a image and fill with data
 *
 *  The rectangle will be considered relative to the data.
 *  A given array will be filled. Allocation of a new array2d object happens as
 *  needed.
 *
 *  @param[in]     image               the image
 *  @param[in]     rectangle           the image rectangle in a relative unit
 *                                     a rectangle in the source image
 *  @param[in]     allocate_method
 *                                     - 0 assign the rows without copy
 *                                     - 1 do copy into the array
 *                                     - 2 allocate empty rows
 *  @param[out]    array               array to fill; If array is empty, it is
 *                                     allocated as per allocate_method
 *  @param[in]     array_rectangle     the array rectangle in samples
 *                                     For NULL the image rectangle will be
 *                                     placed to the top left corner in array.
 *                                     If array_rectangle is provided, image
 *                                     samples will be placed in array at 
 *                                     array_rectangle's position and size.
 *                                     The unit is relative to the image.
 *  @param[in]     obj                 the optional user object
 *
 *  @version Oyranos: 0.9.0
 *  @date    2012/10/20
 *  @since   2008/10/02 (Oyranos: 0.1.8)
 */
int            oyImage_FillArray     ( oyImage_s         * image,
                                       oyRectangle_s     * rectangle,
                                       int                 allocate_method,
                                       oyArray2d_s      ** array,
                                       oyRectangle_s     * array_rectangle,
                                       oyObject_s          obj )
{
  int error;
  oyArray2d_s_ * a = (oyArray2d_s_*) *array;
  oyImage_s_ * s = (oyImage_s_*)image;
  oyRectangle_s_ image_roi_chan = {oyOBJECT_RECTANGLE_S,0,0,0, 0,0,0,0},
                r = {oyOBJECT_RECTANGLE_S,0,0,0, 0,0,0,0};
  oyDATATYPE_e data_type = oyUINT8;
  int is_allocated = 0;
  int data_size, ay;
  oyRectangle_s_ array_roi_pix = {oyOBJECT_RECTANGLE_S,0,0,0, 0,0,0,0};
  int array_width, array_height;
  oyAlloc_f allocateFunc_ = 0;
  unsigned char * line_data = 0;
  int i,j, height, channels_n;
  size_t wlen;

  if(!image)
    return 1;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 1 )

  if( allocate_method < 0 || allocate_method > 2 )
  {
    WARNcc1_S(image, "allocate_method not allowed: %d", allocate_method )
    error = 1;
  }

  data_type = oyToDataType_m( s->layout_[oyLAYOUT] );
  data_size = oyDataTypeGetSize( data_type );
  error = oyImage_PixelsToSamples( image, rectangle,
                                   (oyRectangle_s*)&image_roi_chan );

  if(!error && array_rectangle)
    error = oyImage_PixelsToSamples( image, array_rectangle,
                                     (oyRectangle_s*)&array_roi_pix );
  else
  {
    oyRectangle_SetGeo( (oyRectangle_s*)&r, 0,0,
                                 oyRectanglePriv_m(rectangle)->width,
                                 oyRectanglePriv_m(rectangle)->height );
    error = oyImage_PixelsToSamples( image, (oyRectangle_s*)&r,
                                     (oyRectangle_s*)&array_roi_pix );
  }

  array_width = array_roi_pix.x + array_roi_pix.width;
  array_height = array_roi_pix.y + array_roi_pix.height;

  if(oy_debug > 2)
  {
    char * t = NULL;
    STRING_ADD( t, oyRectangle_Show( (oyRectangle_s*)&array_roi_pix ) );
    DBGs_PROG5_S( image, "image_roi_chan: %s array_roi_pix: %s array[%d] array_width: %d array_height: %d",
                 oyRectangle_Show( (oyRectangle_s*)&image_roi_chan ),
                 t, oyStruct_GetId((oyStruct_s*)*array), array_width, array_height );
    oyFree_m_(t);
  }

  if(!error &&
     (!a ||
      (a && ( array_width > oyRectanglePriv_m(&a->data_area)->width ||
              array_height > oyRectanglePriv_m(&a->data_area)->height ))) &&
     array_width > 0 && array_height > 0
    )
  {
    if(!(array_roi_pix.width && array_roi_pix.height))
      /* array creation is not possible */
      error = -1;

    if(!error)
    {
      if(!a)
      {
        a = oyArray2d_Create_( array_width, array_height, data_type, obj );
      
      } else
      {
        oyMessageFunc_p( oyMSG_WARN, (oyStruct_s*)image,
                 OY_DBG_FORMAT_ "array[%d](%dx%d) is too small: setting to %dx%d",
                 OY_DBG_ARGS_,
                 oyStruct_GetId( (oyStruct_s*) a ),
                 a->width, a->height, array_width, array_height );
        error = oyArray2d_Reset( *array, array_width, array_height, data_type );
      }
    }

    if(!error)
    {
      if(a->oy_)
        allocateFunc_ = a->oy_->allocateFunc_;

      error = !a;
      if(!error)
      {
        /* allocate each single line */
        if(allocate_method == 1 || allocate_method == 2)
        {
          a->own_lines = 2;

          for(ay = 0; ay < array_height; ++ay)
            if(!a->array2d[ay])
              oyAllocHelper_m_( a->array2d[ay], 
                              unsigned char,
                              array_width * data_size,
                              allocateFunc_,
                              error = 1; break );
        } else if(allocate_method == 0)
        {
          for( i = 0; i < array_height; )
          {
            if(!a->array2d[i])
            {
              height = is_allocated = 0;
              line_data = s->getLine( image, i, &height, -1,
                             &is_allocated );
              for( j = 0; j < height; ++j )
              {
                if( i + j >= array_height )
                  break;

                ay = i + j;

                a->array2d[ay] = 
                    &line_data[j*data_size * array_width];
              }
            }

            i += height;

            if(error) break;
          }
        }
      }
    }
  }

  /* a array should have been created */
  if( !a && array_roi_pix.width && array_roi_pix.height )
  {
    WARNcc_S(image, "Could not create array.")
    if(error <= 0) error = -1;
  }

  if( !error && a )
  {
    /* shift array focus to requested region */
    oyArray2d_SetFocus( (oyArray2d_s*)a, (oyRectangle_s*)&array_roi_pix );

    /* change intermediately */
    if(a && a->width > image_roi_chan.width)
    {
      a->width = image_roi_chan.width;
    }
    if(a && a->height > image_roi_chan.height)
    {
      a->height = image_roi_chan.height;
    }
  }

  channels_n = oyToChannels_m( s->layout_[oyLAYOUT] );
  if(a && !error)
  {
  if(s->getLine)
  {
    oyPointer src, dst;
    int image_roi_chan_width = image_roi_chan.x + image_roi_chan.width,
        roi_pix_width = OY_MIN( image_roi_chan_width, array_roi_pix.width ) / channels_n,
        roi_pix_height = OY_MIN( image_roi_chan.y + image_roi_chan.height, array_roi_pix.height );

    wlen = roi_pix_width * data_size * channels_n;

    if(oy_debug > 2)
    {
      char * t = NULL;
      STRING_ADD( t, oyRectangle_Show( (oyRectangle_s*)&array_roi_pix ) );
      DBGs_PROG4_S( image, "image_roi_chan: %s array_roi_pix: %s wlen(size_t): %lu image-hook:\"%s\"",
                 oyRectangle_Show( (oyRectangle_s*)&image_roi_chan ),
                 t, wlen, oyStructTypeToText( s->pixel_data->type_ ) );
      oyFree_m_(t);
    }

    if(roi_pix_width  > s->width ||
       roi_pix_height > s->height)
    {
      oyMessageFunc_p( oyMSG_ERROR, (oyStruct_s*)image,
                 OY_DBG_FORMAT_ "image request is exceeding available memory:%dx%d img_roi: %.0fx%.0f+%.0f+%.0f rectangle: %.gx%.g+%.g+%.g image:\n%s\narray[%d](%dx%d)",
                 OY_DBG_ARGS_, s->width,s->height,
                 image_roi_chan.width/channels_n,image_roi_chan.height,image_roi_chan.x/channels_n,image_roi_chan.y,
                 oyRectangle_GetGeo1(rectangle, 2),oyRectangle_GetGeo1(rectangle, 3),oyRectangle_GetGeo1(rectangle, 0),oyRectangle_GetGeo1(rectangle, 1),
                 oyObject_GetName( image->oy_, oyNAME_NICK ),
                 oyStruct_GetId( (oyStruct_s*) a ),
                 a->width, a->height );
      error = 1;
    }

    if(allocate_method != 2 && !error)
    for( i = 0; i < roi_pix_height; )
    {
      height = is_allocated = 0;
      line_data = s->getLine( image, image_roi_chan.y + i, &height, -1,
                             &is_allocated );

      if(!line_data)
        error = 1;
      else
      for( j = 0; j < height; ++j )
      {
        if( i + j >= array_roi_pix.height )
          break;

        ay = i + j;

        dst = &a->array2d[ay][0];
        src = &line_data[(j
                       * OY_ROUND(s->width * s->layout_[oyCHANS])
                       + OY_ROUND(image_roi_chan.x))
                      * data_size];

        if(dst && src && dst != src)
          error = !memcpy( dst, src, wlen );
      }

      i += height;

      if(error) break;
    }

  } else
  if(s->getPoint)
  {
    WARNcc_S(image, "image->getPoint  not yet supported")
  } else
  if(s->getTile)
  {
    WARNcc_S(image, "image->getTile  not yet supported")
    error = 1;
  }
  }

  DBGs_PROG4_S( image, "array[%d](%dx%d) error: %d",
                oyStruct_GetId( (oyStruct_s*) a), a?a->width:-1, a?a->height:-1, error );

  if(error)
    oyArray2d_Release( (oyArray2d_s**)&a );

  *array = (oyArray2d_s*)a;

  return error;
}

/** Function oyImage_ReadArray
 *  @memberof oyImage_s
 *  @brief   read a array into a image
 *
 *  The rectangle will be considered relative to the image.
 *  The given array should match that rectangle.
 *
 *  @version Oyranos: 0.1.10
 *  @since   2009/02/28 (Oyranos: 0.1.10)
 *  @date    2009/02/28
 */
int            oyImage_ReadArray     ( oyImage_s         * image,
                                       oyRectangle_s     * image_rectangle,
                                       oyArray2d_s       * array,
                                       oyRectangle_s     * array_rectangle )
{
  oyImage_s_ * s = (oyImage_s_*)image;
  oyArray2d_s_ * array_ = (oyArray2d_s_*)array;
  int error = !image || !array;
  oyRectangle_s_ image_roi_chan = {oyOBJECT_RECTANGLE_S,0,0,0,0,0,0,0},
                 array_rect_pix = {oyOBJECT_RECTANGLE_S,0,0,0,0,0,0,0};
  oyDATATYPE_e data_type = oyUINT8;
  int bps = 0, channel_n, i, offset, width, height;

  if(error)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 1 )

  data_type = oyToDataType_m( s->layout_[oyLAYOUT] );
  bps = oyDataTypeGetSize( data_type );
  channel_n = s->layout_[oyCHANS];

  error = oyImage_PixelsToSamples( image, image_rectangle,
                                   (oyRectangle_s*)&image_roi_chan );
  /* We want to check if the array is big enough to hold the pixels */
  if(array_->data_area.width < image_roi_chan.width ||
     array_->data_area.height < image_roi_chan.height)
  {
    WARNcc4_S( image, "array[%d](%dx%d) is too small for image rectangle %s",
               oyStruct_GetId( (oyStruct_s*)array_ ),
               (int)array_->width, (int)array_->height,
               oyRectangle_Show( (oyRectangle_s*)&image_roi_chan ) );
    error = 1;
  }

  if(!error & !s->setLine)
  {
    WARNcc_S( image, "setLine() missed! (Only this interface is yet supported.)" );
    error = 1;
  }

  if(!error)
  {
    if(array_rectangle)
    {
      oyRectangle_SetByRectangle( (oyRectangle_s*)&array_rect_pix, array_rectangle );
      oyRectangle_Scale( (oyRectangle_s*)&array_rect_pix, s->width );
      array_rect_pix.x *= channel_n;
      array_rect_pix.width *= channel_n;
    } else
    {
      oyRectangle_SetGeo( (oyRectangle_s*)&array_rect_pix, 
                          0,0, array_->width, array_->height );
    }
  }

  if(!error &&
     (array_rect_pix.width != image_roi_chan.width ||
      array_rect_pix.height != image_roi_chan.height))
  {
    WARNcc6_S( image, "array[%d] %gx%g does not fit image[%d] rectangle %gx%g",
               oyStruct_GetId( (oyStruct_s*)array_ ),
               array_rect_pix.width, array_rect_pix.height,
               oyStruct_GetId( (oyStruct_s*)image ),
               image_roi_chan.width, image_roi_chan.height
                );
  }

  if(!error)
  {
    offset = image_roi_chan.x / channel_n * bps;
    width = OY_MIN(image_roi_chan.width, array_rect_pix.width);
    width /= channel_n;
    height = OY_MIN( array_rect_pix.y + array_rect_pix.height, image_roi_chan.height );

    for(i = array_rect_pix.y; i < height; ++i)
    {
      s->setLine( image, offset, image_roi_chan.y + i, width, -1,
                      &array_->array2d
                              [i][OY_ROUND(array_rect_pix.x) * bps] );
    }
  }

  return error;
}

/**
 *  @func    oyImage_WritePPM
 *  @memberof oyImage_s
 *  @brief   write buffer to portable pix map format
 *
 *  @param[in]     image               the image
 *  @param[in]     file_name           a writeable file name, The file can 
 *                                     contain "%d" to include the image ID.
 *  @param[in]     free_text           A text to include as comment.
 *
 *  @version Oyranos: 0.3.1
 *  @date    2011/05/12
 *  @since   2008/10/07 (Oyranos: 0.1.8)
 */
int          oyImage_WritePPM        ( oyImage_s         * image,
                                       const char        * file_name,
                                       const char        * free_text )
{
  int error = !file_name;
  FILE * fp = 0;
  char * filename = 0;
  oyImage_s_ * s = (oyImage_s_*)image;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 1 )

  if(!error)
    oyAllocHelper_m_( filename, char, strlen(file_name)+80, 0, return 1 );

  if(!error)
  {
    if(strstr(file_name, "%d"))
      sprintf( filename, file_name, oyStruct_GetId( (oyStruct_s*)s ) );
    else
      strcpy(filename,file_name);
  }

  if(filename)
    fp = fopen( filename, "wb" );
  else
    error = 2;

  if(fp)
  {
      size_t pt = 0;
      char text[128];
      char * t = 0;
      int  len = 0;
      int  i,j,k,l, n;
      char bytes[48];

      int cchan_n = oyProfile_GetChannelsCount( s->profile_ );
      int channels = oyToChannels_m( s->layout_[oyLAYOUT] );
      oyDATATYPE_e data_type = oyToDataType_m( s->layout_[oyLAYOUT] );
      int alpha = channels - cchan_n;
      int byteps = oyDataTypeGetSize( data_type );
      const char * colorspacename = oyProfile_GetText( s->profile_,
                                                        oyNAME_DESCRIPTION );
      char * vs = oyVersionString(1,malloc);
      uint8_t * out_values = 0;
      const uint8_t * u8 = NULL;
      double * dbls;
      float flt;

            fputc( 'P', fp );
      if(alpha ||
         cchan_n > 3) 
            fputc( '7', fp );
      else
      {
        if(byteps == 2 &&
           data_type == oyHALF)
        {
          if(channels == 1)
            fputc( 'h', fp ); /* PFM gray */
          else
            fputc( 'H', fp ); /* PFM rgb */
        } else
        if(byteps == 1 ||
           byteps == 2)
        {
          if(channels == 1)
            fputc( '5', fp );
          else
            fputc( '6', fp );
        } else
        if (byteps == 4 || byteps == 8)
        {
          if(channels == 1)
            fputc( 'f', fp ); /* PFM gray */
          else
            fputc( 'F', fp ); /* PFM rgb */
        }
      }

      fputc( '\n', fp );

      oyStringAddPrintf_( &t, oyAllocateFunc_, oyDeAllocateFunc_,
                "# CREATOR: Oyranos-%s\n",
                oyNoEmptyString_m_(vs) ); 
      oyStringAddPrintf_( &t, oyAllocateFunc_, oyDeAllocateFunc_,
                " COMMENT: %s\n",
                free_text?free_text:"" );
      oyStringAddPrintf_( &t, oyAllocateFunc_, oyDeAllocateFunc_,
                " oyImage_s: %d\n",
                oyObject_GetId( image->oy_ ) );
      if(vs) free(vs); vs = 0;
      len = strlen( t );
      do { fputc ( t[pt] , fp); if(t[pt] == '\n') fputc( '#', fp ); pt++; } while (--len); pt = 0;
      fputc( '\n', fp );
      oyFree_m_( t );

      {
        time_t  cutime;         /* Time since epoch */
        struct tm       *gmt;
        char time_str[24];

        cutime = time(NULL); /* time right NOW */
        gmt = gmtime(&cutime);
        strftime(time_str, 24, "%Y/%m/%d %H:%M:%S", gmt);
        snprintf( text, 128, "# DATE/TIME: %s\n", time_str );
        len = strlen( text );
        do { fputc ( text[pt++] , fp); } while (--len); pt = 0;
      }

      snprintf( text, 128, "# COLORSPACE: %s\n", colorspacename ?
                colorspacename : "--" );
      len = strlen( text );
      do { fputc ( text[pt++] , fp); } while (--len); pt = 0;

      if (byteps == 4 || byteps == 8 ||
          (byteps == 2 && data_type == oyHALF))
      {
        if(oyBigEndian())
          snprintf( bytes, 48, "1.0" );
        else
          snprintf( bytes, 48, "-1.0" );
      }
      else
      if(byteps == 1)
        snprintf( bytes, 48, "255" );
      else
      if(byteps == 2)
        snprintf( bytes, 48, "65535" );
      else
        oyMessageFunc_p( oyMSG_WARN, (oyStruct_s*)image,
             OY_DBG_FORMAT_ " byteps: %d",
             OY_DBG_ARGS_, byteps );


      if(alpha ||
         cchan_n > 3)
      {
        const char *tupl = "RGB_ALPHA";

        if(channels == 1 && alpha)
          tupl = "GRAYSCALE_ALPHA";
        else if(channels == 1)
          tupl = "GRAYSCALE_ALPHA";
        else if(channels == 3 && !alpha)
          tupl = "RGB";
        else if(channels == 4)
          tupl = "CMYK";
        else if(channels >= 5)
          tupl = "DEVN";

        snprintf( text, 128, "WIDTH %d\nHEIGHT %d\nDEPTH %d\nMAXVAL "
                  "%s\nTUPLTYPE %s\nENDHDR\n",
                  s->width, s->height,
                  channels, bytes, tupl );
        len = strlen( text );
        do { fputc ( text[pt++] , fp); } while (--len); pt = 0;

      }
      else
      {
        snprintf( text, 128, "%d %d\n", s->width,
                                       s->height);
        len = strlen( text );
        do { fputc ( text[pt++] , fp); } while (--len); pt = 0;

        snprintf( text, 128, "%s\n", bytes );
        len = strlen( text );
        do { fputc ( text[pt++] , fp); } while (--len); pt = 0;
      }

      n = s->width * channels;
      if(byteps == 8)
        u8 = (uint8_t*) &flt;

      for( k = 0; k < s->height; ++k)
      {
        int height = 0,
            is_allocated = 0;
        out_values = s->getLine( image, k, &height, -1, 
                                            &is_allocated );
        len = n * byteps;

        for( l = 0; l < height; ++l )
        {
          if(byteps == 8)
          {
            dbls = (double*)out_values;
            for(i = 0; i < n; ++i)
            {
              flt = dbls[l * len + i];
              for(j = 0; j < 4; ++j)
                fputc ( u8[j], fp);
            }
          } else 
          for(i = 0; i < len; ++i)
          {
            if(!oyBigEndian() && (byteps == 2))
            { if(i%2)
                fputc ( out_values[l * len + i - 1] , fp);
              else
                fputc ( out_values[l * len + i + 1] , fp);
            } else
              fputc ( out_values[l * len + i] , fp);
          }
        }

        if(is_allocated)
          image->oy_->deallocateFunc_(out_values);
      }

      fflush( fp );
      fclose (fp);
  }

  return error;
}


/** Function oyImage_SetData
 *  @memberof oyImage_s
 *  @brief   set a custom image data module
 *
 *  This function allowes for exchanging of all the module components. 
 *
 *  The pixel_data structure can hold in memory or mmap representations or file
 *  pointers. The according point, line and/or tile functions shall use
 *  the oyImage_s::pixel_data member to access the data and provide in this
 *  interface.
 *
 *  @param         image               the image
 *  @param         pixel_data          data struct will be moved in
 *  @param         getPoint            interface function
 *  @param         getLine             interface function
 *  @param         getTile             interface function
 *  @param         setPoint            interface function
 *  @param         setLine             interface function
 *  @param         setTile             interface function
 *  @return                            error
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/08/23 (Oyranos: 0.1.8)
 *  @date    2008/08/23
 */
int            oyImage_SetData       ( oyImage_s         * image,
                                       oyStruct_s       ** pixel_data,
                                       oyImage_GetPoint_f  getPoint,
                                       oyImage_GetLine_f   getLine,
                                       oyImage_GetTile_f   getTile,
                                       oyImage_SetPoint_f  setPoint,
                                       oyImage_SetLine_f   setLine,
                                       oyImage_SetTile_f   setTile )
{
  oyImage_s_ * s = (oyImage_s_*)image;
  int error = 0;

  if(!s)
    return 1;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 1 )

  if(pixel_data)
  {
    if(s->pixel_data && s->pixel_data->release)
      s->pixel_data->release( &s->pixel_data );
    s->pixel_data = *pixel_data;
    *pixel_data = 0;
  }

  if(getPoint)
    s->getPoint = getPoint;

  if(getLine)
    s->getLine = getLine;

  if(getTile)
    s->getTile = getTile;

  if(setPoint)
    s->setPoint = setPoint;

  if(setLine)
    s->setLine = setLine;

  if(setTile)
    s->setTile = setTile;

  return error;
}
/** Function oyImage_GetWidth
 *  @memberof oyImage_s
 *  @brief   Get the width in pixel
 *
 *  @version Oyranos: 0.5.0
 *  @since   2009/03/05 (Oyranos: 0.1.10)
 *  @date    2012/06/12
 */
int            oyImage_GetWidth      ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->width;
}

/** Function oyImage_GetHeight
 *  @memberof oyImage_s
 *  @brief   Get the width in pixel
 *
 *  @version Oyranos: 0.5.0
 *  @since   2009/03/05 (Oyranos: 0.1.10)
 *  @date    2012/06/12
 */
int            oyImage_GetHeight     ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->height;
}

/** Function oyImage_GetPixelLayout
 *  @memberof oyImage_s
 *  @brief   Get the pixel layout
 *
 *  @see oyPixel_t
 *
 *  @version Oyranos: 0.5.0
 *  @since   2009/03/05 (Oyranos: 0.1.10)
 *  @date    2012/09/05
 */
int            oyImage_GetPixelLayout( oyImage_s         * image,
                                       oyLAYOUT_e          type )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->layout_[type];
}

/** Function oyImage_GetChannelType
 *  @memberof oyImage_s
 *  @brief   Get a channel type
 *
 *  @version Oyranos: 0.5.0
 *  @since   2012/09/05 (Oyranos: 0.5.0)
 *  @date    2012/09/05
 */
oyCHANNELTYPE_e  oyImage_GetChannelType (
                                       oyImage_s         * image,
                                       int                 pos )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->channel_layout[pos];
}
/** Function oyImage_GetSubPositioning
 *  @memberof oyImage_s
 *  @brief   Get sub positioning
 *
 *  @version Oyranos: 0.5.0
 *  @since   2012/09/05 (Oyranos: 0.5.0)
 *  @date    2012/09/05
 */
int            oyImage_GetSubPositioning (
                                       oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->sub_positioning;
}

/** Function oyImage_GetProfile
 *  @memberof oyImage_s
 *  @brief   Get the image profile
 *
 *  @version Oyranos: 0.5.0
 *  @since   2009/08/05 (Oyranos: 0.1.10)
 *  @date    2012/06/12
 */
oyProfile_s *  oyImage_GetProfile    ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;
  oyProfile_s * p;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  p = oyProfile_Copy( s->profile_, 0 );
  return p;
}

/** Function oyImage_GetTags
 *  @memberof oyImage_s
 *  @brief   Get object tags
 *
 *  @version Oyranos: 0.5.0
 *  @since   2009/03/05 (Oyranos: 0.1.10)
 *  @date    2012/06/12
 */
oyOptions_s *  oyImage_GetTags       ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;
  oyOptions_s * opts;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  opts = oyOptions_Copy( s->tags, 0 );
  return opts;
}
/** Function  oyImage_GetPixelData
 *  @memberof oyImage_s
 *  @brief    Get the pixel data
 *
 *  @version Oyranos: 0.5.0
 *  @since   2012/09/05 (Oyranos: 0.5.0)
 *  @date    2012/09/05
 */
oyStruct_s *   oyImage_GetPixelData  ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->pixel_data->copy(s->pixel_data, 0 );
}
/** Function  oyImage_GetPointF
 *  @memberof oyImage_s
 *  @brief    Get the point function
 *
 *  @version Oyranos: 0.5.0
 *  @since   2012/09/05 (Oyranos: 0.5.0)
 *  @date    2012/09/05
 */
oyImage_GetPoint_f oyImage_GetPointF ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->getPoint;
}
/** Function  oyImage_GetLineF
 *  @memberof oyImage_s
 *  @brief    Get the Line function
 *
 *  @version Oyranos: 0.5.0
 *  @since   2012/09/05 (Oyranos: 0.5.0)
 *  @date    2012/09/05
 */
oyImage_GetLine_f  oyImage_GetLineF  ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->getLine;
}
/** Function  oyImage_GetSetPointF
 *  @memberof oyImage_s
 *  @brief    Get the point function
 *
 *  @version Oyranos: 0.9.5
 *  @since   2013/07/22 (Oyranos: 0.9.5)
 *  @date    2012/07/22
 */
oyImage_SetPoint_f oyImage_GetSetPointF ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->setPoint;
}
/** Function  oyImage_GetSetLineF
 *  @memberof oyImage_s
 *  @brief    Get the Line function
 *
 *  @version Oyranos: 0.9.5
 *  @since   2013/07/22 (Oyranos: 0.9.5)
 *  @date    2012/07/22
 */
oyImage_SetLine_f  oyImage_GetSetLineF  ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->setLine;
}
/** Function  oyImage_GetUserData
 *  @memberof oyImage_s
 *  @brief    Get the user data
 *
 *  @version Oyranos: 0.5.0
 *  @since   2012/09/05 (Oyranos: 0.5.0)
 *  @date    2012/09/05
 */
oyStruct_s *   oyImage_GetUserData ( oyImage_s         * image )
{
  oyImage_s_ * s = (oyImage_s_*)image;

  if(!s)
    return 0;

  oyCheckType__m( oyOBJECT_IMAGE_S, return 0 )

  return s->user_data->copy( s->user_data, 0 );
}

/** Function  oyImage_PixelsToSamples
 *  @memberof oyImage_s
 *  @brief    Set sample rectangle from image rectangle
 *
 *  @param[in]     image               a image
 *  @param[in]     image_rectangle     rectangle from image, optional
 *  @param[in,out] pixel_rectangle     rectangle for pixel results
 *  @return                            error
 *
 *  @version  Oyranos: 0.5.0
 *  @date     2012/10/05
 *  @since    2009/03/13 (Oyranos: 0.1.10)
 */
int            oyImage_PixelsToSamples(oyImage_s         * image,
                                       oyRectangle_s     * image_rectangle,
                                       oyRectangle_s     * pixel_rectangle )
{
  int error = !image,
      channel_n = 0;

  oyRectangle_s_ ** pixel_rectangle_ = (oyRectangle_s_**)&pixel_rectangle;

  if(!error && image->type_ != oyOBJECT_IMAGE_S)
    return 0;

  if(!error)
  {
    channel_n = oyImage_GetPixelLayout( image, oyCHANS );

    if(!image_rectangle)
    {
      oyRectangle_SetGeo( pixel_rectangle, 0,0, oyImage_GetWidth(image),
                                                oyImage_GetHeight(image) );
      (*pixel_rectangle_)->width *= channel_n;

    } else
    {
      oyRectangle_SetByRectangle( pixel_rectangle, image_rectangle );
      oyRectangle_Scale( pixel_rectangle, oyImage_GetWidth(image) );
      (*pixel_rectangle_)->x *= channel_n;
      (*pixel_rectangle_)->width *= channel_n;
      oyRectangle_Round( pixel_rectangle );
    }
  }

  return error;
}

/** Function oyImage_FromFile
 *  @memberof oyImage_s
 *  @brief   generate a Oyranos image from a file name
 *
 *  The file name will be stored inside the image's tags.
 *  @verbatim
    
  oyOptions_s * tags = oyImage_GetTags( image );
  const char * filename = oyOptions_FindString( tags,
                                       "//" OY_TYPE_STD "/file_read/filename",
                                                0 );
  oyOptions_Release( &tags );
    @endverbatim
 *
 *  @param[in]     file_name           input
 *  @param[in]     icc_profile_flags   profile selection flags, see oyProfile_FromFile()
 *  @param[out]    image               output
 *  @param[in]     obj                 Oyranos object (optional)
 *  @return                            >0 == error, <0 == issue or zero
 *
 *  @version Oyranos: 0.9.6
 *  @date    2014/04/06
 *  @since   2012/04/21 (Oyranos: 0.5.0)
 */
int    oyImage_FromFile              ( const char        * file_name,
                                       int                 icc_profile_flags,
                                       oyImage_s        ** image,
                                       oyObject_s          obj )
{
  oyFilterNode_s * in, * out;
  int error = 0;
  oyConversion_s * conversion = 0;
  oyOptions_s * options = 0;

  if(!file_name)
    return 1;

  /* start with an empty conversion object */
  conversion = oyConversion_New( obj );
  /* create a filter node */
  in = oyFilterNode_NewWith( "//" OY_TYPE_STD "/file_read.meta", 0, obj );
  /* set the above filter node as the input */
  oyConversion_Set( conversion, in, 0 );

  /* add a file name argument */
  /* get the options of the input node */
  if(in)
  options = oyFilterNode_GetOptions( in, OY_SELECT_FILTER );
  /* add a new option with the appropriate value */
  error = oyOptions_SetFromText( &options, "//" OY_TYPE_STD "/file_read/filename",
                                 file_name, OY_CREATE_NEW );
  DBGs_NUM1_S(in, "set //" OY_TYPE_STD "/file_read/filename to %s", file_name);
  if(icc_profile_flags)
    oyOptions_SetFromInt( &options, "///icc_profile_flags", icc_profile_flags, 0, OY_CREATE_NEW );
  /* release the options object, this means its not any more refered from here*/
  oyOptions_Release( &options );

  /* add a closing node */
  out = oyFilterNode_NewWith( "//" OY_TYPE_STD "/output", 0, obj );
  error = oyFilterNode_Connect( in, "//" OY_TYPE_STD "/data",
                                out, "//" OY_TYPE_STD "/data", 0 );
  /* set the output node of the conversion */
  oyConversion_Set( conversion, 0, out );

  *image = oyConversion_GetImage( conversion, OY_OUTPUT );
  oyImage_Release( image );
  *image = oyConversion_GetImage( conversion, OY_INPUT );

  options = oyImage_GetTags( *image );
  oyOptions_SetFromText( &options, "//" OY_TYPE_STD "/file_read/filename",
                                 file_name, OY_CREATE_NEW );
  oyOptions_Release( &options );

  oyConversion_Release( &conversion );

  return error;
}
/** Function oyImage_ToFile
 *  @memberof oyImage_s
 *  @brief   write a Oyranos image to a file name
 *
 *  @param[in]     image               input
 *  @param[in]     file_name           output
 *  @param[in]     opts                options for file_write node
 *  @return                            >0 == error, <0 == issue or zero
 *
 *  @version Oyranos: 0.5.0
 *  @since   2012/07/18 (Oyranos: 0.5.0)
 *  @date    2012/07/18
 */
int    oyImage_ToFile                ( oyImage_s         * image,
                                       const char        * file_name,
                                       oyOptions_s       * opts )
{
  oyFilterNode_s * in, * out;
  int error = 0;
  oyConversion_s * conversion = 0;
  oyOptions_s * options = 0;

  if(!file_name)
    return 1;

  /* start with an empty conversion object */
  conversion = oyConversion_New( 0 );
  /* create a filter node */
  in = oyFilterNode_NewWith( "//" OY_TYPE_STD "/root", 0, 0 );
  /* set the above filter node as the input */
  oyConversion_Set( conversion, in, 0 );
  /* set the image buffer */
  oyFilterNode_SetData( in, (oyStruct_s*)image, 0, 0 );

  /* add a file name argument */
  /* get the options of the input node */
  if(in)
  options = oyFilterNode_GetOptions( in, OY_SELECT_FILTER );
  if(opts)
  {
    oyOptions_CopyFrom( &options, opts, oyBOOLEAN_UNION, 0,0);
  }
  /* add a new option with the appropriate value */
  error = oyOptions_SetFromText( &options, "//" OY_TYPE_STD "/file_write/filename",
                                 file_name, OY_CREATE_NEW );
  /* release the options object, this means its not any more refered from here*/
  oyOptions_Release( &options );

  /* add a closing node */
  out = oyFilterNode_NewWith( "//" OY_TYPE_STD "/file_write.meta", 0, 0 );
  error = oyFilterNode_Connect( in, "//" OY_TYPE_STD "/data",
                                out, "//" OY_TYPE_STD "/data", 0 );
  /* set the output node of the conversion */
  oyConversion_Set( conversion, 0, out );

  options = oyImage_GetTags( image );
  oyOptions_SetFromText( &options, "//" OY_TYPE_STD "/file_write/filename",
                                 file_name, OY_CREATE_NEW );
  oyOptions_Release( &options );

  error = oyConversion_RunPixels( conversion, 0 );
  if(error > 0)
    WARNcc1_S(in,"oyConversion_RunPixels() returned error: %d", error);

  oyConversion_Release( &conversion );
  oyFilterNode_Release( &in );
  oyFilterNode_Release( &out );

  return error;
}

