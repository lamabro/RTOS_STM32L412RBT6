# Unit test CMake configuration for native host tests
# This directory is included by the root CMakeLists.txt

add_executable(test_master_fsm
    test_master_fsm.c
    ${CMAKE_SOURCE_DIR}/tests/unity/src/unity.c
    ${CMAKE_SOURCE_DIR}/TTA_Protocol/Master/master_fsm.c
    ${CMAKE_SOURCE_DIR}/TTA_Protocol/Common/proto_frame.c
)

target_include_directories(test_master_fsm PRIVATE
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/tests/unity/src
    ${CMAKE_SOURCE_DIR}/TTA_Protocol/Common
    ${CMAKE_SOURCE_DIR}/TTA_Protocol/Master
)

target_compile_definitions(test_master_fsm PRIVATE
    UNITY_INCLUDE_EXECUTABLE
)

add_test(NAME MasterFSMTest COMMAND test_master_fsm)
