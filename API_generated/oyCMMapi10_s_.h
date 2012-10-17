/** @file oyCMMapi10_s_.h

   [Template file inheritance graph]
   +-> oyCMMapi10_s_.template.h
   |
   +-> oyCMMapiFilter_s_.template.h
   |
   +-> oyCMMapi_s_.template.h
   |
   +-- Base_s_.h

 *  Oyranos is an open source Colour Management System
 *
 *  @par Copyright:
 *            2004-2012 (C) Kai-Uwe Behrmann
 *
 *  @author   Kai-Uwe Behrmann <ku.b@gmx.de>
 *  @par License:
 *            new BSD - see: http://www.opensource.org/licenses/bsd-license.php
 *  @date     2012/10/16
 */


#ifndef OY_CMM_API10_S__H
#define OY_CMM_API10_S__H

#ifdef __cplusplus
/*namespace oyranos {*/
  extern "C" {
#endif /* __cplusplus */


#define oyCMMapi10Priv_m( var ) ((oyCMMapi10_s_*) (var))

typedef struct oyCMMapi10_s_ oyCMMapi10_s_;

  
#include <oyranos_object.h>



#include "oyranos_module_internal.h"
#include "oyCMMinfo_s_.h"

#include "oyCMMapi5_s_.h"
  

#include "oyCMMapi10_s.h"

/* Include "CMMapi10.private.h" { */
/** typedef  oyMOptions_Handle_f
 *  @brief   generic data exchange
 *  @ingroup module_api
 *  @memberof oyCMMapi10_s
 *
 *  The provided data passed with oyOptions_s shall be processed according to
 *  the rules of a given command. The result is again a generic oyOptions_s
 *  object.
 *
 *  This module type works similiar to the device modules, which are defined in
 *  oyCMMapi8_s. While the module interface is simple it has minimal meaning to
 *  the intented work or data exchange. The rules are to be defined by the
 *  communicating parties, the host and the module. They use the plug-in
 *  mechanism only for their convinience.
 *
 *  For speed up the "can_handle.xxx" command must be implemented. "xxx" means
 *  in the previous command string the intented command to be performed. The
 *  "can_handle" command shall not produce any output in result, but shall
 *  indicate with zero that it could handle the request. This command is
 *  intented to lightwight select a capable module.
 *
 *  @param[in]     options             the input data; It is typical not
 *                                     changed.
 *  @param[in]     command             a string describing the intention of the
 *                                     caller; Allowed are alphanumerical signs.
 *                                     lower bar '_', minus '-', plus '+' and
 *                                     point '.' . other signs including white
 *                                     space must not occure.
 *                                     Strings are separated by point '.' .
 *                                     The first string is the actual command.
 *                                     The following strings are merely
 *                                     modifiers.
 *  @param[out]    result              the created data
 *  @return                            0 - indifferent, >= 1 - error,
 *                                     <= -1 - issue,
 *                                     + a message should be sent
 *
 *  @version Oyranos: 0.1.10
 *  @since   2009/12/11 (Oyranos: 0.1.10)
 *  @date    2009/12/11
 */
typedef int  (*oyMOptions_Handle_f)  ( oyOptions_s       * options,
                                       const char        * command,
                                       oyOptions_s      ** result );

/* } Include "CMMapi10.private.h" */

/** @internal
 *  @struct   oyCMMapi10_s_
 *  @brief      Generic data exchange
 *  @ingroup  module_api
 *  @extends  oyCMMapiFilter_s
 */
struct oyCMMapi10_s_ {

/* Include "Struct.members.h" { */
oyOBJECT_e           type_;          /**< @private struct type */
oyStruct_Copy_f      copy;           /**< copy function */
oyStruct_Release_f   release;        /**< release function */
oyObject_s           oy_;            /**< @private features name and hash */

/* } Include "Struct.members.h" */





/* Include "CMMapi.members.h" { */
  oyCMMapi_s     * next;               /**< the next CMM api */

  oyCMMInit_f      oyCMMInit;          /**< */
  oyCMMMessageFuncSet_f oyCMMMessageFuncSet; /**< */

  /** e.g. "sw/oyranos.org/colour.tonemap.imaging/hydra.shiva.CPU.GPU" or "sw/oyranos.org/colour/icc.lcms.CPU",
      see as well @ref registration */
  const char     * registration;

  /** 0: major - should be stable for the live time of a filter, \n
      1: minor - mark new features, \n
      2: patch version - correct errors */
  int32_t          version[3];

  /** 0: last major Oyranos version during development time, e.g. 0
   *  1: last minor Oyranos version during development time, e.g. 0
   *  2: last Oyranos patch version during development time, e.g. 10
   */
  int32_t          module_api[3];

  char           * id_;                /**< @private Oyranos id; keep to zero */

/* } Include "CMMapi.members.h" */

/* Include "CMMapiFilter.members.h" { */
  oyCMMapi5_s_   * api5_;            /**< @private meta module; keep to zero */

/* } Include "CMMapiFilter.members.h" */

/* Include "CMMapi10.members.h" { */
  /** translated, e.g. "help": "createICC" "create a ICC profile" "With ..."
   */
  oyCMMGetText_f   getText;
  /** zero terminated list for ::getText()
   *  include "help", "can_handle" and the supported commands
   */
  char          ** texts;              /**< zero terminated list for getText */

  oyMOptions_Handle_f oyMOptions_Handle; /**< @memberof oyCMMapi10_s */

/* } Include "CMMapi10.members.h" */

};


oyCMMapi10_s_*
  oyCMMapi10_New_( oyObject_s object );
oyCMMapi10_s_*
  oyCMMapi10_Copy_( oyCMMapi10_s_ *cmmapi10, oyObject_s object);
oyCMMapi10_s_*
  oyCMMapi10_Copy__( oyCMMapi10_s_ *cmmapi10, oyObject_s object);
int
  oyCMMapi10_Release_( oyCMMapi10_s_ **cmmapi10 );



/* Include "CMMapi10.private_methods_declarations.h" { */

/* } Include "CMMapi10.private_methods_declarations.h" */



void oyCMMapi10_Release__Members( oyCMMapi10_s_ * cmmapi10 );
int oyCMMapi10_Init__Members( oyCMMapi10_s_ * cmmapi10 );
int oyCMMapi10_Copy__Members( oyCMMapi10_s_ * dst, oyCMMapi10_s_ * src);


#ifdef __cplusplus
} /* extern "C" */
/*}*/ /* namespace oyranos */
#endif /* __cplusplus */

#endif /* OY_CMM_API10_S__H */
