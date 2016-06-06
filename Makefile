ifndef JAVA_HOME
    $(error JAVA_HOME not set)
endif

ifeq ($(shell uname -s),Darwin)
  INCLUDE= -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/darwin"
  LINK= -Wl,-install_name,libJvmKill
  ASNEEDED=
else
  INCLUDE= -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux"
  LINK= -Wl,--no-as-needed,-soname=libJvmKill -static-libgcc
  ASNEEDED=-Wl,--no-as-needed -Wl,-rpath=$(PWD)
endif

CPPFLAGS=-Wall -Werror -fPIC -std=c++11 -shared -fno-strict-aliasing $(LINK) -fno-omit-frame-pointer $(INCLUDE)
TARGET=libjvmkill.so

.PHONY: all build clean alltests ctests threadtests threadtestbasic threadtest0 threadtest-10-2 memtests memtest0 memtest-10-2

all: build alltests

build:
	@echo "=============================================="
	g++ $(CPPFLAGS) -o $(TARGET) jvmkill.c++ agentcontroller.c++ parametersparser.c++ threshold.c++ killaction.c++ base.c++ memory.c++ heaphistogramaction.c++
	chmod 644 $(TARGET)

clean:
	@echo "=============================================="
	rm -f $(TARGET)
	rm -f *.class
	rm -f *.hprof
	rm -rf *.dSYM
	rm -f *tests

alltests: ctests threadtests memtests

ctests: build thresholdctests killactionctests
	@echo "=============================================="
	gcc -g -Wall -Werror $(INCLUDE) $(ASNEEDED) -ldl -o tests tests.c
	./tests

thresholdctests:
	g++ -g -Wall -Werror $(INCLUDE) -ldl -o thresholdtests thresholdtests.c++ threshold.c++
	./thresholdtests

killactionctests: build
	g++ -g -Wall -Werror $(INCLUDE) -ldl -o killactiontests killactiontests.c++ killaction.c++
	./killactiontests

agentcontrollerctests:
	g++ -g -Wall -Werror $(INCLUDE) -ldl -o agentcontrollertests agentcontrollertests.c++ agentcontroller.c++
	./agentcontrollertests

parameterparserctests:
	g++ -g -Wall -Werror $(INCLUDE) -ldl -o parameterparsertests parametersparsertests.c++ parametersparser.c++
	./parameterparsertests


threadtests: threadtestbasic threadtest0 threadtest-10-2 threadtestpspawn-10-2

threadtest0: build
	@echo "=============================================="
	$(JAVA_HOME)/bin/javac JvmKillTestThreads.java
	!($(JAVA_HOME)/bin/java -Xmx1m \
	    -agentpath:$(PWD)/$(TARGET) \
	    -cp $(PWD) JvmKillTestThreads)

threadtest-10-2: build
	@echo "=============================================="
	$(JAVA_HOME)/bin/javac JvmKillTestThreads.java
	!($(JAVA_HOME)/bin/java -Xmx1m \
	    -agentpath:$(PWD)/$(TARGET)=time=10,count=2,printHeapHistogram=1 \
	    -cp $(PWD) JvmKillTestThreads)

memtests: memtest0 memtest-10-2

memtest0: build
	@echo "=============================================="
	$(JAVA_HOME)/bin/javac JvmKillTest.java
	!($(JAVA_HOME)/bin/java -Xmx5m \
	    -agentpath:$(PWD)/$(TARGET) \
	    -cp $(PWD) JvmKillTest)


memtest-10-2: build
	@echo "=============================================="
	$(JAVA_HOME)/bin/javac JvmKillTest.java
	($(JAVA_HOME)/bin/java -Xmx5m \
	    -agentpath:$(PWD)/$(TARGET)=time=10,count=2 \
	    -cp $(PWD) JvmKillTest)

threadtestbasic: build
	@echo "=============================================="
	$(JAVA_HOME)/bin/javac JvmKillTest2.java
	!($(JAVA_HOME)/bin/java \
	    -agentpath:$(PWD)/$(TARGET) \
	    -cp $(PWD) JvmKillTest2)

threadtestpspawn-10-2: build
	@echo "=============================================="
	$(JAVA_HOME)/bin/javac JvmKillTestThreadsParallel.java
	!($(JAVA_HOME)/bin/java \
	    -agentpath:$(PWD)/$(TARGET)=time=10,count=2 \
	    -cp $(PWD) JvmKillTestThreadsParallel)
