#include <iostream>


#include "third_party/argh.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

void help() {

}

int main(int argc, char *argv[]) {
    string title = "Juicer";
    string description = "MUST OJ backend core";
    string version = "0.0.1 Alpha";

    auto cmd = argh::parser(argc, argv);

    string program;
    string language;

    uint32_t limit_compile_time; // ms, default 3000ms
    uint32_t limit_run_time;     // ms, default 1000ms
    uint32_t limit_stack;        // Bytes, default 268,435,456 Bytes (256MB)
    uint32_t limit_heap;         // Bytes, default 268,435,456 Bytes (256MB)
    uint32_t limit_output;       // Bytes, default 10240 Bytes (10KB)

    if (argc == 1 || cmd[{"--help", "-h"}]) {
        cout << title << " " << version << " " << endl;
        cout << description << endl;
        help();
        return EXIT_SUCCESS;
    } else if (cmd("--version")) {
        cout << title << " version: " << version << endl;
        return EXIT_SUCCESS;
    }

    if (!(cmd("--sourcecode") >> program))
        cerr << "Must provide file path to `--sourcecode`" << endl;

    if (!(cmd("--language") >> language))
        cerr << "Must provide a `--language` for the specific source code" << endl;

    cmd("--compile-time-limit", 3000) >> limit_compile_time;
    cmd("--run-time-limit", 1000) >> limit_run_time;
    cmd("--stack-limit", 268435456) >> limit_stack;
    cmd("--heap-limit", 268435456) >> limit_heap;
    cmd("--output-limit", 10240) >> limit_output;

    // compile
    // run
    // return
    return EXIT_SUCCESS;
}
