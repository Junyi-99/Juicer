#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <map>
#include <wait.h>
#include <fstream>
#include <cstring>
#include "sandbox.h"

#include "lang/gnu_c_compiler.h"
#include "lang/gnu_cpp_compiler.h"

#include "third_party/argh.h"
#include "helper.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

void signal_handler(int sig) {
    int stat = 0;
    if (sig == SIGCHLD) {
        rusage usage{};
        printf("child exit\n");
        wait(&stat);
    }
}

int main(int argc, char *argv[]) {

    std::vector<JuicerLang::Base *> supported_lang = {
            &JuicerLang::GNU_c_compiler::getInstance(),
            &JuicerLang::GNU_cpp_compiler::getInstance(),
    };

    signal(SIGCHLD, signal_handler);

    string title = "Juicer";
    string description = "MUST OJ backend core";
    string version = "0.0.1 Alpha";

    string program;
    string source_code;
    string language;

    uint32_t limit_compile_time = 0; // ms, default 3000ms
    uint32_t limit_run_time = 0;     // ms, default 1000ms
    uint32_t limit_stack = 0;        // KBytes, default 262,144 KBytes (256MB)
    uint32_t limit_memory = 0;       // KBytes, default 262,144 KBytes (256MB)
    uint32_t limit_output = 0;       // KBytes, default 1024 KBytes (10KB)

    auto cmd = argh::parser(argc, argv);

    if (argc == 1 || cmd[{"--help", "-h"}]) {
        cout << title << " " << version << " " << endl;
        cout << description << endl;
        return EXIT_SUCCESS;
    } else if (cmd["version"]) {
        cout << title << " version: " << version << endl;
        return EXIT_SUCCESS;
    }

    if (!(cmd("--sourcecode") >> program)) {
        cerr << "Must provide file path to `--sourcecode`" << endl;
        exit(1);
    }

    if (!(cmd("--language") >> language)) {
        cerr << "Must provide a `--language` for the specific source ret" << endl;
        exit(1);
    }

    cmd("--compile-time-limit", 3000) >> limit_compile_time;
    cmd("--run-time-limit", 1000) >> limit_run_time;
    cmd("--stack-limit", 256 * 1024) >> limit_stack;
    cmd("--memory-limit", 256 * 1024) >> limit_memory;
    cmd("--output-limit", 1024) >> limit_output;

    int ret;
    JuicerLang::Base *lang = nullptr;

    for (auto &i: supported_lang) if (i->getLang() == language) lang = i;
    if (lang == nullptr) {
        printf("No such language: %s\n", language.c_str());
        exit(1);
    }

    /* Read the file, compile it. */
    source_code = JuicerHelper::read_file(program);
    ret = lang->compile(source_code);
    if (ret != 0) {
        printf("compiling failed, ret = %d\n", ret);
    } else {
        printf("compile finished.\n");
    }

    /* Run the compiled target */
    string output;
    ret = lang->run("", output, limit_run_time, limit_stack, limit_memory, limit_output);
    if (ret != 0) {
        printf("running failed, ret = %d\n", ret);
    } else {
        printf("running finished.\n");
    }

    /* Check the result. */
    ret = lang->diff("", "");
    if (ret != 0) {
        printf("diffing failed, ret = %d\n", ret);
    } else {
        printf("diffing finished.\n");
    }

    wait(nullptr);
    return EXIT_SUCCESS;
}
