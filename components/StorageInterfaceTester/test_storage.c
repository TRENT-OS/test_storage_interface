/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 */

#include "test_storage.h"
#include "system_config.h"
#include "TestMacros.h"

#include <stdlib.h>

static const off_t storageBeginOffset = 0U;

/**
 * @brief   Random data used in the test.
 *
 * @note    If TEST_DATA_SIZE is changed, please update the content of this
 *          array as well, so that it is filled fully with random data.
 */
static const char testData[TEST_DATA_SIZE] =
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
    off_t value);

#define ERASED_PATTERN 0xFF

// Helper functions wrapped in macros so that we get the proper line number in
// a case of the failure.
#define TEST_WRITE(offset, data, size) do \
{ \
    const size_t roundedDownSize = roundDownToBLockSize(size); \
    size_t bytesWritten = 0U; \
\
    Debug_LOG_DEBUG( \
        "%s::TEST_WRITE(" \
        "offset = %" PRIiMAX ", data = %p, size = %zu, " \
        "roundedDownSize = %zu)",  \
        get_instance_name(), offset, data, size, roundedDownSize); \
\
    memcpy(storage_port, data, roundedDownSize); \
    TEST_SUCCESS( \
        storage_rpc_write( \
            roundDownToBLockSize(offset), \
            roundedDownSize, \
            &bytesWritten)); \
    ASSERT_EQ_SZ(roundedDownSize, bytesWritten); \
} while(0)

#define TEST_READ(offset, expectedData, size) do \
{ \
    const size_t roundedDownSize = roundDownToBLockSize(size); \
    size_t bytesRead = 0U; \
\
    Debug_LOG_DEBUG( \
        "%s::TEST_READ(" \
        "offset = %" PRIiMAX ", expectedData = %p, size = %zu, " \
        "roundedDownSize = %zu)",  \
        get_instance_name(), offset, expectedData, size, roundedDownSize); \
\
    memset(storage_port, 0, roundedDownSize); \
    TEST_SUCCESS( \
        storage_rpc_read( \
            roundDownToBLockSize(offset), \
            roundedDownSize, \
            &bytesRead)); \
\
    ASSERT_EQ_SZ(roundedDownSize, bytesRead); \
    ASSERT_EQ_INT( \
        0, \
        memcmp(storage_port, \
            expectedData, \
            roundedDownSize)); \
\
} while(0)

#define TEST_ERASE(offset, size) do \
{ \
    off_t bytesErased = -1; \
\
    Debug_LOG_DEBUG( \
        "%s::TEST_ERASE(" \
        "offset = %" PRIiMAX ", size = %zu, ", \
        get_instance_name(), offset, size); \
\
    const OS_Error_t rslt = storage_rpc_erase( \
                                roundDownToBLockSize(offset), \
                                roundDownToBLockSize(size), \
                                &bytesErased); \
\
    if(OS_ERROR_NOT_IMPLEMENTED == rslt) \
    { \
        /* Erase functionality is considered optional. */ \
        ASSERT_EQ_INT_MAX((intmax_t)0, bytesErased); \
\
        Debug_LOG_WARNING( \
            "Erase function is not implemented for %s. Was it intended?", \
            get_instance_name()); \
\
        break; \
    }\
\
    TEST_SUCCESS( \
        storage_rpc_erase( \
            roundDownToBLockSize(offset), \
            roundDownToBLockSize(size), \
            &bytesErased)); \
\
    ASSERT_EQ_INT_MAX( \
        (off_t)roundDownToBLockSize(size), \
        bytesErased); \
\
    uint8_t* expectedEraseData = malloc(size); \
    memset(expectedEraseData, ERASED_PATTERN, size); \
\
    TEST_READ(offset, expectedEraseData, size); \
    free(expectedEraseData); \
} while(0)

#define TEST_WRITE_READ_ERASE(offset) do \
{ \
    TEST_START(); \
\
    memset(storage_port, 0, TEST_DATA_SIZE); \
\
    TEST_WRITE(offset, testData, TEST_DATA_SIZE); \
    TEST_READ (offset, testData, TEST_DATA_SIZE); \
    TEST_ERASE(offset, TEST_DATA_SIZE); \
\
    TEST_FINISH(); \
} while (0)

void
test_storage_size_pos()
{
    TEST_START();

    off_t storageSize = 0U;
    TEST_SUCCESS(storage_rpc_getSize(&storageSize));

    Debug_LOG_DEBUG("%s size is: %" PRIiMAX, get_instance_name(), storageSize);

    ASSERT_LE_UINT64((off_t)(TEST_STORAGE_MIN_SIZE), storageSize);

    TEST_FINISH();
}

void
test_storage_blockSize_pos()
{
    TEST_START();

    size_t storageBlockSize = 0U;
    TEST_SUCCESS(storage_rpc_getBlockSize(&storageBlockSize));

    Debug_LOG_DEBUG(
        "%s block size is: %zu", get_instance_name(), storageBlockSize);

    // Different storages have different block sizes, but nevertheless the block
    // size must always be greater than 0.
    ASSERT_LT_SZ((size_t)0U, storageBlockSize);

    TEST_FINISH();
}

void
test_storage_state_pos()
{
    TEST_START();

    // Since state is implementation specific we call it and log it, but do not
    // evaluate the result. We are only expecting no crash.
    uint32_t flags;
    const OS_Error_t rslt = storage_rpc_getState(&flags);

    (void)rslt;
    Debug_LOG_DEBUG(
        "%s::storage_rpc_getState(&flags). flags = %u, rslt = %i",
        get_instance_name(),
        flags,
        rslt);

    TEST_FINISH();
}

void
test_storage_writeReadEraseBegin_pos()
{
    TEST_WRITE_READ_ERASE(storageBeginOffset);
}

void
test_storage_writeReadEraseMid_pos()
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage_rpc_getSize(&storageSize));
    const off_t middleOfStorage = storageSize / 2;

    TEST_WRITE_READ_ERASE(middleOfStorage);
}

void
test_storage_writeReadEraseEnd_pos()
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage_rpc_getSize(&storageSize));
    const off_t endOfStorage = storageSize - TEST_DATA_SIZE - 1;

    TEST_WRITE_READ_ERASE(endOfStorage);
}

void
test_storage_writeReadEraseZeroBytes_pos()
{
    TEST_START();

    memset(storage_port, 0, TEST_DATA_SIZE);

    const off_t testOffset = 0;

    TEST_WRITE(testOffset, testData, 0U);
    TEST_READ (testOffset, testData, 0U);

    off_t bytesErased = -1;

    const OS_Error_t rslt = storage_rpc_erase(
                                testOffset,
                                0U,
                                &bytesErased);

    // Erase implementation is considered optional but still function must
    // return zero bytes erased.
    if(OS_ERROR_NOT_IMPLEMENTED != rslt)
    {
        TEST_SUCCESS(rslt);
    }

    ASSERT_EQ_INT_MAX((intmax_t)0, bytesErased);

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
test_storage_neighborRegionsUntouched_pos()
{
    TEST_START();

    size_t storageBlockSize = 0U;
    TEST_SUCCESS(storage_rpc_getBlockSize(&storageBlockSize));

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

    Debug_LOG_TRACE("%s: Writing the front region.", get_instance_name());

    TEST_WRITE(
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    Debug_LOG_TRACE("%s: Verifying the front region.", get_instance_name());

    TEST_READ(
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

    const off_t untouchedBackAddress = testDataAddress + TEST_DATA_SIZE;

    Debug_LOG_TRACE("%s: Verifying the back region.", get_instance_name());
    TEST_WRITE(untouchedBackAddress, untouchedBack, untouchedBack_sz);
    Debug_LOG_TRACE("%s: Verifying the back region.", get_instance_name());
    TEST_READ (untouchedBackAddress, untouchedBack, untouchedBack_sz);

    // Writing verifying and erasing the test data.
    Debug_LOG_TRACE("%s: Writing the test data.", get_instance_name());
    TEST_WRITE(testDataAddress, testData, TEST_DATA_SIZE);
    Debug_LOG_TRACE("%s: Verifying the test data.", get_instance_name());
    TEST_READ (testDataAddress, testData, TEST_DATA_SIZE);
    Debug_LOG_TRACE("%s: Erasing the test data.", get_instance_name());
    TEST_ERASE(testDataAddress, TEST_DATA_SIZE);

    Debug_LOG_TRACE(
        "%s: Verifying that front region was untouched.", get_instance_name());
    TEST_READ(
        untouchedFrontAddress,
        untouchedFront,
        untouchedFront_sz);

    Debug_LOG_TRACE(
        "%s: Verifying that back region was untouched.", get_instance_name());
    TEST_READ(
        untouchedBackAddress,
        untouchedBack,
        untouchedBack_sz);

    //Clean up
    TEST_ERASE(
        untouchedFrontAddress,
        (untouchedFront_sz + TEST_DATA_SIZE + untouchedBack_sz));

    TEST_FINISH();

    free(untouchedFront);
    free(untouchedBack);
}

#define TEST_WRITE_NEG(offset, size) do \
{ \
    const size_t roundedDownSize = roundDownToBLockSize(size); \
    size_t bytesWritten = (size_t)-1; \
\
    Debug_LOG_DEBUG( \
        "%s::TEST_WRITE_NEG(" \
        "offset = %" PRIiMAX ", size = %" PRIiMAX ")" \
        "roundedDownSize = %zu)",  \
        get_instance_name(), offset, size, roundedDownSize); \
\
    memcpy(storage_port, testData, TEST_DATA_SIZE); \
\
    TEST_NOT_SUCCESS( \
        storage_rpc_write(offset, roundedDownSize, &bytesWritten)); \
\
    ASSERT_EQ_SZ((size_t)0U, bytesWritten); \
} while (0)

#define TEST_READ_NEG(offset, size) do \
{ \
    const size_t roundedDownSize = roundDownToBLockSize(size); \
    size_t bytesRead = (size_t)-1; \
\
    Debug_LOG_DEBUG( \
        "%s::TEST_READ_NEG(" \
        "offset = %" PRIiMAX ", size = %" PRIiMAX ")" \
        "roundedDownSize = %zu)",  \
        get_instance_name(), offset, size, roundedDownSize); \
\
    memset(storage_port, 0, TEST_DATA_SIZE); \
    TEST_NOT_SUCCESS( \
        storage_rpc_read(offset, roundedDownSize, &bytesRead)); \
\
    ASSERT_EQ_SZ((size_t)0U, bytesRead); \
} while (0)

#define TEST_ERASE_NEG(offset, size) do \
{ \
    off_t bytesErased = -1; \
\
    Debug_LOG_DEBUG( \
        "%s::TEST_ERASE_NEG(" \
        "offset = %" PRIiMAX ", size = %" PRIiMAX ")", \
        get_instance_name(), offset, size); \
\
    TEST_NOT_SUCCESS( \
        storage_rpc_erase( \
            roundDownToBLockSize(offset), \
            roundDownToBLockSize(size), \
            &bytesErased)); \
\
    ASSERT_EQ_INT_MAX((intmax_t)0, bytesErased); \
} while (0)

#define TEST_WRITE_READ_ERASE_NEG(offset, size) do \
{ \
    TEST_START(); \
\
    TEST_WRITE_NEG(offset, size); \
    TEST_READ_NEG (offset, size); \
    TEST_ERASE_NEG(offset, size); \
\
    TEST_FINISH(); \
} while (0)

void
test_storage_writeReadEraseLargerThanBuf_neg ()
{
    // Writing more bytes than the dataport size.
    const off_t dataport_size = sizeof(storage_port) + TEST_DATA_SIZE;

    TEST_WRITE_READ_ERASE_NEG(dataport_size, (off_t)TEST_DATA_SIZE);
}

void
test_storage_writeReadEraseOutside_neg()
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage_rpc_getSize(&storageSize));

    TEST_WRITE_READ_ERASE_NEG(storageSize, (off_t)TEST_DATA_SIZE);
}

void
test_storage_writeReadEraseNegOffset_neg()
{
    TEST_WRITE_READ_ERASE_NEG(
        -((off_t)TEST_DATA_SIZE),
        (off_t)TEST_DATA_SIZE);
}

void
test_storage_writeReadEraseIntMax_neg()
{
    TEST_WRITE_READ_ERASE_NEG(
        (off_t)INTMAX_MAX,
        (off_t)TEST_DATA_SIZE);
}

void
test_storage_writeReadEraseIntMin_neg()
{
    TEST_WRITE_READ_ERASE_NEG(
        (off_t)INTMAX_MIN,
        (off_t)TEST_DATA_SIZE);
}

void
test_storage_writeReadEraseSizeTooLarge_neg()
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage_rpc_getSize(&storageSize));

    ++storageSize; // Writing more bytes than the storage size.

    TEST_WRITE_READ_ERASE_NEG(
        storageBeginOffset,
        storageSize);
}

void
test_storage_writeReadEraseSizeMax_neg()
{
    off_t storageSize = 0U;
    TEST_SUCCESS(storage_rpc_getSize(&storageSize));

    ++storageSize; // Writing more bytes than the storage size.

    TEST_WRITE_READ_ERASE_NEG(
        storageBeginOffset,
        (off_t)SIZE_MAX);
}

off_t
roundDownToBLockSize(
    off_t value)
{
    size_t storageBlockSize = 0U;
    TEST_SUCCESS(storage_rpc_getBlockSize(&storageBlockSize));

    const off_t adjustedValue = (value / storageBlockSize)
                                        * storageBlockSize;

    Debug_LOG_DEBUG(
        "%s: Adjusting given value to be aligned with the block size: "
        "value = %" PRIiMAX " "
        "adjustedValue = %" PRIiMAX,
        get_instance_name(),
        value,
        adjustedValue);

    return adjustedValue;
}

void
test_storage_apiWithMediumNotPresent()
{
    size_t   dummySize   = 0;
    off_t    dummyOffset = 0;
    uint32_t flags;

    TEST_START();
    TEST_DEVICE_NOT_PRESENT(storage_rpc_write(0, 1, &dummySize));
    TEST_DEVICE_NOT_PRESENT(storage_rpc_read(0, 1, &dummySize));
    TEST_NOT_IMPLEMENTED(storage_rpc_erase(0, 1, &dummyOffset));
    TEST_DEVICE_NOT_PRESENT(storage_rpc_getSize(&dummyOffset));
    TEST_DEVICE_NOT_PRESENT(storage_rpc_getBlockSize(&dummySize));
    TEST_DEVICE_NOT_PRESENT(storage_rpc_getState(&flags));
    TEST_FINISH()
}
