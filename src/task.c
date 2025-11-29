/**
 * task.c - Task management module implementation
 * 
 * Implements functions for creating, editing, organizing, and managing tasks
 * including sorting, searching, and filtering capabilities.
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <ctype.h>
#include "../include/task.h"
#include "../include/user.h"
#include "../include/types.h"

/* Static counter for task IDs */
static int nextTaskId = 1;

/* Initialize task list with dynamic memory allocation */
void initTaskList(TaskList *list) {
    if (list == NULL) return;
    
    list->tasks = (Task *)malloc(INITIAL_CAPACITY * sizeof(Task));
    if (list->tasks == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for task list\n");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

/* Free task list memory */
void freeTaskList(TaskList *list) {
    if (list == NULL) return;
    
    free(list->tasks);
    list->tasks = NULL;
    list->count = 0;
    list->capacity = 0;
}

/* Helper function to resize task list if needed */
static int resizeTaskList(TaskList *list) {
    if (list == NULL) return 0;
    
    if (list->count >= list->capacity) {
        int newCapacity = list->capacity * 2;
        Task *newTasks = (Task *)realloc(list->tasks, newCapacity * sizeof(Task));
        if (newTasks == NULL) {
            fprintf(stderr, "Error: Memory reallocation failed for task list\n");
            return 0;
        }
        list->tasks = newTasks;
        list->capacity = newCapacity;
    }
    return 1;
}

/* Add a new task */
int addTask(TaskList *list, const char *title, const char *description, 
            Priority priority, const char *deadline, int createdByUserId) {
    if (list == NULL || title == NULL) {
        return -1;
    }
    
    /* Resize if needed */
    if (!resizeTaskList(list)) {
        return -1;
    }
    
    /* Create new task */
    Task *newTask = &list->tasks[list->count];
    newTask->id = nextTaskId++;
    
    strncpy(newTask->title, title, MAX_TITLE_LEN - 1);
    newTask->title[MAX_TITLE_LEN - 1] = '\0';
    
    if (description != NULL) {
        strncpy(newTask->description, description, MAX_DESC_LEN - 1);
        newTask->description[MAX_DESC_LEN - 1] = '\0';
    } else {
        newTask->description[0] = '\0';
    }
    
    newTask->priority = priority;
    newTask->status = STATUS_PENDING;
    newTask->assignedUserId = -1;
    newTask->createdByUserId = createdByUserId;
    
    if (deadline != NULL && isValidDateFormat(deadline)) {
        strncpy(newTask->deadline, deadline, MAX_DATE_LEN - 1);
        newTask->deadline[MAX_DATE_LEN - 1] = '\0';
    } else {
        newTask->deadline[0] = '\0';
    }
    
    newTask->tagCount = 0;
    newTask->createdAt = time(NULL);
    newTask->completedAt = 0;
    
    list->count++;
    
    return newTask->id;
}

/* Find task by ID */
Task* findTaskById(TaskList *list, int id) {
    if (list == NULL) return NULL;
    
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].id == id) {
            return &list->tasks[i];
        }
    }
    return NULL;
}

/* Get task index by ID */
int getTaskIndexById(TaskList *list, int id) {
    if (list == NULL) return -1;
    
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Remove task by ID */
int removeTask(TaskList *list, int id) {
    if (list == NULL) return 0;
    
    int index = getTaskIndexById(list, id);
    if (index == -1) {
        return 0;
    }
    
    /* Shift remaining tasks */
    for (int i = index; i < list->count - 1; i++) {
        list->tasks[i] = list->tasks[i + 1];
    }
    list->count--;
    
    return 1;
}

/* Update task title */
int updateTaskTitle(Task *task, const char *newTitle) {
    if (task == NULL || newTitle == NULL) return 0;
    
    strncpy(task->title, newTitle, MAX_TITLE_LEN - 1);
    task->title[MAX_TITLE_LEN - 1] = '\0';
    return 1;
}

/* Update task description */
int updateTaskDescription(Task *task, const char *newDescription) {
    if (task == NULL || newDescription == NULL) return 0;
    
    strncpy(task->description, newDescription, MAX_DESC_LEN - 1);
    task->description[MAX_DESC_LEN - 1] = '\0';
    return 1;
}

/* Update task priority */
int updateTaskPriority(Task *task, Priority newPriority) {
    if (task == NULL) return 0;
    
    task->priority = newPriority;
    return 1;
}

/* Update task deadline */
int updateTaskDeadline(Task *task, const char *newDeadline) {
    if (task == NULL || newDeadline == NULL) return 0;
    
    if (!isValidDateFormat(newDeadline)) {
        printf("Error: Invalid date format. Use YYYY-MM-DD\n");
        return 0;
    }
    
    strncpy(task->deadline, newDeadline, MAX_DATE_LEN - 1);
    task->deadline[MAX_DATE_LEN - 1] = '\0';
    return 1;
}

/* Update task status */
int updateTaskStatus(Task *task, TaskStatus newStatus) {
    if (task == NULL) return 0;
    
    task->status = newStatus;
    if (newStatus == STATUS_COMPLETED) {
        task->completedAt = time(NULL);
    }
    return 1;
}

/* Assign task to user */
int assignTaskToUser(Task *task, int userId) {
    if (task == NULL) return 0;
    
    task->assignedUserId = userId;
    return 1;
}

/* Unassign task */
int unassignTask(Task *task) {
    if (task == NULL) return 0;
    
    task->assignedUserId = -1;
    return 1;
}

/* Add tag to task */
int addTagToTask(Task *task, const char *tag) {
    if (task == NULL || tag == NULL) return 0;
    
    if (task->tagCount >= MAX_TAGS_PER_TASK) {
        printf("Error: Maximum tags reached for this task\n");
        return 0;
    }
    
    /* Check if tag already exists */
    for (int i = 0; i < task->tagCount; i++) {
        if (strcmp(task->tags[i], tag) == 0) {
            printf("Tag '%s' already exists on this task\n", tag);
            return 0;
        }
    }
    
    strncpy(task->tags[task->tagCount], tag, MAX_TAG_LEN - 1);
    task->tags[task->tagCount][MAX_TAG_LEN - 1] = '\0';
    task->tagCount++;
    
    return 1;
}

/* Remove tag from task */
int removeTagFromTask(Task *task, const char *tag) {
    if (task == NULL || tag == NULL) return 0;
    
    for (int i = 0; i < task->tagCount; i++) {
        if (strcmp(task->tags[i], tag) == 0) {
            /* Shift remaining tags */
            for (int j = i; j < task->tagCount - 1; j++) {
                strcpy(task->tags[j], task->tags[j + 1]);
            }
            task->tagCount--;
            return 1;
        }
    }
    return 0;
}

/* Mark task as complete */
int completeTask(Task *task) {
    if (task == NULL) return 0;
    
    task->status = STATUS_COMPLETED;
    task->completedAt = time(NULL);
    return 1;
}

/* Display single task details */
void displayTask(const Task *task, const UserList *userList) {
    if (task == NULL) {
        printf("Task not found.\n");
        return;
    }
    
    printf("\n=== Task Details ===\n");
    printf("ID:          %d\n", task->id);
    printf("Title:       %s\n", task->title);
    printf("Description: %s\n", task->description[0] ? task->description : "(none)");
    printf("Priority:    %s\n", getPriorityString(task->priority));
    printf("Status:      %s\n", getStatusString(task->status));
    printf("Deadline:    %s\n", task->deadline[0] ? task->deadline : "(none)");
    
    if (task->assignedUserId > 0 && userList != NULL) {
        User *user = findUserById((UserList *)userList, task->assignedUserId);
        if (user != NULL) {
            printf("Assigned To: %s (%s)\n", user->fullName, user->username);
        } else {
            printf("Assigned To: Unknown (ID: %d)\n", task->assignedUserId);
        }
    } else {
        printf("Assigned To: Unassigned\n");
    }
    
    if (task->tagCount > 0) {
        printf("Tags:        ");
        for (int i = 0; i < task->tagCount; i++) {
            printf("[%s]", task->tags[i]);
            if (i < task->tagCount - 1) printf(" ");
        }
        printf("\n");
    } else {
        printf("Tags:        (none)\n");
    }
    
    char timeBuffer[64];
    struct tm *tm_info = localtime(&task->createdAt);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Created:     %s\n", timeBuffer);
    
    if (task->completedAt > 0) {
        tm_info = localtime(&task->completedAt);
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Completed:   %s\n", timeBuffer);
    }
    
    printf("====================\n");
}

/* Display all tasks */
void displayAllTasks(const TaskList *list, const UserList *userList) {
    if (list == NULL || list->count == 0) {
        printf("No tasks found.\n");
        return;
    }
    
    printf("\n%-4s %-30s %-10s %-12s %-12s %-15s\n", 
           "ID", "Title", "Priority", "Status", "Deadline", "Assigned To");
    printf("--------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < list->count; i++) {
        Task *task = &list->tasks[i];
        char assignedName[MAX_NAME_LEN] = "Unassigned";
        
        if (task->assignedUserId > 0 && userList != NULL) {
            User *user = findUserById((UserList *)userList, task->assignedUserId);
            if (user != NULL) {
                strncpy(assignedName, user->username, MAX_NAME_LEN - 1);
            }
        }
        
        char truncTitle[31];
        strncpy(truncTitle, task->title, 30);
        truncTitle[30] = '\0';
        
        printf("%-4d %-30s %-10s %-12s %-12s %-15s\n",
               task->id,
               truncTitle,
               getPriorityString(task->priority),
               getStatusString(task->status),
               task->deadline[0] ? task->deadline : "No deadline",
               assignedName);
    }
    printf("--------------------------------------------------------------------------------------------\n");
    printf("Total Tasks: %d\n", list->count);
}

/* Display tasks by status */
void displayTasksByStatus(const TaskList *list, const UserList *userList, TaskStatus status) {
    if (list == NULL || list->count == 0) {
        printf("No tasks found.\n");
        return;
    }
    
    printf("\n=== Tasks with status: %s ===\n", getStatusString(status));
    int found = 0;
    
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].status == status) {
            displayTask(&list->tasks[i], userList);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No tasks found with status '%s'\n", getStatusString(status));
    } else {
        printf("Found %d task(s)\n", found);
    }
}

/* Display tasks by priority */
void displayTasksByPriority(const TaskList *list, const UserList *userList, Priority priority) {
    if (list == NULL || list->count == 0) {
        printf("No tasks found.\n");
        return;
    }
    
    printf("\n=== Tasks with priority: %s ===\n", getPriorityString(priority));
    int found = 0;
    
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].priority == priority) {
            displayTask(&list->tasks[i], userList);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No tasks found with priority '%s'\n", getPriorityString(priority));
    } else {
        printf("Found %d task(s)\n", found);
    }
}

/* Display tasks assigned to user */
void displayTasksByUser(const TaskList *list, const UserList *userList, int userId) {
    if (list == NULL || list->count == 0) {
        printf("No tasks found.\n");
        return;
    }
    
    User *user = findUserById((UserList *)userList, userId);
    if (user != NULL) {
        printf("\n=== Tasks assigned to: %s ===\n", user->fullName);
    } else {
        printf("\n=== Tasks assigned to User ID: %d ===\n", userId);
    }
    
    int found = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].assignedUserId == userId) {
            displayTask(&list->tasks[i], userList);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No tasks assigned to this user\n");
    } else {
        printf("Found %d task(s)\n", found);
    }
}

/* Comparison function for sorting by priority */
static int comparePriority(const void *a, const void *b) {
    const Task *taskA = (const Task *)a;
    const Task *taskB = (const Task *)b;
    return (int)taskB->priority - (int)taskA->priority;  /* Descending order */
}

/* Sort tasks by priority (highest first) */
void sortTasksByPriority(TaskList *list) {
    if (list == NULL || list->count <= 1) return;
    
    qsort(list->tasks, list->count, sizeof(Task), comparePriority);
    printf("Tasks sorted by priority (highest first)\n");
}

/* Compare deadlines (for sorting) */
int compareDeadlines(const char *deadline1, const char *deadline2) {
    /* Handle empty deadlines - put them at the end */
    if (deadline1[0] == '\0' && deadline2[0] == '\0') return 0;
    if (deadline1[0] == '\0') return 1;
    if (deadline2[0] == '\0') return -1;
    
    return strcmp(deadline1, deadline2);
}

/* Comparison function for sorting by deadline */
static int compareDeadlineSort(const void *a, const void *b) {
    const Task *taskA = (const Task *)a;
    const Task *taskB = (const Task *)b;
    return compareDeadlines(taskA->deadline, taskB->deadline);
}

/* Sort tasks by deadline (earliest first) */
void sortTasksByDeadline(TaskList *list) {
    if (list == NULL || list->count <= 1) return;
    
    qsort(list->tasks, list->count, sizeof(Task), compareDeadlineSort);
    printf("Tasks sorted by deadline (earliest first)\n");
}

/* Comparison function for sorting by status */
static int compareStatus(const void *a, const void *b) {
    const Task *taskA = (const Task *)a;
    const Task *taskB = (const Task *)b;
    return (int)taskA->status - (int)taskB->status;
}

/* Sort tasks by status */
void sortTasksByStatus(TaskList *list) {
    if (list == NULL || list->count <= 1) return;
    
    qsort(list->tasks, list->count, sizeof(Task), compareStatus);
    printf("Tasks sorted by status\n");
}

/* Helper function for case-insensitive substring search */
static char* strcasestr_custom(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return NULL;
    if (*needle == '\0') return (char *)haystack;
    
    size_t needle_len = strlen(needle);
    for (; *haystack != '\0'; haystack++) {
        if (strncasecmp(haystack, needle, needle_len) == 0) {
            return (char *)haystack;
        }
    }
    return NULL;
}

/* Search tasks by title (partial match) */
void searchTasksByTitle(const TaskList *list, const UserList *userList, const char *searchTerm) {
    if (list == NULL || searchTerm == NULL) {
        printf("Invalid search parameters\n");
        return;
    }
    
    printf("\n=== Search Results for: '%s' ===\n", searchTerm);
    int found = 0;
    
    for (int i = 0; i < list->count; i++) {
        if (strcasestr_custom(list->tasks[i].title, searchTerm) != NULL ||
            strcasestr_custom(list->tasks[i].description, searchTerm) != NULL) {
            displayTask(&list->tasks[i], userList);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No tasks found matching '%s'\n", searchTerm);
    } else {
        printf("Found %d task(s) matching '%s'\n", found, searchTerm);
    }
}

/* Search tasks by tag */
void searchTasksByTag(const TaskList *list, const UserList *userList, const char *tag) {
    if (list == NULL || tag == NULL) {
        printf("Invalid search parameters\n");
        return;
    }
    
    printf("\n=== Tasks with tag: '%s' ===\n", tag);
    int found = 0;
    
    for (int i = 0; i < list->count; i++) {
        for (int j = 0; j < list->tasks[i].tagCount; j++) {
            if (strcasecmp(list->tasks[i].tags[j], tag) == 0) {
                displayTask(&list->tasks[i], userList);
                found++;
                break;
            }
        }
    }
    
    if (found == 0) {
        printf("No tasks found with tag '%s'\n", tag);
    } else {
        printf("Found %d task(s) with tag '%s'\n", found, tag);
    }
}

/* Check and update overdue tasks */
void checkOverdueTasks(TaskList *list) {
    if (list == NULL) return;
    
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    char currentDate[MAX_DATE_LEN];
    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", tm_now);
    
    for (int i = 0; i < list->count; i++) {
        Task *task = &list->tasks[i];
        if (task->status != STATUS_COMPLETED && 
            task->deadline[0] != '\0' &&
            strcmp(task->deadline, currentDate) < 0) {
            task->status = STATUS_OVERDUE;
        }
    }
}

/* Validate date format (YYYY-MM-DD) */
int isValidDateFormat(const char *date) {
    if (date == NULL || strlen(date) != 10) return 0;
    
    /* Check format YYYY-MM-DD */
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return 0;
        } else {
            if (!isdigit((unsigned char)date[i])) return 0;
        }
    }
    
    /* Basic validation of values */
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return 0;
    
    if (year < 2020 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    return 1;
}
