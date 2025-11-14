class SocialMediaPlatform:
    """
    社交媒体平台类，用于管理用户、帖子、评论和社交关系
    """
    
    def __init__(self):
        self.users = {}  # 用户ID到用户信息的映射
        self.posts = {}  # 帖子ID到帖子信息的映射
        self.comments = {}  # 评论ID到评论信息的映射
        self.likes = {}  # 点赞记录
        self.friendships = {}  # 好友关系
        self.notifications = {}  # 通知记录
        self.trending_topics = []  # 热门话题
    
    def create_user(self, user_id, username, email, password, profile_info=None):
        """
        创建新用户
        @param user_id: 用户唯一标识符
        @param username: 用户名
        @param email: 电子邮件地址
        @param password: 密码
        @param profile_info: 个人资料信息（可选）
        @return: 创建成功返回True，否则返回False
        """
        if user_id in self.users:
            return False
        
        if any(user['email'] == email for user in self.users.values()):
            return False
        
        if any(user['username'] == username for user in self.users.values()):
            return False
        
        self.users[user_id] = {
            'username': username,
            'email': email,
            'password': password,  # 实际应用中应该使用哈希存储密码
            'profile_info': profile_info or {},
            'friends': [],
            'posts': [],
            'notifications': [],
            'created_at': self.get_current_timestamp()
        }
        return True
    
    def authenticate_user(self, email, password):
        """
        验证用户登录
        @param email: 电子邮件地址
        @param password: 密码
        @return: 验证成功返回用户ID，否则返回None
        """
        for user_id, user_info in self.users.items():
            if user_info['email'] == email and user_info['password'] == password:
                return user_id
        return None
    
    def create_post(self, user_id, content, media_urls=None, tags=None):
        """
        创建新帖子
        @param user_id: 用户ID
        @param content: 帖子内容
        @param media_urls: 媒体URL列表（可选）
        @param tags: 标签列表（可选）
        @return: 创建成功返回帖子ID，否则返回None
        """
        if user_id not in self.users:
            return None
        
        post_id = self.generate_id('post')
        self.posts[post_id] = {
            'user_id': user_id,
            'content': content,
            'media_urls': media_urls or [],
            'tags': tags or [],
            'likes': 0,
            'comments': [],
            'created_at': self.get_current_timestamp()
        }
        
        # 更新用户的帖子列表
        self.users[user_id]['posts'].append(post_id)
        
        # 更新热门话题
        if tags:
            for tag in tags:
                self.update_trending_topics(tag)
        
        return post_id
    
    def add_comment(self, user_id, post_id, content):
        """
        添加评论
        @param user_id: 用户ID
        @param post_id: 帖子ID
        @param content: 评论内容
        @return: 评论成功返回评论ID，否则返回None
        """
        if user_id not in self.users or post_id not in self.posts:
            return None
        
        comment_id = self.generate_id('comment')
        self.comments[comment_id] = {
            'user_id': user_id,
            'post_id': post_id,
            'content': content,
            'likes': 0,
            'created_at': self.get_current_timestamp()
        }
        
        # 更新帖子的评论列表
        self.posts[post_id]['comments'].append(comment_id)
        
        # 通知帖子作者
        post_author_id = self.posts[post_id]['user_id']
        if post_author_id != user_id:
            self.add_notification(
                user_id=post_author_id,
                notification_type='comment',
                content=f"{self.users[user_id]['username']} 评论了你的帖子",
                reference_id=post_id
            )
        
        return comment_id
    
    def like_post(self, user_id, post_id):
        """
        点赞帖子
        @param user_id: 用户ID
        @param post_id: 帖子ID
        @return: 点赞成功返回True，否则返回False
        """
        if user_id not in self.users or post_id not in self.posts:
            return False
        
        like_key = f"{user_id}_{post_id}"
        if like_key in self.likes:
            return False  # 已经点赞过
        
        self.likes[like_key] = {
            'user_id': user_id,
            'post_id': post_id,
            'created_at': self.get_current_timestamp()
        }
        
        self.posts[post_id]['likes'] += 1
        
        # 通知帖子作者
        post_author_id = self.posts[post_id]['user_id']
        if post_author_id != user_id:
            self.add_notification(
                user_id=post_author_id,
                notification_type='like',
                content=f"{self.users[user_id]['username']} 点赞了你的帖子",
                reference_id=post_id
            )
        
        return True
    
    def unlike_post(self, user_id, post_id):
        """
        取消点赞帖子
        @param user_id: 用户ID
        @param post_id: 帖子ID
        @return: 取消成功返回True，否则返回False
        """
        if user_id not in self.users or post_id not in self.posts:
            return False
        
        like_key = f"{user_id}_{post_id}"
        if like_key not in self.likes:
            return False  # 没有点赞过
        
        del self.likes[like_key]
        self.posts[post_id]['likes'] -= 1
        
        return True
    
    def send_friend_request(self, sender_id, receiver_id):
        """
        发送好友请求
        @param sender_id: 发送者用户ID
        @param receiver_id: 接收者用户ID
        @return: 发送成功返回True，否则返回False
        """
        if sender_id not in self.users or receiver_id not in self.users:
            return False
        
        if sender_id == receiver_id:
            return False
        
        friendship_key = f"{sender_id}_{receiver_id}"
        if friendship_key in self.friendships:
            return False  # 已经发送过请求或已经是好友
        
        self.friendships[friendship_key] = {
            'user1_id': sender_id,
            'user2_id': receiver_id,
            'status': 'pending',  # pending, accepted, rejected
            'created_at': self.get_current_timestamp()
        }
        
        # 通知接收者
        self.add_notification(
            user_id=receiver_id,
            notification_type='friend_request',
            content=f"{self.users[sender_id]['username']} 向你发送了好友请求",
            reference_id=friendship_key
        )
        
        return True
    
    def accept_friend_request(self, sender_id, receiver_id):
        """
        接受好友请求
        @param sender_id: 发送者用户ID
        @param receiver_id: 接收者用户ID
        @return: 接受成功返回True，否则返回False
        """
        friendship_key = f"{sender_id}_{receiver_id}"
        if friendship_key not in self.friendships:
            return False
        
        if self.friendships[friendship_key]['status'] != 'pending':
            return False
        
        self.friendships[friendship_key]['status'] = 'accepted'
        self.friendships[friendship_key]['updated_at'] = self.get_current_timestamp()
        
        # 更新用户的好友列表
        self.users[sender_id]['friends'].append(receiver_id)
        self.users[receiver_id]['friends'].append(sender_id)
        
        # 通知发送者
        self.add_notification(
            user_id=sender_id,
            notification_type='friend_accepted',
            content=f"{self.users[receiver_id]['username']} 接受了你的好友请求",
            reference_id=friendship_key
        )
        
        return True
    
    def reject_friend_request(self, sender_id, receiver_id):
        """
        拒绝好友请求
        @param sender_id: 发送者用户ID
        @param receiver_id: 接收者用户ID
        @return: 拒绝成功返回True，否则返回False
        """
        friendship_key = f"{sender_id}_{receiver_id}"
        if friendship_key not in self.friendships:
            return False
        
        if self.friendships[friendship_key]['status'] != 'pending':
            return False
        
        self.friendships[friendship_key]['status'] = 'rejected'
        self.friendships[friendship_key]['updated_at'] = self.get_current_timestamp()
        
        # 通知发送者
        self.add_notification(
            user_id=sender_id,
            notification_type='friend_rejected',
            content=f"{self.users[receiver_id]['username']} 拒绝了你的好友请求",
            reference_id=friendship_key
        )
        
        return True
    
    def remove_friend(self, user_id, friend_id):
        """
        移除好友
        @param user_id: 用户ID
        @param friend_id: 好友ID
        @return: 移除成功返回True，否则返回False
        """
        if user_id not in self.users or friend_id not in self.users:
            return False
        
        if friend_id not in self.users[user_id]['friends']:
            return False
        
        # 从用户的好友列表中移除
        self.users[user_id]['friends'].remove(friend_id)
        self.users[friend_id]['friends'].remove(user_id)
        
        # 删除好友关系记录
        friendship_key1 = f"{user_id}_{friend_id}"
        friendship_key2 = f"{friend_id}_{user_id}"
        
        if friendship_key1 in self.friendships:
            del self.friendships[friendship_key1]
        if friendship_key2 in self.friendships:
            del self.friendships[friendship_key2]
        
        return True
    
    def get_user_feed(self, user_id, limit=10):
        """
        获取用户动态流
        @param user_id: 用户ID
        @param limit: 返回帖子数量限制
        @return: 帖子列表
        """
        if user_id not in self.users:
            return []
        
        # 获取用户和好友的帖子
        user_and_friends = [user_id] + self.users[user_id]['friends']
        
        feed_posts = []
        for post_id, post_info in self.posts.items():
            if post_info['user_id'] in user_and_friends:
                post_copy = post_info.copy()
                post_copy['post_id'] = post_id
                post_copy['author'] = self.users[post_info['user_id']]['username']
                feed_posts.append(post_copy)
        
        # 按创建时间降序排序
        feed_posts.sort(key=lambda x: x['created_at'], reverse=True)
        
        # 限制返回数量
        if len(feed_posts) > limit:
            feed_posts = feed_posts[:limit]
        
        return feed_posts
    
    def search_posts(self, keyword):
        """
        搜索帖子
        @param keyword: 搜索关键词
        @return: 匹配的帖子列表
        """
        keyword = keyword.lower()
        result = []
        
        for post_id, post_info in self.posts.items():
            if (keyword in post_info['content'].lower() or
                any(keyword in tag.lower() for tag in post_info['tags'])):
                post_copy = post_info.copy()
                post_copy['post_id'] = post_id
                post_copy['author'] = self.users[post_info['user_id']]['username']
                result.append(post_copy)
        
        return result
    
    def get_trending_topics(self, limit=10):
        """
        获取热门话题
        @param limit: 返回话题数量限制
        @return: 热门话题列表
        """
        return self.trending_topics[:limit]
    
    def update_trending_topics(self, tag):
        """
        更新热门话题
        @param tag: 标签
        """
        # 检查标签是否已在热门话题中
        for topic in self.trending_topics:
            if topic['tag'] == tag:
                topic['count'] += 1
                return
        
        # 添加新话题
        self.trending_topics.append({
            'tag': tag,
            'count': 1,
            'last_updated': self.get_current_timestamp()
        })
        
        # 按出现次数排序
        self.trending_topics.sort(key=lambda x: x['count'], reverse=True)
    
    def add_notification(self, user_id, notification_type, content, reference_id=None):
        """
        添加通知
        @param user_id: 用户ID
        @param notification_type: 通知类型
        @param content: 通知内容
        @param reference_id: 相关内容ID（可选）
        @return: 通知ID
        """
        if user_id not in self.users:
            return None
        
        notification_id = self.generate_id('notification')
        notification = {
            'id': notification_id,
            'type': notification_type,
            'content': content,
            'reference_id': reference_id,
            'read': False,
            'created_at': self.get_current_timestamp()
        }
        
        if user_id not in self.notifications:
            self.notifications[user_id] = []
        
        self.notifications[user_id].append(notification)
        self.users[user_id]['notifications'].append(notification_id)
        
        return notification_id
    
    def mark_notification_as_read(self, user_id, notification_id):
        """
        标记通知为已读
        @param user_id: 用户ID
        @param notification_id: 通知ID
        @return: 标记成功返回True，否则返回False
        """
        if user_id not in self.users:
            return False
        
        if user_id not in self.notifications:
            return False
        
        for notification in self.notifications[user_id]:
            if notification['id'] == notification_id:
                notification['read'] = True
                return True
        
        return False
    
    def get_user_notifications(self, user_id, limit=10, unread_only=False):
        """
        获取用户通知
        @param user_id: 用户ID
        @param limit: 返回通知数量限制
        @param unread_only: 是否只返回未读通知
        @return: 通知列表
        """
        if user_id not in self.users:
            return []
        
        if user_id not in self.notifications:
            return []
        
        notifications = self.notifications[user_id]
        
        if unread_only:
            notifications = [n for n in notifications if not n['read']]
        
        # 按创建时间降序排序
        notifications.sort(key=lambda x: x['created_at'], reverse=True)
        
        # 限制返回数量
        if len(notifications) > limit:
            notifications = notifications[:limit]
        
        return notifications
    
    def generate_id(self, prefix):
        """
        生成唯一ID
        @param prefix: ID前缀
        @return: 唯一ID字符串
        """
        import uuid
        return f"{prefix}_{uuid.uuid4().hex[:8]}"
    
    def get_current_timestamp(self):
        """
        获取当前时间戳
        @return: 当前时间戳字符串
        """
        import datetime
        return datetime.datetime.now().isoformat()