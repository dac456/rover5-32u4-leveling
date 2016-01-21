set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # AVR Toolchain


cmake_minimum_required(VERSION 2.8)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(rover-32u4-leveling C CXX)

#SET(ASM_OPTIONS "-x assembler-with-cpp")
#SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )
SET(CMAKE_C_FLAGS "${CFLAGS} -std=gnu99 -O2 -Wl,-u,vfprintf -lprintf_flt -lm" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wl,-u,vfprintf -lprintf_flt -lm")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Wire)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Wire/utility)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/AStar32U4)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/LSM303)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/L3G)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/LPS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/EnableInterrupt)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/EnableInterrupt/utility)

#print_board_list()
#print_programmer_list()

file(GLOB C_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS
    src/*.cpp src/*.c
    Wire/*.cpp Wire/utility/*.c
    AStar32U4/*.cpp
    LSM303/*.cpp
    L3G/*.cpp
    LPS/*.cpp
)


generate_arduino_firmware(rover5-leveling
SRCS ${C_SRC}
BOARD leonardo)

set(${FIRMWARE_NAME}_PORT /dev/ttyACM0)
