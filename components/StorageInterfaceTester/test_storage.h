/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Generic storage interface tests
 *
 * It is indpendent of what the underlying storage is used. The only assumption
 * is that it is three times bigger than the test data.
 *
 */
#pragma once

#include "OS_Error.h"
#include "stddef.h"

typedef
OS_Error_t
(*StorageAction_t)(
    size_t  const address,
    size_t  const size,
    size_t* const result);

typedef
OS_Error_t (*StorageGetter_t)(size_t* result);

typedef struct StorageIf
{
    StorageAction_t write;
    StorageAction_t read;
    StorageAction_t erase;
    StorageGetter_t getSize;
} StorageIf_t;

typedef struct Storage
{
    StorageIf_t interface;
    void*       port;
} Storage_t;

void test_storage_size_pos                    (const Storage_t* storage);

void test_storage_writeReadEraseBegin_pos     (const Storage_t* storage);
void test_storage_writeReadEraseMid_pos       (const Storage_t* storage);
void test_storage_writeReadEraseEnd_pos       (const Storage_t* storage);
void test_storage_writeReadEraseZeroBytes_pos (const Storage_t* storage);
void test_storage_writeReadEraseOutside_neg   (const Storage_t* storage);
void test_storage_writeReadEraseTooLarge_neg  (const Storage_t* storage);

void test_storage_neighborRegionsUntouched_pos(const Storage_t* storage);
