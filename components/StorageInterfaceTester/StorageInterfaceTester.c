/*
 * Storage interface tester
 *
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "test_storage.h"
#include "lib_compiler/compiler.h"
#include "lib_debug/Debug.h"
#include "SysLoggerClient.h"

#include <camkes.h>

#include <string.h>

int run()
{
    DECL_UNUSED_VAR(OS_Error_t err) = SysLoggerClient_init(sysLogger_Rpc_log);
    Debug_ASSERT(err == OS_SUCCESS);

    uint32_t stateBitmap = 0;

    if (!strcmp(get_instance_name(), "tester_sdhc") &&
        storage_rpc_getState(&stateBitmap) == OS_ERROR_DEVICE_NOT_PRESENT)
    {
        // This test systems is used to test also the special case of a system
        // booting with no SD CARD. In this case the sd_card related storage
        // interface has to be tested differently. Basically it should just
        // return an error that says that the medium is not present and reject
        // further operations.
        test_storage_apiWithMediumNotPresent();
    }
    else
    {
        test_storage_size_pos();
        test_storage_blockSize_pos();
        test_storage_state_pos();

        test_storage_writeReadEraseBegin_pos();
        test_storage_writeReadEraseMid_pos();
        test_storage_writeReadEraseEnd_pos();
        test_storage_writeReadEraseZeroBytes_pos();
        test_storage_neighborRegionsUntouched_pos();

        // test_storage_writeReadEraseLargerThanBuf_neg();

        test_storage_writeReadEraseOutside_neg();
        test_storage_writeReadEraseNegOffset_neg();
        test_storage_writeReadEraseIntMax_neg();
        test_storage_writeReadEraseIntMin_neg();

        test_storage_writeReadEraseSizeTooLarge_neg();
        test_storage_writeReadEraseSizeMax_neg();
    }

    Debug_LOG_INFO(
        "%s -> !!! All tests successfully completed.",
        get_instance_name());

    return 0;
}
