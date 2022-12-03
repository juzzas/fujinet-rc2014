cmake_minimum_required(VERSION 3.0)

# target name "example.rom"
project(fujinet-rc2014 C ASM)

include_directories(${CMAKE_CURRENT_LIST_DIR}/lib/include)

add_library(fujinet STATIC
        lib/src/hal/cpm/hal.asm
        lib/src/hal/cpm/hal_console.asm
        lib/src/fujinet.c
        lib/src/fujinet.c
        lib/src/fujinet_device.c
        lib/src/fujinet_modem.c
        lib/src/fujinet_network.c
        lib/src/fujinet_printer.c
       )


add_executable(fnwifi.com apps/fnwifi/fnwifi.c)
target_link_libraries(fnwifi.com fujinet)