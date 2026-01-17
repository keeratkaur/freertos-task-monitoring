/*
 * FreeRTOS Task Monitoring & Stack Usage Analyzer
 * 
 * This application demonstrates real-time task monitoring, stack usage analysis,
 * and stack overflow detection in FreeRTOS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Task priorities */
#define HIGH_PRIORITY_TASK_PRIORITY  (tskIDLE_PRIORITY + 3)  /* Highest priority - stress test */
#define MONITOR_TASK_PRIORITY         (tskIDLE_PRIORITY + 2)
#define WORKER_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/* Stack sizes */
#define MONITOR_STACK_SIZE       2048
#define WORKER_STACK_SIZE        1024
#define SMALL_STACK_SIZE         512  /* Intentionally small to demonstrate overflow detection */

/* Task handles */
static TaskHandle_t xHighPriorityTaskHandle = NULL;
static TaskHandle_t xWorkerTaskHandle1 = NULL;
static TaskHandle_t xWorkerTaskHandle2 = NULL;
static TaskHandle_t xSmallStackTaskHandle = NULL;

/* Stack overflow hook */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("\n!!! STACK OVERFLOW DETECTED !!!\n");
    printf("Task: %s\n", pcTaskName);
    printf("System halted.\n");
    for(;;);
}

/* Worker task 1 - performs periodic work */
void vWorkerTask1(void *pvParameters)
{
    const char *taskName = "WorkerTask1";
    TickType_t xLastWakeTime = xTaskGetTickCount();
    int counter = 0;
    
    /* Allocate some stack space to demonstrate usage */
    char stackBuffer[256];
    memset(stackBuffer, 0, sizeof(stackBuffer));
    
    printf("[%s] Started\n", taskName);
    
    for(;;)
    {
        /* Simulate work */
        for(int i = 0; i < 1000; i++)
        {
            stackBuffer[i % 256] = (char)(counter + i);
        }
        
        counter++;
        if(counter % 10 == 0)
        {
            printf("[%s] Completed %d iterations\n", taskName, counter);
        }
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
    }
}

/* Worker task 2 - different work pattern */
void vWorkerTask2(void *pvParameters)
{
    const char *taskName = "WorkerTask2";
    TickType_t xLastWakeTime = xTaskGetTickCount();
    int counter = 0;
    
    char stackBuffer[128];
    memset(stackBuffer, 0, sizeof(stackBuffer));
    
    printf("[%s] Started\n", taskName);
    
    for(;;)
    {
        /* Different work pattern */
        for(int i = 0; i < 500; i++)
        {
            stackBuffer[i % 128] = (char)(counter * i);
        }
        
        counter++;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(3000));
    }
}

/* High-priority stress test task - demonstrates scheduling pressure */
void vHighPriorityTask(void *pvParameters)
{
    const char *taskName = "HighPriorityTask";
    
    printf("[%s] Started (HIGH PRIORITY - CPU stress test)\n", taskName);
    
    for(;;)
    {
        /* Simulate CPU pressure with busy loop */
        /* This creates scheduling pressure and demonstrates priority behavior */
        for(volatile int i = 0; i < 1000000; i++)
        {
            /* Busy wait - prevents compiler optimization */
        }
        
        /* Minimal delay - keeps task ready most of the time */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/* Small stack task - demonstrates stack monitoring */
void vSmallStackTask(void *pvParameters)
{
    const char *taskName = "SmallStackTask";
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    /* Use significant portion of small stack */
    char stackBuffer[300];  /* Close to 512 byte limit */
    memset(stackBuffer, 0, sizeof(stackBuffer));
    
    printf("[%s] Started (small stack - monitoring critical)\n", taskName);
    
    for(;;)
    {
        /* Minimal work to avoid overflow */
        for(int i = 0; i < 100; i++)
        {
            stackBuffer[i % 300] = (char)i;
        }
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5000));
    }
}

/* Monitoring task - analyzes all tasks */
void vMonitorTask(void *pvParameters)
{
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, xTask;
    uint32_t ulTotalRunTime;
    UBaseType_t uxTaskCount;
    
    /* Allocate array to hold task status */
    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
    
    if(pxTaskStatusArray == NULL)
    {
        printf("ERROR: Failed to allocate memory for task status array\n");
        vTaskDelete(NULL);
        return;
    }
    
    printf("\n=== FreeRTOS Task Monitor Started ===\n");
    printf("Monitoring interval: 5 seconds\n\n");
    
    for(;;)
    {
        /* Get system state */
        uxTaskCount = uxTaskGetSystemState(
            pxTaskStatusArray,
            uxArraySize,
            &ulTotalRunTime
        );
        
        if(uxTaskCount > 0)
        {
            printf("\n--- Task Status Report ---\n");
            printf("%-20s | %-12s | %-15s | %-10s\n", 
                   "Task Name", "State", "Stack HW Mark", "Priority");
            printf("------------------------------------------------------------\n");
            
            for(xTask = 0; xTask < uxTaskCount; xTask++)
            {
                const char *stateStr;
                UBaseType_t stackHWMark = pxTaskStatusArray[xTask].usStackHighWaterMark;
                
                /* Determine task state */
                switch(pxTaskStatusArray[xTask].eCurrentState)
                {
                    case eRunning:
                        stateStr = "Running";
                        break;
                    case eReady:
                        stateStr = "Ready";
                        break;
                    case eBlocked:
                        stateStr = "Blocked";
                        break;
                    case eSuspended:
                        stateStr = "Suspended";
                        break;
                    case eDeleted:
                        stateStr = "Deleted";
                        break;
                    default:
                        stateStr = "Unknown";
                        break;
                }
                
                /* Check for stack overflow risk */
                UBaseType_t stackRemaining = stackHWMark;
                const char *riskLevel = "";
                
                if(stackRemaining < 100)
                {
                    riskLevel = " [CRITICAL]";
                }
                else if(stackRemaining < 200)
                {
                    riskLevel = " [WARNING]";
                }
                
                printf("%-20s | %-12s | %-15u | %-10u%s\n",
                       pxTaskStatusArray[xTask].pcTaskName,
                       stateStr,
                       stackHWMark,
                       pxTaskStatusArray[xTask].uxCurrentPriority,
                       riskLevel);
            }
            
            printf("\nTotal Runtime: %lu ticks\n", (unsigned long)ulTotalRunTime);
            printf("Total Tasks: %u\n", uxTaskCount);
        }
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf("FreeRTOS Task Monitoring & Stack Analyzer\n");
    printf("========================================\n");
    printf("\n");
    
    /* Create high-priority stress test task */
    xTaskCreate(
        vHighPriorityTask,
        "HighPriorityTask",
        WORKER_STACK_SIZE,
        NULL,
        HIGH_PRIORITY_TASK_PRIORITY,
        &xHighPriorityTaskHandle
    );
    
    /* Create worker tasks */
    xTaskCreate(
        vWorkerTask1,
        "WorkerTask1",
        WORKER_STACK_SIZE,
        NULL,
        WORKER_TASK_PRIORITY,
        &xWorkerTaskHandle1
    );
    
    xTaskCreate(
        vWorkerTask2,
        "WorkerTask2",
        WORKER_STACK_SIZE,
        NULL,
        WORKER_TASK_PRIORITY,
        &xWorkerTaskHandle2
    );
    
    /* Create small stack task to demonstrate monitoring */
    xTaskCreate(
        vSmallStackTask,
        "SmallStackTask",
        SMALL_STACK_SIZE,
        NULL,
        WORKER_TASK_PRIORITY,
        &xSmallStackTaskHandle
    );
    
    /* Create monitoring task */
    xTaskCreate(
        vMonitorTask,
        "MonitorTask",
        MONITOR_STACK_SIZE,
        NULL,
        MONITOR_TASK_PRIORITY,
        NULL
    );
    
    /* Start the scheduler */
    printf("Starting FreeRTOS scheduler...\n");
    vTaskStartScheduler();
    
    /* Should never reach here */
    printf("ERROR: Scheduler exited unexpectedly\n");
    return 1;
}

