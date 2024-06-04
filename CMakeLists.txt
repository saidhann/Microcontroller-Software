cmake_minimum_required(VERSION 3.13)

set(PROGRAM_NAME pico_w_webclient )
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

project(pico_w_webclient C CXX ASM)

pico_sdk_init()

add_subdirectory(modules/pico-onewire)

add_executable(${PROGRAM_NAME}
    main.cpp
)

add_library(pico_hd44780 INTERFACE)

target_sources(pico_hd44780 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/HD44780_LCD_PCF8574.cpp
  ${CMAKE_CURRENT_LIST_DIR}/HD44780_LCD_PCF8574_Print.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add pico_stdlib library, add more if used
target_link_libraries(${PROJECT_NAME}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_cyw43_arch_none
    pico_stdlib
    hardware_adc
    hardware_i2c
    pico_hd44780
    pico_one_wire
    hardware_gpio
)
# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Need to generate UF2 file for upload to RP2040
pico_add_extra_outputs(${PROJECT_NAME})