/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <cstring>
#include "agentcontroller.h"

static AgentController* agentController;
static jrawMonitorID monitorID;

void resourceExhausted(
      jvmtiEnv *jvmti_env,
      JNIEnv *jni_env,
      jint flags,
      const void *reserved,
      const char *description) {
   jvmtiError err;

   err = jvmti_env->RawMonitorEnter(monitorID);
   if (err != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: RawMonitorEnter failed: %d\n", err);
      return;
   }

   agentController->onOOM();

   err = jvmti_env->RawMonitorExit(monitorID);
   if (err != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: RawMonitorExit failed: %d\n", err);
   }
}

int setCallbacks(jvmtiEnv *jvmti) {
   jvmtiError err;

   err = jvmti->CreateRawMonitor("jvmkillMonitor", &monitorID);
   if (err != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: CreateRawMonitor failed: %d\n", err);
      return JNI_ERR;
   }

   jvmtiEventCallbacks callbacks;
   memset(&callbacks, 0, sizeof(callbacks));

   callbacks.ResourceExhausted = &resourceExhausted;

   err = jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
   if (err != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: SetEventCallbacks failed: %d\n", err);
      return JNI_ERR;
   }

   err = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_RESOURCE_EXHAUSTED, NULL);
   if (err != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: SetEventNotificationMode failed: %d\n", err);
      return JNI_ERR;
   }

   return JNI_OK;
}

JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *options, void *reserved)
{
   jvmtiEnv *jvmti;

   jint rc = vm->GetEnv((void **) &jvmti, JVMTI_VERSION);
   if (rc != JNI_OK) {
      fprintf(stderr, "ERROR: GetEnv failed: %d\n", rc);
      return JNI_ERR;
   }
   agentController = new AgentController(jvmti);
   agentController->setup(options);
   return setCallbacks(jvmti);
}
