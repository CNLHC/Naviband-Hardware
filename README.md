# Naviband-Hardware

`Naviband` 的硬件部分。面向 `STM32F103c8tx`, 理论上兼容其他所有`ARM Cortex M3` 架构处理器

基于被 `CMSIS OS`接口包装的`FreeRTOS` 进行开发。实现了异步的从蓝牙串口读取信息，将消息通过`queue`传输到Parser，解析后控制电机振动。
