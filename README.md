# Juicer

来自谐音梗

评测机 -> Judge机 -> 榨汁机 -> Juicer

## 目标

可投入生产使用的、安全的、快速的、企业级开源评测器

## 安全措施

一定要做好安全措施，已知的攻击方式主要分为两种

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