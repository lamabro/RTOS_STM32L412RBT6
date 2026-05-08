#ifndef PROTO_FRAME_H
#define PROTO_FRAME_H

#include <stdint.h>
#include "proto_defs.h"   // For MessageID_t, addresses, etc.

// -------------------------------------------------------------
// Frame constants
// -------------------------------------------------------------
#define FRAME_SOF       0xAA
#define FRAME_EOF       0x55
#define FRAME_SIZE      7       // Fixed-length frame

// -------------------------------------------------------------
// Frame structure (logical representation)
// -------------------------------------------------------------
typedef struct
{
    uint8_t sof;        // Start of Frame
    uint8_t msgID;      // Message ID (1 byte)
    uint8_t addr;       // Address (1 byte)
    uint8_t length;     // Payload length (always 1 for now)
    uint8_t payload;    // Payload byte
    uint8_t crc;        // CRC or checksum
    uint8_t eof;        // End of Frame
} ProtoFrame_t;

// -------------------------------------------------------------
// API
// -------------------------------------------------------------

// Build a 7-byte frame into a buffer
void ProtoFrame_Build(uint8_t *buffer,
                      MessageID_t msgID,
                      uint8_t addr,
                      uint8_t length,
                      uint8_t payload);

// Compute CRC (simple XOR or CRC8)
uint8_t ProtoFrame_ComputeCRC(MessageID_t msgID,
                              uint8_t addr,
                              uint8_t length,
                              uint8_t payload);

#endif // PROTO_FRAME_H
