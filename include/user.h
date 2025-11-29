/**
 * user.h - User management module header
 * 
 * Provides functions for creating, managing, and querying users
 * in the Multi-User Task Management System.
 */

#ifndef USER_H
#define USER_H

#include "types.h"

/* Initialize user list with dynamic memory allocation */
void initUserList(UserList *list);

/* Free user list memory */
void freeUserList(UserList *list);

/* Add a new user */
int addUser(UserList *list, const char *username, const char *fullName);

/* Find user by ID */
User* findUserById(UserList *list, int id);

/* Find user by username */
User* findUserByUsername(UserList *list, const char *username);

/* Get user index by ID */
int getUserIndexById(UserList *list, int id);

/* Remove user by ID */
int removeUser(UserList *list, int id);

/* Display all users */
void displayAllUsers(const UserList *list);

/* Display user details */
void displayUserDetails(const User *user);

/* Update user's task statistics */
void updateUserTaskStats(User *user, int assigned, int completed);

/* Get priority as string */
const char* getPriorityString(Priority priority);

/* Get status as string */
const char* getStatusString(TaskStatus status);

#endif /* USER_H */
