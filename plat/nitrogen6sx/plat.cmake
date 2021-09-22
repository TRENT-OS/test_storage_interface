# Copyright (C) 2021, HENSOLDT Cyber GmbH

cmake_minimum_required(VERSION 3.7.2)

ChanMux_UART_DeclareCAmkESComponents(
    ChanMux_UART
    components/ChanMux/ChanMux_config.c
    system_config
)

Storage_ChanMux_DeclareCAmkESComponent(
    Storage_ChanMux
)
