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
#include <stdio.h>
#include <stdlib.h>

#include <jvmti.h>

#include "agentcontroller.h"
#include "parameters.h"
#include "heuristic.h"
#include "threshold.h"
#include "killaction.h"
#include "heaphistogramaction.h"
#include "action.h"

static int ActionRunCounter = 0;

static int MockPrintHeapActionCount = 0;
static int MockKillActionRunOrder = -1;
static int MockPrintHeapActionRunOrder = -1;
static int MockKillActionCount = 0;
static int MockThresholdEventCount = 0;
AgentController* agentController;
jvmtiEnv* jvm;
/************************************************
 *  mocks
 ************************************************/
HeapHistogramAction::HeapHistogramAction(jvmtiEnv* jvm) {
	MockPrintHeapActionCount++;
}
void HeapHistogramAction::act() {
	MockPrintHeapActionRunOrder = ActionRunCounter++;
}

//KillAction

KillAction::KillAction() {
	MockKillActionCount++;
}
void KillAction::act() {
	MockKillActionRunOrder = ActionRunCounter++;
}

//Threshold
Threshold::Threshold(AgentParameters param) {
}

long Threshold::getMillisLimit() {
	return 0;
}

void Threshold::addEvent() {
}

int Threshold::countEvents() {
	return 0;
}

bool Threshold::onOOM() {
   return ++MockThresholdEventCount > 1;
}

//end of mocks

void setup() {
	agentController = new AgentController(NULL);
}

void teardown() {
	delete(agentController);
}
bool testAlwaysAddsKillAction() {
	setup();
	AgentParameters params;
	params.print_heap_histogram = true;
	agentController->setParameters(params);
	bool passed = (MockKillActionCount == 1);
  if (!passed) {
     fprintf(stdout, "testAlwaysAddsKillAction FAILED\n");
  }
	teardown();
	return passed;
}


bool testDoesNotAddHeapActionWhenOff() {
	setup();
	AgentParameters params;
	params.print_heap_histogram = false;
	agentController->setParameters(params);
  bool passed = (MockPrintHeapActionCount == 0);
  if (!passed) {
     fprintf(stdout, "testDoesNotAddHeapActionWhenOff FAILED\n");
  }
	teardown();
	return passed;
}

bool testAddsHeapActionWhenOn() {
	setup();
	AgentParameters params;
	params.print_heap_histogram = true;
	agentController->setParameters(params);
	bool passed = (MockPrintHeapActionCount == 1);
  if (!passed) {
     fprintf(stdout, "testAddsHeapActionWhenOn FAILED\n");
  }
	teardown();
	return passed;
}

bool testRunsAllActionsInCorrectOrderOnOOM() {
	setup();
	AgentParameters params;
	params.print_heap_histogram = true;
	agentController->setParameters(params);

	//MockThreshold returns true for OOM on second attempt, therefore should not
	//run actions on first call
	agentController->onOOM();
	bool firstAssertions = ((MockPrintHeapActionRunOrder == -1) &&
	               (MockKillActionRunOrder == -1) &&
								 (MockThresholdEventCount == 1));

	agentController->onOOM();
  bool passed = ((MockPrintHeapActionRunOrder == 0) &&
	               (MockKillActionRunOrder == 1) &&
								 (MockThresholdEventCount > 1) &&
							   (firstAssertions));
  fprintf(stdout, "%d\n", MockPrintHeapActionRunOrder);
  if (!passed) {
    fprintf(stdout, "testRunsAllActionsInCorrectOrderOnOOM FAILED\n");
  }
	teardown();
  return passed;
}


int main() {
	bool result = (testDoesNotAddHeapActionWhenOff() &&
								 testAddsHeapActionWhenOn() &&
							   testRunsAllActionsInCorrectOrderOnOOM());
	if (result) {
       fprintf(stdout, "SUCCESS\n");
	   exit(EXIT_SUCCESS);
	}
	else {
    	fprintf(stdout, "FAILURE\n");
    	exit(EXIT_FAILURE);
	}
}
