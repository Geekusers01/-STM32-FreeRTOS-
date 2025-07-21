# groups.cmake

# group Application/MDK-ARM
add_library(Group_Application_MDK-ARM OBJECT
  "${SOLUTION_ROOT}/startup_stm32f103xb.s"
)
target_include_directories(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Application_MDK-ARM_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Application_MDK-ARM_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_MDK-ARM PUBLIC
  Group_Application_MDK-ARM_ABSTRACTIONS
)
set(COMPILE_DEFINITIONS
  __MICROLIB
  STM32F10X_MD
  _RTE_
)
cbuild_set_defines(AS_ARM COMPILE_DEFINITIONS)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32f103xb.s" PROPERTIES
  COMPILE_FLAGS "${COMPILE_DEFINITIONS}"
)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32f103xb.s" PROPERTIES
  COMPILE_OPTIONS "-masm=auto"
)

# group Application/User/Core
add_library(Group_Application_User_Core OBJECT
  "${SOLUTION_ROOT}/../Core/Src/main.c"
  "${SOLUTION_ROOT}/../Core/Src/gpio.c"
  "${SOLUTION_ROOT}/../Core/Src/i2c.c"
  "${SOLUTION_ROOT}/../Core/Src/spi.c"
  "${SOLUTION_ROOT}/../Core/Src/tim.c"
  "${SOLUTION_ROOT}/../Core/Src/usart.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32f1xx_it.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32f1xx_hal_msp.c"
)
target_include_directories(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Application_User_Core_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Application_User_Core_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_User_Core PUBLIC
  Group_Application_User_Core_ABSTRACTIONS
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/i2c.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/spi.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/tim.c" PROPERTIES
  COMPILE_OPTIONS ""
)

# group Drivers/STM32F1xx_HAL_Driver
add_library(Group_Drivers_STM32F1xx_HAL_Driver OBJECT
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c"
)
target_include_directories(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_STM32F1xx_HAL_Driver_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_STM32F1xx_HAL_Driver_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  Group_Drivers_STM32F1xx_HAL_Driver_ABSTRACTIONS
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c" PROPERTIES
  COMPILE_OPTIONS ""
)

# group Drivers/CMSIS
add_library(Group_Drivers_CMSIS OBJECT
  "${SOLUTION_ROOT}/../Core/Src/system_stm32f1xx.c"
)
target_include_directories(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_CMSIS_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_CMSIS_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_CMSIS PUBLIC
  Group_Drivers_CMSIS_ABSTRACTIONS
)

# group Common
add_library(Group_Common OBJECT
  "${SOLUTION_ROOT}/../Common/config/Com_Config.c"
  "${SOLUTION_ROOT}/../Common/debug/Com_Debug.c"
  "${SOLUTION_ROOT}/../Common/util/Com_Filter.c"
)
target_include_directories(Group_Common PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Common PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Common_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Common_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Common PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Common PUBLIC
  Group_Common_ABSTRACTIONS
)

# group Middleware
add_library(Group_Middleware OBJECT
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/protable/ARM_CM3/port.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/protable/MemMang/heap_4.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/source/croutine.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/source/event_groups.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/source/list.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/source/queue.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/source/stream_buffer.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/source/tasks.c"
  "${SOLUTION_ROOT}/../Middleware/FreeRTOS/source/timers.c"
)
target_include_directories(Group_Middleware PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  ${SOLUTION_ROOT}/../Middleware/FreeRTOS
)
target_compile_definitions(Group_Middleware PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Middleware_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Middleware_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Middleware PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Middleware PUBLIC
  Group_Middleware_ABSTRACTIONS
)

# group App
add_library(Group_App OBJECT
  "${SOLUTION_ROOT}/../App/task/App_Task.c"
  "${SOLUTION_ROOT}/../App/flight/App_Flight.c"
  "${SOLUTION_ROOT}/../App/communication/App_communication.c"
)
target_include_directories(Group_App PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_App PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_App_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_App_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_App PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_App PUBLIC
  Group_App_ABSTRACTIONS
)

# group Interface
add_library(Group_Interface OBJECT
  "${SOLUTION_ROOT}/../Interface/led/Inf_LED.c"
  "${SOLUTION_ROOT}/../Interface/motor/Inf_motor.c"
  "${SOLUTION_ROOT}/../Interface/2_4g/Inf_SI24R1.c"
  "${SOLUTION_ROOT}/../Interface/mpu/Inf_mpu6050.c"
)
target_include_directories(Group_Interface PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Interface PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Interface_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Interface_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Interface PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Interface PUBLIC
  Group_Interface_ABSTRACTIONS
)
