/** @file oyArray2d_s_.c

   [Template file inheritance graph]
   +-> oyArray2d_s_.template.c
   |
   +-- Base_s_.c

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



  
#include "oyArray2d_s.h"
#include "oyArray2d_s_.h"





#include "oyObject_s.h"
#include "oyranos_object_internal.h"


  


/* Include "Array2d.private_custom_definitions.c" { */
#include "oyranos_image_internal.h"

/** Function    oyArray2d_Release__Members
 *  @memberof   oyArray2d_s
 *  @brief      Custom Array2d destructor
 *  @internal
 *
 *  This function will free up all memmory allocated by the
 *  input object. First all object members witch have their
 *  own release method are deallocated. Then the deallocateFunc_
 *  of the oy_ object is used to release the rest of the members
 *  that were allocated with oy_->allocateFunc_.
 *
 *  @param[in]  array2d  the Array2d object
 *
 *  @version Oyranos: x.x.x
 *  @since   YYYY/MM/DD (Oyranos: x.x.x)
 *  @date    YYYY/MM/DD
 */
void oyArray2d_Release__Members( oyArray2d_s_ * array2d )
{
  /* Deallocate members here
   * E.g: oyXXX_Release( &array2d->member );
   */
  oyArray2d_ReleaseArray( (oyArray2d_s*)array2d );

  if(array2d->oy_->deallocateFunc_)
  {
#if 0
    oyDeAlloc_f deallocateFunc = array2d->oy_->deallocateFunc_;
#endif

    /* Deallocate members of basic type here
     * E.g.: deallocateFunc( array2d->member );
     */
  }
}

/** Function    oyArray2d_Init__Members
 *  @memberof   oyArray2d_s
 *  @brief      Custom Array2d constructor
 *  @internal
 *
 *  This function will allocate all memmory for the input object.
 *  For the basic member types this is done using the allocateFunc_
 *  of the attatced (oyObject_s)oy_ object.
 *
 *  @param[in]  array2d  the Array2d object
 *
 *  @version Oyranos: x.x.x
 *  @since   YYYY/MM/DD (Oyranos: x.x.x)
 *  @date    YYYY/MM/DD
 */
int oyArray2d_Init__Members( oyArray2d_s_ * array2d )
{
  return 0;
}

/** Function    oyArray2d_Copy__Members
 *  @memberof   oyArray2d_s
 *  @brief      Custom Array2d copy constructor
 *  @internal
 *
 *  This function makes a copy of all values from the input
 *  to the output object. The destination object and all of its
 *  members should already be allocated.
 *
 *  @param[in]   src  the oyArray2d_s_ input object
 *  @param[out]  dst  the output oyArray2d_s_ object
 *
 *  @version Oyranos: x.x.x
 *  @since   YYYY/MM/DD (Oyranos: x.x.x)
 *  @date    YYYY/MM/DD
 */
int oyArray2d_Copy__Members( oyArray2d_s_ * dst, oyArray2d_s_ * src)
{
  int error = 0, i, size;
  oyAlloc_f allocateFunc_ = 0;
  oyDeAlloc_f deallocateFunc_ = 0;

  if(!dst || !src)
    return 1;

  allocateFunc_ = dst->oy_->allocateFunc_;
  deallocateFunc_ = dst->oy_->deallocateFunc_;

  /* Copy each value of src to dst here */

  dst->own_lines = 2;
  for(i = 0; i < dst->height; ++i)
  {
    size = dst->width * oySizeofDatatype( dst->t );
    oyAllocHelper_m_( dst->array2d[i], unsigned char, size, allocateFunc_,
                      error = 1; break );
    error = !memcpy( dst->array2d[i], src->array2d[i], size );
  }

  return error;
}

/* } Include "Array2d.private_custom_definitions.c" */



/** @internal
 *  Function oyArray2d_New_
 *  @memberof oyArray2d_s_
 *  @brief   allocate a new oyArray2d_s_  object
 *
 *  @version Oyranos: 
 *  @since   2010/04/26 (Oyranos: 0.1.10)
 *  @date    2010/04/26
 */
oyArray2d_s_ * oyArray2d_New_ ( oyObject_s object )
{
  /* ---- start of common object constructor ----- */
  oyOBJECT_e type = oyOBJECT_ARRAY2D_S;
  int error = 0;
  oyObject_s    s_obj = oyObject_NewFrom( object );
  oyArray2d_s_ * s = 0;

  if(s_obj)
    s = (oyArray2d_s_*)s_obj->allocateFunc_(sizeof(oyArray2d_s_));

  if(!s || !s_obj)
  {
    WARNc_S(_("MEM Error."));
    return NULL;
  }

  error = !memset( s, 0, sizeof(oyArray2d_s_) );
  if(error)
    WARNc_S( "memset failed" );

  s->type_ = type;
  s->copy = (oyStruct_Copy_f) oyArray2d_Copy;
  s->release = (oyStruct_Release_f) oyArray2d_Release;

  s->oy_ = s_obj;

  
  /* ---- start of custom Array2d constructor ----- */
  error += !oyObject_SetParent( s_obj, oyOBJECT_ARRAY2D_S, (oyPointer)s );
  /* ---- end of custom Array2d constructor ------- */
  
  
  
  
  /* ---- end of common object constructor ------- */
  if(error)
    WARNc_S( "oyObject_SetParent failed" );


  
  

  
  /* ---- start of custom Array2d constructor ----- */
  error += oyArray2d_Init__Members( s );
  /* ---- end of custom Array2d constructor ------- */
  
  
  
  

  if(error)
    WARNc1_S("%d", error);

  return s;
}

/** @internal
 *  Function oyArray2d_Copy__
 *  @memberof oyArray2d_s_
 *  @brief   real copy a Array2d object
 *
 *  @param[in]     array2d                 Array2d struct object
 *  @param         object              the optional object
 *
 *  @version Oyranos: 
 *  @since   2010/04/26 (Oyranos: 0.1.10)
 *  @date    2010/04/26
 */
oyArray2d_s_ * oyArray2d_Copy__ ( oyArray2d_s_ *array2d, oyObject_s object )
{
  oyArray2d_s_ *s = 0;
  int error = 0;

  if(!array2d || !object)
    return s;

  s = (oyArray2d_s_*)oyArray2d_Create( 0, array2d->height, 0, array2d->t, object );
  error = !s;

  if(!error) {
    
    /* ---- start of custom Array2d copy constructor ----- */
    error = oyArray2d_Copy__Members( s, array2d );
    /* ---- end of custom Array2d copy constructor ------- */
    
    
    
    
    
    
  }

  if(error)
    oyArray2d_Release_( &s );

  return s;
}

/** @internal
 *  Function oyArray2d_Copy_
 *  @memberof oyArray2d_s_
 *  @brief   copy or reference a Array2d object
 *
 *  @param[in]     array2d                 Array2d struct object
 *  @param         object              the optional object
 *
 *  @version Oyranos: 
 *  @since   2010/04/26 (Oyranos: 0.1.10)
 *  @date    2010/04/26
 */
oyArray2d_s_ * oyArray2d_Copy_ ( oyArray2d_s_ *array2d, oyObject_s object )
{
  oyArray2d_s_ *s = array2d;

  if(!array2d)
    return 0;

  if(array2d && !object)
  {
    s = array2d;
    
    oyObject_Copy( s->oy_ );
    return s;
  }

  s = oyArray2d_Copy__( array2d, object );

  return s;
}
 
/** @internal
 *  Function oyArray2d_Release_
 *  @memberof oyArray2d_s_
 *  @brief   release and possibly deallocate a Array2d object
 *
 *  @param[in,out] array2d                 Array2d struct object
 *
 *  @version Oyranos: 
 *  @since   2010/04/26 (Oyranos: 0.1.10)
 *  @date    2010/04/26
 */
int oyArray2d_Release_( oyArray2d_s_ **array2d )
{
  /* ---- start of common object destructor ----- */
  oyArray2d_s_ *s = 0;

  if(!array2d || !*array2d)
    return 0;

  s = *array2d;

  *array2d = 0;

  if(oyObject_UnRef(s->oy_))
    return 0;
  /* ---- end of common object destructor ------- */

  
  /* ---- start of custom Array2d destructor ----- */
  oyArray2d_Release__Members( s );
  /* ---- end of custom Array2d destructor ------- */
  
  
  
  



  if(s->oy_->deallocateFunc_)
  {
    oyDeAlloc_f deallocateFunc = s->oy_->deallocateFunc_;

    oyObject_Release( &s->oy_ );

    deallocateFunc( s );
  }

  return 0;
}



/* Include "Array2d.private_methods_definitions.c" { */
/** Function  oyArray2d_Create_
 *  @memberof oyArray2d_s
 *  @brief    Allocate and initialise a oyArray2d_s object widthout pixel
 *  @internal
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/08/23 (Oyranos: 0.1.8)
 *  @date    2008/10/03
 */
oyArray2d_s_ *
                   oyArray2d_Create_ ( int                 width,
                                       int                 height,
                                       oyDATATYPE_e        data_type,
                                       oyObject_s          object )
{
  oyArray2d_s_ * s = 0;
  int error = 0;

  if(!width || !height)
    return s;

  s = (oyArray2d_s_*)oyArray2d_New( object );
  error = !s;

  if(error <= 0)
  {
    int y_len = sizeof(unsigned char *) * (height + 1);

    s->width = width;
    s->height = height;
    s->t = data_type;
    s->data_area.type_ = oyOBJECT_RECTANGLE_S;
    oyRectangle_SetGeo( (oyRectangle_s*)&s->data_area, 0,0, width, height );
    s->array2d = s->oy_->allocateFunc_( y_len );
    error = !memset( s->array2d, 0, y_len );
    s->own_lines = oyNO;
  }

  return s;
}

/** Function  oyArray2d_ToPPM_
 *  @memberof oyArray2d_s
 *  @brief    Dump array to a netppm file 
 *  @internal
 *
 *  @param[in]     array               the array to fill read from
 *  @param[in]     file_name           rectangle of interesst in samples
 *
 *  @version Oyranos: 0.1.10
 *  @since   2009/06/18 (Oyranos: 0.1.10)
 *  @date    2009/06/18
 */
int              oyArray2d_ToPPM_    ( oyArray2d_s_      * array,
                                       const char        * file_name )
{
  oyArray2d_s_ * s = array;
  int error = 0, i,j, len, shift;
  size_t size,
         byteps;
  char * buf,
       * data;

  if(!array || !file_name || !file_name[0])
    return 1;

  oyCheckType__m( oyOBJECT_ARRAY2D_S, return 1 )

  if(!error)
  {
    if(s->array2d[0] == NULL)
    {
      printf("oyArray2d_s[%d] is not yet used/allocated\n",
             oyObject_GetId(s->oy_));
      return 1;
    }

    byteps = oySizeofDatatype(s->t); /* byte per sample */
    size = s->width * s->height * byteps;
    buf = oyAllocateFunc_(size + 1024);

    if(buf && size)
    {
      switch(s->t) {
      case oyUINT8:     /*  8-bit integer */
           sprintf( buf, "P5\n#%s:%d oyArray2d_s[%d]\n%d %d\n255\n", 
                    __FILE__,__LINE__, oyObject_GetId(s->oy_),
                    s->width, s->height );
           break;
      case oyUINT16:    /* 16-bit integer */
      case oyUINT32:    /* 32-bit integer */
           sprintf( buf, "P5\n#%s:%d oyArray2d_s[%d]\n%d %d\n65535\n", 
                    __FILE__,__LINE__, oyObject_GetId(s->oy_),
                    s->width, s->height );
           break;
      case oyHALF:      /* 16-bit floating point number */
      case oyFLOAT:     /* IEEE floating point number */
      case oyDOUBLE:    /* IEEE double precission floating point number */
           sprintf( buf, "Pf\n#%s:%d oyArray2d_s[%d]\n%d %d\n%s\n", 
                    __FILE__,__LINE__, oyObject_GetId(s->oy_),
                    s->width, s->height,
                    oyBigEndian()? "1.0" : "-1.0" );
           break;
      default: return 1;
      }

      len = oyStrlen_(buf);
      data = &buf[len];
      shift = oyBigEndian() ? 0 : 1;

      switch(s->t) {
      case oyUINT8:     /*  8-bit integer */
      case oyFLOAT:     /* IEEE floating point number */
           for(i = 0; i < s->height; ++i)
             memcpy( &data[i * s->width * byteps],
                     s->array2d[i],
                     s->width * byteps );
           break;
      case oyUINT16:    /* 16-bit integer */
           for(i = 0; i < s->height; ++i)
             memcpy( &data[i * s->width * byteps + shift],
                     s->array2d[i],
                     s->width * byteps );
           break;
      case oyUINT32:    /* 32-bit integer */
           for(i = 0; i < s->height; ++i)
             for(j = 0; j < s->width; ++j)
               ((uint16_t*)&data[i*s->width*2])[j] =
                                       *((uint32_t*)&s->array2d[i][j*byteps]) /
                                                     65537;
           break;
      case oyHALF:      /* 16-bit floating point number */
           for(i = 0; i < s->height; ++i)
             for(j = 0; j < s->width; ++j)
               ((uint16_t*)&data[i*s->width*2])[j] = 
                                       *((uint16_t*)&s->array2d[i][j*byteps]);
           break;
      case oyDOUBLE:    /* IEEE double precission floating point number */
           for(i = 0; i < s->height; ++i)
             for(j = 0; j < s->width; ++j)
               ((float*)&data[i*s->width*2])[j] =
                                       *((double*)&s->array2d[i][j*byteps]);
           break;
      default: return 1;
      }

      error = oyWriteMemToFile_( file_name, buf, len + size );
    }

    if(buf) oyDeAllocateFunc_(buf);
      size = 0;
  }

  return error;
}

/* } Include "Array2d.private_methods_definitions.c" */
