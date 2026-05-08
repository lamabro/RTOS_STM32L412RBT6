#include "master_scheduler.h"

/**
 * @file master_scheduler.c
 * @brief Implementation of 40 ms polling scheduler with 20 ms offset.
 */

MasterScheduler_t g_masterScheduler;

/**
 * @brief Initialize scheduler.
 */
void MasterScheduler_Init(void)
{
    g_masterScheduler.tickCount  = 0;
    g_masterScheduler.nextTarget = MASTER_POLL_TARGET_A;
}

/**
 * @brief Called every 40 ms by TX task.
 * @return which slave to poll next (A or B)
 */
MasterPollTarget_t MasterScheduler_GetNextTarget(void)
{
    MasterPollTarget_t target = g_masterScheduler.nextTarget;

    g_masterScheduler.nextTarget =
        (g_masterScheduler.nextTarget == MASTER_POLL_TARGET_A)
        ? MASTER_POLL_TARGET_B
        : MASTER_POLL_TARGET_A;

    g_masterScheduler.tickCount++;

    return target;
}
