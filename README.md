# Quadcopter_Control-STM32-FreeRTOS

技术栈：STM32F103c8t6、FreeRTOS、通信（USART、I2C、SPI）、8520电机（PWM控制）、外设（MPU6050、Vl53L1X、SI24R1、0.96寸OLED）、摇杆值与电池电压（ADC、DMA）、串级PID控制（姿态、定高）

项目简介：设计基于STM32与FreeRTOS的四轴飞行器，遥控通过2.4G发送摇杆、按键数据控制飞行器自主飞行，通过姿态解算和串级PID控制实现飞行器自动平衡姿态、定高悬停功能。

功能与职责：

  飞控核心功能实现：姿态解算（角速度用低通滤波、加速度用卡尔曼滤波、四元数解算出角度），串级PID控制（内环：角速度环、Z轴速度环，外环：角度环、高度），运动控制（自主飞行、定高悬停）。
  
  FreeRTOS应用：任务划分（飞行控制、姿态解算与PID控制、LED状态提示、2.4G通信、摇杆按键数据处理、OLED显示），优先级调度与任务循环周期设置，任务间通信（队列、互斥量）。
  
  外设驱动开发与调试：PWM输出控制电机和LED、陀螺仪驱动与校准、激光测距驱动与障碍物检测、2.4G通信与定义数据帧、OLED显示与取字模、ADC读取DMA搬运摇杆值和电池电压。
  
  硬件调试与问题排查：解决电机鸣叫、传感器噪声、通信丢包、遇到障碍物突升、飞行器严重抖动等问题。
  
实现成果：

  实现稳定的姿态控制和定高悬停，姿态平衡精度达到±1度，定高悬停精度达到±3厘米。
  
  设计并调优串级PID参数，将姿态平衡响应时间缩短50%，姿态抖动幅度降低到±1度。

【STM32&FreeRTOS四轴飞行器功能演示视频】https://www.bilibili.com/video/BV1hJb1zME6Q?vd_source=1d6eb56c93cf780491517dccbef2468a

Technology stack: STM32F103c8t6, FreeRTOS, communication (USART, I2C, SPI), 8520 motor (PWM control), peripherals (MPU6050, Vl53L1X, SI24R1, 0.96-inch OLED), joystick values and battery voltage (ADC, DMA), cascaded PID control (attitude, altitude stabilization)

Project Introduction: The project involves designing a quadcopter based on STM32 and FreeRTOS. The remote control transmits joystick and button data via 2.4G to control the autonomous flight of the quadcopter. The automatic balance of the quadcopter's attitude and the function of hovering at a fixed height are achieved through attitude calculation and cascade PID control. 

Functions and Responsibilities: 
The core functions of flight control are realized as follows: attitude calculation (low-pass filtering for angular velocity, Kalman filtering for acceleration, and quaternion calculation for angles), cascade PID control (inner loop: angular velocity loop, Z-axis velocity loop; outer loop: angle loop, height loop), and motion control (autonomous flight, altitude-holding hover). 
FreeRTOS application: task division (flight control, attitude calculation and PID control, LED status indication, 2.4G communication, joystick and key data processing, OLED display), priority scheduling and task cycle period setting, inter-task communication (queue, mutex). 
Peripheral driver development and debugging: PWM output control for motors and LEDs, gyroscope driver and calibration, laser ranging driver and obstacle detection, 2.4G communication and definition of data frames, OLED display and font extraction, ADC reading and DMA transfer of joystick values and battery voltage. 
Hardware debugging and problem-solving: Address issues such as motor whining, sensor noise, communication packet loss, sudden elevation upon encountering obstacles, and severe aircraft shaking. 

Achievements: 
Achieve stable attitude control and altitude-holding hovering, with attitude balance accuracy reaching ±1 degree and altitude-holding hovering accuracy reaching ±3 centimeters. 
Design and optimize the cascade PID parameters to reduce the attitude balance response time by 50% and lower the attitude jitter amplitude to ±1 degree.

【STM32 & FreeRTOS Quadcopter Flight Control Function Demonstration Video】https://www.bilibili.com/video/BV1hJb1zME6Q?vd_source=1d6eb56c93cf780491517dccbef2468a
