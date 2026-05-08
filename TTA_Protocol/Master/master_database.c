#include "master_database.h"

/**
 * @file master_database.c
 * @brief Implementation of the master-side slave database.
 */

MasterDatabase_t g_masterDb;

/**
 * @brief Initialize database and load from flash.
 */
void MasterDb_Init(void)
{
    memset(&g_masterDb, 0, sizeof(g_masterDb));
    MasterDb_LoadFromFlash();
}

/**
 * @brief Compare two UniqueIDs.
 */
static int uniqueID_equal(const uint8_t *a, const uint8_t *b)
{
    return (memcmp(a, b, MAX_UNIQUE_ID_LEN) == 0);
}

/**
 * @brief Find slave address by UniqueID.
 * @return address or -1 if not found
 */
int8_t MasterDb_FindByUniqueID(const uint8_t *uniqueID)
{
    for (uint8_t i = 0; i < MAX_SLAVES; i++)
    {
        if (g_masterDb.records[i].valid &&
            uniqueID_equal(g_masterDb.records[i].uniqueID, uniqueID))
        {
            return g_masterDb.records[i].addr;
        }
    }
    return -1;
}

/**
 * @brief Add a new slave to the database.
 * @return 0 on success, -1 if full
 */
int8_t MasterDb_Add(const uint8_t *uniqueID, uint8_t addr)
{
    for (uint8_t i = 0; i < MAX_SLAVES; i++)
    {
        if (!g_masterDb.records[i].valid)
        {
            memcpy(g_masterDb.records[i].uniqueID, uniqueID, MAX_UNIQUE_ID_LEN);
            g_masterDb.records[i].addr  = addr;
            g_masterDb.records[i].valid = 1;

            if (g_masterDb.count < MAX_SLAVES)
                g_masterDb.count++;

            MasterDb_SaveToFlash();
            return 0;
        }
    }
    return -1;
}

/**
 * @brief Get next free address (1..MAX_SLAVES).
 */
uint8_t MasterDb_GetNextFreeAddress(void)
{
    for (uint8_t addr = 1; addr <= MAX_SLAVES; addr++)
    {
        uint8_t used = 0;
        for (uint8_t i = 0; i < MAX_SLAVES; i++)
        {
            if (g_masterDb.records[i].valid &&
                g_masterDb.records[i].addr == addr)
            {
                used = 1;
                break;
            }
        }
        if (!used)
            return addr;
    }
    return 0; // no free address
}

/**
 * @brief Load database from flash (stub).
 */
void MasterDb_LoadFromFlash(void)
{
    // TODO: integrate with flash driver
}

/**
 * @brief Save database to flash (stub).
 */
void MasterDb_SaveToFlash(void)
{
    // TODO: integrate with flash driver
}
