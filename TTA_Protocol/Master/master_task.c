#include "protocol_api.h"
#include "master_fsm.h"
#include "master_scheduler.h"
#include "proto_frame.h"
#include "master_task.h"

static ProtocolState_t g_protoState;

void Protocol_Init(void)
{
    memset(&g_protoState, 0, sizeof(g_protoState));
 //   MasterScheduler_Init();
    MasterFSM_Init(&g_protoState);
}

void Protocol_GetNextTxFrame(uint8_t *txBuffer)
{
    // FSM decides what to send based on state
    //Master Schelurar checks errors , Slave address etc before sending the frame
    MasterFSM_RequestNextTxFrame(txBuffer);
}

void Protocol_HandleRxFrame(const uint8_t *rxBuffer, uint8_t len)
{

    // Parse frame and update FSM state/database, 
    // Verify frame integrity, check addresses, etc, SOF, EFO, CRC etc
    // 
    MasterFSM_HandleRxFrame(rxBuffer, len);
}


