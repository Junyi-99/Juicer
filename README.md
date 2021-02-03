# Juicer

来自谐音梗

评测机 -> Judge机 -> 榨汁机 -> Juicer

## 目标

可投入生产使用的、安全的、快速的、企业级开源评测器

代码风格是适合人类阅读的。

## 特性 Features

- CLI 支持
- 远程 Socket
- 安全沙盒
- 多语言（GNU_c_compiler++ Python Go）

## TL;DR

如果要编译代码，请安装 libseccomp

`sudo apt install libseccomp-dev libseccomp2 seccomp`


## 安全措施

一定要做好安全措施，这样才不会被日穿。

Juicer 除了拥有 Sandbox 功能，还运行在 Docker 环境内，方便万一哪天炸了可以迅速重启恢复。

已知的攻击方式主要分为两种

**编译时攻击**

- 编译超时
- 编译出巨大文件
- 模版炸弹
- include 攻击
- 开启高级优化

**运行时攻击**

- `system("shutdown -h now")`
- fork 炸弹
- 多线程攻击
- 网络访问攻击
- 访问答案文件并且输出
- 巨型文件写出攻击
- 内联汇编攻击
- I/O 磁盘碎片攻击
- 故意运行超时（跟 DDoS 一样低级下流的手段）

## 代码规范
https://google.github.io/styleguide/cppguide.html

## 好文分享

https://blog.cloudflare.com/sandboxing-in-linux-with-zero-lines-of-code/
https://veritas501.space/2018/05/05/seccomp%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/
