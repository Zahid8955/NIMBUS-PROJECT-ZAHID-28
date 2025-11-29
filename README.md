# NIMBUS-PROJECT-ZAHID-28

## Multi-User Task Management System

A collaborative Multi-User Task Management System built using C programming language. This console-based application enables multiple users to add, assign, organize and complete tasks with support for priorities, deadlines, tags, and productivity reporting.

The system is fully modular and demonstrates important concepts such as structures, dynamic memory, function-based design, sorting, searching, string handling, and menu-driven programming.

---

## Features

### User Management
- Create and manage multiple users
- Track tasks assigned and completed per user
- View individual user statistics

### Task Management
- Add tasks with title, description, priority, and deadline
- Assign tasks to team members
- Tag tasks with custom labels (up to 5 tags per task)
- Support for multiple priority levels: Low, Medium, High, Urgent
- Track task status: Pending, In Progress, Completed, Overdue

### Task Operations
- Edit task title, description, priority, and deadline
- Update task status
- Mark tasks as complete
- Add/remove tags
- Delete tasks

### Search & Filter
- Search tasks by title/description (partial match, case-insensitive)
- Search tasks by tag
- Filter by status (Pending, In Progress, Completed)
- Filter by priority level
- Filter by assigned user

### Sorting
- Sort by priority (highest first)
- Sort by deadline (earliest first)
- Sort by status

### Reports & Statistics
- Overall productivity report with completion rate
- Individual user productivity reports
- Priority-based task summary
- Upcoming deadlines report
- Overdue tasks report
- Tag distribution analysis
- Team workload distribution

---

## Technical Highlights

- **Modular Design**: Separate modules for users, tasks, reports, and menu interface
- **Dynamic Memory**: Uses `malloc`, `realloc`, and `free` for efficient memory management
- **Data Structures**: Custom structs for Users, Tasks, and Lists with dynamic arrays
- **Sorting Algorithms**: Uses `qsort` for efficient sorting by multiple criteria
- **String Handling**: Comprehensive input validation and string operations
- **Date Handling**: Validates and compares date strings in YYYY-MM-DD format

---

## Project Structure

```
NIMBUS-PROJECT-ZAHID-28/
├── include/
│   ├── types.h      # Data types and constants
│   ├── user.h       # User management declarations
│   ├── task.h       # Task management declarations
│   ├── report.h     # Reporting module declarations
│   └── menu.h       # Menu interface declarations
├── src/
│   ├── main.c       # Entry point and main loop
│   ├── user.c       # User management implementation
│   ├── task.c       # Task management implementation
│   ├── report.c     # Reporting module implementation
│   └── menu.c       # Menu interface implementation
├── Makefile         # Build configuration
└── README.md        # This file
```

---

## Building the Project

### Prerequisites
- GCC compiler (C11 standard)
- Make utility

### Build Commands

```bash
# Build the project
make

# Build and run
make run

# Build with debug symbols
make debug

# Clean build artifacts
make clean

# Show help
make help
```

---

## Usage

After building, run the executable:

```bash
./task_manager
```

The application presents a menu-driven interface:

```
========================================
  MULTI-USER TASK MANAGEMENT SYSTEM
========================================

  1. User Management
  2. Task Management
  3. Task Operations (Edit/Delete/Complete)
  4. Search & Filter Tasks
  5. Reports & Statistics
  6. Sort Tasks
  0. Exit

Enter your choice:
```

### Quick Start

1. **Add Users**: Go to User Management → Add New User
2. **Create Tasks**: Go to Task Management → Add New Task
3. **Assign Tasks**: Go to Task Management → Assign Task to User
4. **Track Progress**: Go to Reports & Statistics → Overall Productivity Report

---

## Sample Data

The application loads sample data on startup for demonstration:
- 3 sample users (John Smith, Alice Jones, Bob Wilson)
- 5 sample tasks with various priorities, deadlines, and tags

---

## Key Concepts Demonstrated

1. **Structures and Typedef**: Custom data types for Users, Tasks, and Lists
2. **Dynamic Memory Allocation**: Growing arrays with malloc/realloc
3. **Function-Based Design**: Modular functions with clear responsibilities
4. **Sorting and Searching**: Multiple sorting criteria and search algorithms
5. **String Handling**: Input validation, string comparison, substring search
6. **Menu-Driven Programming**: Interactive console interface
7. **File Organization**: Header files for declarations, source files for implementation

---

## Author

NIMBUS Project - Academic Collaborative Systems Project

---

## License

This project is created for educational purposes as part of an academic collaborative systems project.
