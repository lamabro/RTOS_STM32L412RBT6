#include "unity.h"
#include "master_fsm.h"
#include "proto_frame.h"
#include <string.h>

static ProtocolState_t testProtoState;
static uint8_t frame[FRAME_SIZE];

void setUp(void)
{
    memset(&testProtoState, 0, sizeof(testProtoState));
    MasterFSM_Init(&testProtoState);
}

void tearDown(void)
{
}

static void build_test_frame(MessageID_t msgID, uint8_t addr, uint8_t payload)
{
    frame[0] = FRAME_SOF;
    frame[1] = (uint8_t)msgID;
    frame[2] = addr;
    frame[3] = 1;
    frame[4] = payload;
    frame[5] = CRC_dummy;
    frame[6] = FRAME_EOF;
}

void test_MasterFSM_HandleRxFrame_InvalidLength_DoesNotChangeState(void)
{
    build_test_frame(MSG_ID_ACK, 0x10, 0x10);
    MasterFSM_HandleRxFrame(frame, FRAME_SIZE - 1);

    TEST_ASSERT_EQUAL(MASTER_STATE_DISCOVERY, g_masterFsm.state);
    TEST_ASSERT_EQUAL_UINT8(0, testProtoState.statusByte.bits.commRunning);
}

void test_MasterFSM_HandleRxFrame_InvalidSofOrEof_DoesNotChangeState(void)
{
    build_test_frame(MSG_ID_ACK, 0x10, 0x10);
    frame[0] = 0x00;
    MasterFSM_HandleRxFrame(frame, FRAME_SIZE);

    TEST_ASSERT_EQUAL(MASTER_STATE_DISCOVERY, g_masterFsm.state);
    TEST_ASSERT_EQUAL_UINT8(0, testProtoState.statusByte.bits.commRunning);
}

void test_MasterFSM_HandleRxFrame_AckNewSlave_SetsDiscoveryAndCommRunning(void)
{
    build_test_frame(MSG_ID_ACK, 0x10, 0x10);
    MasterFSM_HandleRxFrame(frame, FRAME_SIZE);

    TEST_ASSERT_EQUAL(MASTER_STATE_DISCOVERY, g_masterFsm.state);
    TEST_ASSERT_EQUAL_UINT8(1, testProtoState.statusByte.bits.commRunning);
}

void test_MasterFSM_HandleRxFrame_AckExistingSlave_SetsNormalOperation(void)
{
    testProtoState.addressTable.slaveCount = 1;
    testProtoState.addressTable.slaveAddress[0] = 0x10;
    MasterFSM_Init(&testProtoState);

    build_test_frame(MSG_ID_ACK, 0x10, 0x10);
    MasterFSM_HandleRxFrame(frame, FRAME_SIZE);

    TEST_ASSERT_EQUAL(MASTER_STATE_NORMAL_OPERATION, g_masterFsm.state);
    TEST_ASSERT_EQUAL_UINT8(1, testProtoState.statusByte.bits.commRunning);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_MasterFSM_HandleRxFrame_InvalidLength_DoesNotChangeState);
    RUN_TEST(test_MasterFSM_HandleRxFrame_InvalidSofOrEof_DoesNotChangeState);
    RUN_TEST(test_MasterFSM_HandleRxFrame_AckNewSlave_SetsDiscoveryAndCommRunning);
    RUN_TEST(test_MasterFSM_HandleRxFrame_AckExistingSlave_SetsNormalOperation);
    return UNITY_END();
}
