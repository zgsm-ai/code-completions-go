#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <functional>
#include <cmath>
#include <fstream>
#include <sstream>
#include <random>

/**
 * Social Network Simulation
 * 
 * This C++ program implements a basic social network system with users,
 * posts, comments, likes, and friend relationships.
 */

// Forward declarations
class User;
class Post;
class Comment;

// Utility functions
std::string generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(100000, 999999);
    return std::to_string(dis(gen));
}

std::time_t getCurrentTime() {
    return std::time(nullptr);
}

std::string timeToString(std::time_t time) {
    std::tm* tm = std::localtime(&time);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return buffer;
}

// Like class
class Like {
private:
    std::string id;
    std::shared_ptr<User> user;
    std::time_t timestamp;
    
public:
    Like(std::shared_ptr<User> user) 
        : id(generateId()), user(user), timestamp(getCurrentTime()) {}
    
    std::string getId() const { return id; }
    std::shared_ptr<User> getUser() const { return user; }
    std::time_t getTimestamp() const { return timestamp; }
    
    std::string getTimestampString() const {
        return timeToString(timestamp);
    }
};

// Comment class
class Comment {
private:
    std::string id;
    std::shared_ptr<User> author;
    std::string content;
    std::time_t timestamp;
    std::vector<std::shared_ptr<Like>> likes;
    std::vector<std::shared_ptr<Comment>> replies;
    std::shared_ptr<Comment> parentComment; // For nested comments
    
public:
    Comment(std::shared_ptr<User> author, const std::string& content, 
            std::shared_ptr<Comment> parent = nullptr)
        : id(generateId()), author(author), content(content), 
          timestamp(getCurrentTime()), parentComment(parent) {}
    
    std::string getId() const { return id; }
    std::shared_ptr<User> getAuthor() const { return author; }
    std::string getContent() const { return content; }
    std::time_t getTimestamp() const { return timestamp; }
    std::vector<std::shared_ptr<Comment>> getReplies() const { return replies; }
    std::shared_ptr<Comment> getParent() const { return parentComment; }
    
    void setContent(const std::string& newContent) {
        content = newContent;
    }
    
    // Like management
    void addLike(std::shared_ptr<User> user) {
        // Check if user already liked this comment
        for (const auto& like : likes) {
            if (like->getUser()->getId() == user->getId()) {
                return; // User already liked
            }
        }
        
        likes.push_back(std::make_shared<Like>(user));
    }
    
    void removeLike(std::shared_ptr<User> user) {
        likes.erase(
            std::remove_if(likes.begin(), likes.end(), 
                           [user](const std::shared_ptr<Like>& like) {
                               return like->getUser()->getId() == user->getId();
                           }),
            likes.end()
        );
    }
    
    int getLikeCount() const { return likes.size(); }
    
    // Reply management
    std::shared_ptr<Comment> addReply(std::shared_ptr<User> author, const std::string& content) {
        auto reply = std::make_shared<Comment>(author, content, shared_from_this());
        replies.push_back(reply);
        return reply;
    }
    
    void removeReply(const std::string& commentId) {
        replies.erase(
            std::remove_if(replies.begin(), replies.end(),
                           [commentId](const std::shared_ptr<Comment>& comment) {
                               return comment->getId() == commentId;
                           }),
            replies.end()
        );
    }
    
    // Display methods
    void displayDetails(int indent = 0) const {
        std::string indentStr(indent * 2, ' ');
        std::cout << indentStr << author->getName() << " (" << timeToString(timestamp) << "):" << std::endl;
        std::cout << indentStr << content << std::endl;
        std::cout << indentStr << "Likes: " << likes.size() << " | Replies: " << replies.size() << std::endl;
        
        // Display replies recursively
        for (const auto& reply : replies) {
            std::cout << std::endl;
            reply->displayDetails(indent + 1);
        }
    }
};

// Enable shared_from_this for Comment
class Comment : public std::enable_shared_from_this<Comment> {
    // Rest of the Comment class implementation remains the same
    // ...
};

// Post class
class Post : public std::enable_shared_from_this<Post> {
private:
    std::string id;
    std::shared_ptr<User> author;
    std::string content;
    std::time_t timestamp;
    std::vector<std::shared_ptr<Like>> likes;
    std::vector<std::shared_ptr<Comment>> comments;
    std::vector<std::string> tags;
    bool isPublic;
    
public:
    Post(std::shared_ptr<User> author, const std::string& content, bool publicPost = true)
        : id(generateId()), author(author), content(content), 
          timestamp(getCurrentTime()), isPublic(publicPost) {}
    
    std::string getId() const { return id; }
    std::shared_ptr<User> getAuthor() const { return author; }
    std::string getContent() const { return content; }
    std::time_t getTimestamp() const { return timestamp; }
    bool getIsPublic() const { return isPublic; }
    
    void setContent(const std::string& newContent) {
        content = newContent;
    }
    
    void setVisibility(bool isPublicPost) {
        isPublic = isPublicPost;
    }
    
    // Like management
    void addLike(std::shared_ptr<User> user) {
        // Check if user already liked this post
        for (const auto& like : likes) {
            if (like->getUser()->getId() == user->getId()) {
                return; // User already liked
            }
        }
        
        likes.push_back(std::make_shared<Like>(user));
    }
    
    void removeLike(std::shared_ptr<User> user) {
        likes.erase(
            std::remove_if(likes.begin(), likes.end(), 
                           [user](const std::shared_ptr<Like>& like) {
                               return like->getUser()->getId() == user->getId();
                           }),
            likes.end()
        );
    }
    
    int getLikeCount() const { return likes.size(); }
    
    // Comment management
    std::shared_ptr<Comment> addComment(std::shared_ptr<User> author, const std::string& content) {
        auto comment = std::make_shared<Comment>(author, content);
        comments.push_back(comment);
        return comment;
    }
    
    void removeComment(const std::string& commentId) {
        comments.erase(
            std::remove_if(comments.begin(), comments.end(),
                           [commentId](const std::shared_ptr<Comment>& comment) {
                               return comment->getId() == commentId;
                           }),
            comments.end()
        );
    }
    
    int getCommentCount() const { return comments.size(); }
    
    // Tag management
    void addTag(const std::string& tag) {
        if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
        }
    }
    
    void removeTag(const std::string& tag) {
        tags.erase(
            std::remove(tags.begin(), tags.end(), tag),
            tags.end()
        );
    }
    
    std::vector<std::string> getTags() const { return tags; }
    
    // Display methods
    void displayDetails() const {
        std::cout << "Post ID: " << id << std::endl;
        std::cout << "Author: " << author->getName() << std::endl;
        std::cout << "Timestamp: " << timeToString(timestamp) << std::endl;
        std::cout << "Visibility: " << (isPublic ? "Public" : "Private") << std::endl;
        std::cout << "Content: " << content << std::endl;
        
        if (!tags.empty()) {
            std::cout << "Tags: ";
            for (const auto& tag : tags) {
                std::cout << "#" << tag << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "Likes: " << likes.size() << " | Comments: " << comments.size() << std::endl;
        
        // Display comments
        if (!comments.empty()) {
            std::cout << "\nComments:" << std::endl;
            for (const auto& comment : comments) {
                std::cout << "  - ";
                comment->displayDetails(1);
                std::cout << std::endl;
            }
        }
    }
    
    std::string getSummary() const {
        return author->getName() + " posted: " + 
               (content.length() > 50 ? content.substr(0, 47) + "..." : content) + 
               " (" + std::to_string(likes.size()) + " likes, " + 
               std::to_string(comments.size()) + " comments)";
    }
};

// User class
class User : public std::enable_shared_from_this<User> {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string bio;
    std::time_t joinDate;
    std::vector<std::shared_ptr<User>> friends;
    std::vector<std::shared_ptr<Post>> posts;
    std::unordered_map<std::string, std::shared_ptr<User>> friendRequests;
    std::map<std::string, std::string> notifications;
    
public:
    User(const std::string& name, const std::string& email, const std::string& bio = "")
        : id(generateId()), name(name), email(email), bio(bio), 
          joinDate(getCurrentTime()) {}
    
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getBio() const { return bio; }
    std::time_t getJoinDate() const { return joinDate; }
    
    void setName(const std::string& newName) { name = newName; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setBio(const std::string& newBio) { bio = newBio; }
    
    // Friend management
    bool sendFriendRequest(std::shared_ptr<User> user) {
        if (isFriendWith(user) || hasSentFriendRequestTo(user)) {
            return false;
        }
        
        user->friendRequests[id] = name;
        addNotification("friend_request", name + " sent you a friend request");
        return true;
    }
    
    bool acceptFriendRequest(const std::string& userId) {
        if (friendRequests.find(userId) != friendRequests.end()) {
            std::string friendName = friendRequests[userId];
            
            // In a real system, we would look up the user by ID
            // For simplicity, we'll just remove the request
            friendRequests.erase(userId);
            
            addNotification("friend_accepted", "You accepted " + friendName + "'s friend request");
            return true;
        }
        return false;
    }
    
    bool rejectFriendRequest(const std::string& userId) {
        if (friendRequests.find(userId) != friendRequests.end()) {
            friendRequests.erase(userId);
            return true;
        }
        return false;
    }
    
    bool isFriendWith(std::shared_ptr<User> user) {
        return std::find(friends.begin(), friends.end(), user) != friends.end();
    }
    
    bool hasSentFriendRequestTo(std::shared_ptr<User> user) {
        return user->friendRequests.find(id) != user->friendRequests.end();
    }
    
    // Post management
    std::shared_ptr<Post> createPost(const std::string& content, bool isPublic = true) {
        auto post = std::make_shared<Post>(shared_from_this(), content, isPublic);
        posts.push_back(post);
        return post;
    }
    
    void deletePost(const std::string& postId) {
        posts.erase(
            std::remove_if(posts.begin(), posts.end(),
                           [postId](const std::shared_ptr<Post>& post) {
                               return post->getId() == postId;
                           }),
            posts.end()
        );
    }
    
    std::vector<std::shared_ptr<Post>> getPosts() const { return posts; }
    
    // Notification management
    void addNotification(const std::string& type, const std::string& message) {
        std::string notificationId = generateId();
        notifications[notificationId] = "[" + type + "] " + message;
    }
    
    std::map<std::string, std::string> getNotifications() const { return notifications; }
    
    void clearNotifications() {
        notifications.clear();
    }
    
    // Display methods
    void displayProfile() const {
        std::cout << "User Profile:" << std::endl;
        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Bio: " << bio << std::endl;
        std::cout << "Join Date: " << timeToString(joinDate) << std::endl;
        std::cout << "Friends: " << friends.size() << std::endl;
        std::cout << "Posts: " << posts.size() << std::endl;
        std::cout << "Pending Friend Requests: " << friendRequests.size() << std::endl;
    }
    
    void displayFriends() const {
        std::cout << name << "'s Friends (" << friends.size() << "):" << std::endl;
        for (const auto& friendUser : friends) {
            std::cout << "- " << friendUser->getName() << std::endl;
        }
    }
    
    void displayPosts() const {
        std::cout << name << "'s Posts (" << posts.size() << "):" << std::endl;
        for (const auto& post : posts) {
            std::cout << "- " << post->getSummary() << std::endl;
        }
    }
    
    void displayNotifications() const {
        std::cout << name << "'s Notifications (" << notifications.size() << "):" << std::endl;
        for (const auto& pair : notifications) {
            std::cout << "- " << pair.second << std::endl;
        }
    }
};

// Social Network class
class SocialNetwork {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    std::unordered_map<std::string, std::shared_ptr<Post>> posts;
    std::vector<std::shared_ptr<Post>> feed;
    
public:
    // User management
    std::shared_ptr<User> createUser(const std::string& name, const std::string& email, const std::string& bio = "") {
        auto user = std::make_shared<User>(name, email, bio);
        users[user->getId()] = user;
        return user;
    }
    
    std::shared_ptr<User> getUser(const std::string& userId) {
        if (users.find(userId) != users.end()) {
            return users[userId];
        }
        return nullptr;
    }
    
    // Post management
    void addPost(std::shared_ptr<Post> post) {
        if (post) {
            posts[post->getId()] = post;
            feed.push_back(post);
        }
    }
    
    std::shared_ptr<Post> getPost(const std::string& postId) {
        if (posts.find(postId) != posts.end()) {
            return posts[postId];
        }
        return nullptr;
    }
    
    // Search functionality
    std::vector<std::shared_ptr<User>> searchUsers(const std::string& query) {
        std::vector<std::shared_ptr<User>> results;
        
        for (const auto& pair : users) {
            const auto& user = pair.second;
            if (user->getName().find(query) != std::string::npos ||
                user->getEmail().find(query) != std::string::npos ||
                user->getBio().find(query) != std::string::npos) {
                results.push_back(user);
            }
        }
        
        return results;
    }
    
    std::vector<std::shared_ptr<Post>> searchPosts(const std::string& query) {
        std::vector<std::shared_ptr<Post>> results;
        
        for (const auto& pair : posts) {
            const auto& post = pair.second;
            if (post->getContent().find(query) != std::string::npos) {
                results.push_back(post);
            }
        }
        
        return results;
    }
    
    std::vector<std::shared_ptr<Post>> getFeed(std::shared_ptr<User> user, int limit = 20) {
        std::vector<std::shared_ptr<Post>> userFeed;
        
        // Get posts from the user and their friends
        for (const auto& postPair : posts) {
            const auto& post = postPair.second;
            
            // Include user's own posts and friends' posts (if public)
            if (post->getAuthor()->getId() == user->getId() ||
                (post->getIsPublic() && user->isFriendWith(post->getAuthor()))) {
                userFeed.push_back(post);
            }
        }
        
        // Sort by timestamp (most recent first)
        std::sort(userFeed.begin(), userFeed.end(), 
                 [](const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) {
                     return a->getTimestamp() > b->getTimestamp();
                 });
        
        // Limit results
        if (userFeed.size() > limit) {
            userFeed.resize(limit);
        }
        
        return userFeed;
    }
    
    // Trending topics (simplified)
    std::vector<std::string> getTrendingTopics(int limit = 5) {
        std::map<std::string, int> tagCounts;
        
        // Count all tags across all posts
        for (const auto& pair : posts) {
            const auto& post = pair.second;
            for (const auto& tag : post->getTags()) {
                tagCounts[tag]++;
            }
        }
        
        // Sort tags by count
        std::vector<std::pair<std::string, int>> sortedTags(
            tagCounts.begin(), tagCounts.end()
        );
        
        std::sort(sortedTags.begin(), sortedTags.end(),
                 [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                     return a.second > b.second;
                 });
        
        // Extract just the tag names
        std::vector<std::string> result;
        for (int i = 0; i < std::min(limit, (int)sortedTags.size()); i++) {
            result.push_back(sortedTags[i].first);
        }
        
        return result;
    }
    
    // Display methods
    void displayAllUsers() const {
        std::cout << "All Users (" << users.size() << "):" << std::endl;
        for (const auto& pair : users) {
            std::cout << "- " << pair.second->getName() << " (" << pair.second->getEmail() << ")" << std::endl;
        }
    }
    
    void displayAllPosts() const {
        std::cout << "All Posts (" << posts.size() << "):" << std::endl;
        for (const auto& pair : posts) {
            std::cout << "- " << pair.second->getSummary() << std::endl;
        }
    }
    
    void displayNetworkStats() const {
        std::cout << "Social Network Statistics:" << std::endl;
        std::cout << "Total Users: " << users.size() << std::endl;
        std::cout << "Total Posts: " << posts.size() << std::endl;
        
        // Calculate total friendships
        int totalFriendships = 0;
        for (const auto& pair : users) {
            totalFriendships += pair.second->getFriends().size();
        }
        std::cout << "Total Friendships: " << totalFriendships / 2 << std::endl; // Divided by 2 since each friendship is counted twice
        
        // Calculate total likes and comments
        int totalLikes = 0;
        int totalComments = 0;
        for (const auto& pair : posts) {
            totalLikes += pair.second->getLikeCount();
            totalComments += pair.second->getCommentCount();
        }
        std::cout << "Total Likes: " << totalLikes << std::endl;
        std::cout << "Total Comments: " << totalComments << std::endl;
    }
};

int main() {
    // Create social network
    SocialNetwork network;
    
    // Create users
    auto john = network.createUser("John Smith", "john@example.com", "Software developer who loves hiking");
    auto jane = network.createUser("Jane Doe", "jane@example.com", "Artist and photographer");
    auto bob = network.createUser("Bob Johnson", "bob@example.com", "Fitness enthusiast");
    auto alice = network.createUser("Alice Williams", "alice@example.com", "Travel blogger and foodie");
    
    // Build friend connections
    john->sendFriendRequest(jane);
    john->sendFriendRequest(bob);
    jane->sendFriendRequest(alice);
    
    jane->acceptFriendRequest(john->getId());
    bob->acceptFriendRequest(john->getId());
    alice->acceptFriendRequest(jane->getId());
    
    // Create posts
    auto johnPost1 = john->createPost("Just finished a 10-mile hike today! The views were amazing. #hiking #nature");
    johnPost1->addTag("hiking");
    johnPost1->addTag("nature");
    
    auto janePost1 = jane->createPost("New painting is complete! Check out my latest work. #art #painting");
    janePost1->addTag("art");
    janePost1->addTag("painting");
    
    auto bobPost1 = bob->createPost("Morning workout done! 5K run in under 25 minutes. #fitness #running");
    bobPost1->addTag("fitness");
    bobPost1->addTag("running");
    
    auto alicePost1 = alice->createPost("Trying a new recipe tonight. Italian pasta with homemade sauce! #food #cooking");
    alicePost1->addTag("food");
    alicePost1->addTag("cooking");
    
    // Add posts to network
    network.addPost(johnPost1);
    network.addPost(janePost1);
    network.addPost(bobPost1);
    network.addPost(alicePost1);
    
    // Create interactions
    jane->addLike(johnPost1);
    bob->addLike(johnPost1);
    john->addLike(janePost1);
    alice->addLike(janePost1);
    
    // Add comments
    auto comment1 = johnPost1->addComment(jane, "Great job! Which trail did you take?");
    johnPost1->addComment(bob, "Inspiring! I need to start hiking more.");
    
    auto comment2 = janePost1->addComment(john, "Beautiful work! What's the title?");
    janePost1->addComment(alice, "Love the colors!");
    
    // Add replies to comments
    comment1->addReply(john, "Thanks! It was the North Ridge Trail.");
    comment2->addReply(jane, "Thanks! It's called 'Summer Sunset'");
    
    // More posts
    auto johnPost2 = john->createPost("Working on a new open source project. Check it out on GitHub! #coding #opensource");
    johnPost2->addTag("coding");
    johnPost2->addTag("opensource");
    
    network.addPost(johnPost2);
    
    alice->addLike(johnPost2);
    bob->addLike(johnPost2);
    
    // Display network information
    std::cout << "=== Social Network Simulation ===" << std::endl;
    network.displayAllUsers();
    std::cout << std::endl;
    
    // Display user profiles
    std::cout << "\n=== User Profiles ===" << std::endl;
    john->displayProfile();
    std::cout << std::endl;
    
    // Display posts
    std::cout << "\n=== All Posts ===" << std::endl;
    network.displayAllPosts();
    std::cout << std::endl;
    
    // Display a detailed post with comments
    std::cout << "\n=== Detailed Post ===" << std::endl;
    johnPost1->displayDetails();
    std::cout << std::endl;
    
    // Display user feed
    std::cout << "\n=== John's Feed ===" << std::endl;
    auto johnFeed = network.getFeed(john, 10);
    for (const auto& post : johnFeed) {
        std::cout << "- " << post->getSummary() << std::endl;
    }
    std::cout << std::endl;
    
    // Search functionality
    std::cout << "\n=== Search Results ===" << std::endl;
    auto artPosts = network.searchPosts("art");
    std::cout << "Posts containing 'art':" << std::endl;
    for (const auto& post : artPosts) {
        std::cout << "- " << post->getSummary() << std::endl;
    }
    std::cout << std::endl;
    
    // Trending topics
    std::cout << "\n=== Trending Topics ===" << std::endl;
    auto trending = network.getTrendingTopics();
    std::cout << "Trending tags:" << std::endl;
    for (const auto& tag : trending) {
        std::cout << "- #" << tag << std::endl;
    }
    std::cout << std::endl;
    
    // Display network statistics
    <｜fim▁hole›>network.displayNetworkStats();
    
    // Display user notifications
    std::cout << "\n=== User Notifications ===" << std::endl;
    jane->displayNotifications();
    std::cout << std::endl;
    
    return 0;
}