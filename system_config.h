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

// we can't make this and enum, because CAmkES does not understand enums.
#define CHANMUX_CHANNEL_NVM_1       6
#define CHANMUX_CHANNEL_NVM_2       9

//-----------------------------------------------------------------------------
// ChanMUX clients
//-----------------------------------------------------------------------------

// we can't make this and enum, because CAmkES does not understand enums.
#define CHANMUX_ID_TESTER_1        101
#define CHANMUX_ID_TESTER_2        102
