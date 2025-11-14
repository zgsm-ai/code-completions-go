/**
 * 任务管理系统类
 * 用于管理项目、任务、团队成员和工作流
 */
class TaskManagementSystem {
    private users: Map<string, User>;               // 用户ID到用户信息的映射
    private projects: Map<string, Project>;          // 项目ID到项目信息的映射
    private tasks: Map<string, Task>;               // 任务ID到任务信息的映射
    private subtasks: Map<string, Subtask>;         // 子任务ID到子任务信息的映射
    private comments: Map<string, Comment>;          // 评论ID到评论信息的映射
    private attachments: Map<string, Attachment>;    // 附件ID到附件信息的映射
    private timeLogs: Map<string, TimeLog>;        // 时间日志ID到时间日志信息的映射
    private projectMembers: Map<string, ProjectMember>; // 项目成员ID到项目成员信息的映射
    private taskAssignments: Map<string, TaskAssignment>; // 任务分配ID到任务分配信息的映射
    private tags: Map<string, Tag>;                 // 标签ID到标签信息的映射
    private taskTags: Map<string, TaskTag>;        // 任务标签ID到任务标签信息的映射
    private notifications: Map<string, Notification>; // 通知ID到通知信息的映射
    
    /**
     * 构造函数
     */
    constructor() {
        this.users = new Map();
        this.projects = new Map();
        this.tasks = new Map();
        this.subtasks = new Map();
        this.comments = new Map();
        this.attachments = new Map();
        this.timeLogs = new Map();
        this.projectMembers = new Map();
        this.taskAssignments = new Map();
        this.tags = new Map();
        this.taskTags = new Map();
        this.notifications = new Map();
    }
    
    /**
     * 添加新用户
     * @param userId - 用户唯一标识符
     * @param username - 用户名
     * @param email - 电子邮件
     * @param password - 密码
     * @param firstName - 名字
     * @param lastName - 姓氏
     * @param avatar - 头像URL（可选）
     * @param role - 角色
     * @param hourlyRate - 小时费率（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addUser(userId: string, username: string, email: string, password: string, 
             firstName: string, lastName: string, avatar?: string, role?: Role, 
             hourlyRate?: number): boolean {
        if (this.users.has(userId)) {
            return false;
        }
        
        // 检查用户名是否已被使用
        for (const user of this.users.values()) {
            if (user.username === username) {
                return false;
            }
        }
        
        // 检查电子邮件是否已被使用
        for (const user of this.users.values()) {
            if (user.email === email) {
                return false;
            }
        }
        
        const user = new User(
            userId,
            username,
            email,
            password,
            firstName,
            lastName,
            avatar,
            role || Role.Member,
            hourlyRate
        );
        
        this.users.set(userId, user);
        
        return true;
    }
    
    /**
     * 更新用户信息
     * @param userId - 用户ID
     * @param username - 新的用户名（可选）
     * @param email - 新的电子邮件（可选）
     * @param password - 新的密码（可选）
     * @param firstName - 新的名字（可选）
     * @param lastName - 新的姓氏（可选）
     * @param avatar - 新的头像URL（可选）
     * @param role - 新的角色（可选）
     * @param hourlyRate - 新的小时费率（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateUser(userId: string, username?: string, email?: string, password?: string, 
               firstName?: string, lastName?: string, avatar?: string, role?: Role, 
               hourlyRate?: number): boolean {
        if (!this.users.has(userId)) {
            return false;
        }
        
        const user = this.users.get(userId)!;
        
        // 如果提供了用户名，检查是否已被其他用户使用
        if (username && username !== user.username) {
            for (const [id, u] of this.users) {
                if (id !== userId && u.username === username) {
                    return false;
                }
            }
            user.username = username;
        }
        
        // 如果提供了电子邮件，检查是否已被其他用户使用
        if (email && email !== user.email) {
            for (const [id, u] of this.users) {
                if (id !== userId && u.email === email) {
                    return false;
                }
            }
            user.email = email;
        }
        
        if (password !== undefined) {
            user.password = password;
        }
        
        if (firstName !== undefined) {
            user.firstName = firstName;
        }
        
        if (lastName !== undefined) {
            user.lastName = lastName;
        }
        
        if (avatar !== undefined) {
            user.avatar = avatar;
        }
        
        if (role !== undefined) {
            user.role = role;
        }
        
        if (hourlyRate !== undefined) {
            user.hourlyRate = hourlyRate;
        }
        
        user.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取用户信息
     * @param userId - 用户ID
     * @returns 用户对象，若不存在则返回null
     */
    getUser(userId: string): User | null {
        if (!this.users.has(userId)) {
            return null;
        }
        
        return this.users.get(userId)!;
    }
    
    /**
     * 删除用户
     * @param userId - 用户ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteUser(userId: string): boolean {
        if (!this.users.has(userId)) {
            return false;
        }
        
        // 检查用户是否是项目的所有者
        for (const project of this.projects.values()) {
            if (project.ownerId === userId) {
                return false;
            }
        }
        
        // 删除用户的所有项目成员关系
        for (const [membershipId, membership] of this.projectMembers) {
            if (membership.userId === userId) {
                this.projectMembers.delete(membershipId);
            }
        }
        
        // 删除用户的所有任务分配
        for (const [assignmentId, assignment] of this.taskAssignments) {
            if (assignment.assigneeId === userId || assignment.assignerId === userId) {
                this.taskAssignments.delete(assignmentId);
            }
        }
        
        // 删除用户的所有评论
        for (const [commentId, comment] of this.comments) {
            if (comment.userId === userId) {
                this.comments.delete(commentId);
            }
        }
        
        // 删除用户的所有时间日志
        for (const [timeLogId, timeLog] of this.timeLogs) {
            if (timeLog.userId === userId) {
                this.timeLogs.delete(timeLogId);
            }
        }
        
        // 删除用户的所有通知
        for (const [notificationId, notification] of this.notifications) {
            if (notification.userId === userId) {
                this.notifications.delete(notificationId);
            }
        }
        
        // 删除用户
        this.users.delete(userId);
        
        return true;
    }
    
    /**
     * 创建新项目
     * @param projectId - 项目唯一标识符
     * @param name - 项目名称
     * @param description - 项目描述
     * @param ownerId - 所有者ID
     * @param startDate - 开始日期
     * @param endDate - 结束日期（可选）
     * @param status - 项目状态
     * @param priority - 项目优先级
     * @param color - 项目颜色（可选）
     * @param budget - 项目预算（可选）
     * @returns 创建成功返回true，否则返回false
     */
    createProject(projectId: string, name: string, description: string, 
                  ownerId: string, startDate: Date, endDate?: Date, status?: ProjectStatus, 
                  priority?: Priority, color?: string, budget?: number): boolean {
        if (this.projects.has(projectId)) {
            return false;
        }
        
        if (!this.users.has(ownerId)) {
            return false;
        }
        
        if (!name || name.trim() === '') {
            return false;
        }
        
        if (startDate > (endDate || new Date(9999, 11, 31))) {
            return false;
        }
        
        const project = new Project(
            name,
            description,
            ownerId,
            startDate,
            endDate,
            status || ProjectStatus.Planning,
            priority || Priority.Medium,
            color,
            budget
        );
        
        this.projects.set(projectId, project);
        
        // 所有者自动成为项目成员
        const membershipId = `membership_${projectId}_${ownerId}`;
        this.addProjectMember(membershipId, projectId, ownerId, ProjectMemberRole.Owner);
        
        return true;
    }
    
    /**
     * 更新项目信息
     * @param projectId - 项目ID
     * @param name - 新的名称（可选）
     * @param description - 新的描述（可选）
     * @param endDate - 新的结束日期（可选）
     * @param status - 新的状态（可选）
     * @param priority - 新的优先级（可选）
     * @param color - 新的颜色（可选）
     * @param budget - 新的预算（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateProject(projectId: string, name?: string, description?: string, 
                  endDate?: Date, status?: ProjectStatus, priority?: Priority, 
                  color?: string, budget?: number): boolean {
        if (!this.projects.has(projectId)) {
            return false;
        }
        
        const project = this.projects.get(projectId)!;
        
        if (name !== undefined) {
            if (!name || name.trim() === '') {
                return false;
            }
            project.name = name;
        }
        
        if (description !== undefined) {
            project.description = description;
        }
        
        if (endDate !== undefined) {
            if (project.startDate > endDate) {
                return false;
            }
            project.endDate = endDate;
        }
        
        if (status !== undefined) {
            project.status = status;
        }
        
        if (priority !== undefined) {
            project.priority = priority;
        }
        
        if (color !== undefined) {
            project.color = color;
        }
        
        if (budget !== undefined) {
            project.budget = budget;
        }
        
        project.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取项目信息
     * @param projectId - 项目ID
     * @returns 项目对象，若不存在则返回null
     */
    getProject(projectId: string): Project | null {
        if (!this.projects.has(projectId)) {
            return null;
        }
        
        return this.projects.get(projectId)!;
    }
    
    /**
     * 删除项目
     * @param projectId - 项目ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteProject(projectId: string): boolean {
        if (!this.projects.has(projectId)) {
            return false;
        }
        
        const project = this.projects.get(projectId)!;
        
        // 检查项目是否已完成或已取消
        if (project.status === ProjectStatus.Completed || project.status === ProjectStatus.Cancelled) {
            // 删除项目的所有成员关系
            for (const [membershipId, membership] of this.projectMembers) {
                if (membership.projectId === projectId) {
                    this.projectMembers.delete(membershipId);
                }
            }
            
            // 删除项目的所有任务
            for (const [taskId, task] of this.tasks) {
                if (task.projectId === projectId) {
                    // 删除任务的所有子任务
                    for (const [subtaskId, subtask] of this.subtasks) {
                        if (subtask.taskId === taskId) {
                            this.subtasks.delete(subtaskId);
                        }
                    }
                    
                    // 删除任务的所有评论
                    for (const [commentId, comment] of this.comments) {
                        if (comment.taskId === taskId) {
                            this.comments.delete(commentId);
                        }
                    }
                    
                    // 删除任务的所有附件
                    for (const [attachmentId, attachment] of this.attachments) {
                        if (attachment.taskId === taskId) {
                            this.attachments.delete(attachmentId);
                        }
                    }
                    
                    // 删除任务的所有时间日志
                    for (const [timeLogId, timeLog] of this.timeLogs) {
                        if (timeLog.taskId === taskId) {
                            this.timeLogs.delete(timeLogId);
                        }
                    }
                    
                    // 删除任务的所有分配
                    for (const [assignmentId, assignment] of this.taskAssignments) {
                        if (assignment.taskId === taskId) {
                            this.taskAssignments.delete(assignmentId);
                        }
                    }
                    
                    // 删除任务的所有标签
                    for (const [taskTagId, taskTag] of this.taskTags) {
                        if (taskTag.taskId === taskId) {
                            this.taskTags.delete(taskTagId);
                        }
                    }
                    
                    this.tasks.delete(taskId);
                }
            }
            
            // 删除项目
            this.projects.delete(projectId);
            
            return true;
        }
        
        return false;
    }
    
    /**
     * 获取用户的项目
     * @param userId - 用户ID
     * @param status - 状态筛选（可选）
     * @returns 项目ID列表
     */
    getUserProjects(userId: string, status?: ProjectStatus): string[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const projectIds: string[] = [];
        
        // 获取用户是成员的所有项目
        for (const [projectId, project] of this.projects) {
            if (project.ownerId === userId) {
                if (status === undefined || project.status === status) {
                    projectIds.push(projectId);
                }
                continue;
            }
            
            let isMember = false;
            for (const membership of this.projectMembers.values()) {
                if (membership.projectId === projectId && membership.userId === userId) {
                    isMember = true;
                    break;
                }
            }
            
            if (isMember && (status === undefined || project.status === status)) {
                projectIds.push(projectId);
            }
        }
        
        return projectIds;
    }
    
    /**
     * 添加项目成员
     * @param membershipId - 成员关系唯一标识符
     * @param projectId - 项目ID
     * @param userId - 用户ID
     * @param role - 成员角色
     * @returns 添加成功返回true，否则返回false
     */
    addProjectMember(membershipId: string, projectId: string, userId: string, 
                      role: ProjectMemberRole): boolean {
        if (this.projectMembers.has(membershipId)) {
            return false;
        }
        
        if (!this.projects.has(projectId) || !this.users.has(userId)) {
            return false;
        }
        
        // 检查用户是否已是项目成员
        for (const membership of this.projectMembers.values()) {
            if (membership.projectId === projectId && membership.userId === userId) {
                return false;
            }
        }
        
        const membership = new ProjectMember(
            projectId,
            userId,
            role
        );
        
        this.projectMembers.set(membershipId, membership);
        
        // 创建通知
        this.createNotification(
            userId,
            NotificationType.ProjectMembership,
            this.projects.get(projectId)!.ownerId,
            projectId,
            `将你添加到了项目 ${this.projects.get(projectId)!.name}`
        );
        
        return true;
    }
    
    /**
     * 更新项目成员角色
     * @param projectId - 项目ID
     * @param userId - 用户ID
     * @param newRole - 新的角色
     * @param updaterId - 更新者ID
     * @returns 更新成功返回true，否则返回false
     */
    updateProjectMemberRole(projectId: string, userId: string, 
                           newRole: ProjectMemberRole, updaterId: string): boolean {
        if (!this.projects.has(projectId) || !this.users.has(userId) || !this.users.has(updaterId)) {
            return false;
        }
        
        const project = this.projects.get(projectId)!;
        
        // 只有项目所有者才能更改成员角色
        if (project.ownerId !== updaterId) {
            return false;
        }
        
        // 查找成员关系
        for (const [membershipId, membership] of this.projectMembers) {
            if (membership.projectId === projectId && membership.userId === userId) {
                membership.role = newRole;
                membership.updatedAt = new Date();
                
                // 创建通知
                this.createNotification(
                    userId,
                    NotificationType.ProjectRoleChange,
                    updaterId,
                    projectId,
                    `将你在项目 ${project.name} 中的角色更改为 ${newRole}`
                );
                
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 移除项目成员
     * @param projectId - 项目ID
     * @param userId - 要移除的用户ID
     * @param removerId - 移除者ID
     * @returns 移除成功返回true，否则返回false
     */
    removeProjectMember(projectId: string, userId: string, removerId: string): boolean {
        if (!this.projects.has(projectId) || !this.users.has(userId) || !this.users.has(removerId)) {
            return false;
        }
        
        const project = this.projects.get(projectId)!;
        
        // 只有项目所有者才能移除成员
        if (project.ownerId !== removerId) {
            return false;
        }
        
        // 项目所有者不能移除自己
        if (userId === project.ownerId) {
            return false;
        }
        
        // 查找并删除成员关系
        let membershipRemoved = false;
        for (const [membershipId, membership] of this.projectMembers) {
            if (membership.projectId === projectId && membership.userId === userId) {
                this.projectMembers.delete(membershipId);
                membershipRemoved = true;
                break;
            }
        }
        
        if (membershipRemoved) {
            // 创建通知
            this.createNotification(
                userId,
                NotificationType.ProjectRemoval,
                removerId,
                projectId,
                `将你从项目 ${project.name} 中移除`
            );
        }
        
        return membershipRemoved;
    }
    
    /**
     * 获取项目成员
     * @param projectId - 项目ID
     * @returns 成员ID列表
     */
    getProjectMembers(projectId: string): string[] {
        if (!this.projects.has(projectId)) {
            return [];
        }
        
        const members: string[] = [];
        
        // 添加项目所有者
        const project = this.projects.get(projectId)!;
        members.push(project.ownerId);
        
        // 添加项目成员
        for (const membership of this.projectMembers.values()) {
            if (membership.projectId === projectId && !members.includes(membership.userId)) {
                members.push(membership.userId);
            }
        }
        
        return members;
    }
    
    /**
     * 创建新任务
     * @param taskId - 任务唯一标识符
     * @param projectId - 项目ID
     * @param title - 任务标题
     * @param description - 任务描述
     * @param creatorId - 创建者ID
     * @param assigneeId - 被分配人ID（可选）
     * @param dueDate - 截止日期（可选）
     * @param status - 任务状态
     * @param priority - 任务优先级
     * @param estimatedHours - 预计工时（可选）
     * @returns 创建成功返回true，否则返回false
     */
    createTask(taskId: string, projectId: string, title: string, description: string, 
                creatorId: string, assigneeId?: string, dueDate?: Date, 
                status?: TaskStatus, priority?: Priority, estimatedHours?: number): boolean {
        if (this.tasks.has(taskId)) {
            return false;
        }
        
        if (!this.projects.has(projectId) || !this.users.has(creatorId)) {
            return false;
        }
        
        if (assigneeId && !this.users.has(assigneeId)) {
            return false;
        }
        
        if (!title || title.trim() === '') {
            return false;
        }
        
        if (dueDate && dueDate < new Date()) {
            return false;
        }
        
        const task = new Task(
            projectId,
            title,
            description,
            creatorId,
            assigneeId,
            dueDate,
            status || TaskStatus.ToDo,
            priority || Priority.Medium,
            estimatedHours
        );
        
        this.tasks.set(taskId, task);
        
        // 如果有被分配人，创建任务分配
        if (assigneeId) {
            const assignmentId = `assignment_${taskId}_${assigneeId}`;
            this.assignTask(assignmentId, taskId, creatorId, assigneeId);
        }
        
        return true;
    }
    
    /**
     * 更新任务信息
     * @param taskId - 任务ID
     * @param title - 新的标题（可选）
     * @param description - 新的描述（可选）
     * @param assigneeId - 新的被分配人ID（可选）
     * @param dueDate - 新的截止日期（可选）
     * @param status - 新的状态（可选）
     * @param priority - 新的优先级（可选）
     * @param estimatedHours - 新的预计工时（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateTask(taskId: string, title?: string, description?: string, 
               assigneeId?: string, dueDate?: Date, status?: TaskStatus, 
               priority?: Priority, estimatedHours?: number): boolean {
        if (!this.tasks.has(taskId)) {
            return false;
        }
        
        const task = this.tasks.get(taskId)!;
        
        if (assigneeId && !this.users.has(assigneeId)) {
            return false;
        }
        
        if (title !== undefined) {
            if (!title || title.trim() === '') {
                return false;
            }
            task.title = title;
        }
        
        if (description !== undefined) {
            task.description = description;
        }
        
        if (assigneeId !== undefined) {
            // 如果被分配人有变化，创建新的任务分配
            if (task.assigneeId !== assigneeId) {
                task.assigneeId = assigneeId;
                
                // 创建新的任务分配
                if (assigneeId) {
                    const assignmentId = `assignment_${taskId}_${assigneeId}`;
                    this.assignTask(assignmentId, taskId, task.creatorId, assigneeId);
                }
            }
        }
        
        if (dueDate !== undefined) {
            if (dueDate < new Date()) {
                return false;
            }
            task.dueDate = dueDate;
        }
        
        if (status !== undefined) {
            task.status = status;
        }
        
        if (priority !== undefined) {
            task.priority = priority;
        }
        
        if (estimatedHours !== undefined) {
            task.estimatedHours = estimatedHours;
        }
        
        task.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取任务信息
     * @param taskId - 任务ID
     * @returns 任务对象，若不存在则返回null
     */
    getTask(taskId: string): Task | null {
        if (!this.tasks.has(taskId)) {
            return null;
        }
        
        return this.tasks.get(taskId)!;
    }
    
    /**
     * 删除任务
     * @param taskId - 任务ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteTask(taskId: string): boolean {
        if (!this.tasks.has(taskId)) {
            return false;
        }
        
        const task = this.tasks.get(taskId)!;
        
        // 检查任务是否已完成或已取消
        if (task.status === TaskStatus.Completed || task.status === TaskStatus.Cancelled) {
            // 删除任务的所有子任务
            for (const [subtaskId, subtask] of this.subtasks) {
                if (subtask.taskId === taskId) {
                    this.subtasks.delete(subtaskId);
                }
            }
            
            // 删除任务的所有评论
            for (const [commentId, comment] of this.comments) {
                if (comment.taskId === taskId) {
                    this.comments.delete(commentId);
                }
            }
            
            // 删除任务的所有附件
            for (const [attachmentId, attachment] of this.attachments) {
                if (attachment.taskId === taskId) {
                    this.attachments.delete(attachmentId);
                }
            }
            
            // 删除任务的所有时间日志
            for (const [timeLogId, timeLog] of this.timeLogs) {
                if (timeLog.taskId === taskId) {
                    this.timeLogs.delete(timeLogId);
                }
            }
            
            // 删除任务的所有分配
            for (const [assignmentId, assignment] of this.taskAssignments) {
                if (assignment.taskId === taskId) {
                    this.taskAssignments.delete(assignmentId);
                }
            }
            
            // 删除任务的所有标签
            for (const [taskTagId, taskTag] of this.taskTags) {
                if (taskTag.taskId === taskId) {
                    this.taskTags.delete(taskTagId);
                }
            }
            
            // 删除任务
            this.tasks.delete(taskId);
            
            return true;
        }
        
        return false;
    }
    
    /**
     * 获取项目的任务
     * @param projectId - 项目ID
     * @param status - 状态筛选（可选）
     * @param assigneeId - 被分配人筛选（可选）
     * @param priority - 优先级筛选（可选）
     * @param dueBefore - 截止日期筛选（可选）
     * @returns 任务ID列表
     */
    getProjectTasks(projectId: string, status?: TaskStatus, assigneeId?: string, 
                    priority?: Priority, dueBefore?: Date): string[] {
        if (!this.projects.has(projectId)) {
            return [];
        }
        
        const taskIds: string[] = [];
        
        for (const [taskId, task] of this.tasks) {
            if (task.projectId === projectId) {
                if (status !== undefined && task.status !== status) {
                    continue;
                }
                
                if (assigneeId !== undefined && task.assigneeId !== assigneeId) {
                    continue;
                }
                
                if (priority !== undefined && task.priority !== priority) {
                    continue;
                }
                
                if (dueBefore !== undefined && task.dueDate && task.dueDate > dueBefore) {
                    continue;
                }
                
                taskIds.push(taskId);
            }
        }
        
        return taskIds;
    }
    
    /**
     * 获取用户的任务
     * @param userId - 用户ID
     * @param status - 状态筛选（可选）
     * @param priority - 优先级筛选（可选）
     * @param dueBefore - 截止日期筛选（可选）
     * @returns 任务ID列表
     */
    getUserTasks(userId: string, status?: TaskStatus, priority?: Priority, 
                  dueBefore?: Date): string[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const taskIds: string[] = [];
        
        for (const [taskId, task] of this.tasks) {
            if (task.assigneeId === userId) {
                if (status !== undefined && task.status !== status) {
                    continue;
                }
                
                if (priority !== undefined && task.priority !== priority) {
                    continue;
                }
                
                if (dueBefore !== undefined && task.dueDate && task.dueDate > dueBefore) {
                    continue;
                }
                
                taskIds.push(taskId);
            }
        }
        
        return taskIds;
    }
    
    /**
     * 分配任务
     * @param assignmentId - 分配唯一标识符
     * @param taskId - 任务ID
     * @param assignerId - 分配者ID
     * @param assigneeId - 被分配人ID
     * @returns 分配成功返回true，否则返回false
     */
    assignTask(assignmentId: string, taskId: string, assignerId: string, 
                assigneeId: string): boolean {
        if (this.taskAssignments.has(assignmentId)) {
            return false;
        }
        
        if (!this.tasks.has(taskId) || !this.users.has(assignerId) || !this.users.has(assigneeId)) {
            return false;
        }
        
        // 检查是否已有分配
        for (const assignment of this.taskAssignments.values()) {
            if (assignment.taskId === taskId && assignment.assigneeId === assigneeId) {
                return false;
            }
        }
        
        const assignment = new TaskAssignment(
            taskId,
            assignerId,
            assigneeId
        );
        
        this.taskAssignments.set(assignmentId, assignment);
        
        // 更新任务的被分配人
        const task = this.tasks.get(taskId)!;
        if (task.assigneeId !== assigneeId) {
            task.assigneeId = assigneeId;
            task.updatedAt = new Date();
        }
        
        // 创建通知
        this.createNotification(
            assigneeId,
            NotificationType.TaskAssignment,
            assignerId,
            taskId,
            `将你分配到了任务 ${task.title}`
        );
        
        return true;
    }
    
    /**
     * 添加子任务
     * @param subtaskId - 子任务唯一标识符
     * @param taskId - 任务ID
     * @param title - 子任务标题
     * @param description - 子任务描述
     * @param creatorId - 创建者ID
     * @param assigneeId - 被分配人ID（可选）
     * @param dueDate - 截止日期（可选）
     * @param status - 子任务状态
     * @param priority - 子任务优先级
     * @param estimatedHours - 预计工时（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addSubtask(subtaskId: string, taskId: string, title: string, description: string, 
                creatorId: string, assigneeId?: string, dueDate?: Date, 
                status?: TaskStatus, priority?: Priority, estimatedHours?: number): boolean {
        if (this.subtasks.has(subtaskId)) {
            return false;
        }
        
        if (!this.tasks.has(taskId) || !this.users.has(creatorId)) {
            return false;
        }
        
        if (assigneeId && !this.users.has(assigneeId)) {
            return false;
        }
        
        if (!title || title.trim() === '') {
            return false;
        }
        
        if (dueDate && dueDate < new Date()) {
            return false;
        }
        
        const subtask = new Subtask(
            taskId,
            title,
            description,
            creatorId,
            assigneeId,
            dueDate,
            status || TaskStatus.ToDo,
            priority || Priority.Medium,
            estimatedHours
        );
        
        this.subtasks.set(subtaskId, subtask);
        
        return true;
    }
    
    /**
     * 更新子任务信息
     * @param subtaskId - 子任务ID
     * @param title - 新的标题（可选）
     * @param description - 新的描述（可选）
     * @param assigneeId - 新的被分配人ID（可选）
     * @param dueDate - 新的截止日期（可选）
     * @param status - 新的状态（可选）
     * @param priority - 新的优先级（可选）
     * @param estimatedHours - 新的预计工时（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateSubtask(subtaskId: string, title?: string, description?: string, 
                   assigneeId?: string, dueDate?: Date, status?: TaskStatus, 
                   priority?: Priority, estimatedHours?: number): boolean {
        if (!this.subtasks.has(subtaskId)) {
            return false;
        }
        
        const subtask = this.subtasks.get(subtaskId)!;
        
        if (assigneeId && !this.users.has(assigneeId)) {
            return false;
        }
        
        if (title !== undefined) {
            if (!title || title.trim() === '') {
                return false;
            }
            subtask.title = title;
        }
        
        if (description !== undefined) {
            subtask.description = description;
        }
        
        if (assigneeId !== undefined) {
            subtask.assigneeId = assigneeId;
        }
        
        if (dueDate !== undefined) {
            if (dueDate < new Date()) {
                return false;
            }
            subtask.dueDate = dueDate;
        }
        
        if (status !== undefined) {
            subtask.status = status;
        }
        
        if (priority !== undefined) {
            subtask.priority = priority;
        }
        
        if (estimatedHours !== undefined) {
            subtask.estimatedHours = estimatedHours;
        }
        
        subtask.updatedAt = new Date();
        
        // 检查是否所有子任务都已完成，如果是，则更新父任务状态
        if (status === TaskStatus.Completed) {
            this.checkAndUpdateParentTaskStatus(subtask.taskId);
        }
        
        return true;
    }
    
    /**
     * 获取子任务信息
     * @param subtaskId - 子任务ID
     * @returns 子任务对象，若不存在则返回null
     */
    getSubtask(subtaskId: string): Subtask | null {
        if (!this.subtasks.has(subtaskId)) {
            return null;
        }
        
        return this.subtasks.get(subtaskId)!;
    }
    
    /**
     * 删除子任务
     * @param subtaskId - 子任务ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteSubtask(subtaskId: string): boolean {
        if (!this.subtasks.has(subtaskId)) {
            return false;
        }
        
        const subtask = this.subtasks.get(subtaskId)!;
        
        // 检查子任务是否已完成或已取消
        if (subtask.status === TaskStatus.Completed || subtask.status === TaskStatus.Cancelled) {
            // 删除子任务
            this.subtasks.delete(subtaskId);
            
            // 检查是否所有子任务都已完成，如果是，则更新父任务状态
            this.checkAndUpdateParentTaskStatus(subtask.taskId);
            
            return true;
        }
        
        return false;
    }
    
    /**
     * 获取任务的子任务
     * @param taskId - 任务ID
     * @param status - 状态筛选（可选）
     * @param assigneeId - 被分配人筛选（可选）
     * @param priority - 优先级筛选（可选）
     * @returns 子任务ID列表
     */
    getTaskSubtasks(taskId: string, status?: TaskStatus, assigneeId?: string, 
                     priority?: Priority): string[] {
        if (!this.tasks.has(taskId)) {
            return [];
        }
        
        const subtaskIds: string[] = [];
        
        for (const [subtaskId, subtask] of this.subtasks) {
            if (subtask.taskId === taskId) {
                if (status !== undefined && subtask.status !== status) {
                    continue;
                }
                
                if (assigneeId !== undefined && subtask.assigneeId !== assigneeId) {
                    continue;
                }
                
                if (priority !== undefined && subtask.priority !== priority) {
                    continue;
                }
                
                subtaskIds.push(subtaskId);
            }
        }
        
        return subtaskIds;
    }
    
    /**
     * 检查并更新父任务状态
     * @param taskId - 任务ID
     */
    private checkAndUpdateParentTaskStatus(taskId: string): void {
        if (!this.tasks.has(taskId)) {
            return;
        }
        
        const task = this.tasks.get(taskId)!;
        const subtasks = this.getTaskSubtasks(taskId);
        
        // 如果没有子任务，不更新状态
        if (subtasks.length === 0) {
            return;
        }
        
        // 检查所有子任务是否都已完成
        let allCompleted = true;
        for (const subtaskId of subtasks) {
            const subtask = this.subtasks.get(subtaskId)!;
            if (subtask.status !== TaskStatus.Completed) {
                allCompleted = false;
                break;
            }
        }
        
        // 如果所有子任务都已完成，将父任务状态更新为已完成
        if (allCompleted && task.status !== TaskStatus.Completed) {
            task.status = TaskStatus.Completed;
            task.updatedAt = new Date();
            
            // 创建通知
            if (task.assigneeId) {
                this.createNotification(
                    task.assigneeId,
                    NotificationType.TaskCompletion,
                    task.creatorId,
                    taskId,
                    `任务 ${task.title} 的所有子任务已完成`
                );
            }
        }
    }
    
    /**
     * 添加评论
     * @param commentId - 评论唯一标识符
     * @param taskId - 任务ID
     * @param userId - 用户ID
     * @param content - 评论内容
     * @param parentCommentId - 父评论ID（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addComment(commentId: string, taskId: string, userId: string, 
                content: string, parentCommentId?: string): boolean {
        if (this.comments.has(commentId)) {
            return false;
        }
        
        if (!this.tasks.has(taskId) || !this.users.has(userId)) {
            return false;
        }
        
        if (!content || content.trim() === '') {
            return false;
        }
        
        // 如果有父评论，检查父评论是否存在
        if (parentCommentId && !this.comments.has(parentCommentId)) {
            return false;
        }
        
        const comment = new Comment(
            taskId,
            userId,
            content,
            parentCommentId
        );
        
        this.comments.set(commentId, comment);
        
        // 如果评论的是别人的任务，创建通知
        const task = this.tasks.get(taskId)!;
        if (task.assigneeId && task.assigneeId !== userId) {
            this.createNotification(
                task.assigneeId,
                NotificationType.Comment,
                userId,
                taskId,
                `评论了任务 ${task.title}`
            );
        }
        
        // 如果是回复评论，创建通知
        if (parentCommentId) {
            const parentComment = this.comments.get(parentCommentId)!;
            if (parentComment.userId !== userId) {
                this.createNotification(
                    parentComment.userId,
                    NotificationType.CommentReply,
                    userId,
                    taskId,
                    `回复了你的评论`
                );
            }
        }
        
        return true;
    }
    
    /**
     * 更新评论
     * @param commentId - 评论ID
     * @param content - 新的内容
     * @returns 更新成功返回true，否则返回false
     */
    updateComment(commentId: string, content: string): boolean {
        if (!this.comments.has(commentId)) {
            return false;
        }
        
        if (!content || content.trim() === '') {
            return false;
        }
        
        const comment = this.comments.get(commentId)!;
        comment.content = content;
        comment.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取评论
     * @param commentId - 评论ID
     * @returns 评论对象，若不存在则返回null
     */
    getComment(commentId: string): Comment | null {
        if (!this.comments.has(commentId)) {
            return null;
        }
        
        return this.comments.get(commentId)!;
    }
    
    /**
     * 删除评论
     * @param commentId - 评论ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteComment(commentId: string): boolean {
        if (!this.comments.has(commentId)) {
            return false;
        }
        
        // 删除评论的所有回复
        for (const [id, comment] of this.comments) {
            if (comment.parentCommentId === commentId) {
                this.comments.delete(id);
            }
        }
        
        // 删除评论
        this.comments.delete(commentId);
        
        return true;
    }
    
    /**
     * 获取任务的评论
     * @param taskId - 任务ID
     * @returns 评论ID列表
     */
    getTaskComments(taskId: string): string[] {
        if (!this.tasks.has(taskId)) {
            return [];
        }
        
        const commentIds: string[] = [];
        
        for (const [commentId, comment] of this.comments) {
            if (comment.taskId === taskId && !comment.parentCommentId) {
                commentIds.push(commentId);
            }
        }
        
        return commentIds;
    }
    
    /**
     * 获取评论的回复
     * @param commentId - 评论ID
     * @returns 回复评论ID列表
     */
    getCommentReplies(commentId: string): string[] {
        if (!this.comments.has(commentId)) {
            return [];
        }
        
        const replyIds: string[] = [];
        
        for (const [id, comment] of this.comments) {
            if (comment.parentCommentId === commentId) {
                replyIds.push(id);
            }
        }
        
        return replyIds;
    }
    
    /**
     * 添加附件
     * @param attachmentId - 附件唯一标识符
     * @param taskId - 任务ID
     * @param userId - 用户ID
     * @param fileName - 文件名
     * @param fileSize - 文件大小
     * @param fileType - 文件类型
     * @param fileUrl - 文件URL
     * @param description - 文件描述（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addAttachment(attachmentId: string, taskId: string, userId: string, 
                  fileName: string, fileSize: number, fileType: string, 
                  fileUrl: string, description?: string): boolean {
        if (this.attachments.has(attachmentId)) {
            return false;
        }
        
        if (!this.tasks.has(taskId) || !this.users.has(userId)) {
            return false;
        }
        
        if (!fileName || fileName.trim() === '') {
            return false;
        }
        
        if (fileSize <= 0) {
            return false;
        }
        
        if (!fileType || fileType.trim() === '') {
            return false;
        }
        
        if (!fileUrl || fileUrl.trim() === '') {
            return false;
        }
        
        const attachment = new Attachment(
            taskId,
            userId,
            fileName,
            fileSize,
            fileType,
            fileUrl,
            description
        );
        
        this.attachments.set(attachmentId, attachment);
        
        return true;
    }
    
    /**
     * 获取附件
     * @param attachmentId - 附件ID
     * @returns 附件对象，若不存在则返回null
     */
    getAttachment(attachmentId: string): Attachment | null {
        if (!this.attachments.has(attachmentId)) {
            return null;
        }
        
        return this.attachments.get(attachmentId)!;
    }
    
    /**
     * 删除附件
     * @param attachmentId - 附件ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteAttachment(attachmentId: string): boolean {
        if (!this.attachments.has(attachmentId)) {
            return false;
        }
        
        // 删除附件
        this.attachments.delete(attachmentId);
        
        return true;
    }
    
    /**
     * 获取任务的附件
     * @param taskId - 任务ID
     * @returns 附件ID列表
     */
    getTaskAttachments(taskId: string): string[] {
        if (!this.tasks.has(taskId)) {
            return [];
        }
        
        const attachmentIds: string[] = [];
        
        for (const [attachmentId, attachment] of this.attachments) {
            if (attachment.taskId === taskId) {
                attachmentIds.push(attachmentId);
            }
        }
        
        return attachmentIds;
    }
    
    /**
     * 添加时间日志
     * @param timeLogId - 时间日志唯一标识符
     * @param taskId - 任务ID
     * @param userId - 用户ID
     * @param date - 日期
     * @param hours - 工时
     * @param description - 描述（可选）
     * @param billable - 是否可计费
     * @returns 添加成功返回true，否则返回false
     */
    addTimeLog(timeLogId: string, taskId: string, userId: string, 
                date: Date, hours: number, description?: string, billable?: boolean): boolean {
        if (this.timeLogs.has(timeLogId)) {
            return false;
        }
        
        if (!this.tasks.has(taskId) || !this.users.has(userId)) {
            return false;
        }
        
        if (hours <= 0) {
            return false;
        }
        
        const timeLog = new TimeLog(
            taskId,
            userId,
            date,
            hours,
            description,
            billable || false
        );
        
        this.timeLogs.set(timeLogId, timeLog);
        
        return true;
    }
    
    /**
     * 更新时间日志
     * @param timeLogId - 时间日志ID
     * @param date - 新的日期（可选）
     * @param hours - 新的工时（可选）
     * @param description - 新的描述（可选）
     * @param billable - 新的可计费标志（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateTimeLog(timeLogId: string, date?: Date, hours?: number, 
                   description?: string, billable?: boolean): boolean {
        if (!this.timeLogs.has(timeLogId)) {
            return false;
        }
        
        const timeLog = this.timeLogs.get(timeLogId)!;
        
        if (hours !== undefined && hours <= 0) {
            return false;
        }
        
        if (date !== undefined) {
            timeLog.date = date;
        }
        
        if (hours !== undefined) {
            timeLog.hours = hours;
        }
        
        if (description !== undefined) {
            timeLog.description = description;
        }
        
        if (billable !== undefined) {
            timeLog.billable = billable;
        }
        
        timeLog.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取时间日志
     * @param timeLogId - 时间日志ID
     * @returns 时间日志对象，若不存在则返回null
     */
    getTimeLog(timeLogId: string): TimeLog | null {
        if (!this.timeLogs.has(timeLogId)) {
            return null;
        }
        
        return this.timeLogs.get(timeLogId)!;
    }
    
    /**
     * 删除时间日志
     * @param timeLogId - 时间日志ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteTimeLog(timeLogId: string): boolean {
        if (!this.timeLogs.has(timeLogId)) {
            return false;
        }
        
        // 删除时间日志
        this.timeLogs.delete(timeLogId);
        
        return true;
    }
    
    /**
     * 获取任务的时间日志
     * @param taskId - 任务ID
     * @param userId - 用户ID筛选（可选）
     * @param startDate - 开始日期筛选（可选）
     * @param endDate - 结束日期筛选（可选）
     * @returns 时间日志ID列表
     */
    getTaskTimeLogs(taskId: string, userId?: string, 
                     startDate?: Date, endDate?: Date): string[] {
        if (!this.tasks.has(taskId)) {
            return [];
        }
        
        const timeLogIds: string[] = [];
        
        for (const [timeLogId, timeLog] of this.timeLogs) {
            if (timeLog.taskId === taskId) {
                if (userId !== undefined && timeLog.userId !== userId) {
                    continue;
                }
                
                if (startDate !== undefined && timeLog.date < startDate) {
                    continue;
                }
                
                if (endDate !== undefined && timeLog.date > endDate) {
                    continue;
                }
                
                timeLogIds.push(timeLogId);
            }
        }
        
        return timeLogIds;
    }
    
    /**
     * 计算任务的总工时
     * @param taskId - 任务ID
     * @param userId - 用户ID筛选（可选）
     * @param billableOnly - 是否只计算可计费工时
     * @returns 总工时
     */
    calculateTaskTotalHours(taskId: string, userId?: string, billableOnly?: boolean): number {
        const timeLogIds = this.getTaskTimeLogs(taskId, userId);
        
        let totalHours = 0;
        
        for (const timeLogId of timeLogIds) {
            const timeLog = this.timeLogs.get(timeLogId)!;
            
            if (billableOnly && !timeLog.billable) {
                continue;
            }
            
            totalHours += timeLog.hours;
        }
        
        return totalHours;
    }
    
    /**
     * 创建新标签
     * @param tagId - 标签唯一标识符
     * @param name - 标签名称
     * @param color - 标签颜色（可选）
     * @param description - 标签描述（可选）
     * @returns 创建成功返回true，否则返回false
     */
    createTag(tagId: string, name: string, color?: string, description?: string): boolean {
        if (this.tags.has(tagId)) {
            return false;
        }
        
        if (!name || name.trim() === '') {
            return false;
        }
        
        // 检查标签名称是否已被使用
        for (const tag of this.tags.values()) {
            if (tag.name === name) {
                return false;
            }
        }
        
        const tag = new Tag(
            name,
            color,
            description
        );
        
        this.tags.set(tagId, tag);
        
        return true;
    }
    
    /**
     * 更新标签信息
     * @param tagId - 标签ID
     * @param name - 新的名称（可选）
     * @param color - 新的颜色（可选）
     * @param description - 新的描述（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateTag(tagId: string, name?: string, color?: string, 
               description?: string): boolean {
        if (!this.tags.has(tagId)) {
            return false;
        }
        
        const tag = this.tags.get(tagId)!;
        
        if (name !== undefined) {
            if (!name || name.trim() === '') {
                return false;
            }
            
            // 检查标签名称是否已被其他标签使用
            for (const [id, t] of this.tags) {
                if (id !== tagId && t.name === name) {
                    return false;
                }
            }
            
            tag.name = name;
        }
        
        if (color !== undefined) {
            tag.color = color;
        }
        
        if (description !== undefined) {
            tag.description = description;
        }
        
        tag.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取标签
     * @param tagId - 标签ID
     * @returns 标签对象，若不存在则返回null
     */
    getTag(tagId: string): Tag | null {
        if (!this.tags.has(tagId)) {
            return null;
        }
        
        return this.tags.get(tagId)!;
    }
    
    /**
     * 删除标签
     * @param tagId - 标签ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteTag(tagId: string): boolean {
        if (!this.tags.has(tagId)) {
            return false;
        }
        
        // 删除所有与该标签相关的任务标签关系
        for (const [taskTagId, taskTag] of this.taskTags) {
            if (taskTag.tagId === tagId) {
                this.taskTags.delete(taskTagId);
            }
        }
        
        // 删除标签
        this.tags.delete(tagId);
        
        return true;
    }
    
    /**
     * 为任务添加标签
     * @param taskTagId - 任务标签唯一标识符
     * @param taskId - 任务ID
     * @param tagId - 标签ID
     * @returns 添加成功返回true，否则返回false
     */
    addTagToTask(taskTagId: string, taskId: string, tagId: string): boolean {
        if (this.taskTags.has(taskTagId)) {
            return false;
        }
        
        if (!this.tasks.has(taskId) || !this.tags.has(tagId)) {
            return false;
        }
        
        // 检查任务是否已有该标签
        for (const taskTag of this.taskTags.values()) {
            if (taskTag.taskId === taskId && taskTag.tagId === tagId) {
                return false;
            }
        }
        
        const taskTag = new TaskTag(
            taskId,
            tagId
        );
        
        this.taskTags.set(taskTagId, taskTag);
        
        return true;
    }
    
    /**
     * 从任务移除标签
     * @param taskId - 任务ID
     * @param tagId - 标签ID
     * @returns 移除成功返回true，否则返回false
     */
    removeTagFromTask(taskId: string, tagId: string): boolean {
        let tagRemoved = false;
        
        for (const [taskTagId, taskTag] of this.taskTags) {
            if (taskTag.taskId === taskId && taskTag.tagId === tagId) {
                this.taskTags.delete(taskTagId);
                tagRemoved = true;
                break;
            }
        }
        
        return tagRemoved;
    }
    
    /**
     * 获取任务的标签
     * @param taskId - 任务ID
     * @returns 标签ID列表
     */
    getTaskTags(taskId: string): string[] {
        if (!this.tasks.has(taskId)) {
            return [];
        }
        
        const tagIds: string[] = [];
        
        for (const [taskTagId, taskTag] of this.taskTags) {
            if (taskTag.taskId === taskId) {
                tagIds.push(taskTag.tagId);
            }
        }
        
        return tagIds;
    }
    
    /**
     * 创建通知
     * @param userId - 用户ID
     * @param type - 通知类型
     * @param actorId - 触发通知的用户ID
     * @param targetId - 目标ID
     * @param message - 通知消息
     * @returns 通知ID
     */
    private createNotification(userId: string, type: NotificationType, 
                            actorId: string, targetId: string, message: string): string {
        const notificationId = this.generateId("notification");
        
        const notification = new Notification(
            userId,
            type,
            actorId,
            targetId,
            message
        );
        
        this.notifications.set(notificationId, notification);
        
        return notificationId;
    }
    
    /**
     * 获取用户的通知
     * @param userId - 用户ID
     * @param unreadOnly - 是否只获取未读通知
     * @param limit - 返回通知数量限制（可选）
     * @returns 通知ID列表
     */
    getUserNotifications(userId: string, unreadOnly: boolean, limit?: number): string[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const notificationIds: string[] = [];
        
        for (const [notificationId, notification] of this.notifications) {
            if (notification.userId === userId) {
                if (unreadOnly && notification.isRead) {
                    continue;
                }
                
                notificationIds.push(notificationId);
            }
        }
        
        // 按创建时间降序排序
        notificationIds.sort((a, b) => {
            const notificationA = this.notifications.get(a)!;
            const notificationB = this.notifications.get(b)!;
            return notificationB.createdAt.getTime() - notificationA.createdAt.getTime();
        });
        
        if (limit && notificationIds.length > limit) {
            return notificationIds.slice(0, limit);
        }
        
        return notificationIds;
    }
    
    /**
     * 标记通知为已读
     * @param notificationId - 通知ID
     * @param userId - 用户ID
     * @returns 标记成功返回true，否则返回false
     */
    markNotificationAsRead(notificationId: string, userId: string): boolean {
        if (!this.notifications.has(notificationId)) {
            return false;
        }
        
        const notification = this.notifications.get(notificationId)!;
        
        if (notification.userId !== userId) {
            return false;
        }
        
        notification.isRead = true;
        notification.readAt = new Date();
        
        return true;
    }
    
    /**
     * 标记所有通知为已读
     * @param userId - 用户ID
     * @returns 标记成功返回true，否则返回false
     */
    markAllNotificationsAsRead(userId: string): boolean {
        if (!this.users.has(userId)) {
            return false;
        }
        
        let notificationsUpdated = false;
        
        for (const notification of this.notifications.values()) {
            if (notification.userId === userId && !notification.isRead) {
                notification.isRead = true;
                notification.readAt = new Date();
                notificationsUpdated = true;
            }
        }
        
        return notificationsUpdated;
    }
    
    /**
     * 获取未读通知数
     * @param userId - 用户ID
     * @returns 未读通知数
     */
    getUnreadNotificationCount(userId: string): number {
        if (!this.users.has(userId)) {
            return 0;
        }
        
        let count = 0;
        
        for (const notification of this.notifications.values()) {
            if (notification.userId === userId && !notification.isRead) {
                count++;
            }
        }
        
        return count;
    }
    
    /**
     * 生成唯一ID
     * @param prefix - ID前缀
     * @returns 唯一ID字符串
     */
    private generateId(prefix: string): string {
        return `${prefix}_${Date.now()}_${Math.random().toString(36).substring(2, 10)}`;
    }
}

/**
 * 用户类
 */
class User {
    userId: string;
    username: string;
    email: string;
    password: string;
    firstName: string;
    lastName: string;
    avatar?: string;
    role: Role;
    hourlyRate?: number;
    createdAt: Date;
    updatedAt: Date;
    lastLoginAt?: Date;
    isActive: boolean;
    
    constructor(userId: string, username: string, email: string, password: string, 
                firstName: string, lastName: string, avatar?: string, role?: Role, 
                hourlyRate?: number) {
        this.userId = userId;
        this.username = username;
        this.email = email;
        this.password = password;
        this.firstName = firstName;
        this.lastName = lastName;
        this.avatar = avatar;
        this.role = role || Role.Member;
        this.hourlyRate = hourlyRate;
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.isActive = true;
    }
    
    /**
     * 获取全名
     */
    get fullName(): string {
        return `${this.firstName} ${this.lastName}`;
    }
}

/**
 * 角色枚举
 */
enum Role {
    Admin = "admin",
    Manager = "manager",
    Member = "member"
}

/**
 * 项目类
 */
class Project {
    name: string;
    description: string;
    ownerId: string;
    startDate: Date;
    endDate?: Date;
    status: ProjectStatus;
    priority: Priority;
    color?: string;
    budget?: number;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(name: string, description: string, ownerId: string, 
                startDate: Date, endDate?: Date, status?: ProjectStatus, 
                priority?: Priority, color?: string, budget?: number) {
        this.name = name;
        this.description = description;
        this.ownerId = ownerId;
        this.startDate = startDate;
        this.endDate = endDate;
        this.status = status || ProjectStatus.Planning;
        this.priority = priority || Priority.Medium;
        this.color = color;
        this.budget = budget;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 项目状态枚举
 */
enum ProjectStatus {
    Planning = "planning",
    Active = "active",
    OnHold = "on_hold",
    Completed = "completed",
    Cancelled = "cancelled"
}

/**
 * 优先级枚举
 */
enum Priority {
    Low = "low",
    Medium = "medium",
    High = "high",
    Urgent = "urgent"
}

/**
 * 项目成员类
 */
class ProjectMember {
    projectId: string;
    userId: string;
    role: ProjectMemberRole;
    joinedAt: Date;
    updatedAt: Date;
    
    constructor(projectId: string, userId: string, role: ProjectMemberRole) {
        this.projectId = projectId;
        this.userId = userId;
        this.role = role;
        this.joinedAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 项目成员角色枚举
 */
enum ProjectMemberRole {
    Owner = "owner",
    Admin = "admin",
    Member = "member"
}

/**
 * 任务类
 */
class Task {
    projectId: string;
    title: string;
    description: string;
    creatorId: string;
    assigneeId?: string;
    dueDate?: Date;
    status: TaskStatus;
    priority: Priority;
    estimatedHours?: number;
    createdAt: Date;
    updatedAt: Date;
    completedAt?: Date;
    
    constructor(projectId: string, title: string, description: string, 
                creatorId: string, assigneeId?: string, dueDate?: Date, 
                status?: TaskStatus, priority?: Priority, estimatedHours?: number) {
        this.projectId = projectId;
        this.title = title;
        this.description = description;
        this.creatorId = creatorId;
        this.assigneeId = assigneeId;
        this.dueDate = dueDate;
        this.status = status || TaskStatus.ToDo;
        this.priority = priority || Priority.Medium;
        this.estimatedHours = estimatedHours;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 任务状态枚举
 */
enum TaskStatus {
    ToDo = "to_do",
    InProgress = "in_progress",
    Review = "review",
    Completed = "completed",
    Cancelled = "cancelled"
}

/**
 * 子任务类
 */
class Subtask {
    taskId: string;
    title: string;
    description: string;
    creatorId: string;
    assigneeId?: string;
    dueDate?: Date;
    status: TaskStatus;
    priority: Priority;
    estimatedHours?: number;
    createdAt: Date;
    updatedAt: Date;
    completedAt?: Date;
    
    constructor(taskId: string, title: string, description: string, 
                creatorId: string, assigneeId?: string, dueDate?: Date, 
                status?: TaskStatus, priority?: Priority, estimatedHours?: number) {
        this.taskId = taskId;
        this.title = title;
        this.description = description;
        this.creatorId = creatorId;
        this.assigneeId = assigneeId;
        this.dueDate = dueDate;
        this.status = status || TaskStatus.ToDo;
        this.priority = priority || Priority.Medium;
        this.estimatedHours = estimatedHours;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 评论类
 */
class Comment {
    taskId: string;
    userId: string;
    content: string;
    parentCommentId?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(taskId: string, userId: string, content: string, parentCommentId?: string) {
        this.taskId = taskId;
        this.userId = userId;
        this.content = content;
        this.parentCommentId = parentCommentId;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 附件类
 */
class Attachment {
    taskId: string;
    userId: string;
    fileName: string;
    fileSize: number;
    fileType: string;
    fileUrl: string;
    description?: string;
    uploadedAt: Date;
    
    constructor(taskId: string, userId: string, fileName: string, 
                fileSize: number, fileType: string, fileUrl: string, description?: string) {
        this.taskId = taskId;
        this.userId = userId;
        this.fileName = fileName;
        this.fileSize = fileSize;
        this.fileType = fileType;
        this.fileUrl = fileUrl;
        this.description = description;
        this.uploadedAt = new Date();
    }
}

/**
 * 时间日志类
 */
class TimeLog {
    taskId: string;
    userId: string;
    date: Date;
    hours: number;
    description?: string;
    billable: boolean;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(taskId: string, userId: string, date: Date, hours: number, 
                description?: string, billable?: boolean) {
        this.taskId = taskId;
        this.userId = userId;
        this.date = date;
        this.hours = hours;
        this.description = description;
        this.billable = billable || false;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 任务分配类
 */
class TaskAssignment {
    taskId: string;
    assignerId: string;
    assigneeId: string;
    assignedAt: Date;
    
    constructor(taskId: string, assignerId: string, assigneeId: string) {
        this.taskId = taskId;
        this.assignerId = assignerId;
        this.assigneeId = assigneeId;
        this.assignedAt = new Date();
    }
}

/**
 * 标签类
 */
class Tag {
    name: string;
    color?: string;
    description?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(name: string, color?: string, description?: string) {
        this.name = name;
        this.color = color;
        this.description = description;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 任务标签类
 */
class TaskTag {
    taskId: string;
    tagId: string;
    createdAt: Date;
    
    constructor(taskId: string, tagId: string) {
        this.taskId = taskId;
        this.tagId = tagId;
        this.createdAt = new Date();
    }
}

/**
 * 通知类
 */
class Notification {
    userId: string;
    type: NotificationType;
    actorId: string;
    targetId: string;
    message: string;
    isRead: boolean;
    createdAt: Date;
    readAt?: Date;
    
    constructor(userId: string, type: NotificationType, actorId: string, 
                targetId: string, message: string) {
        this.userId = userId;
        this.type = type;
        this.actorId = actorId;
        this.targetId = targetId;
        this.message = message;
        this.isRead = false;
        this.createdAt = new Date();
    }
}

/**
 * 通知类型枚举
 */
enum NotificationType {
    ProjectMembership = "project_membership",
    ProjectRoleChange = "project_role_change",
    ProjectRemoval = "project_removal",
    TaskAssignment = "task_assignment",
    TaskCompletion = "task_completion",
    Comment = "comment",
    CommentReply = "comment_reply"
}