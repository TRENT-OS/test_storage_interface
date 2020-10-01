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
        os_libs
)

DeclareCAmkESComponent(
    Iomux
    SOURCES
        components/Iomux/Iomux.c
    C_FLAGS
        -Wall -Werror
    LIBS
        system_config
        os_core_api
        os_libs
)

#DeclareCAmkESComponent_UART(
#    UART
#)
#
#DeclareCAmkESComponent_ChanMux(
#    ChanMux
#    components/ChanMux/ChanMux_config.c
#    system_config
#)
#
#DeclareCAmkESComponent_ChanMux_Storage(
#    ChanMux_Storage
#)
#
#DeclareCAmkESComponent_RamDisk(
#    RamDisk
#    system_config
#)

#DeclareCAmkESComponent_StorageServer(
#    StorageServer
#)

DeclareCAmkESComponent_SdHostController(
    Sdhc
)

#ROOT SERVER
DeclareAndCreateCamkESSystem(main.camkes)
GenerateSimulateScript()
