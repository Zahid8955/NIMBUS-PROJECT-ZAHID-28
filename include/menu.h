/**
 * menu.h - Menu-driven interface header
 * 
 * Provides functions for the console-based menu interface
 * of the Multi-User Task Management System.
 */

#ifndef MENU_H
#define MENU_H

#include "types.h"

/* Display main menu and get user choice */
int displayMainMenu(void);

/* User management submenu */
void userManagementMenu(UserList *userList);

/* Task management submenu */
void taskManagementMenu(TaskList *taskList, UserList *userList);

/* Task operations submenu */
void taskOperationsMenu(TaskList *taskList, UserList *userList);

/* Search and filter submenu */
void searchFilterMenu(TaskList *taskList, UserList *userList);

/* Reports submenu */
void reportsMenu(TaskList *taskList, UserList *userList);

/* Utility: Clear input buffer */
void clearInputBuffer(void);

/* Utility: Get integer input with validation */
int getIntInput(const char *prompt, int min, int max);

/* Utility: Get string input */
void getStringInput(const char *prompt, char *buffer, int maxLen);

/* Utility: Get priority from user */
Priority getPriorityInput(void);

/* Utility: Get status from user */
TaskStatus getStatusInput(void);

/* Utility: Display separator line */
void printSeparator(void);

/* Utility: Display header */
void printHeader(const char *title);

/* Utility: Pause and wait for user input */
void pauseScreen(void);

#endif /* MENU_H */
