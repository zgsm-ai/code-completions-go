/**
 * Social Media Analytics Platform
 * 
 * This module implements a comprehensive social media analytics platform
 * that can track engagement metrics, analyze trends, and generate
 * reports for social media performance.
 * 
 * Features:
 * - Social media account management
 * - Post performance tracking
 * - Engagement analytics
 * - Trend analysis
 * - Competitor analysis
 * - Report generation
 * - Influencer identification
 */

// SocialMediaAccount class represents a social media account
class SocialMediaAccount {
    constructor(id, platform, username, accessToken = null) {
        this.id = id;
        this.platform = platform; // 'twitter', 'facebook', 'instagram', 'linkedin', etc.
        this.username = username;
        this.displayName = null;
        this.accessToken = accessToken;
        this.profileUrl = null;
        this.followersCount = 0;
        this.followingCount = 0;
        this.postsCount = 0;
        this.verified = false;
        this.createdAt = new Date();
        this.lastSync = null;
        this.isActive = false;
        this.avatar = null;
        this.bio = null;
        this.location = null;
        this.website = null;
        this.engagementRate = 0;
        this.metrics = {
            impressions: 0,
            reach: 0,
            likes: 0,
            comments: 0,
            shares: 0,
            saves: 0,
            clicks: 0
        };
    }
    
    /**
     * Update account profile
     * @param {Object} profileData - Profile data from API
     */
    updateProfile(profileData) {
        if (profileData.displayName) this.displayName = profileData.displayName;
        if (profileData.profileUrl) this.profileUrl = profileData.profileUrl;
        if (profileData.followersCount !== undefined) this.followersCount = profileData.followersCount;
        if (profileData.followingCount !== undefined) this.followingCount = profileData.followingCount;
        if (profileData.postsCount !== undefined) this.postsCount = profileData.postsCount;
        if (profileData.verified !== undefined) this.verified = profileData.verified;
        if (profileData.avatar) this.avatar = profileData.avatar;
        if (profileData.bio) this.bio = profileData.bio;
        if (profileData.location) this.location = profileData.location;
        if (profileData.website) this.website = profileData.website;
        
        this.lastSync = new Date();
        this.calculateEngagementRate();
    }
    
    /**
     * Update metrics
     * @param {Object} metricsData - New metrics data
     */
    updateMetrics(metricsData) {
        if (metricsData.impressions !== undefined) this.metrics.impressions += metricsData.impressions;
        if (metricsData.reach !== undefined) this.metrics.reach += metricsData.reach;
        if (metricsData.likes !== undefined) this.metrics.likes += metricsData.likes;
        if (metricsData.comments !== undefined) this.metrics.comments += metricsData.comments;
        if (metricsData.shares !== undefined) this.metrics.shares += metricsData.shares;
        if (metricsData.saves !== undefined) this.metrics.saves += metricsData.saves;
        if (metricsData.clicks !== undefined) this.metrics.clicks += metricsData.clicks;
        
        this.lastSync = new Date();
        this.calculateEngagementRate();
    }
    
    /**
     * Calculate engagement rate
     */
    calculateEngagementRate() {
        const totalEngagements = this.metrics.likes + this.metrics.comments + 
            this.metrics.shares + this.metrics.saves;
        const totalImpressions = this.metrics.impressions || this.followersCount;
        
        this.engagementRate = totalImpressions > 0 ? 
            (totalEngagements / totalImpressions) * 100 : 0;
    }
    
    /**
     * Get account summary
     */
    getSummary() {
        return {
            id: this.id,
            platform: this.platform,
            username: this.username,
            displayName: this.displayName,
            followersCount: this.followersCount,
            postsCount: this.postsCount,
            verified: this.verified,
            engagementRate: this.engagementRate.toFixed(2),
            lastSync: this.lastSync,
            isActive: this.isActive
        };
    }
}

// Post class represents a social media post
class Post {
    constructor(id, accountId, content, platform, postType = 'text') {
        this.id = id;
        this.accountId = accountId;
        this.content = content;
        this.platform = platform;
        this.postType = postType; // 'text', 'image', 'video', 'link', 'carousel'
        this.mediaUrls = [];
        this.tags = [];
        this.mentions = [];
        this.publishedAt = new Date();
        this.metrics = {
            impressions: 0,
            reach: 0,
            likes: 0,
            comments: 0,
            shares: 0,
            saves: 0,
            clicks: 0
        };
        this.sentiment = null;
        this.topics = [];
        this.isPromoted = false;
        this.promotionSpend = 0;
        this.callToAction = null;
        this.linkClicks = 0;
        this.engagementRate = 0;
    }
    
    /**
     * Add media URL
     * @param {string} url - Media URL
     * @param {string} type - Media type ('image', 'video')
     * @param {Object} metadata - Additional metadata
     */
    addMediaUrl(url, type, metadata = {}) {
        this.mediaUrls.push({
            url,
            type,
            metadata,
            addedAt: new Date()
        });
    }
    
    /**
     * Add tag
     * @param {string} tag - Tag to add
     */
    addTag(tag) {
        const normalizedTag = tag.startsWith('#') ? tag.substring(1) : tag;
        if (!this.tags.includes(normalizedTag)) {
            this.tags.push(normalizedTag);
        }
    }
    
    /**
     * Add mention
     * @param {string} username - Username to mention
     */
    addMention(username) {
        if (!this.mentions.includes(username)) {
            this.mentions.push(username);
        }
    }
    
    /**
     * Update metrics
     * @param {Object} metricsData - New metrics data
     */
    updateMetrics(metricsData) {
        if (metricsData.impressions !== undefined) this.metrics.impressions = metricsData.impressions;
        if (metricsData.reach !== undefined) this.metrics.reach = metricsData.reach;
        if (metricsData.likes !== undefined) this.metrics.likes = metricsData.likes;
        if (metricsData.comments !== undefined) this.metrics.comments = metricsData.comments;
        if (metricsData.shares !== undefined) this.metrics.shares = metricsData.shares;
        if (metricsData.saves !== undefined) this.metrics.saves = metricsData.saves;
        if (metricsData.clicks !== undefined) this.metrics.clicks = metricsData.clicks;
        if (metricsData.linkClicks !== undefined) this.linkClicks = metricsData.linkClicks;
        
        this.calculateEngagementRate();
    }
    
    /**
     * Calculate engagement rate
     */
    calculateEngagementRate() {
        const totalEngagements = this.metrics.likes + this.metrics.comments + 
            this.metrics.shares + this.metrics.saves;
        const totalImpressions = this.metrics.impressions;
        
        this.engagementRate = totalImpressions > 0 ? 
            (totalEngagements / totalImpressions) * 100 : 0;
    }
    
    /**
     * Set sentiment
     * @param {string} sentiment - Sentiment ('positive', 'negative', 'neutral')
     */
    setSentiment(sentiment) {
        const validSentiments = ['positive', 'negative', 'neutral'];
        if (validSentiments.includes(sentiment)) {
            this.sentiment = sentiment;
        }
    }
    
    /**
     * Add topic
     * @param {string} topic - Topic to add
     * @param {number} confidence - Confidence score (0-1)
     */
    addTopic(topic, confidence = 0.5) {
        this.topics.push({
            topic,
            confidence,
            detectedAt: new Date()
        });
    }
    
    /**
     * Promote post
     * @param {number} budget - Promotion budget
     * @param {Object} settings - Promotion settings
     */
    promote(budget, settings = {}) {
        this.isPromoted = true;
        this.promotionSpend += budget;
        this.callToAction = settings.callToAction || null;
    }
    
    /**
     * Get post summary
     */
    getSummary() {
        return {
            id: this.id,
            accountId: this.accountId,
            platform: this.platform,
            postType: this.postType,
            content: this.content.length > 100 ? 
                this.content.substring(0, 100) + '...' : this.content,
            publishedAt: this.publishedAt,
            metrics: {
                ...this.metrics,
                engagementRate: this.engagementRate.toFixed(2)
            },
            tags: this.tags,
            mentions: this.mentions,
            sentiment: this.sentiment,
            isPromoted: this.isPromoted,
            promotionSpend: this.promotionSpend
        };
    }
}

// Campaign class represents a social media campaign
class Campaign {
    constructor(id, name, objective, platforms, startDate, endDate) {
        this.id = id;
        this.name = name;
        this.objective = objective; // 'awareness', 'engagement', 'conversion', etc.
        this.platforms = platforms;
        this.startDate = startDate;
        this.endDate = endDate;
        this.budget = 0;
        this.spent = 0;
        this.posts = [];
        this.targetAudience = {};
        this.status = 'draft'; // 'draft', 'active', 'paused', 'completed'
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.metrics = {
            impressions: 0,
            reach: 0,
            engagement: 0,
            clicks: 0,
            conversions: 0,
            costPerClick: 0,
            costPerConversion: 0,
            returnOnInvestment: 0
        };
    }
    
    /**
     * Add post to campaign
     * @param {string} postId - Post ID
     * @param {Date} scheduledDate - Scheduled publish date
     */
    addPost(postId, scheduledDate = null) {
        const campaignPost = {
            id: this.generateId(),
            postId,
            scheduledDate: scheduledDate || new Date(),
            publishedAt: null,
            status: 'scheduled', // 'scheduled', 'published', 'failed'
            metrics: {
                impressions: 0,
                reach: 0,
                engagement: 0,
                clicks: 0,
                spend: 0
            }
        };
        
        this.posts.push(campaignPost);
        this.updatedAt = new Date();
        
        return campaignPost.id;
    }
    
    /**
     * Remove post from campaign
     * @param {string} campaignPostId - Campaign post ID
     */
    removePost(campaignPostId) {
        const index = this.posts.findIndex(post => post.id === campaignPostId);
        if (index !== -1) {
            this.posts.splice(index, 1);
            this.updatedAt = new Date();
            return true;
        }
        return false;
    }
    
    /**
     * Set target audience
     * @param {Object} audience - Target audience criteria
     */
    setTargetAudience(audience) {
        this.targetAudience = {
            ageRange: audience.ageRange || null,
            gender: audience.gender || null,
            location: audience.location || null,
            interests: audience.interests || [],
            language: audience.language || null,
            device: audience.device || null,
            ...audience
        };
        this.updatedAt = new Date();
    }
    
    /**
     * Set budget
     * @param {number} budget - Campaign budget
     */
    setBudget(budget) {
        this.budget = budget;
        this.updatedAt = new Date();
    }
    
    /**
     * Update spend
     * @param {number} amount - Amount to add to spent
     */
    updateSpend(amount) {
        this.spent += amount;
        this.updatedAt = new Date();
        
        // Check if campaign should be paused
        if (this.spent >= this.budget && this.status === 'active') {
            this.status = 'paused';
        }
    }
    
    /**
     * Start campaign
     */
    start() {
        const now = new Date();
        if (now >= this.startDate && now <= this.endDate) {
            this.status = 'active';
            this.updatedAt = new Date();
            return true;
        }
        return false;
    }
    
    /**
     * Pause campaign
     */
    pause() {
        if (this.status === 'active') {
            this.status = 'paused';
            this.updatedAt = new Date();
            return true;
        }
        return false;
    }
    
    /**
     * Resume campaign
     */
    resume() {
        if (this.status === 'paused') {
            const now = new Date();
            if (now <= this.endDate) {
                this.status = 'active';
                this.updatedAt = new Date();
                return true;
            }
        }
        return false;
    }
    
    /**
     * Update metrics
     */
    updateMetrics() {
        let totalImpressions = 0;
        let totalReach = 0;
        let totalEngagement = 0;
        let totalClicks = 0;
        let totalConversions = 0;
        let postSpend = 0;
        
        for (const post of this.posts) {
            totalImpressions += post.metrics.impressions;
            totalReach += post.metrics.reach;
            totalEngagement += post.metrics.engagement;
            totalClicks += post.metrics.clicks;
            totalConversions += post.metrics.conversions || 0;
            postSpend += post.metrics.spend || 0;
        }
        
        this.metrics.impressions = totalImpressions;
        this.metrics.reach = totalReach;
        this.metrics.engagement = totalEngagement;
        this.metrics.clicks = totalClicks;
        this.metrics.conversions = totalConversions;
        this.metrics.costPerClick = totalClicks > 0 ? postSpend / totalClicks : 0;
        this.metrics.costPerConversion = totalConversions > 0 ? postSpend / totalConversions : 0;
        
        // Calculate ROI (simplified)
        const conversionValue = totalConversions * 50; // Assume $50 per conversion
        this.metrics.returnOnInvestment = postSpend > 0 ? 
            ((conversionValue - postSpend) / postSpend) * 100 : 0;
        
        this.updatedAt = new Date();
    }
    
    /**
     * Generate unique ID
     */
    generateId() {
        return Date.now().toString(36) + Math.random().toString(36).substring(2);
    }
    
    /**
     * Get campaign summary
     */
    getSummary() {
        return {
            id: this.id,
            name: this.name,
            objective: this.objective,
            platforms: this.platforms,
            status: this.status,
            startDate: this.startDate,
            endDate: this.endDate,
            budget: this.budget,
            spent: this.spent,
            remainingBudget: this.budget - this.spent,
            postsCount: this.posts.length,
            metrics: {
                ...this.metrics,
                costPerClick: this.metrics.costPerClick.toFixed(2),
                costPerConversion: this.metrics.costPerConversion.toFixed(2),
                returnOnInvestment: this.metrics.returnOnInvestment.toFixed(2)
            },
            createdAt: this.createdAt,
            updatedAt: this.updatedAt
        };
    }
}

// Trend class represents a social media trend
class Trend {
    constructor(id, platform, keyword, startDate = new Date()) {
        this.id = id;
        this.platform = platform;
        this.keyword = keyword;
        this.startDate = startDate;
        this.endDate = null;
        this.isActive = true;
        this.volume = 0;
        this.peakVolume = 0;
        this.peakDate = null;
        this.growthRate = 0;
        this.relatedKeywords = [];
        this.sentiment = null;
        this.topPosts = [];
        this demographics = {};
        this.locations = [];
        this.hourlyData = new Map(); // hour -> volume
    }
    
    /**
     * Update trend data
     * @param {number} volume - Current volume
     * @param {number} previousVolume - Previous volume
     */
    updateData(volume, previousVolume = null) {
        this.volume = volume;
        
        // Update peak
        if (volume > this.peakVolume) {
            this.peakVolume = volume;
            this.peakDate = new Date();
        }
        
        // Calculate growth rate
        if (previousVolume !== null && previousVolume > 0) {
            this.growthRate = ((volume - previousVolume) / previousVolume) * 100;
        }
        
        // Record hourly data
        const hour = new Date().getHours();
        this.hourlyData.set(hour, volume);
        
        // Keep only last 24 hours
        if (this.hourlyData.size > 24) {
            const oldestHour = Math.min(...this.hourlyData.keys());
            this.hourlyData.delete(oldestHour);
        }
    }
    
    /**
     * Add related keyword
     * @param {string} keyword - Related keyword
     * @param {number} correlation - Correlation strength (0-1)
     */
    addRelatedKeyword(keyword, correlation = 0.5) {
        // Check if already exists
        const existingIndex = this.relatedKeywords.findIndex(
            item => item.keyword === keyword
        );
        
        if (existingIndex !== -1) {
            this.relatedKeywords[existingIndex].correlation = correlation;
        } else {
            this.relatedKeywords.push({
                keyword,
                correlation,
                detectedAt: new Date()
            });
        }
        
        // Sort by correlation (highest first)
        this.relatedKeywords.sort((a, b) => b.correlation - a.correlation);
    }
    
    /**
     * Add top post
     * @param {string} postId - Post ID
     * @param {string} platform - Platform
     * @param {Object} metrics - Post metrics
     */
    addTopPost(postId, platform, metrics) {
        const topPost = {
            postId,
            platform,
            metrics,
            addedAt: new Date()
        };
        
        this.topPosts.push(topPost);
        
        // Keep only top 10 posts
        if (this.topPosts.length > 10) {
            this.topPosts.sort((a, b) => 
                b.metrics.engagement - a.metrics.engagement
            );
            this.topPosts = this.topPosts.slice(0, 10);
        }
    }
    
    /**
     * Update sentiment
     * @param {string} sentiment - Overall sentiment
     */
    updateSentiment(sentiment) {
        const validSentiments = ['positive', 'negative', 'neutral'];
        if (validSentiments.includes(sentiment)) {
            this.sentiment = sentiment;
        }
    }
    
    /**
     * Add demographic data
     * @param {string} demographic - Demographic category
     * @param {number} percentage - Percentage of interest
     */
    addDemographic(demographic, percentage) {
        this.demographics[demographic] = percentage;
    }
    
    /**
     * Add location data
     * @param {string} location - Location name
     * @param {number} percentage - Percentage of mentions
     */
    addLocation(location, percentage) {
        this.locations.push({
            location,
            percentage,
            detectedAt: new Date()
        });
        
        // Sort by percentage (highest first)
        this.locations.sort((a, b) => b.percentage - a.percentage);
        
        // Keep only top 20 locations
        if (this.locations.length > 20) {
            this.locations = this.locations.slice(0, 20);
        }
    }
    
    /**
     * End trend
     */
    end() {
        this.isActive = false;
        this.endDate = new Date();
    }
    
    /**
     * Get trend summary
     */
    getSummary() {
        return {
            id: this.id,
            platform: this.platform,
            keyword: this.keyword,
            isActive: this.isActive,
            startDate: this.startDate,
            endDate: this.endDate,
            volume: this.volume,
            peakVolume: this.peakVolume,
            peakDate: this.peakDate,
            growthRate: this.growthRate.toFixed(2),
            sentiment: this.sentiment,
            relatedKeywords: this.relatedKeywords.slice(0, 5),
            topLocations: this.locations.slice(0, 5),
            duration: this.endDate ? 
                this.endDate - this.startDate : 
                new Date() - this.startDate
        };
    }
}

// SocialMediaAnalytics class manages social media analytics
class SocialMediaAnalytics {
    constructor() {
        this.accounts = new Map(); // accountId -> SocialMediaAccount
        this.posts = new Map(); // postId -> Post
        this.campaigns = new Map(); // campaignId -> Campaign
        this.trends = new Map(); // trendId -> Trend
        this.competitors = new Map(); // competitorId -> Competitor
        this.reports = new Map(); // reportId -> Report
        this.syncInterval = 3600000; // 1 hour
        this.syncTimer = null;
        this.eventListeners = new Map();
    }
    
    /**
     * Add social media account
     * @param {SocialMediaAccount} account - Account to add
     */
    addAccount(account) {
        this.accounts.set(account.id, account);
        this.emit('account-added', { account });
    }
    
    /**
     * Remove social media account
     * @param {string} accountId - Account ID to remove
     */
    removeAccount(accountId) {
        const account = this.accounts.get(accountId);
        if (account) {
            this.accounts.delete(accountId);
            this.emit('account-removed', { account });
            return true;
        }
        return false;
    }
    
    /**
     * Add post
     * @param {Post} post - Post to add
     */
    addPost(post) {
        this.posts.set(post.id, post);
        this.emit('post-added', { post });
        
        // Analyze post for sentiment and topics
        this.analyzePost(post);
    }
    
    /**
     * Update post
     * @param {string} postId - Post ID
     * @param {Object} metricsData - New metrics data
     */
    updatePost(postId, metricsData) {
        const post = this.posts.get(postId);
        if (post) {
            post.updateMetrics(metricsData);
            this.emit('post-updated', { post });
            return true;
        }
        return false;
    }
    
    /**
     * Add campaign
     * @param {Campaign} campaign - Campaign to add
     */
    addCampaign(campaign) {
        this.campaigns.set(campaign.id, campaign);
        this.emit('campaign-added', { campaign });
    }
    
    /**
     * Update campaign
     * @param {string} campaignId - Campaign ID
     */
    updateCampaign(campaignId) {
        const campaign = this.campaigns.get(campaignId);
        if (campaign) {
            campaign.updateMetrics();
            this.emit('campaign-updated', { campaign });
            return true;
        }
        return false;
    }
    
    /**
     * Add trend
     * @param {Trend} trend - Trend to add
     */
    addTrend(trend) {
        this.trends.set(trend.id, trend);
        this.emit('trend-added', { trend });
    }
    
    /**
     * Update trend
     * @param {string} trendId - Trend ID
     * @param {number} volume - Current volume
     */
    updateTrend(trendId, volume) {
        const trend = this.trends.get(trendId);
        if (trend) {
            const previousVolume = trend.volume;
            trend.updateData(volume, previousVolume);
            this.emit('trend-updated', { trend });
            return true;
        }
        return false;
    }
    
    /**
     * Analyze post for sentiment and topics
     * @param {Post} post - Post to analyze
     */
    analyzePost(post) {
        // In a real implementation, this would use NLP/ML
        // For now, we'll simulate
        
        // Simulate sentiment analysis
        const positiveWords = ['great', 'amazing', 'love', 'awesome', 'fantastic'];
        const negativeWords = ['bad', 'hate', 'terrible', 'awful', 'disappointing'];
        
        const content = post.content.toLowerCase();
        let positiveCount = 0;
        let negativeCount = 0;
        
        for (const word of positiveWords) {
            if (content.includes(word)) positiveCount++;
        }
        
        for (const word of negativeWords) {
            if (content.includes(word)) negativeCount++;
        }
        
        let sentiment = 'neutral';
        if (positiveCount > negativeCount) {
            sentiment = 'positive';
        } else if (negativeCount > positiveCount) {
            sentiment = 'negative';
        }
        
        post.setSentiment(sentiment);
        
        // Simulate topic extraction
        const topics = this.extractTopics(post.content);
        for (const topic of topics) {
            post.addTopic(topic, Math.random() * 0.5 + 0.5);
        }
    }
    
    /**
     * Extract topics from text
     * @param {string} text - Text to analyze
     */
    extractTopics(text) {
        // Simplified topic extraction
        const commonTopics = [
            'technology', 'business', 'marketing', 'social media',
            'design', 'development', 'innovation', 'data',
            'analytics', 'strategy', 'content', 'engagement'
        ];
        
        const topics = [];
        const lowerText = text.toLowerCase();
        
        for (const topic of commonTopics) {
            if (lowerText.includes(topic)) {
                topics.push(topic);
            }
        }
        
        return topics;
    }
    
    /**
     * Get top performing posts
     * @param {string} accountId - Account ID (optional)
     * @param {number} limit - Maximum number of posts
     * @param {string} metric - Metric to sort by
     */
    getTopPerformingPosts(accountId = null, limit = 10, metric = 'engagement') {
        let posts = Array.from(this.posts.values());
        
        if (accountId) {
            posts = posts.filter(post => post.accountId === accountId);
        }
        
        // Sort by metric
        posts.sort((a, b) => {
            const aMetric = this.calculatePostMetric(a, metric);
            const bMetric = this.calculatePostMetric(b, metric);
            return bMetric - aMetric;
        });
        
        return posts.slice(0, limit);
    }
    
    /**
     * Calculate specific metric for a post
     * @param {Post} post - Post to analyze
     * @param {string} metric - Metric name
     */
    calculatePostMetric(post, metric) {
        switch (metric) {
            case 'engagement':
                return post.metrics.likes + post.metrics.comments + 
                    post.metrics.shares + post.metrics.saves;
            case 'reach':
                return post.metrics.reach;
            case 'impressions':
                return post.metrics.impressions;
            case 'clicks':
                return post.metrics.clicks;
            case 'engagement_rate':
                return post.engagementRate;
            default:
                return 0;
        }
    }
    
    /**
     * Get account performance summary
     * @param {string} accountId - Account ID
     * @param {Date} startDate - Start date
     * @param {Date} endDate - End date
     */
    getAccountPerformance(accountId, startDate, endDate) {
        const account = this.accounts.get(accountId);
        if (!account) {
            throw new Error(`Account with ID ${accountId} not found`);
        }
        
        // Get posts in date range
        const posts = Array.from(this.posts.values()).filter(post => 
            post.accountId === accountId &&
            post.publishedAt >= startDate &&
            post.publishedAt <= endDate
        );
        
        // Calculate metrics
        let totalImpressions = 0;
        let totalReach = 0;
        let totalLikes = 0;
        let totalComments = 0;
        let totalShares = 0;
        let totalSaves = 0;
        let totalClicks = 0;
        
        for (const post of posts) {
            totalImpressions += post.metrics.impressions;
            totalReach += post.metrics.reach;
            totalLikes += post.metrics.likes;
            totalComments += post.metrics.comments;
            totalShares += post.metrics.shares;
            totalSaves += post.metrics.saves;
            totalClicks += post.metrics.clicks;
        }
        
        // Calculate averages
        const postCount = posts.length;
        const avgEngagementRate = posts.length > 0 ? 
            posts.reduce((sum, post) => sum + post.engagementRate, 0) / postCount : 0;
        
        return {
            account: account.getSummary(),
            period: { startDate, endDate },
            postCount,
            metrics: {
                totalImpressions,
                totalReach,
                totalLikes,
                totalComments,
                totalShares,
                totalSaves,
                totalClicks,
                avgEngagementRate: avgEngagementRate.toFixed(2)
            },
            topPosts: this.getTopPerformingPosts(accountId, 5),
            growthMetrics: this.calculateAccountGrowth(accountId, startDate, endDate)
        };
    }
    
    /**
     * Calculate account growth metrics
     * @param {string} accountId - Account ID
     * @param {Date} startDate - Start date
     * @param {Date} endDate - End date
     */
    calculateAccountGrowth(accountId, startDate, endDate) {
        const account = this.accounts.get(accountId);
        if (!account || !account.lastSync) {
            return null;
        }
        
        // In a real implementation, this would compare historical data
        // For now, we'll simulate growth
        const followerGrowth = Math.floor(Math.random() * 100) - 20; // -20 to 80
        const engagementGrowth = (Math.random() * 10) - 2; // -2 to 8
        
        return {
            followerGrowth,
            engagementGrowth,
            engagementRateChange: engagementGrowth.toFixed(2)
        };
    }
    
    /**
     * Get platform performance comparison
     * @param {Date} startDate - Start date
     * @param {Date} endDate - End date
     */
    getPlatformComparison(startDate, endDate) {
        const platforms = new Map();
        
        // Group posts by platform
        for (const post of this.posts.values()) {
            if (post.publishedAt >= startDate && post.publishedAt <= endDate) {
                if (!platforms.has(post.platform)) {
                    platforms.set(post.platform, {
                        posts: [],
                        totalImpressions: 0,
                        totalReach: 0,
                        totalLikes: 0,
                        totalComments: 0,
                        totalShares: 0
                    });
                }
                
                const platform = platforms.get(post.platform);
                platform.posts.push(post);
                platform.totalImpressions += post.metrics.impressions;
                platform.totalReach += post.metrics.reach;
                platform.totalLikes += post.metrics.likes;
                platform.totalComments += post.metrics.comments;
                platform.totalShares += post.metrics.shares;
            }
        }
        
        // Calculate averages and engagement rates
        for (const [platformName, data] of platforms) {
            const avgEngagement = data.posts.length > 0 ? 
                data.posts.reduce((sum, post) => sum + post.engagementRate, 0) / data.posts.length : 0;
            
            data.avgEngagementRate = avgEngagement.toFixed(2);
            data.postCount = data.posts.length;
        }
        
        return platforms;
    }
    
    /**
     * Start automatic syncing
     */
    startSyncing() {
        if (this.syncTimer) {
            clearInterval(this.syncTimer);
        }
        
        this.syncTimer = setInterval(() => {
            this.syncAllAccounts();
        }, this.syncInterval);
    }
    
    /**
     * Stop automatic syncing
     */
    stopSyncing() {
        if (this.syncTimer) {
            clearInterval(this.syncTimer);
            this.syncTimer = null;
        }
    }
    
    /**
     * Sync all accounts
     */
    async syncAllAccounts() {
        for (const account of this.accounts.values()) {
            try {
                await this.syncAccount(account);
            } catch (error) {
                console.error(`Error syncing account ${account.id}:`, error);
            }
        }
        
        this.emit('sync-completed', { 
            timestamp: new Date(),
            accountCount: this.accounts.size
        });
    }
    
    /**
     * Sync single account
     * @param {SocialMediaAccount} account - Account to sync
     */
    async syncAccount(account) {
        // In a real implementation, this would fetch data from social media APIs
        // For now, we'll simulate data updates
        
        // Simulate profile update
        const followerGrowth = Math.floor(Math.random() * 20) - 5; // -5 to 15
        account.followersCount += followerGrowth;
        
        // Simulate metrics update
        const impressions = Math.floor(Math.random() * 10000);
        const likes = Math.floor(Math.random() * 500);
        const comments = Math.floor(Math.random() * 100);
        const shares = Math.floor(Math.random() * 50);
        
        account.updateMetrics({
            impressions,
            likes,
            comments,
            shares
        });
        
        this.emit('account-synced', { account });
    }
    
    /**
     * Add event listener
     * @param {string} event - Event name
     * @param {Function} callback - Event callback
     */
    on(event, callback) {
        if (!this.eventListeners.has(event)) {
            this.eventListeners.set(event, []);
        }
        
        this.eventListeners.get(event).push(callback);
    }
    
    /**
     * Remove event listener
     * @param {string} event - Event name
     * @param {Function} callback - Event callback
     */
    off(event, callback) {
        if (this.eventListeners.has(event)) {
            const listeners = this.eventListeners.get(event);
            const index = listeners.indexOf(callback);
            if (index > -1) {
                listeners.splice(index, 1);
            }
        }
    }
    
    /**
     * Emit event
     * @param {string} event - Event name
     * @param {Object} data - Event data
     */
    emit(event, data) {
        if (this.eventListeners.has(event)) {
            this.eventListeners.get(event).forEach(callback => {
                try {
                    callback(data);
                } catch (error) {
                    console.error(`Error in event listener for '${event}':`, error);
                }
            });
        }
    }
    
    /**
     * Get analytics summary
     */
    getSummary() {
        return {
            accountCount: this.accounts.size,
            postCount: this.posts.size,
            campaignCount: this.campaigns.size,
            activeTrends: Array.from(this.trends.values()).filter(trend => trend.isActive).length,
            lastSync: new Date()
        };
    }
}

// Main function to demonstrate social media analytics
function main() {
    console.log('Initializing Social Media Analytics Platform...\n');
    
    // Create analytics platform
    const analytics = new SocialMediaAnalytics();
    
    // Create social media accounts
    const twitterAccount = new SocialMediaAccount(
        'account1',
        'twitter',
        'company_name',
        'access_token_123'
    );
    twitterAccount.updateProfile({
        displayName: 'Company Name',
        followersCount: 15000,
        followingCount: 500,
        postsCount: 850,
        verified: true
    });
    
    const instagramAccount = new SocialMediaAccount(
        'account2',
        'instagram',
        'company_name',
        'access_token_456'
    );
    instagramAccount.updateProfile({
        displayName: 'Company Name',
        followersCount: 25000,
        followingCount: 800,
        postsCount: 420,
        verified: false
    });
    
    const facebookAccount = new SocialMediaAccount(
        'account3',
        'facebook',
        'company_page',
        'access_token_789'
    );
    facebookAccount.updateProfile({
        displayName: 'Company Name',
        followersCount: 5000,
        followingCount: 100,
        postsCount: 320,
        verified: true
    });
    
    // Add accounts to analytics
    analytics.addAccount(twitterAccount);
    analytics.addAccount(instagramAccount);
    analytics.addAccount(facebookAccount);
    
    console.log('Added social media accounts:');
    console.log(`- Twitter: ${twitterAccount.username} (${twitterAccount.followersCount} followers)`);
    console.log(`- Instagram: ${instagramAccount.username} (${instagramAccount.followersCount} followers)`);
    console.log(`- Facebook: ${facebookAccount.username} (${facebookAccount.followersCount} followers)`);
    
    // Create sample posts
    const post1 = new Post(
        'post1',
        twitterAccount.id,
        'Check out our latest product launch! 🚀 #innovation #technology',
        'twitter'
    );
    post1.addTag('innovation');
    post1.addTag('technology');
    post1.addMediaUrl('https://example.com/image1.jpg', 'image');
    post1.updateMetrics({
        impressions: 5000,
        reach: 3000,
        likes: 250,
        comments: 30,
        shares: 45,
        saves: 15,
        clicks: 120
    });
    
    const post2 = new Post(
        'post2',
        instagramAccount.id,
        'Behind the scenes at our headquarters #team #culture',
        'instagram',
        'image'
    );
    post2.addTag('team');
    post2.addTag('culture');
    post2.addMediaUrl('https://example.com/image2.jpg', 'image');
    post2.updateMetrics({
        impressions: 8000,
        reach: 6000,
        likes: 750,
        comments: 85,
        shares: 120,
        saves: 200,
        clicks: 0
    });
    
    const post3 = new Post(
        'post3',
        facebookAccount.id,
        'We\'re hiring! Join our amazing team. Link in bio. #hiring #careers',
        'facebook',
        'link'
    );
    post3.addTag('hiring');
    post3.addTag('careers');
    post3.updateMetrics({
        impressions: 3000,
        reach: 2500,
        likes: 180,
        comments: 65,
        shares: 80,
        saves: 40,
        clicks: 250
    });
    
    // Add posts to analytics
    analytics.addPost(post1);
    analytics.addPost(post2);
    analytics.addPost(post3);
    
    console.log('\nAdded sample posts:');
    console.log(`- Post 1: ${post1.content.substring(0, 50)}... (${post1.platform})`);
    console.log(`- Post 2: ${post2.content.substring(0, 50)}... (${post2.platform})`);
    console.log(`- Post 3: ${post3.content.substring(0, 50)}... (${post3.platform})`);
    
    // Create campaign
    const campaign = new Campaign(
        'campaign1',
        'Summer Product Launch',
        'awareness',
        ['twitter', 'instagram', 'facebook'],
        new Date('2023-06-01'),
        new Date('2023-06-30')
    );
    campaign.setBudget(5000);
    campaign.setTargetAudience({
        ageRange: [18, 35],
        interests: ['technology', 'innovation'],
        location: 'USA'
    });
    
    campaign.addPost(post1.id);
    campaign.addPost(post2.id);
    campaign.addPost(post3.id);
    
    analytics.addCampaign(campaign);
    
    console.log('\nCreated campaign:');
    console.log(`- Name: ${campaign.name}`);
    console.log(`- Objective: ${campaign.objective}`);
    console.log(`- Budget: $${campaign.budget}`);
    console.log(`- Posts: ${campaign.posts.length}`);
    
    // Create trends
    const trend1 = new Trend('trend1', 'twitter', 'innovation');
    trend1.updateData(5000, 3000);
    trend1.addRelatedKeyword('technology', 0.8);
    trend1.addRelatedKeyword('future', 0.6);
    trend1.updateSentiment('positive');
    
    const trend2 = new Trend('trend2', 'instagram', 'sustainability');
    trend2.updateData(3500, 2000);
    trend2.addRelatedKeyword('environment', 0.9);
    trend2.addRelatedKeyword('green', 0.7);
    trend2.updateSentiment('positive');
    
    analytics.addTrend(trend1);
    analytics.addTrend(trend2);
    
    console.log('\nAdded trends:');
    console.log(`- ${trend1.keyword} on ${trend1.platform} (Volume: ${trend1.volume})`);
    console.log(`- ${trend2.keyword} on ${trend2.platform} (Volume: ${trend2.volume})`);
    
    // Get top performing posts
    const topPosts = analytics.getTopPerformingPosts(null, 5, 'engagement');
    console.log('\nTop performing posts by engagement:');
    topPosts.forEach((post, index) => {
        const engagement = post.metrics.likes + post.metrics.comments + 
            post.metrics.shares + post.metrics.saves;
        console.log(`${index + 1}. ${post.content.substring(0, 40)}... (${post.platform}) - ${engagement} engagements`);
    });
    
    // Get account performance
    const startDate = new Date('2023-06-01');
    const endDate = new Date('2023-06-30');
    const twitterPerformance = analytics.getAccountPerformance(twitterAccount.id, startDate, endDate);
    
    console.log('\nTwitter Account Performance:');
    console.log(`- Followers: ${twitterPerformance.account.followersCount}`);
    console.log(`- Posts: ${twitterPerformance.postCount}`);
    console.log(`- Total Likes: ${twitterPerformance.metrics.totalLikes}`);
    console.log(`- Avg Engagement Rate: ${twitterPerformance.metrics.avgEngagementRate}%`);
    
    // Get platform comparison
    const platformComparison = analytics.getPlatformComparison(startDate, endDate);
    
    console.log('\nPlatform Performance Comparison:');
    for (const [platform, data] of platformComparison) {
        console.log(`- ${platform.charAt(0).toUpperCase() + platform.slice(1)}:`);
        console.log(`  Posts: ${data.postCount}`);
        console.log(`  Impressions: ${data.totalImpressions}`);
        console.log(`  Likes: ${data.totalLikes}`);
        console.log(`  Avg Engagement: ${data.avgEngagementRate}%`);
    }
    
    // Start syncing
    analytics.startSyncing();
    console.log('\nStarted automatic syncing');
    
    // Set up event listener
    analytics.on('sync-completed', (data) => {
        console.log(`\nSync completed at ${data.timestamp} for ${data.accountCount} accounts`);
    });
    
    // Get analytics summary
    const summary = analytics.getSummary();
    console.log('\nAnalytics Summary:');
    console.log(`- Accounts: ${summary.accountCount}`);
    console.log(`- Posts: ${summary.postCount}`);
    console.log(`- Campaigns: ${summary.campaignCount}`);
    console.log(`- Active Trends: ${summary.activeTrends}`);
}

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        SocialMediaAccount,
        Post,
        Campaign,
        Trend,
        SocialMediaAnalytics
    };
}