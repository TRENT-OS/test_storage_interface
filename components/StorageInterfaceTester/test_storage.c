/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

#include "test_storage.h"
#include "TestMacros.h"

#include <stdlib.h>

static const off_t storageBeginOffset = 0U;

/**
 * @brief   Random data used in the test.
 *
 * @note    Size of the test data is a prime number so that it is very unlikely
 *          that it collides with any block size, and also it is bigger than two
 *          blocks of common 512 bytes.
 */
static const char testData[] =
    "537QNNHTI4PNJ207V9X4EQ6N7IT1S02EYBTUZOBDLL4IDSCDBJB6Y1QHX5JKIH6G"
    "05G73K3SIIFJ0D601PDZUM2N58472UBW5SO4T6YU8X7ZFH0LABTXLJ9GFNTR0A8Q"
    "AYTS13BDOOJM0M5J9PF51L3Z5M91SSJVFZI4TLJLXYHT5O9H3V3MK2W54I5FZQPA"
    "0S946EET067EZ3ZX00A9489UBKIS2XX0N9107MY161DEKMWOVI803DPV8WLCK7CT"
    "Z56Z3OXOSHQL1VKO3QE01MGJAIAREQBVISLQT76WZYO4QYFOI40EXWQA5LTBT239"
    "Q3UKGBO36JG58QSUPFD1SX3AJWTUIB7IRC8GWBFQ519QLMW9QJRRYW53UWJ8QM7S"
    "JT4R969S6545F3XUDEOZGUWMN2I1URKJWCKZFLQG6KFKW07LL37VJWTU5VWTWFRQ"
    "5H1CD34ZRASMAQJM5X37ABVFE7GER2FG2EZ5186ZSX51JPIFLBRR5J3FPJD78THO"
    // 512 bytes
    "DPM6RFQAME1CV1YMCOSFTJ9XVZH3XVO1FQEQ232J99B7PJTGQV0J36PHKHA4GLBA"
    "1V1L5X8YL1IP7RGX62DYJJ5Z4DMQT17PWT2OENMPVJQN5XW82T9CRLQREGVXUKU8"
    "GFFA9P1O8YG9SADTWKUY9YU8X6GDGP5FYD0APXRYQSZOKFXKVPH2CCL1N4PNJQL8"
    "QBM7EOFSFOH3KJ05MLI44PHBYDBYCYMYG2IFD8737LBRIFKNNSBFON5G2HJGSIP0"
    "1140L33HLUZWGBR7UQR8U3M5SM53AZPO3KF60YI8N5EI9QH2IRSI8AWCI7JD7HMW"
    "0OVZB0EPY3B57JP9FOSQ1SDKA96F5DWZMFU5YBF19PQBYUFVV8ZZ6ST90HCKXH5K"
    "RN0FPPUE3COM4I2CEWGUMYC38TEV91EZW5RPUX8HC2JGPFY846969G1UI2FTLBDT"
    "CYVUU9UCS48X6UMRVQXXPEVYW3UDLR89XJ0J7M5E4YGZ5TLG3J99D399Y9P90QF"
    // 1024 bytes
    "F85885C02D724F7989DEF58B854F96A4E1868D31E0C3448AA6454996AA89115C"
    "D79B4BB84F694AE8B243164251815348F703888C93C544BF8F9B1BD27434B26E"
    "DAF042AB20B643DA93C564443AA49384CBDA71A534E54E2E865354E4447C0347"
    "F783254358EB4A3FAFA89";
    // 1237 bytes

static
off_t
roundDownToBLockSize(
    Storage_t const * const storage,
    off_t value);

#define TEST_DATA_SZ (sizeof(testData) / sizeof(*testData))
#define ERASED_PATTERN 0xFF

// Helper functions wrapped in macros so that we get the proper line number in
// a case of the failure.
#define TEST_WRITE(storage, offset, data, size) do \
{ \
    const size_t roundedDownSize = roundDownToBLockSize(storage, size); \
    size_t bytesWritten = 0U; \
    memcpy(OS_Dataport_getBuf(storage->port), data, roundedDownSize); \
    TEST_SUCCESS( \
        storage->interface.write( \
            roundDownToBLockSize(storage, offset), \
            roundedDownSize, \
            &bytesWritten)); \
    ASSERT_EQ_SZ(roundedDownSize, bytesWritten); \
} while(0)

#define TEST_READ(storage, offset, expectedData, size) do \
{ \
    const size_t roundedDownSize = roundDownToBLockSize(storage, size); \
    size_t bytesRead = 0U; \
    memset(OS_Dataport_getBuf(storage->port), 0, roundedDownSize); \
    TEST_SUCCESS( \
        storage->interface.read( \
            roundDownToBLockSize(storage, offset), \
            roundedDownSize, \
            &bytesRead)); \
\
    ASSERT_EQ_SZ(roundedDownSize, bytesRead); \
    ASSERT_EQ_INT( \
        0, \
        memcmp(OS_Dataport_getBuf(storage->port), \
            expectedData, \
            roundedDownSize)); \
\
} while(0)

#define TEST_ERASE(storage, offset, size) do \
{ \
    off_t bytesErased = -1; \
    TEST_SUCCESS( \
        storage->interface.erase( \
            roundDownToBLockSize(storage, offset), \
            roundDownToBLockSize(storage, size), \
            &bytesErased)); \
\
    ASSERT_EQ_INT_MAX( \
        (off_t)roundDownToBLockSize(storage, size), \
        bytesErased); \
\
    uint8_t* expectedEraseData = malloc(size); \
    memset(expectedEraseData, ERASED_PATTERN, size); \
\
    TEST_READ(storage, offset, expectedEraseData, size); \
    free(expectedEraseData); \
} while(0)

#define TEST_WRITE_READ_ERASE(idx, storage, offset) do \
{ \
    TEST_START(idx); \
\
    memset(OS_Dataport_getBuf(storage->port), 0, TEST_DATA_SZ); \
\
    TEST_WRITE(storage, offset, testData, TEST_DATA_SZ); \
    TEST_READ (storage, offset, testData, TEST_DATA_SZ); \
    TEST_ERASE(storage, offset, TEST_DATA_SZ); \
\
    TEST_FINISH(); \
} while (0)

void test_storage_size_pos(int idx, Storage_t const * const storage)
{
    TEST_START(idx);

    off_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));

    Debug_LOG_DEBUG("Storage under test's size is: %" PRIiMAX, storageSize);

    // Storage shall be at least three times bigger than the test string, so
    // that it can be stored at the beginning, in the center and at the end of
    // the storage.
    ASSERT_LE_UINT64((off_t)(3 * TEST_DATA_SZ), storageSize);

    TEST_FINISH();
}

void test_storage_blockSize_pos(int idx, Storage_t const * const storage)
{
    TEST_START(idx);

    size_t storageBlockSize = 0U;
    TEST_SUCCESS(storage->interface.getBlockSize(&storageBlockSize));

    // Different storages have different block sizes, but nevertheless the block
    // size must always be greater than 0.
    ASSERT_LT_SZ(0U, storageBlockSize);

    TEST_FINISH();
}

void
test_storage_state_pos(
    int idx,
    Storage_t const * const storage)
{
    TEST_START(idx);

    // Since state is implementation specific we call it and log it, but do not
    // evaluate the result. We are only expecting no crash.
    uint32_t flags;
    const OS_Error_t rslt = storage->interface.getState(&flags);

    Debug_LOG_INFO(
        "Called storage->interface.getState(&flags). flags = %u, rslt = %i",
        flags,
        rslt);

    TEST_FINISH();
}

void
test_storage_writeReadEraseBegin_pos(
    int idx,
    Storage_t const * const storage)
{
    TEST_WRITE_READ_ERASE(idx, storage, storageBeginOffset);
}

void
test_storage_writeReadEraseMid_pos(
    int idx,
    Storage_t const * const storage)
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));
    const off_t middleOfStorage = storageSize / 2;

    TEST_WRITE_READ_ERASE(idx, storage, middleOfStorage);
}

void
test_storage_writeReadEraseEnd_pos(
    int idx,
    Storage_t const * const storage)
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));
    const off_t endOfStorage = storageSize - TEST_DATA_SZ - 1;

    TEST_WRITE_READ_ERASE(idx, storage, endOfStorage);
}

void
test_storage_writeReadEraseZeroBytes_pos(
    int idx,
    Storage_t const * const storage)
{
    TEST_START(idx);

    memset(OS_Dataport_getBuf(storage->port), 0, TEST_DATA_SZ);

    const off_t testOffset = 0;

    TEST_WRITE(storage, testOffset, testData, 0U);
    TEST_READ (storage, testOffset, testData, 0U);

    off_t bytesErased = -1;
    TEST_SUCCESS(storage->interface.erase(testOffset, 0U, &bytesErased));
    ASSERT_EQ_INT_MAX(0LL, bytesErased);

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
void
test_storage_neighborRegionsUntouched_pos(
    int idx,
    Storage_t const * const storage)
{
    TEST_START(idx);

    size_t storageBlockSize = 0U;
    TEST_SUCCESS(storage->interface.getBlockSize(&storageBlockSize));

    // Setting up untouched region at the front
    const char untouchedFrontContent[] = "Please don't overwrite me!";

    const size_t untouchedFront_sz =
        storageBlockSize > sizeof(untouchedFrontContent)
            ? storageBlockSize
            : sizeof(untouchedFrontContent);

    char* untouchedFront = malloc(untouchedFront_sz);

    memcpy(
        untouchedFront + untouchedFront_sz - sizeof(untouchedFrontContent),
        untouchedFrontContent,
        sizeof(untouchedFrontContent));

    const off_t untouchedFrontAddress = storageBeginOffset;

    Debug_LOG_TRACE("Writing the front region.");

    TEST_WRITE(
        storage,
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    Debug_LOG_TRACE("Verifying the front region.");

    TEST_READ(
        storage,
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    const off_t testDataAddress = untouchedFrontAddress + untouchedFront_sz;

    // Setting up untouched region at the back
    const char untouchedBackContent[] = "Do not ever touch this!";

    const size_t untouchedBack_sz =
        storageBlockSize > sizeof(untouchedBackContent)
            ? storageBlockSize
            : sizeof(untouchedBackContent);

    char* untouchedBack = malloc(untouchedBack_sz);

    memcpy(
        untouchedBack,
        untouchedBackContent,
        sizeof(untouchedBackContent));

    const off_t untouchedBackAddress = testDataAddress + TEST_DATA_SZ;

    Debug_LOG_TRACE("Verifying the back region.");
    TEST_WRITE(storage, untouchedBackAddress, untouchedBack, untouchedBack_sz);
    Debug_LOG_TRACE("Verifying the back region.");
    TEST_READ (storage, untouchedBackAddress, untouchedBack, untouchedBack_sz);

    // Writing verifying and erasing the test data.
    Debug_LOG_TRACE("Writing the test data.");
    TEST_WRITE(storage, testDataAddress, testData, TEST_DATA_SZ);
    Debug_LOG_TRACE("Verifying the test data.");
    TEST_READ (storage, testDataAddress, testData, TEST_DATA_SZ);
    Debug_LOG_TRACE("Erasing the test data.");
    TEST_ERASE(storage, testDataAddress, TEST_DATA_SZ);

    Debug_LOG_TRACE("Verifying that front region was untouched.");
    TEST_READ(
        storage,
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    Debug_LOG_TRACE("Verifying that back region was untouched.");
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

    free(untouchedFront);
    free(untouchedBack);
}

#define TEST_WRITE_READ_ERASE_NEG(idx, storage, offset, size) do               \
{                                                                              \
    TEST_START(idx);                                                           \
                                                                               \
    {                                                                          \
      size_t bytesWritten = (size_t)-1;                                        \
      memcpy(OS_Dataport_getBuf(storage->port), testData, TEST_DATA_SZ);       \
                                                                               \
      TEST_NOT_SUCCESS(                                                        \
          storage->interface.write(offset, size, &bytesWritten));              \
                                                                               \
      ASSERT_EQ_SZ(0U, bytesWritten);                                          \
    }                                                                          \
                                                                               \
    {                                                                          \
      size_t bytesRead = (size_t)-1;                                           \
      memset(OS_Dataport_getBuf(storage->port), 0, TEST_DATA_SZ);              \
      TEST_NOT_SUCCESS(                                                        \
          storage->interface.read(offset, size, &bytesRead));                  \
                                                                               \
      ASSERT_EQ_SZ(0U, bytesRead);                                             \
    }                                                                          \
                                                                               \
    {                                                                          \
      off_t bytesErased = -1;                                                  \
      TEST_NOT_SUCCESS(                                                        \
          storage->interface.erase(offset, size, &bytesErased));               \
                                                                               \
      ASSERT_EQ_INT_MAX(0LL, bytesErased);                                     \
    }                                                                          \
                                                                               \
    TEST_FINISH();                                                             \
} while (0)

void
test_storage_writeReadEraseLargerThanBuf_neg (
    int idx,
    Storage_t const * const storage)
{
    // Writing more bytes than the dataport size.
    const size_t dataport_size = OS_Dataport_getSize(
                                    storage->port) + TEST_DATA_SZ;

    TEST_WRITE_READ_ERASE_NEG(idx, storage, dataport_size, TEST_DATA_SZ);
}

void
test_storage_writeReadEraseOutside_neg(
    int idx,
    Storage_t const * const storage)
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));

    TEST_WRITE_READ_ERASE_NEG(idx, storage, storageSize, TEST_DATA_SZ);
}

void
test_storage_writeReadEraseNegOffset_neg(
    int idx,
    Storage_t const * const storage)
{
    TEST_WRITE_READ_ERASE_NEG(idx, storage, -1, TEST_DATA_SZ);
}

void
test_storage_writeReadEraseIntMax_neg(
    int idx,
    Storage_t const * const storage)
{
    TEST_WRITE_READ_ERASE_NEG(idx, storage, INTMAX_MAX, TEST_DATA_SZ);
}

void
test_storage_writeReadEraseIntMin_neg(
    int idx,
    Storage_t const * const storage)
{
    TEST_WRITE_READ_ERASE_NEG(idx, storage, INTMAX_MIN, TEST_DATA_SZ);
}

void
test_storage_writeReadEraseSizeTooLarge_neg(
    int idx,
    Storage_t const * const storage)
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));

    ++storageSize; // Writing more bytes than the storage size.

    TEST_WRITE_READ_ERASE_NEG(idx, storage, storageBeginOffset, storageSize);
}

void
test_storage_writeReadEraseSizeMax_neg(
    int idx,
    Storage_t const * const storage)
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage->interface.getSize(&storageSize));

    ++storageSize; // Writing more bytes than the storage size.

    TEST_WRITE_READ_ERASE_NEG(idx, storage, storageBeginOffset, SIZE_MAX);
}

off_t
roundDownToBLockSize(
    Storage_t const * const storage,
    off_t value)
{
    size_t storageBlockSize = 0U;
    TEST_SUCCESS(storage->interface.getBlockSize(&storageBlockSize));

    const off_t adjustedValue = (value / storageBlockSize)
                                        * storageBlockSize;

    Debug_LOG_DEBUG(
        "Adjusting given value to be aligned with the block size: "
        "value = %" PRIiMAX " "
        "adjustedValue = %" PRIiMAX,
        value,
        adjustedValue);

    return adjustedValue;
}
