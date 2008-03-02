/**
 * Oyranos is an open source Colour Management System 
 * 
 * Copyright (C) 2004-2005  Kai-Uwe Behrmann
 *
 * @autor: Kai-Uwe Behrmann <ku.b@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * -----------------------------------------------------------------------------
 *
 * API
 * 
 */

/** @date      25. 11. 2004 */


#ifndef OYRANOS_H
#define OYRANOS_H

#include <stdlib.h> // for linux size_t
#include "oyranos_definitions.h"

/** \namespace oyranos
    The Oyranos namespace.
 */
#ifdef __cplusplus
extern "C" {
namespace oyranos
{
#endif /* __cplusplus */

/** * kdb stuff */
void  oyOpen                (void);
void  oyClose               (void);

/**
 * @param[in] size the size to allocate
 *
 * Place here your allocator, like: \code new char [size]; \endcode<br>
 */
typedef void* (*oyAllocFunc_t)         (size_t size);

/**
 * @param[in] data the pointer to deallocate
 *
 * Place here your deallocator, like: \code delete [] data; \endcode
 */
//typedef void (*oyDeAllocFunc_t)        (void *data);


/* path names */

int   oyPathsCount                     (void);
char* oyPathName                       (int         number,
                                        oyAllocFunc_t);
int   oyPathAdd                        (const char* pathname);
void  oyPathRemove                     (const char* pathname);
void  oyPathSleep                      (const char* pathname);
void  oyPathActivate                   (const char* pathname);
char* oyGetPathFromProfileName         (const char* profile_name,
                                        oyAllocFunc_t);

/* --- default profiles --- */

/** enum default profiles */
typedef enum  {
  oyWORKSPACE,            /**< Workspace Profile or Editing Profile */
  oyINPUT_XYZ,            /**< standard XYZ input profile */
  oyINPUT_Lab,            /**< standard Lab input profile */
  oyINPUT_RGB,            /**< standard RGB input profile */
  oyINPUT_Cmyk,           /**< standard Cmyk input profile */
  oyDEFAULT_PROFILE_TYPES /**< just for easen Gui design */
} oyDEFAULT_PROFILE;

int         oySetDefaultProfile        (oyDEFAULT_PROFILE type,
                                        const char*       file_name);
int         oySetDefaultProfileBlock   (oyDEFAULT_PROFILE type,
                                        const char*       file_name,
                                        void*             mem,
                                        size_t            size);
const char* oyGetDefaultProfileUITitle (oyDEFAULT_PROFILE type);
char*       oyGetDefaultProfileName    (oyDEFAULT_PROFILE type,
                                        oyAllocFunc_t     alloc_func);

int   oySetDefaultWorkspaceProfile     (const char* name);
int   oySetDefaultWorkspaceProfileBlock(const char* name, void* mem, size_t size);
int   oySetDefaultXYZInputProfile      (const char* name);
int   oySetDefaultXYZInputProfileBlock (const char* name, void* mem, size_t size);
int   oySetDefaultLabInputProfile      (const char* name);
int   oySetDefaultLabInputProfileBlock (const char* name, void* mem, size_t size);
int   oySetDefaultRGBInputProfile      (const char* name);
int   oySetDefaultRGBInputProfileBlock (const char* name, void* mem, size_t size);
int   oySetDefaultCmykInputProfile     (const char* name);
int   oySetDefaultCmykInputProfileBlock(const char* name, void* mem, size_t size);

char* oyGetDefaultWorkspaceProfileName (oyAllocFunc_t);
char* oyGetDefaultXYZInputProfileName  (oyAllocFunc_t);
char* oyGetDefaultLabInputProfileName  (oyAllocFunc_t);
char* oyGetDefaultRGBInputProfileName  (oyAllocFunc_t);
char* oyGetDefaultCmykInputProfileName (oyAllocFunc_t);

/** --- profile lists --- */

char**oyProfileList                    (const char* colourspace, size_t * size);

/** --- profile checking --- */

int   oyCheckProfile                   (const char* name, int flag);
int   oyCheckProfileMem                (const void* mem, size_t size, int flags);


/** --- profile access through oyranos --- */

size_t oyGetProfileSize                (const char* profilename);
void*  oyGetProfileBlock               (const char* profilename, size_t* size,
                                        oyAllocFunc_t);


/* device profiles */
/** enum identifying device types for distinguishing in searches */
typedef enum  {
  /*oyNOTYPE,*/
  oyDISPLAY,          /**< dynamic viewing */
  oyPRINTER,          /**< static media (dye, ink, offset, imagesetters) */
  oySCANNER,          /**< contact digitiser */
  oyCAMERA,           /**< virtual or contactless image capturing */
} oyDEVICETYP;


char* oyGetDeviceProfile                  (oyDEVICETYP typ,
                                           const char* manufacturer,
                                           const char* model,
                                           const char* product_id,
                                           const char* host,
                                           const char* port,
                                           const char* attrib1,
                                           const char* attrib2,
                                           const char* attrib3,
                                           oyAllocFunc_t);

int	oySetDeviceProfile                    (oyDEVICETYP typ,
                                           const char* manufacturer,
                                           const char* model,
                                           const char* product_id,
                                           const char* host,
                                           const char* port,
                                           const char* attrib1,
                                           const char* attrib2,
                                           const char* attrib3,
                                           const char* profilename,
                                           void* mem,
                                           size_t size);
int oyEraseDeviceProfile                  (oyDEVICETYP typ,
                                           const char* manufacturer,
                                           const char* model,
                                           const char* product_id,
                                           const char* host,
                                           const char* port,
                                           const char* attrib1,
                                           const char* attrib2,
                                           const char* attrib3);
#if 0
int	oySetProfileProperty                  (char* profilename,
                                           char* property,
                                           char* value);
#endif


#ifdef __cplusplus
} // extern "C"
} // namespace oyranos
#endif /* __cplusplus */

#endif /* OYRANOS_H */
