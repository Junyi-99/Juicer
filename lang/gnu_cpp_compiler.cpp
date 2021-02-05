//
// Created by Junyi Hou on 2/2/21.
//

#include <fstream>
#include "gnu_cpp_compiler.h"
#include "../helper.h"


int JuicerLang::GNU_cpp_compiler::compile() {
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
    printf("\nGNU CPP stage: compile\n");

    int fd_out = open(this->compile_log.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        std::string details("SYSTEM_ERROR\n");
        details += "open fd_out failed: " + std::string(strerror(errno));
        throw ResultException(ResultType::SYSTEM_ERROR, details);
    }

    // compiler should not enable sandbox
    int ret = JuicerSandbox::run_with_constrains(STDIN_FILENO, fd_out, fd_out,
                                                 path, args, envp,
                                                 this->limit_compile_time, 256 * 1024, 256 * 1024, 1024, false);
    if (ret != 0) {
        // TODO: compile log return
        std::string details("COMPILE_ERROR\n");
        details += "return code: " + std::to_string(ret);
        throw ResultException(ResultType::COMPILE_ERROR, details);
    }
    return ret;
}

int
JuicerLang::GNU_cpp_compiler::run(const uint8_t &case_id) {

    printf("\nGNU CPP stage: run - case %s\n", this->cases_in[case_id].c_str());

    char *const args[] = {(char *) this->binary_path.c_str(), nullptr};
    char *const envp[] = {nullptr};

    int fd_in = open(this->cases_in[case_id].c_str(), O_RDONLY);
    if (fd_in == -1) {
        std::string details("SYSTEM_ERROR\n");
        details += "open fd_in failed: " + std::string(strerror(errno));
        throw ResultException(ResultType::SYSTEM_ERROR, details);
    }

    int fd_out = open(this->output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        std::string details("SYSTEM_ERROR\n");
        details += "open fd_out failed: " + std::string(strerror(errno));
        throw ResultException(ResultType::SYSTEM_ERROR, details);
    }

    int ret = JuicerSandbox::run_with_constrains(
            fd_in, fd_out, 2,
            this->binary_path, args, envp,
            this->limit_run_time,
            this->limit_stack,
            this->limit_memory,
            this->limit_output, true
    );
    close(fd_in);
    close(fd_out);

    if (ret != 0) {
        std::string details("RUNTIME_ERROR\n");
        details += "return code: " + std::to_string(ret);
        details += ", Judge terminated.";
        throw ResultException(ResultType::RUNTIME_ERROR, details);
    }
    return 0;
}

int JuicerLang::GNU_cpp_compiler::diff(const uint8_t &case_id) {
    string output = JuicerHelper::read_file(this->output_file);
    string standard = JuicerHelper::read_file(this->cases_out[case_id]);
    JuicerHelper::trim(output);
    JuicerHelper::trim(standard);

    if (output == standard) {
        std::string details = "ACCEPTED\n";
        throw ResultException(ResultType::ACCEPTED, details);
    } else {
        std::string details("WRONG_ANSWER\n");
        details += "output  : " + output + "\n";
        details += "standard: ";
        details += standard;
        throw ResultException(ResultType::WRONG_ANSWER, details);
    }
}

int JuicerLang::GNU_cpp_compiler::setRules() {
    return 0;
}

