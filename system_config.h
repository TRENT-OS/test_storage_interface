/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 *
 * @brief System configuration
 */
#pragma once


//-----------------------------------------------------------------------------
// Debug
//-----------------------------------------------------------------------------

#if !defined(NDEBUG)
#   define Debug_Config_STANDARD_ASSERT
#   define Debug_Config_ASSERT_SELF_PTR
#else
#   define Debug_Config_DISABLE_ASSERT
#   define Debug_Config_NO_ASSERT_SELF_PTR
#endif

#define Debug_Config_LOG_LEVEL              Debug_LOG_LEVEL_DEBUG
#define Debug_Config_INCLUDE_LEVEL_IN_MSG
#define Debug_Config_LOG_WITH_FILE_LINE


//-----------------------------------------------------------------------------
// Memory
//-----------------------------------------------------------------------------
#define Memory_Config_USE_STDLIB_ALLOC


//-----------------------------------------------------------------------------
// CHanMux
//-----------------------------------------------------------------------------

// we can't make this an enum, because CAmkES does not understand enums.
#define CHANMUX_CHANNEL_NVM       6

//-----------------------------------------------------------------------------
// ChanMUX clients
//-----------------------------------------------------------------------------

// we can't make this an enum, because CAmkES does not understand enums.
#define CHANMUX_ID 101

//-----------------------------------------------------------------------------
// StorageServer clients
//-----------------------------------------------------------------------------

#define STORAGESERVER_TESTER_IF1_ID 1
#define STORAGESERVER_TESTER_IF2_ID 2
#define STORAGESERVER_TESTER_IF3_ID 3

//-----------------------------------------------------------------------------
// RamDisk
//-----------------------------------------------------------------------------
#define RAMDISK_SIZE_BYTES 3 * 2048
