#ifndef PROTO_DEFS_H
#define PROTO_DEFS_H

#include <stdint.h>

//
// -------------------------------------------------------------
//  MESSAGE IDs (1 BYTE)
// -------------------------------------------------------------
//  These IDs define the type of communication frame.
//  All IDs are 1 byte as required by the protocol spec.
//
typedef enum
{
    MSG_ID_DISCOVER      = 0x01,   // Master → broadcast: request slaves to announce themselves
    MSG_ID_ACK           = 0x02,   // Slave → master: acknowledge presence or command
    MSG_ID_STATUS_REQ    = 0x03,   // Master → slave: request GPIO status
    MSG_ID_COMMAND       = 0x04    // Master → slave: command to set GPIO or other action
} MessageID_t;


//
// -------------------------------------------------------------
//  MASTER & SLAVE ADDRESSING
// -------------------------------------------------------------
//
#define MASTER_ADDRESS        0x00     // Master always address 0
#define MAX_SLAVES            3        // Maximum number of slaves supported

// Slave address table (populated at runtime)
typedef struct
{
    uint8_t slaveCount;                // Number of discovered slaves
    uint8_t slaveAddress[MAX_SLAVES];  // List of slave addresses
} SlaveAddressTable_t;


//
// -------------------------------------------------------------
//  COMMUNICATION ERROR BYTE (BYTE 1)
// -------------------------------------------------------------
//
//  Bit 0: No slave found
//  Bit 1: Communication timeout after 1 minute
//
typedef union
{
    uint8_t all;
    struct
    {
        uint8_t noSlaveFound      : 1;   // Bit 0
        uint8_t timeout1min       : 1;   // Bit 1
        uint8_t reserved          : 6;   // Remaining bits
    } bits;
} CommErrorByte_t;


//
// -------------------------------------------------------------
//  COMMUNICATION STATUS BYTE (BYTE 2)
// -------------------------------------------------------------
//
//  Bit 0: Communication running
//         Communication failed for 1 minute (removed)
//
typedef union
{
    uint8_t all;
    struct
    {
        uint8_t commRunning       : 1;   // Bit 0
        uint8_t reserved          : 7;
    } bits;
} CommStatusByte_t;


//
// -------------------------------------------------------------
//  PROTOCOL STATE STRUCT
// -------------------------------------------------------------
//  This struct stores all protocol‑level runtime information.
//  Highly modular, unit‑testable, and easy to maintain.
//
typedef struct
{
    SlaveAddressTable_t addressTable;   // Discovered slaves
    CommErrorByte_t     errorByte;      // Error flags
    CommStatusByte_t    statusByte;     // Status flags
} ProtocolState_t;

#endif // PROTO_DEFS_H
