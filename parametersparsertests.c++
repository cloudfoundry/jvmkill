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
#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameters.h"
#include "parametersparser.h"


ParametersParser *parser;

void setup() {
    parser = new ParametersParser();
}

void teardown() {
}


bool testsDefaults() {
	AgentParameters params = parser->parse(strdup(""));
	bool passed = ((params.time_threshold == 1) && (params.count_threshold == 0));
  if (!passed) {
     fprintf(stdout, "testsDefaults FAILED\n");
  }	return passed;
}

bool testsParsesTimeThreshold() {
	AgentParameters params = parser->parse(strdup("time=99"));
	bool passed = (params.time_threshold == 99);
  if (!passed) {
     fprintf(stdout, "testsParsesTimeThreshold FAILED\n");
  }	return passed;
}

bool testsParsesCountThreshold() {
	AgentParameters params = parser->parse(strdup("count=88"));
	bool passed = (params.time_threshold == 88);
  if (!passed) {
     fprintf(stdout, "testsParsesCountThreshold FAILED\n");
  }	return passed;
}

bool testsParsesPrintHeapHistogramOn() {
	AgentParameters params = parser->parse(strdup("printHeapHistogram=1"));
	bool passed = (params.print_heap_histogram == true);
  if (!passed) {
     fprintf(stdout, "testsParsesPrintHeapHistogramOn FAILED\n");
  }	return passed;
}

bool testsParsesPrintHeapHistogramOff() {
	AgentParameters params = parser->parse(strdup("printHeapHistogram=0"));
	bool passed = (params.print_heap_histogram == false);
  if (!passed) {
     fprintf(stdout, "testsParsesPrintHeapHistogramOff FAILED\n");
  }	return passed;
}
//
// bool testConstructionWithParameters() {
// 	Threshold *threshold = new Threshold("time=10,count=5");
// 	bool passed = ((threshold->getCount_Threshold() == 5) && (threshold->getTime_Threshold() == 10));
//     if (!passed) {
//        fprintf(stdout, "testConstructionWithParameters FAILED\n");
//     }	return passed;
// }
//
// bool testConstructionWithNullPointer() {
// 	Threshold *threshold = new Threshold(0);
// 	bool passed = ((threshold->getCount_Threshold() == 0) && (threshold->getTime_Threshold() == 1));
//     if (!passed) {
//        fprintf(stdout, "testConstructionWithNullPointer FAILED\n");
//     }	return passed;
// }
//
// bool testConstructionWithNoParameters() {
// 	Threshold *threshold = new Threshold("");
// 	bool passed = ((threshold->getCount_Threshold() == 0) && (threshold->getTime_Threshold() == 1));
//     if (!passed) {
//        fprintf(stdout, "testConstructionWithNoParameters FAILED\n");
//     }	return passed;
// }


int main() {
	setup();
	bool result = testsDefaults() && testsParsesTimeThreshold() && testsParsesPrintHeapHistogramOn() && testsParsesPrintHeapHistogramOff();
	teardown();
	if (result) {
       fprintf(stdout, "SUCCESS\n");
	   exit(EXIT_SUCCESS);
	}
	else {
       fprintf(stdout, "FAILURE\n");
       exit(EXIT_FAILURE);
	}
}
