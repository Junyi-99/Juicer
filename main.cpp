#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/resource.h>
#include <wait.h>
#include "third_party/argh.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

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
        printf("SIGCHLD\n");
        wait(&stat);
    }
}

void *timeout_killer(void *pid) {
    pid_t p = *(pid_t *) pid;
    printf("monitoring %d\n", p);
    if(pthread_detach(pthread_self()) != 0){
        killpg(p, SIGKILL);
        perror("pthread_detach");
        exit(1);
    }
    usleep(1000*1000);
    killpg(p, SIGKILL);
    return nullptr;
}

void help() {

}

void test() {
    pid_t pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // child
        struct rlimit rlimit_nproc{
                10000,
                10000
        };
        struct rlimit rlimit_time{
                (limit_run_time + 1000) / 1000,
                (limit_run_time + 1000) / 1000
        };
        struct rlimit rlimit_stack{
                limit_stack * 1024,
                limit_stack * 1024,
        };
        struct rlimit rlimit_memory{
                limit_memory * 1024,
                limit_memory * 1024
        };
        struct rlimit rlimit_output{};

        /* RLIMIT_CPU 不统计使用了 sleep() 的程序，所以必须靠另外线程来侦测运行时间 */
        setrlimit(RLIMIT_CPU, &rlimit_time);
        setrlimit(RLIMIT_STACK, &rlimit_stack);
        setrlimit(RLIMIT_DATA, &rlimit_memory);
        setrlimit(RLIMIT_NPROC, &rlimit_nproc);

        char *args[] = {nullptr};
        char *envp[] = {nullptr};

        setpgid(getpid(), getpid());
        //sleep(5);
        execve(program.c_str(), args, envp);
    } else {
        // parent
        pthread_t monitor = 0;
        int ret = pthread_create(&monitor, nullptr, timeout_killer, reinterpret_cast<void *>(&pid));
        if (ret != 0) {
            perror("pthread_create");
            killpg(pid, SIGKILL);
            exit(1);
        }

        int status;;
        printf("waiting child\n");
        waitpid(pid, &status, WUNTRACED | WCONTINUED);
    }
    exit(1);
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
    // 64 - 63.695312 = 0.30468799999999874
    // 14 - 13.863281 = 0.13671899999999937
    // 128 - 127.406250 = 0.59375
    // 512 - 509.898438 = 2.1015620000000013
    test();
    // compile
    // run
    // return
    return EXIT_SUCCESS;
}
