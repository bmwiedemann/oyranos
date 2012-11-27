{% load gsoc_extensions %}
{% include "source_file_header.txt" %}

#ifndef {{ file_name|underscores|upper|tr:". _" }}
#define {{ file_name|underscores|upper|tr:". _" }}

#include <oyranos_icc.h>
#include <oyranos_types.h>

{% include "cpp_begin.h" %}

/** \addtogroup objects_image
 *
 *  @{
 */

/** @enum    oyDATATYPE_e
 *  @brief   Basic data types for anonymous pointers 
 *
 *  @version Oyranos: 0.1.8
 *  @since   2007/00/00 (Oyranos: 0.1.8)
 *  @date    2007/00/00
 */
typedef enum oyDATATYPE_e {
  oyUINT8,     /*!<  8-bit integer */
  oyUINT16,    /*!< 16-bit integer */
  oyUINT32,    /*!< 32-bit integer */
  oyHALF,      /*!< 16-bit floating point number */
  oyFLOAT,     /*!< IEEE floating point number */
  oyDOUBLE     /*!< IEEE double precission floating point number */
} oyDATATYPE_e;

size_t             oyDataTypeGetSize ( oyDATATYPE_e        data_type ); 
const char *       oyDataTypeToText  ( oyDATATYPE_e        data_type );

/** @enum    oyCHANNELTYPE_e
 *  @brief   Channels types of Colours 
 *
 *  This type is useful to present colour manipulation UI elements in a 
 *  sensible way.
 *
 *  @version Oyranos: 0.1.10
 *  @since   2008/00/00 (Oyranos: 0.1.8)
 *  @date    2009/08/03
 */
typedef enum oyCHANNELTYPE_e {
  oyCHANNELTYPE_UNDEFINED,            /*!< as well for the list end */
  oyCHANNELTYPE_OTHER,                /*!< other */
  oyCHANNELTYPE_LIGHTNESS,            /*!< pure lightness */
  oyCHANNELTYPE_COLOUR,               /*!< pure colour */
  oyCHANNELTYPE_COLOUR_LIGHTNESS,     /*!< lightness and colour */
  oyCHANNELTYPE_COLOUR_LIGHTNESS_PREMULTIPLIED, /*!< premultiplied lightness and colour */
  oyCHANNELTYPE_DEPTH,                /*!< depth field */
  oyCHANNELTYPE_ALPHA1,               /*!< alpha */
  oyCHANNELTYPE_ALPHA2,               /*!< alpha2 */
  oyCHANNELTYPE_ALPHA3,               /*!< alpha3 */
  oyCHANNELTYPE_U,                    /*!< u */
  oyCHANNELTYPE_V                     /*!< v */
} oyCHANNELTYPE_e;

oyCHANNELTYPE_e oyICCColourSpaceToChannelLayout (
                                       icColorSpaceSignature sig,
                                       int                 pos );


/** @enum    oyPIXEL_ACCESS_TYPE_e
 *  @brief   Pixel access types 
 *
 *  @version Oyranos: 0.1.8
 *  @since   2008/00/00 (Oyranos: 0.1.8)
 *  @date    2008/00/00
 */
typedef enum {
  oyPIXEL_ACCESS_IMAGE,                /*!< image accessor */
  oyPIXEL_ACCESS_POINT,                /*!< dont use */
  oyPIXEL_ACCESS_LINE                  /*!< dont use */
} oyPIXEL_ACCESS_TYPE_e;

/** @typedef oyPixel_t
    parametric type as shorthand for the channel layout in bitmaps \n

    should fit into a 32bit type, usual unsigned int or uint32_t \n

    X F P S TTTT OOOOOOOO CCCCCCCC \n

    C  channels count per pixel (3 for RGB); max 255 \n
    O  colour channel offset (0 for RGB, 1 for ARGB) \n
    P  Planar bit: 0 - interwoven, 1 - one channel after the other \n
    S  Swap colour channels bit (BGR) \n
    T  Type oyDATATYPE_e \n
    X  non host byte order bit \n
    F  Revert bit: 0 - MinIsBlack(Chocolate) 1 - MinIsWhite(Vanilla); \n
       exchange min and max : (1-x)

 */
typedef uint32_t oyPixel_t;

char   *           oyPixelPrint      ( oyPixel_t           pixel_layout,
                                       oyAlloc_f           allocateFunc );


/** @enum    oyLAYOUT_e
 *  @brief   Pixel mask descriptions.
 */
typedef enum {
  oyLAYOUT = 0, /*!< remembering the layout @see oyPixel_t */
  oyPOFF_X,     /*!< pixel count x offset */
  oyPOFF_Y,     /*!< pixel count y offset */
  oyCOFF,       /*!< channel offset */
  oyDATA_SIZE,  /*!< sample size in byte */
  oyCHANS,      /*!< number of channels */
  oyCHAN0       /*!< first colour channel position */
} oyLAYOUT_e;

/** Set channels count in @ref oyPixel_t */
#define oyChannels_m(c)             (c)
/** Set offset count to first colour channel in  @ref oyPixel_t */
#define oyColourOffset_m(o)         ((o) << 8)
/** Set @ref oyDATATYPE_e in  @ref oyPixel_t */
#define oyDataType_m(t)             ((t) << 16)
/** Set swapping of colour channels in @ref oyPixel_t , e.g. BGR */
#define oySwapColourChannels_m(s)   ((s) << 20)
/** Set planar instead of interwoven colour channels in @ref oyPixel_t */
#define oyPlanar_m(p)               ((p) << 21)
/** Set min is white in @ref oyPixel_t */
#define oyFlavor_m(p)               ((f) << 22)
/** Set byte swapping for integers larger than 8 byte in @ref oyPixel_t */
#define oyByteSwap_m(x)             ((x) << 23)

/* define some common types */
/** 8-bit RGB/YUV/XYZ/... */
#define OY_TYPE_123_8       (oyChannels_m(3)|oyDataType_m(oyUINT8))
/** 16-bit RGB/... */
#define OY_TYPE_123_16      (oyChannels_m(3)|oyDataType_m(oyUINT16))
/** 16-bit float RGB/... */
#define OY_TYPE_123_HALF    (oyChannels_m(3)|oyDataType_m(oyHALF))
/** 32-bit float RGB/... */
#define OY_TYPE_123_FLOAT   (oyChannels_m(3)|oyDataType_m(oyFLOAT))
/** 64-bit double RGB/... */
#define OY_TYPE_123_DBL     (oyChannels_m(3)|oyDataType_m(oyDOUBLE))

/** 8-bit RGB/... alpha */
#define OY_TYPE_123A_8      (oyChannels_m(4)|oyDataType_m(oyUINT8))
/** 16-bit RGB/... alpha */
#define OY_TYPE_123A_16     (oyChannels_m(4)|oyDataType_m(oyUINT16))
/** 16-bit float RGB/... alpha */
#define OY_TYPE_123A_HALF   (oyChannels_m(4)|oyDataType_m(oyHALF))
/** 32-bit float RGB/... alpha */
#define OY_TYPE_123A_FLOAT  (oyChannels_m(4)|oyDataType_m(oyFLOAT))
/** 64-bit double RGB/... alpha */
#define OY_TYPE_123A_DBL    (oyChannels_m(4)|oyDataType_m(oyDOUBLE))

/** 16-bit float RGB/... alpha + Z */
#define OY_TYPE_123AZ_HALF  (oyChannels_m(5)|oyDataType_m(oyHALF))
/** 32-bit float RGB/... alpha + Z */
#define OY_TYPE_123AZ_FLOAT (oyChannels_m(5)|oyDataType_m(oyFLOAT))
/** 64-bit double RGB/... alpha + Z */
#define OY_TYPE_123AZ_DBL   (oyChannels_m(5)|oyDataType_m(oyDOUBLE))

#define OY_TYPE_A123_8      (oyChannels_m(4)|oyColourOffset_m(1)|oyDataType_m(oyUINT8))
#define OY_TYPE_A123_16     (oyChannels_m(4)|oyColourOffset_m(1)|oyDataType_m(oyUINT16))

#define OY_TYPE_A321_8      (oyChannels_m(4)|oyColourOffset_m(1)|oyDataType_m(oyUINT8)|oySwapColourChannels_m(oyYES))
#define OY_TYPE_A321_16     (oyChannels_m(4)|oyColourOffset_m(1)|oyDataType_m(oyUINT16)|oySwapColourChannels_m(oyYES))

/* some intermixed types will work as well */
#define OY_TYPE_123A_HALF_Z_FLOAT (oyChannels_m(4+1*2)|oyDataType_m(oyHALF))

/** 8-bit Cmyk */
#define OY_TYPE_1234_8      (oyChannels_m(4)|oyDataType_m(oyUINT8))
/** 8-bit Cmyk alpha */
#define OY_TYPE_1234A_8     (oyChannels_m(5)|oyDataType_m(oyUINT8))
#define OY_TYPE_1234_16     (oyChannels_m(4)|oyDataType_m(oyUINT16))
#define OY_TYPE_1234A_16    (oyChannels_m(5)|oyDataType_m(oyUINT16))

#define OY_TYPE_1234_8_REV  (oyChannels_m(4)|oyDataType_m(oyUINT8)|oyFlavor_m(oyYes))
#define OY_TYPE_1234A_8_REV (oyChannels_m(5)|oyDataType_m(oyUINT8)|oyFlavor_m(oyYes))
#define OY_TYPE_1234_16_REV (oyChannels_m(4)|oyDataType_m(oyUINT16)|oyFlavor_m(oyYes))
#define OY_TYPE_1234A_16_REV (oyChannels_m(5)|oyDataType_m(oyUINT16)|oyFlavor_m(oyYes))


/** 8-bit Gray */
#define OY_TYPE_1_8         (oyChannels_m(1)|oyDataType_m(oyUINT8))
/** 16-bit Gray */
#define OY_TYPE_1_16        (oyChannels_m(1)|oyDataType_m(oyUINT16))
#define OY_TYPE_1_HALF      (oyChannels_m(1)|oyDataType_m(oyHALF))
#define OY_TYPE_1_FLOAT     (oyChannels_m(1)|oyDataType_m(oyFLOAT))
#define OY_TYPE_1A_8        (oyChannels_m(2)|oyDataType_m(oyUINT8))
#define OY_TYPE_1A_16       (oyChannels_m(2)|oyDataType_m(oyUINT16))
#define OY_TYPE_1A_HALF     (oyChannels_m(2)|oyDataType_m(oyHALF))
#define OY_TYPE_1A_FLOAT    (oyChannels_m(2)|oyDataType_m(oyFLOAT))

#define OY_TYPE_1_8_REV     (oyChannels_m(1)|oyDataType_m(oyUINT8)|oyFlavor_m(oyYes))
#define OY_TYPE_1_16_REV    (oyChannels_m(1)|oyDataType_m(oyUINT16)|oyFlavor_m(oyYes))
#define OY_TYPE_1_HALF_REV  (oyChannels_m(1)|oyDataType_m(oyHALF)|oyFlavor_m(oyYes))
#define OY_TYPE_1_FLOAT_REV (oyChannels_m(1)|oyDataType_m(oyFLOAT))|oyFlavor_m(oyYes)
#define OY_TYPE_1A_8_REV    (oyChannels_m(2)|oyDataType_m(oyUINT8)|oyFlavor_m(oyYes))
#define OY_TYPE_1A_16_REV   (oyChannels_m(2)|oyDataType_m(oyUINT16)|oyFlavor_m(oyYes))
#define OY_TYPE_1A_HALF_REV (oyChannels_m(2)|oyDataType_m(oyHALF)|oyFlavor_m(oyYes))
#define OY_TYPE_1A_FLOAT_REV (oyChannels_m(2)|oyDataType_m(oyFLOAT)|oyFlavor_m(oyYes))

/* decode */
/** Get channels count in @ref oyPixel_t */
#define oyToChannels_m(c)           ((c)&255)
/** Get offset count to first colour channel in  @ref oyPixel_t */
#define oyToColourOffset_m(o)       (((o) >> 8)&255)
/** Get @ref oyDATATYPE_e in  @ref oyPixel_t */
#define oyToDataType_m(t)           ((oyDATATYPE_e)(((t) >> 16)&15))
/** Get swapping of colour channels in @ref oyPixel_t , e.g. BGR */
#define oyToSwapColourChannels_m(s) (((s) >> 20)&1)
/** Get planar instead of interwoven colour channels in @ref oyPixel_t */
#define oyToPlanar_m(p)             (((p) >> 21)&1)
/** Get min is white in @ref oyPixel_t */
#define oyToFlavor_m(f)             (((f) >> 22)&1)
/** Get byte swapping for integers larger than 8 byte in @ref oyPixel_t */
#define oyToByteswap_m(x)           (((x) >> 23)&1)

/** } *//* objects_image */

{% include "cpp_end.h" %}

#endif /* {{ file_name|underscores|upper|tr:". _" }} */