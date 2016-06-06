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

#include "parameters.h"
#include "threshold.h"
#include "heuristic.h"

#ifdef __MACH__
#include <mach/mach_time.h>
#define CLOCK_MONOTONIC 0
int clock_gettime(int clk_id, struct timespec *t){
    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);
    uint64_t time;
    time = mach_absolute_time();
    t->tv_sec = ((double)time * (double)timebase.numer)/((double)timebase.denom * 1e9);
    t->tv_nsec = ((double)time * (double)timebase.numer)/((double)timebase.denom);
    return 0;
}
#else
#include <time.h>
#endif

Threshold::Threshold(AgentParameters param) {
   parameters = param;
   eventIndex = 0;
   events = new long[parameters.count_threshold + 1];
   //prefill with a safe value
   for (int i=0; i <= parameters.count_threshold; i++) {
          events[i]=0;
   }

}

static long getTimeMillis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec*1000)+(ts.tv_nsec/1000000);
}

long Threshold::getMillisLimit() {
   return getTimeMillis()-parameters.time_threshold*1000;
}

void Threshold::addEvent() {
   events[eventIndex]=getTimeMillis();
   if (++eventIndex > parameters.count_threshold) {
      eventIndex=0;
   }
}

int Threshold::countEvents() {
   long millisLimit = getMillisLimit();
   int count = 0;
   for (int i=0;i <= parameters.count_threshold;i++) {
      if (events[i] != 0 && events[i]>=millisLimit) {
     	   count++;
       }
   }
   return count;
}

bool Threshold::onOOM() {
   addEvent();
   int eventCount = countEvents();
   fprintf(stderr, "ResourceExhausted! (%d/%d)\n", eventCount, parameters.count_threshold);
   return eventCount > parameters.count_threshold;
}
