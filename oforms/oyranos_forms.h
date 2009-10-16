/** @file oyranos_forms.h
 *
 *  Oyranos is an open source Colour Management System 
 *
 *  @par Copyright:
 *            2009 (C) Kai-Uwe Behrmann
 *
 *  @brief    common Oyranos forms declarations
 *  @internal
 *  @author   Kai-Uwe Behrmann <ku.b@gmx.de>
 *  @par License:
 *            new BSD <http://www.opensource.org/licenses/bsd-license.php>
 *  @since    2009/10/13
 */

#ifndef OYRANOS_FORMS_H
#define OYRANOS_FORMS_H


#ifdef __cplusplus
namespace oyranos {
namespace forms {
extern "C" {
#endif /* __cplusplus */

typedef struct oyFormsArgs_s oyFormsArgs_s;

/** @struct  oyFormsArgs_s
 *  @brief   a FormsArgs object
 *  @extends oyStruct_s
 *
 *  @version Oyranos: 0.1.10
 *  @since   2009/10/12 (Oyranos: 0.1.10)
 *  @date    2009/10/12
 */
struct oyFormsArgs_s {
  int              type_;
  oyFormsArgs_s *  (*copy) (oyFormsArgs_s*, void*);
  int              (*release) (oyFormsArgs_s**);
  void           * oy_;

  int              silent;
  void           * xforms_data_model_;
};

oyFormsArgs_s *  oyFormsArgs_New     ( void              * private_object_ );
oyFormsArgs_s *  oyFormsArgs_Copy    ( oyFormsArgs_s     * args,
                                       void              * private_object_ );
int          oyFormsArgs_Release     ( oyFormsArgs_s    ** args );

const char * oyFormsArgs_ModelGet    ( oyFormsArgs_s     * args );


extern oyUiHandler_s * oy_ui_cmd_line_handlers[];


#ifdef __cplusplus
} /* extern "C" */
} /* namespace forms */
} /* namespace oyranos */
#endif /* __cplusplus */

#endif /* OYRANOS_FORMS_H */
