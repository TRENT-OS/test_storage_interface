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

// SPI_SD  
#define SPI_SD_STORAGE_OFFSET   0
#define SPI_SD_STORAGE_SIZE     (128 * MiB)

// integrated SD
#define SD_STORAGE_OFFSET       (20 * 4096)
#define SD_STORAGE_SIZE         (128 * MiB)

#include "plat.camkes"
