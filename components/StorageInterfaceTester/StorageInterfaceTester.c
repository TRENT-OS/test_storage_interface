/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "test_storage.h"
#include "LibDebug/Debug.h"

#include <camkes.h>

int run()
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

    Debug_LOG_INFO(
        "%s -> !!! All tests successfully completed.",
        get_instance_name());

    return 0;
}
