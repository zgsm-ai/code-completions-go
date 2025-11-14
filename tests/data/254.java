import java.util.*;
import java.time.*;

/**
 * 社交媒体平台类
 * 用于管理用户、帖子、评论、点赞和社交关系
 */
public class SocialMediaPlatform {
    private Map<String, User> users;
    private Map<String, Post> posts;
    private Map<String, Comment> comments;
    private Map<String, Like> likes;
    private Map<String, FriendRequest> friendRequests;
    private Map<String, Friendship> friendships;
    private Map<String, Notification> notifications;
    private List<TrendingTopic> trendingTopics;
    
    /**
     * 社交媒体平台构造函数
     */
    public SocialMediaPlatform() {
        this.users = new HashMap<>();
        this.posts = new HashMap<>();
        this.comments = new HashMap<>();
        this.likes = new HashMap<>();
        this.friendRequests = new HashMap<>();
        this.friendships = new HashMap<>();
        this.notifications = new HashMap<>();
        this.trendingTopics = new ArrayList<>();
    }
    
    /**
     * 创建新用户
     * @param userId 用户唯一标识符
     * @param username 用户名
     * @param email 电子邮件
     * @param password 密码
     * @param profileInfo 个人资料信息（可选）
     * @return 创建成功返回true，否则返回false
     */
    public boolean createUser(String userId, String username, String email, 
                              String password, Map<String, String> profileInfo) {
        if (users.containsKey(userId)) {
            return false;
        }
        
        if (users.values().stream().anyMatch(u -> u.getEmail().equals(email))) {
            return false;
        }
        
        if (users.values().stream().anyMatch(u -> u.getUsername().equals(username))) {
            return false;
        }
        
        User user = new User(userId, username, email, password, profileInfo);
        users.put(userId, user);
        return true;
    }
    
    /**
     * 验证用户登录
     * @param email 电子邮件
     * @param password 密码
     * @return 验证成功返回用户ID，否则返回null
     */
    public String authenticateUser(String email, String password) {
        for (Map.Entry<String, User> entry : users.entrySet()) {
            User user = entry.getValue();
            if (user.getEmail().equals(email) && user.getPassword().equals(password)) {
                return entry.getKey();
            }
        }
        return null;
    }
    
    /**
     * 创建新帖子
     * @param userId 用户ID
     * @param content 帖子内容
     * @param mediaUrls 媒体URL列表（可选）
     * @param tags 标签列表（可选）
     * @return 创建成功返回帖子ID，否则返回null
     */
    public String createPost(String userId, String content, List<String> mediaUrls, List<String> tags) {
        if (!users.containsKey(userId)) {
            return null;
        }
        
        String postId = generateId("post");
        Post post = new Post(postId, userId, content, mediaUrls, tags);
        posts.put(postId, post);
        
        // 更新用户的帖子列表
        users.get(userId).addPost(postId);
        
        // 更新热门话题
        if (tags != null) {
            for (String tag : tags) {
                updateTrendingTopics(tag);
            }
        }
        
        return postId;
    }
    
    /**
     * 添加评论
     * @param userId 用户ID
     * @param postId 帖子ID
     * @param content 评论内容
     * @return 评论成功返回评论ID，否则返回null
     */
    public String addComment(String userId, String postId, String content) {
        if (!users.containsKey(userId) || !posts.containsKey(postId)) {
            return null;
        }
        
        String commentId = generateId("comment");
        Comment comment = new Comment(commentId, userId, postId, content);
        comments.put(commentId, comment);
        
        // 更新帖子的评论列表
        posts.get(postId).addComment(commentId);
        
        // 通知帖子作者
        String postAuthorId = posts.get(postId).getUserId();
        if (!postAuthorId.equals(userId)) {
            addNotification(
                postAuthorId, 
                "comment", 
                users.get(userId).getUsername() + " 评论了你的帖子", 
                postId
            );
        }
        
        return commentId;
    }
    
    /**
     * 点赞帖子
     * @param userId 用户ID
     * @param postId 帖子ID
     * @return 点赞成功返回true，否则返回false
     */
    public boolean likePost(String userId, String postId) {
        if (!users.containsKey(userId) || !posts.containsKey(postId)) {
            return false;
        }
        
        String likeKey = userId + "_" + postId;
        if (likes.containsKey(likeKey)) {
            return false;  // 已经点赞过
        }
        
        Like like = new Like(generateId("like"), userId, postId);
        likes.put(likeKey, like);
        
        // 更新帖子的点赞数
        posts.get(postId).incrementLikes();
        
        // 通知帖子作者
        String postAuthorId = posts.get(postId).getUserId();
        if (!postAuthorId.equals(userId)) {
            addNotification(
                postAuthorId, 
                "like", 
                users.get(userId).getUsername() + " 点赞了你的帖子", 
                postId
            );
        }
        
        return true;
    }
    
    /**
     * 取消点赞帖子
     * @param userId 用户ID
     * @param postId 帖子ID
     * @return 取消成功返回true，否则返回false
     */
    public boolean unlikePost(String userId, String postId) {
        if (!users.containsKey(userId) || !posts.containsKey(postId)) {
            return false;
        }
        
        String likeKey = userId + "_" + postId;
        if (!likes.containsKey(likeKey)) {
            return false;  // 没有点赞过
        }
        
        likes.remove(likeKey);
        posts.get(postId).decrementLikes();
        
        return true;
    }
    
    /**
     * 发送好友请求
     * @param senderId 发送者用户ID
     * @param receiverId 接收者用户ID
     * @return 发送成功返回好友请求ID，否则返回null
     */
    public String sendFriendRequest(String senderId, String receiverId) {
        if (!users.containsKey(senderId) || !users.containsKey(receiverId)) {
            return null;
        }
        
        if (senderId.equals(receiverId)) {
            return null;
        }
        
        // 检查是否已经发送过请求或已经是好友
        String requestKey = senderId + "_" + receiverId;
        if (friendRequests.containsKey(requestKey) || friendships.containsKey(requestKey)) {
            return null;
        }
        
        String requestId = generateId("friend_request");
        FriendRequest request = new FriendRequest(requestId, senderId, receiverId);
        friendRequests.put(requestKey, request);
        
        // 通知接收者
        addNotification(
            receiverId, 
            "friend_request", 
            users.get(senderId).getUsername() + " 向你发送了好友请求", 
            requestId
        );
        
        return requestId;
    }
    
    /**
     * 接受好友请求
     * @param requestId 好友请求ID
     * @return 接受成功返回true，否则返回false
     */
    public boolean acceptFriendRequest(String requestId) {
        if (!friendRequests.containsKey(requestId)) {
            return false;
        }
        
        FriendRequest request = friendRequests.get(requestId);
        if (!request.getStatus().equals("pending")) {
            return false;
        }
        
        // 更新请求状态
        request.setStatus("accepted");
        
        // 创建好友关系
        String friendshipKey = request.getSenderId() + "_" + request.getReceiverId();
        Friendship friendship = new Friendship(generateId("friendship"), 
                                            request.getSenderId(), 
                                            request.getReceiverId());
        friendships.put(friendshipKey, friendship);
        
        // 更新用户的好友列表
        users.get(request.getSenderId()).addFriend(request.getReceiverId());
        users.get(request.getReceiverId()).addFriend(request.getSenderId());
        
        // 通知发送者
        addNotification(
            request.getSenderId(), 
            "friend_accepted", 
            users.get(request.getReceiverId()).getUsername() + " 接受了你的好友请求", 
            requestId
        );
        
        return true;
    }
    
    /**
     * 拒绝好友请求
     * @param requestId 好友请求ID
     * @return 拒绝成功返回true，否则返回false
     */
    public boolean rejectFriendRequest(String requestId) {
        if (!friendRequests.containsKey(requestId)) {
            return false;
        }
        
        FriendRequest request = friendRequests.get(requestId);
        if (!request.getStatus().equals("pending")) {
            return false;
        }
        
        // 更新请求状态
        request.setStatus("rejected");
        
        // 通知发送者
        addNotification(
            request.getSenderId(), 
            "friend_rejected", 
            users.get(request.getReceiverId()).getUsername() + " 拒绝了你的好友请求", 
            requestId
        );
        
        return true;
    }
    
    /**
     * 移除好友
     * @param userId 用户ID
     * @param friendId 好友ID
     * @return 移除成功返回true，否则返回false
     */
    public boolean removeFriend(String userId, String friendId) {
        if (!users.containsKey(userId) || !users.containsKey(friendId)) {
            return false;
        }
        
        if (!users.get(userId).getFriendIds().contains(friendId)) {
            return false;
        }
        
        // 从用户的好友列表中移除
        users.get(userId).removeFriend(friendId);
        users.get(friendId).removeFriend(userId);
        
        // 删除好友关系记录
        String friendshipKey1 = userId + "_" + friendId;
        String friendshipKey2 = friendId + "_" + userId;
        
        if (friendships.containsKey(friendshipKey1)) {
            friendships.remove(friendshipKey1);
        }
        if (friendships.containsKey(friendshipKey2)) {
            friendships.remove(friendshipKey2);
        }
        
        return true;
    }
    
    /**
     * 获取用户动态流
     * @param userId 用户ID
     * @param limit 返回帖子数量限制
     * @return 帖子列表
     */
    public List<Post> getUserFeed(String userId, int limit) {
        if (!users.containsKey(userId)) {
            return new ArrayList<>();
        }
        
        // 获取用户和好友的帖子
        List<String> userAndFriends = new ArrayList<>();
        userAndFriends.add(userId);
        userAndFriends.addAll(users.get(userId).getFriendIds());
        
        List<Post> feedPosts = new ArrayList<>();
        for (Post post : posts.values()) {
            if (userAndFriends.contains(post.getUserId())) {
                feedPosts.add(post);
            }
        }
        
        // 按创建时间降序排序
        feedPosts.sort((p1, p2) -> p2.getCreatedAt().compareTo(p1.getCreatedAt()));
        
        // 限制返回数量
        if (feedPosts.size() > limit) {
            feedPosts = feedPosts.subList(0, limit);
        }
        
        return feedPosts;
    }
    
    /**
     * 搜索帖子
     * @param keyword 搜索关键词
     * @return 匹配的帖子列表
     */
    public List<Post> searchPosts(String keyword) {
        keyword = keyword.toLowerCase();
        List<Post> result = new ArrayList<>();
        
        for (Post post : posts.values()) {
            if (post.getContent().toLowerCase().contains(keyword) ||
                (post.getTags() != null && post.getTags().stream().anyMatch(t -> t.toLowerCase().contains(keyword)))) {
                result.add(post);
            }
        }
        
        return result;
    }
    
    /**
     * 获取热门话题
     * @param limit 返回话题数量限制
     * @return 热门话题列表
     */
    public List<TrendingTopic> getTrendingTopics(int limit) {
        // 按出现次数排序
        trendingTopics.sort((t1, t2) -> t2.getCount() - t1.getCount());
        
        if (trendingTopics.size() > limit) {
            return trendingTopics.subList(0, limit);
        }
        
        return new ArrayList<>(trendingTopics);
    }
    
    /**
     * 更新热门话题
     * @param tag 标签
     */
    private void updateTrendingTopics(String tag) {
        // 检查标签是否已在热门话题中
        for (TrendingTopic topic : trendingTopics) {
            if (topic.getTag().equals(tag)) {
                topic.incrementCount();
                return;
            }
        }
        
        // 添加新话题
        trendingTopics.add(new TrendingTopic(tag, 1, LocalDateTime.now()));
    }
    
    /**
     * 添加通知
     * @param userId 用户ID
     * @param notificationType 通知类型
     * @param content 通知内容
     * @param referenceId 相关内容ID
     * @return 通知ID
     */
    private String addNotification(String userId, String notificationType, String content, String referenceId) {
        if (!users.containsKey(userId)) {
            return null;
        }
        
        String notificationId = generateId("notification");
        Notification notification = new Notification(notificationId, notificationType, content, referenceId);
        
        if (!notifications.containsKey(userId)) {
            notifications.put(userId, new ArrayList<>());
        }
        
        notifications.get(userId).add(notification);
        users.get(userId).addNotification(notificationId);
        
        return notificationId;
    }
    
    /**
     * 标记通知为已读
     * @param userId 用户ID
     * @param notificationId 通知ID
     * @return 标记成功返回true，否则返回false
     */
    public boolean markNotificationAsRead(String userId, String notificationId) {
        if (!users.containsKey(userId) || !notifications.containsKey(userId)) {
            return false;
        }
        
        for (Notification notification : notifications.get(userId)) {
            if (notification.getNotificationId().equals(notificationId)) {
                notification.markAsRead();
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * 获取用户通知
     * @param userId 用户ID
     * @param limit 返回通知数量限制
     * @param unreadOnly 是否只返回未读通知
     * @return 通知列表
     */
    public List<Notification> getUserNotifications(String userId, int limit, boolean unreadOnly) {
        if (!users.containsKey(userId) || !notifications.containsKey(userId)) {
            return new ArrayList<>();
        }
        
        List<Notification> result = notifications.get(userId);
        
        if (unreadOnly) {
            result = result.stream()
                          .filter(n -> !n.isRead())
                          .collect(ArrayList::new, ArrayList::add, ArrayList::addAll);
        }
        
        // 按创建时间降序排序
        result.sort((n1, n2) -> n2.getCreatedAt().compareTo(n1.getCreatedAt()));
        
        if (result.size() > limit) {
            result = result.subList(0, limit);
        }
        
        return result;
    }
    
    /**
     * 生成唯一ID
     * @param prefix ID前缀
     * @return 唯一ID字符串
     */
    private String generateId(String prefix) {
        return prefix + "_" + UUID.randomUUID().toString().substring(0, 8);
    }
}

/**
 * 用户类
 */
class User {
    private String userId;
    private String username;
    private String email;
    private String password;
    private Map<String, String> profileInfo;
    private List<String> friendIds;
    private List<String> postIds;
    private List<String> notificationIds;
    private LocalDateTime createdAt;
    
    public User(String userId, String username, String email, String password, Map<String, String> profileInfo) {
        this.userId = userId;
        this.username = username;
        this.email = email;
        this.password = password;
        this.profileInfo = profileInfo != null ? new HashMap<>(profileInfo) : new HashMap<>();
        this.friendIds = new ArrayList<>();
        this.postIds = new ArrayList<>();
        this.notificationIds = new ArrayList<>();
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getUserId() {
        return userId;
    }
    
    public String getUsername() {
        return username;
    }
    
    public String getEmail() {
        return email;
    }
    
    public String getPassword() {
        return password;
    }
    
    public Map<String, String> getProfileInfo() {
        return new HashMap<>(profileInfo);
    }
    
    public List<String> getFriendIds() {
        return new ArrayList<>(friendIds);
    }
    
    public List<String> getPostIds() {
        return new ArrayList<>(postIds);
    }
    
    public List<String> getNotificationIds() {
        return new ArrayList<>(notificationIds);
    }
    
    public void addFriend(String friendId) {
        if (!friendIds.contains(friendId)) {
            friendIds.add(friendId);
        }
    }
    
    public void removeFriend(String friendId) {
        friendIds.remove(friendId);
    }
    
    public void addPost(String postId) {
        postIds.add(postId);
    }
    
    public void addNotification(String notificationId) {
        notificationIds.add(notificationId);
    }
}

/**
 * 帖子类
 */
class Post {
    private String postId;
    private String userId;
    private String content;
    private List<String> mediaUrls;
    private List<String> tags;
    private int likes;
    private List<String> commentIds;
    private LocalDateTime createdAt;
    
    public Post(String postId, String userId, String content, List<String> mediaUrls, List<String> tags) {
        this.postId = postId;
        this.userId = userId;
        this.content = content;
        this.mediaUrls = mediaUrls != null ? new ArrayList<>(mediaUrls) : new ArrayList<>();
        this.tags = tags != null ? new ArrayList<>(tags) : new ArrayList<>();
        this.likes = 0;
        this.commentIds = new ArrayList<>();
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getPostId() {
        return postId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public String getContent() {
        return content;
    }
    
    public List<String> getMediaUrls() {
        return new ArrayList<>(mediaUrls);
    }
    
    public List<String> getTags() {
        return new ArrayList<>(tags);
    }
    
    public int getLikes() {
        return likes;
    }
    
    public List<String> getCommentIds() {
        return new ArrayList<>(commentIds);
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
    
    public void incrementLikes() {
        likes++;
    }
    
    public void decrementLikes() {
        likes = Math.max(0, likes - 1);
    }
    
    public void addComment(String commentId) {
        commentIds.add(commentId);
    }
}

/**
 * 评论类
 */
class Comment {
    private String commentId;
    private String userId;
    private String postId;
    private String content;
    private int likes;
    private LocalDateTime createdAt;
    
    public Comment(String commentId, String userId, String postId, String content) {
        this.commentId = commentId;
        this.userId = userId;
        this.postId = postId;
        this.content = content;
        this.likes = 0;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getCommentId() {
        return commentId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public String getPostId() {
        return postId;
    }
    
    public String getContent() {
        return content;
    }
    
    public int getLikes() {
        return likes;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
    
    public void incrementLikes() {
        likes++;
    }
    
    public void decrementLikes() {
        likes = Math.max(0, likes - 1);
    }
}

/**
 * 点赞类
 */
class Like {
    private String likeId;
    private String userId;
    private String postId;
    private LocalDateTime createdAt;
    
    public Like(String likeId, String userId, String postId) {
        this.likeId = likeId;
        this.userId = userId;
        this.postId = postId;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getLikeId() {
        return likeId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public String getPostId() {
        return postId;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}

/**
 * 好友请求类
 */
class FriendRequest {
    private String requestId;
    private String senderId;
    private String receiverId;
    private String status;  // pending, accepted, rejected
    private LocalDateTime createdAt;
    
    public FriendRequest(String requestId, String senderId, String receiverId) {
        this.requestId = requestId;
        this.senderId = senderId;
        this.receiverId = receiverId;
        this.status = "pending";
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getRequestId() {
        return requestId;
    }
    
    public String getSenderId() {
        return senderId;
    }
    
    public String getReceiverId() {
        return receiverId;
    }
    
    public String getStatus() {
        return status;
    }
    
    public void setStatus(String status) {
        this.status = status;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}

/**
 * 好友关系类
 */
class Friendship {
    private String friendshipId;
    private String user1Id;
    private String user2Id;
    private LocalDateTime createdAt;
    
    public Friendship(String friendshipId, String user1Id, String user2Id) {
        this.friendshipId = friendshipId;
        this.user1Id = user1Id;
        this.user2Id = user2Id;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getFriendshipId() {
        return friendshipId;
    }
    
    public String getUser1Id() {
        return user1Id;
    }
    
    public String getUser2Id() {
        return user2Id;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}

/**
 * 通知类
 */
class Notification {
    private String notificationId;
    private String type;
    private String content;
    private String referenceId;
    private boolean read;
    private LocalDateTime createdAt;
    
    public Notification(String notificationId, String type, String content, String referenceId) {
        this.notificationId = notificationId;
        this.type = type;
        this.content = content;
        this.referenceId = referenceId;
        this.read = false;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getNotificationId() {
        return notificationId;
    }
    
    public String getType() {
        return type;
    }
    
    public String getContent() {
        return content;
    }
    
    public String getReferenceId() {
        return referenceId;
    }
    
    public boolean isRead() {
        return read;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
    
    public void markAsRead() {
        this.read = true;
    }
}

/**
 * 热门话题类
 */
class TrendingTopic {
    private String tag;
    private int count;
    private LocalDateTime lastUpdated;
    
    public TrendingTopic(String tag, int count, LocalDateTime lastUpdated) {
        this.tag = tag;
        this.count = count;
        this.lastUpdated = lastUpdated;
    }
    
    // Getters and setters
    public String getTag() {
        return tag;
    }
    
    public int getCount() {
        return count;
    }
    
    public LocalDateTime getLastUpdated() {
        return lastUpdated;
    }
    
    public void incrementCount() {
        count++;
        lastUpdated = LocalDateTime.now();
    }
}