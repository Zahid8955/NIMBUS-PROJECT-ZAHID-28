/**
 * report.h - Productivity reporting module header
 * 
 * Provides functions for generating productivity reports and statistics
 * for the Multi-User Task Management System.
 */

#ifndef REPORT_H
#define REPORT_H

#include "types.h"

/* Calculate overall productivity statistics */
ProductivityStats calculateOverallStats(const TaskList *taskList);

/* Calculate user-specific productivity statistics */
ProductivityStats calculateUserStats(const TaskList *taskList, int userId);

/* Display overall productivity report */
void displayOverallReport(const TaskList *taskList, const UserList *userList);

/* Display user productivity report */
void displayUserReport(const TaskList *taskList, const User *user);

/* Display tasks summary by priority */
void displayPrioritySummary(const TaskList *taskList);

/* Display upcoming deadlines (tasks due within n days) */
void displayUpcomingDeadlines(const TaskList *taskList, const UserList *userList, int days);

/* Display overdue tasks report */
void displayOverdueReport(const TaskList *taskList, const UserList *userList);

/* Display tag distribution */
void displayTagDistribution(const TaskList *taskList);

/* Display team workload distribution */
void displayWorkloadDistribution(const TaskList *taskList, const UserList *userList);

#endif /* REPORT_H */
