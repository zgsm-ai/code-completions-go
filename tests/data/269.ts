/**
 * 社交媒体平台类
 * 用于管理用户、帖子、评论和社交关系
 */
class SocialMediaPlatform {
    private users: Map<string, User>;                 // 用户ID到用户信息的映射
    private posts: Map<string, Post>;                 // 帖子ID到帖子信息的映射
    private comments: Map<string, Comment>;           // 评论ID到评论信息的映射
    private likes: Map<string, Like>;                 // 点赞ID到点赞信息的映射
    private friendships: Map<string, Friendship>;     // 友谊ID到友谊信息的映射
    private follows: Map<string, Follow>;             // 关注ID到关注信息的映射
    private groups: Map<string, Group>;              // 群组ID到群组信息的映射
    private groupMemberships: Map<string, GroupMembership>; // 群组成员ID到群组成员信息的映射
    private messages: Map<string, Message>;           // 消息ID到消息信息的映射
    private notifications: Map<string, Notification>; // 通知ID到通知信息的映射
    
    /**
     * 构造函数
     */
    constructor() {
        this.users = new Map();
        this.posts = new Map();
        this.comments = new Map();
        this.likes = new Map();
        this.friendships = new Map();
        this.follows = new Map();
        this.groups = new Map();
        this.groupMemberships = new Map();
        this.messages = new Map();
        this.notifications = new Map();
    }
    
    /**
     * 注册新用户
     * @param userId - 用户唯一标识符
     * @param username - 用户名
     * @param email - 电子邮件
     * @param password - 密码
     * @param firstName - 名字
     * @param lastName - 姓氏
     * @param dateOfBirth - 出生日期
     * @param gender - 性别
     * @param profilePicture - 个人资料图片URL（可选）
     * @param bio - 个人简介（可选）
     * @param location - 位置（可选）
     * @param website - 网站URL（可选）
     * @returns 注册成功返回true，否则返回false
     */
    registerUser(userId: string, username: string, email: string, password: string, 
                firstName: string, lastName: string, dateOfBirth: Date, gender: Gender, 
                profilePicture?: string, bio?: string, location?: string, website?: string): boolean {
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
            dateOfBirth,
            gender,
            profilePicture,
            bio,
            location,
            website
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
     * @param gender - 新的性别（可选）
     * @param profilePicture - 新的个人资料图片URL（可选）
     * @param bio - 新的个人简介（可选）
     * @param location - 新的位置（可选）
     * @param website - 新的网站URL（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateUser(userId: string, username?: string, email?: string, password?: string, 
               firstName?: string, lastName?: string, gender?: Gender, 
               profilePicture?: string, bio?: string, location?: string, website?: string): boolean {
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
        
        if (gender !== undefined) {
            user.gender = gender;
        }
        
        if (profilePicture !== undefined) {
            user.profilePicture = profilePicture;
        }
        
        if (bio !== undefined) {
            user.bio = bio;
        }
        
        if (location !== undefined) {
            user.location = location;
        }
        
        if (website !== undefined) {
            user.website = website;
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
        
        // 删除用户的所有帖子
        for (const [postId, post] of this.posts) {
            if (post.userId === userId) {
                // 删除帖子的所有评论
                for (const [commentId, comment] of this.comments) {
                    if (comment.postId === postId) {
                        this.comments.delete(commentId);
                    }
                }
                
                // 删除帖子的所有点赞
                for (const [likeId, like] of this.likes) {
                    if (like.targetId === postId && like.targetType === TargetType.Post) {
                        this.likes.delete(likeId);
                    }
                }
                
                this.posts.delete(postId);
            }
        }
        
        // 删除用户的所有评论
        for (const [commentId, comment] of this.comments) {
            if (comment.userId === userId) {
                this.comments.delete(commentId);
            }
        }
        
        // 删除用户的所有点赞
        for (const [likeId, like] of this.likes) {
            if (like.userId === userId) {
                this.likes.delete(likeId);
            }
        }
        
        // 删除用户的所有友谊关系
        for (const [friendshipId, friendship] of this.friendships) {
            if (friendship.userId1 === userId || friendship.userId2 === userId) {
                this.friendships.delete(friendshipId);
            }
        }
        
        // 删除用户的所有关注关系
        for (const [followId, follow] of this.follows) {
            if (follow.followerId === userId || follow.followingId === userId) {
                this.follows.delete(followId);
            }
        }
        
        // 删除用户的所有群组成员关系
        for (const [membershipId, membership] of this.groupMemberships) {
            if (membership.userId === userId) {
                this.groupMemberships.delete(membershipId);
            }
        }
        
        // 如果用户是群组管理员，将群组删除
        for (const [groupId, group] of this.groups) {
            if (group.adminId === userId) {
                // 删除群组的所有成员关系
                for (const [membershipId, membership] of this.groupMemberships) {
                    if (membership.groupId === groupId) {
                        this.groupMemberships.delete(membershipId);
                    }
                }
                
                this.groups.delete(groupId);
            }
        }
        
        // 删除用户的所有消息
        for (const [messageId, message] of this.messages) {
            if (message.senderId === userId || message.receiverId === userId) {
                this.messages.delete(messageId);
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
     * 创建新帖子
     * @param postId - 帖子唯一标识符
     * @param userId - 用户ID
     * @param content - 帖子内容
     * @param images - 图片URL列表（可选）
     * @param visibility - 可见性（可选）
     * @param location - 位置（可选）
     * @param tags - 标签列表（可选）
     * @returns 创建成功返回true，否则返回false
     */
    createPost(postId: string, userId: string, content: string, 
               images?: string[], visibility?: Visibility, location?: string, 
               tags?: string[]): boolean {
        if (this.posts.has(postId)) {
            return false;
        }
        
        if (!this.users.has(userId)) {
            return false;
        }
        
        if (!content || content.trim() === '') {
            return false;
        }
        
        const post = new Post(
            postId,
            userId,
            content,
            images,
            visibility || Visibility.Public,
            location,
            tags
        );
        
        this.posts.set(postId, post);
        
        return true;
    }
    
    /**
     * 更新帖子
     * @param postId - 帖子ID
     * @param content - 新的内容（可选）
     * @param images - 新的图片URL列表（可选）
     * @param visibility - 新的可见性（可选）
     * @param location - 新的位置（可选）
     * @param tags - 新的标签列表（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updatePost(postId: string, content?: string, images?: string[], 
               visibility?: Visibility, location?: string, tags?: string[]): boolean {
        if (!this.posts.has(postId)) {
            return false;
        }
        
        const post = this.posts.get(postId)!;
        
        if (content !== undefined) {
            if (!content || content.trim() === '') {
                return false;
            }
            post.content = content;
        }
        
        if (images !== undefined) {
            post.images = images ? [...images] : [];
        }
        
        if (visibility !== undefined) {
            post.visibility = visibility;
        }
        
        if (location !== undefined) {
            post.location = location;
        }
        
        if (tags !== undefined) {
            post.tags = tags ? [...tags] : [];
        }
        
        post.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取帖子
     * @param postId - 帖子ID
     * @returns 帖子对象，若不存在则返回null
     */
    getPost(postId: string): Post | null {
        if (!this.posts.has(postId)) {
            return null;
        }
        
        return this.posts.get(postId)!;
    }
    
    /**
     * 删除帖子
     * @param postId - 帖子ID
     * @returns 删除成功返回true，否则返回false
     */
    deletePost(postId: string): boolean {
        if (!this.posts.has(postId)) {
            return false;
        }
        
        // 删除帖子的所有评论
        for (const [commentId, comment] of this.comments) {
            if (comment.postId === postId) {
                this.comments.delete(commentId);
            }
        }
        
        // 删除帖子的所有点赞
        for (const [likeId, like] of this.likes) {
            if (like.targetId === postId && like.targetType === TargetType.Post) {
                this.likes.delete(likeId);
            }
        }
        
        // 删除帖子
        this.posts.delete(postId);
        
        return true;
    }
    
    /**
     * 获取用户的帖子
     * @param userId - 用户ID
     * @param limit - 返回帖子数量限制（可选）
     * @returns 帖子列表
     */
    getUserPosts(userId: string, limit?: number): Post[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const posts: Post[] = [];
        
        for (const [postId, post] of this.posts) {
            if (post.userId === userId) {
                posts.push({...post, postId});
            }
        }
        
        // 按创建时间降序排序
        posts.sort((a, b) => b.createdAt.getTime() - a.createdAt.getTime());
        
        if (limit && posts.length > limit) {
            return posts.slice(0, limit);
        }
        
        return posts;
    }
    
    /**
     * 获取用户的动态（用户和关注用户的帖子）
     * @param userId - 用户ID
     * @param limit - 返回帖子数量限制（可选）
     * @returns 帖子列表
     */
    getUserFeed(userId: string, limit?: number): Post[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        // 获取用户关注的所有用户ID
        const followingIds = this.getFollowing(userId);
        
        // 包含用户自己
        followingIds.push(userId);
        
        const posts: Post[] = [];
        
        for (const [postId, post] of this.posts) {
            if (followingIds.includes(post.userId)) {
                // 检查帖子可见性
                if (post.visibility === Visibility.Public || 
                    (post.visibility === Visibility.Friends && this.isFriend(userId, post.userId)) ||
                    post.userId === userId) {
                    posts.push({...post, postId});
                }
            }
        }
        
        // 按创建时间降序排序
        posts.sort((a, b) => b.createdAt.getTime() - a.createdAt.getTime());
        
        if (limit && posts.length > limit) {
            return posts.slice(0, limit);
        }
        
        return posts;
    }
    
    /**
     * 添加评论
     * @param commentId - 评论唯一标识符
     * @param postId - 帖子ID
     * @param userId - 用户ID
     * @param content - 评论内容
     * @param parentCommentId - 父评论ID（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addComment(commentId: string, postId: string, userId: string, 
                content: string, parentCommentId?: string): boolean {
        if (this.comments.has(commentId)) {
            return false;
        }
        
        if (!this.posts.has(postId) || !this.users.has(userId)) {
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
            postId,
            userId,
            content,
            parentCommentId
        );
        
        this.comments.set(commentId, comment);
        
        // 如果评论的是别人的帖子，创建通知
        const post = this.posts.get(postId)!;
        if (post.userId !== userId) {
            this.createNotification(
                post.userId,
                NotificationType.Comment,
                userId,
                postId,
                `评论了你的帖子`
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
                    postId,
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
        
        // 删除评论的所有点赞
        for (const [likeId, like] of this.likes) {
            if (like.targetId === commentId && like.targetType === TargetType.Comment) {
                this.likes.delete(likeId);
            }
        }
        
        // 删除评论
        this.comments.delete(commentId);
        
        return true;
    }
    
    /**
     * 获取帖子的评论
     * @param postId - 帖子ID
     * @param limit - 返回评论数量限制（可选）
     * @returns 评论列表
     */
    getPostComments(postId: string, limit?: number): Comment[] {
        if (!this.posts.has(postId)) {
            return [];
        }
        
        const comments: Comment[] = [];
        
        for (const [commentId, comment] of this.comments) {
            if (comment.postId === postId && !comment.parentCommentId) {
                comments.push({...comment, commentId});
            }
        }
        
        // 按创建时间降序排序
        comments.sort((a, b) => b.createdAt.getTime() - a.createdAt.getTime());
        
        if (limit && comments.length > limit) {
            return comments.slice(0, limit);
        }
        
        return comments;
    }
    
    /**
     * 获取评论的回复
     * @param commentId - 评论ID
     * @param limit - 返回回复数量限制（可选）
     * @returns 评论列表
     */
    getCommentReplies(commentId: string, limit?: number): Comment[] {
        if (!this.comments.has(commentId)) {
            return [];
        }
        
        const replies: Comment[] = [];
        
        for (const [id, comment] of this.comments) {
            if (comment.parentCommentId === commentId) {
                replies.push({...comment, commentId: id});
            }
        }
        
        // 按创建时间升序排序
        replies.sort((a, b) => a.createdAt.getTime() - b.createdAt.getTime());
        
        if (limit && replies.length > limit) {
            return replies.slice(0, limit);
        }
        
        return replies;
    }
    
    /**
     * 添加点赞
     * @param likeId - 点赞唯一标识符
     * @param userId - 用户ID
     * @param targetType - 目标类型
     * @param targetId - 目标ID（帖子ID或评论ID）
     * @returns 添加成功返回true，否则返回false
     */
    addLike(likeId: string, userId: string, targetType: TargetType, targetId: string): boolean {
        if (this.likes.has(likeId)) {
            return false;
        }
        
        if (!this.users.has(userId)) {
            return false;
        }
        
        // 检查目标是否存在
        if (targetType === TargetType.Post && !this.posts.has(targetId)) {
            return false;
        }
        
        if (targetType === TargetType.Comment && !this.comments.has(targetId)) {
            return false;
        }
        
        // 检查用户是否已经点赞过该目标
        for (const like of this.likes.values()) {
            if (like.userId === userId && like.targetType === targetType && like.targetId === targetId) {
                return false;
            }
        }
        
        const like = new Like(
            userId,
            targetType,
            targetId
        );
        
        this.likes.set(likeId, like);
        
        // 如果点赞的是别人的帖子或评论，创建通知
        if (targetType === TargetType.Post) {
            const post = this.posts.get(targetId)!;
            if (post.userId !== userId) {
                this.createNotification(
                    post.userId,
                    NotificationType.Like,
                    userId,
                    targetId,
                    `点赞了你的帖子`
                );
            }
        } else if (targetType === TargetType.Comment) {
            const comment = this.comments.get(targetId)!;
            if (comment.userId !== userId) {
                this.createNotification(
                    comment.userId,
                    NotificationType.Like,
                    userId,
                    targetId,
                    `点赞了你的评论`
                );
            }
        }
        
        return true;
    }
    
    /**
     * 取消点赞
     * @param userId - 用户ID
     * @param targetType - 目标类型
     * @param targetId - 目标ID（帖子ID或评论ID）
     * @returns 取消成功返回true，否则返回false
     */
    removeLike(userId: string, targetType: TargetType, targetId: string): boolean {
        let likeRemoved = false;
        
        for (const [likeId, like] of this.likes) {
            if (like.userId === userId && like.targetType === targetType && like.targetId === targetId) {
                this.likes.delete(likeId);
                likeRemoved = true;
                break;
            }
        }
        
        return likeRemoved;
    }
    
    /**
     * 检查用户是否已点赞目标
     * @param userId - 用户ID
     * @param targetType - 目标类型
     * @param targetId - 目标ID（帖子ID或评论ID）
     * @returns 已点赞返回true，否则返回false
     */
    hasLiked(userId: string, targetType: TargetType, targetId: string): boolean {
        for (const like of this.likes.values()) {
            if (like.userId === userId && like.targetType === targetType && like.targetId === targetId) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 获取目标的点赞数
     * @param targetType - 目标类型
     * @param targetId - 目标ID（帖子ID或评论ID）
     * @returns 点赞数
     */
    getLikeCount(targetType: TargetType, targetId: string): number {
        let count = 0;
        
        for (const like of this.likes.values()) {
            if (like.targetType === targetType && like.targetId === targetId) {
                count++;
            }
        }
        
        return count;
    }
    
    /**
     * 发送好友请求
     * @param friendshipId - 友谊唯一标识符
     * @param userId1 - 用户1 ID
     * @param userId2 - 用户2 ID
     * @returns 发送成功返回true，否则返回false
     */
    sendFriendRequest(friendshipId: string, userId1: string, userId2: string): boolean {
        if (this.friendships.has(friendshipId)) {
            return false;
        }
        
        if (!this.users.has(userId1) || !this.users.has(userId2)) {
            return false;
        }
        
        if (userId1 === userId2) {
            return false;
        }
        
        // 检查是否已存在友谊关系
        for (const friendship of this.friendships.values()) {
            if ((friendship.userId1 === userId1 && friendship.userId2 === userId2) ||
                (friendship.userId1 === userId2 && friendship.userId2 === userId1)) {
                return false;
            }
        }
        
        const friendship = new Friendship(
            userId1,
            userId2,
            FriendshipStatus.Pending
        );
        
        this.friendships.set(friendshipId, friendship);
        
        // 创建通知
        this.createNotification(
            userId2,
            NotificationType.FriendRequest,
            userId1,
            '',
            `向你发送了好友请求`
        );
        
        return true;
    }
    
    /**
     * 接受好友请求
     * @param friendshipId - 友谊ID
     * @returns 接受成功返回true，否则返回false
     */
    acceptFriendRequest(friendshipId: string): boolean {
        if (!this.friendships.has(friendshipId)) {
            return false;
        }
        
        const friendship = this.friendships.get(friendshipId)!;
        
        if (friendship.status !== FriendshipStatus.Pending) {
            return false;
        }
        
        friendship.status = FriendshipStatus.Accepted;
        friendship.acceptedAt = new Date();
        friendship.updatedAt = new Date();
        
        // 创建通知
        this.createNotification(
            friendship.userId1,
            NotificationType.FriendRequestAccepted,
            friendship.userId2,
            '',
            `接受了你的好友请求`
        );
        
        return true;
    }
    
    /**
     * 拒绝好友请求
     * @param friendshipId - 友谊ID
     * @returns 拒绝成功返回true，否则返回false
     */
    rejectFriendRequest(friendshipId: string): boolean {
        if (!this.friendships.has(friendshipId)) {
            return false;
        }
        
        const friendship = this.friendships.get(friendshipId)!;
        
        if (friendship.status !== FriendshipStatus.Pending) {
            return false;
        }
        
        friendship.status = FriendshipStatus.Rejected;
        friendship.rejectedAt = new Date();
        friendship.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 删除好友
     * @param userId1 - 用户1 ID
     * @param userId2 - 用户2 ID
     * @returns 删除成功返回true，否则返回false
     */
    removeFriend(userId1: string, userId2: string): boolean {
        let friendshipRemoved = false;
        
        for (const [friendshipId, friendship] of this.friendships) {
            if ((friendship.userId1 === userId1 && friendship.userId2 === userId2) ||
                (friendship.userId1 === userId2 && friendship.userId2 === userId1)) {
                if (friendship.status === FriendshipStatus.Accepted) {
                    friendship.status = FriendshipStatus.Deleted;
                    friendship.deletedAt = new Date();
                    friendship.updatedAt = new Date();
                    friendshipRemoved = true;
                    break;
                }
            }
        }
        
        return friendshipRemoved;
    }
    
    /**
     * 检查两个用户是否是好友
     * @param userId1 - 用户1 ID
     * @param userId2 - 用户2 ID
     * @returns 是好友返回true，否则返回false
     */
    isFriend(userId1: string, userId2: string): boolean {
        for (const friendship of this.friendships.values()) {
            if (((friendship.userId1 === userId1 && friendship.userId2 === userId2) ||
                 (friendship.userId1 === userId2 && friendship.userId2 === userId1)) &&
                friendship.status === FriendshipStatus.Accepted) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 获取用户的好友列表
     * @param userId - 用户ID
     * @returns 好友ID列表
     */
    getFriends(userId: string): string[] {
        const friends: string[] = [];
        
        for (const friendship of this.friendships.values()) {
            if (friendship.status === FriendshipStatus.Accepted) {
                if (friendship.userId1 === userId) {
                    friends.push(friendship.userId2);
                } else if (friendship.userId2 === userId) {
                    friends.push(friendship.userId1);
                }
            }
        }
        
        return friends;
    }
    
    /**
     * 获取用户收到的好友请求
     * @param userId - 用户ID
     * @returns 好友请求列表
     */
    getFriendRequests(userId: string): Friendship[] {
        const requests: Friendship[] = [];
        
        for (const [friendshipId, friendship] of this.friendships) {
            if (friendship.userId2 === userId && friendship.status === FriendshipStatus.Pending) {
                requests.push({...friendship, friendshipId});
            }
        }
        
        return requests;
    }
    
    /**
     * 关注用户
     * @param followId - 关注唯一标识符
     * @param followerId - 关注者ID
     * @param followingId - 被关注者ID
     * @returns 关注成功返回true，否则返回false
     */
    followUser(followId: string, followerId: string, followingId: string): boolean {
        if (this.follows.has(followId)) {
            return false;
        }
        
        if (!this.users.has(followerId) || !this.users.has(followingId)) {
            return false;
        }
        
        if (followerId === followingId) {
            return false;
        }
        
        // 检查是否已关注
        for (const follow of this.follows.values()) {
            if (follow.followerId === followerId && follow.followingId === followingId) {
                return false;
            }
        }
        
        const follow = new Follow(
            followerId,
            followingId
        );
        
        this.follows.set(followId, follow);
        
        // 如果不是好友关系，创建通知
        if (!this.isFriend(followerId, followingId)) {
            this.createNotification(
                followingId,
                NotificationType.Follow,
                followerId,
                '',
                `关注了你`
            );
        }
        
        return true;
    }
    
    /**
     * 取消关注用户
     * @param followerId - 关注者ID
     * @param followingId - 被关注者ID
     * @returns 取消成功返回true，否则返回false
     */
    unfollowUser(followerId: string, followingId: string): boolean {
        let followRemoved = false;
        
        for (const [followId, follow] of this.follows) {
            if (follow.followerId === followerId && follow.followingId === followingId) {
                this.follows.delete(followId);
                followRemoved = true;
                break;
            }
        }
        
        return followRemoved;
    }
    
    /**
     * 检查用户是否已关注另一个用户
     * @param followerId - 关注者ID
     * @param followingId - 被关注者ID
     * @returns 已关注返回true，否则返回false
     */
    isFollowing(followerId: string, followingId: string): boolean {
        for (const follow of this.follows.values()) {
            if (follow.followerId === followerId && follow.followingId === followingId) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 获取用户关注的用户列表
     * @param userId - 用户ID
     * @returns 被关注用户ID列表
     */
    getFollowing(userId: string): string[] {
        const following: string[] = [];
        
        for (const follow of this.follows.values()) {
            if (follow.followerId === userId) {
                following.push(follow.followingId);
            }
        }
        
        return following;
    }
    
    /**
     * 获取关注用户的用户列表
     * @param userId - 用户ID
     * @returns 关注者ID列表
     */
    getFollowers(userId: string): string[] {
        const followers: string[] = [];
        
        for (const follow of this.follows.values()) {
            if (follow.followingId === userId) {
                followers.push(follow.followerId);
            }
        }
        
        return followers;
    }
    
    /**
     * 创建群组
     * @param groupId - 群组唯一标识符
     * @param name - 群组名称
     * @param description - 群组描述
     * @param adminId - 管理员ID
     * @param isPrivate - 是否为私密群组
     * @param coverPicture - 封面图片URL（可选）
     * @returns 创建成功返回true，否则返回false
     */
    createGroup(groupId: string, name: string, description: string, 
                adminId: string, isPrivate: boolean, coverPicture?: string): boolean {
        if (this.groups.has(groupId)) {
            return false;
        }
        
        if (!this.users.has(adminId)) {
            return false;
        }
        
        if (!name || name.trim() === '') {
            return false;
        }
        
        const group = new Group(
            name,
            description,
            adminId,
            isPrivate,
            coverPicture
        );
        
        this.groups.set(groupId, group);
        
        // 管理员自动成为群组成员
        const membershipId = `membership_${groupId}_${adminId}`;
        this.addGroupMember(membershipId, groupId, adminId, GroupMemberRole.Admin);
        
        return true;
    }
    
    /**
     * 更新群组信息
     * @param groupId - 群组ID
     * @param name - 新的名称（可选）
     * @param description - 新的描述（可选）
     * @param isPrivate - 新的私密设置（可选）
     * @param coverPicture - 新的封面图片URL（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateGroup(groupId: string, name?: string, description?: string, 
                isPrivate?: boolean, coverPicture?: string): boolean {
        if (!this.groups.has(groupId)) {
            return false;
        }
        
        const group = this.groups.get(groupId)!;
        
        if (name !== undefined) {
            if (!name || name.trim() === '') {
                return false;
            }
            group.name = name;
        }
        
        if (description !== undefined) {
            group.description = description;
        }
        
        if (isPrivate !== undefined) {
            group.isPrivate = isPrivate;
        }
        
        if (coverPicture !== undefined) {
            group.coverPicture = coverPicture;
        }
        
        group.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取群组信息
     * @param groupId - 群组ID
     * @returns 群组对象，若不存在则返回null
     */
    getGroup(groupId: string): Group | null {
        if (!this.groups.has(groupId)) {
            return null;
        }
        
        return this.groups.get(groupId)!;
    }
    
    /**
     * 删除群组
     * @param groupId - 群组ID
     * @param userId - 操作用户ID
     * @returns 删除成功返回true，否则返回false
     */
    deleteGroup(groupId: string, userId: string): boolean {
        if (!this.groups.has(groupId)) {
            return false;
        }
        
        const group = this.groups.get(groupId)!;
        
        // 只有管理员才能删除群组
        if (group.adminId !== userId) {
            return false;
        }
        
        // 删除群组的所有成员关系
        for (const [membershipId, membership] of this.groupMemberships) {
            if (membership.groupId === groupId) {
                this.groupMemberships.delete(membershipId);
            }
        }
        
        // 删除群组
        this.groups.delete(groupId);
        
        return true;
    }
    
    /**
     * 添加群组成员
     * @param membershipId - 成员关系唯一标识符
     * @param groupId - 群组ID
     * @param userId - 用户ID
     * @param role - 成员角色
     * @returns 添加成功返回true，否则返回false
     */
    addGroupMember(membershipId: string, groupId: string, userId: string, 
                   role: GroupMemberRole): boolean {
        if (this.groupMemberships.has(membershipId)) {
            return false;
        }
        
        if (!this.groups.has(groupId) || !this.users.has(userId)) {
            return false;
        }
        
        // 检查用户是否已是群组成员
        for (const membership of this.groupMemberships.values()) {
            if (membership.groupId === groupId && membership.userId === userId) {
                return false;
            }
        }
        
        const membership = new GroupMembership(
            groupId,
            userId,
            role
        );
        
        this.groupMemberships.set(membershipId, membership);
        
        // 创建通知
        this.createNotification(
            userId,
            NotificationType.GroupMembership,
            this.groups.get(groupId)!.adminId,
            groupId,
            `将你添加到了群组 ${this.groups.get(groupId)!.name}`
        );
        
        return true;
    }
    
    /**
     * 更新群组成员角色
     * @param groupId - 群组ID
     * @param userId - 用户ID
     * @param newRole - 新的角色
     * @param adminId - 操作管理员ID
     * @returns 更新成功返回true，否则返回false
     */
    updateGroupMemberRole(groupId: string, userId: string, 
                         newRole: GroupMemberRole, adminId: string): boolean {
        if (!this.groups.has(groupId) || !this.users.has(userId) || !this.users.has(adminId)) {
            return false;
        }
        
        const group = this.groups.get(groupId)!;
        
        // 只有管理员才能更改成员角色
        if (group.adminId !== adminId) {
            return false;
        }
        
        // 查找成员关系
        for (const [membershipId, membership] of this.groupMemberships) {
            if (membership.groupId === groupId && membership.userId === userId) {
                membership.role = newRole;
                membership.updatedAt = new Date();
                
                // 创建通知
                this.createNotification(
                    userId,
                    NotificationType.GroupRoleChange,
                    adminId,
                    groupId,
                    `将你在群组 ${group.name} 中的角色更改为 ${newRole}`
                );
                
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 移除群组成员
     * @param groupId - 群组ID
     * @param userId - 要移除的用户ID
     * @param adminId - 操作管理员ID
     * @returns 移除成功返回true，否则返回false
     */
    removeGroupMember(groupId: string, userId: string, adminId: string): boolean {
        if (!this.groups.has(groupId) || !this.users.has(userId) || !this.users.has(adminId)) {
            return false;
        }
        
        const group = this.groups.get(groupId)!;
        
        // 只有管理员才能移除成员
        if (group.adminId !== adminId) {
            return false;
        }
        
        // 管理员不能移除自己
        if (userId === adminId) {
            return false;
        }
        
        // 查找并删除成员关系
        let membershipRemoved = false;
        for (const [membershipId, membership] of this.groupMemberships) {
            if (membership.groupId === groupId && membership.userId === userId) {
                this.groupMemberships.delete(membershipId);
                membershipRemoved = true;
                break;
            }
        }
        
        if (membershipRemoved) {
            // 创建通知
            this.createNotification(
                userId,
                NotificationType.GroupRemoval,
                adminId,
                groupId,
                `将你从群组 ${group.name} 中移除`
            );
        }
        
        return membershipRemoved;
    }
    
    /**
     * 用户离开群组
     * @param groupId - 群组ID
     * @param userId - 用户ID
     * @returns 离开成功返回true，否则返回false
     */
    leaveGroup(groupId: string, userId: string): boolean {
        if (!this.groups.has(groupId) || !this.users.has(userId)) {
            return false;
        }
        
        const group = this.groups.get(groupId)!;
        
        // 管理员不能离开群组，只能删除群组或转让管理员权限
        if (group.adminId === userId) {
            return false;
        }
        
        // 查找并删除成员关系
        let membershipRemoved = false;
        for (const [membershipId, membership] of this.groupMemberships) {
            if (membership.groupId === groupId && membership.userId === userId) {
                this.groupMemberships.delete(membershipId);
                membershipRemoved = true;
                break;
            }
        }
        
        return membershipRemoved;
    }
    
    /**
     * 获取群组成员
     * @param groupId - 群组ID
     * @returns 成员ID列表
     */
    getGroupMembers(groupId: string): string[] {
        if (!this.groups.has(groupId)) {
            return [];
        }
        
        const members: string[] = [];
        
        for (const membership of this.groupMemberships.values()) {
            if (membership.groupId === groupId) {
                members.push(membership.userId);
            }
        }
        
        return members;
    }
    
    /**
     * 检查用户是否是群组成员
     * @param groupId - 群组ID
     * @param userId - 用户ID
     * @returns 是成员返回true，否则返回false
     */
    isGroupMember(groupId: string, userId: string): boolean {
        for (const membership of this.groupMemberships.values()) {
            if (membership.groupId === groupId && membership.userId === userId) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 获取用户的群组
     * @param userId - 用户ID
     * @returns 群组ID列表
     */
    getUserGroups(userId: string): string[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const groups: string[] = [];
        
        for (const membership of this.groupMemberships.values()) {
            if (membership.userId === userId) {
                groups.push(membership.groupId);
            }
        }
        
        return groups;
    }
    
    /**
     * 发送消息
     * @param messageId - 消息唯一标识符
     * @param senderId - 发送者ID
     * @param receiverId - 接收者ID
     * @param content - 消息内容
     * @param type - 消息类型
     * @returns 发送成功返回true，否则返回false
     */
    sendMessage(messageId: string, senderId: string, receiverId: string, 
                content: string, type: MessageType): boolean {
        if (this.messages.has(messageId)) {
            return false;
        }
        
        if (!this.users.has(senderId) || !this.users.has(receiverId)) {
            return false;
        }
        
        if (!content || content.trim() === '') {
            return false;
        }
        
        const message = new Message(
            senderId,
            receiverId,
            content,
            type
        );
        
        this.messages.set(messageId, message);
        
        // 创建通知
        if (type === MessageType.Direct) {
            this.createNotification(
                receiverId,
                NotificationType.Message,
                senderId,
                '',
                `向你发送了一条消息`
            );
        }
        
        return true;
    }
    
    /**
     * 获取两个用户之间的消息历史
     * @param userId1 - 用户1 ID
     * @param userId2 - 用户2 ID
     * @param limit - 返回消息数量限制（可选）
     * @returns 消息列表
     */
    getMessagesBetweenUsers(userId1: string, userId2: string, limit?: number): Message[] {
        if (!this.users.has(userId1) || !this.users.has(userId2)) {
            return [];
        }
        
        const messages: Message[] = [];
        
        for (const [messageId, message] of this.messages) {
            if ((message.senderId === userId1 && message.receiverId === userId2) ||
                (message.senderId === userId2 && message.receiverId === userId1)) {
                messages.push({...message, messageId});
            }
        }
        
        // 按创建时间升序排序
        messages.sort((a, b) => a.timestamp.getTime() - b.timestamp.getTime());
        
        if (limit && messages.length > limit) {
            return messages.slice(messages.length - limit);
        }
        
        return messages;
    }
    
    /**
     * 获取用户的所有对话
     * @param userId - 用户ID
     * @returns 对话用户ID列表
     */
    getUserConversations(userId: string): string[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const conversationIds = new Set<string>();
        
        for (const message of this.messages.values()) {
            if (message.senderId === userId) {
                conversationIds.add(message.receiverId);
            } else if (message.receiverId === userId) {
                conversationIds.add(message.senderId);
            }
        }
        
        return Array.from(conversationIds);
    }
    
    /**
     * 创建通知
     * @param userId - 用户ID
     * @param type - 通知类型
     * @param actorId - 触发通知的用户ID
     * @param targetId - 目标ID（可选）
     * @param message - 通知消息
     * @returns 通知ID
     */
    private createNotification(userId: string, type: NotificationType, 
                               actorId: string, targetId: string, message: string): string {
        const notificationId = this.generateId("notification");
        
        const notification = new Notification(
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
     * @returns 通知列表
     */
    getUserNotifications(userId: string, unreadOnly: boolean, limit?: number): Notification[] {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const notifications: Notification[] = [];
        
        for (const [notificationId, notification] of this.notifications) {
            if (notification.userId === userId) {
                if (unreadOnly && notification.isRead) {
                    continue;
                }
                
                notifications.push({...notification, notificationId});
            }
        }
        
        // 按创建时间降序排序
        notifications.sort((a, b) => b.createdAt.getTime() - a.createdAt.getTime());
        
        if (limit && notifications.length > limit) {
            return notifications.slice(0, limit);
        }
        
        return notifications;
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
    dateOfBirth: Date;
    gender: Gender;
    profilePicture?: string;
    bio?: string;
    location?: string;
    website?: string;
    createdAt: Date;
    updatedAt: Date;
    lastLoginAt?: Date;
    isActive: boolean;
    
    constructor(userId: string, username: string, email: string, password: string, 
                firstName: string, lastName: string, dateOfBirth: Date, gender: Gender, 
                profilePicture?: string, bio?: string, location?: string, website?: string) {
        this.userId = userId;
        this.username = username;
        this.email = email;
        this.password = password;
        this.firstName = firstName;
        this.lastName = lastName;
        this.dateOfBirth = dateOfBirth;
        this.gender = gender;
        this.profilePicture = profilePicture;
        this.bio = bio;
        this.location = location;
        this.website = website;
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
    
    /**
     * 计算年龄
     */
    get age(): number {
        const today = new Date();
        let age = today.getFullYear() - this.dateOfBirth.getFullYear();
        const monthDiff = today.getMonth() - this.dateOfBirth.getMonth();
        
        if (monthDiff < 0 || (monthDiff === 0 && today.getDate() < this.dateOfBirth.getDate())) {
            age--;
        }
        
        return age;
    }
}

/**
 * 性别枚举
 */
enum Gender {
    Male = "male",
    Female = "female",
    Other = "other",
    PreferNotToSay = "prefer_not_to_say"
}

/**
 * 帖子类
 */
class Post {
    userId: string;
    content: string;
    images?: string[];
    visibility: Visibility;
    location?: string;
    tags?: string[];
    createdAt: Date;
    updatedAt: Date;
    
    constructor(userId: string, content: string, images?: string[], 
                visibility?: Visibility, location?: string, tags?: string[]) {
        this.userId = userId;
        this.content = content;
        this.images = images ? [...images] : undefined;
        this.visibility = visibility || Visibility.Public;
        this.location = location;
        this.tags = tags ? [...tags] : undefined;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 可见性枚举
 */
enum Visibility {
    Public = "public",
    Friends = "friends",
    Private = "private"
}

/**
 * 评论类
 */
class Comment {
    postId: string;
    userId: string;
    content: string;
    parentCommentId?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(postId: string, userId: string, content: string, parentCommentId?: string) {
        this.postId = postId;
        this.userId = userId;
        this.content = content;
        this.parentCommentId = parentCommentId;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 点赞类
 */
class Like {
    userId: string;
    targetType: TargetType;
    targetId: string;
    createdAt: Date;
    
    constructor(userId: string, targetType: TargetType, targetId: string) {
        this.userId = userId;
        this.targetType = targetType;
        this.targetId = targetId;
        this.createdAt = new Date();
    }
}

/**
 * 目标类型枚举
 */
enum TargetType {
    Post = "post",
    Comment = "comment"
}

/**
 * 友谊类
 */
class Friendship {
    userId1: string;
    userId2: string;
    status: FriendshipStatus;
    createdAt: Date;
    updatedAt: Date;
    acceptedAt?: Date;
    rejectedAt?: Date;
    deletedAt?: Date;
    
    constructor(userId1: string, userId2: string, status: FriendshipStatus) {
        this.userId1 = userId1;
        this.userId2 = userId2;
        this.status = status;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 友谊状态枚举
 */
enum FriendshipStatus {
    Pending = "pending",
    Accepted = "accepted",
    Rejected = "rejected",
    Deleted = "deleted"
}

/**
 * 关注类
 */
class Follow {
    followerId: string;
    followingId: string;
    createdAt: Date;
    
    constructor(followerId: string, followingId: string) {
        this.followerId = followerId;
        this.followingId = followingId;
        this.createdAt = new Date();
    }
}

/**
 * 群组类
 */
class Group {
    name: string;
    description: string;
    adminId: string;
    isPrivate: boolean;
    coverPicture?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(name: string, description: string, adminId: string, 
                isPrivate: boolean, coverPicture?: string) {
        this.name = name;
        this.description = description;
        this.adminId = adminId;
        this.isPrivate = isPrivate;
        this.coverPicture = coverPicture;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 群组成员角色枚举
 */
enum GroupMemberRole {
    Admin = "admin",
    Moderator = "moderator",
    Member = "member"
}

/**
 * 群组成员关系类
 */
class GroupMembership {
    groupId: string;
    userId: string;
    role: GroupMemberRole;
    joinedAt: Date;
    updatedAt: Date;
    
    constructor(groupId: string, userId: string, role: GroupMemberRole) {
        this.groupId = groupId;
        this.userId = userId;
        this.role = role;
        this.joinedAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 消息类
 */
class Message {
    senderId: string;
    receiverId: string;
    content: string;
    type: MessageType;
    timestamp: Date;
    isRead: boolean;
    readAt?: Date;
    
    constructor(senderId: string, receiverId: string, content: string, type: MessageType) {
        this.senderId = senderId;
        this.receiverId = receiverId;
        this.content = content;
        this.type = type;
        this.timestamp = new Date();
        this.isRead = false;
    }
}

/**
 * 消息类型枚举
 */
enum MessageType {
    Direct = "direct",
    Group = "group"
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
    
    constructor(type: NotificationType, actorId: string, targetId: string, message: string) {
        this.userId = '';  // 将在createNotification方法中设置
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
    Like = "like",
    Comment = "comment",
    CommentReply = "comment_reply",
    FriendRequest = "friend_request",
    FriendRequestAccepted = "friend_request_accepted",
    Follow = "follow",
    GroupMembership = "group_membership",
    GroupRoleChange = "group_role_change",
    GroupRemoval = "group_removal",
    Message = "message"
}