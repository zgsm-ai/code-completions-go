#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_ROOMS 100
#define MAX_BOOKINGS 500
#define MAX_GUESTS 300
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define FILENAME_ROOMS "rooms.dat"
#define FILENAME_BOOKINGS "bookings.dat"
#define FILENAME_GUESTS "guests.dat"

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
    SINGLE,
    DOUBLE,
    TWIN,
    SUITE,
    DELUXE,
    PENTHOUSE,
    PRESIDENTIAL
} RoomType;

typedef enum {
    CLEAN,
    OCCUPIED,
    DIRTY,
    MAINTENANCE,
    OUT_OF_SERVICE
} RoomStatus;

typedef struct {
    int id;
    int number;
    int floor;
    RoomType type;
    float price_per_night;
    int capacity;
    char amenities[200];
    RoomStatus status;
    int last_cleaned_day;
    int last_cleaned_month;
    int last_cleaned_year;
} Room;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char phone[15];
    char email[50];
    char address[MAX_ADDRESS_LENGTH];
    Date birth_date;
    char nationality[MAX_NAME_LENGTH];
    char passport_number[20];
    int loyalty_member;
    int loyalty_points;
} Guest;

typedef struct {
    int id;
    int room_id;
    int guest_id;
    Date check_in_date;
    Date check_out_date;
    DateTime check_in_time;
    DateTime check_out_time;
    float total_amount;
    int nights;
    char payment_method[20];
    char special_requests[200];
    int active;
} Booking;

typedef struct {
    Room rooms[MAX_ROOMS];
    int room_count;
    Guest guests[MAX_GUESTS];
    int guest_count;
    Booking bookings[MAX_BOOKINGS];
    int booking_count;
} HotelDatabase;

// Function prototypes
void initialize_database(HotelDatabase *db);
int add_room(HotelDatabase *db, int number, int floor, RoomType type, 
              float price_per_night, int capacity, const char *amenities);
int add_guest(HotelDatabase *db, const char *first_name, const char *last_name,
                const char *phone, const char *email, const char *address,
                Date birth_date, const char *nationality, 
                const char *passport_number, int loyalty_member);
int create_booking(HotelDatabase *db, int room_id, int guest_id, 
                    Date check_in_date, Date check_out_date);
void check_in_booking(HotelDatabase *db, int booking_id);
void check_out_booking(HotelDatabase *db, int booking_id);
void cancel_booking(HotelDatabase *db, int booking_id);
void update_room_status(HotelDatabase *db, int room_id, RoomStatus status);
void display_room(const Room *room);
void display_guest(const Guest *guest);
void display_booking(const Booking *booking, const HotelDatabase *db);
void display_all_rooms(const HotelDatabase *db);
void display_all_guests(const HotelDatabase *db);
void display_all_bookings(const HotelDatabase *db);
void search_rooms_by_type(const HotelDatabase *db, RoomType type);
void search_rooms_by_price_range(const HotelDatabase *db, float min_price, float max_price);
void search_bookings_by_guest_name(const HotelDatabase *db, const char *name);
void generate_occupancy_report(const HotelDatabase *db, Date date);
void generate_revenue_report(const HotelDatabase *db, int month, int year);
int find_room_by_id(const HotelDatabase *db, int room_id);
int find_guest_by_id(const HotelDatabase *db, int guest_id);
int find_booking_by_id(const HotelDatabase *db, int booking_id);
int save_database(const HotelDatabase *db);
int load_database(HotelDatabase *db);
void generate_sample_data(HotelDatabase *db);
void display_hotel_statistics(const HotelDatabase *db);
void update_loyalty_points(HotelDatabase *db, int guest_id, int points);
void mark_room_cleaned(HotelDatabase *db, int room_id);
int is_valid_date(int day, int month, int year);
int is_valid_time(int hour, int minute);
const char* get_room_type_name(RoomType type);
const char* get_room_status_name(RoomStatus status);

int main() {
    HotelDatabase db;
    initialize_database(&db);
    
    printf("Hotel Management System\n");
    printf("1. Add Room\n");
    printf("2. Add Guest\n");
    printf("3. Create Booking\n");
    printf("4. Check In Booking\n");
    printf("5. Check Out Booking\n");
    printf("6. Cancel Booking\n");
    printf("7. Update Room Status\n");
    printf("8. Display All Rooms\n");
    printf("9. Display All Guests\n");
    printf("10. Display All Bookings\n");
    printf("11. Search Rooms by Type\n");
    printf("12. Search Rooms by Price Range\n");
    printf("13. Search Bookings by Guest Name\n");
    printf("14. Generate Occupancy Report\n");
    printf("15. Generate Revenue Report\n");
    printf("16. Save Database\n");
    printf("17. Load Database\n");
    printf("18. Generate Sample Data\n");
    printf("19. Display Hotel Statistics\n");
    printf("20. Update Loyalty Points\n");
    printf("21. Mark Room as Cleaned\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                int number, floor, capacity, type;
                float price;
                char amenities[200];
                
                printf("Enter room number: ");
                scanf("%d", &number);
                printf("Enter floor: ");
                scanf("%d", &floor);
                printf("Enter room type (0-Single, 1-Double, 2-Twin, 3-Suite, 4-Deluxe, 5-Penthouse, 6-Presidential): ");
                scanf("%d", &type);
                printf("Enter price per night: ");
                scanf("%f", &price);
                printf("Enter capacity: ");
                scanf("%d", &capacity);
                printf("Enter amenities: ");
                scanf(" %[^\n]", amenities);
                
                int room_id = add_room(&db, number, floor, (RoomType)type, price, capacity, amenities);
                if (room_id != -1) {
                    printf("Room added with ID: %d\n", room_id);
                } else {
                    printf("Failed to add room. Database is full.\n");
                }
                break;
            }
            case 2: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char phone[15], email[50], address[MAX_ADDRESS_LENGTH];
                char nationality[MAX_NAME_LENGTH], passport_number[20];
                int day, month, year, loyalty_member;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter nationality: ");
                scanf(" %[^\n]", nationality);
                printf("Enter passport number: ");
                scanf("%s", passport_number);
                printf("Is loyalty member (0-No, 1-Yes): ");
                scanf("%d", &loyalty_member);
                
                Date birth_date = {day, month, year};
                
                int guest_id = add_guest(&db, first_name, last_name, phone, email, address,
                                      birth_date, nationality, passport_number, loyalty_member);
                if (guest_id != -1) {
                    printf("Guest added with ID: %d\n", guest_id);
                } else {
                    printf("Failed to add guest. Database is full.\n");
                }
                break;
            }
            case 3: {
                int room_id, guest_id, day, month, year;
                Date check_in_date, check_out_date;
                
                printf("Enter room ID: ");
                scanf("%d", &room_id);
                printf("Enter guest ID: ");
                scanf("%d", &guest_id);
                printf("Enter check-in date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                check_in_date = {day, month, year};
                printf("Enter check-out date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                check_out_date = {day, month, year};
                
                int booking_id = create_booking(&db, room_id, guest_id, check_in_date, check_out_date);
                if (booking_id != -1) {
                    printf("Booking created with ID: %d\n", booking_id);
                } else {
                    printf("Failed to create booking. Check IDs and room availability.\n");
                }
                break;
            }
            case 4: {
                int booking_id;
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                check_in_booking(&db, booking_id);
                break;
            }
            case 5: {
                int booking_id;
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                check_out_booking(&db, booking_id);
                break;
            }
            case 6: {
                int booking_id;
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                cancel_booking(&db, booking_id);
                break;
            }
            case 7: {
                int room_id, status;
                printf("Enter room ID: ");
                scanf("%d", &room_id);
                printf("Enter room status (0-Clean, 1-Occupied, 2-Dirty, 3-Maintenance, 4-Out of Service): ");
                scanf("%d", &status);
                update_room_status(&db, room_id, (RoomStatus)status);
                break;
            }
            case 8: {
                display_all_rooms(&db);
                break;
            }
            case 9: {
                display_all_guests(&db);
                break;
            }
            case 10: {
                display_all_bookings(&db);
                break;
            }
            case 11: {
                int type;
                printf("Enter room type (0-Single, 1-Double, 2-Twin, 3-Suite, 4-Deluxe, 5-Penthouse, 6-Presidential): ");
                scanf("%d", &type);
                search_rooms_by_type(&db, (RoomType)type);
                break;
            }
            case 12: {
                float min_price, max_price;
                printf("Enter minimum price: ");
                scanf("%f", &min_price);
                printf("Enter maximum price: ");
                scanf("%f", &max_price);
                search_rooms_by_price_range(&db, min_price, max_price);
                break;
            }
            case 13: {
                char name[MAX_NAME_LENGTH];
                printf("Enter guest name to search: ");
                scanf(" %[^\n]", name);
                search_bookings_by_guest_name(&db, name);
                break;
            }
            case 14: {
                int day, month, year;
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                Date date = {day, month, year};
                generate_occupancy_report(&db, date);
                break;
            }
            case 15: {
                int month, year;
                printf("Enter month (1-12): ");
                scanf("%d", &month);
                printf("Enter year: ");
                scanf("%d", &year);
                generate_revenue_report(&db, month, year);
                break;
            }
            case 16: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 17: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 18: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 19: {
                display_hotel_statistics(&db);
                break;
            }
            case 20: {
                int guest_id, points;
                printf("Enter guest ID: ");
                scanf("%d", &guest_id);
                printf("Enter points to add: ");
                scanf("%d", &points);
                update_loyalty_points(&db, guest_id, points);
                break;
            }
            case 21: {
                int room_id;
                printf("Enter room ID: ");
                scanf("%d", &room_id);
                mark_room_cleaned(&db, room_id);
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

void initialize_database(HotelDatabase *db) {
    <｜fim▁hole｜>db->room_count = 0;
    db->guest_count = 0;
    db->booking_count = 0;
}

int add_room(HotelDatabase *db, int number, int floor, RoomType type, 
              float price_per_night, int capacity, const char *amenities) {
    if (db->room_count >= MAX_ROOMS) {
        return -1;
    }
    
    Room *room = &db->rooms[db->room_count];
    room->id = db->room_count + 1;
    room->number = number;
    room->floor = floor;
    room->type = type;
    room->price_per_night = price_per_night;
    room->capacity = capacity;
    strncpy(room->amenities, amenities, 199);
    room->amenities[199] = '\0';
    room->status = CLEAN;
    
    // Set last cleaned date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    room->last_cleaned_day = current_time->tm_mday;
    room->last_cleaned_month = current_time->tm_mon + 1;
    room->last_cleaned_year = current_time->tm_year + 1900;
    
    db->room_count++;
    return room->id;
}

int add_guest(HotelDatabase *db, const char *first_name, const char *last_name,
                const char *phone, const char *email, const char *address,
                Date birth_date, const char *nationality, 
                const char *passport_number, int loyalty_member) {
    if (db->guest_count >= MAX_GUESTS) {
        return -1;
    }
    
    Guest *guest = &db->guests[db->guest_count];
    guest->id = db->guest_count + 1;
    strncpy(guest->first_name, first_name, MAX_NAME_LENGTH - 1);
    guest->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(guest->last_name, last_name, MAX_NAME_LENGTH - 1);
    guest->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(guest->phone, phone, 14);
    guest->phone[14] = '\0';
    strncpy(guest->email, email, 49);
    guest->email[49] = '\0';
    strncpy(guest->address, address, MAX_ADDRESS_LENGTH - 1);
    guest->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    guest->birth_date = birth_date;
    strncpy(guest->nationality, nationality, MAX_NAME_LENGTH - 1);
    guest->nationality[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(guest->passport_number, passport_number, 19);
    guest->passport_number[19] = '\0';
    guest->loyalty_member = loyalty_member;
    guest->loyalty_points = loyalty_member ? 100 : 0; // Start with 100 points if member
    
    db->guest_count++;
    return guest->id;
}

int create_booking(HotelDatabase *db, int room_id, int guest_id, 
                    Date check_in_date, Date check_out_date) {
    if (db->booking_count >= MAX_BOOKINGS) {
        return -1;
    }
    
    // Validate room and guest exist
    int room_index = find_room_by_id(db, room_id);
    int guest_index = find_guest_by_id(db, guest_id);
    
    if (room_index == -1 || guest_index == -1) {
        return -1;
    }
    
    // Validate dates
    if (!is_valid_date(check_in_date.day, check_in_date.month, check_in_date.year) ||
        !is_valid_date(check_out_date.day, check_out_date.month, check_out_date.year)) {
        return -1;
    }
    
    // Check if check-out is after check-in
    time_t check_in_seconds = 0;
    time_t check_out_seconds = 0;
    
    struct tm tm_in = {0};
    tm_in.tm_year = check_in_date.year - 1900;
    tm_in.tm_mon = check_in_date.month - 1;
    tm_in.tm_mday = check_in_date.day;
    check_in_seconds = mktime(&tm_in);
    
    struct tm tm_out = {0};
    tm_out.tm_year = check_out_date.year - 1900;
    tm_out.tm_mon = check_out_date.month - 1;
    tm_out.tm_mday = check_out_date.day;
    check_out_seconds = mktime(&tm_out);
    
    if (check_out_seconds <= check_in_seconds) {
        printf("Check-out date must be after check-in date.\n");
        return -1;
    }
    
    // Check if room is available for the specified dates
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        
        if (booking->active && booking->room_id == room_id) {
            // Check date overlap
            time_t booking_check_in = 0;
            time_t booking_check_out = 0;
            
            struct tm tm_booking_in = {0};
            tm_booking_in.tm_year = booking->check_in_date.year - 1900;
            tm_booking_in.tm_mon = booking->check_in_date.month - 1;
            tm_booking_in.tm_mday = booking->check_in_date.day;
            booking_check_in = mktime(&tm_booking_in);
            
            struct tm tm_booking_out = {0};
            tm_booking_out.tm_year = booking->check_out_date.year - 1900;
            tm_booking_out.tm_mon = booking->check_out_date.month - 1;
            tm_booking_out.tm_mday = booking->check_out_date.day;
            booking_check_out = mktime(&tm_booking_out);
            
            // Check for any overlap
            if (!((check_out_seconds <= booking_check_in) || 
                  (check_in_seconds >= booking_check_out))) {
                printf("Room is not available for the specified dates.\n");
                return -1;
            }
        }
    }
    
    // Calculate nights
    double seconds_diff = difftime(check_out_seconds, check_in_seconds);
    int nights = (int)(seconds_diff / (24 * 60 * 60));
    
    Booking *booking = &db->bookings[db->booking_count];
    booking->id = db->booking_count + 1;
    booking->room_id = room_id;
    booking->guest_id = guest_id;
    booking->check_in_date = check_in_date;
    booking->check_out_date = check_out_date;
    booking->nights = nights;
    booking->total_amount = nights * db->rooms[room_index].price_per_night;
    strcpy(booking->payment_method, "Pending");
    strcpy(booking->special_requests, "");
    booking->active = 1;
    
    // Set initial times
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    booking->check_in_time.day = check_in_date.day;
    booking->check_in_time.month = check_in_date.month;
    booking->check_in_time.year = check_in_date.year;
    booking->check_in_time.hour = 12; // Default 12:00 PM
    booking->check_in_time.minute = 0;
    
    booking->check_out_time.day = check_out_date.day;
    booking->check_out_time.month = check_out_date.month;
    booking->check_out_time.year = check_out_date.year;
    booking->check_out_time.hour = 11; // Default 11:00 AM
    booking->check_out_time.minute = 0;
    
    db->booking_count++;
    return booking->id;
}

void check_in_booking(HotelDatabase *db, int booking_id) {
    int booking_index = find_booking_by_id(db, booking_id);
    if (booking_index == -1) {
        printf("Booking not found.\n");
        return;
    }
    
    Booking *booking = &db->bookings[booking_index];
    
    if (!booking->active) {
        printf("Booking is not active.\n");
        return;
    }
    
    if (strlen(booking->payment_method) > 0 && strcmp(booking->payment_method, "Pending") != 0) {
        printf("Booking is already checked in.\n");
        return;
    }
    
    // Update check-in time to now
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    booking->check_in_time.day = current_time->tm_mday;
    booking->check_in_time.month = current_time->tm_mon + 1;
    booking->check_in_time.year = current_time->tm_year + 1900;
    booking->check_in_time.hour = current_time->tm_hour;
    booking->check_in_time.minute = current_time->tm_min;
    
    // Update room status to occupied
    update_room_status(db, booking->room_id, OCCUPIED);
    
    printf("Booking %d checked in successfully.\n", booking_id);
}

void check_out_booking(HotelDatabase *db, int booking_id) {
    int booking_index = find_booking_by_id(db, booking_id);
    if (booking_index == -1) {
        printf("Booking not found.\n");
        return;
    }
    
    Booking *booking = &db->bookings[booking_index];
    
    if (!booking->active) {
        printf("Booking is not active.\n");
        return;
    }
    
    if (strlen(booking->payment_method) == 0 || strcmp(booking->payment_method, "Pending") == 0) {
        printf("Booking is not checked in.\n");
        return;
    }
    
    // Update check-out time to now
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    booking->check_out_time.day = current_time->tm_mday;
    booking->check_out_time.month = current_time->tm_mon + 1;
    booking->check_out_time.year = current_time->tm_year + 1900;
    booking->check_out_time.hour = current_time->tm_hour;
    booking->check_out_time.minute = current_time->tm_min;
    
    // Calculate actual nights and adjust amount if necessary
    time_t check_in_seconds = 0;
    time_t check_out_seconds = 0;
    
    struct tm tm_in = {0};
    tm_in.tm_year = booking->check_in_time.year - 1900;
    tm_in.tm_mon = booking->check_in_time.month - 1;
    tm_in.tm_mday = booking->check_in_time.day;
    tm_in.tm_hour = booking->check_in_time.hour;
    tm_in.tm_min = booking->check_in_time.minute;
    check_in_seconds = mktime(&tm_in);
    
    struct tm tm_out = {0};
    tm_out.tm_year = booking->check_out_time.year - 1900;
    tm_out.tm_mon = booking->check_out_time.month - 1;
    tm_out.tm_mday = booking->check_out_time.day;
    tm_out.tm_hour = booking->check_out_time.hour;
    tm_out.tm_min = booking->check_out_time.minute;
    check_out_seconds = mktime(&tm_out);
    
    double seconds_diff = difftime(check_out_seconds, check_in_seconds);
    int actual_nights = (int)(seconds_diff / (24 * 60 * 60));
    
    if (actual_nights > 0 && actual_nights != booking->nights) {
        booking->total_amount = actual_nights * (booking->total_amount / booking->nights);
        printf("Adjusted total amount from $%.2f to $%.2f due to stay of %d nights.\n",
               booking->nights * (booking->total_amount / booking->nights),
               booking->total_amount, actual_nights);
    }
    
    // Add loyalty points (10 points per night)
    int guest_index = find_guest_by_id(db, booking->guest_id);
    if (guest_index != -1) {
        int old_points = db->guests[guest_index].loyalty_points;
        db->guests[guest_index].loyalty_points += actual_nights * 10;
        
        printf("Loyalty points updated: %d → %d\n", 
               old_points, db->guests[guest_index].loyalty_points);
    }
    
    // Set room status to dirty (needs cleaning)
    update_room_status(db, booking->room_id, DIRTY);
    
    printf("Booking %d checked out successfully.\n", booking_id);
}

void cancel_booking(HotelDatabase *db, int booking_id) {
    int booking_index = find_booking_by_id(db, booking_id);
    if (booking_index == -1) {
        printf("Booking not found.\n");
        return;
    }
    
    Booking *booking = &db->bookings[booking_index];
    
    if (!booking->active) {
        printf("Booking is not active.\n");
        return;
    }
    
    booking->active = 0;
    
    // Update room status to clean (room becomes available)
    update_room_status(db, booking->room_id, CLEAN);
    
    printf("Booking %d cancelled successfully.\n", booking_id);
}

void update_room_status(HotelDatabase *db, int room_id, RoomStatus status) {
    int room_index = find_room_by_id(db, room_id);
    if (room_index == -1) {
        printf("Room not found.\n");
        return;
    }
    
    RoomStatus old_status = db->rooms[room_index].status;
    db->rooms[room_index].status = status;
    
    printf("Room %d status updated from %s to %s\n", 
           room_id, get_room_status_name(old_status), get_room_status_name(status));
}

void display_room(const Room *room) {
    printf("\nRoom ID: %d\n", room->id);
    printf("Number: %d\n", room->number);
    printf("Floor: %d\n", room->floor);
    printf("Type: %s\n", get_room_type_name(room->type));
    printf("Price per Night: $%.2f\n", room->price_per_night);
    printf("Capacity: %d\n", room->capacity);
    printf("Amenities: %s\n", room->amenities);
    printf("Status: %s\n", get_room_status_name(room->status));
    printf("Last Cleaned: %02d/%02d/%d\n", 
           room->last_cleaned_day, room->last_cleaned_month, room->last_cleaned_year);
}

void display_guest(const Guest *guest) {
    printf("\nGuest ID: %d\n", guest->id);
    printf("Name: %s %s\n", guest->first_name, guest->last_name);
    printf("Phone: %s\n", guest->phone);
    printf("Email: %s\n", guest->email);
    printf("Address: %s\n", guest->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           guest->birth_date.day, guest->birth_date.month, guest->birth_date.year);
    printf("Nationality: %s\n", guest->nationality);
    printf("Passport Number: %s\n", guest->passport_number);
    printf("Loyalty Member: %s\n", guest->loyalty_member ? "Yes" : "No");
    printf("Loyalty Points: %d\n", guest->loyalty_points);
}

void display_booking(const Booking *booking, const HotelDatabase *db) {
    printf("\nBooking ID: %d\n", booking->id);
    
    // Display room information
    int room_index = find_room_by_id(db, booking->room_id);
    if (room_index != -1) {
        printf("Room: %d (Floor %d, %s)\n", 
               db->rooms[room_index].number, 
               db->rooms[room_index].floor,
               get_room_type_name(db->rooms[room_index].type));
    }
    
    // Display guest information
    int guest_index = find_guest_by_id(db, booking->guest_id);
    if (guest_index != -1) {
        printf("Guest: %s %s (ID: %d)\n", 
               db->guests[guest_index].first_name,
               db->guests[guest_index].last_name,
               booking->guest_id);
    }
    
    printf("Check-in Date: %02d/%02d/%d\n", 
           booking->check_in_date.day, booking->check_in_date.month, booking->check_in_date.year);
    printf("Check-out Date: %02d/%02d/%d\n", 
           booking->check_out_date.day, booking->check_out_date.month, booking->check_out_date.year);
    
    printf("Nights: %d\n", booking->nights);
    printf("Total Amount: $%.2f\n", booking->total_amount);
    printf("Payment Method: %s\n", booking->payment_method);
    
    if (strlen(booking->special_requests) > 0) {
        printf("Special Requests: %s\n", booking->special_requests);
    }
    
    printf("Status: %s\n", booking->active ? "Active" : "Cancelled");
}

void display_all_rooms(const HotelDatabase *db) {
    printf("\n=== All Rooms ===\n");
    printf("Total Rooms: %d\n\n", db->room_count);
    
    for (int i = 0; i < db->room_count; i++) {
        display_room(&db->rooms[i]);
        printf("------------------------\n");
    }
}

void display_all_guests(const HotelDatabase *db) {
    printf("\n=== All Guests ===\n");
    printf("Total Guests: %d\n\n", db->guest_count);
    
    for (int i = 0; i < db->guest_count; i++) {
        display_guest(&db->guests[i]);
        printf("------------------------\n");
    }
}

void display_all_bookings(const HotelDatabase *db) {
    printf("\n=== All Bookings ===\n");
    printf("Total Bookings: %d\n\n", db->booking_count);
    
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].active) {
            display_booking(&db->bookings[i], db);
            printf("========================\n");
        }
    }
}

void search_rooms_by_type(const HotelDatabase *db, RoomType type) {
    printf("\n=== Rooms of Type: %s ===\n", get_room_type_name(type));
    int found = 0;
    
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].type == type) {
            display_room(&db->rooms[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No rooms found of this type.\n");
    }
}

void search_rooms_by_price_range(const HotelDatabase *db, float min_price, float max_price) {
    printf("\n=== Rooms in Price Range: $%.2f - $%.2f ===\n", min_price, max_price);
    int found = 0;
    
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].price_per_night >= min_price && 
            db->rooms[i].price_per_night <= max_price) {
            display_room(&db->rooms[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No rooms found in this price range.\n");
    }
}

void search_bookings_by_guest_name(const HotelDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].active) {
            int guest_index = find_guest_by_id(db, db->bookings[i].guest_id);
            if (guest_index != -1) {
                Guest *guest = &db->guests[guest_index];
                
                if (strstr(guest->first_name, name) != NULL || 
                    strstr(guest->last_name, name) != NULL) {
                    display_booking(&db->bookings[i], db);
                    printf("========================\n");
                    found++;
                }
            }
        }
    }
    
    if (found == 0) {
        printf("No bookings found for guests with name containing '%s'.\n", name);
    }
}

void generate_occupancy_report(const HotelDatabase *db, Date date) {
    printf("\n=== Occupancy Report for %02d/%02d/%d ===\n", 
           date.day, date.month, date.year);
    
    int total_rooms = db->room_count;
    int occupied_rooms = 0;
    int total_occupied_capacity = 0;
    int total_capacity = 0;
    
    for (int i = 0; i < db->room_count; i++) {
        total_capacity += db->rooms[i].capacity;
        
        // Check if room is occupied on the specified date
        for (int j = 0; j < db->booking_count; j++) {
            Booking *booking = &db->bookings[j];
            
            if (booking->active && booking->room_id == db->rooms[i].id) {
                // Check if date is within booking range
                if ((date.day >= booking->check_in_date.day && date.month >= booking->check_in_date.month && date.year >= booking->check_in_date.year) &&
                    (date.day <= booking->check_out_date.day && date.month <= booking->check_out_date.month && date.year <= booking->check_out_date.year)) {
                    
                    if (db->rooms[i].status == OCCUPIED) {
                        occupied_rooms++;
                        total_occupied_capacity += db->rooms[i].capacity;
                    }
                    break;
                }
            }
        }
    }
    
    float occupancy_rate = total_capacity > 0 ? (float)occupied_rooms / total_rooms * 100 : 0.0f;
    float capacity_utilization = total_capacity > 0 ? (float)total_occupied_capacity / total_capacity * 100 : 0.0f;
    
    printf("Total Rooms: %d\n", total_rooms);
    printf("Occupied Rooms: %d\n", occupied_rooms);
    printf("Occupancy Rate: %.1f%%\n", occupancy_rate);
    printf("Capacity Utilization: %.1f%%\n", capacity_utilization);
    
    // Room status distribution
    int status_counts[5] = {0}; // CLEAN, OCCUPIED, DIRTY, MAINTENANCE, OUT_OF_SERVICE
    
    for (int i = 0; i < db->room_count; i++) {
        status_counts[db->rooms[i].status]++;
    }
    
    printf("\nRoom Status Distribution:\n");
    printf("  Clean: %d (%.1f%%)\n", status_counts[0], 
           total_rooms > 0 ? (float)status_counts[0] / total_rooms * 100 : 0.0f);
    printf("  Occupied: %d (%.1f%%)\n", status_counts[1],
           total_rooms > 0 ? (float)status_counts[1] / total_rooms * 100 : 0.0f);
    printf("  Dirty: %d (%.1f%%)\n", status_counts[2],
           total_rooms > 0 ? (float)status_counts[2] / total_rooms * 100 : 0.0f);
    printf("  Maintenance: %d (%.1f%%)\n", status_counts[3],
           total_rooms > 0 ? (float)status_counts[3] / total_rooms * 100 : 0.0f);
    printf("  Out of Service: %d (%.1f%%)\n", status_counts[4],
           total_rooms > 0 ? (float)status_counts[4] / total_rooms * 100 : 0.0f);
    
    // Room type distribution
    int type_counts[7] = {0}; // SINGLE, DOUBLE, TWIN, SUITE, DELUXE, PENTHOUSE, PRESIDENTIAL
    
    for (int i = 0; i < db->room_count; i++) {
        type_counts[db->rooms[i].type]++;
    }
    
    printf("\nRoom Type Distribution:\n");
    const char* type_names[] = {"Single", "Double", "Twin", "Suite", "Deluxe", "Penthouse", "Presidential"};
    
    for (int i = 0; i < 7; i++) {
        if (type_counts[i] > 0) {
            printf("  %s: %d (%.1f%%)\n", type_names[i], type_counts[i],
                   total_rooms > 0 ? (float)type_counts[i] / total_rooms * 100 : 0.0f);
        }
    }
}

void generate_revenue_report(const HotelDatabase *db, int month, int year) {
    if (month < 1 || month > 12 || year < 2000 || year > 2100) {
        printf("Invalid month or year.\n");
        return;
    }
    
    char month_names[][10] = {"January", "February", "March", "April", "May", "June",
                             "July", "August", "September", "October", "November", "December"};
    
    printf("\n=== Revenue Report for %s %d ===\n", month_names[month - 1], year);
    
    int total_bookings = 0;
    float total_revenue = 0.0f;
    int total_nights = 0;
    
    // Revenue by room type
    float type_revenue[7] = {0.0f}; // SINGLE to PRESIDENTIAL
    int type_nights[7] = {0};
    
    // Revenue by payment method
    int payment_counts[5] = {0}; // We'll track up to 5 payment methods
    float payment_revenue[5] = {0.0f};
    char payment_methods[5][20];
    
    // Process all bookings for the month
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        
        if (booking->active && booking->check_in_date.month == month && 
            booking->check_in_date.year == year) {
            
            total_bookings++;
            total_revenue += booking->total_amount;
            total_nights += booking->nights;
            
            // Revenue by room type
            int room_index = find_room_by_id(db, booking->room_id);
            if (room_index != -1) {
                type_revenue[db->rooms[room_index].type] += booking->total_amount;
                type_nights[db->rooms[room_index].type] += booking->nights;
            }
            
            // Revenue by payment method
            if (strcmp(booking->payment_method, "Pending") != 0) {
                int method_index = -1;
                for (int j = 0; j < 5; j++) {
                    if (strcmp(payment_methods[j], "") == 0) {
                        strcpy(payment_methods[j], booking->payment_method);
                        method_index = j;
                        break;
                    } else if (strcmp(payment_methods[j], booking->payment_method) == 0) {
                        method_index = j;
                        break;
                    }
                }
                
                if (method_index >= 0) {
                    payment_counts[method_index]++;
                    payment_revenue[method_index] += booking->total_amount;
                }
            }
        }
    }
    
    printf("Total Bookings: %d\n", total_bookings);
    printf("Total Nights: %d\n", total_nights);
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("Average Revenue per Booking: $%.2f\n", 
           total_bookings > 0 ? total_revenue / total_bookings : 0.0f);
    printf("Average Revenue per Night: $%.2f\n", 
           total_nights > 0 ? total_revenue / total_nights : 0.0f);
    
    // Revenue by room type
    printf("\nRevenue by Room Type:\n");
    const char* type_names[] = {"Single", "Double", "Twin", "Suite", "Deluxe", "Penthouse", "Presidential"};
    
    printf("%-15s %-15s %-10s %-10s\n", "Type", "Bookings", "Nights", "Revenue");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 7; i++) {
        if (type_nights[i] > 0) {
            float avg_revenue = type_nights[i] > 0 ? type_revenue[i] / type_nights[i] : 0.0f;
            
            printf("%-15s %-15d %-10d $%-9.2f\n", 
                   type_names[i], type_nights[i], type_nights[i], type_revenue[i]);
        }
    }
    
    // Revenue by payment method
    printf("\nRevenue by Payment Method:\n");
    printf("%-20s %-15s %-15s\n", "Payment Method", "Count", "Revenue");
    printf("-------------------------------------\n");
    
    for (int i = 0; i < 5; i++) {
        if (payment_counts[i] > 0) {
            float avg_revenue = payment_counts[i] > 0 ? payment_revenue[i] / payment_counts[i] : 0.0f;
            
            printf("%-20s %-15d $%-14.2f\n", 
                   payment_methods[i], payment_counts[i], payment_revenue[i]);
        }
    }
}

int find_room_by_id(const HotelDatabase *db, int room_id) {
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].id == room_id) {
            return i;
        }
    }
    return -1;
}

int find_guest_by_id(const HotelDatabase *db, int guest_id) {
    for (int i = 0; i < db->guest_count; i++) {
        if (db->guests[i].id == guest_id) {
            return i;
        }
    }
    return -1;
}

int find_booking_by_id(const HotelDatabase *db, int booking_id) {
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].id == booking_id) {
            return i;
        }
    }
    return -1;
}

int save_database(const HotelDatabase *db) {
    FILE *rooms_file = fopen(FILENAME_ROOMS, "wb");
    if (rooms_file != NULL) {
        fwrite(&db->room_count, sizeof(int), 1, rooms_file);
        fwrite(db->rooms, sizeof(Room), db->room_count, rooms_file);
        fclose(rooms_file);
    } else {
        return 0;
    }
    
    FILE *guests_file = fopen(FILENAME_GUESTS, "wb");
    if (guests_file != NULL) {
        fwrite(&db->guest_count, sizeof(int), 1, guests_file);
        fwrite(db->guests, sizeof(Guest), db->guest_count, guests_file);
        fclose(guests_file);
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

int load_database(HotelDatabase *db) {
    FILE *rooms_file = fopen(FILENAME_ROOMS, "rb");
    if (rooms_file != NULL) {
        fread(&db->room_count, sizeof(int), 1, rooms_file);
        fread(db->rooms, sizeof(Room), db->room_count, rooms_file);
        fclose(rooms_file);
    } else {
        return 0;
    }
    
    FILE *guests_file = fopen(FILENAME_GUESTS, "rb");
    if (guests_file != NULL) {
        fread(&db->guest_count, sizeof(int), 1, guests_file);
        fread(db->guests, sizeof(Guest), db->guest_count, guests_file);
        fclose(guests_file);
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

void generate_sample_data(HotelDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    char* nationalities[] = {"American", "British", "Canadian", "Australian", "German",
                           "French", "Spanish", "Italian", "Chinese", "Japanese", "Brazilian"};
    
    char* payment_methods[] = {"Cash", "Credit Card", "Debit Card", "Mobile Payment", "Check"};
    
    char* amenities_combinations[] = {
        "WiFi, TV, Air Conditioning, Mini Bar",
        "WiFi, TV, Air Conditioning, Mini Bar, Safe, Coffee Maker",
        "WiFi, TV, Air Conditioning, Mini Bar, Safe, Coffee Maker, Balcony",
        "WiFi, TV, Air Conditioning, Mini Bar, Safe, Coffee Maker, Balcony, Kitchenette",
        "WiFi, TV, Air Conditioning, Mini Bar, Safe, Coffee Maker, Balcony, Kitchenette, Ocean View",
        "WiFi, TV, Air Conditioning, Mini Bar, Safe, Coffee Maker, Balcony, Kitchenette, Ocean View, Jacuzzi"
    };
    
    srand(time(NULL));
    
    // Add rooms
    for (int i = 0; i < 50 && db->room_count < MAX_ROOMS; i++) {
        int room_number = 100 + i;
        int floor = 1 + (i / 10); // 10 rooms per floor
        RoomType type = (RoomType)(rand() % 7); // SINGLE to PRESIDENTIAL
        float price = 50.0f + (rand() % 250); // $50 to $300
        int capacity;
        
        switch (type) {
            case SINGLE: capacity = 1; break;
            case DOUBLE: capacity = 2; break;
            case TWIN: capacity = 2; break;
            case SUITE: capacity = 4; break;
            case DELUXE: capacity = 3; break;
            case PENTHOUSE: capacity = 6; break;
            case PRESIDENTIAL: capacity = 8; break;
        }
        
        const char* amenities = amenities_combinations[rand() % 5];
        
        add_room(db, room_number, floor, type, price, capacity, amenities);
    }
    
    // Add guests
    for (int i = 0; i < 100 && db->guest_count < MAX_GUESTS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 18]);
        strcpy(last_name, last_names[rand() % 12]);
        
        char phone[15], email[50], address[MAX_ADDRESS_LENGTH];
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(email, "%s.%s@email.com", first_name, last_name);
        sprintf(address, "%d Main St, City, State", 100 + rand() % 900);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1960 + (rand() % 50);
        Date birth_date = {day, month, year};
        
        const char* nationality = nationalities[rand() % 11];
        char passport[20];
        sprintf(passport, "PP%08d%04d", 10000000 + rand() % 90000000, rand() % 10000);
        
        int loyalty_member = (rand() % 2 == 0); // 50% chance
        
        int guest_id = add_guest(db, first_name, last_name, phone, email, address,
                                 birth_date, nationality, passport, loyalty_member);
        
        // Set loyalty points for some members
        if (guest_id != -1 && loyalty_member && rand() % 2 == 0) {
            db->guests[guest_id - 1].loyalty_points = 100 + (rand() % 500);
        }
    }
    
    // Create bookings for the next 30 days
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    
    for (int i = 0; i < 200 && db->booking_count < MAX_BOOKINGS; i++) {
        int room_id = 1 + (rand() % db->room_count);
        int guest_id = 1 + (rand() % db->guest_count);
        
        // Random dates within the next 30 days
        int days_offset = rand() % 30;
        int nights = 1 + (rand() % 14); // 1 to 14 nights
        
        Date check_in_date;
        check_in_date.day = current_time->tm_mday + days_offset;
        check_in_date.month = current_time->tm_mon + 1;
        check_in_date.year = current_time->tm_year + 1900;
        
        // Normalize date if days exceed month days
        if (check_in_date.day > 28) {
            check_in_date.day -= 28;
            check_in_date.month += 1;
            if (check_in_date.month > 12) {
                check_in_date.month -= 12;
                check_in_date.year += 1;
            }
        }
        
        Date check_out_date = check_in_date;
        check_out_date.day += nights;
        
        // Normalize check-out date
        if (check_out_date.day > 28) {
            check_out_date.day -= 28;
            check_out_date.month += 1;
            if (check_out_date.month > 12) {
                check_out_date.month -= 12;
                check_out_date.year += 1;
            }
        }
        
        int booking_id = create_booking(db, room_id, guest_id, check_in_date, check_out_date);
        
        if (booking_id != -1) {
            // Randomly set some bookings as checked in or checked out
            if (rand() % 10 < 3) { // 30% chance
                check_in_booking(db, booking_id);
                
                // Some of those are also checked out
                if (rand() % 10 < 3) { // 30% of checked in
                    check_out_booking(db, booking_id);
                }
            } else if (rand() % 10 < 1) { // 10% chance
                cancel_booking(db, booking_id);
            } else {
                // Set payment method for non-cancelled bookings
                int payment_index = find_booking_by_id(db, booking_id);
                if (payment_index != -1) {
                    const char* payment_method = payment_methods[rand() % 5];
                    strcpy(db->bookings[payment_index].payment_method, payment_method);
                }
            }
        }
    }
}

void display_hotel_statistics(const HotelDatabase *db) {
    printf("\n=== Hotel Statistics ===\n");
    
    printf("Total Rooms: %d\n", db->room_count);
    printf("Total Guests: %d\n", db->guest_count);
    printf("Total Bookings: %d\n", db->booking_count);
    
    // Room statistics
    float total_capacity = 0.0f;
    float avg_price_per_night = 0.0f;
    
    for (int i = 0; i < db->room_count; i++) {
        total_capacity += db->rooms[i].capacity;
        avg_price_per_night += db->rooms[i].price_per_night;
    }
    
    avg_price_per_night /= db->room_count;
    
    printf("\nRoom Statistics:\n");
    printf("Total Capacity: %.0f\n", total_capacity);
    printf("Average Price per Night: $%.2f\n", avg_price_per_night);
    
    // Room type distribution
    int type_counts[7] = {0}; // SINGLE to PRESIDENTIAL
    
    for (int i = 0; i < db->room_count; i++) {
        type_counts[db->rooms[i].type]++;
    }
    
    printf("Room Type Distribution:\n");
    const char* type_names[] = {"Single", "Double", "Twin", "Suite", "Deluxe", "Penthouse", "Presidential"};
    
    for (int i = 0; i < 7; i++) {
        printf("  %s: %d (%.1f%%)\n", type_names[i], type_counts[i],
               db->room_count > 0 ? (float)type_counts[i] / db->room_count * 100 : 0.0f);
    }
    
    // Guest statistics
    int loyalty_members = 0;
    int total_loyalty_points = 0;
    float avg_points = 0.0f;
    
    for (int i = 0; i < db->guest_count; i++) {
        if (db->guests[i].loyalty_member) {
            loyalty_members++;
        }
        total_loyalty_points += db->guests[i].loyalty_points;
    }
    
    avg_points = db->guest_count > 0 ? (float)total_loyalty_points / db->guest_count : 0.0f;
    
    printf("\nGuest Statistics:\n");
    printf("Total Guests: %d\n", db->guest_count);
    printf("Loyalty Members: %d (%.1f%%)\n", loyalty_members,
           db->guest_count > 0 ? (float)loyalty_members / db->guest_count * 100 : 0.0f);
    printf("Average Loyalty Points: %.1f\n", avg_points);
    
    // Booking statistics
    int active_bookings = 0;
    float total_revenue = 0.0f;
    
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].active) {
            active_bookings++;
            total_revenue += db->bookings[i].total_amount;
        }
    }
    
    printf("\nBooking Statistics:\n");
    printf("Active Bookings: %d\n", active_bookings);
    printf("Total Revenue from Active Bookings: $%.2f\n", total_revenue);
    printf("Average Revenue per Active Booking: $%.2f\n", 
           active_bookings > 0 ? total_revenue / active_bookings : 0.0f);
}

void update_loyalty_points(HotelDatabase *db, int guest_id, int points) {
    int guest_index = find_guest_by_id(db, guest_id);
    if (guest_index == -1) {
        printf("Guest not found.\n");
        return;
    }
    
    int old_points = db->guests[guest_index].loyalty_points;
    db->guests[guest_index].loyalty_points += points;
    
    printf("Loyalty points updated for guest %s %s: %d → %d\n", 
           db->guests[guest_index].first_name,
           db->guests[guest_index].last_name,
           old_points, db->guests[guest_index].loyalty_points);
}

void mark_room_cleaned(HotelDatabase *db, int room_id) {
    int room_index = find_room_by_id(db, room_id);
    if (room_index == -1) {
        printf("Room not found.\n");
        return;
    }
    
    Room *room = &db->rooms[room_index];
    room->status = CLEAN;
    
    // Update last cleaned date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    room->last_cleaned_day = current_time->tm_mday;
    room->last_cleaned_month = current_time->tm_mon + 1;
    room->last_cleaned_year = current_time->tm_year + 1900;
    
    printf("Room %d marked as cleaned on %02d/%02d/%d\n", 
           room_id, room->last_cleaned_day, room->last_cleaned_month, room->last_cleaned_year);
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

const char* get_room_type_name(RoomType type) {
    switch (type) {
        case SINGLE: return "Single";
        case DOUBLE: return "Double";
        case TWIN: return "Twin";
        case SUITE: return "Suite";
        case DELUXE: return "Deluxe";
        case PENTHOUSE: return "Penthouse";
        case PRESIDENTIAL: return "Presidential";
        default: return "Unknown";
    }
}

const char* get_room_status_name(RoomStatus status) {
    switch (status) {
        case CLEAN: return "Clean";
        case OCCUPIED: return "Occupied";
        case DIRTY: return "Dirty";
        case MAINTENANCE: return "Maintenance";
        case OUT_OF_SERVICE: return "Out of Service";
        default: return "Unknown";
    }
}