#include "proto_frame.h"

// -------------------------------------------------------------
// Compute CRC (simple XOR of fields)
// -------------------------------------------------------------


/**
 * @brief Initialize FSM and database.

 We will use this later

 uint8_t ProtoFrame_ComputeCRC(MessageID_t msgID,
                              uint8_t addr,
                              uint8_t length,
                              uint8_t payload)
{
    uint8_t crc = 0;
    crc ^= msgID;
    crc ^= addr;
    crc ^= length;
    crc ^= payload;
    return crc;
}
 
 */

// -------------------------------------------------------------
// Frame context (Needed for building frames, if needed)

// -------------------------------------------------------------
// Build a 7-byte frame
// -------------------------------------------------------------
void ProtoFrame_Build(uint8_t *buffer,
                      MessageID_t msgID,
                      uint8_t addr,
                      uint8_t length,
                      uint8_t payload)
{
    buffer[0] = FRAME_SOF;
    buffer[1] = (uint8_t)msgID;
    buffer[2] = addr;
    buffer[3] = length;
    buffer[4] = payload; 
    buffer[5] = CRC_dummy;


   // buffer[5] = ProtoFrame_ComputeCRC(msgID, addr, length, payload);

    buffer[6] = FRAME_EOF;
}
