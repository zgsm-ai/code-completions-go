/**
 * Library Management System
 * 
 * This class implements a comprehensive library management system
 * that can handle books, members, loans, and catalog management.
 * 
 * Features:
 * - Book catalog management
 * - Member registration and management
 * - Book loan and return tracking
 * - Inventory management
 * - Search functionality
 * - Fine calculation
 * - Reporting and analytics
 */

import java.util.*;
import java.text.*;
import java.io.*;

/**
 * Book class represents a book in the library
 */
class Book {
    private String isbn;
    private String title;
    private String author;
    private String publisher;
    private Date publishDate;
    private String genre;
    private int pageCount;
    private String language;
    private boolean available;
    private Date dueDate;
    private String borrowedBy;
    
    /**
     * Constructor for Book
     * @param ISBN ISBN number
     * @param title Book title
     * @param author Book author
     * @param publisher Book publisher
     * @param publishDate Publication date
     * @param genre Book genre
     * @param pageCount Page count
     * @param language Book language
     */
    public Book(String isbn, String title, String author, String publisher,
               Date publishDate, String genre, int pageCount, String language) {
        this.isbn = isbn;
        this.title = title;
        this.author = author;
        this.publisher = publisher;
        this.publishDate = publishDate;
        this.genre = genre;
        this.pageCount = pageCount;
        this.language = language;
        this.available = true;
        this.dueDate = null;
        this.borrowedBy = null;
    }
    
    /**
     * Get ISBN
     * @return ISBN number
     */
    public String getIsbn() {
        return isbn;
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
     * Get publisher
     * @return Book publisher
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
     * Get genre
     * @return Book genre
     */
    public String getGenre() {
        return genre;
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
     * Check if book is available
     * @return True if available, false otherwise
     */
    public boolean isAvailable() {
        return available;
    }
    
    /**
     * Get due date
     * @return Due date
     */
    public Date getDueDate() {
        return dueDate;
    }
    
    /**
     * Get borrower
     * @return Borrower ID
     */
    public String getBorrowedBy() {
        return borrowedBy;
    }
    
    /**
     * Set availability
     * @param available Availability status
     */
    public void setAvailable(boolean available) {
        this.available = available;
    }
    
    /**
     * Set due date
     * @param dueDate Due date
     */
    public void setDueDate(Date dueDate) {
        this.dueDate = dueDate;
    }
    
    /**
     * Set borrower
     * @param borrowedBy Borrower ID
     */
    public void setBorrowedBy(String borrowedBy) {
        this.borrowedBy = borrowedBy;
    }
    
    /**
     * Get book summary
     * @return Book summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("ISBN: ").append(isbn).append("\n");
        sb.append("Title: ").append(title).append("\n");
        sb.append("Author: ").append(author).append("\n");
        sb.append("Publisher: ").append(publisher).append("\n");
        sb.append("Publish Date: ").append(publishDate).append("\n");
        sb.append("Genre: ").append(genre).append("\n");
        sb.append("Page Count: ").append(pageCount).append("\n");
        sb.append("Language: ").append(language).append("\n");
        sb.append("Available: ").append(available ? "Yes" : "No").append("\n");
        sb.append("Due Date: ").append(dueDate != null ? dueDate : "N/A").append("\n");
        sb.append("Borrowed By: ").append(borrowedBy != null ? borrowedBy : "N/A").append("\n");
        
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
    private String address;
    private String phone;
    private String email;
    private Date registrationDate;
    private Date expirationDate;
    private boolean active;
    private List<String> borrowedBooks;
    
    /**
     * Constructor for Member
     * @param memberId Member ID
     * @param firstName First name
     * @param lastName Last name
     * @param address Address
     * @param phone Phone number
     * @param email Email address
     * @param registrationDate Registration date
     * @param expirationDate Expiration date
     * @param active Active status
     */
    public Member(String memberId, String firstName, String lastName,
                String address, String phone, String email,
                Date registrationDate, Date expirationDate, boolean active) {
        this.memberId = memberId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.registrationDate = registrationDate;
        this.expirationDate = expirationDate;
        this.active = active;
        this.borrowedBooks = new ArrayList<>();
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
     * Get address
     * @return Address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get phone number
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get email address
     * @return Email address
     */
    public String getEmail() {
        return email;
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
     * Check if membership is active
     * @return True if active, false otherwise
     */
    public boolean isActive() {
        return active;
    }
    
    /**
     * Get borrowed books
     * @return List of borrowed book ISBNs
     */
    public List<String> getBorrowedBooks() {
        return borrowedBooks;
    }
    
    /**
     * Set active status
     * @param active Active status
     */
    public void setActive(boolean active) {
        this.active = active;
    }
    
    /**
     * Set expiration date
     * @param expirationDate Expiration date
     */
    public void setExpirationDate(Date expirationDate) {
        this.expirationDate = expirationDate;
    }
    
    /**
     * Borrow a book
     * @param isbn ISBN of the book
     * @return True if borrowed successfully, false otherwise
     */
    public boolean borrowBook(String isbn) {
        if (borrowedBooks.size() >= 5) {  // Maximum of 5 books
            return false;
        }
        
        if (borrowedBooks.contains(isbn)) {
            return false;
        }
        
        borrowedBooks.add(isbn);
        return true;
    }
    
    /**
     * Return a book
     * @param isbn ISBN of the book
     * @return True if returned successfully, false otherwise
     */
    public boolean returnBook(String isbn) {
        if (borrowedBooks.contains(isbn)) {
            borrowedBooks.remove(isbn);
            return true;
        }
        
        return false;
    }
    
    /**
     * Get member summary
     * @return Member summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Member ID: ").append(memberId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Address: ").append(address).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Registration Date: ").append(registrationDate).append("\n");
        sb.append("Expiration Date: ").append(expirationDate).append("\n");
        sb.append("Status: ").append(active ? "Active" : "Inactive").append("\n");
        sb.append("Borrowed Books: ").append(borrowedBooks.size()).append("\n");
        sb.append("Borrowed Book List: ").append(borrowedBooks.toString()).append("\n");
        
        return sb.toString();
    }
}

/**
 * Loan class represents a book loan
 */
class Loan {
    private String loanId;
    private String memberId;
    private String isbn;
    private Date loanDate;
    private Date dueDate;
    private Date returnDate;
    private double fine;
    private boolean active;
    
    /**
     * Constructor for Loan
     * @param loanId Loan ID
     * @param memberId Member ID
     * @param isbn ISBN of the book
     * @param loanDate Loan date
     * @param dueDate Due date
     */
    public Loan(String loanId, String memberId, String isbn,
               Date loanDate, Date dueDate) {
        this.loanId = loanId;
        this.memberId = memberId;
        this.isbn = isbn;
        this.loanDate = loanDate;
        this.dueDate = dueDate;
        this.returnDate = null;
        this.fine = 0.0;
        this.active = true;
    }
    
    /**
     * Get loan ID
     * @return Loan ID
     */
    public String getLoanId() {
        return loanId;
    }
    
    /**
     * Get member ID
     * @return Member ID
     */
    public String getMemberId() {
        return memberId;
    }
    
    /**
     * Get ISBN
     * @return ISBN of the book
     */
    public String getIsbn() {
        return isbn;
    }
    
    /**
     * Get loan date
     * @return Loan date
     */
    public Date getLoanDate() {
        return loanDate;
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
     * Get fine
     * @return Fine amount
     */
    public double getFine() {
        return fine;
    }
    
    /**
     * Check if loan is active
     * @return True if active, false otherwise
     */
    public boolean isActive() {
        return active;
    }
    
    /**
     * Return the book
     * @param returnDate Return date
     * @return True if returned successfully, false otherwise
     */
    public boolean returnBook(Date returnDate) {
        if (!active) {
            return false;
        }
        
        this.returnDate = returnDate;
        
        // Calculate fine if overdue
        if (returnDate.after(dueDate)) {
            long daysLate = (returnDate.getTime() - dueDate.getTime()) / (1000 * 60 * 60 * 24);
            this.fine = daysLate * 0.50;  // $0.50 per day
        }
        
        this.active = false;
        return true;
    }
    
    /**
     * Get loan summary
     * @return Loan summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Loan ID: ").append(loanId).append("\n");
        sb.append("Member ID: ").append(memberId).append("\n");
        sb.append("ISBN: ").append(isbn).append("\n");
        sb.append("Loan Date: ").append(loanDate).append("\n");
        sb.append("Due Date: ").append(dueDate).append("\n");
        sb.append("Return Date: ").append(returnDate != null ? returnDate : "N/A").append("\n");
        sb.append("Fine: $").append(String.format("%.2f", fine)).append("\n");
        sb.append("Status: ").append(active ? "Active" : "Returned").append("\n");
        
        return sb.toString();
    }
}

/**
 * LibraryManagementSystem class manages the entire library
 */
public class LibraryManagementSystem {
    private String libraryName;
    private String address;
    private List<Book> books;
    private Map<String, Book> bookMap;
    private List<Member> members;
    private Map<String, Member> memberMap;
    private List<Loan> loans;
    private Map<String, Loan> loanMap;
    
    /**
     * Constructor for LibraryManagementSystem
     * @param libraryName Library name
     * @param address Library address
     */
    public LibraryManagementSystem(String libraryName, String address) {
        this.libraryName = libraryName;
        this.address = address;
        this.books = new ArrayList<>();
        this.bookMap = new HashMap<>();
        this.members = new ArrayList<>();
        this.memberMap = new HashMap<>();
        this.loans = new ArrayList<>();
        this.loanMap = new HashMap<>();
    }
    
    /**
     * Get library name
     * @return Library name
     */
    public String getLibraryName() {
        return libraryName;
    }
    
    /**
     * Get library address
     * @return Library address
     */
    public String getAddress() {
        return address;
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
     * Get loans
     * @return List of loans
     */
    public List<Loan> getLoans() {
        return loans;
    }
    
    /**
     * Add a book to the library
     * @param book Book to add
     * @return True if added successfully, false otherwise
     */
    public boolean addBook(Book book) {
        if (bookMap.containsKey(book.getIsbn())) {
            return false;
        }
        
        books.add(book);
        bookMap.put(book.getIsbn(), book);
        return true;
    }
    
    /**
     * Remove a book from the library
     * @param isbn ISBN of the book to remove
     * @return True if removed successfully, false otherwise
     */
    public boolean removeBook(String isbn) {
        if (!bookMap.containsKey(isbn)) {
            return false;
        }
        
        books.remove(bookMap.get(isbn));
        bookMap.remove(isbn);
        return true;
    }
    
    /**
     * Get a book by ISBN
     * @param isbn ISBN of the book
     * @return Book if found, null otherwise
     */
    public Book getBook(String isbn) {
        return bookMap.get(isbn);
    }
    
    /**
     * Add a member to the library
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
     * Remove a member from the library
     * @param memberId Member ID
     * @return True if removed successfully, false otherwise
     */
    public boolean removeMember(String memberId) {
        if (!memberMap.containsKey(memberId)) {
            return false;
        }
        
        members.remove(memberMap.get(memberId));
        memberMap.remove(memberId);
        return true;
    }
    
    /**
     * Get a member by ID
     * @param memberId Member ID
     * @return Member if found, null otherwise
     */
    public Member getMember(String memberId) {
        return memberMap.get(memberId);
    }
    
    /**
     * Lend a book
     * @param isbn ISBN of the book
     * @param memberId Member ID
     * @return True if loaned successfully, false otherwise
     */
    public boolean lendBook(String isbn, String memberId) {
        Book book = getBook(isbn);
        Member member = getMember(memberId);
        
        if (book == null) {
            return false;
        }
        
        if (member == null) {
            return false;
        }
        
        if (!book.isAvailable()) {
            return false;
        }
        
        if (!member.isActive()) {
            return false;
        }
        
        Date today = new Date();
        Calendar cal = Calendar.getInstance();
        cal.setTime(today);
        cal.add(Calendar.DAY_OF_MONTH, 14);  // Due in 2 weeks
        
        // Create loan
        String loanId = generateLoanId();
        Date dueDate = cal.getTime();
        
        Loan loan = new Loan(loanId, memberId, isbn, today, dueDate);
        
        // Update book status
        book.setAvailable(false);
        book.setDueDate(dueDate);
        book.setBorrowedBy(memberId);
        
        // Update member borrowed books
        member.borrowBook(isbn);
        
        // Add loan to system
        loans.add(loan);
        loanMap.put(loanId, loan);
        
        return true;
    }
    
    /**
     * Return a book
     * @param loanId Loan ID
     * @param returnDate Return date
     * @return True if returned successfully, false otherwise
     */
    public boolean returnBook(String loanId, Date returnDate) {
        Loan loan = loanMap.get(loanId);
        
        if (loan == null) {
            return false;
        }
        
        if (!loan.isActive()) {
            return false;
        }
        
        // Return the book
        if (!loan.returnBook(returnDate)) {
            return false;
        }
        
        // Update book status
        Book book = getBook(loan.getIsbn());
        if (book != null) {
            book.setAvailable(true);
            book.setDueDate(null);
            book.setBorrowedBy(null);
            
            // Update member borrowed books
            Member member = getMember(loan.getMemberId());
            if (member != null) {
                member.returnBook(loan.getIsbn());
            }
        }
        
        return true;
    }
    
    /**
     * Generate a unique loan ID
     * @return Generated loan ID
     */
    private String generateLoanId() {
        Random random = new Random();
        int number = 1000 + random.nextInt(9000);
        return "L" + String.valueOf(number);
    }
    
    /**
     * Search books by title
     * @param title Title to search
     * @return List of matching books
     */
    public List<Book> searchBooksByTitle(String title) {
        List<Book> results = new ArrayList<>();
        String lowerTitle = title.toLowerCase();
        
        for (Book book : books) {
            if (book.getTitle().toLowerCase().contains(lowerTitle)) {
                results.add(book);
            }
        }
        
        return results;
    }
    
    /**
     * Search books by author
     * @param author Author to search
     * @return List of matching books
     */
    public List<Book> searchBooksByAuthor(String author) {
        List<Book> results = new ArrayList<>();
        String lowerAuthor = author.toLowerCase();
        
        for (Book book : books) {
            if (book.getAuthor().toLowerCase().contains(lowerAuthor)) {
                results.add(book);
            }
        }
        
        return results;
    }
    
    /**
     * Search books by genre
     * @param genre Genre to search
     * @return List of matching books
     */
    public List<Book> searchBooksByGenre(String genre) {
        List<Book> results = new ArrayList<>();
        String lowerGenre = genre.toLowerCase();
        
        for (Book book : books) {
            if (book.getGenre().toLowerCase().contains(lowerGenre)) {
                results.add(book);
            }
        }
        
        return results;
    }
    
    /**
     * Get available books
     * @return List of available books
     */
    public List<Book> getAvailableBooks() {
        List<Book> availableBooks = new ArrayList<>();
        
        for (Book book : books) {
            if (book.isAvailable()) {
                availableBooks.add(book);
            }
        }
        
        return availableBooks;
    }
    
    /**
     * Get overdue books
     * @return List of overdue books
     */
    public List<Loan> getOverdueLoans() {
        List<Loan> overdueLoans = new ArrayList<>();
        Date today = new Date();
        
        for (Loan loan : loans) {
            if (loan.isActive() && loan.getDueDate().before(today)) {
                overdueLoans.add(loan);
            }
        }
        
        return overdueLoans;
    }
    
    /**
     * Get total fine amount
     * @return Total fine amount
     */
    public double getTotalFineAmount() {
        double totalFine = 0.0;
        
        for (Loan loan : loans) {
            totalFine += loan.getFine();
        }
        
        return totalFine;
    }
    
    /**
     * Calculate overdue fine for a loan
     * @param loanId Loan ID
     * @return Calculated fine
     */
    public double calculateFine(String loanId) {
        Loan loan = loanMap.get(loanId);
        if (loan == null || !loan.isActive()) {
            return 0.0;
        }
        
        Date today = new Date();
        
        if (loan.getDueDate().before(today)) {
            long daysLate = (today.getTime() - loan.getDueDate().getTime()) / (1000 * 60 * 60 * 24);
            return daysLate * 0.50;  // $0.50 per day
        }
        
        return 0.0;
    }
    
    /**
     * Get most popular books
     * @param limit Maximum number of books to return
     * @return List of most popular books
     */
    public List<Book> getMostPopularBooks(int limit) {
        Map<String, Integer> borrowCount = new HashMap<>();
        
        // Count how many times each book has been borrowed
        for (Loan loan : loans) {
            String isbn = loan.getIsbn();
            borrowCount.put(isbn, borrowCount.getOrDefault(isbn, 0) + 1);
        }
        
        // Create a list of books with their borrow count
        List<Map.Entry<String, Integer>> entries = new ArrayList<>(borrowCount.entrySet());
        Collections.sort(entries, (a, b) -> b.getValue().compareTo(a.getValue()));
        
        List<Book> popularBooks = new ArrayList<>();
        for (int i = 0; i < Math.min(limit, entries.size()); i++) {
            String isbn = entries.get(i).getKey();
            Book book = getBook(isbn);
            if (book != null) {
                popularBooks.add(book);
            }
        }
        
        return popularBooks;
    }
    
    /**
     * Get most active members
     * @param limit Maximum number of members to return
     * @return List of most active members
     */
    public List<Member> getMostActiveMembers(int limit) {
        Map<String, Integer> borrowCount = new HashMap<>();
        
        // Count how many books each member has borrowed
        for (Loan loan : loans) {
            String memberId = loan.getMemberId();
            borrowCount.put(memberId, borrowCount.getOrDefault(memberId, 0) + 1);
        }
        
        // Create a list of members with their borrow count
        List<Map.Entry<String, Integer>> entries = new ArrayList<>(borrowCount.entrySet());
        Collections.sort(entries, (a, b) -> b.getValue().compareTo(a.getValue()));
        
        List<Member> activeMembers = new ArrayList<>();
        for (int i = 0; i < Math.min(limit, entries.size()); i++) {
            String memberId = entries.get(i).getKey();
            Member member = getMember(memberId);
            if (member != null) {
                activeMembers.add(member);
            }
        }
        
        return activeMembers;
    }
    
    /**
     * Generate library report
     * @return Library report string
     */
    public String generateLibraryReport() {
        StringBuilder sb = new StringBuilder();
        sb.append("======================================\n");
        sb.append("           LIBRARY REPORT\n");
        sb.append("======================================\n");
        sb.append("Library Name: ").append(libraryName).append("\n");
        sb.append("Address: ").append(address).append("\n");
        sb.append("Report Date: ").append(new Date()).append("\n\n");
        
        sb.append("SUMMARY STATISTICS\n");
        sb.append("-------------------\n");
        sb.append("Total Books: ").append(books.size()).append("\n");
        sb.append("Total Members: ").append(members.size()).append("\n");
        sb.append("Active Loans: ").append(getActiveLoanCount()).append("\n");
        sb.append("Overdue Loans: ").append(getOverdueLoanCount()).append("\n");
        sb.append("Available Books: ").append(getAvailableBookCount()).append("\n");
        sb.append("Total Fine Amount: $").append(String.format("%.2f", getTotalFineAmount())).append("\n\n");
        
        sb.append("TOP 5 MOST POPULAR BOOKS\n");
        sb.append("-------------------------\n");
        List<Book> popularBooks = getMostPopularBooks(5);
        for (int i = 0; i < popularBooks.size(); i++) {
            Book book = popularBooks.get(i);
            sb.append(i + 1).append(". ").append(book.getTitle()).append(" (").append(book.getIsbn()).append(")\n");
        }
        
        sb.append("\nTOP 5 MOST ACTIVE MEMBERS\n");
        sb.append("-----------------------\n");
        List<Member> activeMembers = getMostActiveMembers(5);
        for (int i = 0; i < activeMembers.size(); i++) {
            Member member = activeMembers.get(i);
            sb.append(i + 1).append(". ").append(member.getFullName()).append(" (").append(member.getMemberId()).append(")\n");
        }
        
        sb.append("\nOVERDUE LOANS\n");
        sb.append("--------------\n");
        List<Loan> overdueLoans = getOverdueLoans();
        for (int i = 0; i < overdueLoans.size(); i++) {
            Loan loan = overdueLoans.get(i);
            Book book = getBook(loan.getIsbn());
            Member member = getMember(loan.getMemberId());
            
            sb.append("Loan ID: ").append(loan.getLoanId()).append("\n");
            if (book != null) {
                sb.append("Book: ").append(book.getTitle()).append("\n");
            }
            if (member != null) {
                sb.append("Member: ").append(member.getFullName()).append("\n");
            }
            sb.append("Due Date: ").append(loan.getDueDate()).append("\n");
            sb.append("Fine: $").append(String.format("%.2f", loan.getFine())).append("\n\n");
        }
        
        return sb.toString();
    }
    
    /**
     * Get active loan count
     * @return Number of active loans
     */
    private int getActiveLoanCount() {
        int count = 0;
        
        for (Loan loan : loans) {
            if (loan.isActive()) {
                count++;
            }
        }
        
        return count;
    }
    
    /**
     * Get overdue loan count
     * @return Number of overdue loans
     */
    private int getOverdueLoanCount() {
        Date today = new Date();
        int count = 0;
        
        for (Loan loan : loans) {
            if (loan.isActive() && loan.getDueDate().before(today)) {
                count++;
            }
        }
        
        return count;
    }
    
    /**
     * Get available book count
     * @return Number of available books
     */
    private int getAvailableBookCount() {
        int count = 0;
        
        for (Book book : books) {
            if (book.isAvailable()) {
                count++;
            }
        }
        
        return count;
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
            writer.println("Library Name: " + libraryName);
            writer.println("Address: " + address);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export books
            writer.println("BOOKS");
            writer.println("-----");
            for (Book book : books) {
                writer.println("ISBN: " + book.getIsbn());
                writer.println("Title: " + book.getTitle());
                writer.println("Author: " + book.getAuthor());
                writer.println("Publisher: " + book.getPublisher());
                writer.println("Publish Date: " + book.getPublishDate());
                writer.println("Genre: " + book.getGenre());
                writer.println("Page Count: " + book.getPageCount());
                writer.println("Language: " + book.getLanguage());
                writer.println("Available: " + (book.isAvailable() ? "Yes" : "No"));
                writer.println("Due Date: " + (book.getDueDate() != null ? book.getDueDate() : "N/A"));
                writer.println("Borrowed By: " + (book.getBorrowedBy() != null ? book.getBorrowedBy() : "N/A"));
                writer.println();
            }
            
            // Export members
            writer.println("MEMBERS");
            writer.println("--------");
            for (Member member : members) {
                writer.println("ID: " + member.getMemberId());
                writer.println("Name: " + member.getFullName());
                writer.println("Address: " + member.getAddress());
                writer.println("Phone: " + member.getPhone());
                writer.println("Email: " + member.getEmail());
                writer.println("Registration Date: " + member.getRegistrationDate());
                writer.println("Expiration Date: " + member.getExpirationDate());
                writer.println("Status: " + (member.isActive() ? "Active" : "Inactive"));
                writer.println("Borrowed Books: " + member.getBorrowedBooks().size());
                writer.println();
            }
            
            // Export loans
            writer.println("LOANS");
            writer.println("----");
            for (Loan loan : loans) {
                writer.println("ID: " + loan.getLoanId());
                writer.println("Member ID: " + loan.getMemberId());
                writer.println("ISBN: " + loan.getIsbn());
                writer.println("Loan Date: " + loan.getLoanDate());
                writer.println("Due Date: " + loan.getDueDate());
                writer.println("Return Date: " + (loan.getReturnDate() != null ? loan.getReturnDate() : "N/A"));
                writer.println("Fine: $" + String.format("%.2f", loan.getFine()));
                writer.println("Status: " + (loan.isActive() ? "Active" : "Returned"));
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
        LibraryManagementSystem library = new LibraryManagementSystem("City Library", "123 Main St, Anytown, USA");
        
        // Create some books
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
        
        Book book1 = new Book("978-3-16-148410-0", "The Great Gatsby", 
                                "F. Scott Fitzgerald", "Scribner", 
                                dateFormat.parse("1925-04-10"), 
                                "Fiction", 180, "English");
        
        Book book2 = new Book("978-0-06-081771-2", "To Kill a Mockingbird", 
                                "Harper Lee", "J.B. Lippincott & Co.", 
                                dateFormat.parse("1960-07-11"), 
                                "Fiction", 281, "English");
        
        Book book3 = new Book("978-1-4028-9462-4", "1984", 
                                "George Orwell", "Secker & Warburg", 
                                dateFormat.parse("1949-06-08"), 
                                "Fiction", 328, "English");
        
        Book book4 = new Book("978-0-06-085590-3", "Brave New World", 
                                "Aldous Huxley", "Chatto & Windus", 
                                dateFormat.parse("1932-01-01"), 
                                "Fiction", 311, "English");
        
        Book book5 = new Book("978-0-7432-7353-3", "The Catcher in the Rye", 
                                "J.D. Salinger", "Little, Brown and Company", 
                                dateFormat.parse("1951-07-16"), 
                                "Fiction", 277, "English");
        
        // Add books to library
        library.addBook(book1);
        library.addBook(book2);
        library.addBook(book3);
        library.addBook(book4);
        library.addBook(book5);
        
        // Create some members
        try {
            Member member1 = new Member("M001", "John", "Smith", 
                                      "123 Main St, Anytown, USA", 
                                      "(555) 123-4567", "john.smith@email.com",
                                      dateFormat.parse("2020-01-15"), 
                                      dateFormat.parse("2024-01-15"), true);
            
            Member member2 = new Member("M002", "Jane", "Doe", 
                                      "456 Oak Ave, Anytown, USA", 
                                      "(555) 987-6543", "jane.doe@email.com",
                                      dateFormat.parse("2019-09-10"), 
                                      dateFormat.parse("2023-09-10"), true);
            
            Member member3 = new Member("M003", "Michael", "Johnson", 
                                      "789 Pine Rd, Anytown, USA", 
                                      "(555) 246-8135", "michael.johnson@email.com",
                                      dateFormat.parse("2021-03-20"), 
                                      dateFormat.parse("2025-03-20"), true);
            
            // Add members to library
            library.addMember(member1);
            library.addMember(member2);
            library.addMember(member3);
            
            // Lend some books
            library.lendBook("978-3-16-148410-0", "M001");  // Gatsby to John
            library.lendBook("978-0-06-081771-2", "M002");      // Mockingbird to Jane
            library.lendBook("978-1-4028-9462-4", "M003");      // 1984 to Michael
            
            // Generate and print library report
            String report = library.generateLibraryReport();
            System.out.println(report);
            
            // Print book details
            System.out.println("\n\nBOOK DETAILS");
            System.out.println("=============");
            for (Book book : library.getBooks()) {
                System.out.println(book.getSummary());
                System.out.println();
            }
            
            // Print member details
            System.out.println("\n\nMEMBER DETAILS");
            System.out.println("==============");
            for (Member member : library.getMembers()) {
                System.out.println(member.getSummary());
                System.out.println();
            }
            
            // Print loan details
            System.out.println("\n\nLOAN DETAILS");
            System.out.println("=============");
            for (Loan loan : library.getLoans()) {
                System.out.println(loan.getSummary());
                System.out.println();
            }
            
            // Return some books
            Date today = new Date();
            library.returnBook("L1001", today);  // Return Gatsby
            library.returnBook("L1002", today);  // Return Mockingbird
            
            // Calculate fines
            System.out.println("\n\nFINE CALCULATIONS");
            System.out.println("==================");
            for (Loan loan : library.getLoans()) {
                double fine = library.calculateFine(loan.getLoanId());
                System.out.println("Loan ID " + loan.getLoanId() + ": $" + String.format("%.2f", fine));
            }
            
            // Save library data to file
            library.saveToFile("library_data_export.txt");
            
            System.out.println("\n\nLibrary Management System demonstration completed successfully!");
        } catch (ParseException e) {
            System.err.println("Error parsing date: " + e.getMessage());
        }
    }
}
<｜fim▁hole｜>