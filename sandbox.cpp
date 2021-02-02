//
// Created by Junyi Hou on 2/2/21.
//

#include "sandbox.h"

int Sandbox::foo() {
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

    scmp_filter_ctx ctx;
    if ((ctx = seccomp_init(SCMP_ACT_KILL)) == nullptr) {
        return 1;
    }
    for (int i : white) {
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, i, 0) != 0) {
            return 1;
        }
    }

    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t) ("hello.out")));
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0));
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0));
    //seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
    seccomp_load(ctx);
    seccomp_release(ctx);

    char *argv[] = {nullptr};
    char *envp[] = {nullptr};
    execve("hello.out", argv, envp);
    return 0;
}
