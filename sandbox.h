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
#include <csignal>
#include <cstring>
#include <wait.h>
#include "helper.h"

using std::string;
using std::vector;

namespace JuicerSandbox {
    void *timeout_killer(void *pid);


    /*
    内部进行 fork，并且开启线程监视被执行程序
    可选是否限制 system call 的调用（通过 enable_sandbox）
    该方法可以用于编译控制，以及目标程序控制

     fd_in 你想让它的 stdin 从哪读？
     fd_out 你想让它写到哪里？
     fd_err 你想让它错误输出到哪？
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
                            uint32_t limit_memory, uint32_t limit_output, bool enable_sandbox);

}

#endif //OJ_SANDBOX_H
