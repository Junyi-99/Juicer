//
// Created by Junyi Hou on 2/2/21.
//

#include <csignal>
#include <wait.h>
#include "sandbox.h"

namespace JuicerSandbox {
    int white[] = {
            SCMP_SYS(exit_group),
            SCMP_SYS(brk),
            SCMP_SYS(arch_prctl),
            SCMP_SYS(access),
            SCMP_SYS(openat),
            SCMP_SYS(fstat),
            SCMP_SYS(mmap),
            SCMP_SYS(munmap),
            SCMP_SYS(close),
            SCMP_SYS(read),
            SCMP_SYS(pread64),
            SCMP_SYS(write),
            SCMP_SYS(mprotect),
    };

    struct arg_struct {
        pid_t pgid;          // process group id
        uint32_t sleep_time; // ms
    };

    void *timeout_killer(void *args) {
        auto *p = (struct arg_struct *) args;

        if (pthread_detach(pthread_self()) != 0) {
            killpg(p->pgid, SIGKILL);
            perror("pthread_detach");
            exit(1);
        }

        usleep(p->sleep_time * 1000);

        printf("wake up and kill -%d\n", p->pgid);
        killpg(p->pgid, SIGKILL);
        return nullptr;
    }

    int run_with_constrains(int fd_in, int fd_out, int fd_err,
                            const string &path, char *const argv[],
                            char *const envp[],
                            uint32_t limit_time, uint32_t limit_stack,
                            uint32_t limit_memory, uint32_t limit_output, bool enable_sandbox) {
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
                    (limit_time + 1000) / 1000,
                    (limit_time + 1000) / 1000
            };
            struct rlimit rlimit_stack{
                    limit_stack * 1024,
                    limit_stack * 1024,
            };
            struct rlimit rlimit_memory{
                    limit_memory * 1024,
                    limit_memory * 1024
            };
            struct rlimit rlimit_output{
                    limit_output * 1024,
                    limit_output * 1024
            };

            int ret[5] = {0};
            /* RLIMIT_CPU 不统计使用了 sleep() 的程序，所以必须靠另外线程来侦测运行时间 */
            ret[0] = setrlimit(RLIMIT_CPU, &rlimit_time);
            ret[1] = setrlimit(RLIMIT_STACK, &rlimit_stack);
            ret[2] = setrlimit(RLIMIT_DATA, &rlimit_memory);
            ret[3] = setrlimit(RLIMIT_NPROC, &rlimit_nproc);
            ret[4] = setrlimit(RLIMIT_FSIZE, &rlimit_output);

            for (int i : ret) {
                if (i != 0) throw "set rlimit failed";
            }

            setpgid(getpid(), getpid());
            //execve(path.c_str(), argv, envp);

            if (enable_sandbox) {
                scmp_filter_ctx ctx;
                if ((ctx = seccomp_init(SCMP_ACT_KILL)) == nullptr) {
                    throw "seccomp_init failed";
                }
                for (int i : white) {
                    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, i, 0) != 0) {
                        throw "seccomp_rule_add failed";
                    }
                }

                seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1,
                                 SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t) (path.c_str())));
                seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 1,
                                 SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0));
                seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1,
                                 SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0));
                if (seccomp_load(ctx) != 0) throw "seccomp_load failed";
                seccomp_release(ctx); // does not return a value
            }

            execvp(path.c_str(), argv);
        } else {
            // parent
            int ret, status;
            pthread_t monitor = 0;
            struct arg_struct args{
                    pid,
                    limit_time
            };

            ret = pthread_create(&monitor, nullptr, timeout_killer, reinterpret_cast<void *>(&args));
            if (ret != 0) {
                perror("pthread_create");
                killpg(pid, SIGKILL);
                exit(1);
            }

            waitpid(pid, &status, WUNTRACED | WCONTINUED);
            return status;
        }

        // no one can execute here.
        exit(1);
    }
}
