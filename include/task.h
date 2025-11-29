/**
 * task.h - Task management module header
 * 
 * Provides functions for creating, editing, organizing, and managing tasks
 * including sorting, searching, and filtering capabilities.
 */

#ifndef TASK_H
#define TASK_H

#include "types.h"

/* Initialize task list with dynamic memory allocation */
void initTaskList(TaskList *list);

/* Free task list memory */
void freeTaskList(TaskList *list);

/* Add a new task */
int addTask(TaskList *list, const char *title, const char *description, 
            Priority priority, const char *deadline, int createdByUserId);

/* Find task by ID */
Task* findTaskById(TaskList *list, int id);

/* Get task index by ID */
int getTaskIndexById(TaskList *list, int id);

/* Remove task by ID */
int removeTask(TaskList *list, int id);

/* Update task title */
int updateTaskTitle(Task *task, const char *newTitle);

/* Update task description */
int updateTaskDescription(Task *task, const char *newDescription);

/* Update task priority */
int updateTaskPriority(Task *task, Priority newPriority);

/* Update task deadline */
int updateTaskDeadline(Task *task, const char *newDeadline);

/* Update task status */
int updateTaskStatus(Task *task, TaskStatus newStatus);

/* Assign task to user */
int assignTaskToUser(Task *task, int userId);

/* Unassign task */
int unassignTask(Task *task);

/* Add tag to task */
int addTagToTask(Task *task, const char *tag);

/* Remove tag from task */
int removeTagFromTask(Task *task, const char *tag);

/* Mark task as complete */
int completeTask(Task *task);

/* Display single task details */
void displayTask(const Task *task, const UserList *userList);

/* Display all tasks */
void displayAllTasks(const TaskList *list, const UserList *userList);

/* Display tasks by status */
void displayTasksByStatus(const TaskList *list, const UserList *userList, TaskStatus status);

/* Display tasks by priority */
void displayTasksByPriority(const TaskList *list, const UserList *userList, Priority priority);

/* Display tasks assigned to user */
void displayTasksByUser(const TaskList *list, const UserList *userList, int userId);

/* Sort tasks by priority (highest first) */
void sortTasksByPriority(TaskList *list);

/* Sort tasks by deadline (earliest first) */
void sortTasksByDeadline(TaskList *list);

/* Sort tasks by status */
void sortTasksByStatus(TaskList *list);

/* Search tasks by title (partial match) */
void searchTasksByTitle(const TaskList *list, const UserList *userList, const char *searchTerm);

/* Search tasks by tag */
void searchTasksByTag(const TaskList *list, const UserList *userList, const char *tag);

/* Check and update overdue tasks */
void checkOverdueTasks(TaskList *list);

/* Compare deadlines (for sorting) */
int compareDeadlines(const char *deadline1, const char *deadline2);

/* Validate date format */
int isValidDateFormat(const char *date);

#endif /* TASK_H */
