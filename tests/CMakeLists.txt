cmake_minimum_required(VERSION 3.15)
project(juicer-tests)

set(CMAKE_CXX_STANDARD 14)

link_libraries(seccomp)

add_executable(test-runtime-huge-memory runtime-huge-memory.cpp)
add_executable(test-runtime-timeout runtime-timeout.cpp)
add_executable(test-runtime-infinite-stack runtime-infinite-stack.cpp)
add_executable(test-runtime-fork-bomb runtime-fork-bomb.cpp)
add_executable(test-runtime-fork-bomb2 runtime-fork-bomb2.cpp)
add_executable(test-runtime-system-shutdown runtime-system-shutdown.cpp)
add_executable(test-runtime-many-files runtime-many-files.cpp)
add_executable(test-runtime-system-killall runtime-system-killall.cpp)



# !!! never use it !!!
# add_executable(test-compile-timeout compile-timeout.cpp)
# add_executable(test-compile-timeout2 compile-timeout2.cpp)
add_executable(test-compile-huge-binary compile-huge-binary.cpp)
# add_executable(test-compile-multidefine compile-multidefine.cpp)

add_executable(test-normal normal.cpp)