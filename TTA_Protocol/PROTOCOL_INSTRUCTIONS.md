
 ┌──────────────────────────────┐
 │        Application Layer      │
 │  (main.c or Master TX Task)   │
 └───────────────┬──────────────┘
                 │ calls
                 ▼
 ┌──────────────────────────────────────┐
 │          protocol_api.c              │
 │  Protocol_GetNextTxFrame(txBuf)      │
 └───────────────┬──────────────────────┘
                 │ calls
                 ▼
 ┌──────────────────────────────────────┐
 │            master_fsm.c              │
 │  MasterFSM_RequestNextTxFrame()      │
 │   - checks current state             │
 │   - asks scheduler which slave       │
 │   - selects correct frame type       │
 └───────────────┬──────────────────────┘
                 │ calls
                 ▼
 ┌──────────────────────────────────────┐
 │         master_scheduler.c           │
 │  MasterScheduler_GetNextTarget()     │
 │   - round‑robin or offset logic      │
 └───────────────┬──────────────────────┘
                 │ returns slave index
                 ▼
 ┌──────────────────────────────────────┐
 │         master_fsm.c (again)         │
 │  build_xxx_frame(txBuf)              │
 │   - DISCOVERY                        │
 │   - PRESENCE CHECK                   │
 │   - STATUS REQUEST                   │
 └───────────────┬──────────────────────┘
                 │ calls
                 ▼
 ┌──────────────────────────────────────┐
 │          proto_frame.c               │
 │        ProtoFrame_Build()            │
 │   - fills 7‑byte frame buffer        │
 │   - SOF, MsgID, Addr, Len, Payload   │
 │   - CRC, EOF                         │
 └───────────────┬──────────────────────┘
                 │ returns txBuffer
                 ▼
 ┌──────────────────────────────────────┐
 │        Application Layer             │
 │   HAL_UART_Transmit(txBuffer)        │
 └──────────────────────────────────────┘


This structure is designed for:

High modularity

Clear separation of layers

Easy unit testing

Protocol evolution

Maintainability

RTOS‑friendly task organization

Hardware abstraction

Everything is structured so that protocol logic is independent of HAL, independent of FreeRTOS, and independent of hardware.