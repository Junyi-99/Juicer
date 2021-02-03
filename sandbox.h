//
// Created by Junyi Hou on 2/2/21.
//

#ifndef OJ_SANDBOX_H
#define OJ_SANDBOX_H

#include <linux/seccomp.h>
#include <sys/resource.h>
#include <unistd.h>
#include <seccomp.h>
#include <fcntl.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace JuicerSandbox {
    void *timeout_killer(void *pid);

    /*
    内部进行 fork，并且开启线程监视被执行程序
    不限制 system call 的调用
    该方法可以用于编译控制，以及目标程序控制

    limit_time: ms
    limit_stack: KB
    limit_memory: KB
    limit_output: KB

    返回值：
    成功返回0
    setrlimit 失败返回 1
    */
    int run_with_constrains(int fd_in, int fd_out, int fd_err,
                            const string &path, char *const argv[], char *const envp[],
                            uint32_t limit_time, uint32_t limit_stack,
                            uint32_t limit_memory, uint32_t limit_output);

}


class Sandbox {
public:
    int foo();


};


#endif //OJ_SANDBOX_H
