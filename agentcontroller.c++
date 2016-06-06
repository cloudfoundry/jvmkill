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

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "threshold.h"
#include "agentcontroller.h"
#include "heaphistogramaction.h"
#include "killaction.h"
#include "parametersparser.h"

#define MAX_ACTIONS 2

AgentController::AgentController(jvmtiEnv* jvm) {
  jvmti = jvm;
  actionCount = 0;
  actions = new Action*[MAX_ACTIONS];
}

void AgentController::onOOM() {
  if (heuristic->onOOM()) {
    for (int i=0;i<actionCount;i++) {
      actions[i]->act();
    }
  }
}

void AgentController::setup(char *options) {
  ParametersParser* parser = new ParametersParser();
  setParameters(parser->parse(options));
}
void AgentController::setParameters(AgentParameters parameters) {
  heuristic = new Threshold(parameters);
  if (parameters.print_heap_histogram) {
      actions[actionCount++] = new HeapHistogramAction(jvmti);
  }
  actions[actionCount++] = new KillAction();

}
