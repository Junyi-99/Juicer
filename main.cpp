#include <iostream>
#include <unistd.h>
#include <map>
#include <wait.h>
#include <cstring>
#include "sandbox.h"
#include "syscall-names.h"
#include "lang/gnu_cpp_compiler.h"

#include "third_party/argh.h"
#include "helper.h"

#if defined(__i386__)
#define REG_RESULT    REG_EAX
#define REG_SYSCALL    REG_EAX
#define REG_ARG0    REG_EBX
#define REG_ARG1    REG_ECX
#define REG_ARG2    REG_EDX
#define REG_ARG3    REG_ESI
#define REG_ARG4    REG_EDI
#define REG_ARG5    REG_EBP
#elif defined(__x86_64__)
#define REG_RESULT    REG_RAX
#define REG_SYSCALL    REG_RAX
#define REG_ARG0    REG_RDI
#define REG_ARG1    REG_RSI
#define REG_ARG2    REG_RDX
#define REG_ARG3    REG_R10
#define REG_ARG4    REG_R8
#define REG_ARG5    REG_R9
#endif
#ifndef SYS_SECCOMP
#define SYS_SECCOMP 1
#endif

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
    } else if (sig == SIGSYS) {

    }
}

/* Since "sprintf" is technically not signal-safe, reimplement %d here. */
static void write_uint(char *buf, unsigned int val) {
    int width = 0;
    unsigned int tens;

    if (val == 0) {
        strcpy(buf, "0");
        return;
    }
    for (tens = val; tens; tens /= 10)
        ++width;
    buf[width] = '\0';
    for (tens = val; tens; tens /= 10)
        buf[--width] = (char) ('0' + (tens % 10));
}

static void helper(int nr, siginfo_t *info, void *void_context) {
    char buf[255];
    auto *ctx = (ucontext_t *) (void_context);
    unsigned int syscall;

    if (info->si_code != SYS_SECCOMP)
        return;
    if (!ctx)
        return;

    syscall = (unsigned int) ctx->uc_mcontext.gregs[REG_SYSCALL];
    strcpy(buf, "Invalid system call: ");
    if (syscall < sizeof(syscall_names)) {
        strcat(buf, syscall_names[syscall]);
        strcat(buf, "(");
    }
    write_uint(buf + strlen(buf), syscall);
    if (syscall < sizeof(syscall_names))
        strcat(buf, ")");
    strcat(buf, "\n");
    write(STDOUT_FILENO, buf, strlen(buf));
}

static int install_helper() {
    syscall_names_init();
    struct sigaction act;
    sigset_t mask;
    memset(&act, 0, sizeof(act));
    sigemptyset(&mask);
    sigaddset(&mask, SIGSYS);

    act.sa_sigaction = &helper;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSYS, &act, nullptr) < 0) {
        std::string details("SYSTEM_ERROR");
        details += "sigaction failed: " + std::string(strerror(errno));
        throw ResultException(ResultType::SYSTEM_ERROR, details);
    }
    if (sigprocmask(SIG_UNBLOCK, &mask, nullptr)) {
        std::string details("SYSTEM_ERROR");
        details += "sigprocmask failed: " + std::string(strerror(errno));
        throw ResultException(ResultType::SYSTEM_ERROR, details);
    }
    return 0;
}

void proc(int argc, char *argv[]) {

    std::vector<JuicerLang::Base *> supported_lang = {
            // &JuicerLang::GNU_c_compiler::getInstance(),
            &JuicerLang::GNU_cpp_compiler::getInstance(),
    };

    signal(SIGCHLD, signal_handler);
    signal(SIGSYS, signal_handler);

    if (install_helper())
        throw ResultException(ResultType::SYSTEM_ERROR, "install helper failed");

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
        exit(EXIT_SUCCESS);
    } else if (cmd["version"]) {
        cout << title << " version: " << version << endl;
        exit(EXIT_SUCCESS);
    }

    if (!(cmd("--sourcecode") >> program)) {
        cerr << "Must provide file path to `--sourcecode`" << endl;
        exit(EXIT_FAILURE);
    }

    if (!(cmd("--language") >> language)) {
        cerr << "Must provide a `--language` for the specific source ret" << endl;
        exit(EXIT_FAILURE);
    }

    if (!(cmd("--case-in") >> case_in)) {
        cerr << "Must provide case in" << endl;
        exit(EXIT_FAILURE);
    }

    if (!(cmd("--case-out") >> case_out)) {
        cerr << "Must provide case out" << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> cases_in;
    vector<string> cases_out;

    cases_in = JuicerHelper::split(case_in);
    cases_out = JuicerHelper::split(case_out);
    if (cases_in.size() != cases_out.size()) {
        cerr << "The number of case in and case out are not match." << endl;
        exit(EXIT_FAILURE);
    }

    cmd("--limit-compile-time-ms", 3000) >> limit_compile_time;
    cmd("--limit-run-time-ms", 1000) >> limit_run_time;
    cmd("--limit-stack-kb", 256 * 1024) >> limit_stack;
    cmd("--limit-memory-kb", 256 * 1024) >> limit_memory;
    cmd("--limit-output-kb", 4096) >> limit_output;

    JuicerLang::Base *lang = nullptr;

    for (auto &i: supported_lang) if (i->getLang() == language) lang = i;
    if (lang == nullptr) {
        std::string details("SYSTEM_ERROR");
        details += "no such language: " + language;
        throw ResultException(ResultType::SYSTEM_ERROR, details);
    }

    /* Read the file, compile it. */
    source_code = JuicerHelper::read_file(program);


    lang->config(source_code, limit_compile_time, limit_run_time,
                 limit_stack, limit_memory, limit_output,
                 cases_in, cases_out);
    lang->compile();

    for (int i = 0; i < cases_in.size(); i++) {
        try {
            lang->run(i);
            lang->diff(i);
        } catch (ResultException &e) {
            if (e.what_type() == ResultType::COMPILE_ERROR ||
                e.what_type() == ResultType::SYSTEM_ERROR) {
                throw ResultException(e.what_type(), e.what());
            } else {
                cout << (int) e.what_type() << endl;
                cout << e.what() << endl;
                continue;
            }
        }
    }
}

int main(int argc, char *argv[]) {

    try {
        proc(argc, argv);
    } catch (ResultException &e) {
        //cout << (int) e.what_type() << endl;
        cout << e.what() << endl;
    }

    wait(nullptr);
    return EXIT_SUCCESS;
}
