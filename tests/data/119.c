#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_MOVIES 500
#define MAX_SHOWTIMES 2000
#define MAX_BOOKINGS 3000
#define MAX_SCREENINGS 1000
#define MAX_CINEMAS 50
#define MAX_CUSTOMERS 2000
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define FILENAME_MOVIES "movies.dat"
#define FILENAME_SHOWTIMES "showtimes.dat"
#define FILENAME_BOOKINGS "bookings.dat"
#define FILENAME_SCREENINGS "screenings.dat"
#define FILENAME_CINEMAS "cinemas.dat"
#define FILENAME_CUSTOMERS "customers.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
} DateTime;

typedef enum {
    ACTION,
    COMEDY,
    DRAMA,
    HORROR,
    THRILLER,
    SCIFI,
    ROMANCE,
    ANIMATION,
    DOCUMENTARY,
    FANTASY,
    ADVENTURE,
    CRIME,
    MYSTERY,
    FAMILY,
    MUSICAL
} Genre;

typedef struct {
    int id;
    char title[MAX_NAME_LENGTH];
    char director[MAX_NAME_LENGTH];
    Genre genre;
    int release_year;
    int duration_minutes;
    char description[300];
    float rating; // 1.0 to 10.0
    int age_restriction; // 0 for all ages, otherwise minimum age
    char poster_url[100];
    int active;
} Movie;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char phone[15];
    char email[50];
    int total_screens;
    int total_seats;
} Cinema;

typedef struct {
    int id;
    int cinema_id;
    int screen_number;
    char screen_name[MAX_NAME_LENGTH];
    int capacity;
    int is_3d_capable;
    int is_imax;
} Screening;

typedef struct {
    int id;
    int movie_id;
    int screening_id;
    DateTime start_time;
    DateTime end_time;
    float price_regular;
    float price_vip;
    float price_3d;
    int available_seats;
    int total_seats;
    int active;
} Showtime;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char phone[15];
    char email[50];
    Date birth_date;
    char membership_number[20];
    int loyalty_points;
    int active;
} Customer;

typedef struct {
    int id;
    int showtime_id;
    int customer_id;
    Date booking_date;
    int seat_count;
    float total_price;
    char payment_method[20];
    int cancelled;
    int checked_in;
} Booking;

typedef struct {
    Movie movies[MAX_MOVIES];
    int movie_count;
    Cinema cinemas[MAX_CINEMAS];
    int cinema_count;
    Screening screenings[MAX_SCREENINGS];
    int screening_count;
    Showtime showtimes[MAX_SHOWTIMES];
    int showtime_count;
    Customer customers[MAX_CUSTOMERS];
    int customer_count;
    Booking bookings[MAX_BOOKINGS];
    int booking_count;
} TheaterDatabase;

// Function prototypes
void initialize_database(TheaterDatabase *db);
int add_movie(TheaterDatabase *db, const char *title, const char *director,
               Genre genre, int release_year, int duration_minutes,
               const char *description, float rating, int age_restriction,
               const char *poster_url);
int add_cinema(TheaterDatabase *db, const char *name, const char *address,
                const char *phone, const char *email, int total_screens, int total_seats);
int add_screening(TheaterDatabase *db, int cinema_id, int screen_number,
                  const char *screen_name, int capacity, int is_3d_capable, int is_imax);
int add_showtime(TheaterDatabase *db, int movie_id, int screening_id,
                 DateTime start_time, DateTime end_time,
                 float price_regular, float price_vip, float price_3d);
int add_customer(TheaterDatabase *db, const char *first_name, const char *last_name,
                 const char *phone, const char *email, Date birth_date);
int create_booking(TheaterDatabase *db, int showtime_id, int customer_id,
                  int seat_count, const char *payment_method);
void cancel_booking(TheaterDatabase *db, int booking_id);
void check_in_booking(TheaterDatabase *db, int booking_id);
void display_movie(const Movie *movie);
void display_cinema(const Cinema *cinema);
void display_screening(const Screening *screening);
void display_showtime(const Showtime *showtime, const TheaterDatabase *db);
void display_customer(const Customer *customer);
void display_booking(const Booking *booking, const TheaterDatabase *db);
void display_all_movies(const TheaterDatabase *db);
void display_all_cinemas(const TheaterDatabase *db);
void display_all_screenings(const TheaterDatabase *db);
void display_all_showtimes(const TheaterDatabase *db);
void display_all_customers(const TheaterDatabase *db);
void display_all_bookings(const TheaterDatabase *db);
void search_movies_by_genre(const TheaterDatabase *db, Genre genre);
void search_movies_by_title(const TheaterDatabase *db, const char *title);
void search_showtimes_by_movie(const TheaterDatabase *db, int movie_id, Date date);
void search_showtimes_by_cinema(const TheaterDatabase *db, int cinema_id, Date date);
void generate_daily_report(const TheaterDatabase *db, int cinema_id, Date date);
void generate_sales_report(const TheaterDatabase *db, int month, int year);
int find_movie_by_id(const TheaterDatabase *db, int movie_id);
int find_cinema_by_id(const TheaterDatabase *db, int cinema_id);
int find_screening_by_id(const TheaterDatabase *db, int screening_id);
int find_showtime_by_id(const TheaterDatabase *db, int showtime_id);
int find_customer_by_id(const TheaterDatabase *db, int customer_id);
int find_booking_by_id(const TheaterDatabase *db, int booking_id);
int save_database(const TheaterDatabase *db);
int load_database(TheaterDatabase *db);
void generate_sample_data(TheaterDatabase *db);
void display_theater_statistics(const TheaterDatabase *db);
void update_loyalty_points(TheaterDatabase *db, int customer_id, int points);
int is_valid_date(int day, int month, int year);
int is_valid_time(int hour, int minute);
const char* get_genre_name(Genre genre);

int main() {
    TheaterDatabase db;
    initialize_database(&db);
    
    printf("Theater Management System\n");
    printf("1. Add Movie\n");
    printf("2. Add Cinema\n");
    printf("3. Add Screening\n");
    printf("4. Add Showtime\n");
    printf("5. Add Customer\n");
    printf("6. Create Booking\n");
    printf("7. Cancel Booking\n");
    printf("8. Check In Booking\n");
    printf("9. Display All Movies\n");
    printf("10. Display All Cinemas\n");
    printf("11. Display All Screenings\n");
    printf("12. Display All Showtimes\n");
    printf("13. Display All Customers\n");
    printf("14. Display All Bookings\n");
    printf("15. Search Movies by Genre\n");
    printf("16. Search Movies by Title\n");
    printf("17. Search Showtimes by Movie\n");
    printf("18. Search Showtimes by Cinema\n");
    printf("19. Generate Daily Report\n");
    printf("20. Generate Sales Report\n");
    printf("21. Save Database\n");
    printf("22. Load Database\n");
    printf("23. Generate Sample Data\n");
    printf("24. Display Theater Statistics\n");
    printf("25. Update Loyalty Points\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char title[MAX_NAME_LENGTH], director[MAX_NAME_LENGTH];
                char description[300], poster_url[100];
                int genre, release_year, duration_minutes, age_restriction;
                float rating;
                
                printf("Enter movie title: ");
                scanf(" %[^\n]", title);
                printf("Enter director: ");
                scanf(" %[^\n]", director);
                printf("Enter genre (0-15): ");
                scanf("%d", &genre);
                printf("Enter release year: ");
                scanf("%d", &release_year);
                printf("Enter duration (minutes): ");
                scanf("%d", &duration_minutes);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter rating (1.0-10.0): ");
                scanf("%f", &rating);
                printf("Enter age restriction (0 for all ages): ");
                scanf("%d", &age_restriction);
                printf("Enter poster URL: ");
                scanf(" %[^\n]", poster_url);
                
                int movie_id = add_movie(&db, title, director, (Genre)genre, release_year,
                                         duration_minutes, description, rating, 
                                         age_restriction, poster_url);
                if (movie_id != -1) {
                    printf("Movie added with ID: %d\n", movie_id);
                } else {
                    printf("Failed to add movie. Database is full.\n");
                }
                break;
            }
            case 2: {
                char name[MAX_NAME_LENGTH], address[MAX_ADDRESS_LENGTH];
                char phone[15], email[50];
                int total_screens, total_seats;
                
                printf("Enter cinema name: ");
                scanf(" %[^\n]", name);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter total screens: ");
                scanf("%d", &total_screens);
                printf("Enter total seats: ");
                scanf("%d", &total_seats);
                
                int cinema_id = add_cinema(&db, name, address, phone, email, 
                                         total_screens, total_seats);
                if (cinema_id != -1) {
                    printf("Cinema added with ID: %d\n", cinema_id);
                } else {
                    printf("Failed to add cinema. Database is full.\n");
                }
                break;
            }
            case 3: {
                int cinema_id, screen_number;
                char screen_name[MAX_NAME_LENGTH];
                int capacity, is_3d_capable, is_imax;
                
                printf("Enter cinema ID: ");
                scanf("%d", &cinema_id);
                printf("Enter screen number: ");
                scanf("%d", &screen_number);
                printf("Enter screen name: ");
                scanf(" %[^\n]", screen_name);
                printf("Enter capacity: ");
                scanf("%d", &capacity);
                printf("Is 3D capable (0-No, 1-Yes): ");
                scanf("%d", &is_3d_capable);
                printf("Is IMAX (0-No, 1-Yes): ");
                scanf("%d", &is_imax);
                
                int screening_id = add_screening(&db, cinema_id, screen_number, screen_name,
                                              capacity, is_3d_capable, is_imax);
                if (screening_id != -1) {
                    printf("Screening added with ID: %d\n", screening_id);
                } else {
                    printf("Failed to add screening. Check cinema ID.\n");
                }
                break;
            }
            case 4: {
                int movie_id, screening_id;
                DateTime start_time, end_time;
                float price_regular, price_vip, price_3d;
                
                printf("Enter movie ID: ");
                scanf("%d", &movie_id);
                printf("Enter screening ID: ");
                scanf("%d", &screening_id);
                printf("Enter start date and time (DD MM YYYY HH MM): ");
                scanf("%d %d %d %d %d", &start_time.day, &start_time.month,
                      &start_time.year, &start_time.hour, &start_time.minute);
                printf("Enter end date and time (DD MM YYYY HH MM): ");
                scanf("%d %d %d %d %d", &end_time.day, &end_time.month,
                      &end_time.year, &end_time.hour, &end_time.minute);
                printf("Enter regular price: ");
                scanf("%f", &price_regular);
                printf("Enter VIP price: ");
                scanf("%f", &price_vip);
                printf("Enter 3D price: ");
                scanf("%f", &price_3d);
                
                int showtime_id = add_showtime(&db, movie_id, screening_id, start_time,
                                              end_time, price_regular, price_vip, price_3d);
                if (showtime_id != -1) {
                    printf("Showtime added with ID: %d\n", showtime_id);
                } else {
                    printf("Failed to add showtime. Check IDs and availability.\n");
                }
                break;
            }
            case 5: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char phone[15], email[50], membership_number[20];
                int day, month, year;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter membership number (press Enter for none): ");
                getchar();
                fgets(membership_number, 20, stdin);
                membership_number[strcspn(membership_number, "\n")] = '\0';
                
                Date birth_date = {day, month, year};
                
                int customer_id = add_customer(&db, first_name, last_name, phone, email,
                                              birth_date);
                if (customer_id != -1) {
                    if (strlen(membership_number) > 0) {
                        strcpy(db.customers[customer_id - 1].membership_number, membership_number);
                        db.customers[customer_id - 1].loyalty_points = 100; // Starting points
                    }
                    printf("Customer added with ID: %d\n", customer_id);
                } else {
                    printf("Failed to add customer. Database is full.\n");
                }
                break;
            }
            case 6: {
                int showtime_id, customer_id, seat_count;
                char payment_method[20];
                
                printf("Enter showtime ID: ");
                scanf("%d", &showtime_id);
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter number of seats: ");
                scanf("%d", &seat_count);
                printf("Enter payment method: ");
                scanf(" %[^\n]", payment_method);
                
                int booking_id = create_booking(&db, showtime_id, customer_id, seat_count, payment_method);
                if (booking_id != -1) {
                    printf("Booking created with ID: %d\n", booking_id);
                } else {
                    printf("Failed to create booking. Check IDs and seat availability.\n");
                }
                break;
            }
            case 7: {
                int booking_id;
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                cancel_booking(&db, booking_id);
                break;
            }
            case 8: {
                int booking_id;
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                check_in_booking(&db, booking_id);
                break;
            }
            case 9: {
                display_all_movies(&db);
                break;
            }
            case 10: {
                display_all_cinemas(&db);
                break;
            }
            case 11: {
                display_all_screenings(&db);
                break;
            }
            case 12: {
                display_all_showtimes(&db);
                break;
            }
            case 13: {
                display_all_customers(&db);
                break;
            }
            case 14: {
                display_all_bookings(&db);
                break;
            }
            case 15: {
                int genre;
                printf("Enter genre (0-15): ");
                scanf("%d", &genre);
                search_movies_by_genre(&db, (Genre)genre);
                break;
            }
            case 16: {
                char title[MAX_NAME_LENGTH];
                printf("Enter movie title to search: ");
                scanf(" %[^\n]", title);
                search_movies_by_title(&db, title);
                break;
            }
            case 17: {
                int movie_id, day, month, year;
                printf("Enter movie ID: ");
                scanf("%d", &movie_id);
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                Date date = {day, month, year};
                search_showtimes_by_movie(&db, movie_id, date);
                break;
            }
            case 18: {
                int cinema_id, day, month, year;
                printf("Enter cinema ID: ");
                scanf("%d", &cinema_id);
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                Date date = {day, month, year};
                search_showtimes_by_cinema(&db, cinema_id, date);
                break;
            }
            case 19: {
                int cinema_id, day, month, year;
                printf("Enter cinema ID: ");
                scanf("%d", &cinema_id);
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                Date date = {day, month, year};
                generate_daily_report(&db, cinema_id, date);
                break;
            }
            case 20: {
                int month, year;
                printf("Enter month (1-12): ");
                scanf("%d", &month);
                printf("Enter year: ");
                scanf("%d", &year);
                generate_sales_report(&db, month, year);
                break;
            }
            case 21: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 22: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 23: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 24: {
                display_theater_statistics(&db);
                break;
            }
            case 25: {
                int customer_id, points;
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter points to add: ");
                scanf("%d", &points);
                update_loyalty_points(&db, customer_id, points);
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

void initialize_database(TheaterDatabase *db) {
    <｜fim▁hole｜>db->movie_count = 0;
    db->cinema_count = 0;
    db->screening_count = 0;
    db->showtime_count = 0;
    db->customer_count = 0;
    db->booking_count = 0;
}

int add_movie(TheaterDatabase *db, const char *title, const char *director,
               Genre genre, int release_year, int duration_minutes,
               const char *description, float rating, int age_restriction,
               const char *poster_url) {
    if (db->movie_count >= MAX_MOVIES) {
        return -1;
    }
    
    Movie *movie = &db->movies[db->movie_count];
    movie->id = db->movie_count + 1;
    strncpy(movie->title, title, MAX_NAME_LENGTH - 1);
    movie->title[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(movie->director, director, MAX_NAME_LENGTH - 1);
    movie->director[MAX_NAME_LENGTH - 1] = '\0';
    movie->genre = genre;
    movie->release_year = release_year;
    movie->duration_minutes = duration_minutes;
    strncpy(movie->description, description, 299);
    movie->description[299] = '\0';
    movie->rating = rating;
    movie->age_restriction = age_restriction;
    strncpy(movie->poster_url, poster_url, 99);
    movie->poster_url[99] = '\0';
    movie->active = 1;
    
    db->movie_count++;
    return movie->id;
}

int add_cinema(TheaterDatabase *db, const char *name, const char *address,
                const char *phone, const char *email, int total_screens, int total_seats) {
    if (db->cinema_count >= MAX_CINEMAS) {
        return -1;
    }
    
    Cinema *cinema = &db->cinemas[db->cinema_count];
    cinema->id = db->cinema_count + 1;
    strncpy(cinema->name, name, MAX_NAME_LENGTH - 1);
    cinema->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(cinema->address, address, MAX_ADDRESS_LENGTH - 1);
    cinema->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    strncpy(cinema->phone, phone, 14);
    cinema->phone[14] = '\0';
    strncpy(cinema->email, email, 49);
    cinema->email[49] = '\0';
    cinema->total_screens = total_screens;
    cinema->total_seats = total_seats;
    
    db->cinema_count++;
    return cinema->id;
}

int add_screening(TheaterDatabase *db, int cinema_id, int screen_number,
                  const char *screen_name, int capacity, int is_3d_capable, int is_imax) {
    if (db->screening_count >= MAX_SCREENINGS) {
        return -1;
    }
    
    // Validate cinema exists
    if (find_cinema_by_id(db, cinema_id) == -1) {
        return -1;
    }
    
    Screening *screening = &db->screenings[db->screening_count];
    screening->id = db->screening_count + 1;
    screening->cinema_id = cinema_id;
    screening->screen_number = screen_number;
    strncpy(screening->screen_name, screen_name, MAX_NAME_LENGTH - 1);
    screening->screen_name[MAX_NAME_LENGTH - 1] = '\0';
    screening->capacity = capacity;
    screening->is_3d_capable = is_3d_capable;
    screening->is_imax = is_imax;
    
    db->screening_count++;
    return screening->id;
}

int add_showtime(TheaterDatabase *db, int movie_id, int screening_id,
                 DateTime start_time, DateTime end_time,
                 float price_regular, float price_vip, float price_3d) {
    if (db->showtime_count >= MAX_SHOWTIMES) {
        return -1;
    }
    
    // Validate movie and screening exist
    if (find_movie_by_id(db, movie_id) == -1 || 
        find_screening_by_id(db, screening_id) == -1) {
        return -1;
    }
    
    // Validate date and time
    if (!is_valid_date(start_time.day, start_time.month, start_time.year) ||
        !is_valid_time(start_time.hour, start_time.minute) ||
        !is_valid_date(end_time.day, end_time.month, end_time.year) ||
        !is_valid_time(end_time.hour, end_time.minute)) {
        return -1;
    }
    
    Screening *screening = &db->screenings[find_screening_by_id(db, screening_id)];
    
    Showtime *showtime = &db->showtimes[db->showtime_count];
    showtime->id = db->showtime_count + 1;
    showtime->movie_id = movie_id;
    showtime->screening_id = screening_id;
    showtime->start_time = start_time;
    showtime->end_time = end_time;
    showtime->price_regular = price_regular;
    showtime->price_vip = price_vip;
    showtime->price_3d = price_3d;
    showtime->total_seats = screening->capacity;
    showtime->available_seats = screening->capacity;
    showtime->active = 1;
    
    db->showtime_count++;
    return showtime->id;
}

int add_customer(TheaterDatabase *db, const char *first_name, const char *last_name,
                 const char *phone, const char *email, Date birth_date) {
    if (db->customer_count >= MAX_CUSTOMERS) {
        return -1;
    }
    
    Customer *customer = &db->customers[db->customer_count];
    customer->id = db->customer_count + 1;
    strncpy(customer->first_name, first_name, MAX_NAME_LENGTH - 1);
    customer->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->last_name, last_name, MAX_NAME_LENGTH - 1);
    customer->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->phone, phone, 14);
    customer->phone[14] = '\0';
    strncpy(customer->email, email, 49);
    customer->email[49] = '\0';
    customer->birth_date = birth_date;
    strcpy(customer->membership_number, ""); // Will be set separately
    customer->loyalty_points = 0;
    customer->active = 1;
    
    db->customer_count++;
    return customer->id;
}

int create_booking(TheaterDatabase *db, int showtime_id, int customer_id,
                  int seat_count, const char *payment_method) {
    if (db->booking_count >= MAX_BOOKINGS) {
        return -1;
    }
    
    // Validate showtime and customer exist
    int showtime_index = find_showtime_by_id(db, showtime_id);
    int customer_index = find_customer_by_id(db, customer_id);
    
    if (showtime_index == -1 || customer_index == -1) {
        return -1;
    }
    
    Showtime *showtime = &db->showtimes[showtime_index];
    
    // Check if enough seats are available
    if (showtime->available_seats < seat_count) {
        printf("Not enough seats available. Only %d seats available.\n", 
               showtime->available_seats);
        return -1;
    }
    
    // Determine price based on screening type
    float price_per_seat;
    Screening *screening = &db->screenings[find_screening_by_id(db, showtime->screening_id)];
    
    if (screening->is_imax) {
        price_per_seat = showtime->price_vip;
    } else if (screening->is_3d_capable) {
        price_per_seat = showtime->price_3d;
    } else {
        price_per_seat = showtime->price_regular;
    }
    
    Booking *booking = &db->bookings[db->booking_count];
    booking->id = db->booking_count + 1;
    booking->showtime_id = showtime_id;
    booking->customer_id = customer_id;
    
    // Set booking date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    booking->booking_date.day = current_time->tm_mday;
    booking->booking_date.month = current_time->tm_mon + 1;
    booking->booking_date.year = current_time->tm_year + 1900;
    
    booking->seat_count = seat_count;
    booking->total_price = price_per_seat * seat_count;
    strncpy(booking->payment_method, payment_method, 19);
    booking->payment_method[19] = '\0';
    booking->cancelled = 0;
    booking->checked_in = 0;
    
    // Update available seats
    showtime->available_seats -= seat_count;
    
    db->booking_count++;
    return booking->id;
}

void cancel_booking(TheaterDatabase *db, int booking_id) {
    int booking_index = find_booking_by_id(db, booking_id);
    if (booking_index == -1) {
        printf("Booking not found.\n");
        return;
    }
    
    Booking *booking = &db->bookings[booking_index];
    
    if (booking->cancelled) {
        printf("Booking is already cancelled.\n");
        return;
    }
    
    // Update booking status
    booking->cancelled = 1;
    
    // Return seats to availability
    Showtime *showtime = &db->showtimes[find_showtime_by_id(db, booking->showtime_id)];
    showtime->available_seats += booking->seat_count;
    
    printf("Booking %d cancelled successfully.\n", booking_id);
}

void check_in_booking(TheaterDatabase *db, int booking_id) {
    int booking_index = find_booking_by_id(db, booking_id);
    if (booking_index == -1) {
        printf("Booking not found.\n");
        return;
    }
    
    Booking *booking = &db->bookings[booking_index];
    
    if (booking->cancelled) {
        printf("Cannot check in a cancelled booking.\n");
        return;
    }
    
    if (booking->checked_in) {
        printf("Booking is already checked in.\n");
        return;
    }
    
    // Update booking status
    booking->checked_in = 1;
    
    // Add loyalty points (10 points per ticket)
    Customer *customer = &db->customers[find_customer_by_id(db, booking->customer_id)];
    int old_points = customer->loyalty_points;
    customer->loyalty_points += booking->seat_count * 10;
    
    printf("Booking %d checked in successfully.\n", booking_id);
    printf("Loyalty points updated: %d → %d\n", 
           old_points, customer->loyalty_points);
}

void display_movie(const Movie *movie) {
    printf("\nMovie ID: %d\n", movie->id);
    printf("Title: %s\n", movie->title);
    printf("Director: %s\n", movie->director);
    printf("Genre: %s\n", get_genre_name(movie->genre));
    printf("Release Year: %d\n", movie->release_year);
    printf("Duration: %d minutes\n", movie->duration_minutes);
    printf("Rating: %.1f/10\n", movie->rating);
    printf("Age Restriction: %s\n", movie->age_restriction == 0 ? "All ages" : "18+");
    printf("Description: %s\n", movie->description);
    printf("Poster URL: %s\n", movie->poster_url);
    printf("Status: %s\n", movie->active ? "Active" : "Inactive");
}

void display_cinema(const Cinema *cinema) {
    printf("\nCinema ID: %d\n", cinema->id);
    printf("Name: %s\n", cinema->name);
    printf("Address: %s\n", cinema->address);
    printf("Phone: %s\n", cinema->phone);
    printf("Email: %s\n", cinema->email);
    printf("Total Screens: %d\n", cinema->total_screens);
    printf("Total Seats: %d\n", cinema->total_seats);
}

void display_screening(const Screening *screening) {
    printf("\nScreening ID: %d\n", screening->id);
    printf("Screen Number: %d\n", screening->screen_number);
    printf("Screen Name: %s\n", screening->screen_name);
    printf("Capacity: %d seats\n", screening->capacity);
    printf("3D Capable: %s\n", screening->is_3d_capable ? "Yes" : "No");
    printf("IMAX: %s\n", screening->is_imax ? "Yes" : "No");
}

void display_showtime(const Showtime *showtime, const TheaterDatabase *db) {
    printf("\nShowtime ID: %d\n", showtime->id);
    
    // Display movie information
    int movie_index = find_movie_by_id(db, showtime->movie_id);
    if (movie_index != -1) {
        printf("Movie: %s\n", db->movies[movie_index].title);
    }
    
    // Display screening information
    int screening_index = find_screening_by_id(db, showtime->screening_id);
    if (screening_index != -1) {
        printf("Screen: %s\n", db->screenings[screening_index].screen_name);
    }
    
    printf("Start Time: %02d/%02d/%d %02d:%02d\n", 
           showtime->start_time.day, showtime->start_time.month, showtime->start_time.year,
           showtime->start_time.hour, showtime->start_time.minute);
    
    printf("End Time: %02d/%02d/%d %02d:%02d\n", 
           showtime->end_time.day, showtime->end_time.month, showtime->end_time.year,
           showtime->end_time.hour, showtime->end_time.minute);
    
    printf("Prices: Regular $%.2f, VIP $%.2f, 3D $%.2f\n",
           showtime->price_regular, showtime->price_vip, showtime->price_3d);
    
    printf("Available Seats: %d/%d\n", showtime->available_seats, showtime->total_seats);
    printf("Status: %s\n", showtime->active ? "Active" : "Inactive");
}

void display_customer(const Customer *customer) {
    printf("\nCustomer ID: %d\n", customer->id);
    printf("Name: %s %s\n", customer->first_name, customer->last_name);
    printf("Phone: %s\n", customer->phone);
    printf("Email: %s\n", customer->email);
    printf("Birth Date: %02d/%02d/%d\n", 
           customer->birth_date.day, customer->birth_date.month, customer->birth_date.year);
    
    if (strlen(customer->membership_number) > 0) {
        printf("Membership Number: %s\n", customer->membership_number);
        printf("Loyalty Points: %d\n", customer->loyalty_points);
    }
    
    printf("Status: %s\n", customer->active ? "Active" : "Inactive");
}

void display_booking(const Booking *booking, const TheaterDatabase *db) {
    printf("\nBooking ID: %d\n", booking->id);
    
    // Display customer information
    int customer_index = find_customer_by_id(db, booking->customer_id);
    if (customer_index != -1) {
        printf("Customer: %s %s (ID: %d)\n", 
               db->customers[customer_index].first_name,
               db->customers[customer_index].last_name,
               booking->customer_id);
    }
    
    // Display showtime information
    int showtime_index = find_showtime_by_id(db, booking->showtime_id);
    if (showtime_index != -1) {
        Showtime *showtime = &db->showtimes[showtime_index];
        
        // Display movie information
        int movie_index = find_movie_by_id(db, showtime->movie_id);
        if (movie_index != -1) {
            printf("Movie: %s\n", db->movies[movie_index].title);
        }
        
        printf("Showtime: %02d/%02d/%d %02d:%02d\n", 
               showtime->start_time.day, showtime->start_time.month, showtime->start_time.year,
               showtime->start_time.hour, showtime->start_time.minute);
    }
    
    printf("Booking Date: %02d/%02d/%d\n", 
           booking->booking_date.day, booking->booking_date.month, booking->booking_date.year);
    printf("Seat Count: %d\n", booking->seat_count);
    printf("Total Price: $%.2f\n", booking->total_price);
    printf("Payment Method: %s\n", booking->payment_method);
    printf("Status: %s\n", booking->cancelled ? "Cancelled" : 
                              (booking->checked_in ? "Checked In" : "Active"));
}

void display_all_movies(const TheaterDatabase *db) {
    printf("\n=== All Movies ===\n");
    printf("Total Movies: %d\n\n", db->movie_count);
    
    for (int i = 0; i < db->movie_count; i++) {
        if (db->movies[i].active) {
            display_movie(&db->movies[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_cinemas(const TheaterDatabase *db) {
    printf("\n=== All Cinemas ===\n");
    printf("Total Cinemas: %d\n\n", db->cinema_count);
    
    for (int i = 0; i < db->cinema_count; i++) {
        display_cinema(&db->cinemas[i]);
        printf("------------------------\n");
    }
}

void display_all_screenings(const TheaterDatabase *db) {
    printf("\n=== All Screenings ===\n");
    printf("Total Screenings: %d\n\n", db->screening_count);
    
    for (int i = 0; i < db->screening_count; i++) {
        display_screening(&db->screenings[i]);
        printf("------------------------\n");
    }
}

void display_all_showtimes(const TheaterDatabase *db) {
    printf("\n=== All Showtimes ===\n");
    printf("Total Showtimes: %d\n\n", db->showtime_count);
    
    for (int i = 0; i < db->showtime_count; i++) {
        if (db->showtimes[i].active) {
            display_showtime(&db->showtimes[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_customers(const TheaterDatabase *db) {
    printf("\n=== All Customers ===\n");
    printf("Total Customers: %d\n\n", db->customer_count);
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            display_customer(&db->customers[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_bookings(const TheaterDatabase *db) {
    printf("\n=== All Bookings ===\n");
    printf("Total Bookings: %d\n\n", db->booking_count);
    
    for (int i = 0; i < db->booking_count; i++) {
        display_booking(&db->bookings[i], db);
        printf("------------------------\n");
    }
}

void search_movies_by_genre(const TheaterDatabase *db, Genre genre) {
    printf("\nMovies in Genre: %s\n", get_genre_name(genre));
    int found = 0;
    
    for (int i = 0; i < db->movie_count; i++) {
        if (db->movies[i].active && db->movies[i].genre == genre) {
            display_movie(&db->movies[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No movies found in this genre.\n");
    }
}

void search_movies_by_title(const TheaterDatabase *db, const char *title) {
    printf("\nSearch Results for '%s':\n", title);
    int found = 0;
    
    for (int i = 0; i < db->movie_count; i++) {
        if (db->movies[i].active && strstr(db->movies[i].title, title) != NULL) {
            display_movie(&db->movies[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No movies found with title containing '%s'.\n", title);
    }
}

void search_showtimes_by_movie(const TheaterDatabase *db, int movie_id, Date date) {
    int movie_index = find_movie_by_id(db, movie_id);
    if (movie_index == -1) {
        printf("Movie not found.\n");
        return;
    }
    
    printf("\nShowtimes for Movie '%s' on %02d/%02d/%d:\n", 
           db->movies[movie_index].title, date.day, date.month, date.year);
    
    int found = 0;
    
    for (int i = 0; i < db->showtime_count; i++) {
        Showtime *showtime = &db->showtimes[i];
        
        if (showtime->movie_id == movie_id && showtime->active &&
            showtime->start_time.day == date.day &&
            showtime->start_time.month == date.month &&
            showtime->start_time.year == date.year) {
            
            display_showtime(showtime, db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No showtimes found for this movie on the specified date.\n");
    }
}

void search_showtimes_by_cinema(const TheaterDatabase *db, int cinema_id, Date date) {
    int cinema_index = find_cinema_by_id(db, cinema_id);
    if (cinema_index == -1) {
        printf("Cinema not found.\n");
        return;
    }
    
    printf("\nShowtimes at Cinema '%s' on %02d/%02d/%d:\n", 
           db->cinemas[cinema_index].name, date.day, date.month, date.year);
    
    int found = 0;
    
    for (int i = 0; i < db->showtime_count; i++) {
        Showtime *showtime = &db->showtimes[i];
        
        // Find the screening for this showtime
        int screening_index = find_screening_by_id(db, showtime->screening_id);
        if (screening_index == -1) continue;
        
        Screening *screening = &db->screenings[screening_index];
        
        if (screening->cinema_id == cinema_id && showtime->active &&
            showtime->start_time.day == date.day &&
            showtime->start_time.month == date.month &&
            showtime->start_time.year == date.year) {
            
            display_showtime(showtime, db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No showtimes found at this cinema on the specified date.\n");
    }
}

void generate_daily_report(const TheaterDatabase *db, int cinema_id, Date date) {
    int cinema_index = find_cinema_by_id(db, cinema_id);
    if (cinema_index == -1) {
        printf("Cinema not found.\n");
        return;
    }
    
    Cinema *cinema = &db->cinemas[cinema_index];
    
    printf("\n=== Daily Report for %s on %02d/%02d/%d ===\n", 
           cinema->name, date.day, date.month, date.year);
    
    int total_bookings = 0;
    int checked_in_bookings = 0;
    int cancelled_bookings = 0;
    float total_revenue = 0.0f;
    int total_occupancy = 0;
    int total_capacity = 0;
    
    // Count bookings for this cinema and date
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        
        // Find the showtime for this booking
        int showtime_index = find_showtime_by_id(db, booking->showtime_id);
        if (showtime_index == -1) continue;
        
        Showtime *showtime = &db->showtimes[showtime_index];
        
        // Find the screening for this showtime
        int screening_index = find_screening_by_id(db, showtime->screening_id);
        if (screening_index == -1) continue;
        
        Screening *screening = &db->screenings[screening_index];
        
        // Check if this booking is for the specified cinema and date
        if (screening->cinema_id == cinema_id &&
            showtime->start_time.day == date.day &&
            showtime->start_time.month == date.month &&
            showtime->start_time.year == date.year) {
            
            total_bookings++;
            total_occupancy += booking->seat_count;
            total_capacity += showtime->total_seats;
            
            if (!booking->cancelled) {
                total_revenue += booking->total_price;
                
                if (booking->checked_in) {
                    checked_in_bookings++;
                }
            } else {
                cancelled_bookings++;
            }
        }
    }
    
    float occupancy_rate = total_capacity > 0 ? (float)total_occupancy / total_capacity * 100 : 0.0f;
    
    printf("Total Bookings: %d\n", total_bookings);
    printf("Checked In: %d\n", checked_in_bookings);
    printf("Cancelled: %d\n", cancelled_bookings);
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("Occupancy Rate: %.1f%%\n", occupancy_rate);
    
    // Showtime details
    printf("\nShowtime Details:\n");
    printf("%-10s %-20s %-10s %-10s %-10s %-15s\n", 
           "Showtime", "Movie", "Start Time", "Available", "Booked", "Revenue");
    printf("-----------------------------------------------------------------\n");
    
    for (int i = 0; i < db->showtime_count; i++) {
        Showtime *showtime = &db->showtimes[i];
        
        // Find the screening for this showtime
        int screening_index = find_screening_by_id(db, showtime->screening_id);
        if (screening_index == -1) continue;
        
        Screening *screening = &db->screenings[screening_index];
        
        // Check if this showtime is for the specified cinema and date
        if (screening->cinema_id == cinema_id && showtime->active &&
            showtime->start_time.day == date.day &&
            showtime->start_time.month == date.month &&
            showtime->start_time.year == date.year) {
            
            // Find the movie for this showtime
            int movie_index = find_movie_by_id(db, showtime->movie_id);
            if (movie_index == -1) continue;
            
            int booked_seats = showtime->total_seats - showtime->available_seats;
            float showtime_revenue = 0.0f;
            
            // Calculate revenue for this showtime
            for (int j = 0; j < db->booking_count; j++) {
                Booking *booking = &db->bookings[j];
                
                if (booking->showtime_id == showtime->id && !booking->cancelled) {
                    showtime_revenue += booking->total_price;
                }
            }
            
            printf("%-10d %-20s %02d:%02d      %-10d %-10d $%-14.2f\n", 
                   showtime->id, db->movies[movie_index].title,
                   showtime->start_time.hour, showtime->start_time.minute,
                   showtime->available_seats, booked_seats, showtime_revenue);
        }
    }
}

void generate_sales_report(const TheaterDatabase *db, int month, int year) {
    if (month < 1 || month > 12 || year < 2000 || year > 2100) {
        printf("Invalid month or year.\n");
        return;
    }
    
    char month_names[][10] = {"January", "February", "March", "April", "May", "June",
                             "July", "August", "September", "October", "November", "December"};
    
    printf("\n=== Sales Report for %s %d ===\n", month_names[month - 1], year);
    
    int total_bookings = 0;
    int checked_in_bookings = 0;
    int cancelled_bookings = 0;
    float total_revenue = 0.0f;
    
    // Revenue by genre
    int genre_counts[16] = {0}; // ACTION to MUSICAL
    float genre_revenue[16] = {0.0f};
    
    // Revenue by cinema
    int cinema_bookings[MAX_CINEMAS] = {0};
    float cinema_revenue[MAX_CINEMAS] = {0.0f};
    
    // Count bookings and revenue
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        
        if (booking->booking_date.month == month && booking->booking_date.year == year) {
            total_bookings++;
            
            if (!booking->cancelled) {
                total_revenue += booking->total_price;
                
                if (booking->checked_in) {
                    checked_in_bookings++;
                }
                
                // Find showtime for this booking
                int showtime_index = find_showtime_by_id(db, booking->showtime_id);
                if (showtime_index != -1) {
                    Showtime *showtime = &db->showtimes[showtime_index];
                    
                    // Find movie for this showtime
                    int movie_index = find_movie_by_id(db, showtime->movie_id);
                    if (movie_index != -1) {
                        genre_counts[db->movies[movie_index].genre]++;
                        genre_revenue[db->movies[movie_index].genre] += booking->total_price;
                    }
                    
                    // Find screening for this showtime
                    int screening_index = find_screening_by_id(db, showtime->screening_id);
                    if (screening_index != -1) {
                        Screening *screening = &db->screenings[screening_index];
                        
                        // Only count the first cinema (index 0)
                        if (screening->cinema_id > 0 && screening->cinema_id <= db->cinema_count) {
                            cinema_bookings[screening->cinema_id - 1]++;
                            cinema_revenue[screening->cinema_id - 1] += booking->total_price;
                        }
                    }
                }
            } else {
                cancelled_bookings++;
            }
        }
    }
    
    printf("Total Bookings: %d\n", total_bookings);
    printf("Checked In: %d\n", checked_in_bookings);
    printf("Cancelled: %d\n", cancelled_bookings);
    printf("Total Revenue: $%.2f\n", total_revenue);
    
    // Revenue by genre
    printf("\nRevenue by Genre:\n");
    printf("%-15s %-10s %-15s\n", "Genre", "Bookings", "Revenue");
    printf("---------------------------------\n");
    
    for (int i = 0; i < 16; i++) {
        if (genre_counts[i] > 0) {
            printf("%-15s %-10d $%-14.2f\n", 
                   get_genre_name((Genre)i), genre_counts[i], genre_revenue[i]);
        }
    }
    
    // Revenue by cinema
    printf("\nRevenue by Cinema:\n");
    printf("%-20s %-10s %-15s\n", "Cinema", "Bookings", "Revenue");
    printf("-------------------------------------\n");
    
    for (int i = 0; i < db->cinema_count; i++) {
        if (cinema_bookings[i] > 0) {
            printf("%-20s %-10d $%-14.2f\n", 
                   db->cinemas[i].name, cinema_bookings[i], cinema_revenue[i]);
        }
    }
}

int find_movie_by_id(const TheaterDatabase *db, int movie_id) {
    for (int i = 0; i < db->movie_count; i++) {
        if (db->movies[i].id == movie_id && db->movies[i].active) {
            return i;
        }
    }
    return -1;
}

int find_cinema_by_id(const TheaterDatabase *db, int cinema_id) {
    for (int i = 0; i < db->cinema_count; i++) {
        if (db->cinemas[i].id == cinema_id) {
            return i;
        }
    }
    return -1;
}

int find_screening_by_id(const TheaterDatabase *db, int screening_id) {
    for (int i = 0; i < db->screening_count; i++) {
        if (db->screenings[i].id == screening_id) {
            return i;
        }
    }
    return -1;
}

int find_showtime_by_id(const TheaterDatabase *db, int showtime_id) {
    for (int i = 0; i < db->showtime_count; i++) {
        if (db->showtimes[i].id == showtime_id) {
            return i;
        }
    }
    return -1;
}

int find_customer_by_id(const TheaterDatabase *db, int customer_id) {
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].id == customer_id && db->customers[i].active) {
            return i;
        }
    }
    return -1;
}

int find_booking_by_id(const TheaterDatabase *db, int booking_id) {
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].id == booking_id) {
            return i;
        }
    }
    return -1;
}

int save_database(const TheaterDatabase *db) {
    FILE *movies_file = fopen(FILENAME_MOVIES, "wb");
    if (movies_file != NULL) {
        fwrite(&db->movie_count, sizeof(int), 1, movies_file);
        fwrite(db->movies, sizeof(Movie), db->movie_count, movies_file);
        fclose(movies_file);
    } else {
        return 0;
    }
    
    FILE *cinemas_file = fopen(FILENAME_CINEMAS, "wb");
    if (cinemas_file != NULL) {
        fwrite(&db->cinema_count, sizeof(int), 1, cinemas_file);
        fwrite(db->cinemas, sizeof(Cinema), db->cinema_count, cinemas_file);
        fclose(cinemas_file);
    } else {
        return 0;
    }
    
    FILE *screenings_file = fopen(FILENAME_SCREENINGS, "wb");
    if (screenings_file != NULL) {
        fwrite(&db->screening_count, sizeof(int), 1, screenings_file);
        fwrite(db->screenings, sizeof(Screening), db->screening_count, screenings_file);
        fclose(screenings_file);
    } else {
        return 0;
    }
    
    FILE *showtimes_file = fopen(FILENAME_SHOWTIMES, "wb");
    if (showtimes_file != NULL) {
        fwrite(&db->showtime_count, sizeof(int), 1, showtimes_file);
        fwrite(db->showtimes, sizeof(Showtime), db->showtime_count, showtimes_file);
        fclose(showtimes_file);
    } else {
        return 0;
    }
    
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "wb");
    if (customers_file != NULL) {
        fwrite(&db->customer_count, sizeof(int), 1, customers_file);
        fwrite(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *bookings_file = fopen(FILENAME_BOOKINGS, "wb");
    if (bookings_file != NULL) {
        fwrite(&db->booking_count, sizeof(int), 1, bookings_file);
        fwrite(db->bookings, sizeof(Booking), db->booking_count, bookings_file);
        fclose(bookings_file);
        return 1;
    }
    
    return 0;
}

int load_database(TheaterDatabase *db) {
    FILE *movies_file = fopen(FILENAME_MOVIES, "rb");
    if (movies_file != NULL) {
        fread(&db->movie_count, sizeof(int), 1, movies_file);
        fread(db->movies, sizeof(Movie), db->movie_count, movies_file);
        fclose(movies_file);
    } else {
        return 0;
    }
    
    FILE *cinemas_file = fopen(FILENAME_CINEMAS, "rb");
    if (cinemas_file != NULL) {
        fread(&db->cinema_count, sizeof(int), 1, cinemas_file);
        fread(db->cinemas, sizeof(Cinema), db->cinema_count, cinemas_file);
        fclose(cinemas_file);
    } else {
        return 0;
    }
    
    FILE *screenings_file = fopen(FILENAME_SCREENINGS, "rb");
    if (screenings_file != NULL) {
        fread(&db->screening_count, sizeof(int), 1, screenings_file);
        fread(db->screenings, sizeof(Screening), db->screening_count, screenings_file);
        fclose(screenings_file);
    } else {
        return 0;
    }
    
    FILE *showtimes_file = fopen(FILENAME_SHOWTIMES, "rb");
    if (showtimes_file != NULL) {
        fread(&db->showtime_count, sizeof(int), 1, showtimes_file);
        fread(db->showtimes, sizeof(Showtime), db->showtime_count, showtimes_file);
        fclose(showtimes_file);
    } else {
        return 0;
    }
    
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "rb");
    if (customers_file != NULL) {
        fread(&db->customer_count, sizeof(int), 1, customers_file);
        fread(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *bookings_file = fopen(FILENAME_BOOKINGS, "rb");
    if (bookings_file != NULL) {
        fread(&db->booking_count, sizeof(int), 1, bookings_file);
        fread(db->bookings, sizeof(Booking), db->booking_count, bookings_file);
        fclose(bookings_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(TheaterDatabase *db) {
    char* movie_titles[] = {
        "The Matrix", "Inception", "Interstellar", "The Dark Knight", "Pulp Fiction",
        "The Shawshank Redemption", "Fight Club", "Forrest Gump", "The Godfather",
        "Goodfellas", "The Silence of the Lambs", "Saving Private Ryan",
        "Schindler's List", "Titanic", "Avatar", "The Avengers", "Toy Story",
        "Finding Nemo", "The Lion King", "Jurassic Park", "Star Wars",
        "The Lord of the Rings", "Back to the Future", "E.T. the Extra-Terrestrial"
    };
    
    char* directors[] = {
        "The Wachowskis", "Christopher Nolan", "Christopher Nolan", "Christopher Nolan",
        "Quentin Tarantino", "Frank Darabont", "David Fincher", "Robert Zemeckis",
        "Francis Ford Coppola", "Martin Scorsese", "Jonathan Demme", "Steven Spielberg",
        "Steven Spielberg", "James Cameron", "James Cameron", "Joss Whedon",
        "John Lasseter", "Andrew Stanton", "Roger Allers", "Steven Spielberg",
        "George Lucas", "Peter Jackson", "Robert Zemeckis", "Steven Spielberg"
    };
    
    char* cinema_names[] = {
        "Downtown Cinema", "Westside Mall Theater", "Eastside Multiplex",
        "Grand Theater", "Sunset Cinema", "Northgate Plaza", "Central City Cinema",
        "Southtown Theater", "Riverside Mall Cinema", "Hilltop Theater"
    };
    
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    char* payment_methods[] = {"Credit Card", "Cash", "Debit Card", "Mobile Payment", "Gift Card"};
    
    srand(time(NULL));
    
    // Add movies
    for (int i = 0; i < 30 && db->movie_count < MAX_MOVIES; i++) {
        char description[300];
        sprintf(description, "A thrilling %s movie directed by %s", 
                (rand() % 2) ? "action" : "drama", directors[i]);
        
        float rating = 6.0f + (rand() % 41) / 10.0f; // 6.0 to 10.0
        int age_restriction = (rand() % 2 == 0) ? 0 : 18;
        int duration = 90 + (rand() % 120); // 90 to 210 minutes
        
        add_movie(db, movie_titles[i], directors[i], (Genre)(i % 16),
                  1990 + (rand() % 30), duration, description, rating,
                  age_restriction, "http://example.com/poster.jpg");
    }
    
    // Add cinemas
    for (int i = 0; i < 5 && db->cinema_count < MAX_CINEMAS; i++) {
        char address[MAX_ADDRESS_LENGTH], phone[15], email[50];
        sprintf(address, "%d Cinema Street, City, State", 100 + i * 100);
        sprintf(phone, "555-%04d", 1000 + i * 1000);
        sprintf(email, "contact@cinema%d.com", i + 1);
        
        add_cinema(db, cinema_names[i], address, phone, email, 5 + (rand() % 10), 
                   500 + (rand() % 1000));
    }
    
    // Add screenings
    for (int i = 0; i < 20 && db->screening_count < MAX_SCREENINGS; i++) {
        int cinema_id = 1 + (rand() % db->cinema_count);
        char screen_name[20];
        sprintf(screen_name, "Screen %d", i + 1);
        int capacity = 100 + (rand() % 300); // 100 to 400 seats
        int is_3d_capable = (rand() % 2 == 0); // 50% chance
        int is_imax = (rand() % 4 == 0); // 25% chance
        
        add_screening(db, cinema_id, i + 1, screen_name, capacity, is_3d_capable, is_imax);
    }
    
    // Add customers
    for (int i = 0; i < 100 && db->customer_count < MAX_CUSTOMERS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 12]);
        strcpy(last_name, last_names[rand() % 12]);
        
        char phone[15], email[50];
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(email, "%s.%s@email.com", first_name, last_name);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1970 + (rand() % 40);
        Date birth_date = {day, month, year};
        
        int customer_id = add_customer(db, first_name, last_name, phone, email, birth_date);
        
        // Set membership for some customers
        if (customer_id != -1 && rand() % 2 == 0) {
            sprintf(db->customers[customer_id - 1].membership_number, "MEM%06d", 100000 + i);
            db->customers[customer_id - 1].loyalty_points = 50 + (rand() % 500);
        }
    }
    
    // Add showtimes for the next 14 days
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    
    for (int i = 0; i < 300 && db->showtime_count < MAX_SHOWTIMES; i++) {
        int movie_id = 1 + (rand() % db->movie_count);
        int screening_id = 1 + (rand() % db->screening_count);
        
        // Random date within next 14 days
        int days_offset = rand() % 14;
        DateTime start_time;
        start_time.day = current_time->tm_mday + days_offset;
        start_time.month = current_time->tm_mon + 1;
        start_time.year = current_time->tm_year + 1900;
        start_time.hour = 10 + (rand() % 12); // 10 AM to 10 PM
        start_time.minute = (rand() % 4) * 15; // 0, 15, 30, or 45 minutes
        
        // Normalize date if days exceed month days
        if (start_time.day > 28) {
            start_time.day -= 28;
            start_time.month += 1;
            if (start_time.month > 12) {
                start_time.month -= 12;
                start_time.year += 1;
            }
        }
        
        // Calculate end time based on movie duration
        int movie_index = find_movie_by_id(db, movie_id);
        int duration_minutes = db->movies[movie_index].duration_minutes;
        
        DateTime end_time = start_time;
        end_time.minute += duration_minutes;
        
        // Normalize time
        while (end_time.minute >= 60) {
            end_time.minute -= 60;
            end_time.hour += 1;
        }
        
        while (end_time.hour >= 24) {
            end_time.hour -= 24;
            end_time.day += 1;
            
            if (end_time.day > 28) {
                end_time.day -= 28;
                end_time.month += 1;
                if (end_time.month > 12) {
                    end_time.month -= 12;
                    end_time.year += 1;
                }
            }
        }
        
        // Set prices
        float base_price = 8.0f + (rand() % 8); // $8 to $15
        float price_regular = base_price;
        float price_vip = base_price * 1.5f; // 50% more for VIP
        float price_3d = base_price * 1.3f; // 30% more for 3D
        
        add_showtime(db, movie_id, screening_id, start_time, end_time,
                    price_regular, price_vip, price_3d);
    }
    
    // Create bookings
    for (int i = 0; i < 500 && db->booking_count < MAX_BOOKINGS; i++) {
        int showtime_id = 1 + (rand() % db->showtime_count);
        int customer_id = 1 + (rand() % db->customer_count);
        int seat_count = 1 + (rand() % 6); // 1 to 6 seats
        const char *payment_method = payment_methods[rand() % 5];
        
        int booking_id = create_booking(db, showtime_id, customer_id, seat_count, payment_method);
        
        if (booking_id != -1) {
            // Check in or cancel some bookings
            if (rand() % 10 < 7) { // 70% chance
                check_in_booking(db, booking_id);
            } else if (rand() % 10 < 2) { // 20% chance
                cancel_booking(db, booking_id);
            }
        }
    }
}

void display_theater_statistics(const TheaterDatabase *db) {
    printf("\n=== Theater Statistics ===\n");
    
    int active_movies = 0;
    int total_screens = 0;
    int total_capacity = 0;
    int active_customers = 0;
    int total_bookings = 0;
    float total_revenue = 0.0f;
    
    // Count active movies
    for (int i = 0; i < db->movie_count; i++) {
        if (db->movies[i].active) {
            active_movies++;
        }
    }
    
    // Count total screens and capacity
    for (int i = 0; i < db->screening_count; i++) {
        total_screens++;
        total_capacity += db->screenings[i].capacity;
    }
    
    // Count active customers
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            active_customers++;
        }
    }
    
    // Count bookings and revenue
    for (int i = 0; i < db->booking_count; i++) {
        total_bookings++;
        
        if (!db->bookings[i].cancelled) {
            total_revenue += db->bookings[i].total_price;
        }
    }
    
    printf("Total Cinemas: %d\n", db->cinema_count);
    printf("Active Movies: %d\n", active_movies);
    printf("Total Screens: %d\n", total_screens);
    printf("Total Capacity: %d\n", total_capacity);
    printf("Active Customers: %d\n", active_customers);
    printf("Total Bookings: %d\n", total_bookings);
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("Average Revenue per Booking: $%.2f\n", 
           total_bookings > 0 ? total_revenue / total_bookings : 0.0f);
    
    // Genre distribution
    int genre_counts[16] = {0};
    
    for (int i = 0; i < db->movie_count; i++) {
        if (db->movies[i].active) {
            genre_counts[db->movies[i].genre]++;
        }
    }
    
    printf("\nMovie Genre Distribution:\n");
    for (int i = 0; i < 16; i++) {
        if (genre_counts[i] > 0) {
            printf("  %s: %d (%.1f%%)\n", 
                   get_genre_name((Genre)i), genre_counts[i],
                   active_movies > 0 ? (float)genre_counts[i] / active_movies * 100 : 0);
        }
    }
    
    // Top 5 movies by bookings
    int movie_bookings[MAX_MOVIES] = {0};
    
    for (int i = 0; i < db->booking_count; i++) {
        if (!db->bookings[i].cancelled) {
            // Find showtime for this booking
            int showtime_index = find_showtime_by_id(db, db->bookings[i].showtime_id);
            if (showtime_index != -1) {
                int movie_id = db->showtimes[showtime_index].movie_id;
                if (movie_id > 0 && movie_id <= db->movie_count) {
                    movie_bookings[movie_id - 1]++;
                }
            }
        }
    }
    
    // Sort movies by booking count
    int sorted_indices[MAX_MOVIES];
    for (int i = 0; i < db->movie_count; i++) {
        sorted_indices[i] = i;
    }
    
    for (int i = 0; i < db->movie_count - 1; i++) {
        for (int j = 0; j < db->movie_count - i - 1; j++) {
            if (movie_bookings[sorted_indices[j]] < movie_bookings[sorted_indices[j + 1]]) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    printf("\nTop 5 Movies by Bookings:\n");
    printf("%-25s %-10s\n", "Title", "Bookings");
    printf("---------------------------------\n");
    
    int display_count = db->movie_count < 5 ? db->movie_count : 5;
    for (int i = 0; i < display_count; i++) {
        int movie_index = sorted_indices[db->movie_count - i - 1];
        if (movie_bookings[movie_index] > 0) {
            printf("%-25s %-10d\n", 
                   db->movies[movie_index].title, movie_bookings[movie_index]);
        }
    }
    
    // Customer statistics
    int total_loyalty_points = 0;
    int members_with_points = 0;
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            total_loyalty_points += db->customers[i].loyalty_points;
            if (db->customers[i].loyalty_points > 0) {
                members_with_points++;
            }
        }
    }
    
    printf("\nCustomer Statistics:\n");
    printf("Members with Loyalty Points: %d\n", members_with_points);
    printf("Total Loyalty Points: %d\n", total_loyalty_points);
    printf("Average Points per Member: %.1f\n", 
           members_with_points > 0 ? (float)total_loyalty_points / members_with_points : 0);
}

void update_loyalty_points(TheaterDatabase *db, int customer_id, int points) {
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    int old_points = db->customers[customer_index].loyalty_points;
    db->customers[customer_index].loyalty_points += points;
    
    printf("Loyalty points updated for customer %d: %d → %d\n", 
           customer_id, old_points, db->customers[customer_index].loyalty_points);
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Check for leap year
    if (month == 2 && ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0))) {
        days_in_month[1] = 29;
    }
    
    if (day < 1 || day > days_in_month[month - 1]) return 0;
    
    return 1;
}

int is_valid_time(int hour, int minute) {
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    return 1;
}

const char* get_genre_name(Genre genre) {
    switch (genre) {
        case ACTION: return "Action";
        case COMEDY: return "Comedy";
        case DRAMA: return "Drama";
        case HORROR: return "Horror";
        case THRILLER: return "Thriller";
        case SCIFI: return "Sci-Fi";
        case ROMANCE: return "Romance";
        case ANIMATION: return "Animation";
        case DOCUMENTARY: return "Documentary";
        case FANTASY: return "Fantasy";
        case ADVENTURE: return "Adventure";
        case CRIME: return "Crime";
        case MYSTERY: return "Mystery";
        case FAMILY: return "Family";
        case MUSICAL: return "Musical";
        default: return "Unknown";
    }
}