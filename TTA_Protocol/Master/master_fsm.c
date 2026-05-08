#include "master_fsm.h"
#include "master_database.h"
#include "proto_frame.h"

/**
 * @file master_fsm.c
 * @brief Implementation of the master-side protocol state machine.
 */

MasterFSM_t g_masterFsm;

/* Internal helpers */
static void build_discovery_frame(uint8_t *txBuf);
static void build_presence_check_frame(uint8_t *txBuf);
static void build_status_request_frame(uint8_t *txBuf);

/**
 * @brief Initialize FSM and database.
 */
void MasterFSM_Init(ProtocolState_t *protoState)
{
    g_masterFsm.state      = MASTER_STATE_INIT;
    g_masterFsm.protoState = protoState;

    MasterDb_Init();
}

/**
 * @brief Called periodically (e.g., every 40 ms).
 *        Handles state transitions.
 */
void MasterFSM_OnTimerTick(void)
{
    switch (g_masterFsm.state)
    {
        case MASTER_STATE_INIT:
            g_masterFsm.state = MASTER_STATE_DISCOVERY;
            break;

        case MASTER_STATE_DISCOVERY:
            if (g_masterFsm.protoState->addressTable.slaveCount > 0)
                g_masterFsm.state = MASTER_STATE_PRESENCE_CHECK;
            break;

        case MASTER_STATE_PRESENCE_CHECK:
            g_masterFsm.state = MASTER_STATE_NORMAL_OPERATION;
            break;

        case MASTER_STATE_NORMAL_OPERATION:
        default:
            break;
    }
}

/**
 * @brief Handle received frame from slave.
 */
void MasterFSM_HandleRxFrame(const uint8_t *frame, uint8_t len)
{
    if (len != FRAME_SIZE)
        return;

    if (frame[0] != FRAME_SOF || frame[6] != FRAME_EOF)
        return;

    MessageID_t msgID = (MessageID_t)frame[1];
    uint8_t payload   = frame[4];

    switch (msgID)
    {
        case MSG_ID_ACK:
            g_masterFsm.protoState->statusByte.bits.commRunning = 1;
            break;

        default:
            break;
    }

    (void)payload;
}

/**
 * @brief Build next TX frame based on FSM state.
 */
void MasterFSM_RequestNextTxFrame(uint8_t *txBuf)
{
    switch (g_masterFsm.state)
    {
        case MASTER_STATE_DISCOVERY:
            build_discovery_frame(txBuf);
            break;

        case MASTER_STATE_PRESENCE_CHECK:
            build_presence_check_frame(txBuf);
            break;

        case MASTER_STATE_NORMAL_OPERATION:
            build_status_request_frame(txBuf);
            break;

        default:
            build_discovery_frame(txBuf);
            break;
    }
}

/* ---------------- Internal frame builders ---------------- */

static void build_discovery_frame(uint8_t *txBuf)
{
    ProtoFrame_Build(txBuf,
                     MSG_ID_DISCOVER,
                     MASTER_ADDRESS,
                     0x00,
                     0x00);
}

static void build_presence_check_frame(uint8_t *txBuf)
{
    if (g_masterFsm.protoState->addressTable.slaveCount == 0)
    {
        build_discovery_frame(txBuf);
        return;
    }

    uint8_t addr = g_masterFsm.protoState->addressTable.slaveAddress[0];

    ProtoFrame_Build(txBuf,
                     MSG_ID_ACK,
                     addr,
                     0x01,
                     0xC0);
}

static void build_status_request_frame(uint8_t *txBuf)
{
    if (g_masterFsm.protoState->addressTable.slaveCount == 0)
    {
        build_discovery_frame(txBuf);
        return;
    }

    uint8_t addr = g_masterFsm.protoState->addressTable.slaveAddress[0];

    ProtoFrame_Build(txBuf,
                     MSG_ID_STATUS_REQ,
                     addr,
                     0x01,
                     0x01);
}
