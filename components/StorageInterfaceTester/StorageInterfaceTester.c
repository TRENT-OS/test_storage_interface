/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "test_storage.h"
#include <camkes.h>

int run()
{
    const StorageIf_t chanMuxStorage_rpc =
    {
        .write   = chanMuxStorage_rpc_write,
        .read    = chanMuxStorage_rpc_read,
        .erase   = chanMuxStorage_rpc_erase,
        .getSize = chanMuxStorage_rpc_getSize
    };

    const Storage_t chanMuxStorage =
    {
        .interface = chanMuxStorage_rpc,
        .port      = chanMuxStorage_port
    };

    test_storage_size_pos                    (&chanMuxStorage);
    test_storage_writeReadEraseBegin_pos     (&chanMuxStorage);
    test_storage_writeReadEraseMid_pos       (&chanMuxStorage);
    test_storage_writeReadEraseEnd_pos       (&chanMuxStorage);

    test_storage_writeReadEraseZeroBytes_pos (&chanMuxStorage);

    // TODO Below tests are disabled as they crash chanMux.
    //test_storage_writeReadEraseOutside_neg   (&chanMuxStorage);
    //test_storage_writeReadEraseTooLarge_neg  (&chanMuxStorage);

    test_storage_neighborRegionsUntouched_pos(&chanMuxStorage);

    return 0;
}
