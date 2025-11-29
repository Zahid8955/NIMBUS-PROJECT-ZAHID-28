/**
 * user.c - User management module implementation
 * 
 * Implements functions for creating, managing, and querying users
 * in the Multi-User Task Management System.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/user.h"
#include "../include/types.h"

/* Static counter for user IDs */
static int nextUserId = 1;

/* Initialize user list with dynamic memory allocation */
void initUserList(UserList *list) {
    if (list == NULL) return;
    
    list->users = (User *)malloc(INITIAL_CAPACITY * sizeof(User));
    if (list->users == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for user list\n");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
}

/* Free user list memory */
void freeUserList(UserList *list) {
    if (list == NULL) return;
    
    free(list->users);
    list->users = NULL;
    list->count = 0;
    list->capacity = 0;
}

/* Helper function to resize user list if needed */
static int resizeUserList(UserList *list) {
    if (list == NULL) return 0;
    
    if (list->count >= list->capacity) {
        int newCapacity = list->capacity * 2;
        User *newUsers = (User *)realloc(list->users, newCapacity * sizeof(User));
        if (newUsers == NULL) {
            fprintf(stderr, "Error: Memory reallocation failed for user list\n");
            return 0;
        }
        list->users = newUsers;
        list->capacity = newCapacity;
    }
    return 1;
}

/* Add a new user */
int addUser(UserList *list, const char *username, const char *fullName) {
    if (list == NULL || username == NULL || fullName == NULL) {
        return -1;
    }
    
    /* Check if username already exists */
    if (findUserByUsername(list, username) != NULL) {
        printf("Error: Username '%s' already exists\n", username);
        return -1;
    }
    
    /* Resize if needed */
    if (!resizeUserList(list)) {
        return -1;
    }
    
    /* Create new user */
    User *newUser = &list->users[list->count];
    newUser->id = nextUserId++;
    strncpy(newUser->username, username, MAX_NAME_LEN - 1);
    newUser->username[MAX_NAME_LEN - 1] = '\0';
    strncpy(newUser->fullName, fullName, MAX_NAME_LEN - 1);
    newUser->fullName[MAX_NAME_LEN - 1] = '\0';
    newUser->tasksAssigned = 0;
    newUser->tasksCompleted = 0;
    
    list->count++;
    
    return newUser->id;
}

/* Find user by ID */
User* findUserById(UserList *list, int id) {
    if (list == NULL) return NULL;
    
    for (int i = 0; i < list->count; i++) {
        if (list->users[i].id == id) {
            return &list->users[i];
        }
    }
    return NULL;
}

/* Find user by username */
User* findUserByUsername(UserList *list, const char *username) {
    if (list == NULL || username == NULL) return NULL;
    
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->users[i].username, username) == 0) {
            return &list->users[i];
        }
    }
    return NULL;
}

/* Get user index by ID */
int getUserIndexById(UserList *list, int id) {
    if (list == NULL) return -1;
    
    for (int i = 0; i < list->count; i++) {
        if (list->users[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Remove user by ID */
int removeUser(UserList *list, int id) {
    if (list == NULL) return 0;
    
    int index = getUserIndexById(list, id);
    if (index == -1) {
        return 0;
    }
    
    /* Shift remaining users */
    for (int i = index; i < list->count - 1; i++) {
        list->users[i] = list->users[i + 1];
    }
    list->count--;
    
    return 1;
}

/* Display all users */
void displayAllUsers(const UserList *list) {
    if (list == NULL || list->count == 0) {
        printf("No users found.\n");
        return;
    }
    
    printf("\n%-5s %-15s %-20s %-12s %-12s\n", 
           "ID", "Username", "Full Name", "Assigned", "Completed");
    printf("----------------------------------------------------------------------\n");
    
    for (int i = 0; i < list->count; i++) {
        printf("%-5d %-15s %-20s %-12d %-12d\n",
               list->users[i].id,
               list->users[i].username,
               list->users[i].fullName,
               list->users[i].tasksAssigned,
               list->users[i].tasksCompleted);
    }
    printf("----------------------------------------------------------------------\n");
    printf("Total Users: %d\n", list->count);
}

/* Display user details */
void displayUserDetails(const User *user) {
    if (user == NULL) {
        printf("User not found.\n");
        return;
    }
    
    printf("\n=== User Details ===\n");
    printf("ID:              %d\n", user->id);
    printf("Username:        %s\n", user->username);
    printf("Full Name:       %s\n", user->fullName);
    printf("Tasks Assigned:  %d\n", user->tasksAssigned);
    printf("Tasks Completed: %d\n", user->tasksCompleted);
    
    if (user->tasksAssigned > 0) {
        double rate = (double)user->tasksCompleted / user->tasksAssigned * 100.0;
        printf("Completion Rate: %.1f%%\n", rate);
    } else {
        printf("Completion Rate: N/A\n");
    }
    printf("====================\n");
}

/* Update user's task statistics */
void updateUserTaskStats(User *user, int assigned, int completed) {
    if (user == NULL) return;
    
    user->tasksAssigned += assigned;
    user->tasksCompleted += completed;
}

/* Get priority as string */
const char* getPriorityString(Priority priority) {
    switch (priority) {
        case PRIORITY_LOW:    return "Low";
        case PRIORITY_MEDIUM: return "Medium";
        case PRIORITY_HIGH:   return "High";
        case PRIORITY_URGENT: return "Urgent";
        default:              return "Unknown";
    }
}

/* Get status as string */
const char* getStatusString(TaskStatus status) {
    switch (status) {
        case STATUS_PENDING:     return "Pending";
        case STATUS_IN_PROGRESS: return "In Progress";
        case STATUS_COMPLETED:   return "Completed";
        case STATUS_OVERDUE:     return "Overdue";
        default:                 return "Unknown";
    }
}
