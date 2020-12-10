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

#ChanMux_UART_DeclareCAmkESComponents(
#    ChanMux_UART
#    components/ChanMux/ChanMux_config.c
#    system_config
#)

#Storage_ChanMux_DeclareCAmkESComponent(
#    Storage_ChanMux
#)

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

TimeServer_DeclareCAmkESComponent(
    TimeServer
)

RPi_SPI_SD_DeclareCAmkESComponent (
    RPi_SPI_SD
    LIBS
        TimeServer_client
)

DeclareCAmkESComponent_RPI_SD_DRV (
    RPI_SD
    LIBS
        TimeServer_client
)

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/plat/${KernelPlatform}")

#ROOT SERVER
DeclareAndCreateCamkESSystem(main.camkes)
GenerateSimulateScript()
