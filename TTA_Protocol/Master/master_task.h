
#include "protocol_api.h"
#include "master_fsm.h"
#include "master_scheduler.h"
#include "proto_frame.h"
#include "master_task.h"

static ProtocolState_t g_protoState;

void Protocol_Init(void);

void Protocol_GetNextTxFrame(uint8_t *txBuffer);

//void Protocol_HandleRxFrame(const uint8_t *rxBuffer, uint8_t len);
