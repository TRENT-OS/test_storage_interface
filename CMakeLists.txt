#
# Test Storage Interface
#
# Copyright (C) 2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.7.2)

os_set_config_file(system_config.h)

project(system_config C)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})


project(test_storage_interface C)

# Overwrite the default log level of the underlying Data61 libraries to only
# print error logs as the SdHostController driver otherwise prints a lot of
# debug logs and clutters the output.
set(LibUtilsDefaultZfLogLevel 5 CACHE STRING "" FORCE)

DeclareCAmkESComponent(
    StorageInterfaceTester
    SOURCES
        components/StorageInterfaceTester/StorageInterfaceTester.c
        components/StorageInterfaceTester/test_storage.c
    C_FLAGS
        -Wall -Werror
    LIBS
        system_config
        os_core_api
        lib_compiler
        lib_debug
        syslogger_client
)

if(NOT (${PLATFORM} STREQUAL "rpi3"))
ChanMux_UART_DeclareCAmkESComponents(
    ChanMux_UART
    components/ChanMux/ChanMux_config.c
    system_config
)

Storage_ChanMux_DeclareCAmkESComponent(
    Storage_ChanMux
)
endif()

RamDisk_DeclareCAmkESComponent(
    RamDisk
    system_config
)

StorageServer_DeclareCAmkESComponent(
    StorageServer
)

SdHostController_DeclareCAmkESComponent(
    SdHostController
)

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/plat/${KernelPlatform}")

DeclareCAmkESComponent_SysLogger(
    SysLogger
    system_config
)

#ROOT SERVER
DeclareAndCreateCamkESSystem(main.camkes)
GenerateSimulateScript()
