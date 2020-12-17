cmake_minimum_required( VERSION 3.12...3.13 )

set(CMAKE_C_COMPILER "gcc")

project( dac161s997_example
         VERSION 0.0.0
         DESCRIPTION "Example for the DAC161S997 4/20mA driver chip" )

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable( dac161s997_bin
                "main.c"
                "mock_spi.c")

set_target_properties( dac161s997_bin
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            )

target_link_libraries(dac161s997_bin dac161s997)