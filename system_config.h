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
#define Debug_Config_LOG_LEVEL              Debug_LOG_LEVEL_INFO
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

/**
 * @brief   Size of the test data set.
 *
 * @note    Size of the test data set is a prime number so that it is very
 *          unlikely that it collides with any block size, and also it is bigger
 *          than two blocks of common 512 bytes.
 */
#define TEST_DATA_SIZE 1237

/**
 * @brief   The minimum test storage size.
 *
 * @note    Must be at least three times the size of the test data array used in
 *          this test system.
 */
#define TEST_STORAGE_MIN_SIZE   (3 * TEST_DATA_SIZE)
