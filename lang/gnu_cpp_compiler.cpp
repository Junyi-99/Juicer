//
// Created by Junyi Hou on 2/2/21.
//

#include <fstream>
#include "gnu_cpp_compiler.h"
#include "../helper.h"


int JuicerLang::GNU_cpp_compiler::compile(const string &source_code) {
    //printf("USING GNU C++ COMPILER\n");
    JuicerHelper::write_file("main.cpp", source_code);

    this->binary_path = "./main"; // output binary path

    char *path = (char *) "/usr/bin/g++";
    char *const args[] = {path,
                          (char *) "main.cpp",
                          (char *) "-o",
                          (char *) this->binary_path.c_str(),
                          (char *) "-fmax-errors=5",
                          (char *) "-fno-asm",
                          (char *) "-DONLINE_JUDGE",
                          (char *) "-std=c++11",
                          (char *) "-Wall",
                          (char *) "-O2",
                          (char *) "-lm",
                          nullptr};
    char *const envp[] = {nullptr};

    // compiler should not enable sandbox
    return JuicerSandbox::run_with_constrains(0, 1, 2,
                                              path, args, envp,
                                              5000, 256 * 1024, 256 * 1024, 1024, false);
}

int
JuicerLang::GNU_cpp_compiler::run(const string &input, string &output, uint32_t limit_time,
                                  uint32_t limit_stack, uint32_t limit_memory, uint32_t limit_output) {

    char *const args[] = {(char *) this->binary_path.c_str(), nullptr};
    char *const envp[] = {nullptr};
    return JuicerSandbox::run_with_constrains(0, 1, 2,
                                              this->binary_path, args, envp,
                                              limit_time, limit_stack, limit_memory, limit_output, true);
}

int JuicerLang::GNU_cpp_compiler::diff(const string &input, const string &output) {
    return 0;
}

int JuicerLang::GNU_cpp_compiler::setRules() {
    return 0;
}

