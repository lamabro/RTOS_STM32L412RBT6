#ifndef MASTER_DATABASE_H
#define MASTER_DATABASE_H

#include <stdint.h>
#include <string.h>
#include "proto_defs.h"

/**
 * @file master_database.h
 * @brief Database for storing discovered slave devices.
 *
 * This module maintains a table of slaves discovered during the
 * discovery phase. Each slave has:
 *   - UniqueID (factory/programmed)
 *   - Assigned bus address (1..MAX_SLAVES)
 *   - Valid flag
 *
 * The database can be saved/loaded from flash for persistence.
 */

#define MAX_UNIQUE_ID_LEN   8   /**< Length of slave UniqueID in bytes */

/**
 * @brief One slave record in the database.
 */
typedef struct
{
    uint8_t  uniqueID[MAX_UNIQUE_ID_LEN]; /**< Unique ID of slave */
    uint8_t  addr;                        /**< Assigned bus address */
    uint8_t  valid;                       /**< 1 = valid entry */
} SlaveRecord_t;

/**
 * @brief Full master database containing up to MAX_SLAVES entries.
 */
typedef struct
{
    SlaveRecord_t records[MAX_SLAVES];
    uint8_t       count;
} MasterDatabase_t;

/** Global database instance */
extern MasterDatabase_t g_masterDb;

/* API */
void MasterDb_Init(void);
int8_t MasterDb_FindByUniqueID(const uint8_t *uniqueID);
int8_t MasterDb_Add(const uint8_t *uniqueID, uint8_t addr);
uint8_t MasterDb_GetNextFreeAddress(void);

/* Flash persistence (stubbed for now) */
void MasterDb_LoadFromFlash(void);
void MasterDb_SaveToFlash(void);

#endif // MASTER_DATABASE_H
