/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "test_storage.h"
#include <camkes.h>

int run()
{
    const Storage_t storagesUnderTest[] =
    {
        // ramDisk
        {
            .interface =
            {
                .write        = ramDisk_rpc_write,
                .read         = ramDisk_rpc_read,
                .erase        = ramDisk_rpc_erase,
                .getSize      = ramDisk_rpc_getSize,
                .getBlockSize = ramDisk_rpc_getBlockSize,
                .getState     = ramDisk_rpc_getState
            },
            .port = OS_DATAPORT_ASSIGN(ramDisk_port)
        },
        // chanMuxStorage
        {
            .interface =
            {
                .write        = chanMuxStorage_rpc_write,
                .read         = chanMuxStorage_rpc_read,
                .erase        = chanMuxStorage_rpc_erase,
                .getSize      = chanMuxStorage_rpc_getSize,
                .getBlockSize = chanMuxStorage_rpc_getBlockSize,
                .getState     = chanMuxStorage_rpc_getState
            },
            .port = OS_DATAPORT_ASSIGN(chanMuxStorage_port)
        },
        // storageServer 1
        {
            .interface =
            {
                .write        = storageServer1_rpc_write,
                .read         = storageServer1_rpc_read,
                .erase        = storageServer1_rpc_erase,
                .getSize      = storageServer1_rpc_getSize,
                .getBlockSize = storageServer1_rpc_getBlockSize,
                .getState     = storageServer1_rpc_getState
            },
            .port = OS_DATAPORT_ASSIGN(storageServer1_port)
        },
        // storageServer 2
        {
            .interface =
            {
                .write        = storageServer2_rpc_write,
                .read         = storageServer2_rpc_read,
                .erase        = storageServer2_rpc_erase,
                .getSize      = storageServer2_rpc_getSize,
                .getBlockSize = storageServer2_rpc_getBlockSize,
                .getState     = storageServer2_rpc_getState
            },
            .port = OS_DATAPORT_ASSIGN(storageServer2_port)
        },
        // storageServer 3
        {
            .interface =
            {
                .write        = storageServer3_rpc_write,
                .read         = storageServer3_rpc_read,
                .erase        = storageServer3_rpc_erase,
                .getSize      = storageServer3_rpc_getSize,
                .getBlockSize = storageServer3_rpc_getBlockSize,
                .getState     = storageServer3_rpc_getState
            },
            .port = OS_DATAPORT_ASSIGN(storageServer3_port)
        }
    };

    const size_t    storageUnderTest_sz = sizeof(storagesUnderTest)
                                        / sizeof(*storagesUnderTest);

    for(size_t idx = 0U; idx < storageUnderTest_sz; ++idx)
    {
        test_storage_size_pos                    (idx, &storagesUnderTest[idx]);
        test_storage_blockSize_pos               (idx, &storagesUnderTest[idx]);
        test_storage_state_pos                   (idx, &storagesUnderTest[idx]);

        test_storage_writeReadEraseBegin_pos     (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseMid_pos       (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseEnd_pos       (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseZeroBytes_pos (idx, &storagesUnderTest[idx]);
        test_storage_neighborRegionsUntouched_pos(idx, &storagesUnderTest[idx]);

        // test_storage_writeReadEraseLargerThanBuf_neg(
        //     idx, &storagesUnderTest[idx]);

        test_storage_writeReadEraseOutside_neg   (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseNegOffset_neg (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseIntMax_neg    (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseIntMin_neg    (idx, &storagesUnderTest[idx]);

        test_storage_writeReadEraseSizeTooLarge_neg(
            idx, &storagesUnderTest[idx]);

        test_storage_writeReadEraseSizeMax_neg(idx, &storagesUnderTest[idx]);
    }

    return 0;
}
