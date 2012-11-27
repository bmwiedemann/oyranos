/** @file oyranos_io.c
 *
 *  Oyranos is an open source Colour Management System 
 *
 *  @par Copyright:
 *            2004-2011 (C) Kai-Uwe Behrmann
 *
 *  @brief    input / output  methods
 *  @internal
 *  @author   Kai-Uwe Behrmann <ku.b@gmx.de>
 *  @par License:
 *            new BSD <http://www.opensource.org/licenses/bsd-license.php>
 *  @since    2004/11/25
 */

#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "config.h"
#include "oyranos.h"
#include "oyranos_check.h"
#include "oyranos_debug.h"
#include "oyranos_elektra.h"
#include "oyranos_helper.h"
#include "oyranos_internal.h"
#include "oyranos_io.h"
#include "oyranos_sentinel.h"
#include "oyranos_string.h"
#include "oyranos_xml.h"

/* --- Helpers  --- */

/* --- static variables   --- */

/* --- structs, typedefs, enums --- */

/* --- internal API definition --- */

/* search in profile path and in current path */
char* oyFindProfile_ (const char* name);



/* --- Helpers  --- */
/* small helpers */
#define WARNc_PROFILE_S(text_,fileName_) \
      { \
        int l_1 = oyStrlen_(text_); \
        int l_2 = oyStrlen_(oyNoEmptyName_m_(fileName_)); \
        char * tmp = oyAllocateFunc_ (l_1 + l_2 + 12); \
        memcpy(tmp, text_, l_1); \
        memcpy(&tmp[l_1], " ", 1); \
        memcpy(&tmp[l_1+1], oyNoEmptyName_m_(fileName_), l_2); \
        tmp[l_1+1+l_2] = 0; \
 \
        WARNc_S( tmp ); \
        oyDeAllocateFunc_(tmp); \
      }



/* --- function definitions --- */


int
oyGetPathFromProfileNameCb_          ( oyFileList_s      * data,
                                       const char        * full_name,
                                       const char        * filename )
{
  int success = 0;
  oyFileList_s * l = data;
  char * search = l->names[0];
  const char * name = 0;
  int l1 = strlen(full_name),
      l2 = strlen(search),
      len = 0;

  if(l1 > l2)
  {
    len = l1 - l2;
    if(full_name[len-1] == OY_SLASH_C)
      name = &full_name[len];
    else
      name = filename;
  }

  DBG_MEM_S( search )
  if(name && l2 && strcmp( search, name ) == 0)
  {
    size_t size = 128;
    char* header = oyReadFileToMem_ (full_name, &size, oyAllocateFunc_);
    success = !oyCheckProfileMem_ (header, size, 0);
    oyFree_m_ (header);
    if (success) {
      DBG_MEM_S(full_name)
      DBG_MEM_V(oyStrlen_(full_name))
      if (oyStrlen_(full_name) < MAX_PATH) {
        oySprintf_( search, "%s", full_name );
        search[oyStrlen_(full_name)] = '\000';
      } else
        search[0] = '\000';
    } else
      WARNc_PROFILE_S( _("not a profile:"), oyNoEmptyName_m_(full_name) )
  }
  /* break on success */
  DBG_MEM_V(success)
  return success;
}

char*
oyGetPathFromProfileName_       (const char*   fileName,
                                 oyAlloc_f     allocate_func)
{
  char  *fullFileName = 0;
  char  *pathName = 0;
  int    success = 0;
  char  *header = 0;
  size_t    size;

  DBG_PROG_START

  if(fileName && fileName[0] && strlen(fileName) > 7 &&
     memcmp(fileName, "file://", 7) == 0)
    fileName = &fileName[7];

  /*DBG_NUM_S((fileName)) */
  /* search in configured paths */
  if (fileName && fileName[0] != OY_SLASH_C)
  {
    char search[MAX_PATH];
    int count = 0;
    size_t len = (oyStrlen_(fileName) < MAX_PATH) ? 
                          oyStrlen_(fileName) : MAX_PATH;
    char ** path_names = oyProfilePathsGet_( &count, oyAllocateFunc_ );
    char * l_names[2] = { 0, 0 };
    oyFileList_s l = {oyOBJECT_FILE_LIST_S_, 0, NULL, 0, 0, 0};

    l_names[0] = search;
    l.names = l_names;

    DBG_PROG

    if(oyStrlen_(fileName) < MAX_PATH)
    {
      memcpy(search, fileName, len); search[len] = '\000';
    } else {
      WARNc2_S( "%s %d", _("name longer than"), MAX_PATH)
      DBG_PROG_ENDE
      return 0;
    }
    success = oyRecursivePaths_( oyGetPathFromProfileNameCb_, &l,
                                 (const char**)path_names, count );

    oyStringListRelease_( &path_names, count, oyDeAllocateFunc_ );

      if (success) { /* found */
        len = 0;
        DBG_PROG_S((search))
        if(search[0] != 0) len = oyStrlen_(search);
        if(len) {
          char *ptr = 0;
          oyAllocHelper_m_( pathName, char, len+1, allocate_func, return 0 );
          oyStrcpy_(pathName, search);
          ptr = oyStrrchr_(pathName , OY_SLASH_C);
          if(ptr)
            ptr[0] = '\000';
        }
        DBG_PROG_S( pathName )
        DBG_PROG_ENDE
        return pathName;
      } else

    if (!success && !strchr(fileName, OY_SLASH_C)) {
      if(oy_warn_)
        WARNc_PROFILE_S(_("profile not found in colour path:"), fileName);
      DBG_PROG_ENDE
      return 0;
    }
  }

  if(fileName && !success)
  {/* use fileName as an full qualified name, check name and test profile*/
    DBG_PROG_S("dir/filename found")
    fullFileName = oyMakeFullFileDirName_ (fileName);

    if (oyIsFileFull_(fullFileName,"rb"))
    {
      size = 128;
      header = oyReadFileToMem_ (fullFileName, &size, allocate_func);

      if (size >= 128)
        success = !oyCheckProfileMem_ (header, 128, 0);
    }

    if (!success) {
      WARNc_PROFILE_S( _("profile not found:"), oyNoEmptyName_m_(fileName))
      DBG_PROG_ENDE
      return 0;
    }

    pathName = oyExtractPathFromFileName_(fullFileName);

    oyFree_m_ (header);
  }

  if (!success)
  { oyFree_m_ (pathName);
    pathName = 0;
  }

  oyFree_m_ (fullFileName);

  DBG_PROG_ENDE
  return pathName;
}


char **
oyProfilePathsGet_    (int             * count,
                       oyAlloc_f         allocateFunc)
{
  char ** path_names = NULL;
#if 1
  char ** tmp = NULL;
  int tmp_n = -1;

  path_names = oyDataPathsGet_( count, "color/icc", oyALL, oyUSER_SYS,
                                          oyAllocateFunc_ );
# if defined(__APPLE__)
# define TestAndSetDefaultPATH( path ) \
  if(oyIsDir_( path )) \
    oyStringListAddStaticString_ ( &path_names, count, path, oyAllocateFunc_, \
                                   oyDeAllocateFunc_ );

  /* Apples ColorSync default paths */

# define CSSystemPATH        "/System/Library/ColorSync/Profiles"
# define CSGlobalInstallPATH "/Library/ColorSync/Profiles"
# define CSUserPATH          "~/Library/ColorSync/Profiles"
# define CSNetworkPath       "/Network/Library/ColorSync/Profiles"
  TestAndSetDefaultPATH( CSSystemPATH );
  TestAndSetDefaultPATH( CSGlobalInstallPATH );
  TestAndSetDefaultPATH( CSUserPATH );
  TestAndSetDefaultPATH( CSNetworkPath );

# undef TestAndSetDefaultPATH
# endif
  tmp = oyStringListAppend_( 0, 0, (const char**)path_names, *count,
                             &tmp_n, allocateFunc );
  oyStringListRelease_( &path_names, *count,  oyDeAllocateFunc_ );
  path_names = tmp; tmp = 0;
  *count = tmp_n;

#else
  int i,
      n = 0;
  int c = oyPathsCount_();

  oyAllocHelper_m_ (path_names, char*, c, allocateFunc, return NULL);

  for( i = 0; i < c; ++i)
  {
    char * test = oyPathName_( i, oyAllocateFunc_ );
    char * checked = oyMakeFullFileDirName_( test );

    if( checked )
    {
      path_names[n] = allocateFunc( oyStrblen_(checked) + 1 );
      oySprintf_( path_names[n], "%s", checked );
      oyFree_m_( checked );
      ++n;
    }
    oyFree_m_( test );
  }

  *count = n;
#endif
  return path_names;
}


char*
oyFindProfile_ (const char* fileName)
{
  char  *fullFileName = 0;
  char* path_name = 0;

  DBG_PROG_START

  if(!fileName || !fileName[0])
    return fullFileName;

  /*DBG_NUM_S((fileName)) */
  if (fileName && fileName[0] != OY_SLASH_C)
  {
    path_name = oyGetPathFromProfileName_(fileName, oyAllocateFunc_);

    if(!path_name)
    {
      FILE * fp = fopen( fileName, "rb" );
      if(fp)
      {
        path_name = oyStringCopy_( "./", oyAllocateFunc_ );
        fclose(fp); fp = 0;
      }
    }

    DBG_PROG
    if(path_name)
    {
      oyAllocString_m_( fullFileName, MAX_PATH,
                        oyAllocateFunc_, return 0 );
      if(strrchr(fileName,OY_SLASH_C) == NULL)
        oySprintf_(fullFileName, "%s%s%s", path_name, OY_SLASH, fileName);
      else
        oySprintf_( fullFileName, "%s%s%s", path_name, OY_SLASH,
                    strrchr( fileName, OY_SLASH_C ) + 1 );
    } else
    {
      DBG_PROG_ENDE
      return NULL;
    }
    DBG_PROG_S( fullFileName )
  }


  if(!path_name)
  {
    if (oyIsFileFull_(fileName,"rb")) {
      fullFileName = oyStringCopy_( fileName, oyAllocateFunc_ );
    } else
      fullFileName = oyMakeFullFileDirName_ (fileName);
  }

  if(path_name)
    oyFree_m_(path_name)

  DBG_PROG_ENDE
  return fullFileName;
}




/* public API implementation */

/* profile and other file lists API */

int oyProfileListCb_ (oyFileList_s * data,
                      const char* full_name, const char* filename)
{
  oyFileList_s *l = (oyFileList_s*)data;

  if(l->type != oyOBJECT_FILE_LIST_S_)
    WARNc_S("Could not find a oyFileList_s objetc.");

      if (!oyCheckProfile_(full_name, l->coloursig))
      {
        if(l->count_files >= l->mem_count)
        {
          char** temp = l->names;

          l->names = 0;
          oyAllocHelper_m_( l->names, char*, l->mem_count+l->hopp,
                            oyAllocateFunc_, return 1);
          memcpy(l->names, temp, sizeof(char*) * l->mem_count);
          l->mem_count += l->hopp;
        }

        oyAllocString_m_( l->names[l->count_files], oyStrblen_(full_name) + 1,
                          oyAllocateFunc_, return 1 );
        strcpy(l->names[l->count_files], full_name);
        ++l->count_files;
      }

  return 0;
}

int oyPolicyListCb_ (oyFileList_s * data,
                     const char* full_name, const char* filename)
{
  oyFileList_s *l = (oyFileList_s*)data;
  /* last 4 chars */
  const char * end = NULL;

  if(l->type != oyOBJECT_FILE_LIST_S_)
    WARNc_S("Could not find a oyFileList_s objetc.");

  if(strlen(full_name) > 4)
    end = full_name + strlen(full_name) - 4;

      if( (end[0] == '.') &&
          (end[1] == 'x' || end[1] == 'X') &&
          (end[2] == 'm' || end[2] == 'M') &&
          (end[3] == 'l' || end[3] == 'L') &&
          !oyCheckPolicy_(full_name) )
      {
        if(l->count_files >= l->mem_count)
        {
          char** temp = l->names;

          l->names = 0;
          oyAllocHelper_m_( l->names, char*, l->mem_count+l->hopp,
                            oyAllocateFunc_, return 1);
          memcpy(l->names, temp, sizeof(char*) * l->mem_count);
          l->mem_count += l->hopp;
        }

        oyAllocString_m_( l->names[l->count_files], oyStrblen_(full_name),
                          oyAllocateFunc_, return 1 );
        
        oyStrcpy_(l->names[l->count_files], full_name);
        ++l->count_files;
      } /*else */
        /*WARNc_S(("%s in %s is not a valid profile", file_name, path)); */
  return 0;
}


char **  oyProfileListGet_           ( const char        * coloursig,
                                       uint32_t          * size )
{
  oyFileList_s l = {oyOBJECT_FILE_LIST_S_, 128, NULL, 128, 0, 0};
  int32_t count = 0;/*oyPathsCount_();*/
  char ** path_names = NULL;

  path_names = oyProfilePathsGet_( &count, oyAllocateFunc_ );

  l.coloursig = coloursig;

  DBG_PROG_START
 
  oy_warn_ = 0;

  l.names = 0;
  l.mem_count = l.hopp;
  l.count_files = 0;

  oyAllocHelper_m_(l.names, char*, l.mem_count, oyAllocateFunc_, return 0);

  oyRecursivePaths_( oyProfileListCb_, &l,
                     (const char**)path_names, count );

  oyStringListRelease_( &path_names, count, oyDeAllocateFunc_ );

  *size = l.count_files;
  oy_warn_ = 1;
  DBG_PROG_ENDE
  return l.names;
}

char**
oyPolicyListGet_                  (int * size)
{
  oyFileList_s l = {oyOBJECT_FILE_LIST_S_, 128, NULL, 128, 0, 0};
  int count = 0;
  char ** path_names = NULL;
  DBG_PROG_START
 
  path_names = oyDataPathsGet_( &count, "color/settings", oyALL, oyUSER_SYS,
                                oyAllocateFunc_ );

  oy_warn_ = 0;

  l.names = 0;
  l.mem_count = l.hopp;
  l.count_files = 0;

  oyAllocHelper_m_(l.names, char*, l.mem_count, oyAllocateFunc_, return 0);

  oyRecursivePaths_(oyPolicyListCb_, &l,(const char**)path_names, count);

  oyStringListRelease_(&path_names, count, oyDeAllocateFunc_);

  *size = l.count_files;
  oy_warn_ = 1;
  DBG_PROG_ENDE
  return l.names;
}

/* profile handling API */
size_t
oyReadFileSize_(const char* name);

size_t
oyGetProfileSize_                  (const char* profilename)
{
  size_t size = 0;
  char* fullFileName = oyFindProfile_ (profilename);

  DBG_PROG_START

  size = oyReadFileSize_ (fullFileName);

  DBG_PROG_ENDE
  return size;
}

void*
oyGetProfileBlock_                 (const char* profilename, size_t *size,
                                    oyAlloc_f     allocate_func)
{
  char* fullFileName = 0;
  char* block = 0;

  DBG_PROG_START

  fullFileName = oyFindProfile_ (profilename);

  if(fullFileName)
  {
    block = oyReadFileToMem_ (fullFileName, size, allocate_func);
    oyFree_m_( fullFileName );
  }

  DBG_PROG_ENDE
  return block;
}


