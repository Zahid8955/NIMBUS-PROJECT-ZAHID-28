/**
 * main.c - Entry point for Multi-User Task Management System
 * 
 * A collaborative Multi-User Task Management System built using C programming language.
 * This console-based application enables multiple users to add, assign, organize and
 * complete tasks with support for priorities, deadlines, tags, and productivity reporting.
 * 
 * Author: NIMBUS Project
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/types.h"
#include "../include/user.h"
#include "../include/task.h"
#include "../include/menu.h"
#include "../include/report.h"

/* Sort tasks menu */
void sortTasksMenu(TaskList *taskList, UserList *userList) {
    int choice;
    
    do {
        printHeader("SORT TASKS");
        printf("\n");
        printf("  1. Sort by Priority (Highest First)\n");
        printf("  2. Sort by Deadline (Earliest First)\n");
        printf("  3. Sort by Status\n");
        printf("  0. Back to Main Menu\n");
        printf("\n");
        
        choice = getIntInput("Enter your choice: ", 0, 3);
        
        switch (choice) {
            case 1:
                sortTasksByPriority(taskList);
                displayAllTasks(taskList, userList);
                pauseScreen();
                break;
            
            case 2:
                sortTasksByDeadline(taskList);
                displayAllTasks(taskList, userList);
                pauseScreen();
                break;
            
            case 3:
                sortTasksByStatus(taskList);
                displayAllTasks(taskList, userList);
                pauseScreen();
                break;
            
            case 0:
                break;
        }
    } while (choice != 0);
}

int main(void) {
    UserList userList;
    TaskList taskList;
    
    /* Initialize data structures */
    initUserList(&userList);
    initTaskList(&taskList);
    
    /* Welcome message */
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║      WELCOME TO MULTI-USER TASK MANAGEMENT SYSTEM            ║\n");
    printf("║                                                              ║\n");
    printf("║   A collaborative task management tool for teams            ║\n");
    printf("║   with priorities, deadlines, tags, and reporting           ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    /* Add some sample data for demonstration */
    printf("Loading sample data for demonstration...\n\n");
    
    /* Sample users */
    addUser(&userList, "jsmith", "John Smith");
    addUser(&userList, "ajones", "Alice Jones");
    addUser(&userList, "bwilson", "Bob Wilson");
    
    /* Sample tasks */
    int taskId1 = addTask(&taskList, "Complete project documentation", 
                          "Write comprehensive documentation for the task management system",
                          PRIORITY_HIGH, "2025-12-15", 1);
    Task *task1 = findTaskById(&taskList, taskId1);
    if (task1) {
        assignTaskToUser(task1, 1);
        addTagToTask(task1, "documentation");
        addTagToTask(task1, "priority");
        updateUserTaskStats(findUserById(&userList, 1), 1, 0);
    }
    
    int taskId2 = addTask(&taskList, "Implement user authentication", 
                          "Add login functionality with secure password handling",
                          PRIORITY_URGENT, "2025-12-01", 1);
    Task *task2 = findTaskById(&taskList, taskId2);
    if (task2) {
        assignTaskToUser(task2, 2);
        addTagToTask(task2, "security");
        addTagToTask(task2, "backend");
        updateUserTaskStats(findUserById(&userList, 2), 1, 0);
    }
    
    int taskId3 = addTask(&taskList, "Design database schema", 
                          "Create efficient database structure for task storage",
                          PRIORITY_MEDIUM, "2025-12-10", 1);
    Task *task3 = findTaskById(&taskList, taskId3);
    if (task3) {
        assignTaskToUser(task3, 3);
        addTagToTask(task3, "database");
        updateUserTaskStats(findUserById(&userList, 3), 1, 0);
    }
    
    int taskId4 = addTask(&taskList, "Write unit tests", 
                          "Create comprehensive test suite for all modules",
                          PRIORITY_HIGH, "2025-12-20", 1);
    Task *task4 = findTaskById(&taskList, taskId4);
    if (task4) {
        assignTaskToUser(task4, 1);
        addTagToTask(task4, "testing");
        addTagToTask(task4, "quality");
        updateUserTaskStats(findUserById(&userList, 1), 1, 0);
    }
    
    int taskId5 = addTask(&taskList, "Code review session", 
                          "Review and approve pending pull requests",
                          PRIORITY_LOW, "2025-12-05", 1);
    Task *task5 = findTaskById(&taskList, taskId5);
    if (task5) {
        addTagToTask(task5, "review");
    }
    
    printf("✓ Sample data loaded: 3 users, 5 tasks\n");
    pauseScreen();
    
    /* Main program loop */
    int running = 1;
    while (running) {
        int choice = displayMainMenu();
        
        switch (choice) {
            case 1:
                userManagementMenu(&userList);
                break;
            
            case 2:
                taskManagementMenu(&taskList, &userList);
                break;
            
            case 3:
                taskOperationsMenu(&taskList, &userList);
                break;
            
            case 4:
                searchFilterMenu(&taskList, &userList);
                break;
            
            case 5:
                reportsMenu(&taskList, &userList);
                break;
            
            case 6:
                sortTasksMenu(&taskList, &userList);
                break;
            
            case 0:
                printf("\n");
                printf("Thank you for using Multi-User Task Management System!\n");
                printf("Goodbye!\n\n");
                running = 0;
                break;
        }
    }
    
    /* Cleanup */
    freeUserList(&userList);
    freeTaskList(&taskList);
    
    return 0;
}
