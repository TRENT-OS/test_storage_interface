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
                .write   = ramDisk_rpc_write,
                .read    = ramDisk_rpc_read,
                .erase   = ramDisk_rpc_erase,
                .getSize = ramDisk_rpc_getSize
            },
            .port = ramDisk_port
        },
        // chanMuxStorage
        {
            .interface =
            {
                .write   = chanMuxStorage_rpc_write,
                .read    = chanMuxStorage_rpc_read,
                .erase   = chanMuxStorage_rpc_erase,
                .getSize = chanMuxStorage_rpc_getSize
            },
            .port = chanMuxStorage_port
        }
    };

    const size_t    storageUnderTest_sz = sizeof(storagesUnderTest)
                                        / sizeof(*storagesUnderTest);

    for(size_t idx = 0U; idx < storageUnderTest_sz; ++idx)
    {
        test_storage_size_pos                    (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseBegin_pos     (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseMid_pos       (idx, &storagesUnderTest[idx]);
        test_storage_writeReadEraseEnd_pos       (idx, &storagesUnderTest[idx]);

        test_storage_writeReadEraseZeroBytes_pos (idx, &storagesUnderTest[idx]);

        // TODO Below tests are disabled as they crash chanMux.
        // See https://jira.hensoldt-cyber.systems/browse/SEOS-1431 for more
        // details.
        //test_storage_writeReadEraseOutside_neg   (idx, &storagesUnderTest[idx]);
        //test_storage_writeReadEraseTooLarge_neg  (idx, &storagesUnderTest[idx]);

        test_storage_neighborRegionsUntouched_pos(idx, &storagesUnderTest[idx]);
    }

    return 0;
}
