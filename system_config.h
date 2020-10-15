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

#if !defined(Debug_Config_LOG_LEVEL)
#define Debug_Config_LOG_LEVEL              Debug_LOG_LEVEL_DEBUG
#endif
#define Debug_Config_INCLUDE_LEVEL_IN_MSG
#define Debug_Config_LOG_WITH_FILE_LINE

//-----------------------------------------------------------------------------
// Logger
//-----------------------------------------------------------------------------
// The value here is arbitrary and depends on the intended length of the log
// message. It can be at most SysLogger_MAX_MSG_SIZE
#define SysLogger_Config_MSG_SIZE                256

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
/**
 * @brief   The storage size available for each of the storage server clients.
 *
 * @note    Must be three times larger than the test data array used in this
 *          test system.
 */
#define STORAGE_SERVER_CLIENT_SZ 4096

//-----------------------------------------------------------------------------
// RamDisk
//-----------------------------------------------------------------------------
/**
 * @brief   Size of the RAM Disks used in this test system
 *
 * @note    Must be large enough to hold data for all of the storage server
 *          clients.
 */
#define RAMDISK_SIZE_BYTES (3 * STORAGE_SERVER_CLIENT_SZ)
