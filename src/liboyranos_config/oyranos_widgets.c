/** @file oyranos_widgets.c
 *
 *  Oyranos is an open source Color Management System 
 *
 *  Copyright (C) 2008-2018  Kai-Uwe Behrmann
 *
 */

/**
 *  @brief    widget APIs
 *  @internal
 *  @author   Kai-Uwe Behrmann <ku.b@gmx.de>
 *  @license: new BSD <http://www.opensource.org/licenses/BSD-3-Clause>
 *  @since    2008/02/09
 */

#include "oyranos_texts.h"
#include "oyranos_version.h"

#include <string.h>

void oyUiFill                        ( openiccUi_s         * ui,
                                       const char          * nick,
                                       const char          * name,
                                       const char          * description,
                                       const char          * icon,
                                       const char          * documentation )
{
  openiccUiHeaderSection_s s[] = {
    /* type,  nick,      label,name,                 description */
    { "oihs", "version", NULL, OYRANOS_VERSION_NAME, NULL },
    { "oihs", "manufacturer", NULL, "Kai-Uwe Behrmann", "http://www.oyranos.org" },
    { "oihs", "copyright", NULL, "Copyright 2018 Kai-Uwe Behrmann", NULL },
    { "oihs", "license", NULL, "newBSD", "http://www.oyranos.org" },
    { "oihs", "url", NULL, "http://www.oyranos.org", NULL },
    { "oihs", "support", NULL, "https://www.github.com/oyranos-cms/oyranos/issues", NULL },
    { "oihs", "download", NULL, "http://www.oyranos.org", NULL },
    { "oihs", "sources", NULL, "http://www.oyranos.org", NULL },
    { "oihs", "development", NULL, "https://github.com/oyranos-cms/oyranos", NULL },
    { "oihs", "openicc_module_author", NULL, "Kai-Uwe Behrmann", "http://www.behrmann.name" },
    { "oihs", "documentation", NULL, "http://www.openicc.info", documentation },
    { "", NULL, NULL, NULL, NULL }
  };
  ui->app_type = "tool";
  memcpy( ui->nick, nick, 4 );
  ui->name = name;
  ui->description = description;
  ui->logo = icon;
  ui->sections = malloc(sizeof(s));
  memcpy( ui->sections, s, sizeof(s) );
}


