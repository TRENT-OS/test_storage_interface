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
#include "stdio.h"

typedef
OS_Error_t
(*StorageAction_t)(
    off_t   const offset,
    size_t  const size,
    size_t* const result);

typedef
OS_Error_t
(*StorageErase_t)(
    off_t  const offset,
    off_t  const size,
    off_t* const erased);

typedef
OS_Error_t (*StorageGetSize_t)(off_t* result);

typedef
OS_Error_t (*StorageGetBlockSize_t)(size_t* result);

typedef
typedef struct StorageIf
{
    StorageAction_t         write;
    StorageAction_t         read;
    StorageErase_t          erase;
    StorageGetSize_t        getSize;
    StorageGetBlockSize_t   getBlockSize;
} StorageIf_t;

typedef struct Storage
{
    StorageIf_t interface;
    void*       port;
} Storage_t;

void test_storage_size_pos                          (int idx, const Storage_t*);
void test_storage_blockSize_pos                     (int idx, const Storage_t*);

void test_storage_writeReadEraseBegin_pos           (int idx, const Storage_t*);
void test_storage_writeReadEraseMid_pos             (int idx, const Storage_t*);
void test_storage_writeReadEraseEnd_pos             (int idx, const Storage_t*);

void test_storage_writeReadEraseZeroBytes_pos       (int idx, const Storage_t*);
void test_storage_neighborRegionsUntouched_pos      (int idx, const Storage_t*);

void test_storage_writeReadEraseOutside_neg         (int idx, const Storage_t*);
void test_storage_writeReadEraseNegOffset_neg       (int idx, const Storage_t*);
void test_storage_writeReadEraseIntMax_neg          (int idx, const Storage_t*);
void test_storage_writeReadEraseIntMin_neg          (int idx, const Storage_t*);

void test_storage_writeReadEraseSizeTooLarge_neg    (int idx, const Storage_t*);
void test_storage_writeReadEraseSizeMax_neg         (int idx, const Storage_t*);
