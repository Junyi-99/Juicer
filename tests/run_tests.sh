#!/bin/bash

./juicer --language=cpp --sourcecode=./compile-huge-binary.cpp
./juicer --language=cpp --sourcecode=./compile-huge-error-output.cpp
./juicer --language=cpp --sourcecode=./compile-multidefine.cpp
./juicer --language=cpp --sourcecode=./compile-sensitive-data.cpp
./juicer --language=cpp --sourcecode=./compile-timeout.cpp
./juicer --language=cpp --sourcecode=./compile-timeout2.cpp
#./juicer --language=cpp --sourcecode=./runtime-alarm.cpp
#./juicer --language=cpp --sourcecode=./runtime-fork-bomb.cpp
#./juicer --language=cpp --sourcecode=./runtime-fork-bomb2.cpp
#./juicer --language=cpp --sourcecode=./runtime-fork-bomb3.cpp
#./juicer --language=cpp --sourcecode=./runtime-huge-memory.cpp
#./juicer --language=cpp --sourcecode=./runtime-infinite-stack.cpp
#./juicer --language=cpp --sourcecode=./runtime-many-files.cpp
#./juicer --language=cpp --sourcecode=./runtime-mmap-asm.cpp
#./juicer --language=cpp --sourcecode=./runtime-system-killall.cpp
#./juicer --language=cpp --sourcecode=./runtime-system-shutdown.cpp
#./juicer --language=cpp --sourcecode=./runtime-timeout.cpp


mv /tmp/tmp.7TpESdgFAE/cmake-build-debug/juicer /tmp/tmp.7TpESdgFAE/tests; ./juicer --run-time-limit=1000 --case-in=1.in,2.in --case-out=1.out,2.out --language=cpp --sourcecode=./normal.cpp

