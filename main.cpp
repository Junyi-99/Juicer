#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <map>
#include <wait.h>
#include "sandbox.h"

#include "lang/gnu_c_compiler.h"
#include "lang/gnu_cpp_compiler.h"

#include "third_party/argh.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

std::vector<JuicerLang::Base *> supported_lang = {
        &JuicerLang::GNU_c_compiler::getInstance(),
        &JuicerLang::GNU_cpp_compiler::getInstance(),
};

string program;
string language;

uint32_t limit_compile_time = 0; // ms, default 3000ms
uint32_t limit_run_time = 0;     // ms, default 1000ms
uint32_t limit_stack = 0;        // KBytes, default 262,144 KBytes (256MB)
uint32_t limit_memory = 0;       // KBytes, default 262,144 KBytes (256MB)
uint32_t limit_output = 0;       // KBytes, default 1024 KBytes (10KB)

void signal_handler(int sig) {
    int stat = 0;
    if (sig == SIGCHLD) {
        rusage usage{};
        printf("child exit\n");
        wait(&stat);
    }
}

void help() {

}

int main(int argc, char *argv[]) {

    signal(SIGCHLD, signal_handler);
    string title = "Juicer";
    string description = "MUST OJ backend core";
    string version = "0.0.1 Alpha";

    auto cmd = argh::parser(argc, argv);

    if (argc == 1 || cmd[{"--help", "-h"}]) {
        cout << title << " " << version << " " << endl;
        cout << description << endl;
        help();
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
        cerr << "Must provide a `--language` for the specific source code" << endl;
        exit(1);
    }

    cmd("--compile-time-limit", 3000) >> limit_compile_time;
    cmd("--run-time-limit", 1000) >> limit_run_time;
    cmd("--stack-limit", 256 * 1024) >> limit_stack;
    cmd("--memory-limit", 256 * 1024) >> limit_memory;
    cmd("--output-limit", 1024) >> limit_output;

    // memory bias
    // 64 - 63.695312 = 0.30468799999999874
    // 14 - 13.863281 = 0.13671899999999937
    // 128 - 127.406250 = 0.59375
    // 512 - 509.898438 = 2.1015620000000013
    int code;
    JuicerLang::Base *lang;

    for (auto &i: supported_lang) if (i->getLang() == language) lang = i;
    string src = "#include <stdio.h>\nint main(){printf(\"Who let the dogs out?\\n\"); return 0;}\n";
    code = lang->compile(src);
    if (code != 0) {
        printf("compiling failed, code = %d\n", code);
    }
    printf("compile finished.\n");

    string output;
    code = lang->run("", "", "", "", output);
    if (code != 0) {
        printf("running failed, code = %d\n", code);
    }
    printf("running finished.\n");

    code = lang->diff("", "");
    if (code != 0) {
        printf("diffing failed, code = %d\n", code);
    }
    printf("diffing finished.\n");

    wait(nullptr);
    return EXIT_SUCCESS;
}
