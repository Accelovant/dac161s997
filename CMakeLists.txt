cmake_minimum_required( VERSION 3.12...3.13 )

project( dac161s997_lib
         VERSION 0.0.1
         DESCRIPTION "Platform independent driver for the DAC161S997 4/20mA driver chip" )

add_library( dac161s997 OBJECT )
target_sources( dac161s997
    PRIVATE   "src/dac161s997.c"
              "src/dac161s997_regs.c"
              "include/internal/dac161s997_regs.h"
    PUBLIC    "${CMAKE_CURRENT_SOURCE_DIR}/include/dac161s997.h"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/dac161s997_port.h"
              "${CMAKE_CURRENT_SOURCE_DIR}/include/dac161s997_types.h")

target_include_directories( dac161s997
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
