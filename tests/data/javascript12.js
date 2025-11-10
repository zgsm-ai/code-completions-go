/**
 * Task Management System
 * 
 * This module implements a comprehensive task management system
 * that can handle projects, tasks, assignments, and deadlines.
 * 
 * Features:
 * - Project management
 * - Task creation and assignment
 * - Priority and status tracking
 * - Progress tracking
 * - Team collaboration
 * - Notification system
 */

// Task class represents a single task
class Task {
    constructor(taskId, title, description, priority = 'medium', dueDate = null, projectId = null) {
        this.taskId = taskId;
        this.title = title;
        this.description = description;
        this.priority = priority; // low, medium, high, critical
        this.status = 'new'; // new, in_progress, completed, cancelled
        this.dueDate = dueDate;
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.completedAt = null;
        this.assignedTo = [];
        this.tags = [];
        this.comments = [];
        this.progress = 0; // 0-100 percentage
        this.projectId = projectId;
        this.dependencies = [];
        this.attachments = [];
    }
    
    // Update task information
    updateTask(updates) {
        Object.assign(this, updates);
        this.updatedAt = new Date();
    }
    
    // Assign to user
    assignTo(userId) {
        if (!this.assignedTo.includes(userId)) {
            this.assignedTo.push(userId);
            this.updatedAt = new Date();
        }
    }
    
    // Remove assignment
    removeAssignment(userId) {
        const index = this.assignedTo.indexOf(userId);
        if (index > -1) {
            this.assignedTo.splice(index, 1);
            this.updatedAt = new Date();
        }
    }
    
    // Update status
    updateStatus(status) {
        this.status = status;
        this.updatedAt = new Date();
        
        if (status === 'completed' && !this.completedAt) {
            this.completedAt = new Date();
            this.progress = 100;
        }
    }
    
    // Update progress
    updateProgress(progress) {
        this.progress = Math.min(100, Math.max(0, progress));
        this.updatedAt = new Date();
        
        if (this.progress === 100 && this.status !== 'completed') {
            this.updateStatus('completed');
        }
    }
    
    // Add tag
    addTag(tag) {
        if (!this.tags.includes(tag)) {
            this.tags.push(tag);
            this.updatedAt = new Date();
        }
    }
    
    // Remove tag
    removeTag(tag) {
        const index = this.tags.indexOf(tag);
        if (index > -1) {
            this.tags.splice(index, 1);
            this.updatedAt = new Date();
        }
    }
    
    // Add dependency
    addDependency(taskId) {
        if (!this.dependencies.includes(taskId)) {
            this.dependencies.push(taskId);
            this.updatedAt = new Date();
        }
    }
    
    // Remove dependency
    removeDependency(taskId) {
        const index = this.dependencies.indexOf(taskId);
        if (index > -1) {
            this.dependencies.splice(index, 1);
            this.updatedAt = new Date();
        }
    }
    
    // Add comment
    addComment(comment) {
        this.comments.push({
            text: comment.text,
            author: comment.author,
            createdAt: comment.createdAt || new Date()
        });
        this.updatedAt = new Date();
    }
    
    // Add attachment
    addAttachment(attachment) {
        this.attachments.push({
            id: Date.now().toString(),
            name: attachment.name,
            url: attachment.url,
            size: attachment.size || 0,
            type: attachment.type
        });
        this.updatedAt = new Date();
    }
    
    // Check if task is overdue
    isOverdue() {
        if (!this.dueDate) return false;
        return new Date() > this.dueDate && this.status !== 'completed';
    }
    
    // Get days until due
    getDaysUntilDue() {
        if (!this.dueDate) return null;
        const now = new Date();
        const due = new Date(this.dueDate);
        const timeDiff = due.getTime() - now.getTime();
        return Math.ceil(timeDiff / (1000 * 60 * 60 * 24));
    }
    
    // Get priority weight
    getPriorityWeight() {
        const weights = {
            low: 1,
            medium: 2,
            high: 3,
            critical: 4
        };
        return weights[this.priority] || 2;
    }
    
    // Get task summary
    getSummary() {
        return {
            taskId: this.taskId,
            title: this.title,
            description: this.description,
            priority: this.priority,
            status: this.status,
            progress: this.progress,
            dueDate: this.dueDate,
            createdAt: this.createdAt,
            updatedAt: this.updatedAt,
            completedAt: this.completedAt,
            assignedTo: this.assignedTo,
            tags: this.tags,
            commentCount: this.comments.length,
            attachmentCount: this.attachments.length,
            projectId: this.projectId,
            dependencyCount: this.dependencies.length,
            isOverdue: this.isOverdue(),
            daysUntilDue: this.getDaysUntilDue(),
            priorityWeight: this.getPriorityWeight()
        };
    }
}

// Project class represents a project
class Project {
    constructor(projectId, name, description, startDate = null, endDate = null) {
        this.projectId = projectId;
        this.name = name;
        this.description = description;
        this.status = 'planning'; // planning, active, on_hold, completed, cancelled
        this.startDate = startDate;
        this.endDate = endDate;
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.teamMembers = [];
        this.tasks = [];
        this.milestones = [];
        this.budget = null;
        this.progress = 0; // 0-100 percentage
    }
    
    // Update project
    updateProject(updates) {
        Object.assign(this, updates);
        this.updatedAt = new Date();
        
        // If status changes to active and no start date, set start date to today
        if (updates.status === 'active' && !this.startDate) {
            this.startDate = new Date();
        }
    }
    
    // Add team member
    addTeamMember(userId, role = 'member') {
        if (!this.teamMembers.find(m => m.userId === userId)) {
            this.teamMembers.push({
                userId: userId,
                role: role,
                joinedAt: new Date()
            });
            this.updatedAt = new Date();
        }
    }
    
    // Remove team member
    removeTeamMember(userId) {
        const index = this.teamMembers.findIndex(m => m.userId === userId);
        if (index > -1) {
            this.teamMembers.splice(index, 1);
            this.updatedAt = new Date();
        }
    }
    
    // Add task
    addTask(task) {
        if (!this.tasks.find(t => t.taskId === task.taskId)) {
            this.tasks.push(task);
            
            // Update task project reference
            task.projectId = this.projectId;
            this.updatedAt = new Date();
            
            // Update project progress based on tasks
            this.updateProgress();
        }
    }
    
    // Remove task
    removeTask(taskId) {
        const index = this.tasks.findIndex(t => t.taskId === taskId);
        if (index > -1) {
            this.tasks.splice(index, 1);
            this.updatedAt = new Date();
            
            // Update project progress
            this.updateProgress();
        }
    }
    
    // Update task in project
    updateTask(taskId, updates) {
        const task = this.tasks.find(t => t.taskId === taskId);
        if (task) {
            task.updateTask(updates);
            this.updatedAt = new Date();
            
            // Update project progress
            this.updateProgress();
        }
    }
    
    // Add milestone
    addMilestone(title, dueDate, description = '') {
        this.milestones.push({
            id: Date.now().toString(),
            title: title,
            description: description,
            dueDate: dueDate,
            completed: false,
            completedAt: null,
            createdAt: new Date()
        });
        this.updatedAt = new Date();
    }
    
    // Complete milestone
    completeMilestone(milestoneId) {
        const milestone = this.milestones.find(m => m.id === milestoneId);
        if (milestone) {
            milestone.completed = true;
            milestone.completedAt = new Date();
            this.updatedAt = new Date();
        }
    }
    
    // Update project progress based on tasks
    updateProgress() {
        if (this.tasks.length === 0) {
            this.progress = 0;
            return;
        }
        
        const totalTasks = this.tasks.length;
        const completedTasks = this.tasks.filter(t => t.status === 'completed').length;
        
        // Simple progress calculation based on completed tasks
        this.progress = Math.round((completedTasks / totalTasks) * 100);
        
        // If all tasks are completed, mark project as completed
        if (this.progress === 100 && this.status !== 'completed') {
            this.status = 'completed';
        }
    }
    
    // Get project summary
    getSummary() {
        return {
            projectId: this.projectId,
            name: this.name,
            description: this.description,
            status: this.status,
            progress: this.progress,
            startDate: this.startDate,
            endDate: this.endDate,
            createdAt: this.createdAt,
            updatedAt: this.updatedAt,
            teamMemberCount: this.teamMembers.length,
            taskCount: this.tasks.length,
            completedTaskCount: this.tasks.filter(t => t.status === 'completed').length,
            milestoneCount: this.milestones.length,
            completedMilestoneCount: this.milestones.filter(m => m.completed).length,
            budget: this.budget
        };
    }
}

// User class represents a user
class User {
    constructor(userId, username, email, firstName, lastName) {
        this.userId = userId;
        this.username = username;
        this.email = email;
        this.firstName = firstName;
        this.lastName = lastName;
        this.fullName = `${firstName} ${lastName}`;
        this.avatar = '';
        this.skills = [];
        this.preferences = {
            notifications: {
                email: true,
                push: true,
                inApp: true
            },
            theme: 'light',
            language: 'en'
        };
        this.createdAt = new Date();
        this.lastLogin = new Date();
    }
    
    // Update profile
    updateProfile(updates) {
        Object.assign(this, updates);
    }
    
    // Add skill
    addSkill(skill) {
        if (!this.skills.includes(skill)) {
            this.skills.push(skill);
        }
    }
    
    // Remove skill
    removeSkill(skill) {
        const index = this.skills.indexOf(skill);
        if (index > -1) {
            this.skills.splice(index, 1);
        }
    }
    
    // Update preferences
    updatePreferences(newPreferences) {
        this.preferences = {
            ...this.preferences,
            ...newPreferences
        };
    }
    
    // Record login
    recordLogin() {
        this.lastLogin = new Date();
    }
}

// Notification class represents a notification
class Notification {
    constructor(notificationId, userId, title, message, type = 'info', relatedId = null) {
        this.notificationId = notificationId;
        this.userId = userId;
        this.title = title;
        this.message = message;
        this.type = type; // info, warning, error, success
        this.relatedId = relatedId; // Related task, project, etc.
        this.read = false;
        this.createdAt = new Date();
    }
    
    // Mark as read
    markAsRead() {
        this.read = true;
    }
    
    // Get notification summary
    getSummary() {
        return {
            notificationId: this.notificationId,
            userId: this.userId,
            title: this.title,
            message: this.message,
            type: this.type,
            relatedId: this.relatedId,
            read: this.read,
            createdAt: this.createdAt
        };
    }
}

// TaskManager class manages tasks and projects
class TaskManager {
    constructor() {
        this.users = new Map();
        this.projects = new Map();
        this.tasks = new Map();
        this.notifications = [];
        this.initializeDemoData();
    }
    
    // Initialize demo data
    initializeDemoData() {
        // Create demo users
        const users = [
            new User('u1', 'johndoe', 'john@example.com', 'John', 'Doe'),
            new User('u2', 'janesmith', 'jane@example.com', 'Jane', 'Smith'),
            new User('u3', 'bobjohnson', 'bob@example.com', 'Bob', 'Johnson')
        ];
        
        users.forEach(user => {
            this.users.set(user.userId, user);
        });
        
        // Create demo projects
        const projects = [
            new Project('p1', 'Website Redesign', 'Complete redesign of company website'),
            new Project('p2', 'Mobile App Development', 'Develop mobile app for task management'),
            new Project('p3', 'Marketing Campaign', 'Q4 marketing campaign for new product launch')
        ];
        
        projects.forEach(project => {
            this.projects.set(project.projectId, project);
            
            // Add team members
            if (project.projectId === 'p1') {
                project.addTeamMember('u1', 'manager');
                project.addTeamMember('u2', 'developer');
                project.updateProject({ status: 'active', startDate: new Date(2023, 5, 1) });
                
                // Add tasks
                const task1 = new Task('t1', 'Design homepage mockup', 'Create initial design mockup for homepage', 'high', new Date(2023, 5, 15));
                const task2 = new Task('t2', 'Implement homepage layout', 'Code the HTML/CSS for homepage layout', 'medium', new Date(2023, 5, 25));
                const task3 = new Task('t3', 'Review and test homepage', 'Test homepage on different browsers and devices', 'low', new Date(2023, 5, 30));
                
                project.addTask(task1);
                project.addTask(task2);
                project.addTask(task3);
                
                // Add dependencies
                task2.addDependency('t1');
                task3.addDependency('t1');
                task3.addDependency('t2');
                
                // Update task status
                task1.updateStatus('completed');
                task2.updateStatus('in_progress');
            } else if (project.projectId === 'p2') {
                project.addTeamMember('u2', 'manager');
                project.addTeamMember('u3', 'developer');
                
                // Add milestones
                project.addMilestone('Design Phase Complete', new Date(2023, 6, 1));
                project.addMilestone('Development Phase Complete', new Date(2023, 7, 15));
                project.addMilestone('Beta Release', new Date(2023, 8, 15));
                project.addMilestone('Final Release', new Date(2023, 9, 1));
                
                // Add budget
                project.updateProject({ budget: 50000 });
            }
        });
    }
    
    // Get user by ID
    getUserById(userId) {
        return this.users.get(userId);
    }
    
    // Get all users
    getAllUsers() {
        return Array.from(this.users.values());
    }
    
    // Create a new project
    createProject(name, description, startDate, endDate, budget = null) {
        const projectId = 'p' + Date.now();
        const project = new Project(projectId, name, description, startDate, endDate);
        if (budget !== null) {
            project.budget = budget;
        }
        
        this.projects.set(projectId, project);
        return project;
    }
    
    // Get project by ID
    getProjectById(projectId) {
        return this.projects.get(projectId);
    }
    
    // Get all projects
    getAllProjects() {
        return Array.from(this.projects.values());
    }
    
    // Get projects for user
    getProjectsForUser(userId) {
        return Array.from(this.projects.values()).filter(project => 
            project.teamMembers.some(member => member.userId === userId)
        );
    }
    
    // Create a new task
    createTask(title, description, priority, dueDate, projectId = null) {
        const taskId = 't' + Date.now();
        const task = new Task(taskId, title, description, priority, dueDate, projectId);
        
        this.tasks.set(taskId, task);
        
        // Add task to project if specified
        if (projectId) {
            const project = this.getProjectById(projectId);
            if (project) {
                project.addTask(task);
                this.projects.set(projectId, project);
            }
        }
        
        return task;
    }
    
    // Get task by ID
    getTaskById(taskId) {
        return this.tasks.get(taskId);
    }
    
    // Get all tasks
    getAllTasks() {
        return Array.from(this.tasks.values());
    }
    
    // Get tasks for user
    getTasksForUser(userId) {
        return Array.from(this.tasks.values()).filter(task => 
            task.assignedTo.includes(userId)
        );
    }
    
    // Get tasks for project
    getTasksForProject(projectId) {
        return Array.from(this.tasks.values()).filter(task => 
            task.projectId === projectId
        );
    }
    
    // Update task
    updateTask(taskId, updates) {
        const task = this.getTaskById(taskId);
        if (task) {
            task.updateTask(updates);
            this.tasks.set(taskId, task);
            
            // If task belongs to a project, update project progress
            if (task.projectId) {
                const project = this.getProjectById(task.projectId);
                if (project) {
                    project.updateTask(taskId, updates);
                    this.projects.set(task.projectId, project);
                }
            }
        }
        
        return task;
    }
    
    // Create notification
    createNotification(userId, title, message, type, relatedId) {
        const notificationId = 'n' + Date.now();
        const notification = new Notification(notificationId, userId, title, message, type, relatedId);
        
        this.notifications.unshift(notification);
        return notification;
    }
    
    // Get notifications for user
    getNotificationsForUser(userId) {
        return this.notifications.filter(n => n.userId === userId);
    }
    
    // Mark notification as read
    markNotificationAsRead(notificationId) {
        const notification = this.notifications.find(n => n.notificationId === notificationId);
        if (notification) {
            notification.markAsRead();
        }
    }
    
    // Get analytics data
    getAnalytics() {
        const totalUsers = this.users.size;
        const totalProjects = this.projects.size;
        const totalTasks = this.tasks.size;
        
        // Calculate project statistics
        const projectStats = {
            planning: 0,
            active: 0,
            on_hold: 0,
            completed: 0,
            cancelled: 0,
            total: totalProjects
        };
        
        const projects = Array.from(this.projects.values());
        projects.forEach(project => {
            projectStats[project.status]++;
        });
        
        // Calculate task statistics
        const taskStats = {
            new: 0,
            in_progress: 0,
            completed: 0,
            cancelled: 0,
            total: totalTasks,
            overdue: 0
        };
        
        const tasks = Array.from(this.tasks.values());
        tasks.forEach(task => {
            taskStats[task.status]++;
            if (task.isOverdue()) {
                taskStats.overdue++;
            }
        });
        
        // Get top users by task count
        const userTaskCounts = new Map();
        tasks.forEach(task => {
            task.assignedTo.forEach(userId => {
                userTaskCounts.set(userId, (userTaskCounts.get(userId) || 0) + 1);
            });
        });
        
        const topUsers = Array.from(userTaskCounts.entries())
            .sort((a, b) => b[1] - a[1])
            .slice(0, 3)
            .map(([userId, count]) => {
                const user = this.getUserById(userId);
                return {
                    user: user ? user.getFullName() : 'Unknown',
                    count
                };
            });
        
        return {
            summary: {
                totalUsers,
                totalProjects,
                totalTasks,
                totalNotifications: this.notifications.length
            },
            projectStats,
            taskStats,
            topUsers
        };
    }
}

// Main function to demonstrate task management
function main() {
    // Create a new task manager
    const taskManager = new TaskManager();
    
    // Get all users
    const users = taskManager.getAllUsers();
    console.log('Users:', users.map(u => u.fullName));
    
    // Get all projects
    const projects = taskManager.getAllProjects();
    console.log('Projects:', projects.map(p => `${p.name} (${p.status})`));
    
    // Get all tasks
    const tasks = taskManager.getAllTasks();
    console.log('Tasks:', tasks.map(t => `${t.title} (${t.status})`));
    
    // Get tasks for a specific user
    const userTasks = taskManager.getTasksForUser('u1');
    console.log('Tasks for user u1:', userTasks.map(t => t.title));
    
    // Update a task
    const updatedTask = taskManager.updateTask('t2', { status: 'completed', progress: 100 });
    console.log('Updated task:', updatedTask.getSummary());
    
    // Create a new task
    const newTask = taskManager.createTask(
        'Prepare presentation',
        'Create slides for quarterly review meeting',
        'high',
        new Date(2023, 6, 10)
    );
    console.log('New task created:', newTask.getSummary());
    
    // Create notification
    const notification = taskManager.createNotification(
        'u1',
        'Task Assigned',
        `You have been assigned to task: ${newTask.title}`,
        'info',
        newTask.taskId
    );
    console.log('Notification created:', notification.getSummary());
    
    // Get analytics
    const analytics = taskManager.getAnalytics();
    console.log('Analytics:', analytics);
}

// Run the main function
main();

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        Task,
        Project,
        User,
        Notification,
        TaskManager
    };
}