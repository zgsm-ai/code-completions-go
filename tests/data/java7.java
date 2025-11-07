/**
 * Customer Relationship Management (CRM) System
 * 
 * This class implements a comprehensive customer relationship management system
 * that can handle customer data, interactions, sales opportunities, and marketing campaigns.
 * 
 * Features:
 * - Customer profile management
 * - Interaction history tracking
 * - Sales pipeline management
 * - Lead and opportunity tracking
 * - Campaign management
 * - Customer segmentation
 * - Performance analytics
 * - Task and reminder scheduling
 */

import java.util.*;
import java.text.*;
import java.io.*;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

/**
 * Customer class represents a customer in the CRM system
 */
class Customer {
    private String customerId;
    private String firstName;
    private String lastName;
    private String email;
    private String phone;
    private String address;
    private String city;
    private String state;
    private String country;
    private String postalCode;
    private Date registrationDate;
    private Date lastContactDate;
    private String customerType;
    private String status;
    private double totalSpent;
    private int orderCount;
    private String leadSource;
    private String assignedSalesRep;
    private List<String> tags;
    private List<String> interests;
    private Map<String, String> customFields;
    private Date nextFollowUpDate;
    
    /**
     * Constructor for Customer
     * @param customerId Customer ID
     * @param firstName First name
     * @param lastName Last name
     * @param email Email address
     * @param phone Phone number
     * @param address Street address
     * @param city City
     * @param state State
     * @param country Country
     * @param postalCode Postal code
     * @param customerType Customer type
     * @param leadSource Lead source
     * @param assignedSalesRep Assigned sales representative
     */
    public Customer(String customerId, String firstName, String lastName, String email,
                   String phone, String address, String city, String state, String country,
                   String postalCode, String customerType, String leadSource, String assignedSalesRep) {
        this.customerId = customerId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phone = phone;
        this.address = address;
        this.city = city;
        this.state = state;
        this.country = country;
        this.postalCode = postalCode;
        this.registrationDate = new Date();
        this.lastContactDate = new Date();
        this.customerType = customerType;
        this.status = "Active";
        this.totalSpent = 0.0;
        this.orderCount = 0;
        this.leadSource = leadSource;
        this.assignedSalesRep = assignedSalesRep;
        this.tags = new ArrayList<>();
        this.interests = new ArrayList<>();
        this.customFields = new HashMap<>();
        this.nextFollowUpDate = null;
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get first name
     * @return First name
     */
    public String getFirstName() {
        return firstName;
    }
    
    /**
     * Get last name
     * @return Last name
     */
    public String getLastName() {
        return lastName;
    }
    
    /**
     * Get full name
     * @return Full name
     */
    public String getFullName() {
        return firstName + " " + lastName;
    }
    
    /**
     * Get email
     * @return Email address
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get phone
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get address
     * @return Street address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get city
     * @return City
     */
    public String getCity() {
        return city;
    }
    
    /**
     * Get state
     * @return State
     */
    public String getState() {
        return state;
    }
    
    /**
     * Get country
     * @return Country
     */
    public String getCountry() {
        return country;
    }
    
    /**
     * Get postal code
     * @return Postal code
     */
    public String getPostalCode() {
        return postalCode;
    }
    
    /**
     * Get registration date
     * @return Registration date
     */
    public Date getRegistrationDate() {
        return registrationDate;
    }
    
    /**
     * Get last contact date
     * @return Last contact date
     */
    public Date getLastContactDate() {
        return lastContactDate;
    }
    
    /**
     * Get customer type
     * @return Customer type
     */
    public String getCustomerType() {
        return customerType;
    }
    
    /**
     * Get status
     * @return Customer status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get total spent
     * @return Total amount spent
     */
    public double getTotalSpent() {
        return totalSpent;
    }
    
    /**
     * Get order count
     * @return Number of orders
     */
    public int getOrderCount() {
        return orderCount;
    }
    
    /**
     * Get lead source
     * @return Lead source
     */
    public String getLeadSource() {
        return leadSource;
    }
    
    /**
     * Get assigned sales representative
     * @return Assigned sales representative
     */
    public String getAssignedSalesRep() {
        return assignedSalesRep;
    }
    
    /**
     * Get tags
     * @return List of customer tags
     */
    public List<String> getTags() {
        return tags;
    }
    
    /**
     * Get interests
     * @return List of customer interests
     */
    public List<String> getInterests() {
        return interests;
    }
    
    /**
     * Get custom fields
     * @return Map of custom fields
     */
    public Map<String, String> getCustomFields() {
        return customFields;
    }
    
    /**
     * Get next follow-up date
     * @return Next follow-up date
     */
    public Date getNextFollowUpDate() {
        return nextFollowUpDate;
    }
    
    /**
     * Update total spent
     * @param amount Amount to add
     */
    public void updateTotalSpent(double amount) {
        this.totalSpent += amount;
    }
    
    /**
     * Increment order count
     */
    public void incrementOrderCount() {
        this.orderCount++;
    }
    
    /**
     * Update last contact date
     * @param date New last contact date
     */
    public void updateLastContactDate(Date date) {
        this.lastContactDate = date;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update next follow-up date
     * @param date New next follow-up date
     */
    public void updateNextFollowUpDate(Date date) {
        this.nextFollowUpDate = date;
    }
    
    /**
     * Add tag
     * @param tag Tag to add
     */
    public void addTag(String tag) {
        if (!tags.contains(tag)) {
            tags.add(tag);
        }
    }
    
    /**
     * Remove tag
     * @param tag Tag to remove
     */
    public void removeTag(String tag) {
        tags.remove(tag);
    }
    
    /**
     * Add interest
     * @param interest Interest to add
     */
    public void addInterest(String interest) {
        if (!interests.contains(interest)) {
            interests.add(interest);
        }
    }
    
    /**
     * Remove interest
     * @param interest Interest to remove
     */
    public void removeInterest(String interest) {
        interests.remove(interest);
    }
    
    /**
     * Add custom field
     * @param fieldName Field name
     * @param fieldValue Field value
     */
    public void addCustomField(String fieldName, String fieldValue) {
        customFields.put(fieldName, fieldValue);
    }
    
    /**
     * Remove custom field
     * @param fieldName Field name to remove
     */
    public void removeCustomField(String fieldName) {
        customFields.remove(fieldName);
    }
    
    /**
     * Get customer summary
     * @return Customer summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Customer ID: ").append(customerId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Address: ").append(address).append(", ").append(city).append(", ")
          .append(state).append(", ").append(country).append(" ").append(postalCode).append("\n");
        sb.append("Customer Type: ").append(customerType).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Registration Date: ").append(sdf.format(registrationDate)).append("\n");
        sb.append("Last Contact Date: ").append(sdf.format(lastContactDate)).append("\n");
        sb.append("Total Spent: $").append(String.format("%.2f", totalSpent)).append("\n");
        sb.append("Order Count: ").append(orderCount).append("\n");
        sb.append("Lead Source: ").append(leadSource).append("\n");
        sb.append("Assigned Sales Rep: ").append(assignedSalesRep).append("\n");
        
        if (!tags.isEmpty()) {
            sb.append("Tags: ").append(String.join(", ", tags)).append("\n");
        }
        
        if (!interests.isEmpty()) {
            sb.append("Interests: ").append(String.join(", ", interests)).append("\n");
        }
        
        if (!customFields.isEmpty()) {
            sb.append("Custom Fields:\n");
            for (Map.Entry<String, String> entry : customFields.entrySet()) {
                sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
            }
        }
        
        if (nextFollowUpDate != null) {
            sb.append("Next Follow-up Date: ").append(sdf.format(nextFollowUpDate)).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Interaction class represents an interaction with a customer
 */
class Interaction {
    private String interactionId;
    private String customerId;
    private String type;
    private Date date;
    private String direction;  // INBOUND, OUTBOUND
    private String channel;  // EMAIL, PHONE, MEETING, SOCIAL_MEDIA, ETC.
    private String subject;
    private String notes;
    private String userId;  // User who created the interaction
    private double duration;  // Duration in minutes
    private String outcome;  // SUCCESS, PENDING, FAILED
    private List<String> attachments;
    
    /**
     * Constructor for Interaction
     * @param interactionId Interaction ID
     * @param customerId Customer ID
     * @param type Interaction type
     * @param date Interaction date
     * @param direction Interaction direction
     * @param channel Interaction channel
     * @param subject Subject
     * @param notes Notes
     * @param userId User ID
     * @param duration Duration in minutes
     * @param outcome Interaction outcome
     */
    public Interaction(String interactionId, String customerId, String type, Date date,
                      String direction, String channel, String subject, String notes,
                      String userId, double duration, String outcome) {
        this.interactionId = interactionId;
        this.customerId = customerId;
        this.type = type;
        this.date = date;
        this.direction = direction;
        this.channel = channel;
        this.subject = subject;
        this.notes = notes;
        this.userId = userId;
        this.duration = duration;
        this.outcome = outcome;
        this.attachments = new ArrayList<>();
    }
    
    /**
     * Get interaction ID
     * @return Interaction ID
     */
    public String getInteractionId() {
        return interactionId;
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get interaction type
     * @return Interaction type
     */
    public String getType() {
        return type;
    }
    
    /**
     * Get interaction date
     * @return Interaction date
     */
    public Date getDate() {
        return date;
    }
    
    /**
     * Get interaction direction
     * @return Interaction direction
     */
    public String getDirection() {
        return direction;
    }
    
    /**
     * Get interaction channel
     * @return Interaction channel
     */
    public String getChannel() {
        return channel;
    }
    
    /**
     * Get subject
     * @return Subject
     */
    public String getSubject() {
        return subject;
    }
    
    /**
     * Get notes
     * @return Notes
     */
    public String getNotes() {
        return notes;
    }
    
    /**
     * Get user ID
     * @return User ID
     */
    public String getUserId() {
        return userId;
    }
    
    /**
     * Get duration
     * @return Duration in minutes
     */
    public double getDuration() {
        return duration;
    }
    
    /**
     * Get outcome
     * @return Interaction outcome
     */
    public String getOutcome() {
        return outcome;
    }
    
    /**
     * Get attachments
     * @return List of attachments
     */
    public List<String> getAttachments() {
        return attachments;
    }
    
    /**
     * Add attachment
     * @param attachment Attachment to add
     */
    public void addAttachment(String attachment) {
        attachments.add(attachment);
    }
    
    /**
     * Remove attachment
     * @param attachment Attachment to remove
     */
    public void removeAttachment(String attachment) {
        attachments.remove(attachment);
    }
    
    /**
     * Update outcome
     * @param outcome New outcome
     */
    public void updateOutcome(String outcome) {
        this.outcome = outcome;
    }
    
    /**
     * Get interaction summary
     * @return Interaction summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        
        sb.append("Interaction ID: ").append(interactionId).append("\n");
        sb.append("Customer ID: ").append(customerId).append("\n");
        sb.append("Type: ").append(type).append("\n");
        sb.append("Date: ").append(sdf.format(date)).append("\n");
        sb.append("Direction: ").append(direction).append("\n");
        sb.append("Channel: ").append(channel).append("\n");
        sb.append("Subject: ").append(subject).append("\n");
        sb.append("Notes: ").append(notes).append("\n");
        sb.append("User ID: ").append(userId).append("\n");
        sb.append("Duration: ").append(duration).append(" minutes\n");
        sb.append("Outcome: ").append(outcome).append("\n");
        
        if (!attachments.isEmpty()) {
            sb.append("Attachments: ").append(String.join(", ", attachments)).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Opportunity class represents a sales opportunity
 */
class Opportunity {
    private String opportunityId;
    private String customerId;
    private String name;
    private String description;
    private Date createdDate;
    private Date closeDate;
    private String status;
    private String stage;
    private double amount;
    private String probability;
    private String assignedSalesRep;
    private String leadSource;
    private String product;
    private List<String> tags;
    private List<String> nextSteps;
    private Date nextFollowUpDate;
    private String competitor;
    
    /**
     * Constructor for Opportunity
     * @param opportunityId Opportunity ID
     * @param customerId Customer ID
     * @param name Opportunity name
     * @param description Opportunity description
     * @param closeDate Expected close date
     * @param amount Opportunity amount
     * @param probability Probability of closing
     * @param assignedSalesRep Assigned sales representative
     * @param leadSource Lead source
     * @param product Product
     */
    public Opportunity(String opportunityId, String customerId, String name, String description,
                      Date closeDate, double amount, String probability, String assignedSalesRep,
                      String leadSource, String product) {
        this.opportunityId = opportunityId;
        this.customerId = customerId;
        this.name = name;
        this.description = description;
        this.createdDate = new Date();
        this.closeDate = closeDate;
        this.status = "Open";
        this.stage = "Prospecting";
        this.amount = amount;
        this.probability = probability;
        this.assignedSalesRep = assignedSalesRep;
        this.leadSource = leadSource;
        this.product = product;
        this.tags = new ArrayList<>();
        this.nextSteps = new ArrayList<>();
        this.nextFollowUpDate = null;
        this.competitor = null;
    }
    
    /**
     * Get opportunity ID
     * @return Opportunity ID
     */
    public String getOpportunityId() {
        return opportunityId;
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get opportunity name
     * @return Opportunity name
     */
    public String getName() {
        return name;
    }
    
    /**
     * Get opportunity description
     * @return Opportunity description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get created date
     * @return Created date
     */
    public Date getCreatedDate() {
        return createdDate;
    }
    
    /**
     * Get close date
     * @return Expected close date
     */
    public Date getCloseDate() {
        return closeDate;
    }
    
    /**
     * Get status
     * @return Opportunity status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get stage
     * @return Opportunity stage
     */
    public String getStage() {
        return stage;
    }
    
    /**
     * Get amount
     * @return Opportunity amount
     */
    public double getAmount() {
        return amount;
    }
    
    /**
     * Get probability
     * @return Probability of closing
     */
    public String getProbability() {
        return probability;
    }
    
    /**
     * Get assigned sales representative
     * @return Assigned sales representative
     */
    public String getAssignedSalesRep() {
        return assignedSalesRep;
    }
    
    /**
     * Get lead source
     * @return Lead source
     */
    public String getLeadSource() {
        return leadSource;
    }
    
    /**
     * Get product
     * @return Product
     */
    public String getProduct() {
        return product;
    }
    
    /**
     * Get tags
     * @return List of tags
     */
    public List<String> getTags() {
        return tags;
    }
    
    /**
     * Get next steps
     * @return List of next steps
     */
    public List<String> getNextSteps() {
        return nextSteps;
    }
    
    /**
     * Get next follow-up date
     * @return Next follow-up date
     */
    public Date getNextFollowUpDate() {
        return nextFollowUpDate;
    }
    
    /**
     * Get competitor
     * @return Competitor
     */
    public String getCompetitor() {
        return competitor;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update stage
     * @param stage New stage
     */
    public void updateStage(String stage) {
        this.stage = stage;
    }
    
    /**
     * Update amount
     * @param amount New amount
     */
    public void updateAmount(double amount) {
        this.amount = amount;
    }
    
    /**
     * Update probability
     * @param probability New probability
     */
    public void updateProbability(String probability) {
        this.probability = probability;
    }
    
    /**
     * Update close date
     * @param closeDate New close date
     */
    public void updateCloseDate(Date closeDate) {
        this.closeDate = closeDate;
    }
    
    /**
     * Update next follow-up date
     * @param date New next follow-up date
     */
    public void updateNextFollowUpDate(Date date) {
        this.nextFollowUpDate = date;
    }
    
    /**
     * Update competitor
     * @param competitor New competitor
     */
    public void updateCompetitor(String competitor) {
        this.competitor = competitor;
    }
    
    /**
     * Add tag
     * @param tag Tag to add
     */
    public void addTag(String tag) {
        if (!tags.contains(tag)) {
            tags.add(tag);
        }
    }
    
    /**
     * Remove tag
     * @param tag Tag to remove
     */
    public void removeTag(String tag) {
        tags.remove(tag);
    }
    
    /**
     * Add next step
     * @param step Next step to add
     */
    public void addNextStep(String step) {
        nextSteps.add(step);
    }
    
    /**
     * Remove next step
     * @param step Next step to remove
     */
    public void removeNextStep(String step) {
        nextSteps.remove(step);
    }
    
    /**
     * Get opportunity summary
     * @return Opportunity summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Opportunity ID: ").append(opportunityId).append("\n");
        sb.append("Customer ID: ").append(customerId).append("\n");
        sb.append("Name: ").append(name).append("\n");
        sb.append("Description: ").append(description).append("\n");
        sb.append("Created Date: ").append(sdf.format(createdDate)).append("\n");
        sb.append("Expected Close Date: ").append(sdf.format(closeDate)).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Stage: ").append(stage).append("\n");
        sb.append("Amount: $").append(String.format("%.2f", amount)).append("\n");
        sb.append("Probability: ").append(probability).append("\n");
        sb.append("Assigned Sales Rep: ").append(assignedSalesRep).append("\n");
        sb.append("Lead Source: ").append(leadSource).append("\n");
        sb.append("Product: ").append(product).append("\n");
        
        if (!tags.isEmpty()) {
            sb.append("Tags: ").append(String.join(", ", tags)).append("\n");
        }
        
        if (!nextSteps.isEmpty()) {
            sb.append("Next Steps:\n");
            for (int i = 0; i < nextSteps.size(); i++) {
                sb.append("  ").append(i + 1).append(". ").append(nextSteps.get(i)).append("\n");
            }
        }
        
        if (nextFollowUpDate != null) {
            sb.append("Next Follow-up Date: ").append(sdf.format(nextFollowUpDate)).append("\n");
        }
        
        if (competitor != null) {
            sb.append("Competitor: ").append(competitor).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Campaign class represents a marketing campaign
 */
class Campaign {
    private String campaignId;
    private String name;
    private String description;
    private String type;
    private Date startDate;
    private Date endDate;
    private String status;
    private String budget;
    private List<String> targetAudience;
    private List<String> includedCustomers;
    private String assignedSalesRep;
    private double expectedRevenue;
    private double actualRevenue;
    private int emailsSent;
    private int emailsOpened;
    private int emailsClicked;
    private int conversions;
    
    /**
     * Constructor for Campaign
     * @param campaignId Campaign ID
     * @param name Campaign name
     * @param description Campaign description
     * @param type Campaign type
     * @param startDate Start date
     * @param endDate End date
     * @param budget Campaign budget
     * @param expectedRevenue Expected revenue
     * @param assignedSalesRep Assigned sales representative
     */
    public Campaign(String campaignId, String name, String description, String type,
                   Date startDate, Date endDate, String budget, double expectedRevenue,
                   String assignedSalesRep) {
        this.campaignId = campaignId;
        this.name = name;
        this.description = description;
        this.type = type;
        this.startDate = startDate;
        this.endDate = endDate;
        this.status = "Planning";
        this.budget = budget;
        this.targetAudience = new ArrayList<>();
        this.includedCustomers = new ArrayList<>();
        this.assignedSalesRep = assignedSalesRep;
        this.expectedRevenue = expectedRevenue;
        this.actualRevenue = 0.0;
        this.emailsSent = 0;
        this.emailsOpened = 0;
        this.emailsClicked = 0;
        this.conversions = 0;
    }
    
    /**
     * Get campaign ID
     * @return Campaign ID
     */
    public String getCampaignId() {
        return campaignId;
    }
    
    /**
     * Get campaign name
     * @return Campaign name
     */
    public String getName() {
        return name;
    }
    
    /**
     * Get campaign description
     * @return Campaign description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get campaign type
     * @return Campaign type
     */
    public String getType() {
        return type;
    }
    
    /**
     * Get start date
     * @return Campaign start date
     */
    public Date getStartDate() {
        return startDate;
    }
    
    /**
     * Get end date
     * @return Campaign end date
     */
    public Date getEndDate() {
        return endDate;
    }
    
    /**
     * Get status
     * @return Campaign status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get budget
     * @return Campaign budget
     */
    public String getBudget() {
        return budget;
    }
    
    /**
     * Get target audience
     * @return Target audience criteria
     */
    public List<String> getTargetAudience() {
        return targetAudience;
    }
    
    /**
     * Get included customers
     * @return List of included customers
     */
    public List<String> getIncludedCustomers() {
        return includedCustomers;
    }
    
    /**
     * Get assigned sales representative
     * @return Assigned sales representative
     */
    public String getAssignedSalesRep() {
        return assignedSalesRep;
    }
    
    /**
     * Get expected revenue
     * @return Expected revenue
     */
    public double getExpectedRevenue() {
        return expectedRevenue;
    }
    
    /**
     * Get actual revenue
     * @return Actual revenue
     */
    public double getActualRevenue() {
        return actualRevenue;
    }
    
    /**
     * Get emails sent
     * @return Number of emails sent
     */
    public int getEmailsSent() {
        return emailsSent;
    }
    
    /**
     * Get emails opened
     * @return Number of emails opened
     */
    public int getEmailsOpened() {
        return emailsOpened;
    }
    
    /**
     * Get emails clicked
     * @return Number of emails clicked
     */
    public int getEmailsClicked() {
        return emailsClicked;
    }
    
    /**
     * Get conversions
     * @return Number of conversions
     */
    public int getConversions() {
        return conversions;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update actual revenue
     * @param amount Amount to add
     */
    public void updateActualRevenue(double amount) {
        this.actualRevenue += amount;
    }
    
    /**
     * Increment emails sent
     */
    public void incrementEmailsSent() {
        this.emailsSent++;
    }
    
    /**
     * Increment emails opened
     */
    public void incrementEmailsOpened() {
        this.emailsOpened++;
    }
    
    /**
     * Increment emails clicked
     */
    public void incrementEmailsClicked() {
        this.emailsClicked++;
    }
    
    /**
     * Increment conversions
     */
    public void incrementConversions() {
        this.conversions++;
    }
    
    /**
     * Add target audience criterion
     * @param criterion Criterion to add
     */
    public void addTargetAudience(String criterion) {
        if (!targetAudience.contains(criterion)) {
            targetAudience.add(criterion);
        }
    }
    
    /**
     * Remove target audience criterion
     * @param criterion Criterion to remove
     */
    public void removeTargetAudience(String criterion) {
        targetAudience.remove(criterion);
    }
    
    /**
     * Add included customer
     * @param customerId Customer ID to include
     */
    public void addIncludedCustomer(String customerId) {
        if (!includedCustomers.contains(customerId)) {
            includedCustomers.add(customerId);
        }
    }
    
    /**
     * Remove included customer
     * @param customerId Customer ID to remove
     */
    public void removeIncludedCustomer(String customerId) {
        includedCustomers.remove(customerId);
    }
    
    /**
     * Get campaign summary
     * @return Campaign summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Campaign ID: ").append(campaignId).append("\n");
        sb.append("Name: ").append(name).append("\n");
        sb.append("Description: ").append(description).append("\n");
        sb.append("Type: ").append(type).append("\n");
        sb.append("Start Date: ").append(sdf.format(startDate)).append("\n");
        sb.append("End Date: ").append(sdf.format(endDate)).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Budget: ").append(budget).append("\n");
        sb.append("Expected Revenue: $").append(String.format("%.2f", expectedRevenue)).append("\n");
        sb.append("Actual Revenue: $").append(String.format("%.2f", actualRevenue)).append("\n");
        sb.append("Assigned Sales Rep: ").append(assignedSalesRep).append("\n");
        sb.append("Email Metrics:\n");
        sb.append("  Emails Sent: ").append(emailsSent).append("\n");
        sb.append("  Emails Opened: ").append(emailsOpened).append("\n");
        sb.append("  Emails Clicked: ").append(emailsClicked).append("\n");
        sb.append("  Conversions: ").append(conversions).append("\n");
        
        if (!targetAudience.isEmpty()) {
            sb.append("Target Audience:\n");
            for (String criterion : targetAudience) {
                sb.append("  - ").append(criterion).append("\n");
            }
        }
        
        if (!includedCustomers.isEmpty()) {
            sb.append("Included Customers: ").append(includedCustomers.size()).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Task class represents a task or reminder in the CRM system
 */
class Task {
    private String taskId;
    private String title;
    private String description;
    private Date dueDate;
    private Date reminderDate;
    private String priority;
    private String status;
    private String assignedTo;
    private String relatedToType;
    private String relatedToId;
    private String createdBy;
    
    /**
     * Constructor for Task
     * @param taskId Task ID
     * @param title Task title
     * @param description Task description
     * @param dueDate Due date
     * @param reminderDate Reminder date
     * @param priority Task priority
     * @param assignedTo Assigned to
     * @param relatedToType Related entity type
     * @param relatedToId Related entity ID
     * @param createdBy Created by
     */
    public Task(String taskId, String title, String description, Date dueDate, Date reminderDate,
               String priority, String assignedTo, String relatedToType, String relatedToId,
               String createdBy) {
        this.taskId = taskId;
        this.title = title;
        this.description = description;
        this.dueDate = dueDate;
        this.reminderDate = reminderDate;
        this.priority = priority;
        this.status = "Open";
        this.assignedTo = assignedTo;
        this.relatedToType = relatedToType;
        this.relatedToId = relatedToId;
        this.createdBy = createdBy;
    }
    
    /**
     * Get task ID
     * @return Task ID
     */
    public String getTaskId() {
        return taskId;
    }
    
    /**
     * Get title
     * @return Task title
     */
    public String getTitle() {
        return title;
    }
    
    /**
     * Get description
     * @return Task description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get due date
     * @return Due date
     */
    public Date getDueDate() {
        return dueDate;
    }
    
    /**
     * Get reminder date
     * @return Reminder date
     */
    public Date getReminderDate() {
        return reminderDate;
    }
    
    /**
     * Get priority
     * @return Task priority
     */
    public String getPriority() {
        return priority;
    }
    
    /**
     * Get status
     * @return Task status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get assigned to
     * @return Assigned to
     */
    public String getAssignedTo() {
        return assignedTo;
    }
    
    /**
     * Get related to type
     * @return Related entity type
     */
    public String getRelatedToType() {
        return relatedToType;
    }
    
    /**
     * Get related to ID
     * @return Related entity ID
     */
    public String getRelatedToId() {
        return relatedToId;
    }
    
    /**
     * Get created by
     * @return Created by
     */
    public String getCreatedBy() {
        return createdBy;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update due date
     * @param dueDate New due date
     */
    public void updateDueDate(Date dueDate) {
        this.dueDate = dueDate;
    }
    
    /**
     * Update reminder date
     * @param reminderDate New reminder date
     */
    public void updateReminderDate(Date reminderDate) {
        this.reminderDate = reminderDate;
    }
    
    /**
     * Get task summary
     * @return Task summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        
        sb.append("Task ID: ").append(taskId).append("\n");
        sb.append("Title: ").append(title).append("\n");
        sb.append("Description: ").append(description).append("\n");
        sb.append("Due Date: ").append(sdf.format(dueDate)).append("\n");
        sb.append("Reminder Date: ").append(sdf.format(reminderDate)).append("\n");
        sb.append("Priority: ").append(priority).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Assigned To: ").append(assignedTo).append("\n");
        sb.append("Related To: ").append(relatedToType).append(" (").append(relatedToId).append(")\n");
        sb.append("Created By: ").append(createdBy).append("\n");
        
        return sb.toString();
    }
}

/**
 * CRMSystem class manages the entire CRM system
 */
public class CRMSystem {
    private String systemName;
    private String version;
    private List<Customer> customers;
    private Map<String, Customer> customerMap;
    private List<Interaction> interactions;
    private Map<String, Interaction> interactionMap;
    private List<Opportunity> opportunities;
    private Map<String, Opportunity> opportunityMap;
    private List<Campaign> campaigns;
    private Map<String, Campaign> campaignMap;
    private List<Task> tasks;
    private Map<String, Task> taskMap;
    private Map<String, String> users;
    private Map<String, Integer> userPermissions;
    
    /**
     * Constructor for CRMSystem
     * @param systemName System name
     * @param version System version
     */
    public CRMSystem(String systemName, String version) {
        this.systemName = systemName;
        this.version = version;
        this.customers = new ArrayList<>();
        this.customerMap = new HashMap<>();
        this.interactions = new ArrayList<>();
        this.interactionMap = new HashMap<>();
        this.opportunities = new ArrayList<>();
        this.opportunityMap = new HashMap<>();
        this.campaigns = new ArrayList<>();
        this.campaignMap = new HashMap<>();
        this.tasks = new ArrayList<>();
        this.taskMap = new HashMap<>();
        this.users = new HashMap<>();
        this.userPermissions = new HashMap<>();
    }
    
    /**
     * Get system name
     * @return System name
     */
    public String getSystemName() {
        return systemName;
    }
    
    /**
     * Get version
     * @return System version
     */
    public String getVersion() {
        return version;
    }
    
    /**
     * Get customers
     * @return List of customers
     */
    public List<Customer> getCustomers() {
        return customers;
    }
    
    /**
     * Get interactions
     * @return List of interactions
     */
    public List<Interaction> getInteractions() {
        return interactions;
    }
    
    /**
     * Get opportunities
     * @return List of opportunities
     */
    public List<Opportunity> getOpportunities() {
        return opportunities;
    }
    
    /**
     * Get campaigns
     * @return List of campaigns
     */
    public List<Campaign> getCampaigns() {
        return campaigns;
    }
    
    /**
     * Get tasks
     * @return List of tasks
     */
    public List<Task> getTasks() {
        return tasks;
    }
    
    /**
     * Get users
     * @return Map of users
     */
    public Map<String, String> getUsers() {
        return users;
    }
    
    /**
     * Add a customer
     * @param customer Customer to add
     * @return True if added successfully, false otherwise
     */
    public boolean addCustomer(Customer customer) {
        if (customerMap.containsKey(customer.getCustomerId())) {
            return false;
        }
        
        customers.add(customer);
        customerMap.put(customer.getCustomerId(), customer);
        return true;
    }
    
    /**
     * Add an interaction
     * @param interaction Interaction to add
     * @return True if added successfully, false otherwise
     */
    public boolean addInteraction(Interaction interaction) {
        if (interactionMap.containsKey(interaction.getInteractionId())) {
            return false;
        }
        
        interactions.add(interaction);
        interactionMap.put(interaction.getInteractionId(), interaction);
        return true;
    }
    
    /**
     * Add an opportunity
     * @param opportunity Opportunity to add
     * @return True if added successfully, false otherwise
     */
    public boolean addOpportunity(Opportunity opportunity) {
        if (opportunityMap.containsKey(opportunity.getOpportunityId())) {
            return false;
        }
        
        opportunities.add(opportunity);
        opportunityMap.put(opportunity.getOpportunityId(), opportunity);
        return true;
    }
    
    /**
     * Add a campaign
     * @param campaign Campaign to add
     * @return True if added successfully, false otherwise
     */
    public boolean addCampaign(Campaign campaign) {
        if (campaignMap.containsKey(campaign.getCampaignId())) {
            return false;
        }
        
        campaigns.add(campaign);
        campaignMap.put(campaign.getCampaignId(), campaign);
        return true;
    }
    
    /**
     * Add a task
     * @param task Task to add
     * @return True if added successfully, false otherwise
     */
    public boolean addTask(Task task) {
        if (taskMap.containsKey(task.getTaskId())) {
            return false;
        }
        
        tasks.add(task);
        taskMap.put(task.getTaskId(), task);
        return true;
    }
    
    /**
     * Add a user
     * @param userId User ID
     * @param userName User name
     * @param permissionLevel Permission level
     * @return True if added successfully, false otherwise
     */
    public boolean addUser(String userId, String userName, int permissionLevel) {
        if (users.containsKey(userId)) {
            return false;
        }
        
        users.put(userId, userName);
        userPermissions.put(userId, permissionLevel);
        return true;
    }
    
    /**
     * Get customer by ID
     * @param customerId Customer ID
     * @return Customer if found, null otherwise
     */
    public Customer getCustomerById(String customerId) {
        return customerMap.get(customerId);
    }
    
    /**
     * Get interaction by ID
     * @param interactionId Interaction ID
     * @return Interaction if found, null otherwise
     */
    public Interaction getInteractionById(String interactionId) {
        return interactionMap.get(interactionId);
    }
    
    /**
     * Get opportunity by ID
     * @param opportunityId Opportunity ID
     * @return Opportunity if found, null otherwise
     */
    public Opportunity getOpportunityById(String opportunityId) {
        return opportunityMap.get(opportunityId);
    }
    
    /**
     * Get campaign by ID
     * @param campaignId Campaign ID
     * @return Campaign if found, null otherwise
     */
    public Campaign getCampaignById(String campaignId) {
        return campaignMap.get(campaignId);
    }
    
    /**
     * Get task by ID
     * @param taskId Task ID
     * @return Task if found, null otherwise
     */
    public Task getTaskById(String taskId) {
        return taskMap.get(taskId);
    }
    
    /**
     * Get customer interactions
     * @param customerId Customer ID
     * @return List of customer interactions
     */
    public List<Interaction> getCustomerInteractions(String customerId) {
        List<Interaction> result = new ArrayList<>();
        
        for (Interaction interaction : interactions) {
            if (interaction.getCustomerId().equals(customerId)) {
                result.add(interaction);
            }
        }
        
        return result;
    }
    
    /**
     * Get customer opportunities
     * @param customerId Customer ID
     * @return List of customer opportunities
     */
    public List<Opportunity> getCustomerOpportunities(String customerId) {
        List<Opportunity> result = new ArrayList<>();
        
        for (Opportunity opportunity : opportunities) {
            if (opportunity.getCustomerId().equals(customerId)) {
                result.add(opportunity);
            }
        }
        
        return result;
    }
    
    /**
     * Get customer tasks
     * @param customerId Customer ID
     * @return List of customer tasks
     */
    public List<Task> getCustomerTasks(String customerId) {
        List<Task> result = new ArrayList<>();
        
        for (Task task : tasks) {
            if (task.getRelatedToId().equals(customerId) && task.getRelatedToType().equals("Customer")) {
                result.add(task);
            }
        }
        
        return result;
    }
    
    /**
     * Get user tasks
     * @param userId User ID
     * @return List of user tasks
     */
    public List<Task> getUserTasks(String userId) {
        List<Task> result = new ArrayList<>();
        
        for (Task task : tasks) {
            if (task.getAssignedTo().equals(userId)) {
                result.add(task);
            }
        }
        
        return result;
    }
    
    /**
     * Get sales pipeline by stage
     * @param stage Opportunity stage
     * @return List of opportunities in the stage
     */
    public List<Opportunity> getSalesPipelineByStage(String stage) {
        List<Opportunity> result = new ArrayList<>();
        
        for (Opportunity opportunity : opportunities) {
            if (opportunity.getStage().equals(stage)) {
                result.add(opportunity);
            }
        }
        
        return result;
    }
    
    /**
     * Get campaign customers
     * @param campaignId Campaign ID
     * @return List of customers in the campaign
     */
    public List<Customer> getCampaignCustomers(String campaignId) {
        Campaign campaign = getCampaignById(campaignId);
        if (campaign == null) {
            return new ArrayList<>();
        }
        
        List<Customer> result = new ArrayList<>();
        List<String> customerIds = campaign.getIncludedCustomers();
        
        for (String customerId : customerIds) {
            Customer customer = getCustomerById(customerId);
            if (customer != null) {
                result.add(customer);
            }
        }
        
        return result;
    }
    
    /**
     * Get customer by email
     * @param email Email address
     * @return Customer if found, null otherwise
     */
    public Customer getCustomerByEmail(String email) {
        for (Customer customer : customers) {
            if (customer.getEmail().equals(email)) {
                return customer;
            }
        }
        
        return null;
    }
    
    /**
     * Get customers by city
     * @param city City
     * @return List of customers in the city
     */
    public List<Customer> getCustomersByCity(String city) {
        List<Customer> result = new ArrayList<>();
        
        for (Customer customer : customers) {
            if (customer.getCity().equals(city)) {
                result.add(customer);
            }
        }
        
        return result;
    }
    
    /**
     * Get customers by type
     * @param customerType Customer type
     * @return List of customers of the type
     */
    public List<Customer> getCustomersByType(String customerType) {
        List<Customer> result = new ArrayList<>();
        
        for (Customer customer : customers) {
            if (customer.getCustomerType().equals(customerType)) {
                result.add(customer);
            }
        }
        
        return result;
    }
    
    /**
     * Get customers by assigned sales rep
     * @param assignedSalesRep Assigned sales representative
     * @return List of customers assigned to the sales rep
     */
    public List<Customer> getCustomersByAssignedSalesRep(String assignedSalesRep) {
        List<Customer> result = new ArrayList<>();
        
        for (Customer customer : customers) {
            if (customer.getAssignedSalesRep().equals(assignedSalesRep)) {
                result.add(customer);
            }
        }
        
        return result;
    }
    
    /**
     * Get opportunities by assigned sales rep
     * @param assignedSalesRep Assigned sales representative
     * @return List of opportunities assigned to the sales rep
     */
    public List<Opportunity> getOpportunitiesByAssignedSalesRep(String assignedSalesRep) {
        List<Opportunity> result = new ArrayList<>();
        
        for (Opportunity opportunity : opportunities) {
            if (opportunity.getAssignedSalesRep().equals(assignedSalesRep)) {
                result.add(opportunity);
            }
        }
        
        return result;
    }
    
    /**
     * Get campaigns by assigned sales rep
     * @param assignedSalesRep Assigned sales representative
     * @return List of campaigns assigned to the sales rep
     */
    public List<Campaign> getCampaignsByAssignedSalesRep(String assignedSalesRep) {
        List<Campaign> result = new ArrayList<>();
        
        for (Campaign campaign : campaigns) {
            if (campaign.getAssignedSalesRep().equals(assignedSalesRep)) {
                result.add(campaign);
            }
        }
        
        return result;
    }
    
    /**
     * Search customers by keyword
     * @param keyword Search keyword
     * @return List of matching customers
     */
    public List<Customer> searchCustomers(String keyword) {
        List<Customer> result = new ArrayList<>();
        String lowercaseKeyword = keyword.toLowerCase();
        
        for (Customer customer : customers) {
            boolean matches = false;
            
            // Check name
            if (customer.getFirstName().toLowerCase().contains(lowercaseKeyword) ||
                customer.getLastName().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check email
            if (customer.getEmail().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check phone
            if (customer.getPhone().contains(keyword)) {
                matches = true;
            }
            
            // Check company (if available in custom fields)
            String company = customer.getCustomFields().get("Company");
            if (company != null && company.toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check tags
            for (String tag : customer.getTags()) {
                if (tag.toLowerCase().contains(lowercaseKeyword)) {
                    matches = true;
                    break;
                }
            }
            
            if (matches) {
                result.add(customer);
            }
        }
        
        return result;
    }
    
    /**
     * Get customer statistics
     * @return Map of customer statistics
     */
    public Map<String, Object> getCustomerStatistics() {
        Map<String, Object> stats = new HashMap<>();
        
        // Total customers
        stats.put("totalCustomers", customers.size());
        
        // Customers by type
        Map<String, Integer> customersByType = new HashMap<>();
        for (Customer customer : customers) {
            String type = customer.getCustomerType();
            customersByType.put(type, customersByType.getOrDefault(type, 0) + 1);
        }
        stats.put("customersByType", customersByType);
        
        // Customers by city
        Map<String, Integer> customersByCity = new HashMap<>();
        for (Customer customer : customers) {
            String city = customer.getCity();
            customersByCity.put(city, customersByCity.getOrDefault(city, 0) + 1);
        }
        stats.put("customersByCity", customersByCity);
        
        // Customers by status
        Map<String, Integer> customersByStatus = new HashMap<>();
        for (Customer customer : customers) {
            String status = customer.getStatus();
            customersByStatus.put(status, customersByStatus.getOrDefault(status, 0) + 1);
        }
        stats.put("customersByStatus", customersByStatus);
        
        // Top spenders
        List<Customer> topSpenders = new ArrayList<>(customers);
        topSpenders.sort((a, b) -> Double.compare(b.getTotalSpent(), a.getTotalSpent()));
        
        List<Map<String, Object>> topSpendersInfo = new ArrayList<>();
        for (int i = 0; i < Math.min(5, topSpenders.size()); i++) {
            Customer customer = topSpenders.get(i);
            Map<String, Object> spenderInfo = new HashMap<>();
            spenderInfo.put("name", customer.getFullName());
            spenderInfo.put("totalSpent", customer.getTotalSpent());
            spenderInfo.put("orderCount", customer.getOrderCount());
            topSpendersInfo.add(spenderInfo);
        }
        stats.put("topSpenders", topSpendersInfo);
        
        return stats;
    }
    
    /**
     * Get sales pipeline statistics
     * @return Map of sales pipeline statistics
     */
    public Map<String, Object> getSalesPipelineStatistics() {
        Map<String, Object> stats = new HashMap<>();
        
        // Total opportunities
        stats.put("totalOpportunities", opportunities.size());
        
        // Opportunities by stage
        Map<String, Integer> opportunitiesByStage = new HashMap<>();
        double totalPipelineValue = 0.0;
        double weightedPipelineValue = 0.0;
        
        for (Opportunity opportunity : opportunities) {
            String stage = opportunity.getStage();
            opportunitiesByStage.put(stage, opportunitiesByStage.getOrDefault(stage, 0) + 1);
            
            totalPipelineValue += opportunity.getAmount();
            
            // Calculate weighted value based on probability
            double probabilityFactor = 0.0;
            if (opportunity.getProbability().contains("10")) {
                probabilityFactor = 0.1;
            } else if (opportunity.getProbability().contains("25")) {
                probabilityFactor = 0.25;
            } else if (opportunity.getProbability().contains("50")) {
                probabilityFactor = 0.5;
            } else if (opportunity.getProbability().contains("75")) {
                probabilityFactor = 0.75;
            } else if (opportunity.getProbability().contains("90")) {
                probabilityFactor = 0.9;
            }
            
            weightedPipelineValue += opportunity.getAmount() * probabilityFactor;
        }
        
        stats.put("opportunitiesByStage", opportunitiesByStage);
        stats.put("totalPipelineValue", totalPipelineValue);
        stats.put("weightedPipelineValue", weightedPipelineValue);
        
        // Average deal size
        double averageDealSize = opportunities.isEmpty() ? 0.0 : totalPipelineValue / opportunities.size();
        stats.put("averageDealSize", averageDealSize);
        
        // Largest opportunities
        List<Opportunity> largestOpportunities = new ArrayList<>(opportunities);
        largestOpportunities.sort((a, b) -> Double.compare(b.getAmount(), a.getAmount()));
        
        List<Map<String, Object>> largestOpportunitiesInfo = new ArrayList<>();
        for (int i = 0; i < Math.min(5, largestOpportunities.size()); i++) {
            Opportunity opportunity = largestOpportunities.get(i);
            Customer customer = getCustomerById(opportunity.getCustomerId());
            
            Map<String, Object> opportunityInfo = new HashMap<>();
            opportunityInfo.put("name", opportunity.getName());
            opportunityInfo.put("customerName", customer != null ? customer.getFullName() : "Unknown");
            opportunityInfo.put("amount", opportunity.getAmount());
            opportunityInfo.put("stage", opportunity.getStage());
            opportunityInfo.put("probability", opportunity.getProbability());
            largestOpportunitiesInfo.add(opportunityInfo);
        }
        stats.put("largestOpportunities", largestOpportunitiesInfo);
        
        return stats;
    }
    
    /**
     * Get campaign statistics
     * @return Map of campaign statistics
     */
    public Map<String, Object> getCampaignStatistics() {
        Map<String, Object> stats = new HashMap<>();
        
        // Total campaigns
        stats.put("totalCampaigns", campaigns.size());
        
        // Campaigns by type
        Map<String, Integer> campaignsByType = new HashMap<>();
        Map<String, Integer> campaignsByStatus = new HashMap<>();
        double totalExpectedRevenue = 0.0;
        double totalActualRevenue = 0.0;
        
        for (Campaign campaign : campaigns) {
            String type = campaign.getType();
            String status = campaign.getStatus();
            
            campaignsByType.put(type, campaignsByType.getOrDefault(type, 0) + 1);
            campaignsByStatus.put(status, campaignsByStatus.getOrDefault(status, 0) + 1);
            
            totalExpectedRevenue += campaign.getExpectedRevenue();
            totalActualRevenue += campaign.getActualRevenue();
        }
        
        stats.put("campaignsByType", campaignsByType);
        stats.put("campaignsByStatus", campaignsByStatus);
        stats.put("totalExpectedRevenue", totalExpectedRevenue);
        stats.put("totalActualRevenue", totalActualRevenue);
        
        // ROI calculation
        double roi = 0.0;
        if (totalExpectedRevenue > 0) {
            roi = (totalActualRevenue / totalExpectedRevenue) * 100;
        }
        stats.put("campaignROI", roi);
        
        // Top performing campaigns
        List<Campaign> topPerformingCampaigns = new ArrayList<>(campaigns);
        topPerformingCampaigns.sort((a, b) -> Double.compare(b.getActualRevenue(), a.getActualRevenue()));
        
        List<Map<String, Object>> topPerformingCampaignsInfo = new ArrayList<>();
        for (int i = 0; i < Math.min(5, topPerformingCampaigns.size()); i++) {
            Campaign campaign = topPerformingCampaigns.get(i);
            
            Map<String, Object> campaignInfo = new HashMap<>();
            campaignInfo.put("name", campaign.getName());
            campaignInfo.put("type", campaign.getType());
            campaignInfo.put("status", campaign.getStatus());
            campaignInfo.put("actualRevenue", campaign.getActualRevenue());
            campaignInfo.put("conversions", campaign.getConversions());
            topPerformingCampaignsInfo.add(campaignInfo);
        }
        stats.put("topPerformingCampaigns", topPerformingCampaignsInfo);
        
        return stats;
    }
    
    /**
     * Generate CRM report
     * @return CRM report string
     */
    public String generateCRMReport() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        
        sb.append("=====================================\n");
        sb.append("           CRM SYSTEM REPORT\n");
        sb.append("=====================================\n");
        sb.append("System: ").append(systemName).append("\n");
        sb.append("Version: ").append(version).append("\n");
        sb.append("Report Date: ").append(sdf.format(new Date())).append("\n\n");
        
        // Customer statistics
        sb.append("CUSTOMER STATISTICS\n");
        sb.append("-------------------\n");
        
        Map<String, Object> customerStats = getCustomerStatistics();
        sb.append("Total Customers: ").append(customerStats.get("totalCustomers")).append("\n");
        
        @SuppressWarnings("unchecked")
        Map<String, Integer> customersByType = (Map<String, Integer>) customerStats.get("customersByType");
        sb.append("Customers by Type:\n");
        for (Map.Entry<String, Integer> entry : customersByType.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        @SuppressWarnings("unchecked")
        Map<String, Integer> customersByStatus = (Map<String, Integer>) customerStats.get("customersByStatus");
        sb.append("\nCustomers by Status:\n");
        for (Map.Entry<String, Integer> entry : customersByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        @SuppressWarnings("unchecked")
        List<Map<String, Object>> topSpenders = (List<Map<String, Object>>) customerStats.get("topSpenders");
        sb.append("\nTop 5 Customers by Spending:\n");
        for (int i = 0; i < topSpenders.size(); i++) {
            Map<String, Object> spender = topSpenders.get(i);
            sb.append("  ").append(i + 1).append(". ").append(spender.get("name"))
              .append(" ($").append(String.format("%.2f", (Double) spender.get("totalSpent")))
              .append(")\n");
        }
        
        sb.append("\n");
        
        // Sales pipeline statistics
        sb.append("SALES PIPELINE STATISTICS\n");
        sb.append("-------------------------\n");
        
        Map<String, Object> pipelineStats = getSalesPipelineStatistics();
        sb.append("Total Opportunities: ").append(pipelineStats.get("totalOpportunities")).append("\n");
        sb.append("Total Pipeline Value: $").append(String.format("%.2f", (Double) pipelineStats.get("totalPipelineValue"))).append("\n");
        sb.append("Weighted Pipeline Value: $").append(String.format("%.2f", (Double) pipelineStats.get("weightedPipelineValue"))).append("\n");
        sb.append("Average Deal Size: $").append(String.format("%.2f", (Double) pipelineStats.get("averageDealSize"))).append("\n");
        
        @SuppressWarnings("unchecked")
        Map<String, Integer> opportunitiesByStage = (Map<String, Integer>) pipelineStats.get("opportunitiesByStage");
        sb.append("\nOpportunities by Stage:\n");
        for (Map.Entry<String, Integer> entry : opportunitiesByStage.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        @SuppressWarnings("unchecked")
        List<Map<String, Object>> largestOpportunities = (List<Map<String, Object>>) pipelineStats.get("largestOpportunities");
        sb.append("\nTop 5 Largest Opportunities:\n");
        for (int i = 0; i < largestOpportunities.size(); i++) {
            Map<String, Object> opportunity = largestOpportunities.get(i);
            sb.append("  ").append(i + 1).append(". ").append(opportunity.get("name"))
              .append(" (").append(opportunity.get("customerName")).append(") - $")
              .append(String.format("%.2f", (Double) opportunity.get("amount"))).append("\n");
        }
        
        sb.append("\n");
        
        // Campaign statistics
        sb.append("CAMPAIGN STATISTICS\n");
        sb.append("-------------------\n");
        
        Map<String, Object> campaignStats = getCampaignStatistics();
        sb.append("Total Campaigns: ").append(campaignStats.get("totalCampaigns")).append("\n");
        sb.append("Total Expected Revenue: $").append(String.format("%.2f", (Double) campaignStats.get("totalExpectedRevenue"))).append("\n");
        sb.append("Total Actual Revenue: $").append(String.format("%.2f", (Double) campaignStats.get("totalActualRevenue"))).append("\n");
        sb.append("Campaign ROI: ").append(String.format("%.2f", (Double) campaignStats.get("campaignROI"))).append("%\n");
        
        @SuppressWarnings("unchecked")
        Map<String, Integer> campaignsByType = (Map<String, Integer>) campaignStats.get("campaignsByType");
        sb.append("\nCampaigns by Type:\n");
        for (Map.Entry<String, Integer> entry : campaignsByType.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        @SuppressWarnings("unchecked")
        List<Map<String, Object>> topPerformingCampaigns = (List<Map<String, Object>>) campaignStats.get("topPerformingCampaigns");
        sb.append("\nTop 5 Performing Campaigns:\n");
        for (int i = 0; i < topPerformingCampaigns.size(); i++) {
            Map<String, Object> campaign = topPerformingCampaigns.get(i);
            sb.append("  ").append(i + 1).append(". ").append(campaign.get("name"))
              .append(" ($").append(String.format("%.2f", (Double) campaign.get("actualRevenue")))
              .append(")\n");
        }
        
        sb.append("\n");
        
        // Activity summary
        sb.append("ACTIVITY SUMMARY\n");
        sb.append("---------------\n");
        sb.append("Total Interactions: ").append(interactions.size()).append("\n");
        sb.append("Total Tasks: ").append(tasks.size()).append("\n");
        sb.append("Total Users: ").append(users.size()).append("\n");
        
        // Tasks by status
        Map<String, Integer> tasksByStatus = new HashMap<>();
        for (Task task : tasks) {
            String status = task.getStatus();
            tasksByStatus.put(status, tasksByStatus.getOrDefault(status, 0) + 1);
        }
        
        sb.append("\nTasks by Status:\n");
        for (Map.Entry<String, Integer> entry : tasksByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        return sb.toString();
    }
    
    /**
     * Save CRM data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("CRM SYSTEM DATA EXPORT");
            writer.println("========================");
            writer.println("System: " + systemName);
            writer.println("Version: " + version);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export customers
            writer.println("CUSTOMERS");
            writer.println("--------");
            for (Customer customer : customers) {
                writer.println(customer.getSummary());
                writer.println();
            }
            
            // Export interactions
            writer.println("INTERACTIONS");
            writer.println("------------");
            for (Interaction interaction : interactions) {
                writer.println(interaction.getSummary());
                writer.println();
            }
            
            // Export opportunities
            writer.println("OPPORTUNITIES");
            writer.println("-----------");
            for (Opportunity opportunity : opportunities) {
                writer.println(opportunity.getSummary());
                writer.println();
            }
            
            // Export campaigns
            writer.println("CAMPAIGNS");
            writer.println("---------");
            for (Campaign campaign : campaigns) {
                writer.println(campaign.getSummary());
                writer.println();
            }
            
            // Export tasks
            writer.println("TASKS");
            writer.println("----");
            for (Task task : tasks) {
                writer.println(task.getSummary());
                writer.println();
            }
            
            writer.close();
            return true;
        } catch (IOException e) {
            System.err.println("Error saving to file: " + e.getMessage());
            return false;
        }
    }
    
    /**
     * Main method for demonstration
     */
    public static void main(String[] args) {
        // Create a new CRM system
        CRMSystem system = new CRMSystem("Enterprise CRM", "2.0");
        
        // Add users
        system.addUser("U001", "John Smith", 3);  // Admin
        system.addUser("U002", "Sarah Johnson", 2);  // Sales Manager
        system.addUser("U003", "Mike Davis", 1);  // Sales Rep
        
        // Create customers
        Customer customer1 = new Customer("C001", "Alice", "Williams", "alice.williams@example.com",
                                         "(555) 123-4567", "123 Main St", "New York", "NY", "USA", "10001",
                                         "Enterprise", "Website", "U003");
        customer1.addTag("VIP");
        customer1.addTag("Technology");
        customer1.addInterest("Cloud Solutions");
        customer1.addInterest("Data Analytics");
        customer1.addCustomField("Company", "Tech Corp");
        customer1.addCustomField("Industry", "Technology");
        customer1.updateTotalSpent(25000.00);
        customer1.incrementOrderCount();
        customer1.incrementOrderCount();
        
        Customer customer2 = new Customer("C002", "Bob", "Johnson", "bob.johnson@example.com",
                                         "(555) 987-6543", "456 Oak Ave", "San Francisco", "CA", "USA", "94107",
                                         "Small Business", "Referral", "U002");
        customer2.addTag("New");
        customer2.addTag("Retail");
        customer2.addInterest("Inventory Management");
        customer2.addCustomField("Company", "Retail Store");
        customer2.addCustomField("Industry", "Retail");
        customer2.updateTotalSpent(8500.00);
        customer2.incrementOrderCount();
        
        Customer customer3 = new Customer("C003", "Carol", "Martinez", "carol.martinez@example.com",
                                         "(555) 456-7890", "789 Pine Rd", "Chicago", "IL", "USA", "60607",
                                         "Startup", "Trade Show", "U003");
        customer3.addTag("Growth");
        customer3.addTag("Finance");
        customer3.addInterest("Financial Analytics");
        customer3.addInterest("Reporting");
        customer3.addCustomField("Company", "Finance Startup");
        customer3.addCustomField("Industry", "Finance");
        customer3.updateTotalSpent(12500.00);
        customer3.incrementOrderCount();
        customer3.incrementOrderCount();
        customer3.incrementOrderCount();
        
        Customer customer4 = new Customer("C004", "David", "Brown", "david.brown@example.com",
                                         "(555) 789-0123", "321 Elm Blvd", "Los Angeles", "CA", "USA", "90001",
                                         "Enterprise", "Email Campaign", "U002");
        customer4.addTag("Potential");
        customer4.addTag("Healthcare");
        customer4.addInterest("Patient Management");
        customer4.addCustomField("Company", "Healthcare Inc");
        customer4.addCustomField("Industry", "Healthcare");
        
        Customer customer5 = new Customer("C005", "Emily", "Taylor", "emily.taylor@example.com",
                                         "(555) 234-5678", "654 Maple Dr", "Seattle", "WA", "USA", "98101",
                                         "Small Business", "Cold Call", "U003");
        customer5.addTag("Follow-up");
        customer5.addTag("Education");
        customer5.addInterest("Learning Management");
        customer5.addCustomField("Company", "Edu Solutions");
        customer5.addCustomField("Industry", "Education");
        
        // Add customers to system
        system.addCustomer(customer1);
        system.addCustomer(customer2);
        system.addCustomer(customer3);
        system.addCustomer(customer4);
        system.addCustomer(customer5);
        
        // Create interactions
        Date now = new Date();
        
        Interaction interaction1 = new Interaction("I001", "C001", "Phone Call", 
                                                  new Date(now.getTime() - 86400000 * 3),
                                                  "OUTBOUND", "PHONE", "Initial Discovery Call",
                                                  "Customer expressed interest in our enterprise solutions. Scheduled demo for next week.",
                                                  "U003", 45.0, "SUCCESS");
        
        Interaction interaction2 = new Interaction("I002", "C002", "Meeting", 
                                                  new Date(now.getTime() - 86400000 * 2),
                                                  "OUTBOUND", "MEETING", "Product Demonstration",
                                                  "Demonstrated key features. Customer showed particular interest in inventory management.",
                                                  "U002", 90.0, "SUCCESS");
        
        Interaction interaction3 = new Interaction("I003", "C001", "Email", 
                                                  new Date(now.getTime() - 86400000 * 1),
                                                  "INBOUND", "EMAIL", "Follow-up Questions",
                                                  "Customer sent email with technical questions about integration.",
                                                  "U003", 0.0, "PENDING");
        
        Interaction interaction4 = new Interaction("I004", "C003", "Phone Call", 
                                                  new Date(now.getTime() - 86400000 * 5),
                                                  "OUTBOUND", "PHONE", "Sales Call",
                                                  "Introduced company and products. Customer requested pricing information.",
                                                  "U003", 30.0, "SUCCESS");
        
        Interaction interaction5 = new Interaction("I005", "C004", "Meeting", 
                                                  new Date(now.getTime() - 86400000 * 4),
                                                  "OUTBOUND", "MEETING", "Requirements Gathering",
                                                  "Met with potential customer to understand requirements for patient management system.",
                                                  "U002", 120.0, "SUCCESS");
        
        // Add interactions to system
        system.addInteraction(interaction1);
        system.addInteraction(interaction2);
        system.addInteraction(interaction3);
        system.addInteraction(interaction4);
        system.addInteraction(interaction5);
        
        // Create opportunities
        Opportunity opportunity1 = new Opportunity("O001", "C001", "Enterprise Cloud Solution",
                                                 "Implementation of cloud-based analytics platform for enterprise client.",
                                                 new Date(now.getTime() + 86400000 * 30),
                                                 100000.00, "75%", "U003", "Website", "Analytics Suite");
        opportunity1.updateStage("Proposal");
        opportunity1.addNextStep("Prepare detailed proposal with pricing");
        opportunity1.addNextStep("Schedule technical presentation");
        
        Opportunity opportunity2 = new Opportunity("O002", "C002", "Retail Management System",
                                                 "Complete retail management solution with inventory and POS integration.",
                                                 new Date(now.getTime() + 86400000 * 60),
                                                 25000.00, "50%", "U002", "Referral", "Retail Suite");
        opportunity2.updateStage("Qualification");
        opportunity2.addNextStep("Conduct detailed needs assessment");
        opportunity2.updateCompetitor("CompetitorX");
        
        Opportunity opportunity3 = new Opportunity("O003", "C003", "Financial Analytics Platform",
                                                 "Custom financial analytics dashboard for startup.",
                                                 new Date(now.getTime() + 86400000 * 45),
                                                 35000.00, "25%", "U003", "Trade Show", "Finance Tools");
        opportunity3.updateStage("Discovery");
        opportunity3.addNextStep("Gather detailed requirements");
        opportunity3.addTag("High Growth Potential");
        
        Opportunity opportunity4 = new Opportunity("O004", "C004", "Patient Management System",
                                                 "Comprehensive patient management system for healthcare provider.",
                                                 new Date(now.getTime() + 86400000 * 90),
                                                 75000.00, "10%", "U002", "Email Campaign", "Healthcare Suite");
        opportunity4.updateStage("Prospecting");
        opportunity4.addNextStep("Schedule initial discovery call");
        
        // Add opportunities to system
        system.addOpportunity(opportunity1);
        system.addOpportunity(opportunity2);
        system.addOpportunity(opportunity3);
        system.addOpportunity(opportunity4);
        
        // Create campaigns
        Campaign campaign1 = new Campaign("CAM001", "Q3 Enterprise Solutions",
                                        "Targeted campaign for enterprise customers with cloud solutions.",
                                        "Email",
                                        new Date(now.getTime() - 86400000 * 10),
                                        new Date(now.getTime() + 86400000 * 20),
                                        "$15,000", 500000.00, "U001");
        campaign1.addTargetAudience("Enterprise customers");
        campaign1.addTargetAudience("Technology industry");
        campaign1.addIncludedCustomer("C001");
        campaign1.updateStatus("Active");
        
        Campaign campaign2 = new Campaign("CAM002", "Small Business Retail",
                                        "Campaign targeting small retail businesses.",
                                        "Social Media",
                                        new Date(now.getTime() - 86400000 * 5),
                                        new Date(now.getTime() + 86400000 * 15),
                                        "$8,000", 250000.00, "U002");
        campaign2.addTargetAudience("Small business");
        campaign2.addTargetAudience("Retail industry");
        campaign2.addIncludedCustomer("C002");
        campaign2.updateStatus("Active");
        
        Campaign campaign3 = new Campaign("CAM003", "Healthcare Newsletter",
                                        "Monthly newsletter for healthcare professionals.",
                                        "Email",
                                        new Date(now.getTime() - 86400000 * 15),
                                        new Date(now.getTime() + 86400000 * 45),
                                        "$5,000", 100000.00, "U001");
        campaign3.addTargetAudience("Healthcare industry");
        campaign3.addIncludedCustomer("C004");
        campaign3.updateStatus("Active");
        
        // Add campaigns to system
        system.addCampaign(campaign1);
        system.addCampaign(campaign2);
        system.addCampaign(campaign3);
        
        // Create tasks
        Task task1 = new Task("T001", "Follow up with Tech Corp",
                            "Follow up on initial discovery call and address technical questions.",
                            new Date(now.getTime() + 86400000 * 2),
                            new Date(now.getTime() + 86400000 * 1),
                            "High", "U003", "Customer", "C001", "U003");
        
        Task task2 = new Task("T002", "Prepare retail demo",
                            "Prepare comprehensive demo for Retail Store's requirements.",
                            new Date(now.getTime() + 86400000 * 5),
                            new Date(now.getTime() + 86400000 * 4),
                            "Medium", "U002", "Opportunity", "O002", "U002");
        
        Task task3 = new Task("T003", "Schedule follow-up with Healthcare Inc",
                            "Schedule follow-up meeting to discuss patient management system requirements.",
                            new Date(now.getTime() + 86400000 * 7),
                            new Date(now.getTime() + 86400000 * 6),
                            "Medium", "U002", "Customer", "C004", "U002");
        
        Task task4 = new Task("T004", "Send pricing to Finance Startup",
                            "Send pricing information for financial analytics platform.",
                            new Date(now.getTime() + 86400000 * 3),
                            new Date(now.getTime() + 86400000 * 2),
                            "High", "U003", "Opportunity", "O003", "U003");
        
        // Add tasks to system
        system.addTask(task1);
        system.addTask(task2);
        system.addTask(task3);
        system.addTask(task4);
        
        // Print system information
        System.out.println("Customer Relationship Management System");
        System.out.println("=======================================");
        System.out.println("System: " + system.getSystemName());
        System.out.println("Version: " + system.getVersion());
        System.out.println("Total Customers: " + system.getCustomers().size());
        System.out.println("Total Interactions: " + system.getInteractions().size());
        System.out.println("Total Opportunities: " + system.getOpportunities().size());
        System.out.println("Total Campaigns: " + system.getCampaigns().size());
        System.out.println("Total Tasks: " + system.getTasks().size());
        System.out.println();
        
        // Print customer summaries
        System.out.println("Customer Summaries");
        System.out.println("==================");
        for (Customer customer : system.getCustomers()) {
            System.out.println(customer.getSummary());
            System.out.println();
        }
        
        // Print opportunity summaries
        System.out.println("Opportunity Summaries");
        System.out.println("=====================");
        for (Opportunity opportunity : system.getOpportunities()) {
            System.out.println(opportunity.getSummary());
            System.out.println();
        }
        
        // Print campaign summaries
        System.out.println("Campaign Summaries");
        System.out.println("==================");
        for (Campaign campaign : system.getCampaigns()) {
            System.out.println(campaign.getSummary());
            System.out.println();
        }
        
        // Print task summaries
        System.out.println("Task Summaries");
        System.out.println("===============");
        for (Task task : system.getTasks()) {
            System.out.println(task.getSummary());
            System.out.println();
        }
        
        // Generate and print CRM report
        String report = system.generateCRMReport();
        System.out.println(report);
        
        // Save data to file
        system.saveToFile("crm_data_export.txt");
        
        System.out.println("\nCRM System demonstration completed successfully!");
    }
}
<｜fim▁hole｜>