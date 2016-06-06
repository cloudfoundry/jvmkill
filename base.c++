/*
 * checks.cc
 *
 * Originally based on http://hope.nyc.ny.us/~lprimak/java/demo/jvmti/heapViewer/
 *
 * Original source is Copyright (c) 2004 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Modifications are Copyright (c) 2011 The PolarBear Authors. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * -Redistribution of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 * Neither the name of Sun Microsystems, Inc. or the names of contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN MIDROSYSTEMS, INC. ("SUN")
 * AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE
 * AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE,
 * EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that this software is not designed, licensed or intended
 * for use in the design, construction, operation or maintenance of any
 * nuclear facility.
 */

#include "base.h"

#include <stdio.h>
#include <stdlib.h>


GlobalData globalData, *gdata = &globalData;



/* Deallocate JVMTI memory */
void deallocate(jvmtiEnv *jvmti, void *p) {
  jvmtiError err = jvmti->Deallocate((unsigned char *)p);
  if (err != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: JVMTI Deallocate error err=%d\n", err);
      abort();
  }
}


/* Check for NULL pointer error */
#define CHECK_FOR_NULL(ptr)  checkForNull(ptr, __FILE__, __LINE__)

void checkForNull(void *ptr, const char *file, const int line) {
  if (ptr == NULL) {
    fprintf(stderr, "ERROR: NULL pointer error in %s:%d\n", file, line);
    abort();
  }
}


/* Check for JVMTI errors. */
#define CHECK(result)  checkJvmtiError(jvmti, result, __FILE__, __LINE__)

static char * getErrorName(jvmtiEnv *jvmti, jvmtiError errnum) {
  jvmtiError err;
  char      *name;

  err = jvmti->GetErrorName(errnum, &name);
  if (err != JVMTI_ERROR_NONE) {
    fprintf(stderr, "ERROR: JVMTI GetErrorName error err=%d\n", err);
    abort();
  }
  return name;
}

void checkJvmtiError(jvmtiEnv *jvmti, jvmtiError err, const char *file, const int line) {
  if (err != JVMTI_ERROR_NONE) {
    char *name;

    name = getErrorName(jvmti, err);
    fprintf(stderr, "ERROR: JVMTI error err=%d(%s) in %s:%d\n",
            err, name, file, line);
    deallocate(jvmti, name);
    abort();
  }
}
