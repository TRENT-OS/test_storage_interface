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
#include "OS_Dataport.h"
#include "stddef.h"
#include "stdio.h"

void test_storage_size_pos();
void test_storage_blockSize_pos();
void test_storage_state_pos();

void test_storage_writeReadEraseBegin_pos();
void test_storage_writeReadEraseMid_pos();
void test_storage_writeReadEraseEnd_pos();

void test_storage_writeReadEraseZeroBytes_pos();
void test_storage_neighborRegionsUntouched_pos();

void test_storage_writeReadEraseLargerThanBuf_neg();
void test_storage_writeReadEraseOutside_neg();
void test_storage_writeReadEraseNegOffset_neg();
void test_storage_writeReadEraseIntMax_neg();
void test_storage_writeReadEraseIntMin_neg();

void test_storage_writeReadEraseSizeTooLarge_neg();
void test_storage_writeReadEraseSizeMax_neg();

void test_storage_apiWithMediumNotPresent();
