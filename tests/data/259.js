/**
 * 社交媒体平台类
 * 用于管理用户、帖子、评论、点赞和社交关系
 */
class SocialMediaPlatform {
    /**
     * 构造函数
     */
    constructor() {
        this.users = new Map();           // 用户ID到用户信息的映射
        this.posts = new Map();           // 帖子ID到帖子信息的映射
        this.comments = new Map();        // 评论ID到评论信息的映射
        this.likes = new Map();            // 点赞记录
        this.friendRequests = new Map();   // 好友请求
        this.friendships = new Map();      // 好友关系
        this.notifications = new Map();    // 通知记录
        this.trendingTopics = [];        // 热门话题
    }
    
    /**
     * 创建新用户
     * @param {string} userId - 用户唯一标识符
     * @param {string} username - 用户名
     * @param {string} email - 电子邮件
     * @param {string} password - 密码
     * @param {Object} profileInfo - 个人资料信息（可选）
     * @returns {boolean} 创建成功返回true，否则返回false
     */
    createUser(userId, username, email, password, profileInfo) {
        if (this.users.has(userId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const user of this.users.values()) {
            if (user.email === email) {
                return false;
            }
        }
        
        // 检查用户名是否已被使用
        for (const user of this.users.values()) {
            if (user.username === username) {
                return false;
            }
        }
        
        const user = {
            userId,
            username,
            email,
            password,
            profileInfo: profileInfo || {},
            friendIds: [],
            postIds: [],
            notificationIds: [],
            createdAt: new Date()
        };
        
        this.users.set(userId, user);
        return true;
    }
    
    /**
     * 验证用户登录
     * @param {string} email - 电子邮件
     * @param {string} password - 密码
     * @returns {string|null} 验证成功返回用户ID，否则返回null
     */
    authenticateUser(email, password) {
        for (const [userId, user] of this.users) {
            if (user.email === email && user.password === password) {
                return userId;
            }
        }
        return null;
    }
    
    /**
     * 创建新帖子
     * @param {string} userId - 用户ID
     * @param {string} content - 帖子内容
     * @param {Array} mediaUrls - 媒体URL列表（可选）
     * @param {Array} tags - 标签列表（可选）
     * @returns {string|null} 创建成功返回帖子ID，否则返回null
     */
    createPost(userId, content, mediaUrls, tags) {
        if (!this.users.has(userId)) {
            return null;
        }
        
        const postId = this.generateId('post');
        const post = {
            postId,
            userId,
            content,
            mediaUrls: mediaUrls || [],
            tags: tags || [],
            likes: 0,
            commentIds: [],
            createdAt: new Date()
        };
        
        this.posts.set(postId, post);
        
        // 更新用户的帖子列表
        this.users.get(userId).postIds.push(postId);
        
        // 更新热门话题
        if (tags) {
            for (const tag of tags) {
                this.updateTrendingTopics(tag);
            }
        }
        
        return postId;
    }
    
    /**
     * 添加评论
     * @param {string} userId - 用户ID
     * @param {string} postId - 帖子ID
     * @param {string} content - 评论内容
     * @returns {string|null} 评论成功返回评论ID，否则返回null
     */
    addComment(userId, postId, content) {
        if (!this.users.has(userId) || !this.posts.has(postId)) {
            return null;
        }
        
        const commentId = this.generateId('comment');
        const comment = {
            commentId,
            userId,
            postId,
            content,
            likes: 0,
            createdAt: new Date()
        };
        
        this.comments.set(commentId, comment);
        
        // 更新帖子的评论列表
        this.posts.get(postId).commentIds.push(commentId);
        
        // 通知帖子作者
        const postAuthorId = this.posts.get(postId).userId;
        if (postAuthorId !== userId) {
            this.addNotification(
                postAuthorId, 
                'comment', 
                `${this.users.get(userId).username} 评论了你的帖子`, 
                postId
            );
        }
        
        return commentId;
    }
    
    /**
     * 点赞帖子
     * @param {string} userId - 用户ID
     * @param {string} postId - 帖子ID
     * @returns {boolean} 点赞成功返回true，否则返回false
     */
    likePost(userId, postId) {
        if (!this.users.has(userId) || !this.posts.has(postId)) {
            return false;
        }
        
        const likeKey = `${userId}_${postId}`;
        if (this.likes.has(likeKey)) {
            return false;  // 已经点赞过
        }
        
        const like = {
            likeId: this.generateId('like'),
            userId,
            postId,
            createdAt: new Date()
        };
        
        this.likes.set(likeKey, like);
        
        // 更新帖子的点赞数
        this.posts.get(postId).likes++;
        
        // 通知帖子作者
        const postAuthorId = this.posts.get(postId).userId;
        if (postAuthorId !== userId) {
            this.addNotification(
                postAuthorId, 
                'like', 
                `${this.users.get(userId).username} 点赞了你的帖子`, 
                postId
            );
        }
        
        return true;
    }
    
    /**
     * 取消点赞帖子
     * @param {string} userId - 用户ID
     * @param {string} postId - 帖子ID
     * @returns {boolean} 取消成功返回true，否则返回false
     */
    unlikePost(userId, postId) {
        if (!this.users.has(userId) || !this.posts.has(postId)) {
            return false;
        }
        
        const likeKey = `${userId}_${postId}`;
        if (!this.likes.has(likeKey)) {
            return false;  // 没有点赞过
        }
        
        this.likes.delete(likeKey);
        this.posts.get(postId).likes--;
        
        return true;
    }
    
    /**
     * 发送好友请求
     * @param {string} senderId - 发送者用户ID
     * @param {string} receiverId - 接收者用户ID
     * @returns {string|null} 发送成功返回好友请求ID，否则返回null
     */
    sendFriendRequest(senderId, receiverId) {
        if (!this.users.has(senderId) || !this.users.has(receiverId)) {
            return null;
        }
        
        if (senderId === receiverId) {
            return null;
        }
        
        // 检查是否已经发送过请求或已经是好友
        const requestKey = `${senderId}_${receiverId}`;
        if (this.friendRequests.has(requestKey) || this.friendships.has(requestKey)) {
            return null;
        }
        
        const requestId = this.generateId('friend_request');
        const request = {
            requestId,
            senderId,
            receiverId,
            status: 'pending',  // pending, accepted, rejected
            createdAt: new Date()
        };
        
        this.friendRequests.set(requestKey, request);
        
        // 通知接收者
        this.addNotification(
            receiverId, 
            'friend_request', 
            `${this.users.get(senderId).username} 向你发送了好友请求`, 
            requestId
        );
        
        return requestId;
    }
    
    /**
     * 接受好友请求
     * @param {string} requestId - 好友请求ID
     * @returns {boolean} 接受成功返回true，否则返回false
     */
    acceptFriendRequest(requestId) {
        let request = null;
        let requestKey = null;
        
        // 查找请求
        for (const [key, req] of this.friendRequests) {
            if (req.requestId === requestId) {
                request = req;
                requestKey = key;
                break;
            }
        }
        
        if (!request || request.status !== 'pending') {
            return false;
        }
        
        // 更新请求状态
        request.status = 'accepted';
        request.updatedAt = new Date();
        
        // 创建好友关系
        const friendshipKey = `${request.senderId}_${request.receiverId}`;
        const friendship = {
            friendshipId: this.generateId('friendship'),
            user1Id: request.senderId,
            user2Id: request.receiverId,
            createdAt: new Date()
        };
        
        this.friendships.set(friendshipKey, friendship);
        
        // 更新用户的好友列表
        this.users.get(request.senderId).friendIds.push(request.receiverId);
        this.users.get(request.receiverId).friendIds.push(request.senderId);
        
        // 通知发送者
        this.addNotification(
            request.senderId, 
            'friend_accepted', 
            `${this.users.get(request.receiverId).username} 接受了你的好友请求`, 
            requestId
        );
        
        return true;
    }
    
    /**
     * 拒绝好友请求
     * @param {string} requestId - 好友请求ID
     * @returns {boolean} 拒绝成功返回true，否则返回false
     */
    rejectFriendRequest(requestId) {
        let request = null;
        
        // 查找请求
        for (const req of this.friendRequests.values()) {
            if (req.requestId === requestId) {
                request = req;
                break;
            }
        }
        
        if (!request || request.status !== 'pending') {
            return false;
        }
        
        // 更新请求状态
        request.status = 'rejected';
        request.updatedAt = new Date();
        
        // 通知发送者
        this.addNotification(
            request.senderId, 
            'friend_rejected', 
            `${this.users.get(request.receiverId).username} 拒绝了你的好友请求`, 
            requestId
        );
        
        return true;
    }
    
    /**
     * 移除好友
     * @param {string} userId - 用户ID
     * @param {string} friendId - 好友ID
     * @returns {boolean} 移除成功返回true，否则返回false
     */
    removeFriend(userId, friendId) {
        if (!this.users.has(userId) || !this.users.has(friendId)) {
            return false;
        }
        
        if (!this.users.get(userId).friendIds.includes(friendId)) {
            return false;
        }
        
        // 从用户的好友列表中移除
        const user = this.users.get(userId);
        user.friendIds = user.friendIds.filter(id => id !== friendId);
        
        const friend = this.users.get(friendId);
        friend.friendIds = friend.friendIds.filter(id => id !== userId);
        
        // 删除好友关系记录
        const friendshipKey1 = `${userId}_${friendId}`;
        const friendshipKey2 = `${friendId}_${userId}`;
        
        if (this.friendships.has(friendshipKey1)) {
            this.friendships.delete(friendshipKey1);
        }
        if (this.friendships.has(friendshipKey2)) {
            this.friendships.delete(friendshipKey2);
        }
        
        return true;
    }
    
    /**
     * 获取用户动态流
     * @param {string} userId - 用户ID
     * @param {number} limit - 返回帖子数量限制
     * @returns {Array} 帖子列表
     */
    getUserFeed(userId, limit) {
        if (!this.users.has(userId)) {
            return [];
        }
        
        // 获取用户和好友的帖子
        const userAndFriends = [userId, ...this.users.get(userId).friendIds];
        
        const feedPosts = [];
        for (const [postId, post] of this.posts) {
            if (userAndFriends.includes(post.userId)) {
                feedPosts.push(post);
            }
        }
        
        // 按创建时间降序排序
        feedPosts.sort((a, b) => b.createdAt - a.createdAt);
        
        // 限制返回数量
        return feedPosts.slice(0, limit);
    }
    
    /**
     * 搜索帖子
     * @param {string} keyword - 搜索关键词
     * @returns {Array} 匹配的帖子列表
     */
    searchPosts(keyword) {
        keyword = keyword.toLowerCase();
        const result = [];
        
        for (const post of this.posts.values()) {
            if (post.content.toLowerCase().includes(keyword) ||
                (post.tags && post.tags.some(tag => tag.toLowerCase().includes(keyword)))) {
                result.push(post);
            }
        }
        
        return result;
    }
    
    /**
     * 获取热门话题
     * @param {number} limit - 返回话题数量限制
     * @returns {Array} 热门话题列表
     */
    getTrendingTopics(limit) {
        // 按出现次数排序
        this.trendingTopics.sort((a, b) => b.count - a.count);
        
        return this.trendingTopics.slice(0, limit);
    }
    
    /**
     * 更新热门话题
     * @param {string} tag - 标签
     */
    updateTrendingTopics(tag) {
        // 检查标签是否已在热门话题中
        for (const topic of this.trendingTopics) {
            if (topic.tag === tag) {
                topic.count++;
                topic.lastUpdated = new Date();
                return;
            }
        }
        
        // 添加新话题
        this.trendingTopics.push({
            tag,
            count: 1,
            lastUpdated: new Date()
        });
    }
    
    /**
     * 添加通知
     * @param {string} userId - 用户ID
     * @param {string} notificationType - 通知类型
     * @param {string} content - 通知内容
     * @param {string} referenceId - 相关内容ID
     * @returns {string} 通知ID
     */
    addNotification(userId, notificationType, content, referenceId) {
        if (!this.users.has(userId)) {
            return null;
        }
        
        const notificationId = this.generateId('notification');
        const notification = {
            notificationId,
            type: notificationType,
            content,
            referenceId,
            read: false,
            createdAt: new Date()
        };
        
        if (!this.notifications.has(userId)) {
            this.notifications.set(userId, []);
        }
        
        this.notifications.get(userId).push(notification);
        this.users.get(userId).notificationIds.push(notificationId);
        
        return notificationId;
    }
    
    /**
     * 标记通知为已读
     * @param {string} userId - 用户ID
     * @param {string} notificationId - 通知ID
     * @returns {boolean} 标记成功返回true，否则返回false
     */
    markNotificationAsRead(userId, notificationId) {
        if (!this.users.has(userId) || !this.notifications.has(userId)) {
            return false;
        }
        
        for (const notification of this.notifications.get(userId)) {
            if (notification.notificationId === notificationId) {
                notification.read = true;
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 获取用户通知
     * @param {string} userId - 用户ID
     * @param {number} limit - 返回通知数量限制
     * @param {boolean} unreadOnly - 是否只返回未读通知
     * @returns {Array} 通知列表
     */
    getUserNotifications(userId, limit, unreadOnly) {
        if (!this.users.has(userId) || !this.notifications.has(userId)) {
            return [];
        }
        
        let notifications = this.notifications.get(userId);
        
        if (unreadOnly) {
            notifications = notifications.filter(n => !n.read);
        }
        
        // 按创建时间降序排序
        notifications.sort((a, b) => b.createdAt - a.createdAt);
        
        // 限制返回数量
        return notifications.slice(0, limit);
    }
    
    /**
     * 生成唯一ID
     * @param {string} prefix - ID前缀
     * @returns {string} 唯一ID字符串
     */
    generateId(prefix) {
        return `${prefix}_${Math.random().toString(36).substring(2, 10)}`;
    }
}