//
// Created by Junyi Hou on 2/2/21.
//

#include <fstream>
#include "gnu_cpp_compiler.h"


int JuicerLang::GNU_cpp_compiler::compile(const string &source_code) {
    printf("GNU C++ COMPILER\n");

    std::ofstream out("main.cpp");
    out << source_code;
    out.close();

    char *path = (char *) "/usr/bin/g++";
    char *const args[] = {path,
                          (char *) "main.cpp",
                          (char *) "-o",
                          (char *) "main",
                          (char *) "-fmax-errors=5",
                          (char *) "-fno-asm",
                          (char *) "-DONLINE_JUDGE",
                          (char *) "-std=c++11",
                          (char *) "-Wall",
                          (char *) "-O2",
                          (char *) "-lm",
                          nullptr};
    char *const envp[] = {nullptr};
    JuicerSandbox::run_with_constrains(0, 1, 2, path, args, envp, 5000,
                                       128 * 1024, 128 * 1024, 1024);
    return 0;
}

int
JuicerLang::GNU_cpp_compiler::run(const string &path, const string &args, const string &env, const string &input,
                                  string &output) {
    return 0;
}

int JuicerLang::GNU_cpp_compiler::diff(const string &input, const string &output) {
    return 0;
}

int JuicerLang::GNU_cpp_compiler::setRules() {
    return 0;
}

