cmake_minimum_required(VERSION 3.15)
project(juicer)

set(CMAKE_CXX_STANDARD 14)

link_libraries(seccomp)
link_libraries(pthread)

add_executable(
        juicer
        main.cpp
        lang/base.h
        lang/gnu_cpp_compiler.cpp lang/gnu_cpp_compiler.h
        lang/java.cpp lang/java.h
        lang/go.cpp lang/go.h
        lang/python2.cpp lang/python2.h
        lang/python3.cpp lang/python3.h
        third_party/argh.h
        sandbox.cpp sandbox.h helper.h
)

