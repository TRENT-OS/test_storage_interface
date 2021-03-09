/* Copyright (C) 2020, Hensoldt Cyber GmbH */
#pragma once

//------------------------------------------------------------------------------
// StorageServer Configuration
//
// Depending on the platform we want to write to the SD Card at a different
// offset, which is done with the help of the StorageServer.
//
// Note that TESTAPP_STORAGE_SIZE is chosen arbitrarily as it must be at least
// three times bigger than the TEST_DATA_SZ (see test_storage_size_pos()).
//------------------------------------------------------------------------------
#define MiB                     (1024 * 1024)

// Reserved for GPT
#define GPT_STORAGE_SIZE        (1 * MiB)

#define BOOT_STORAGE_OFFSET     GPT_STORAGE_SIZE
#define BOOT_STORAGE_SIZE       (128 * MiB)

#define TESTAPP_STORAGE_OFFSET  (BOOT_STORAGE_OFFSET + BOOT_STORAGE_SIZE)
#define TESTAPP_STORAGE_SIZE    (128 * MiB)

#include "plat.camkes"
