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




