cmake_minimum_required(VERSION 3.27)

# Roots
include("../roots.cmake")

set(CONTEXT Fly_Control+Fly_Control)
set(TARGET ${CONTEXT})
set(OUT_DIR "${SOLUTION_ROOT}/out/Fly_Control/Fly_Control")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LD_SCRIPT "${SOLUTION_ROOT}/Fly_Control_Fly_Control.sct")
set(LD_SCRIPT_PP ${LD_SCRIPT})

# Processor Options
set(CPU Cortex-M3)
set(FPU NO_FPU)
set(BYTE_ORDER Little-endian)

# Toolchain config map
include("toolchain.cmake")

# Setup project
project(${CONTEXT} LANGUAGES ASM C)

# Compilation database
add_custom_target(database
  COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" "${OUT_DIR}"
)
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})

# Setup context
add_executable(${CONTEXT})
set_target_properties(${CONTEXT} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "Fly_Control")
set_target_properties(${CONTEXT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
add_library(${CONTEXT}_GLOBAL INTERFACE)

# Includes
target_include_directories(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    ${SOLUTION_ROOT}/../Core/Inc
    ${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Inc
    ${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    ${SOLUTION_ROOT}/../Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ${SOLUTION_ROOT}/../Drivers/CMSIS/Include
    ${SOLUTION_ROOT}/../App/task
    ${SOLUTION_ROOT}/../Common/config
    ${SOLUTION_ROOT}/../Common/debug
    ${SOLUTION_ROOT}/../Interface/led
    ${SOLUTION_ROOT}/../Middleware/FreeRTOS/include
    ${SOLUTION_ROOT}/../Middleware/FreeRTOS/protable/ARM_CM3
    ${SOLUTION_ROOT}/../App/flight
    ${SOLUTION_ROOT}/../Interface/motor
    ${SOLUTION_ROOT}/../App/communication
    ${SOLUTION_ROOT}/../Interface/2_4g
    ${SOLUTION_ROOT}/../Interface/mpu
    ${SOLUTION_ROOT}/../Common/util
  >
  ${SOLUTION_ROOT}/RTE/_Fly_Control
  ${CMSIS_PACK_ROOT}/ARM/CMSIS/6.1.0/CMSIS/Core/Include
  ${CMSIS_PACK_ROOT}/Keil/STM32F1xx_DFP/2.4.1/Device/Include
  ${SOLUTION_ROOT}/../Middleware/FreeRTOS
)

# Defines
target_compile_definitions(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    USE_HAL_DRIVER
    STM32F103xB
    __MICROLIB
    STM32F10X_MD
    _RTE_
  >
)

# Compile options
target_compile_options(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:ASM>:
    "SHELL:${ASM_CPU}"
    "SHELL:${ASM_FLAGS}"
    "SHELL:${ASM_BYTE_ORDER}"
    "SHELL:-masm=auto"
  >
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_CPU}"
    "SHELL:${CC_FLAGS}"
    "SHELL:${CC_BYTE_ORDER}"
    "SHELL:-std=c99"
    "SHELL:-fno-rtti"
    "SHELL:-funsigned-char"
    "SHELL:-fshort-enums"
    "SHELL:-fshort-wchar"
    "SHELL:-ffunction-sections"
    "SHELL:-Wno-packed"
    "SHELL:-Wno-missing-variable-declarations"
    "SHELL:-Wno-missing-prototypes"
    "SHELL:-Wno-missing-noreturn"
    "SHELL:-Wno-sign-conversion"
    "SHELL:-Wno-nonportable-include-path"
    "SHELL:-Wno-reserved-id-macro"
    "SHELL:-Wno-unused-macros"
    "SHELL:-Wno-documentation-unknown-command"
    "SHELL:-Wno-documentation"
    "SHELL:-Wno-license-management"
    "SHELL:-Wno-parentheses-equality"
  >
)

# Compile Options Abstractions
add_library(${CONTEXT}_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(ASM "debug" "on" "" "" ASM_OPTIONS_FLAGS_${CONTEXT})
cbuild_set_options_flags(CC "debug" "on" "" "" CC_OPTIONS_FLAGS_${CONTEXT})
target_compile_options(${CONTEXT}_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:ASM>:
    "SHELL:${ASM_OPTIONS_FLAGS_${CONTEXT}}"
  >
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_${CONTEXT}}"
  >
)

# Add groups and components
include("groups.cmake")
include("components.cmake")

target_link_libraries(${CONTEXT} PUBLIC
  Group_Application_MDK-ARM
  Group_Application_User_Core
  Group_Drivers_STM32F1xx_HAL_Driver
  Group_Drivers_CMSIS
  Group_Common
  Group_Middleware
  Group_App
  Group_Interface
  ARM_CMSIS_CORE_6_1_0
)

# Linker options
target_link_options(${CONTEXT} PUBLIC
  "SHELL:${LD_CPU}"
  "SHELL:${_LS}\"${LD_SCRIPT_PP}\""
  "SHELL:--library_type=microlib"
  "SHELL:--map"
  "SHELL:--load_addr_map_info"
  "SHELL:--xref"
  "SHELL:--callgraph"
  "SHELL:--symbols"
  "SHELL:--info sizes"
  "SHELL:--info totals"
  "SHELL:--info unused"
  "SHELL:--info veneers"
  "SHELL:--strict"
  "SHELL:--summary_stderr"
  "SHELL:--info summarysizes"
)
set_target_properties(${CONTEXT} PROPERTIES LINK_DEPENDS ${LD_SCRIPT})
