#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <map>
#include <wait.h>
#include <fstream>
#include <cstring>
#include "sandbox.h"

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
        //printf("child exit\n");
        wait(&stat);
    }
}

int main(int argc, char *argv[]) {

    std::vector<JuicerLang::Base *> supported_lang = {
            // &JuicerLang::GNU_c_compiler::getInstance(),
            &JuicerLang::GNU_cpp_compiler::getInstance(),
    };

    signal(SIGCHLD, signal_handler);

    string title = "Juicer";
    string description = "MUST OJ backend core";
    string version = "0.0.1 Alpha";

    string program;
    string source_code;
    string language;

    string case_in;
    string case_out;

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

    if (!(cmd("--case-in") >> case_in)) {
        cerr << "Must provide case in" << endl;
        exit(1);
    }

    if (!(cmd("--case-out") >> case_out)) {
        cerr << "Must provide case out" << endl;
        exit(1);
    }

    vector<string> cases_in;
    vector<string> cases_out;

    cases_in = JuicerHelper::split(case_in);
    cases_out = JuicerHelper::split(case_out);
    if (cases_in.size() != cases_out.size()) {
        cerr << "The number of case in and case out are not match." << endl;
        exit(1);
    }

    cmd("--limit-compile-time-ms", 3000) >> limit_compile_time;
    cmd("--limit-run-time-ms", 1000) >> limit_run_time;
    cmd("--limit-stack-kb", 256 * 1024) >> limit_stack;
    cmd("--limit-memory-kb", 256 * 1024) >> limit_memory;
    cmd("--limit-output-kb", 4096) >> limit_output;

    JuicerLang::Base *lang = nullptr;

    for (auto &i: supported_lang) if (i->getLang() == language) lang = i;
    if (lang == nullptr) {
        printf("No such language: %s\n", language.c_str());
        exit(1);
    }

    /* Read the file, compile it. */
    source_code = JuicerHelper::read_file(program);

    try {
        lang->set_configs(source_code, limit_compile_time, limit_run_time,
                          limit_stack, limit_memory, limit_output,
                          cases_in, cases_out);
        lang->compile();
        for (int i = 0; i < cases_in.size(); i++) {
            lang->run(cases_in[i]);
            lang->diff(cases_out[i]);
        }
    } catch (const char *msg) {
        cerr << msg << endl;
    }

    wait(nullptr);
    return EXIT_SUCCESS;
}
