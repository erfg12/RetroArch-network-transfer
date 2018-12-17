/*  
 *  Created by Jacob Fliss 2018
 */


#ifndef __RARCH_CLOUD_UPLOAD_H
#define __RARCH_CLOUD_UPLOAD_H

#include "../../deps/mbedtls/mbedtls/base64.h"
#include <net/net_http.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

RETRO_BEGIN_DECLS

void cloud_upload(const char *file, char *data, size_t size);
void cloud_download();
void store_download_cb(void *task_data, void *user_data, const char *error);

RETRO_END_DECLS

#endif
