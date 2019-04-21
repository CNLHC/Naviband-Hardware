# Naviband-Hardware

[Naviband](https://github.com/CNLHC/NaviBand-Andriod)项目的硬件部分。面向 `STM32F103c8tx`开发, 理论上兼容其他所有`ARM Cortex M3` 架构处理器

基于实现`CMSIS RTOS v1.0`接口的`FreeRTOS v9.0` 进行开发。实现了异步的从蓝牙串口读取信息，将消息通过`queue`传输到Parser，解析后控制电机振动。
