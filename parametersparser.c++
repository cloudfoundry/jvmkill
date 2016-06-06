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

#include "parametersparser.h"

enum {
    TIME_OPT = 0,
    COUNT_OPT,
    PRINT_HEAP_HISTOGRAM_OPT,
    THE_END
};

char *tokens[] = {
    [TIME_OPT] = strdup("time"),
    [COUNT_OPT] = strdup("count"),
    [PRINT_HEAP_HISTOGRAM_OPT] = strdup("printHeapHistogram"),
    [THE_END] = NULL
};


ParametersParser::ParametersParser() {
}

AgentParameters ParametersParser::parse(char *options) {
  AgentParameters result;
  //sets defaults
  result.count_threshold = 0;
  result.time_threshold = 1;
  result.print_heap_histogram = 0;

  if (options != NULL) {
     // Copy input options since getsubopt modifies its input
     char *subopts = new char[strlen(options) + 1];
     strcpy(subopts, options);

     char *value;

     while (*subopts != '\0') {
        switch (getsubopt (&subopts, tokens, &value)) {
           case COUNT_OPT:
              if (value == NULL)
                 abort ();
              result.count_threshold = atoi (value);
              break;

           case TIME_OPT:
              if (value == NULL)
                 abort ();
              result.time_threshold = atoi (value);
              break;
          case PRINT_HEAP_HISTOGRAM_OPT:
              if (value == NULL)
                 abort ();
              result.print_heap_histogram = atoi (value);
              break;
          default:
              fprintf (stderr, "Unknown suboption '%s'\n", value);
              break;
        }
     }
  }
  return result;
}
