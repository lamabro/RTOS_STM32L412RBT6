#ifndef MASTER_SCHEDULER_H
#define MASTER_SCHEDULER_H

#include <stdint.h>

/**
 * @file master_scheduler.h
 * @brief Scheduler for alternating slave polling (A/B) every 40 ms.
 */

typedef enum
{
    MASTER_POLL_TARGET_A = 0,
    MASTER_POLL_TARGET_B
} MasterPollTarget_t;

typedef struct
{
    uint32_t tickCount;
    MasterPollTarget_t nextTarget;
} MasterScheduler_t;

extern MasterScheduler_t g_masterScheduler;

void MasterScheduler_Init(void);
MasterPollTarget_t MasterScheduler_GetNextTarget(void);

#endif // MASTER_SCHEDULER_H
