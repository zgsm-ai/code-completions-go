#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 1000
#define MAX_USERS 500
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_USER_NAME_LENGTH 50
#define MAX_ISSUED_BOOKS 5
#define FILENAME_BOOKS "books.dat"
#define FILENAME_USERS "users.dat"

typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char isbn[20];
    char category[30];
    int publication_year;
    int total_copies;
    int available_copies;
    int times_borrowed;
} Book;

typedef struct {
    int id;
    char name[MAX_USER_NAME_LENGTH];
    char email[50];
    char phone[15];
    char address[100];
    int membership_date[3]; // day, month, year
    int books_issued;
    int books_ids[MAX_ISSUED_BOOKS];
    time_t issue_dates[MAX_ISSUED_BOOKS];
    time_t due_dates[MAX_ISSUED_BOOKS];
    int fine_amount;
} User;

typedef struct {
    Book books[MAX_BOOKS];
    int book_count;
    User users[MAX_USERS];
    int user_count;
} LibraryDatabase;

// Function prototypes
void initialize_library(LibraryDatabase *library);
int add_book(LibraryDatabase *library, const char *title, const char *author, 
             const char *isbn, const char *category, int year, int copies);
int add_user(LibraryDatabase *library, const char *name, const char *email, 
             const char *phone, const char *address);
int issue_book(LibraryDatabase *library, int user_id, int book_id);
int return_book(LibraryDatabase *library, int user_id, int book_id);
void display_book(const Book *book);
void display_user(const User *user);
void display_all_books(const LibraryDatabase *library);
void display_all_users(const LibraryDatabase *library);
int find_book_by_id(const LibraryDatabase *library, int book_id);
int find_user_by_id(const LibraryDatabase *library, int user_id);
void search_books_by_title(const LibraryDatabase *library, const char *title);
void search_books_by_author(const LibraryDatabase *library, const char *author);
void search_books_by_category(const LibraryDatabase *library, const char *category);
void display_most_popular_books(const LibraryDatabase *library);
void calculate_fines(LibraryDatabase *library);
void save_library_data(const LibraryDatabase *library);
int load_library_data(LibraryDatabase *library);
void generate_sample_data(LibraryDatabase *library);
void display_overdue_books(const LibraryDatabase *library);
void display_user_history(const LibraryDatabase *library, int user_id);
int calculate_days_overdue(time_t due_date);

const char* categories[] = {
    "Fiction", "Non-Fiction", "Science", "Technology", "History",
    "Biography", "Self-Help", "Children", "Romance", "Mystery",
    "Fantasy", "Cooking", "Travel", "Art", "Music",
    "Sports", "Business", "Psychology", "Philosophy", "Religion"
};

int main() {
    LibraryDatabase library;
    initialize_library(&library);
    
    printf("Library Management System\n");
    printf("1. Add Book\n");
    printf("2. Add User\n");
    printf("3. Issue Book\n");
    printf("4. Return Book\n");
    printf("5. Display All Books\n");
    printf("6. Display All Users\n");
    printf("7. Search Book by Title\n");
    printf("8. Search Book by Author\n");
    printf("9. Search Book by Category\n");
    printf("10. Display Most Popular Books\n");
    printf("11. Calculate Fines\n");
    printf("12. Display Overdue Books\n");
    printf("13. Display User History\n");
    printf("14. Save Library Data\n");
    printf("15. Load Library Data\n");
    printf("16. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char title[MAX_TITLE_LENGTH], author[MAX_AUTHOR_LENGTH];
                char isbn[20], category[30];
                int year, copies;
                
                printf("Enter book title: ");
                scanf(" %[^\n]", title);
                printf("Enter author name: ");
                scanf(" %[^\n]", author);
                printf("Enter ISBN: ");
                scanf("%s", isbn);
                printf("Enter category: ");
                scanf(" %[^\n]", category);
                printf("Enter publication year: ");
                scanf("%d", &year);
                printf("Enter number of copies: ");
                scanf("%d", &copies);
                
                int book_id = add_book(&library, title, author, isbn, category, year, copies);
                if (book_id != -1) {
                    printf("Book added with ID: %d\n", book_id);
                } else {
                    printf("Failed to add book. Library is full.\n");
                }
                break;
            }
            case 2: {
                char name[MAX_USER_NAME_LENGTH], email[50];
                char phone[15], address[100];
                int day, month, year;
                
                printf("Enter user name: ");
                scanf(" %[^\n]", name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter membership date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                int user_id = add_user(&library, name, email, phone, address);
                if (user_id != -1) {
                    library.users[user_id - 1].membership_date[0] = day;
                    library.users[user_id - 1].membership_date[1] = month;
                    library.users[user_id - 1].membership_date[2] = year;
                    printf("User added with ID: %d\n", user_id);
                } else {
                    printf("Failed to add user. Library is full.\n");
                }
                break;
            }
            case 3: {
                int user_id, book_id;
                printf("Enter user ID: ");
                scanf("%d", &user_id);
                printf("Enter book ID: ");
                scanf("%d", &book_id);
                
                if (issue_book(&library, user_id, book_id)) {
                    printf("Book issued successfully.\n");
                } else {
                    printf("Failed to issue book. Check user and book IDs.\n");
                }
                break;
            }
            case 4: {
                int user_id, book_id;
                printf("Enter user ID: ");
                scanf("%d", &user_id);
                printf("Enter book ID: ");
                scanf("%d", &book_id);
                
                if (return_book(&library, user_id, book_id)) {
                    printf("Book returned successfully.\n");
                } else {
                    printf("Failed to return book. Check user and book IDs.\n");
                }
                break;
            }
            case 5: {
                display_all_books(&library);
                break;
            }
            case 6: {
                display_all_users(&library);
                break;
            }
            case 7: {
                char title[MAX_TITLE_LENGTH];
                printf("Enter book title to search: ");
                scanf(" %[^\n]", title);
                search_books_by_title(&library, title);
                break;
            }
            case 8: {
                char author[MAX_AUTHOR_LENGTH];
                printf("Enter author name to search: ");
                scanf(" %[^\n]", author);
                search_books_by_author(&library, author);
                break;
            }
            case 9: {
                char category[30];
                printf("Enter category to search: ");
                scanf(" %[^\n]", category);
                search_books_by_category(&library, category);
                break;
            }
            case 10: {
                display_most_popular_books(&library);
                break;
            }
            case 11: {
                calculate_fines(&library);
                printf("Fines calculated successfully.\n");
                break;
            }
            case 12: {
                display_overdue_books(&library);
                break;
            }
            case 13: {
                int user_id;
                printf("Enter user ID: ");
                scanf("%d", &user_id);
                display_user_history(&library, user_id);
                break;
            }
            case 14: {
                save_library_data(&library);
                printf("Library data saved successfully.\n");
                break;
            }
            case 15: {
                if (load_library_data(&library)) {
                    printf("Library data loaded successfully.\n");
                } else {
                    printf("Failed to load library data.\n");
                }
                break;
            }
            case 16: {
                int num_books, num_users;
                printf("Enter number of books to generate: ");
                scanf("%d", &num_books);
                printf("Enter number of users to generate: ");
                scanf("%d", &num_users);
                generate_sample_data(&library);
                printf("Sample data generated.\n");
                break;
            }
            case 0: {
                printf("Exiting program.\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 0);
    
    return 0;
}

void initialize_library(LibraryDatabase *library) {
    <｜fim▁hole｜>library->book_count = 0;
    library->user_count = 0;
}

int add_book(LibraryDatabase *library, const char *title, const char *author, 
             const char *isbn, const char *category, int year, int copies) {
    if (library->book_count >= MAX_BOOKS) {
        return -1;
    }
    
    Book *book = &library->books[library->book_count];
    book->id = library->book_count + 1;
    strncpy(book->title, title, MAX_TITLE_LENGTH - 1);
    book->title[MAX_TITLE_LENGTH - 1] = '\0';
    strncpy(book->author, author, MAX_AUTHOR_LENGTH - 1);
    book->author[MAX_AUTHOR_LENGTH - 1] = '\0';
    strncpy(book->isbn, isbn, 19);
    book->isbn[19] = '\0';
    strncpy(book->category, category, 29);
    book->category[29] = '\0';
    book->publication_year = year;
    book->total_copies = copies;
    book->available_copies = copies;
    book->times_borrowed = 0;
    
    library->book_count++;
    return book->id;
}

int add_user(LibraryDatabase *library, const char *name, const char *email, 
             const char *phone, const char *address) {
    if (library->user_count >= MAX_USERS) {
        return -1;
    }
    
    User *user = &library->users[library->user_count];
    user->id = library->user_count + 1;
    strncpy(user->name, name, MAX_USER_NAME_LENGTH - 1);
    user->name[MAX_USER_NAME_LENGTH - 1] = '\0';
    strncpy(user->email, email, 49);
    user->email[49] = '\0';
    strncpy(user->phone, phone, 14);
    user->phone[14] = '\0';
    strncpy(user->address, address, 99);
    user->address[99] = '\0';
    user->books_issued = 0;
    user->fine_amount = 0;
    
    library->user_count++;
    return user->id;
}

int issue_book(LibraryDatabase *library, int user_id, int book_id) {
    int user_index = find_user_by_id(library, user_id);
    int book_index = find_book_by_id(library, book_id);
    
    if (user_index == -1 || book_index == -1) {
        return 0;
    }
    
    User *user = &library->users[user_index];
    Book *book = &library->books[book_index];
    
    if (user->books_issued >= MAX_ISSUED_BOOKS) {
        printf("User has reached maximum book limit.\n");
        return 0;
    }
    
    if (book->available_copies <= 0) {
        printf("No copies available for this book.\n");
        return 0;
    }
    
    // Issue the book
    user->books_ids[user->books_issued] = book_id;
    user->issue_dates[user->books_issued] = time(NULL);
    user->due_dates[user->books_issued] = time(NULL) + (14 * 24 * 60 * 60); // 14 days
    user->books_issued++;
    
    book->available_copies--;
    book->times_borrowed++;
    
    return 1;
}

int return_book(LibraryDatabase *library, int user_id, int book_id) {
    int user_index = find_user_by_id(library, user_id);
    int book_index = find_book_by_id(library, book_id);
    
    if (user_index == -1 || book_index == -1) {
        return 0;
    }
    
    User *user = &library->users[user_index];
    Book *book = &library->books[book_index];
    
    // Find the book in user's issued books
    int found_index = -1;
    for (int i = 0; i < user->books_issued; i++) {
        if (user->books_ids[i] == book_id) {
            found_index = i;
            break;
        }
    }
    
    if (found_index == -1) {
        printf("User has not issued this book.\n");
        return 0;
    }
    
    // Calculate fine if overdue
    time_t now = time(NULL);
    int days_overdue = calculate_days_overdue(user->due_dates[found_index]);
    if (days_overdue > 0) {
        int fine = days_overdue * 2; // $2 per day
        user->fine_amount += fine;
        printf("Book returned %d days late. Fine: $%d\n", days_overdue, fine);
    }
    
    // Remove the book from user's issued books
    for (int i = found_index; i < user->books_issued - 1; i++) {
        user->books_ids[i] = user->books_ids[i + 1];
        user->issue_dates[i] = user->issue_dates[i + 1];
        user->due_dates[i] = user->due_dates[i + 1];
    }
    user->books_issued--;
    
    // Increment available copies
    book->available_copies++;
    
    return 1;
}

void display_book(const Book *book) {
    printf("\nBook ID: %d\n", book->id);
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("ISBN: %s\n", book->isbn);
    printf("Category: %s\n", book->category);
    printf("Publication Year: %d\n", book->publication_year);
    printf("Available Copies: %d/%d\n", book->available_copies, book->total_copies);
    printf("Times Borrowed: %d\n", book->times_borrowed);
}

void display_user(const User *user) {
    printf("\nUser ID: %d\n", user->id);
    printf("Name: %s\n", user->name);
    printf("Email: %s\n", user->email);
    printf("Phone: %s\n", user->phone);
    printf("Address: %s\n", user->address);
    printf("Membership Date: %02d/%02d/%d\n", 
           user->membership_date[0], user->membership_date[1], user->membership_date[2]);
    printf("Books Issued: %d\n", user->books_issued);
    printf("Fine Amount: $%d\n", user->fine_amount);
    
    if (user->books_issued > 0) {
        printf("Issued Books:\n");
        for (int i = 0; i < user->books_issued; i++) {
            printf("  Book ID: %d, Due Date: %s", 
                   user->books_ids[i], ctime(&user->due_dates[i]));
        }
    }
}

void display_all_books(const LibraryDatabase *library) {
    printf("\n=== All Books ===\n");
    printf("Total Books: %d\n\n", library->book_count);
    
    for (int i = 0; i < library->book_count; i++) {
        display_book(&library->books[i]);
        printf("------------------------\n");
    }
}

void display_all_users(const LibraryDatabase *library) {
    printf("\n=== All Users ===\n");
    printf("Total Users: %d\n\n", library->user_count);
    
    for (int i = 0; i < library->user_count; i++) {
        display_user(&library->users[i]);
        printf("------------------------\n");
    }
}

int find_book_by_id(const LibraryDatabase *library, int book_id) {
    for (int i = 0; i < library->book_count; i++) {
        if (library->books[i].id == book_id) {
            return i;
        }
    }
    return -1;
}

int find_user_by_id(const LibraryDatabase *library, int user_id) {
    for (int i = 0; i < library->user_count; i++) {
        if (library->users[i].id == user_id) {
            return i;
        }
    }
    return -1;
}

void search_books_by_title(const LibraryDatabase *library, const char *title) {
    printf("\nSearch Results for '%s':\n", title);
    int found = 0;
    
    for (int i = 0; i < library->book_count; i++) {
        if (strstr(library->books[i].title, title) != NULL) {
            display_book(&library->books[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No books found with title containing '%s'.\n", title);
    }
}

void search_books_by_author(const LibraryDatabase *library, const char *author) {
    printf("\nSearch Results for author '%s':\n", author);
    int found = 0;
    
    for (int i = 0; i < library->book_count; i++) {
        if (strstr(library->books[i].author, author) != NULL) {
            display_book(&library->books[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No books found by author '%s'.\n", author);
    }
}

void search_books_by_category(const LibraryDatabase *library, const char *category) {
    printf("\nSearch Results for category '%s':\n", category);
    int found = 0;
    
    for (int i = 0; i < library->book_count; i++) {
        if (strcmp(library->books[i].category, category) == 0) {
            display_book(&library->books[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No books found in category '%s'.\n", category);
    }
}

void display_most_popular_books(const LibraryDatabase *library) {
    // Create a temporary array to sort books by popularity
    Book temp_books[MAX_BOOKS];
    memcpy(temp_books, library->books, sizeof(Book) * library->book_count);
    
    // Sort books by times_borrowed (descending)
    for (int i = 0; i < library->book_count - 1; i++) {
        for (int j = 0; j < library->book_count - i - 1; j++) {
            if (temp_books[j].times_borrowed < temp_books[j + 1].times_borrowed) {
                Book temp = temp_books[j];
                temp_books[j] = temp_books[j + 1];
                temp_books[j + 1] = temp;
            }
        }
    }
    
    printf("\n=== Most Popular Books ===\n");
    int count = library->book_count < 10 ? library->book_count : 10;
    
    for (int i = 0; i < count; i++) {
        printf("%d. %s by %s (Borrowed %d times)\n", 
               i + 1, temp_books[i].title, temp_books[i].author, temp_books[i].times_borrowed);
    }
}

void calculate_fines(LibraryDatabase *library) {
    time_t now = time(NULL);
    
    for (int i = 0; i < library->user_count; i++) {
        User *user = &library->users[i];
        
        for (int j = 0; j < user->books_issued; j++) {
            int days_overdue = calculate_days_overdue(user->due_dates[j]);
            if (days_overdue > 0) {
                int fine = days_overdue * 2; // $2 per day
                user->fine_amount += fine;
                
                // Update due date to avoid double charging
                user->due_dates[j] = now;
                
                printf("User %s fined $%d for overdue book ID %d\n", 
                       user->name, fine, user->books_ids[j]);
            }
        }
    }
}

void save_library_data(const LibraryDatabase *library) {
    FILE *books_file = fopen(FILENAME_BOOKS, "wb");
    if (books_file != NULL) {
        fwrite(&library->book_count, sizeof(int), 1, books_file);
        fwrite(library->books, sizeof(Book), library->book_count, books_file);
        fclose(books_file);
    }
    
    FILE *users_file = fopen(FILENAME_USERS, "wb");
    if (users_file != NULL) {
        fwrite(&library->user_count, sizeof(int), 1, users_file);
        fwrite(library->users, sizeof(User), library->user_count, users_file);
        fclose(users_file);
    }
}

int load_library_data(LibraryDatabase *library) {
    FILE *books_file = fopen(FILENAME_BOOKS, "rb");
    if (books_file != NULL) {
        fread(&library->book_count, sizeof(int), 1, books_file);
        fread(library->books, sizeof(Book), library->book_count, books_file);
        fclose(books_file);
    } else {
        return 0;
    }
    
    FILE *users_file = fopen(FILENAME_USERS, "rb");
    if (users_file != NULL) {
        fread(&library->user_count, sizeof(int), 1, users_file);
        fread(library->users, sizeof(User), library->user_count, users_file);
        fclose(users_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(LibraryDatabase *library) {
    char* titles[] = {
        "The Great Gatsby", "To Kill a Mockingbird", "1984", "Pride and Prejudice",
        "The Catcher in the Rye", "Animal Farm", "Lord of the Flies", "Brave New World",
        "The Hobbit", "Harry Potter and the Sorcerer's Stone", "The Da Vinci Code",
        "The Alchemist", "The Little Prince", "Life of Pi", "The Kite Runner",
        "Gone Girl", "The Girl on the Train", "The Hunger Games", "Divergent", "The Fault in Our Stars"
    };
    
    char* authors[] = {
        "F. Scott Fitzgerald", "Harper Lee", "George Orwell", "Jane Austen",
        "J.D. Salinger", "George Orwell", "William Golding", "Aldous Huxley",
        "J.R.R. Tolkien", "J.K. Rowling", "Dan Brown", "Paulo Coelho",
        "Antoine de Saint-Exupéry", "Yann Martel", "Khaled Hosseini",
        "Gillian Flynn", "Paula Hawkins", "Suzanne Collins", "Veronica Roth", "John Green"
    };
    
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    srand(time(NULL));
    
    // Add 20 books
    for (int i = 0; i < 20 && library->book_count < MAX_BOOKS; i++) {
        char isbn[20];
        sprintf(isbn, "978-%d-%d-%d", 1000 + i, 100 + i, 10 + i);
        
        int category_index = rand() % 20;
        add_book(library, titles[i % 20], authors[i % 20], isbn, 
                categories[category_index], 1950 + (rand() % 70), 3 + (rand() % 8));
    }
    
    // Add 10 users
    for (int i = 0; i < 10 && library->user_count < MAX_USERS; i++) {
        char name[60], email[60], phone[15], address[100];
        sprintf(name, "%s %s", first_names[rand() % 12], last_names[rand() % 12]);
        sprintf(email, "%s.%s@email.com", first_names[rand() % 12], last_names[rand() % 12]);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Main St, City, State", 100 + rand() % 900);
        
        add_user(library, name, email, phone, address);
    }
}

void display_overdue_books(const LibraryDatabase *library) {
    printf("\n=== Overdue Books ===\n");
    int found = 0;
    
    for (int i = 0; i < library->user_count; i++) {
        User *user = &library->users[i];
        
        for (int j = 0; j < user->books_issued; j++) {
            int days_overdue = calculate_days_overdue(user->due_dates[j]);
            if (days_overdue > 0) {
                int book_index = find_book_by_id(library, user->books_ids[j]);
                if (book_index != -1) {
                    printf("User: %s (ID: %d)\n", user->name, user->id);
                    printf("Book: %s (ID: %d)\n", library->books[book_index].title, user->books_ids[j]);
                    printf("Days Overdue: %d\n", days_overdue);
                    printf("Due Date: %s", ctime(&user->due_dates[j]));
                    printf("------------------------\n");
                    found++;
                }
            }
        }
    }
    
    if (found == 0) {
        printf("No overdue books found.\n");
    }
}

void display_user_history(const LibraryDatabase *library, int user_id) {
    int user_index = find_user_by_id(library, user_id);
    if (user_index == -1) {
        printf("User not found.\n");
        return;
    }
    
    User *user = &library->users[user_index];
    printf("\n=== History for User %s ===\n", user->name);
    
    // Display current issued books
    if (user->books_issued > 0) {
        printf("Currently Issued Books:\n");
        for (int i = 0; i < user->books_issued; i++) {
            int book_index = find_book_by_id(library, user->books_ids[i]);
            if (book_index != -1) {
                printf("  %s (ID: %d)\n", library->books[book_index].title, user->books_ids[i]);
                printf("  Issue Date: %s", ctime(&user->issue_dates[i]));
                printf("  Due Date: %s", ctime(&user->due_dates[i]));
                
                int days_overdue = calculate_days_overdue(user->due_dates[i]);
                if (days_overdue > 0) {
                    printf("  Status: Overdue by %d days\n", days_overdue);
                } else {
                    printf("  Status: On Time\n");
                }
            }
        }
    } else {
        printf("No currently issued books.\n");
    }
    
    printf("Total Fine Amount: $%d\n", user->fine_amount);
}

int calculate_days_overdue(time_t due_date) {
    time_t now = time(NULL);
    double seconds_diff = difftime(now, due_date);
    
    if (seconds_diff <= 0) {
        return 0;
    }
    
    return (int)(seconds_diff / (24 * 60 * 60)); // Convert seconds to days
}