#include "master_fsm.h"
#include "master_database.h"
#include "proto_frame.h"

/**
 * @file master_fsm.c

 typedef struct
{
    MasterState_t state;
    ProtocolState_t *protoState;
} MasterFSM_t;

typedef struct
{
    SlaveAddressTable_t addressTable;   // Discovered slaves
    CommErrorByte_t     errorByte;      // Error flags
    CommStatusByte_t    statusByte;     // Status flags
} ProtocolState_t;

typedef struct
{
    uint8_t slaveCount;                // Number of discovered slaves
    uint8_t slaveAddress[MAX_SLAVES];  // List of slave addresses
} SlaveAddressTable_t;





 * @brief Implementation of the master-side protocol state machine.
 */


//define MASTER_ADDRESS 0x01

MasterFSM_t g_masterFsm;

/* Internal helpers */
// GOT Error and removed unused functions to reduce code size and complexity

//static void build_discovery_frame(uint8_t *txBuf);
//static void build_presence_check_frame(uint8_t *txBuf);
//static void build_status_request_frame(uint8_t *txBuf);

/**
 * @brief Initialize FSM and database.
 
 */
void MasterFSM_Init(ProtocolState_t *protoState)
{
    g_masterFsm.state      = MASTER_STATE_DISCOVERY;
    g_masterFsm.protoState = protoState;

   // MasterDb_Init();
}



// This is testing function to check build_presence_check_frame separately and setting conditions for that functions


void MasterFSM_Init_ACK(ProtocolState_t *protoState)
{
    g_masterFsm.state      = MASTER_STATE_PRESENCE_CHECK;
    g_masterFsm.protoState->addressTable.slaveAddress[g_masterFsm.protoState->addressTable.slaveCount++] = 0xA3;


   // MasterDb_Init();
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
 * @brief Handle received frame from slave.\typedef enum
{
    MSG_ID_DISCOVER      = 0x01,   // Master → broadcast: request slaves to announce themselves
    MSG_ID_ACK           = 0x02,   // Slave → master: acknowledge presence or command
    MSG_ID_STATUS_REQ    = 0x03,   // Master → slave: request GPIO status
    MSG_ID_COMMAND       = 0x04    // Master → slave: command to set GPIO or other action
} MessageID_t;

 */
void MasterFSM_HandleRxFrame(const uint8_t *frame, uint8_t len)
{
    if (len != FRAME_SIZE)
        return;

    if (frame[0] != FRAME_SOF || frame[6] != FRAME_EOF)
        return;

        // Extract message ID and payload

        /**
 * @brief Build next TX frame based on FSM state.

 ou are telling the compiler:

“This byte represents one of the defined protocol message types.”

This makes the code self‑documenting and easier to read. if (frame[1] == 0x02)   // what is 0x02?
if (msgID == MSG_ID_ACK)   // clear meaning
MASTER_STATE_DISCOVERY,         /**< Searching for slaves */
   

    MessageID_t msgID = (MessageID_t)frame[1];
    // 
    uint8_t payload   = frame[4];

    switch (msgID)
    {
        case MSG_ID_ACK:
            g_masterFsm.protoState->statusByte.bits.commRunning = 1;
            g_masterFsm.state = MASTER_STATE_NORMAL_OPERATION;

            break;

        case MSG_ID_DISCOVER:
            g_masterFsm.protoState->statusByte.bits.commRunning = 1;
            g_masterFsm.state =  MASTER_STATE_PRESENCE_CHECK,    /**< Confirming discovered slaves */
            g_masterFsm.protoState->addressTable.slaveAddress[g_masterFsm.protoState->addressTable.slaveCount++] = frame[2]

;
            break;

        case MSG_ID_STATUS_REQ:
            g_masterFsm.protoState->statusByte.bits.commRunning = 1;
            g_masterFsm.state = MASTER_STATE_NORMAL_OPERATION;
            break;
        

        case MSG_ID_COMMAND:
            g_masterFsm.protoState->statusByte.bits.commRunning = 1;
            g_masterFsm.state = MASTER_STATE_NORMAL_OPERATION;
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
