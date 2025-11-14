package main

import (
	"fmt"
	"math/rand"
	"time"
)

// Blog System
// This Go program implements a blog system with posts, comments, and tags

// User represents a blog user
type User struct {
	ID       int       `json:"id"`
	Username string    `json:"username"`
	Email    string    `json:"email"`
	Avatar   string    `json:"avatar"`
	Bio      string    `json:"bio"`
	JoinDate time.Time `json:"join_date"`
}

// Post represents a blog post
type Post struct {
	ID        int       `json:"id"`
	Title     string    `json:"title"`
	Content   string    `json:"content"`
	Author    *User     `json:"author"`
	CreatedAt time.Time `json:"created_at"`
	UpdatedAt time.Time `json:"updated_at"`
	Tags      []string  `json:"tags"`
	Published bool      `json:"published"`
}

// Comment represents a comment on a post
type Comment struct {
	ID        int       `json:"id"`
	Content   string    `json:"content"`
	Author    *User     `json:"author"`
	PostID    int       `json:"post_id"`
	CreatedAt time.Time `json:"created_at"`
	ParentID  *int      `json:"parent_id,omitempty"`
}

// Blog represents blog system
type Blog struct {
	Users    map[int]*User     `json:"users"`
	Posts    map[int]*Post     `json:"posts"`
	Comments map[int]*Comment  `json:"comments"`
	nextID   int
}

// NewBlog creates a new blog instance
func NewBlog() *Blog {
	return &Blog{
		Users:    make(map[int]*User),
		Posts:    make(map[int]*Post),
		Comments: make(map[int]*Comment),
		nextID:   1,
	}
}

// getNextID returns next available ID
func (b *Blog) getNextID() int {
	id := b.nextID
	b.nextID++
	return id
}

// AddUser adds a new user to blog
func (b *Blog) AddUser(username, email, avatar, bio string) *User {
	user := &User{
		ID:       b.getNextID(),
		Username: username,
		Email:    email,
		Avatar:   avatar,
		Bio:      bio,
		JoinDate: time.Now(),
	}
	b.Users[user.ID] = user
	return user
}

// GetUser retrieves a user by ID
func (b *Blog) GetUser(id int) *User {
	return b.Users[id]
}

// AddPost adds a new post to blog
func (b *Blog) AddPost(title, content string, author *User, tags []string) *Post {
	post := &Post{
		ID:        b.getNextID(),
		Title:     title,
		Content:   content,
		Author:    author,
		CreatedAt: time.Now(),
		UpdatedAt: time.Now(),
		Tags:      tags,
		Published: false,
	}
	b.Posts[post.ID] = post
	return post
}

// GetPost retrieves a post by ID
func (b *Blog) GetPost(id int) *Post {
	return b.Posts[id]
}

// PublishPost publishes a post
func (b *Blog) PublishPost(id int) bool {
	if post, ok := b.Posts[id]; ok {
		post.Published = true
		post.UpdatedAt = time.Now()
		return true
	}
	return false
}

// UnpublishPost unpublishes a post
func (b *Blog) UnpublishPost(id int) bool {
	if post, ok := b.Posts[id]; ok {
		post.Published = false
		post.UpdatedAt = time.Now()
		return true
	}
	return false
}

// UpdatePost updates a post's title and content
func (b *Blog) UpdatePost(id, title, content string) bool {
	if post, ok := b.Posts[id]; ok {
		post.Title = title
		post.Content = content
		post.UpdatedAt = time.Now()
		return true
	}
	return false
}

// AddComment adds a new comment to a post
func (b *Blog) AddComment(content string, author *User, postID int, parentID *int) *Comment {
	comment := &Comment{
		ID:        b.getNextID(),
		Content:   content,
		Author:    author,
		PostID:    postID,
		CreatedAt: time.Now(),
		ParentID:  parentID,
	}
	b.Comments[comment.ID] = comment
	return comment
}

// GetCommentsByPost retrieves all comments for a post
func (b *Blog) GetCommentsByPost(postID int) []*Comment {
	var comments []*Comment
	for _, comment := range b.Comments {
		if comment.PostID == postID {
			comments = append(comments, comment)
		}
	}
	return comments
}

// GetCommentsByParent retrieves all replies to a comment
func (b *Blog) GetCommentsByParent(parentID int) []*Comment {
	var comments []*Comment
	for _, comment := range b.Comments {
		if comment.ParentID != nil && *comment.ParentID == parentID {
			comments = append(comments, comment)
		}
	}
	return comments
}

// GetPublishedPosts retrieves all published posts
func (b *Blog) GetPublishedPosts() []*Post {
	var posts []*Post
	for _, post := range b.Posts {
		if post.Published {
			posts = append(posts, post)
		}
	}
	return posts
}

// GetPostsByAuthor retrieves all posts by an author
func (b *Blog) GetPostsByAuthor(authorID int) []*Post {
	var posts []*Post
	for _, post := range b.Posts {
		if post.Author.ID == authorID {
			posts = append(posts, post)
		}
	}
	return posts
}

// GetPostsByTag retrieves all posts with a specific tag
func (b *Blog) GetPostsByTag(tag string) []*Post {
	var posts []*Post
	for _, post := range b.Posts {
		for _, postTag := range post.Tags {
			if postTag == tag {
				posts = append(posts, post)
				break
			}
		}
	}
	return posts
}

// SearchPosts searches posts by title or content
func (b *Blog) SearchPosts(query string) []*Post {
	var posts []*Post
	for _, post := range b.Posts {
		if contains(post.Title, query) || contains(post.Content, query) {
			posts = append(posts, post)
		}
	}
	return posts
}

// contains checks if a string contains a substring (case-insensitive)
func contains(s, substr string) bool {
	// Simple implementation for demonstration
	// In a real application, you might use strings.ContainsFold
	return len(s) >= len(substr) && (s == substr || 
		(len(s) > len(substr) && (s[:len(substr)] == substr || 
		s[len(s)-len(substr):] == substr || 
		findSubstring(s, substr))))
}

// findSubstring checks if substr is in s
func findSubstring(s, substr string) bool {
	for i := 0; i <= len(s)-len(substr); i++ {
		match := true
		for j := 0; j < len(substr); j++ {
			if s[i+j] != substr[j] {
				match = false
				break
			}
		}
		if match {
			return true
		}
	}
	return false
}

// DisplayUser prints user information
func DisplayUser(user *User) {
	fmt.Printf("ID: %d\n", user.ID)
	fmt.Printf("Username: %s\n", user.Username)
	fmt.Printf("Email: %s\n", user.Email)
	fmt.Printf("Avatar: %s\n", user.Avatar)
	fmt.Printf("Bio: %s\n", user.Bio)
	fmt.Printf("Join Date: %s\n", user.JoinDate.Format("2006-01-02 15:04:05"))
}

// DisplayPost prints post information
func DisplayPost(post *Post, showComments bool, blog *Blog) {
	fmt.Printf("ID: %d\n", post.ID)
	fmt.Printf("Title: %s\n", post.Title)
	fmt.Printf("Author: %s\n", post.Author.Username)
	fmt.Printf("Created: %s\n", post.CreatedAt.Format("2006-01-02 15:04:05"))
	fmt.Printf("Updated: %s\n", post.UpdatedAt.Format("2006-01-02 15:04:05"))
	fmt.Printf("Published: %t\n", post.Published)
	
	if len(post.Tags) > 0 {
		fmt.Printf("Tags: ")
		for i, tag := range post.Tags {
			if i > 0 {
				fmt.Printf(", ")
			}
			fmt.Printf("#%s", tag)
		}
		fmt.Printf("\n")
	}
	
	fmt.Printf("Content:\n%s\n", post.Content)
	
	if showComments {
		comments := blog.GetCommentsByPost(post.ID)
		if len(comments) > 0 {
			fmt.Printf("\nComments (%d):\n", len(comments))
			for _, comment := range comments {
				DisplayComment(comment, 0, blog)
			}
		}
	}
}

// DisplayComment prints comment information with indentation
func DisplayComment(comment *Comment, level int, blog *Blog) {
	indent := ""
	for i := 0; i < level; i++ {
		indent += "  "
	}
	
	fmt.Printf("%s%s (%d - %s):\n", indent, comment.Author.Username, 
		comment.ID, comment.CreatedAt.Format("2006-01-02 15:04:05"))
	fmt.Printf("%s%s\n", indent, comment.Content)
	
	// Display replies
	replies := blog.GetCommentsByParent(comment.ID)
	for _, reply := range replies {
		DisplayComment(reply, level+1, blog)
	}
}

// GenerateBlog generates a sample blog with users, posts, and comments
func GenerateBlog() *Blog {
	blog := NewBlog()
	
	// Create users
	john := blog.AddUser("johnsmith", "john@example.com", "john.jpg", "Software developer and tech enthusiast")
	jane := blog.AddUser("janedoe", "jane@example.com", "jane.jpg", "Designer and artist")
	bob := blog.AddUser("bobjohnson", "bob@example.com", "bob.jpg", "Marketing specialist")
	alice := blog.AddUser("alicewilliams", "alice@example.com", "alice.jpg", "Travel blogger and foodie")
	
	// Create posts
	post1 := blog.AddPost("Getting Started with Go", 
		"Go is a powerful programming language developed by Google. It's known for its simplicity and efficiency. In this post, we'll explore the basics of Go programming.",
		john, []string{"programming", "golang", "tutorial"})
	blog.PublishPost(post1.ID)
	
	post2 := blog.AddPost("Design Principles for Modern UI",
		"Creating a modern UI requires understanding of fundamental design principles. In this article, we'll explore color theory, typography, and layout design.",
		jane, []string{"design", "ui", "ux"})
	blog.PublishPost(post2.ID)
	
	post3 := blog.AddPost("Effective Marketing Strategies for Startups",
		"Marketing is crucial for startup success. This post covers various marketing strategies that work well for new businesses with limited budgets.",
		bob, []string{"marketing", "startup", "business"})
	blog.PublishPost(post3.ID)
	
	post4 := blog.AddPost("10 Must-Visit Destinations in Asia",
		"Asia offers incredible diversity in culture, food, and landscapes. Here are my top 10 destinations that should be on every traveler's bucket list.",
		alice, []string{"travel", "asia", "destinations"})
	blog.PublishPost(post4.ID)
	
	// Add comments
	blog.AddComment("Great introduction to Go! Looking forward to more advanced topics.", jane, post1.ID, nil)
	blog.AddComment("This really helped me understand Go's concurrency model.", bob, post1.ID, nil)
	
	comment1 := blog.AddComment("Excellent overview of design principles!", john, post2.ID, nil)
	blog.AddComment("Would love to see a follow-up on mobile UI design.", alice, post2.ID, nil)
	blog.AddComment("Mobile UI is definitely worth its own post. I'll write about it soon!", jane, post2.ID, &comment1.ID)
	
	blog.AddComment("As a startup founder, this is incredibly valuable. Thank you!", alice, post3.ID, nil)
	blog.AddComment("Point 3 about content marketing is particularly useful.", john, post3.ID, nil)
	
	comment2 := blog.AddComment("I've been to 3 of these places and they're amazing!", jane, post4.ID, nil)
	blog.AddComment("Tokyo is absolutely incredible! Best food I've ever had.", john, post4.ID, &comment2.ID)
	blog.AddComment("Adding Kyoto to my list immediately!", bob, post4.ID, &comment2.ID)
	
	return blog
}

// DisplayBlogStats prints statistics about the blog
func DisplayBlogStats(blog *Blog) {
	publishedPosts := blog.GetPublishedPosts()
	
	fmt.Printf("=== Blog Statistics ===\n")
	fmt.Printf("Total Users: %d\n", len(blog.Users))
	fmt.Printf("Total Posts: %d\n", len(blog.Posts))
	fmt.Printf("Published Posts: %d\n", len(publishedPosts))
	fmt.Printf("Total Comments: %d\n", len(blog.Comments))
	
	// Top posters
	postCounts := make(map[int]int)
	for _, post := range blog.Posts {
		postCounts[post.Author.ID]++
	}
	
	fmt.Printf("\nTop Posters:\n")
	for userID, count := range postCounts {
		user := blog.GetUser(userID)
		fmt.Printf("- %s: %d posts\n", user.Username, count)
	}
	
	// Most active commenters
	commentCounts := make(map[int]int)
	for _, comment := range blog.Comments {
		commentCounts[comment.Author.ID]++
	}
	
	fmt.Printf("\nMost Active Commenters:\n")
	for userID, count := range commentCounts {
		user := blog.GetUser(userID)
		fmt.Printf("- %s: %d comments\n", user.Username, count)
	}
	
	// Popular tags
	tagCounts := make(map[string]int)
	for _, post := range publishedPosts {
		for _, tag := range post.Tags {
			tagCounts[tag]++
		}
	}
	
	fmt.Printf("\nPopular Tags:\n")
	for tag, count := range tagCounts {
		fmt.Printf("- #%s: %d posts\n", tag, count)
	}
}

// GenerateRandomID generates a random ID for demonstration
func GenerateRandomID() int {
	rand.Seed(time.Now().UnixNano())
	return rand.Intn(1000) + 1
}

// FormatPostTitle formats a post title for display
func FormatPostTitle(post *Post) string {
	prefix := ""
	if !post.Published {
		prefix = "[DRAFT] "
	}
	return prefix + post.Title
}

// GetRecentPosts gets the most recent posts
func (b *Blog) GetRecentPosts(limit int) []*Post {
	var posts []*Post
	for _, post := range b.Posts {
		if post.Published {
			posts = append(posts, post)
		}
	}
	
	// Simple bubble sort by creation date (descending)
	n := len(posts)
	for i := 0; i < n-1; i++ {
		for j := 0; j < n-i-1; j++ {
			if posts[j].CreatedAt.Before(posts[j+1].CreatedAt) {
				posts[j], posts[j+1] = posts[j+1], posts[j]
			}
		}
	}
	
	if len(posts) > limit {
		posts = posts[:limit]
	}
	
	return posts
}

// GetPostsByDateRange gets posts within a date range
func (b *Blog) GetPostsByDateRange(start, end time.Time) []*Post {
	var posts []*Post
	for _, post := range b.Posts {
		if post.Published && post.CreatedAt.After(start) && post.CreatedAt.Before(end) {
			posts = append(posts, post)
		}
	}
	return posts
}

// CalculateReadingTime estimates reading time in minutes
func CalculateReadingTime(content string) int {
	// Average reading speed: 200 words per minute
	// Average word length: 5 characters
	wordsPerMinute := 200
	wordLength := 5
	charCount := len(content)
	estimatedWords := charCount / wordLength
	readingTime := (estimatedWords + wordsPerMinute - 1) / wordsPerMinute // Round up
	
	if readingTime < 1 {
		readingTime = 1
	}
	
	return readingTime
}

func main() {
	// Generate a sample blog
	blog := GenerateBlog()
	
	// Display blog statistics
	DisplayBlogStats(blog)
	
	// Display all published posts
	fmt.Printf("\n=== All Published Posts ===\n")
	publishedPosts := blog.GetPublishedPosts()
	for _, post := range publishedPosts {
		readingTime := CalculateReadingTime(post.Content)
		fmt.Printf("- %s by %s (%d min read)\n", 
			FormatPostTitle(post), post.Author.Username, readingTime)
	}
	
	// Display a detailed post
	fmt.Printf("\n=== Detailed Post ===\n")
	post := blog.GetPublishedPosts()[0]
	<｜fim▁hole｜>DisplayPost(post, true, blog)
	
	// Display posts by tag
	fmt.Printf("\n=== Posts by Tag ===\n")
	golangPosts := blog.GetPostsByTag("golang")
	fmt.Printf("Golang Posts (%d):\n", len(golangPosts))
	for _, post := range golangPosts {
		fmt.Printf("- %s\n", post.Title)
	}
	
	// Search posts
	fmt.Printf("\n=== Search Results ===\n")
	searchResults := blog.SearchPosts("design")
	fmt.Printf("Posts matching 'design' (%d):\n", len(searchResults))
	for _, post := range searchResults {
		fmt.Printf("- %s\n", post.Title)
	}
	
	// Display recent posts
	fmt.Printf("\n=== Recent Posts ===\n")
	recentPosts := blog.GetRecentPosts(3)
	for _, post := range recentPosts {
		readingTime := CalculateReadingTime(post.Content)
		fmt.Printf("- %s by %s (%d min read, %s)\n", 
			post.Title, post.Author.Username, readingTime, 
			post.CreatedAt.Format("2006-01-02"))
	}
}