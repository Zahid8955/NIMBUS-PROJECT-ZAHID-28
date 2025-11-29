/**
 * types.h - Data types and constants for Multi-User Task Management System
 * 
 * This header defines all data structures and constants used throughout
 * the task management system including User, Task, and TagList structures.
 */

#ifndef TYPES_H
#define TYPES_H

#include <time.h>

/* ==================== Constants ==================== */
#define MAX_NAME_LEN 50
#define MAX_TITLE_LEN 100
#define MAX_DESC_LEN 500
#define MAX_TAG_LEN 30
#define MAX_TAGS_PER_TASK 5
#define MAX_DATE_LEN 20
#define INITIAL_CAPACITY 10

/* ==================== Enumerations ==================== */

/* Priority levels for tasks */
typedef enum {
    PRIORITY_LOW = 1,
    PRIORITY_MEDIUM = 2,
    PRIORITY_HIGH = 3,
    PRIORITY_URGENT = 4
} Priority;

/* Task status */
typedef enum {
    STATUS_PENDING = 0,
    STATUS_IN_PROGRESS = 1,
    STATUS_COMPLETED = 2,
    STATUS_OVERDUE = 3
} TaskStatus;

/* ==================== Structures ==================== */

/* User structure */
typedef struct {
    int id;
    char username[MAX_NAME_LEN];
    char fullName[MAX_NAME_LEN];
    int tasksAssigned;
    int tasksCompleted;
} User;

/* Task structure */
typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char description[MAX_DESC_LEN];
    Priority priority;
    TaskStatus status;
    int assignedUserId;           /* -1 if unassigned */
    int createdByUserId;
    char deadline[MAX_DATE_LEN];  /* Format: YYYY-MM-DD */
    char tags[MAX_TAGS_PER_TASK][MAX_TAG_LEN];
    int tagCount;
    time_t createdAt;
    time_t completedAt;
} Task;

/* Dynamic array for users */
typedef struct {
    User *users;
    int count;
    int capacity;
} UserList;

/* Dynamic array for tasks */
typedef struct {
    Task *tasks;
    int count;
    int capacity;
} TaskList;

/* Productivity statistics */
typedef struct {
    int totalTasks;
    int completedTasks;
    int pendingTasks;
    int inProgressTasks;
    int overdueTasks;
    int highPriorityPending;
    double completionRate;
} ProductivityStats;

#endif /* TYPES_H */
