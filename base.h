/*
 * checks.h
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

#ifndef POLARBEAR_BASE_H
#define POLARBEAR_BASE_H


#include "jvmti.h"


/* Global static data */
typedef struct {
  jboolean vmDeathCalled;
  jboolean dumpInProgress;
  int totalCount;

  char *optionsCopy;
  int retainedSizeClassCount;
  char **retainedSizeClasses;
  int timeThreshold;
  int countThreshold;
  int signal;
  int dumpThreads;
  int printHeapHistogram;
} GlobalData;

extern GlobalData *gdata;


void deallocate(jvmtiEnv *jvmti, void *p);

/* Check for NULL pointer error */
#define CHECK_FOR_NULL(ptr)  checkForNull(ptr, __FILE__, __LINE__)

/* Check for JVMTI errors. */
#define CHECK(result)  checkJvmtiError(jvmti, result, __FILE__, __LINE__)


void checkForNull(void *ptr, const char *file, const int line);

void checkJvmtiError(jvmtiEnv *jvmti, jvmtiError err, const char *file, const int line);

#endif
