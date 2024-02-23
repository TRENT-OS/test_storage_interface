#
# Test Storage Interface
#
# Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
# 
# SPDX-License-Identifier: GPL-2.0-or-later
#
# For commercial licensing, contact: info.cyber@hensoldt.net
#

cmake_minimum_required(VERSION 3.7.2)

find_package("os-sdk" REQUIRED)
os_sdk_set_defaults()
os_sdk_setup(CONFIG_FILE "system_config.h" CONFIG_PROJECT "system_config")


#-------------------------------------------------------------------------------
project(test_storage_interface C)

CAmkESAddCPPInclude("plat/${PLATFORM}")
include("plat/${PLATFORM}/plat.cmake")

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

RamDisk_DeclareCAmkESComponent(
    RamDisk
)

StorageServer_DeclareCAmkESComponent(
    StorageServer
)

SdHostController_DeclareCAmkESComponent(
    SdHostController
)

DeclareCAmkESComponent_SysLogger(
    SysLogger
    system_config
)

os_sdk_create_CAmkES_system("main.camkes")
