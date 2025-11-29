/**
 * menu.c - Menu-driven interface implementation
 * 
 * Implements the console-based menu interface for the
 * Multi-User Task Management System.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/task.h"
#include "../include/user.h"
#include "../include/report.h"
#include "../include/types.h"

/* Utility: Clear input buffer */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Utility: Get integer input with validation */
int getIntInput(const char *prompt, int min, int max) {
    int value;
    int valid = 0;
    
    while (!valid) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        if (value < min || value > max) {
            printf("Please enter a value between %d and %d.\n", min, max);
            continue;
        }
        valid = 1;
    }
    
    return value;
}

/* Utility: Get string input */
void getStringInput(const char *prompt, char *buffer, int maxLen) {
    printf("%s", prompt);
    if (fgets(buffer, maxLen, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    /* Remove trailing newline */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

/* Utility: Get priority from user */
Priority getPriorityInput(void) {
    printf("\nSelect Priority:\n");
    printf("  1. Low\n");
    printf("  2. Medium\n");
    printf("  3. High\n");
    printf("  4. Urgent\n");
    
    int choice = getIntInput("Enter choice (1-4): ", 1, 4);
    return (Priority)choice;
}

/* Utility: Get status from user */
TaskStatus getStatusInput(void) {
    printf("\nSelect Status:\n");
    printf("  0. Pending\n");
    printf("  1. In Progress\n");
    printf("  2. Completed\n");
    
    int choice = getIntInput("Enter choice (0-2): ", 0, 2);
    return (TaskStatus)choice;
}

/* Utility: Display separator line */
void printSeparator(void) {
    printf("========================================\n");
}

/* Utility: Display header */
void printHeader(const char *title) {
    printf("\n");
    printSeparator();
    printf("  %s\n", title);
    printSeparator();
}

/* Utility: Pause and wait for user input */
void pauseScreen(void) {
    printf("\nPress Enter to continue...");
    getchar();
}

/* Display main menu and get user choice */
int displayMainMenu(void) {
    printHeader("MULTI-USER TASK MANAGEMENT SYSTEM");
    printf("\n");
    printf("  1. User Management\n");
    printf("  2. Task Management\n");
    printf("  3. Task Operations (Edit/Delete/Complete)\n");
    printf("  4. Search & Filter Tasks\n");
    printf("  5. Reports & Statistics\n");
    printf("  6. Sort Tasks\n");
    printf("  0. Exit\n");
    printf("\n");
    
    return getIntInput("Enter your choice: ", 0, 6);
}

/* User management submenu */
void userManagementMenu(UserList *userList) {
    int choice;
    char username[MAX_NAME_LEN];
    char fullName[MAX_NAME_LEN];
    
    do {
        printHeader("USER MANAGEMENT");
        printf("\n");
        printf("  1. Add New User\n");
        printf("  2. View All Users\n");
        printf("  3. View User Details\n");
        printf("  4. Remove User\n");
        printf("  0. Back to Main Menu\n");
        printf("\n");
        
        choice = getIntInput("Enter your choice: ", 0, 4);
        
        switch (choice) {
            case 1: {
                printHeader("ADD NEW USER");
                getStringInput("Enter username: ", username, MAX_NAME_LEN);
                getStringInput("Enter full name: ", fullName, MAX_NAME_LEN);
                
                if (strlen(username) == 0 || strlen(fullName) == 0) {
                    printf("Error: Username and full name cannot be empty.\n");
                    break;
                }
                
                int userId = addUser(userList, username, fullName);
                if (userId > 0) {
                    printf("\n✓ User '%s' added successfully with ID: %d\n", username, userId);
                }
                pauseScreen();
                break;
            }
            
            case 2:
                printHeader("ALL USERS");
                displayAllUsers(userList);
                pauseScreen();
                break;
            
            case 3: {
                printHeader("USER DETAILS");
                displayAllUsers(userList);
                if (userList->count > 0) {
                    int userId = getIntInput("Enter User ID to view: ", 1, 9999);
                    User *user = findUserById(userList, userId);
                    displayUserDetails(user);
                }
                pauseScreen();
                break;
            }
            
            case 4: {
                printHeader("REMOVE USER");
                displayAllUsers(userList);
                if (userList->count > 0) {
                    int userId = getIntInput("Enter User ID to remove: ", 1, 9999);
                    if (removeUser(userList, userId)) {
                        printf("✓ User removed successfully.\n");
                    } else {
                        printf("✗ Failed to remove user. User not found.\n");
                    }
                }
                pauseScreen();
                break;
            }
            
            case 0:
                break;
        }
    } while (choice != 0);
}

/* Task management submenu */
void taskManagementMenu(TaskList *taskList, UserList *userList) {
    int choice;
    char title[MAX_TITLE_LEN];
    char description[MAX_DESC_LEN];
    char deadline[MAX_DATE_LEN];
    
    do {
        printHeader("TASK MANAGEMENT");
        printf("\n");
        printf("  1. Add New Task\n");
        printf("  2. View All Tasks\n");
        printf("  3. View Task Details\n");
        printf("  4. Assign Task to User\n");
        printf("  5. Add Tag to Task\n");
        printf("  0. Back to Main Menu\n");
        printf("\n");
        
        choice = getIntInput("Enter your choice: ", 0, 5);
        
        switch (choice) {
            case 1: {
                printHeader("ADD NEW TASK");
                getStringInput("Enter task title: ", title, MAX_TITLE_LEN);
                
                if (strlen(title) == 0) {
                    printf("Error: Task title cannot be empty.\n");
                    break;
                }
                
                getStringInput("Enter description (optional): ", description, MAX_DESC_LEN);
                
                Priority priority = getPriorityInput();
                
                getStringInput("Enter deadline (YYYY-MM-DD, or leave empty): ", deadline, MAX_DATE_LEN);
                
                int taskId = addTask(taskList, title, description, priority, 
                                    strlen(deadline) > 0 ? deadline : NULL, 0);
                if (taskId > 0) {
                    printf("\n✓ Task '%s' added successfully with ID: %d\n", title, taskId);
                    
                    /* Ask if user wants to assign immediately */
                    if (userList->count > 0) {
                        printf("\nDo you want to assign this task now? (y/n): ");
                        char resp[10];
                        getStringInput("", resp, 10);
                        if (resp[0] == 'y' || resp[0] == 'Y') {
                            displayAllUsers(userList);
                            int userId = getIntInput("Enter User ID to assign: ", 1, 9999);
                            Task *task = findTaskById(taskList, taskId);
                            User *user = findUserById(userList, userId);
                            if (task != NULL && user != NULL) {
                                assignTaskToUser(task, userId);
                                updateUserTaskStats(user, 1, 0);
                                printf("✓ Task assigned to %s\n", user->fullName);
                            }
                        }
                    }
                }
                pauseScreen();
                break;
            }
            
            case 2:
                printHeader("ALL TASKS");
                checkOverdueTasks(taskList);
                displayAllTasks(taskList, userList);
                pauseScreen();
                break;
            
            case 3: {
                printHeader("TASK DETAILS");
                displayAllTasks(taskList, userList);
                if (taskList->count > 0) {
                    int taskId = getIntInput("Enter Task ID to view: ", 1, 9999);
                    Task *task = findTaskById(taskList, taskId);
                    displayTask(task, userList);
                }
                pauseScreen();
                break;
            }
            
            case 4: {
                printHeader("ASSIGN TASK TO USER");
                if (taskList->count == 0) {
                    printf("No tasks available.\n");
                } else if (userList->count == 0) {
                    printf("No users available. Please add users first.\n");
                } else {
                    displayAllTasks(taskList, userList);
                    int taskId = getIntInput("Enter Task ID to assign: ", 1, 9999);
                    Task *task = findTaskById(taskList, taskId);
                    
                    if (task != NULL) {
                        displayAllUsers(userList);
                        int userId = getIntInput("Enter User ID to assign to: ", 1, 9999);
                        User *user = findUserById(userList, userId);
                        
                        if (user != NULL) {
                            /* Update old user's stats if task was previously assigned */
                            if (task->assignedUserId > 0) {
                                User *oldUser = findUserById(userList, task->assignedUserId);
                                if (oldUser != NULL) {
                                    updateUserTaskStats(oldUser, -1, 0);
                                }
                            }
                            
                            assignTaskToUser(task, userId);
                            updateUserTaskStats(user, 1, 0);
                            printf("✓ Task '%s' assigned to %s\n", task->title, user->fullName);
                        } else {
                            printf("✗ User not found.\n");
                        }
                    } else {
                        printf("✗ Task not found.\n");
                    }
                }
                pauseScreen();
                break;
            }
            
            case 5: {
                printHeader("ADD TAG TO TASK");
                if (taskList->count == 0) {
                    printf("No tasks available.\n");
                } else {
                    displayAllTasks(taskList, userList);
                    int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                    Task *task = findTaskById(taskList, taskId);
                    
                    if (task != NULL) {
                        char tag[MAX_TAG_LEN];
                        getStringInput("Enter tag to add: ", tag, MAX_TAG_LEN);
                        if (strlen(tag) > 0) {
                            if (addTagToTask(task, tag)) {
                                printf("✓ Tag '%s' added to task.\n", tag);
                            }
                        } else {
                            printf("Tag cannot be empty.\n");
                        }
                    } else {
                        printf("✗ Task not found.\n");
                    }
                }
                pauseScreen();
                break;
            }
            
            case 0:
                break;
        }
    } while (choice != 0);
}

/* Task operations submenu */
void taskOperationsMenu(TaskList *taskList, UserList *userList) {
    int choice;
    
    do {
        printHeader("TASK OPERATIONS");
        printf("\n");
        printf("  1. Edit Task Title\n");
        printf("  2. Edit Task Description\n");
        printf("  3. Change Task Priority\n");
        printf("  4. Change Task Deadline\n");
        printf("  5. Update Task Status\n");
        printf("  6. Mark Task as Complete\n");
        printf("  7. Remove Tag from Task\n");
        printf("  8. Delete Task\n");
        printf("  0. Back to Main Menu\n");
        printf("\n");
        
        choice = getIntInput("Enter your choice: ", 0, 8);
        
        if (choice >= 1 && choice <= 8 && taskList->count == 0) {
            printf("No tasks available.\n");
            pauseScreen();
            continue;
        }
        
        switch (choice) {
            case 1: {
                printHeader("EDIT TASK TITLE");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    printf("Current title: %s\n", task->title);
                    char newTitle[MAX_TITLE_LEN];
                    getStringInput("Enter new title: ", newTitle, MAX_TITLE_LEN);
                    if (strlen(newTitle) > 0) {
                        updateTaskTitle(task, newTitle);
                        printf("✓ Title updated.\n");
                    }
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 2: {
                printHeader("EDIT TASK DESCRIPTION");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    printf("Current description: %s\n", task->description);
                    char newDesc[MAX_DESC_LEN];
                    getStringInput("Enter new description: ", newDesc, MAX_DESC_LEN);
                    updateTaskDescription(task, newDesc);
                    printf("✓ Description updated.\n");
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 3: {
                printHeader("CHANGE TASK PRIORITY");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    printf("Current priority: %s\n", getPriorityString(task->priority));
                    Priority newPriority = getPriorityInput();
                    updateTaskPriority(task, newPriority);
                    printf("✓ Priority updated to %s.\n", getPriorityString(newPriority));
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 4: {
                printHeader("CHANGE TASK DEADLINE");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    printf("Current deadline: %s\n", 
                           task->deadline[0] ? task->deadline : "(none)");
                    char newDeadline[MAX_DATE_LEN];
                    getStringInput("Enter new deadline (YYYY-MM-DD): ", newDeadline, MAX_DATE_LEN);
                    if (updateTaskDeadline(task, newDeadline)) {
                        printf("✓ Deadline updated.\n");
                    }
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 5: {
                printHeader("UPDATE TASK STATUS");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    printf("Current status: %s\n", getStatusString(task->status));
                    TaskStatus newStatus = getStatusInput();
                    
                    if (newStatus == STATUS_COMPLETED && task->assignedUserId > 0) {
                        User *user = findUserById(userList, task->assignedUserId);
                        if (user != NULL) {
                            updateUserTaskStats(user, 0, 1);
                        }
                    }
                    
                    updateTaskStatus(task, newStatus);
                    printf("✓ Status updated to %s.\n", getStatusString(newStatus));
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 6: {
                printHeader("MARK TASK AS COMPLETE");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    if (task->status == STATUS_COMPLETED) {
                        printf("Task is already completed.\n");
                    } else {
                        if (task->assignedUserId > 0) {
                            User *user = findUserById(userList, task->assignedUserId);
                            if (user != NULL) {
                                updateUserTaskStats(user, 0, 1);
                            }
                        }
                        completeTask(task);
                        printf("✓ Task '%s' marked as complete!\n", task->title);
                    }
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 7: {
                printHeader("REMOVE TAG FROM TASK");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    if (task->tagCount == 0) {
                        printf("This task has no tags.\n");
                    } else {
                        printf("Current tags: ");
                        for (int i = 0; i < task->tagCount; i++) {
                            printf("[%s] ", task->tags[i]);
                        }
                        printf("\n");
                        
                        char tag[MAX_TAG_LEN];
                        getStringInput("Enter tag to remove: ", tag, MAX_TAG_LEN);
                        if (removeTagFromTask(task, tag)) {
                            printf("✓ Tag '%s' removed.\n", tag);
                        } else {
                            printf("✗ Tag not found.\n");
                        }
                    }
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 8: {
                printHeader("DELETE TASK");
                displayAllTasks(taskList, userList);
                int taskId = getIntInput("Enter Task ID to delete: ", 1, 9999);
                Task *task = findTaskById(taskList, taskId);
                
                if (task != NULL) {
                    printf("Are you sure you want to delete '%s'? (y/n): ", task->title);
                    char confirm[10];
                    getStringInput("", confirm, 10);
                    
                    if (confirm[0] == 'y' || confirm[0] == 'Y') {
                        /* Update user stats if task was assigned */
                        if (task->assignedUserId > 0) {
                            User *user = findUserById(userList, task->assignedUserId);
                            if (user != NULL) {
                                int completed = (task->status == STATUS_COMPLETED) ? -1 : 0;
                                updateUserTaskStats(user, -1, completed);
                            }
                        }
                        
                        if (removeTask(taskList, taskId)) {
                            printf("✓ Task deleted successfully.\n");
                        }
                    } else {
                        printf("Delete cancelled.\n");
                    }
                } else {
                    printf("✗ Task not found.\n");
                }
                pauseScreen();
                break;
            }
            
            case 0:
                break;
        }
    } while (choice != 0);
}

/* Search and filter submenu */
void searchFilterMenu(TaskList *taskList, UserList *userList) {
    int choice;
    
    do {
        printHeader("SEARCH & FILTER TASKS");
        printf("\n");
        printf("  1. Search by Title/Description\n");
        printf("  2. Search by Tag\n");
        printf("  3. Filter by Status\n");
        printf("  4. Filter by Priority\n");
        printf("  5. Filter by Assigned User\n");
        printf("  0. Back to Main Menu\n");
        printf("\n");
        
        choice = getIntInput("Enter your choice: ", 0, 5);
        
        switch (choice) {
            case 1: {
                printHeader("SEARCH BY TITLE/DESCRIPTION");
                char searchTerm[MAX_TITLE_LEN];
                getStringInput("Enter search term: ", searchTerm, MAX_TITLE_LEN);
                if (strlen(searchTerm) > 0) {
                    searchTasksByTitle(taskList, userList, searchTerm);
                }
                pauseScreen();
                break;
            }
            
            case 2: {
                printHeader("SEARCH BY TAG");
                char tag[MAX_TAG_LEN];
                getStringInput("Enter tag to search: ", tag, MAX_TAG_LEN);
                if (strlen(tag) > 0) {
                    searchTasksByTag(taskList, userList, tag);
                }
                pauseScreen();
                break;
            }
            
            case 3: {
                printHeader("FILTER BY STATUS");
                TaskStatus status = getStatusInput();
                displayTasksByStatus(taskList, userList, status);
                pauseScreen();
                break;
            }
            
            case 4: {
                printHeader("FILTER BY PRIORITY");
                Priority priority = getPriorityInput();
                displayTasksByPriority(taskList, userList, priority);
                pauseScreen();
                break;
            }
            
            case 5: {
                printHeader("FILTER BY ASSIGNED USER");
                if (userList->count == 0) {
                    printf("No users available.\n");
                } else {
                    displayAllUsers(userList);
                    int userId = getIntInput("Enter User ID: ", 1, 9999);
                    displayTasksByUser(taskList, userList, userId);
                }
                pauseScreen();
                break;
            }
            
            case 0:
                break;
        }
    } while (choice != 0);
}

/* Reports submenu */
void reportsMenu(TaskList *taskList, UserList *userList) {
    int choice;
    
    do {
        printHeader("REPORTS & STATISTICS");
        printf("\n");
        printf("  1. Overall Productivity Report\n");
        printf("  2. User Productivity Report\n");
        printf("  3. Priority Summary\n");
        printf("  4. Upcoming Deadlines\n");
        printf("  5. Overdue Tasks Report\n");
        printf("  6. Tag Distribution\n");
        printf("  7. Team Workload Distribution\n");
        printf("  0. Back to Main Menu\n");
        printf("\n");
        
        choice = getIntInput("Enter your choice: ", 0, 7);
        
        switch (choice) {
            case 1:
                checkOverdueTasks(taskList);
                displayOverallReport(taskList, userList);
                pauseScreen();
                break;
            
            case 2: {
                if (userList->count == 0) {
                    printf("No users available.\n");
                } else {
                    displayAllUsers(userList);
                    int userId = getIntInput("Enter User ID: ", 1, 9999);
                    User *user = findUserById(userList, userId);
                    if (user != NULL) {
                        displayUserReport(taskList, user);
                    } else {
                        printf("User not found.\n");
                    }
                }
                pauseScreen();
                break;
            }
            
            case 3:
                displayPrioritySummary(taskList);
                pauseScreen();
                break;
            
            case 4: {
                int days = getIntInput("Show tasks due within how many days? (1-365): ", 1, 365);
                displayUpcomingDeadlines(taskList, userList, days);
                pauseScreen();
                break;
            }
            
            case 5:
                displayOverdueReport(taskList, userList);
                pauseScreen();
                break;
            
            case 6:
                displayTagDistribution(taskList);
                pauseScreen();
                break;
            
            case 7:
                displayWorkloadDistribution(taskList, userList);
                pauseScreen();
                break;
            
            case 0:
                break;
        }
    } while (choice != 0);
}
