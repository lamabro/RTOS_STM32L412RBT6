#ifndef MASTER_FSM_H
#define MASTER_FSM_H

#include <stdint.h>
#include "proto_defs.h"

/**
 * @file master_fsm.h
 * @brief Master-side finite state machine for protocol control.
 */

typedef enum
{
    MASTER_STATE_INIT = 0,          /**< Initial state */
    MASTER_STATE_DISCOVERY,         /**< Searching for slaves */
    MASTER_STATE_PRESENCE_CHECK,    /**< Confirming discovered slaves */
    MASTER_STATE_NORMAL_OPERATION   /**< Polling + commands */
} MasterState_t;

/**
 * @brief Master FSM context.
 */
typedef struct
{
    MasterState_t state;
    ProtocolState_t *protoState;
} MasterFSM_t;

extern MasterFSM_t g_masterFsm;

/* API */
void MasterFSM_Init(ProtocolState_t *protoState);
void MasterFSM_OnTimerTick(void);
void MasterFSM_HandleRxFrame(const uint8_t *frame, uint8_t len);
void MasterFSM_RequestNextTxFrame(uint8_t *txBuf);

#endif // MASTER_FSM_H
