//
// Created by Junyi Hou on 2/2/21.
//

#ifndef OJ_SANDBOX_H
#define OJ_SANDBOX_H

#include <linux/seccomp.h>
#include <unistd.h>
#include <seccomp.h>
#include <fcntl.h>

class Sandbox {
public:
    int foo();
};


#endif //OJ_SANDBOX_H
