/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

#include "test_storage.h"
#include "TestMacros.h"

#include <stdlib.h>
#include <stdint.h>

static const size_t storageBeginOffset = 0U;

static const char testData[]  =
"3FB9BA89457642D1ADC51DC168DEA760"
"A2244EA40ADE46A98E4DF7293A378CD5"
"0BB08286142F421F803C61AC35797D90"
"0CD1498862BB40109A353499669BB15A"
"1E3DE76CB6984BF3BB4CAC3A2707C08C"
"F1F58DBA0A904F17A848C18CEC4339CF"
"0C55AF938ECE440B8DFC66CC571D18FA"
"1CB4C86F947B448493F971C58342ABFB"
"6BC2221F244F4BF3986469512888E4CC"
"9D57055CBC324FF7988FE95FD3202BB9"
"4165255F6F814A95AD26B367C7222A8C"
"2A9BADC326934E089D435883D0B8FD91"
"8F29749E47994F7E86AEFE4E4F27F150"
"6A4BF5315CF147FC8132EA4C037A0CB5"
"0D2C2A3E25774F979F3099E07C8B5135"
"B169D1C823C445EB8683F6A37F10CAAD"
"9C50F7DB9A774D5C92AB955885DE78F5"
"44FEC1E6E15F4FA1AB085EA340BCE683"
"FD2FED47B2984740AD8C8339F81C9848"
"48B421069BF04E7C809517DB7EA6636A";

#define TEST_DATA_SZ (sizeof(testData) / sizeof(*testData))
#define ERASED_PATTERN 0xFF

// Helper functions wrapped in macros so that we get the proper line number in
// a case of the failure.
#define TEST_WRITE(storage, offset, data, size) do \
{ \
    size_t bytesWritten = 0U; \
    memcpy(storage->port, data, size); \
    TEST_SUCCESS(storage->interface.write(offset, size, &bytesWritten)); \
    ASSERT_EQ(size, bytesWritten); \
} while(0)

#define TEST_READ(storage, offset, expectedData, size) do \
{ \
    size_t bytesRead = 0U; \
    memset(storage->port, 0, size); \
    TEST_SUCCESS(storage->interface.read(offset, size, &bytesRead)); \
    ASSERT_EQ(size, bytesRead); \
    ASSERT_EQ(0, memcmp(storage->port, expectedData, size)); \
} while(0)

#define TEST_ERASE(storage, offset, size) do \
{ \
    size_t bytesErased = 0U; \
    TEST_SUCCESS(storage->interface.erase(offset, size, &bytesErased)); \
    ASSERT_EQ(size, bytesErased); \
\
    uint8_t* expectedEraseData = malloc(size); \
    memset(expectedEraseData, ERASED_PATTERN, size); \
\
    TEST_READ(storage, offset, expectedEraseData, size); \
    free(expectedEraseData); \
} while(0)

#define TEST_WRITE_READ_ERASE(storage, offset) do \
{ \
    TEST_START(); \
\
    memset(storage->port, 0, TEST_DATA_SZ); \
\
    TEST_WRITE(storage, offset, testData, TEST_DATA_SZ); \
    TEST_READ (storage, offset, testData, TEST_DATA_SZ); \
    TEST_ERASE(storage, offset, TEST_DATA_SZ); \
\
    TEST_FINISH(); \
} while (0)

void test_storage_size_pos(Storage_t const * const storage)
{
    TEST_START();

    size_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));

    // Storage shall be at least three times bigger than the test string, so
    // that it can be stored at the beginning, in the center and at the end of
    // the storage.
    ASSERT_LE((3 * TEST_DATA_SZ), storageSize);

    TEST_FINISH();
}

void test_storage_writeReadEraseBegin_pos(Storage_t const * const storage)
{
    TEST_WRITE_READ_ERASE(storage, storageBeginOffset);
}

void test_storage_writeReadEraseMid_pos(Storage_t const * const storage)
{
    size_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));
    const size_t middleOfStorage = storageSize / 2;

    TEST_WRITE_READ_ERASE(storage, middleOfStorage);
}

void test_storage_writeReadEraseEnd_pos(Storage_t const * const storage)
{
    size_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));
    const size_t endOfStorage = storageSize - TEST_DATA_SZ - 1;

    TEST_WRITE_READ_ERASE(storage, endOfStorage);
}

void test_storage_writeReadEraseZeroBytes_pos(Storage_t const * const storage)
{
    TEST_START();

    memset(storage->port, 0, TEST_DATA_SZ);

    const size_t testOffset = 0;

    TEST_WRITE(storage, testOffset, testData, 0U);
    TEST_READ (storage, testOffset, testData, 0U);

    size_t bytesErased = (size_t)-1;
    TEST_SUCCESS(storage->interface.erase(testOffset, 0U, &bytesErased));
    ASSERT_EQ(0U, bytesErased);

    TEST_FINISH();
}

void test_storage_writeReadEraseOutside_neg(Storage_t const * const storage)
{
    size_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));

    TEST_START();

    {
        size_t bytesWritten = (size_t)-1;
        memcpy(storage->port, testData, TEST_DATA_SZ);

        ASSERT_NE(
            OS_SUCCESS,
            storage->interface.write(storageSize, TEST_DATA_SZ, &bytesWritten));

        ASSERT_EQ(0U, bytesWritten);
    }

    {
        size_t bytesRead = (size_t)-1;
        memset(storage->port, 0, TEST_DATA_SZ);
        ASSERT_NE(
            OS_SUCCESS,
            storage->interface.read(storageSize, TEST_DATA_SZ, &bytesRead));

        ASSERT_EQ(0U, bytesRead);
    }

    {
        size_t bytesErased = (size_t)-1;
        ASSERT_NE(
            OS_SUCCESS,
            storage->interface.erase(storageSize, TEST_DATA_SZ, &bytesErased));

        ASSERT_EQ(0U, bytesErased);
    }

    TEST_FINISH();
}

void test_storage_writeReadEraseTooLarge_neg(Storage_t const * const storage)
{
    size_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));

    TEST_START();

    {
        size_t bytesWritten = (size_t)-1;

        ASSERT_NE(
            OS_SUCCESS,
            storage->interface.write(
                storageBeginOffset,
                storageSize,
                &bytesWritten));

        ASSERT_EQ(0U, bytesWritten);
    }

    {
        size_t bytesRead = (size_t)-1;

        ASSERT_NE(
            OS_SUCCESS,
            storage->interface.read(
                storageBeginOffset,
                storageSize,
                &bytesRead));

        ASSERT_EQ(0U, bytesRead);
    }

    {
        size_t bytesErased = (size_t)-1;
        ASSERT_NE(
            OS_SUCCESS,
            storage->interface.erase(
                storageBeginOffset,
                storageSize,
                &bytesErased));

        ASSERT_EQ(0U, bytesErased);
    }

    TEST_FINISH();
}

/**
 * @brief   Tests if memory regions in front and at the back of the desired area
 *          are untouched during the read, write and erase operations.
 *
 * Data layout of the storage during the test:
 *
 * region index: |        1       |     2    |       3       |
 * content:      | untouchedFront | testData | untouchedBack |
 *
 * Regions 1 and 3 shall never change when region 2 is manipulated.
 *
 */
void test_storage_neighborRegionsUntouched_pos(Storage_t const * const storage)
{
    TEST_START();

    // Setting up untouched region at the front
    const char untouchedFront[] = "Please don't overwrite me!";
    const size_t untouchedFrontAddress = storageBeginOffset;
    const size_t untouchedFront_sz = sizeof(untouchedFront)
                                   / sizeof(*untouchedFront);

    TEST_WRITE(
        storage,
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    TEST_READ(
        storage,
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    const size_t testDataAddress = untouchedFrontAddress + untouchedFront_sz;

    // Setting up untouched region at the back
    const char untouchedBack[] = "!em etirwrevo t'nod esaelP";
    const size_t untouchedBackAddress = testDataAddress + TEST_DATA_SZ;
    const size_t untouchedBack_sz = sizeof(untouchedBack)
                                  / sizeof(*untouchedBack);

    TEST_WRITE(storage, untouchedBackAddress, untouchedBack, untouchedBack_sz);
    TEST_READ (storage, untouchedBackAddress, untouchedBack, untouchedBack_sz);

    // Writing and verifying test data.
    TEST_WRITE(storage, testDataAddress, testData, TEST_DATA_SZ);
    TEST_READ (storage, testDataAddress, testData, TEST_DATA_SZ);
    TEST_ERASE(storage, testDataAddress, TEST_DATA_SZ);

    // Verifying that front and back regions were untouched.
    TEST_READ(
        storage,
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    TEST_READ(
        storage,
        untouchedBackAddress,
        untouchedBack,
        untouchedBack_sz);

    //Clean up
    TEST_ERASE(
        storage,
        untouchedFrontAddress,
        (untouchedFront_sz + TEST_DATA_SZ + untouchedBack_sz));

    TEST_FINISH();
}
