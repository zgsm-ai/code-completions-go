/**
 * Library Management System
 * 
 * This class implements a comprehensive library management system
 * that can handle books, members, borrowing, returns, and catalog management.
 * 
 * Features:
 * - Book catalog management with categories and tags
 * - Member registration and management
 * - Borrowing and return tracking
 * - Fine calculation for overdue books
 * - Search and filtering capabilities
 * - Reservation and hold management
 * - Reporting and analytics
 * - Digital resource management
 */

import java.util.*;
import java.text.*;
import java.io.*;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import java.util.stream.Collectors;

/**
 * Book class represents a book in the library
 */
class Book {
    private String bookId;
    private String title;
    private String author;
    private String isbn;
    private String publisher;
    private Date publishDate;
    private String category;
    private List<String> tags;
    private int pageCount;
    private String language;
    private String format;  // HARDCOVER, PAPERBACK, EBOOK, AUDIOBOOK
    private String location;
    private int totalCopies;
    private int availableCopies;
    private double price;
    private String description;
    private String coverImageUrl;
    private List<String> subjects;
    private boolean referenceOnly;
    private String status;  // AVAILABLE, CHECKED_OUT, RESERVED, LOST, DAMAGED
    
    /**
     * Constructor for Book
     * @param bookId Book ID
     * @param title Book title
     * @param author Book author
     * @param isbn ISBN number
     * @param publisher Publisher
     * @param publishDate Publication date
     * @param category Book category
     * @param pageCount Page count
     * @param language Book language
     * @param format Book format
     * @param location Book location in library
     * @param totalCopies Total number of copies
     * @param price Book price
     * @param description Book description
     */
    public Book(String bookId, String title, String author, String isbn,
               String publisher, Date publishDate, String category, int pageCount,
               String language, String format, String location, int totalCopies,
               double price, String description) {
        this.bookId = bookId;
        this.title = title;
        this.author = author;
        this.isbn = isbn;
        this.publisher = publisher;
        this.publishDate = publishDate;
        this.category = category;
        this.tags = new ArrayList<>();
        this.pageCount = pageCount;
        this.language = language;
        this.format = format;
        this.location = location;
        this.totalCopies = totalCopies;
        this.availableCopies = totalCopies;
        this.price = price;
        this.description = description;
        this.coverImageUrl = "";
        this.subjects = new ArrayList<>();
        this.referenceOnly = false;
        this.status = "AVAILABLE";
    }
    
    /**
     * Get book ID
     * @return Book ID
     */
    public String getBookId() {
        return bookId;
    }
    
    /**
     * Get title
     * @return Book title
     */
    public String getTitle() {
        return title;
    }
    
    /**
     * Get author
     * @return Book author
     */
    public String getAuthor() {
        return author;
    }
    
    /**
     * Get ISBN
     * @return ISBN number
     */
    public String getIsbn() {
        return isbn;
    }
    
    /**
     * Get publisher
     * @return Publisher
     */
    public String getPublisher() {
        return publisher;
    }
    
    /**
     * Get publish date
     * @return Publication date
     */
    public Date getPublishDate() {
        return publishDate;
    }
    
    /**
     * Get category
     * @return Book category
     */
    public String getCategory() {
        return category;
    }
    
    /**
     * Get tags
     * @return List of tags
     */
    public List<String> getTags() {
        return tags;
    }
    
    /**
     * Get page count
     * @return Page count
     */
    public int getPageCount() {
        return pageCount;
    }
    
    /**
     * Get language
     * @return Book language
     */
    public String getLanguage() {
        return language;
    }
    
    /**
     * Get format
     * @return Book format
     */
    public String getFormat() {
        return format;
    }
    
    /**
     * Get location
     * @return Book location
     */
    public String getLocation() {
        return location;
    }
    
    /**
     * Get total copies
     * @return Total number of copies
     */
    public int getTotalCopies() {
        return totalCopies;
    }
    
    /**
     * Get available copies
     * @return Number of available copies
     */
    public int getAvailableCopies() {
        return availableCopies;
    }
    
    /**
     * Get price
     * @return Book price
     */
    public double getPrice() {
        return price;
    }
    
    /**
     * Get description
     * @return Book description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get cover image URL
     * @return Cover image URL
     */
    public String getCoverImageUrl() {
        return coverImageUrl;
    }
    
    /**
     * Get subjects
     * @return List of subjects
     */
    public List<String> getSubjects() {
        return subjects;
    }
    
    /**
     * Check if reference only
     * @return True if reference only, false otherwise
     */
    public boolean isReferenceOnly() {
        return referenceOnly;
    }
    
    /**
     * Get status
     * @return Book status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Set title
     * @param title New title
     */
    public void setTitle(String title) {
        this.title = title;
    }
    
    /**
     * Set author
     * @param author New author
     */
    public void setAuthor(String author) {
        this.author = author;
    }
    
    /**
     * Set category
     * @param category New category
     */
    public void setCategory(String category) {
        this.category = category;
    }
    
    /**
     * Set total copies
     * @param totalCopies New total copies
     */
    public void setTotalCopies(int totalCopies) {
        this.totalCopies = totalCopies;
        this.availableCopies = totalCopies;
    }
    
    /**
     * Set price
     * @param price New price
     */
    public void setPrice(double price) {
        this.price = price;
    }
    
    /**
     * Set description
     * @param description New description
     */
    public void setDescription(String description) {
        this.description = description;
    }
    
    /**
     * Set cover image URL
     * @param coverImageUrl New cover image URL
     */
    public void setCoverImageUrl(String coverImageUrl) {
        this.coverImageUrl = coverImageUrl;
    }
    
    /**
     * Set reference only
     * @param referenceOnly New reference only status
     */
    public void setReferenceOnly(boolean referenceOnly) {
        this.referenceOnly = referenceOnly;
    }
    
    /**
     * Set status
     * @param status New status
     */
    public void setStatus(String status) {
        this.status = status;
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
     * Add subject
     * @param subject Subject to add
     */
    public void addSubject(String subject) {
        if (!subjects.contains(subject)) {
            subjects.add(subject);
        }
    }
    
    /**
     * Remove subject
     * @param subject Subject to remove
     */
    public void removeSubject(String subject) {
        subjects.remove(subject);
    }
    
    /**
     * Decrement available copies
     * @return True if successful, false if no copies available
     */
    public boolean decrementAvailableCopies() {
        if (availableCopies > 0) {
            availableCopies--;
            if (availableCopies == 0) {
                status = "CHECKED_OUT";
            }
            return true;
        }
        return false;
    }
    
    /**
     * Increment available copies
     * @return True if successful, false if already at total copies
     */
    public boolean incrementAvailableCopies() {
        if (availableCopies < totalCopies) {
            availableCopies++;
            if (availableCopies > 0 && status.equals("CHECKED_OUT")) {
                status = "AVAILABLE";
            }
            return true;
        }
        return false;
    }
    
    /**
     * Get book summary
     * @return Book summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Book ID: ").append(bookId).append("\n");
        sb.append("Title: ").append(title).append("\n");
        sb.append("Author: ").append(author).append("\n");
        sb.append("ISBN: ").append(isbn).append("\n");
        sb.append("Publisher: ").append(publisher).append("\n");
        sb.append("Publication Date: ").append(sdf.format(publishDate)).append("\n");
        sb.append("Category: ").append(category).append("\n");
        sb.append("Language: ").append(language).append("\n");
        sb.append("Format: ").append(format).append("\n");
        sb.append("Page Count: ").append(pageCount).append("\n");
        sb.append("Location: ").append(location).append("\n");
        sb.append("Total Copies: ").append(totalCopies).append("\n");
        sb.append("Available Copies: ").append(availableCopies).append("\n");
        sb.append("Price: $").append(String.format("%.2f", price)).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Reference Only: ").append(referenceOnly ? "Yes" : "No").append("\n");
        
        if (!tags.isEmpty()) {
            sb.append("Tags: ").append(String.join(", ", tags)).append("\n");
        }
        
        if (!subjects.isEmpty()) {
            sb.append("Subjects: ").append(String.join(", ", subjects)).append("\n");
        }
        
        if (!description.trim().isEmpty()) {
            sb.append("Description: ").append(description).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Member class represents a library member
 */
class Member {
    private String memberId;
    private String firstName;
    private String lastName;
    private String email;
    private String phone;
    private String address;
    private String city;
    private String state;
    private String postalCode;
    private Date registrationDate;
    private Date expirationDate;
    private String membershipType;
    private String status;
    private double outstandingFines;
    private int currentBorrowed;
    private int totalBorrowed;
    private int currentReservations;
    private List<String> borrowedBooks;
    private List<String> reservedBooks;
    private Map<String, String> preferences;
    private List<String> favoriteCategories;
    
    /**
     * Constructor for Member
     * @param memberId Member ID
     * @param firstName First name
     * @param lastName Last name
     * @param email Email address
     * @param phone Phone number
     * @param address Street address
     * @param city City
     * @param state State
     * @param postalCode Postal code
     * @param membershipType Membership type
     */
    public Member(String memberId, String firstName, String lastName, String email,
                 String phone, String address, String city, String state,
                 String postalCode, String membershipType) {
        this.memberId = memberId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phone = phone;
        this.address = address;
        this.city = city;
        this.state = state;
        this.postalCode = postalCode;
        this.registrationDate = new Date();
        
        // Set expiration date based on membership type
        Calendar cal = Calendar.getInstance();
        if (membershipType.equals("STUDENT")) {
            cal.add(Calendar.YEAR, 1);  // 1 year for student
        } else if (membershipType.equals("ADULT")) {
            cal.add(Calendar.YEAR, 2);  // 2 years for adult
        } else if (membershipType.equals("SENIOR")) {
            cal.add(Calendar.YEAR, 3);  // 3 years for senior
        } else {
            cal.add(Calendar.YEAR, 1);  // Default 1 year
        }
        
        this.expirationDate = cal.getTime();
        this.membershipType = membershipType;
        this.status = "ACTIVE";
        this.outstandingFines = 0.0;
        this.currentBorrowed = 0;
        this.totalBorrowed = 0;
        this.currentReservations = 0;
        this.borrowedBooks = new ArrayList<>();
        this.reservedBooks = new ArrayList<>();
        this.preferences = new HashMap<>();
        this.favoriteCategories = new ArrayList<>();
        
        // Set default preferences
        preferences.put("notificationEmail", "true");
        preferences.put("notificationSMS", "false");
        preferences.put("autoRenew", "false");
        preferences.put("preferredLanguage", "ENGLISH");
    }
    
    /**
     * Get member ID
     * @return Member ID
     */
    public String getMemberId() {
        return memberId;
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
     * Get expiration date
     * @return Expiration date
     */
    public Date getExpirationDate() {
        return expirationDate;
    }
    
    /**
     * Get membership type
     * @return Membership type
     */
    public String getMembershipType() {
        return membershipType;
    }
    
    /**
     * Get status
     * @return Member status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get outstanding fines
     * @return Outstanding fines
     */
    public double getOutstandingFines() {
        return outstandingFines;
    }
    
    /**
     * Get current borrowed
     * @return Number of currently borrowed books
     */
    public int getCurrentBorrowed() {
        return currentBorrowed;
    }
    
    /**
     * Get total borrowed
     * @return Total number of books ever borrowed
     */
    public int getTotalBorrowed() {
        return totalBorrowed;
    }
    
    /**
     * Get current reservations
     * @return Number of current reservations
     */
    public int getCurrentReservations() {
        return currentReservations;
    }
    
    /**
     * Get borrowed books
     * @return List of borrowed book IDs
     */
    public List<String> getBorrowedBooks() {
        return borrowedBooks;
    }
    
    /**
     * Get reserved books
     * @return List of reserved book IDs
     */
    public List<String> getReservedBooks() {
        return reservedBooks;
    }
    
    /**
     * Get preferences
     * @return Map of member preferences
     */
    public Map<String, String> getPreferences() {
        return preferences;
    }
    
    /**
     * Get favorite categories
     * @return List of favorite categories
     */
    public List<String> getFavoriteCategories() {
        return favoriteCategories;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update outstanding fines
     * @param amount Amount to add
     */
    public void updateOutstandingFines(double amount) {
        this.outstandingFines += amount;
        if (this.outstandingFines < 0) {
            this.outstandingFines = 0.0;
        }
    }
    
    /**
     * Renew membership
     * @param years Number of years to extend
     */
    public void renewMembership(int years) {
        Calendar cal = Calendar.getInstance();
        cal.setTime(expirationDate);
        
        // If already expired, extend from today
        if (expirationDate.before(new Date())) {
            cal = Calendar.getInstance();
        }
        
        cal.add(Calendar.YEAR, years);
        this.expirationDate = cal.getTime();
        
        if (status.equals("EXPIRED")) {
            status = "ACTIVE";
        }
    }
    
    /**
     * Add borrowed book
     * @param bookId Book ID to add
     * @return True if added successfully, false otherwise
     */
    public boolean addBorrowedBook(String bookId) {
        // Check borrowing limits based on membership type
        int maxBorrowed = 5;  // Default limit
        if (membershipType.equals("STUDENT")) {
            maxBorrowed = 5;
        } else if (membershipType.equals("ADULT")) {
            maxBorrowed = 10;
        } else if (membershipType.equals("SENIOR")) {
            maxBorrowed = 10;
        }
        
        if (currentBorrowed >= maxBorrowed) {
            return false;
        }
        
        if (!borrowedBooks.contains(bookId)) {
            borrowedBooks.add(bookId);
            currentBorrowed++;
            totalBorrowed++;
            return true;
        }
        
        return false;
    }
    
    /**
     * Remove borrowed book
     * @param bookId Book ID to remove
     * @return True if removed successfully, false otherwise
     */
    public boolean removeBorrowedBook(String bookId) {
        if (borrowedBooks.remove(bookId)) {
            currentBorrowed--;
            return true;
        }
        
        return false;
    }
    
    /**
     * Add reserved book
     * @param bookId Book ID to add
     * @return True if added successfully, false otherwise
     */
    public boolean addReservedBook(String bookId) {
        // Check reservation limits based on membership type
        int maxReservations = 3;  // Default limit
        if (membershipType.equals("STUDENT")) {
            maxReservations = 3;
        } else if (membershipType.equals("ADULT")) {
            maxReservations = 5;
        } else if (membershipType.equals("SENIOR")) {
            maxReservations = 5;
        }
        
        if (currentReservations >= maxReservations) {
            return false;
        }
        
        if (!reservedBooks.contains(bookId)) {
            reservedBooks.add(bookId);
            currentReservations++;
            return true;
        }
        
        return false;
    }
    
    /**
     * Remove reserved book
     * @param bookId Book ID to remove
     * @return True if removed successfully, false otherwise
     */
    public boolean removeReservedBook(String bookId) {
        if (reservedBooks.remove(bookId)) {
            currentReservations--;
            return true;
        }
        
        return false;
    }
    
    /**
     * Update preference
     * @param key Preference key
     * @param value Preference value
     */
    public void updatePreference(String key, String value) {
        preferences.put(key, value);
    }
    
    /**
     * Add favorite category
     * @param category Favorite category to add
     */
    public void addFavoriteCategory(String category) {
        if (!favoriteCategories.contains(category)) {
            favoriteCategories.add(category);
        }
    }
    
    /**
     * Remove favorite category
     * @param category Favorite category to remove
     */
    public void removeFavoriteCategory(String category) {
        favoriteCategories.remove(category);
    }
    
    /**
     * Get member summary
     * @return Member summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Member ID: ").append(memberId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Address: ").append(address).append(", ").append(city).append(", ")
          .append(state).append(", ").append(postalCode).append("\n");
        sb.append("Registration Date: ").append(sdf.format(registrationDate)).append("\n");
        sb.append("Expiration Date: ").append(sdf.format(expirationDate)).append("\n");
        sb.append("Membership Type: ").append(membershipType).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Outstanding Fines: $").append(String.format("%.2f", outstandingFines)).append("\n");
        sb.append("Current Borrowed: ").append(currentBorrowed).append("\n");
        sb.append("Total Borrowed: ").append(totalBorrowed).append("\n");
        sb.append("Current Reservations: ").append(currentReservations).append("\n");
        
        if (!borrowedBooks.isEmpty()) {
            sb.append("Borrowed Books: ").append(String.join(", ", borrowedBooks)).append("\n");
        }
        
        if (!reservedBooks.isEmpty()) {
            sb.append("Reserved Books: ").append(String.join(", ", reservedBooks)).append("\n");
        }
        
        if (!favoriteCategories.isEmpty()) {
            sb.append("Favorite Categories: ").append(String.join(", ", favoriteCategories)).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * BorrowingRecord class represents a borrowing record
 */
class BorrowingRecord {
    private String recordId;
    private String bookId;
    private String memberId;
    private Date borrowDate;
    private Date dueDate;
    private Date returnDate;
    private String status;
    private double fine;
    private int renewalCount;
    private String notes;
    
    /**
     * Constructor for BorrowingRecord
     * @param recordId Record ID
     * @param bookId Book ID
     * @param memberId Member ID
     * @param borrowDate Borrow date
     * @param dueDate Due date
     * @param status Record status
     */
    public BorrowingRecord(String recordId, String bookId, String memberId,
                         Date borrowDate, Date dueDate, String status) {
        this.recordId = recordId;
        this.bookId = bookId;
        this.memberId = memberId;
        this.borrowDate = borrowDate;
        this.dueDate = dueDate;
        this.returnDate = null;
        this.status = status;
        this.fine = 0.0;
        this.renewalCount = 0;
        this.notes = "";
    }
    
    /**
     * Get record ID
     * @return Record ID
     */
    public String getRecordId() {
        return recordId;
    }
    
    /**
     * Get book ID
     * @return Book ID
     */
    public String getBookId() {
        return bookId;
    }
    
    /**
     * Get member ID
     * @return Member ID
     */
    public String getMemberId() {
        return memberId;
    }
    
    /**
     * Get borrow date
     * @return Borrow date
     */
    public Date getBorrowDate() {
        return borrowDate;
    }
    
    /**
     * Get due date
     * @return Due date
     */
    public Date getDueDate() {
        return dueDate;
    }
    
    /**
     * Get return date
     * @return Return date
     */
    public Date getReturnDate() {
        return returnDate;
    }
    
    /**
     * Get status
     * @return Record status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get fine
     * @return Fine amount
     */
    public double getFine() {
        return fine;
    }
    
    /**
     * Get renewal count
     * @return Number of renewals
     */
    public int getRenewalCount() {
        return renewalCount;
    }
    
    /**
     * Get notes
     * @return Notes
     */
    public String getNotes() {
        return notes;
    }
    
    /**
     * Set return date
     * @param returnDate New return date
     */
    public void setReturnDate(Date returnDate) {
        this.returnDate = returnDate;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update fine
     * @param amount Fine amount
     */
    public void updateFine(double amount) {
        this.fine = amount;
    }
    
    /**
     * Add renewal
     * @param days Days to extend
     * @return True if renewed successfully, false otherwise
     */
    public boolean addRenewal(int days) {
        if (renewalCount >= 3) {
            return false;  // Maximum 3 renewals allowed
        }
        
        Calendar cal = Calendar.getInstance();
        cal.setTime(dueDate);
        cal.add(Calendar.DAY_OF_MONTH, days);
        dueDate = cal.getTime();
        
        renewalCount++;
        return true;
    }
    
    /**
     * Add note
     * @param note Note to add
     */
    public void addNote(String note) {
        if (notes.isEmpty()) {
            notes = note;
        } else {
            notes += "; " + note;
        }
    }
    
    /**
     * Calculate days overdue
     * @return Number of days overdue
     */
    public int calculateDaysOverdue() {
        if (status.equals("RETURNED") || returnDate != null) {
            return 0;
        }
        
        Date today = new Date();
        
        if (today.after(dueDate)) {
            long diff = today.getTime() - dueDate.getTime();
            return (int) (diff / (24 * 60 * 60 * 1000));
        }
        
        return 0;
    }
    
    /**
     * Get borrowing record summary
     * @return Record summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Record ID: ").append(recordId).append("\n");
        sb.append("Book ID: ").append(bookId).append("\n");
        sb.append("Member ID: ").append(memberId).append("\n");
        sb.append("Borrow Date: ").append(sdf.format(borrowDate)).append("\n");
        sb.append("Due Date: ").append(sdf.format(dueDate)).append("\n");
        
        if (returnDate != null) {
            sb.append("Return Date: ").append(sdf.format(returnDate)).append("\n");
        }
        
        sb.append("Status: ").append(status).append("\n");
        sb.append("Fine: $").append(String.format("%.2f", fine)).append("\n");
        sb.append("Renewal Count: ").append(renewalCount).append("\n");
        
        int daysOverdue = calculateDaysOverdue();
        if (daysOverdue > 0) {
            sb.append("Days Overdue: ").append(daysOverdue).append("\n");
        }
        
        if (!notes.trim().isEmpty()) {
            sb.append("Notes: ").append(notes).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * LibraryManagementSystem class manages the entire library system
 */
public class LibraryManagementSystem {
    private String libraryName;
    private String libraryCode;
    private String address;
    private String phone;
    private String email;
    private List<Book> books;
    private Map<String, Book> bookMap;
    private List<Member> members;
    private Map<String, Member> memberMap;
    private List<BorrowingRecord> borrowingRecords;
    private Map<String, BorrowingRecord> recordMap;
    private List<String> categories;
    private Map<String, String> borrowingPolicies;
    private Map<String, Integer> memberLimits;
    private Map<String, Integer> fineRates;
    
    /**
     * Constructor for LibraryManagementSystem
     * @param libraryName Library name
     * @param libraryCode Library code
     * @param address Library address
     * @param phone Library phone
     * @param email Library email
     */
    public LibraryManagementSystem(String libraryName, String libraryCode,
                                String address, String phone, String email) {
        this.libraryName = libraryName;
        this.libraryCode = libraryCode;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.books = new ArrayList<>();
        this.bookMap = new HashMap<>();
        this.members = new ArrayList<>();
        this.memberMap = new HashMap<>();
        this.borrowingRecords = new ArrayList<>();
        this.recordMap = new HashMap<>();
        this.categories = new ArrayList<>();
        this.borrowingPolicies = new HashMap<>();
        this.memberLimits = new HashMap<>();
        this.fineRates = new HashMap<>();
        
        // Initialize categories
        categories.add("FICTION");
        categories.add("NON_FICTION");
        categories.add("SCIENCE");
        categories.add("TECHNOLOGY");
        categories.add("HISTORY");
        categories.add("BIOGRAPHY");
        categories.add("CHILDREN");
        categories.add("REFERENCE");
        categories.add("ACADEMIC");
        categories.add("TEXTBOOK");
        categories.add("PERIODICAL");
        categories.add("AUDIOVISUAL");
        
        // Initialize borrowing policies
        borrowingPolicies.put("STANDARD_BORROW_PERIOD", "14");  // 14 days
        borrowingPolicies.put("MAX_RENEWALS", "3");            // Maximum 3 renewals
        borrowingPolicies.put("RENEWAL_PERIOD", "7");          // 7 days per renewal
        
        // Initialize member limits
        memberLimits.put("STUDENT", 5);      // Max 5 books for students
        memberLimits.put("ADULT", 10);       // Max 10 books for adults
        memberLimits.put("SENIOR", 10);      // Max 10 books for seniors
        
        // Initialize fine rates
        fineRates.put("OVERDUE_FINE", 50);    // $0.50 per day
        fineRates.put("LOST_BOOK", 20);      // $20.00 per lost book
    }
    
    /**
     * Get library name
     * @return Library name
     */
    public String getLibraryName() {
        return libraryName;
    }
    
    /**
     * Get library code
     * @return Library code
     */
    public String getLibraryCode() {
        return libraryCode;
    }
    
    /**
     * Get address
     * @return Library address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get phone
     * @return Library phone
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get email
     * @return Library email
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get books
     * @return List of books
     */
    public List<Book> getBooks() {
        return books;
    }
    
    /**
     * Get members
     * @return List of members
     */
    public List<Member> getMembers() {
        return members;
    }
    
    /**
     * Get borrowing records
     * @return List of borrowing records
     */
    public List<BorrowingRecord> getBorrowingRecords() {
        return borrowingRecords;
    }
    
    /**
     * Get categories
     * @return List of book categories
     */
    public List<String> getCategories() {
        return categories;
    }
    
    /**
     * Add a book
     * @param book Book to add
     * @return True if added successfully, false otherwise
     */
    public boolean addBook(Book book) {
        if (bookMap.containsKey(book.getBookId())) {
            return false;
        }
        
        books.add(book);
        bookMap.put(book.getBookId(), book);
        return true;
    }
    
    /**
     * Add a member
     * @param member Member to add
     * @return True if added successfully, false otherwise
     */
    public boolean addMember(Member member) {
        if (memberMap.containsKey(member.getMemberId())) {
            return false;
        }
        
        members.add(member);
        memberMap.put(member.getMemberId(), member);
        return true;
    }
    
    /**
     * Add a borrowing record
     * @param record Borrowing record to add
     * @return True if added successfully, false otherwise
     */
    public boolean addBorrowingRecord(BorrowingRecord record) {
        if (recordMap.containsKey(record.getRecordId())) {
            return false;
        }
        
        borrowingRecords.add(record);
        recordMap.put(record.getRecordId(), record);
        return true;
    }
    
    /**
     * Get book by ID
     * @param bookId Book ID
     * @return Book if found, null otherwise
     */
    public Book getBookById(String bookId) {
        return bookMap.get(bookId);
    }
    
    /**
     * Get member by ID
     * @param memberId Member ID
     * @return Member if found, null otherwise
     */
    public Member getMemberById(String memberId) {
        return memberMap.get(memberId);
    }
    
    /**
     * Get borrowing record by ID
     * @param recordId Record ID
     * @return Borrowing record if found, null otherwise
     */
    public BorrowingRecord getBorrowingRecordById(String recordId) {
        return recordMap.get(recordId);
    }
    
    /**
     * Search books by keyword
     * @param keyword Search keyword
     * @return List of matching books
     */
    public List<Book> searchBooks(String keyword) {
        List<Book> result = new ArrayList<>();
        String lowercaseKeyword = keyword.toLowerCase();
        
        for (Book book : books) {
            boolean matches = false;
            
            // Check title
            if (book.getTitle().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check author
            if (book.getAuthor().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check ISBN
            if (book.getIsbn().contains(keyword)) {
                matches = true;
            }
            
            // Check publisher
            if (book.getPublisher().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check description
            if (book.getDescription().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check tags
            for (String tag : book.getTags()) {
                if (tag.toLowerCase().contains(lowercaseKeyword)) {
                    matches = true;
                    break;
                }
            }
            
            // Check subjects
            for (String subject : book.getSubjects()) {
                if (subject.toLowerCase().contains(lowercaseKeyword)) {
                    matches = true;
                    break;
                }
            }
            
            if (matches) {
                result.add(book);
            }
        }
        
        return result;
    }
    
    /**
     * Get books by category
     * @param category Book category
     * @return List of books in category
     */
    public List<Book> getBooksByCategory(String category) {
        List<Book> result = new ArrayList<>();
        
        for (Book book : books) {
            if (book.getCategory().equals(category)) {
                result.add(book);
            }
        }
        
        return result;
    }
    
    /**
     * Get available books
     * @return List of available books
     */
    public List<Book> getAvailableBooks() {
        List<Book> result = new ArrayList<>();
        
        for (Book book : books) {
            if (book.getAvailableCopies() > 0 && !book.isReferenceOnly()) {
                result.add(book);
            }
        }
        
        return result;
    }
    
    /**
     * Get members by membership type
     * @param membershipType Membership type
     * @return List of members with specified type
     */
    public List<Member> getMembersByMembershipType(String membershipType) {
        List<Member> result = new ArrayList<>();
        
        for (Member member : members) {
            if (member.getMembershipType().equals(membershipType)) {
                result.add(member);
            }
        }
        
        return result;
    }
    
    /**
     * Get active members
     * @return List of active members
     */
    public List<Member> getActiveMembers() {
        List<Member> result = new ArrayList<>();
        
        for (Member member : members) {
            if (member.getStatus().equals("ACTIVE")) {
                result.add(member);
            }
        }
        
        return result;
    }
    
    /**
     * Get overdue records
     * @return List of overdue borrowing records
     */
    public List<BorrowingRecord> getOverdueRecords() {
        List<BorrowingRecord> result = new ArrayList<>();
        
        for (BorrowingRecord record : borrowingRecords) {
            if (record.calculateDaysOverdue() > 0) {
                result.add(record);
            }
        }
        
        return result;
    }
    
    /**
     * Get active borrowing records for a member
     * @param memberId Member ID
     * @return List of active borrowing records
     */
    public List<BorrowingRecord> getActiveRecordsByMember(String memberId) {
        List<BorrowingRecord> result = new ArrayList<>();
        
        for (BorrowingRecord record : borrowingRecords) {
            if (record.getMemberId().equals(memberId) && 
                (record.getStatus().equals("ACTIVE") || record.getStatus().equals("OVERDUE"))) {
                result.add(record);
            }
        }
        
        return result;
    }
    
    /**
     * Borrow a book
     * @param bookId Book ID
     * @param memberId Member ID
     * @return Record ID if successful, null otherwise
     */
    public String borrowBook(String bookId, String memberId) {
        Book book = getBookById(bookId);
        Member member = getMemberById(memberId);
        
        if (book == null || member == null) {
            return null;
        }
        
        // Check if book is available
        if (book.getAvailableCopies() <= 0 || book.isReferenceOnly()) {
            return null;
        }
        
        // Check member status
        if (!member.getStatus().equals("ACTIVE")) {
            return null;
        }
        
        // Check member borrowing limit
        int maxBorrowed = memberLimits.get(member.getMembershipType());
        if (member.getCurrentBorrowed() >= maxBorrowed) {
            return null;
        }
        
        // Check if member has outstanding fines
        if (member.getOutstandingFines() > 5.0) {  // $5.00 fine limit
            return null;
        }
        
        // Create borrowing record
        String recordId = "BR" + (borrowingRecords.size() + 1);
        Date borrowDate = new Date();
        
        // Calculate due date based on membership type
        Calendar cal = Calendar.getInstance();
        int borrowPeriod = Integer.parseInt(borrowingPolicies.get("STANDARD_BORROW_PERIOD"));
        
        if (member.getMembershipType().equals("STUDENT")) {
            borrowPeriod = 14;  // 14 days for students
        } else if (member.getMembershipType().equals("ADULT")) {
            borrowPeriod = 21;  // 21 days for adults
        } else if (member.getMembershipType().equals("SENIOR")) {
            borrowPeriod = 28;  // 28 days for seniors
        }
        
        cal.add(Calendar.DAY_OF_MONTH, borrowPeriod);
        Date dueDate = cal.getTime();
        
        BorrowingRecord record = new BorrowingRecord(recordId, bookId, memberId,
                                                  borrowDate, dueDate, "ACTIVE");
        
        // Update book and member
        book.decrementAvailableCopies();
        member.addBorrowedBook(bookId);
        
        // Add record to system
        addBorrowingRecord(record);
        
        return recordId;
    }
    
    /**
     * Return a book
     * @param recordId Record ID
     * @return True if returned successfully, false otherwise
     */
    public boolean returnBook(String recordId) {
        BorrowingRecord record = getBorrowingRecordById(recordId);
        
        if (record == null || record.getStatus().equals("RETURNED")) {
            return false;
        }
        
        Book book = getBookById(record.getBookId());
        Member member = getMemberById(record.getMemberId());
        
        if (book == null || member == null) {
            return false;
        }
        
        // Set return date
        record.setReturnDate(new Date());
        
        // Calculate fine if overdue
        int daysOverdue = record.calculateDaysOverdue();
        if (daysOverdue > 0) {
            double fineRate = fineRates.get("OVERDUE_FINE") / 100.0;  // Convert cents to dollars
            double fineAmount = daysOverdue * fineRate;
            record.updateFine(fineAmount);
            member.updateOutstandingFines(fineAmount);
        }
        
        // Update record status
        record.updateStatus("RETURNED");
        
        // Update book and member
        book.incrementAvailableCopies();
        member.removeBorrowedBook(record.getBookId());
        
        return true;
    }
    
    /**
     * Renew a borrowing record
     * @param recordId Record ID
     * @return True if renewed successfully, false otherwise
     */
    public boolean renewBook(String recordId) {
        BorrowingRecord record = getBorrowingRecordById(recordId);
        
        if (record == null || record.getStatus().equals("RETURNED")) {
            return false;
        }
        
        // Check if there's a reservation on the book
        Book book = getBookById(record.getBookId());
        if (book.getAvailableCopies() < 1) {
            return false;  // Book is reserved by someone else
        }
        
        // Renew the record
        int renewalPeriod = Integer.parseInt(borrowingPolicies.get("RENEWAL_PERIOD"));
        return record.addRenewal(renewalPeriod);
    }
    
    /**
     * Reserve a book
     * @param bookId Book ID
     * @param memberId Member ID
     * @return True if reserved successfully, false otherwise
     */
    public boolean reserveBook(String bookId, String memberId) {
        Book book = getBookById(bookId);
        Member member = getMemberById(memberId);
        
        if (book == null || member == null) {
            return false;
        }
        
        // Check if book is available
        if (book.getAvailableCopies() > 0) {
            return false;  // Book is available, no need to reserve
        }
        
        // Check if member already has this book reserved
        if (member.getReservedBooks().contains(bookId)) {
            return false;
        }
        
        // Add reservation
        member.addReservedBook(bookId);
        
        return true;
    }
    
    /**
     * Cancel a reservation
     * @param bookId Book ID
     * @param memberId Member ID
     * @return True if cancelled successfully, false otherwise
     */
    public boolean cancelReservation(String bookId, String memberId) {
        Member member = getMemberById(memberId);
        
        if (member == null) {
            return false;
        }
        
        return member.removeReservedBook(bookId);
    }
    
    /**
     * Pay fine
     * @param memberId Member ID
     * @param amount Amount to pay
     * @return True if paid successfully, false otherwise
     */
    public boolean payFine(String memberId, double amount) {
        Member member = getMemberById(memberId);
        
        if (member == null) {
            return false;
        }
        
        if (amount <= 0 || amount > member.getOutstandingFines()) {
            return false;
        }
        
        // Update outstanding fines
        member.updateOutstandingFines(-amount);
        
        return true;
    }
    
    /**
     * Generate library report
     * @return Library report string
     */
    public String generateLibraryReport() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("=====================================\n");
        sb.append("         LIBRARY REPORT\n");
        sb.append("=====================================\n");
        sb.append("Library: ").append(libraryName).append("\n");
        sb.append("Code: ").append(libraryCode).append("\n");
        sb.append("Address: ").append(address).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Report Date: ").append(sdf.format(new Date())).append("\n\n");
        
        // Book statistics
        sb.append("BOOK STATISTICS\n");
        sb.append("---------------\n");
        sb.append("Total Books: ").append(books.size()).append("\n");
        
        Map<String, Integer> booksByCategory = new HashMap<>();
        Map<String, Integer> booksByFormat = new HashMap<>();
        int totalCopies = 0;
        int availableCopies = 0;
        
        for (Book book : books) {
            String category = book.getCategory();
            booksByCategory.put(category, booksByCategory.getOrDefault(category, 0) + 1);
            
            String format = book.getFormat();
            booksByFormat.put(format, booksByFormat.getOrDefault(format, 0) + 1);
            
            totalCopies += book.getTotalCopies();
            availableCopies += book.getAvailableCopies();
        }
        
        sb.append("Total Copies: ").append(totalCopies).append("\n");
        sb.append("Available Copies: ").append(availableCopies).append("\n");
        sb.append("Checked Out Copies: ").append(totalCopies - availableCopies).append("\n");
        
        sb.append("\nBooks by Category:\n");
        for (Map.Entry<String, Integer> entry : booksByCategory.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        sb.append("\nBooks by Format:\n");
        for (Map.Entry<String, Integer> entry : booksByFormat.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Member statistics
        sb.append("\nMEMBER STATISTICS\n");
        sb.append("-----------------\n");
        sb.append("Total Members: ").append(members.size()).append("\n");
        
        Map<String, Integer> membersByType = new HashMap<>();
        Map<String, Integer> membersByStatus = new HashMap<>();
        double totalOutstandingFines = 0.0;
        int totalBorrowedBooks = 0;
        
        for (Member member : members) {
            String type = member.getMembershipType();
            membersByType.put(type, membersByType.getOrDefault(type, 0) + 1);
            
            String status = member.getStatus();
            membersByStatus.put(status, membersByStatus.getOrDefault(status, 0) + 1);
            
            totalOutstandingFines += member.getOutstandingFines();
            totalBorrowedBooks += member.getCurrentBorrowed();
        }
        
        sb.append("Total Outstanding Fines: $").append(String.format("%.2f", totalOutstandingFines)).append("\n");
        sb.append("Total Borrowed Books: ").append(totalBorrowedBooks).append("\n");
        
        sb.append("\nMembers by Type:\n");
        for (Map.Entry<String, Integer> entry : membersByType.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        sb.append("\nMembers by Status:\n");
        for (Map.Entry<String, Integer> entry : membersByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Borrowing statistics
        sb.append("\nBORROWING STATISTICS\n");
        sb.append("---------------------\n");
        sb.append("Total Borrowing Records: ").append(borrowingRecords.size()).append("\n");
        
        Map<String, Integer> recordsByStatus = new HashMap<>();
        double totalFines = 0.0;
        int overdueCount = 0;
        
        for (BorrowingRecord record : borrowingRecords) {
            String status = record.getStatus();
            recordsByStatus.put(status, recordsByStatus.getOrDefault(status, 0) + 1);
            
            totalFines += record.getFine();
            
            if (record.calculateDaysOverdue() > 0) {
                overdueCount++;
            }
        }
        
        sb.append("Total Fines: $").append(String.format("%.2f", totalFines)).append("\n");
        sb.append("Overdue Books: ").append(overdueCount).append("\n");
        
        sb.append("\nRecords by Status:\n");
        for (Map.Entry<String, Integer> entry : recordsByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Most popular books
        Map<String, Integer> bookPopularity = new HashMap<>();
        for (BorrowingRecord record : borrowingRecords) {
            String bookId = record.getBookId();
            bookPopularity.put(bookId, bookPopularity.getOrDefault(bookId, 0) + 1);
        }
        
        // Sort books by popularity
        List<Map.Entry<String, Integer>> sortedBooks = bookPopularity.entrySet().stream()
                .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                .collect(Collectors.toList());
        
        sb.append("\nTop 5 Most Popular Books:\n");
        for (int i = 0; i < Math.min(5, sortedBooks.size()); i++) {
            Map.Entry<String, Integer> entry = sortedBooks.get(i);
            String bookId = entry.getKey();
            int count = entry.getValue();
            Book book = getBookById(bookId);
            
            sb.append("  ").append(i + 1).append(". ").append(book != null ? book.getTitle() : "Unknown")
              .append(" (").append(count).append(" times)\n");
        }
        
        // Most active members
        Map<String, Integer> memberActivity = new HashMap<>();
        for (BorrowingRecord record : borrowingRecords) {
            String memberId = record.getMemberId();
            memberActivity.put(memberId, memberActivity.getOrDefault(memberId, 0) + 1);
        }
        
        // Sort members by activity
        List<Map.Entry<String, Integer>> sortedMembers = memberActivity.entrySet().stream()
                .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                .collect(Collectors.toList());
        
        sb.append("\nTop 5 Most Active Members:\n");
        for (int i = 0; i < Math.min(5, sortedMembers.size()); i++) {
            Map.Entry<String, Integer> entry = sortedMembers.get(i);
            String memberId = entry.getKey();
            int count = entry.getValue();
            Member member = getMemberById(memberId);
            
            sb.append("  ").append(i + 1).append(". ").append(member != null ? member.getFullName() : "Unknown")
              .append(" (").append(count).append(" books)\n");
        }
        
        return sb.toString();
    }
    
    /**
     * Save library data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("LIBRARY DATA EXPORT");
            writer.println("==================");
            writer.println("Library: " + libraryName);
            writer.println("Code: " + libraryCode);
            writer.println("Address: " + address);
            writer.println("Phone: " + phone);
            writer.println("Email: " + email);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export books
            writer.println("BOOKS");
            writer.println("-----");
            for (Book book : books) {
                writer.println(book.getSummary());
                writer.println();
            }
            
            // Export members
            writer.println("MEMBERS");
            writer.println("-------");
            for (Member member : members) {
                writer.println(member.getSummary());
                writer.println();
            }
            
            // Export borrowing records
            writer.println("BORROWING RECORDS");
            writer.println("-----------------");
            for (BorrowingRecord record : borrowingRecords) {
                writer.println(record.getSummary());
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
        // Create a new library management system
        LibraryManagementSystem library = new LibraryManagementSystem(
            "City Central Library", "CCL001",
            "123 Main St, City, State, Country", "(555) 123-4567", "info@citylibrary.com"
        );
        
        // Create books
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        Book book1 = new Book("B001", "The Great Gatsby", "F. Scott Fitzgerald", "978-0743273565",
                              "Scribner", sdf.parse("2004-09-30"), "FICTION", 180, "ENGLISH",
                              "PAPERBACK", "Aisle 1, Shelf A", 5, 12.99,
                              "A classic American novel set in the Jazz Age.");
        book1.addTag("Classic");
        book1.addTag("American Literature");
        book1.addSubject("1920s");
        book1.addSubject("American Dream");
        book1.setCoverImageUrl("https://example.com/gatsby.jpg");
        
        Book book2 = new Book("B002", "To Kill a Mockingbird", "Harper Lee", "978-0061120084",
                              "HarperCollins", sdf.parse("2006-05-23"), "FICTION", 324, "ENGLISH",
                              "PAPERBACK", "Aisle 1, Shelf B", 3, 14.99,
                              "A gripping tale of racial injustice and childhood innocence.");
        book2.addTag("Classic");
        book2.addTag("Social Justice");
        book2.addSubject("Racism");
        book2.addSubject("Southern Gothic");
        book2.setCoverImageUrl("https://example.com/mockingbird.jpg");
        
        Book book3 = new Book("B003", "Clean Code", "Robert C. Martin", "978-0132350884",
                              "Prentice Hall", sdf.parse("2008-08-01"), "TECHNOLOGY", 464, "ENGLISH",
                              "HARDCOVER", "Aisle 3, Shelf C", 2, 39.99,
                              "A handbook of agile software craftsmanship.");
        book3.addTag("Programming");
        book3.addTag("Software Engineering");
        book3.addSubject("Clean Code");
        book3.addSubject("Agile Development");
        book3.setCoverImageUrl("https://example.com/cleancode.jpg");
        
        Book book4 = new Book("B004", "A Brief History of Time", "Stephen Hawking", "978-0553380163",
                              "Bantam", sdf.parse("1998-09-01"), "SCIENCE", 256, "ENGLISH",
                              "PAPERBACK", "Aisle 4, Shelf D", 4, 18.99,
                              "From the Big Bang to black holes, explained for the general reader.");
        book4.addTag("Physics");
        book4.addTag("Cosmology");
        book4.addSubject("Universe");
        book4.addSubject("Black Holes");
        book4.setCoverImageUrl("https://example.com/briefhistory.jpg");
        
        Book book5 = new Book("B005", "Steve Jobs", "Walter Isaacson", "978-1451648539",
                              "Simon & Schuster", sdf.parse("2011-10-24"), "BIOGRAPHY", 656, "ENGLISH",
                              "HARDCOVER", "Aisle 5, Shelf E", 3, 29.99,
                              "The exclusive biography of Steve Jobs, based on forty interviews.");
        book5.addTag("Biography");
        book5.addTag("Technology");
        book5.addSubject("Apple");
        book5.addSubject("Innovation");
        book5.setCoverImageUrl("https://example.com/stevejobs.jpg");
        
        // Add books to library
        library.addBook(book1);
        library.addBook(book2);
        library.addBook(book3);
        library.addBook(book4);
        library.addBook(book5);
        
        // Create members
        Member member1 = new Member("M001", "John", "Smith", "john.smith@email.com",
                                  "(555) 111-2222", "123 Main St", "City", "State", "12345",
                                  "ADULT");
        member1.addFavoriteCategory("FICTION");
        member1.addFavoriteCategory("SCIENCE");
        member1.updatePreference("notificationEmail", "true");
        
        Member member2 = new Member("M002", "Emily", "Johnson", "emily.johnson@email.com",
                                  "(555) 333-4444", "456 Oak Ave", "City", "State", "54321",
                                  "STUDENT");
        member2.addFavoriteCategory("TECHNOLOGY");
        member2.addFavoriteCategory("NON_FICTION");
        member2.updatePreference("notificationSMS", "true");
        
        Member member3 = new Member("M003", "Michael", "Williams", "michael.williams@email.com",
                                  "(555) 555-6666", "789 Pine Rd", "City", "State", "98765",
                                  "SENIOR");
        member3.addFavoriteCategory("HISTORY");
        member3.addFavoriteCategory("BIOGRAPHY");
        member3.updatePreference("autoRenew", "true");
        
        Member member4 = new Member("M004", "Sarah", "Brown", "sarah.brown@email.com",
                                  "(555) 777-8888", "321 Elm Blvd", "City", "State", "13579",
                                  "ADULT");
        member4.addFavoriteCategory("FICTION");
        member4.addFavoriteCategory("CHILDREN");
        
        Member member5 = new Member("M005", "David", "Davis", "david.davis@email.com",
                                  "(555) 999-0000", "654 Maple Dr", "City", "State", "24680",
                                  "ADULT");
        member5.addFavoriteCategory("NON_FICTION");
        member5.addFavoriteCategory("ACADEMIC");
        
        // Add members to library
        library.addMember(member1);
        library.addMember(member2);
        library.addMember(member3);
        library.addMember(member4);
        library.addMember(member5);
        
        // Borrow books
        String record1 = library.borrowBook("B001", "M001");
        String record2 = library.borrowBook("B002", "M002");
        String record3 = library.borrowBook("B003", "M003");
        String record4 = library.borrowBook("B004", "M004");
        String record5 = library.borrowBook("B005", "M005");
        
        // Reserve a book that's already checked out
        library.borrowBook("B001", "M002");
        library.reserveBook("B001", "M002");
        
        // Renew a book
        if (record1 != null) {
            library.renewBook(record1);
        }
        
        // Return a book
        if (record3 != null) {
            library.returnBook(record3);
        }
        
        // Print library information
        System.out.println("Library Management System");
        System.out.println("=========================");
        System.out.println("Library: " + library.getLibraryName());
        System.out.println("Code: " + library.getLibraryCode());
        System.out.println("Address: " + library.getAddress());
        System.out.println("Phone: " + library.getPhone());
        System.out.println("Email: " + library.getEmail());
        System.out.println("Total Books: " + library.getBooks().size());
        System.out.println("Total Members: " + library.getMembers().size());
        System.out.println("Total Borrowing Records: " + library.getBorrowingRecords().size());
        System.out.println();
        
        // Print book summaries
        System.out.println("Book Summaries");
        System.out.println("===============");
        for (Book book : library.getBooks()) {
            System.out.println(book.getSummary());
            System.out.println();
        }
        
        // Print member summaries
        System.out.println("Member Summaries");
        System.out.println("=================");
        for (Member member : library.getMembers()) {
            System.out.println(member.getSummary());
            System.out.println();
        }
        
        // Print borrowing record summaries
        System.out.println("Borrowing Record Summaries");
        System.out.println("===========================");
        for (BorrowingRecord record : library.getBorrowingRecords()) {
            System.out.println(record.getSummary());
            System.out.println();
        }
        
        // Search books
        System.out.println("\nBook Search Results for 'Code':");
        List<Book> searchResults = library.searchBooks("Code");
        for (Book book : searchResults) {
            System.out.println("- " + book.getTitle() + " by " + book.getAuthor());
        }
        
        System.out.println("\nFiction Books:");
        List<Book> fictionBooks = library.getBooksByCategory("FICTION");
        for (Book book : fictionBooks) {
            System.out.println("- " + book.getTitle() + " by " + book.getAuthor());
        }
        
        System.out.println("\nAdult Members:");
        List<Member> adultMembers = library.getMembersByMembershipType("ADULT");
        for (Member member : adultMembers) {
            System.out.println("- " + member.getFullName());
        }
        
        System.out.println("\nOverdue Records:");
        List<BorrowingRecord> overdueRecords = library.getOverdueRecords();
        for (BorrowingRecord record : overdueRecords) {
            System.out.println("- Record " + record.getRecordId() + ": " + record.getDaysOverdue() + " days overdue");
        }
        
        // Generate and print library report
        String report = library.generateLibraryReport();
        System.out.println("\n" + report);
        
        // Save data to file
        library.saveToFile("library_data_export.txt");
        
        System.out.println("\nLibrary Management System demonstration completed successfully!");
    }
}
<｜fim▁hole｜>