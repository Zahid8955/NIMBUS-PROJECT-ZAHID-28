/**
 * report.c - Productivity reporting module implementation
 * 
 * Implements functions for generating productivity reports and statistics
 * for the Multi-User Task Management System.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/report.h"
#include "../include/task.h"
#include "../include/user.h"
#include "../include/types.h"

/* Calculate overall productivity statistics */
ProductivityStats calculateOverallStats(const TaskList *taskList) {
    ProductivityStats stats = {0};
    
    if (taskList == NULL || taskList->count == 0) {
        return stats;
    }
    
    stats.totalTasks = taskList->count;
    
    for (int i = 0; i < taskList->count; i++) {
        const Task *task = &taskList->tasks[i];
        
        switch (task->status) {
            case STATUS_PENDING:
                stats.pendingTasks++;
                break;
            case STATUS_IN_PROGRESS:
                stats.inProgressTasks++;
                break;
            case STATUS_COMPLETED:
                stats.completedTasks++;
                break;
            case STATUS_OVERDUE:
                stats.overdueTasks++;
                break;
        }
        
        /* Count high priority pending tasks */
        if ((task->priority == PRIORITY_HIGH || task->priority == PRIORITY_URGENT) &&
            task->status != STATUS_COMPLETED) {
            stats.highPriorityPending++;
        }
    }
    
    if (stats.totalTasks > 0) {
        stats.completionRate = (double)stats.completedTasks / stats.totalTasks * 100.0;
    }
    
    return stats;
}

/* Calculate user-specific productivity statistics */
ProductivityStats calculateUserStats(const TaskList *taskList, int userId) {
    ProductivityStats stats = {0};
    
    if (taskList == NULL) {
        return stats;
    }
    
    for (int i = 0; i < taskList->count; i++) {
        const Task *task = &taskList->tasks[i];
        
        if (task->assignedUserId != userId) continue;
        
        stats.totalTasks++;
        
        switch (task->status) {
            case STATUS_PENDING:
                stats.pendingTasks++;
                break;
            case STATUS_IN_PROGRESS:
                stats.inProgressTasks++;
                break;
            case STATUS_COMPLETED:
                stats.completedTasks++;
                break;
            case STATUS_OVERDUE:
                stats.overdueTasks++;
                break;
        }
        
        if ((task->priority == PRIORITY_HIGH || task->priority == PRIORITY_URGENT) &&
            task->status != STATUS_COMPLETED) {
            stats.highPriorityPending++;
        }
    }
    
    if (stats.totalTasks > 0) {
        stats.completionRate = (double)stats.completedTasks / stats.totalTasks * 100.0;
    }
    
    return stats;
}

/* Display overall productivity report */
void displayOverallReport(const TaskList *taskList, const UserList *userList) {
    ProductivityStats stats = calculateOverallStats(taskList);
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║           OVERALL PRODUCTIVITY REPORT                      ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║                                                            ║\n");
    printf("║  Total Tasks:           %-6d                             ║\n", stats.totalTasks);
    printf("║  Completed:             %-6d (%.1f%%)                     ║\n", 
           stats.completedTasks, stats.completionRate);
    printf("║  In Progress:           %-6d                             ║\n", stats.inProgressTasks);
    printf("║  Pending:               %-6d                             ║\n", stats.pendingTasks);
    printf("║  Overdue:               %-6d                             ║\n", stats.overdueTasks);
    printf("║  High Priority Pending: %-6d                             ║\n", stats.highPriorityPending);
    printf("║                                                            ║\n");
    printf("║  Total Users:           %-6d                             ║\n", 
           userList ? userList->count : 0);
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    /* Progress bar */
    printf("\nCompletion Progress: [");
    int barWidth = 40;
    int filled = (int)(stats.completionRate / 100.0 * barWidth);
    for (int i = 0; i < barWidth; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("] %.1f%%\n", stats.completionRate);
}

/* Display user productivity report */
void displayUserReport(const TaskList *taskList, const User *user) {
    if (user == NULL) {
        printf("User not found.\n");
        return;
    }
    
    ProductivityStats stats = calculateUserStats(taskList, user->id);
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  USER PRODUCTIVITY REPORT                                  ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  User: %-50s ║\n", user->fullName);
    printf("║  Username: %-45s ║\n", user->username);
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║                                                            ║\n");
    printf("║  Tasks Assigned:        %-6d                             ║\n", stats.totalTasks);
    printf("║  Completed:             %-6d                             ║\n", stats.completedTasks);
    printf("║  In Progress:           %-6d                             ║\n", stats.inProgressTasks);
    printf("║  Pending:               %-6d                             ║\n", stats.pendingTasks);
    printf("║  Overdue:               %-6d                             ║\n", stats.overdueTasks);
    printf("║  High Priority Pending: %-6d                             ║\n", stats.highPriorityPending);
    printf("║                                                            ║\n");
    printf("║  Completion Rate:       %-6.1f%%                           ║\n", stats.completionRate);
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

/* Display tasks summary by priority */
void displayPrioritySummary(const TaskList *taskList) {
    if (taskList == NULL || taskList->count == 0) {
        printf("No tasks to summarize.\n");
        return;
    }
    
    int urgent = 0, high = 0, medium = 0, low = 0;
    int urgentComplete = 0, highComplete = 0, mediumComplete = 0, lowComplete = 0;
    
    for (int i = 0; i < taskList->count; i++) {
        const Task *task = &taskList->tasks[i];
        int completed = (task->status == STATUS_COMPLETED) ? 1 : 0;
        
        switch (task->priority) {
            case PRIORITY_URGENT:
                urgent++;
                urgentComplete += completed;
                break;
            case PRIORITY_HIGH:
                high++;
                highComplete += completed;
                break;
            case PRIORITY_MEDIUM:
                medium++;
                mediumComplete += completed;
                break;
            case PRIORITY_LOW:
                low++;
                lowComplete += completed;
                break;
        }
    }
    
    printf("\n=== Task Summary by Priority ===\n\n");
    printf("Priority     Total    Completed    Pending\n");
    printf("------------------------------------------------\n");
    printf("Urgent       %-8d %-12d %-8d\n", urgent, urgentComplete, urgent - urgentComplete);
    printf("High         %-8d %-12d %-8d\n", high, highComplete, high - highComplete);
    printf("Medium       %-8d %-12d %-8d\n", medium, mediumComplete, medium - mediumComplete);
    printf("Low          %-8d %-12d %-8d\n", low, lowComplete, low - lowComplete);
    printf("------------------------------------------------\n");
    printf("Total        %-8d %-12d %-8d\n", 
           taskList->count, 
           urgentComplete + highComplete + mediumComplete + lowComplete,
           taskList->count - (urgentComplete + highComplete + mediumComplete + lowComplete));
}

/* Display upcoming deadlines (tasks due within n days) */
void displayUpcomingDeadlines(const TaskList *taskList, const UserList *userList, int days) {
    if (taskList == NULL || taskList->count == 0) {
        printf("No tasks found.\n");
        return;
    }
    
    time_t now = time(NULL);
    time_t futureDate = now + (days * 24 * 60 * 60);
    
    struct tm *tm_now = localtime(&now);
    char currentDate[MAX_DATE_LEN];
    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", tm_now);
    
    struct tm *tm_future = localtime(&futureDate);
    char futureDateStr[MAX_DATE_LEN];
    strftime(futureDateStr, sizeof(futureDateStr), "%Y-%m-%d", tm_future);
    
    printf("\n=== Tasks Due Within %d Days ===\n", days);
    printf("(From %s to %s)\n\n", currentDate, futureDateStr);
    
    int found = 0;
    for (int i = 0; i < taskList->count; i++) {
        const Task *task = &taskList->tasks[i];
        
        if (task->status == STATUS_COMPLETED || task->deadline[0] == '\0') {
            continue;
        }
        
        if (strcmp(task->deadline, currentDate) >= 0 && 
            strcmp(task->deadline, futureDateStr) <= 0) {
            displayTask(task, userList);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No tasks due within the next %d days.\n", days);
    } else {
        printf("\nFound %d task(s) due within %d days\n", found, days);
    }
}

/* Display overdue tasks report */
void displayOverdueReport(const TaskList *taskList, const UserList *userList) {
    if (taskList == NULL || taskList->count == 0) {
        printf("No tasks found.\n");
        return;
    }
    
    /* First, check and update overdue status */
    checkOverdueTasks((TaskList *)taskList);
    
    printf("\n=== OVERDUE TASKS REPORT ===\n\n");
    
    int found = 0;
    for (int i = 0; i < taskList->count; i++) {
        const Task *task = &taskList->tasks[i];
        
        if (task->status == STATUS_OVERDUE) {
            displayTask(task, userList);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No overdue tasks! Great job keeping up!\n");
    } else {
        printf("\n⚠️  Found %d overdue task(s) that need attention!\n", found);
    }
}

/* Display tag distribution */
void displayTagDistribution(const TaskList *taskList) {
    if (taskList == NULL || taskList->count == 0) {
        printf("No tasks found.\n");
        return;
    }
    
    /* Simple approach: collect unique tags and count them */
    typedef struct {
        char tag[MAX_TAG_LEN];
        int count;
    } TagCount;
    
    TagCount *tagCounts = NULL;
    int tagCountSize = 0;
    int tagCountCapacity = 0;
    
    for (int i = 0; i < taskList->count; i++) {
        const Task *task = &taskList->tasks[i];
        
        for (int j = 0; j < task->tagCount; j++) {
            /* Check if tag already exists */
            int found = 0;
            for (int k = 0; k < tagCountSize; k++) {
                if (strcmp(tagCounts[k].tag, task->tags[j]) == 0) {
                    tagCounts[k].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                /* Add new tag */
                if (tagCountSize >= tagCountCapacity) {
                    tagCountCapacity = tagCountCapacity == 0 ? INITIAL_CAPACITY : tagCountCapacity * 2;
                    TagCount *newTagCounts = realloc(tagCounts, tagCountCapacity * sizeof(TagCount));
                    if (newTagCounts == NULL) {
                        free(tagCounts);
                        printf("Memory allocation error\n");
                        return;
                    }
                    tagCounts = newTagCounts;
                }
                strncpy(tagCounts[tagCountSize].tag, task->tags[j], MAX_TAG_LEN - 1);
                tagCounts[tagCountSize].tag[MAX_TAG_LEN - 1] = '\0';
                tagCounts[tagCountSize].count = 1;
                tagCountSize++;
            }
        }
    }
    
    printf("\n=== Tag Distribution ===\n\n");
    
    if (tagCountSize == 0) {
        printf("No tags found in any tasks.\n");
    } else {
        printf("%-20s Count\n", "Tag");
        printf("------------------------------\n");
        
        /* Sort by count (simple bubble sort) */
        for (int i = 0; i < tagCountSize - 1; i++) {
            for (int j = 0; j < tagCountSize - i - 1; j++) {
                if (tagCounts[j].count < tagCounts[j + 1].count) {
                    TagCount temp = tagCounts[j];
                    tagCounts[j] = tagCounts[j + 1];
                    tagCounts[j + 1] = temp;
                }
            }
        }
        
        for (int i = 0; i < tagCountSize; i++) {
            printf("%-20s %d\n", tagCounts[i].tag, tagCounts[i].count);
        }
        printf("------------------------------\n");
        printf("Total unique tags: %d\n", tagCountSize);
    }
    
    free(tagCounts);
}

/* Display team workload distribution */
void displayWorkloadDistribution(const TaskList *taskList, const UserList *userList) {
    if (taskList == NULL || userList == NULL || userList->count == 0) {
        printf("No data available for workload distribution.\n");
        return;
    }
    
    printf("\n=== Team Workload Distribution ===\n\n");
    printf("%-20s %-10s %-10s %-10s %-10s\n", 
           "User", "Total", "Completed", "Pending", "Overdue");
    printf("----------------------------------------------------------------------\n");
    
    for (int i = 0; i < userList->count; i++) {
        const User *user = &userList->users[i];
        ProductivityStats stats = calculateUserStats(taskList, user->id);
        
        printf("%-20s %-10d %-10d %-10d %-10d\n",
               user->username,
               stats.totalTasks,
               stats.completedTasks,
               stats.pendingTasks + stats.inProgressTasks,
               stats.overdueTasks);
    }
    
    /* Count unassigned tasks */
    int unassigned = 0;
    for (int i = 0; i < taskList->count; i++) {
        if (taskList->tasks[i].assignedUserId <= 0) {
            unassigned++;
        }
    }
    
    printf("----------------------------------------------------------------------\n");
    printf("Unassigned Tasks: %d\n", unassigned);
}
