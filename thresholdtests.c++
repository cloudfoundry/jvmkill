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
#include <unistd.h> //sleep
#include "parameters.h"
#include "heuristic.h"
#include "threshold.h"

bool testTriggersIfThresholdExceeded() {
	AgentParameters params;
	params.time_threshold=3;
	params.count_threshold=2;
	Heuristic *threshold = new Threshold(params);

	bool passed = !threshold->onOOM() &&
	              !threshold->onOOM() &&
	              threshold->onOOM();
    if (!passed) {
       fprintf(stdout, "testTriggersIfThresholdExceeded FAILED\n");
    }
	return passed;
}

bool testDoesNotTriggerIfThresholdNotExceeded() {
	AgentParameters params;
	params.time_threshold=1;
	params.count_threshold=2;
	Heuristic *threshold = new Threshold(params);

	bool passed = !threshold->onOOM() &&
	              !threshold->onOOM();
  if (!passed) {
     fprintf(stdout, "testTriggersIfThresholdExceeded FAILED\n");
		 return passed;
  }
	sleep(2);
	passed = !threshold->onOOM() &&
	              !threshold->onOOM();
	if (!passed) {
	  fprintf(stdout, "testTriggersIfThresholdExceeded FAILED\n");
	}
	return passed;
}


int main() {
	bool result = (testTriggersIfThresholdExceeded() &&
		           testDoesNotTriggerIfThresholdNotExceeded());
	if (result) {
       fprintf(stdout, "SUCCESS\n");
	   exit(EXIT_SUCCESS);
	}
	else {
    	fprintf(stdout, "FAILURE\n");
    	exit(EXIT_FAILURE);
	}
}
