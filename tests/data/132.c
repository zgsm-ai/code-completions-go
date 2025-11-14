#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define MAX_CUSTOMERS 200
#define MAX_ROOMS 500
#define MAX_RESERVATIONS 1000
#define MAX_SERVICES 100
#define MAX_BILL_ITEMS 500
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define FILENAME_CUSTOMERS "hotel_customers.dat"
#define FILENAME_ROOMS "hotel_rooms.dat"
#define FILENAME_RESERVATIONS "hotel_reservations.dat"
#define FILENAME_SERVICES "hotel_services.dat"
#define FILENAME_BILLS "hotel_bills.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int hour;
    int minute;
} Time;

typedef struct {
    Date date;
    Time time;
} DateTime;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    char address[MAX_ADDRESS_LENGTH];
    char id_number[20];
    char nationality[30];
    int loyalty_tier; // 0=Regular, 1=Silver, 2=Gold, 3=Platinum
    int points;
    DateTime created_at;
    int active;
} Customer;

typedef struct {
    int id;
    int room_number;
    int floor;
    char room_type[20]; // "Standard", "Deluxe", "Suite", "Presidential"
    int bed_count; // Single, Double, King, Queen
    int capacity;
    double price_per_night;
    double price_per_hour;
    char amenities[200];
    int smoking_allowed;
    int active;
    char status[20]; // "Available", "Occupied", "Maintenance", "OutOfService"
} Room;

typedef struct {
    int id;
    int customer_id;
    int room_id;
    DateTime check_in;
    DateTime check_out;
    int nights;
    int guests;
    double total_price;
    double deposit;
    char status[20]; // "Booked", "CheckedIn", "CheckedOut", "Cancelled", "NoShow"
    char payment_method[20]; // "CreditCard", "Cash", "BankTransfer", "Check"
    DateTime booking_date;
    char special_requests[200];
    int active;
} Reservation;

typedef struct {
    int id;
    char name[50];
    char description[200];
    double price;
    char category[30]; // "Restaurant", "Spa", "Laundry", "Transport", "Entertainment"
    int active;
} Service;

typedef struct {
    int id;
    int reservation_id;
    int service_id;
    DateTime date_time;
    int quantity;
    double unit_price;
    double total_price;
    char status[20]; // "Ordered", "Delivered", "Paid"
    char notes[100];
} BillItem;

typedef struct {
    Customer customers[MAX_CUSTOMERS];
    int customer_count;
    Room rooms[MAX_ROOMS];
    int room_count;
    Reservation reservations[MAX_RESERVATIONS];
    int reservation_count;
    Service services[MAX_SERVICES];
    int service_count;
    BillItem bill_items[MAX_BILL_ITEMS];
    int bill_item_count;
} HotelDatabase;

// Function prototypes
void initialize_database(HotelDatabase *db);
int add_customer(HotelDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 const char *id_number, const char *nationality);
int add_room(HotelDatabase *db, int room_number, int floor, const char *room_type,
            int bed_count, int capacity, double price_per_night, double price_per_hour,
            const char *amenities, int smoking_allowed);
int create_reservation(HotelDatabase *db, int customer_id, int room_id,
                      DateTime check_in, DateTime check_out, int guests,
                      double deposit, const char *payment_method, const char *special_requests);
int add_service(HotelDatabase *db, const char *name, const char *description,
               double price, const char *category);
int add_bill_item(HotelDatabase *db, int reservation_id, int service_id,
                 DateTime date_time, int quantity, double unit_price, const char *notes);
void check_in(HotelDatabase *db, int reservation_id);
void check_out(HotelDatabase *db, int reservation_id);
void cancel_reservation(HotelDatabase *db, int reservation_id);
void update_room_status(HotelDatabase *db, int room_id, const char *status);
void display_customer(const Customer *customer);
void display_room(const Room *room);
void display_reservation(const Reservation *reservation, const HotelDatabase *db);
void display_service(const Service *service);
void display_bill_item(const BillItem *item, const HotelDatabase *db);
void display_all_customers(const HotelDatabase *db);
void display_all_rooms(const HotelDatabase *db);
void display_all_reservations(const HotelDatabase *db);
void display_all_services(const HotelDatabase *db);
void display_all_bill_items(const HotelDatabase *db);
void search_customers_by_name(const HotelDatabase *db, const char *name);
void search_rooms_by_type(const HotelDatabase *db, const char *room_type);
void search_reservations_by_status(const HotelDatabase *db, const char *status);
void search_available_rooms(HotelDatabase *db, DateTime check_in, DateTime check_out);
void generate_customer_report(const HotelDatabase *db, int customer_id);
void generate_room_report(const HotelDatabase *db, int room_id);
void generate_reservation_report(const HotelDatabase *db, int reservation_id);
void generate_occupancy_report(const HotelDatabase *db, DateTime start_date, DateTime end_date);
void generate_revenue_report(const HotelDatabase *db, DateTime start_date, DateTime end_date);
void generate_service_report(const HotelDatabase *db, DateTime start_date, DateTime end_date);
int calculate_reservation_total(const HotelDatabase *db, int reservation_id);
int calculate_bill_total(const HotelDatabase *db, int reservation_id);
void apply_promotion(HotelDatabase *db, const char *room_type, double discount_percent);
void update_loyalty_points(HotelDatabase *db, int customer_id, int points);
int is_valid_email(const char *email);
int is_valid_phone(const char *phone);
int is_valid_date(int day, int month, int year);
int is_valid_time(int hour, int minute);
int is_valid_room_type(const char *room_type);
int is_valid_payment_method(const char *payment_method);
int is_valid_service_category(const char *category);
int find_customer_by_id(const HotelDatabase *db, int customer_id);
int find_room_by_id(const HotelDatabase *db, int room_id);
int find_reservation_by_id(const HotelDatabase *db, int reservation_id);
int find_service_by_id(const HotelDatabase *db, int service_id);
int find_bill_item_by_id(const HotelDatabase *db, int bill_item_id);
int find_room_by_number(const HotelDatabase *db, int room_number);
int save_database(const HotelDatabase *db);
int load_database(HotelDatabase *db);
void generate_sample_data(HotelDatabase *db);
const char* get_reservation_status_description(const char *status);
const char* get_room_status_description(const char *status);
int is_room_available(const HotelDatabase *db, int room_id, DateTime check_in, DateTime check_out);
double calculate_total_bill(HotelDatabase *db, int reservation_id);
void process_payment(HotelDatabase *db, int reservation_id);

int main() {
    HotelDatabase db;
    initialize_database(&db);
    
    printf("Hotel Management System\n");
    printf("1. Add Customer\n");
    printf("2. Add Room\n");
    printf("3. Create Reservation\n");
    printf("4. Add Service\n");
    printf("5. Add Bill Item\n");
    printf("6. Check In\n");
    printf("7. Check Out\n");
    printf("8. Cancel Reservation\n");
    printf("9. Update Room Status\n");
    printf("10. Display All Customers\n");
    printf("11. Display All Rooms\n");
    printf("12. Display All Reservations\n");
    printf("13. Display All Services\n");
    printf("14. Display All Bill Items\n");
    printf("15. Search Customers by Name\n");
    printf("16. Search Rooms by Type\n");
    printf("17. Search Reservations by Status\n");
    printf("18. Search Available Rooms\n");
    printf("19. Generate Customer Report\n");
    printf("20. Generate Room Report\n");
    printf("21. Generate Reservation Report\n");
    printf("22. Generate Occupancy Report\n");
    printf("23. Generate Revenue Report\n");
    printf("24. Generate Service Report\n");
    printf("25. Apply Promotion\n");
    printf("26. Update Loyalty Points\n");
    printf("27. Save Database\n");
    printf("28. Load Database\n");
    printf("29. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[MAX_ADDRESS_LENGTH];
                char id_number[20], nationality[30];
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter ID number: ");
                scanf("%s", id_number);
                printf("Enter nationality: ");
                scanf(" %[^\n]", nationality);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                int customer_id = add_customer(&db, first_name, last_name, email, phone,
                                              address, id_number, nationality);
                if (customer_id != -1) {
                    printf("Customer added with ID: %d\n", customer_id);
                } else {
                    printf("Failed to add customer. Database is full.\n");
                }
                break;
            }
            case 2: {
                int room_number, floor, bed_count, capacity, smoking_allowed;
                char room_type[20], amenities[200];
                double price_per_night, price_per_hour;
                
                printf("Enter room number: ");
                scanf("%d", &room_number);
                printf("Enter floor: ");
                scanf("%d", &floor);
                printf("Enter room type (Standard, Deluxe, Suite, Presidential): ");
                scanf(" %[^\n]", room_type);
                printf("Enter bed count (1=Single, 2=Double, 3=King, 4=Queen): ");
                scanf("%d", &bed_count);
                printf("Enter capacity: ");
                scanf("%d", &capacity);
                printf("Enter price per night: ");
                scanf("%lf", &price_per_night);
                printf("Enter price per hour: ");
                scanf("%lf", &price_per_hour);
                printf("Enter amenities: ");
                scanf(" %[^\n]", amenities);
                printf("Smoking allowed (0=No, 1=Yes): ");
                scanf("%d", &smoking_allowed);
                
                if (!is_valid_room_type(room_type)) {
                    printf("Invalid room type. Must be Standard, Deluxe, Suite, or Presidential.\n");
                    break;
                }
                
                int room_id = add_room(&db, room_number, floor, room_type, bed_count,
                                     capacity, price_per_night, price_per_hour,
                                     amenities, smoking_allowed);
                if (room_id != -1) {
                    printf("Room added with ID: %d\n", room_id);
                } else {
                    printf("Failed to add room. Database is full or room number already exists.\n");
                }
                break;
            }
            case 3: {
                int customer_id, room_id, guests;
                int day, month, year, hour, minute;
                DateTime check_in, check_out;
                double deposit;
                char payment_method[20], special_requests[200];
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter room ID: ");
                scanf("%d", &room_id);
                
                printf("Enter check-in date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter check-in time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid check-in date or time.\n");
                    break;
                }
                
                check_in.date.day = day;
                check_in.date.month = month;
                check_in.date.year = year;
                check_in.time.hour = hour;
                check_in.time.minute = minute;
                
                printf("Enter check-out date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter check-out time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid check-out date or time.\n");
                    break;
                }
                
                check_out.date.day = day;
                check_out.date.month = month;
                check_out.date.year = year;
                check_out.time.hour = hour;
                check_out.time.minute = minute;
                
                printf("Enter number of guests: ");
                scanf("%d", &guests);
                printf("Enter deposit amount: ");
                scanf("%lf", &deposit);
                printf("Enter payment method (CreditCard, Cash, BankTransfer, Check): ");
                scanf(" %[^\n]", payment_method);
                printf("Enter special requests: ");
                scanf(" %[^\n]", special_requests);
                
                if (!is_valid_payment_method(payment_method)) {
                    printf("Invalid payment method.\n");
                    break;
                }
                
                int reservation_id = create_reservation(&db, customer_id, room_id,
                                                      check_in, check_out, guests,
                                                      deposit, payment_method, special_requests);
                if (reservation_id != -1) {
                    printf("Reservation created with ID: %d\n", reservation_id);
                } else {
                    printf("Failed to create reservation. Check customer and room IDs, or room availability.\n");
                }
                break;
            }
            case 4: {
                char name[50], description[200], category[30];
                double price;
                
                printf("Enter service name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter price: ");
                scanf("%lf", &price);
                printf("Enter category (Restaurant, Spa, Laundry, Transport, Entertainment): ");
                scanf(" %[^\n]", category);
                
                if (!is_valid_service_category(category)) {
                    printf("Invalid service category.\n");
                    break;
                }
                
                int service_id = add_service(&db, name, description, price, category);
                if (service_id != -1) {
                    printf("Service added with ID: %d\n", service_id);
                } else {
                    printf("Failed to add service. Database is full.\n");
                }
                break;
            }
            case 5: {
                int reservation_id, service_id, quantity;
                int day, month, year, hour, minute;
                DateTime date_time;
                double unit_price;
                char notes[100];
                
                printf("Enter reservation ID: ");
                scanf("%d", &reservation_id);
                printf("Enter service ID: ");
                scanf("%d", &service_id);
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid date or time.\n");
                    break;
                }
                
                date_time.date.day = day;
                date_time.date.month = month;
                date_time.date.year = year;
                date_time.time.hour = hour;
                date_time.time.minute = minute;
                
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                printf("Enter unit price: ");
                scanf("%lf", &unit_price);
                printf("Enter notes: ");
                scanf(" %[^\n]", notes);
                
                int bill_item_id = add_bill_item(&db, reservation_id, service_id,
                                               date_time, quantity, unit_price, notes);
                if (bill_item_id != -1) {
                    printf("Bill item added with ID: %d\n", bill_item_id);
                } else {
                    printf("Failed to add bill item. Check reservation and service IDs.\n");
                }
                break;
            }
            case 6: {
                int reservation_id;
                printf("Enter reservation ID: ");
                scanf("%d", &reservation_id);
                check_in(&db, reservation_id);
                break;
            }
            case 7: {
                int reservation_id;
                printf("Enter reservation ID: ");
                scanf("%d", &reservation_id);
                check_out(&db, reservation_id);
                break;
            }
            case 8: {
                int reservation_id;
                printf("Enter reservation ID: ");
                scanf("%d", &reservation_id);
                cancel_reservation(&db, reservation_id);
                break;
            }
            case 9: {
                int room_id;
                char status[20];
                printf("Enter room ID: ");
                scanf("%d", &room_id);
                printf("Enter new status (Available, Occupied, Maintenance, OutOfService): ");
                scanf(" %[^\n]", status);
                
                update_room_status(&db, room_id, status);
                break;
            }
            case 10: {
                display_all_customers(&db);
                break;
            }
            case 11: {
                display_all_rooms(&db);
                break;
            }
            case 12: {
                display_all_reservations(&db);
                break;
            }
            case 13: {
                display_all_services(&db);
                break;
            }
            case 14: {
                display_all_bill_items(&db);
                break;
            }
            case 15: {
                char name[MAX_NAME_LENGTH];
                printf("Enter customer name to search: ");
                scanf(" %[^\n]", name);
                search_customers_by_name(&db, name);
                break;
            }
            case 16: {
                char room_type[20];
                printf("Enter room type to search: ");
                scanf(" %[^\n]", room_type);
                search_rooms_by_type(&db, room_type);
                break;
            }
            case 17: {
                char status[20];
                printf("Enter reservation status to search: ");
                scanf(" %[^\n]", status);
                search_reservations_by_status(&db, status);
                break;
            }
            case 18: {
                int day, month, year, hour, minute;
                DateTime check_in, check_out;
                
                printf("Enter check-in date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter check-in time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid check-in date or time.\n");
                    break;
                }
                
                check_in.date.day = day;
                check_in.date.month = month;
                check_in.date.year = year;
                check_in.time.hour = hour;
                check_in.time.minute = minute;
                
                printf("Enter check-out date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter check-out time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid check-out date or time.\n");
                    break;
                }
                
                check_out.date.day = day;
                check_out.date.month = month;
                check_out.date.year = year;
                check_out.time.hour = hour;
                check_out.time.minute = minute;
                
                search_available_rooms(&db, check_in, check_out);
                break;
            }
            case 19: {
                int customer_id;
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                generate_customer_report(&db, customer_id);
                break;
            }
            case 20: {
                int room_id;
                printf("Enter room ID: ");
                scanf("%d", &room_id);
                generate_room_report(&db, room_id);
                break;
            }
            case 21: {
                int reservation_id;
                printf("Enter reservation ID: ");
                scanf("%d", &reservation_id);
                generate_reservation_report(&db, reservation_id);
                break;
            }
            case 22: {
                int day, month, year, hour, minute;
                DateTime start_date, end_date;
                
                printf("Enter start date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter start time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid start date or time.\n");
                    break;
                }
                
                start_date.date.day = day;
                start_date.date.month = month;
                start_date.date.year = year;
                start_date.time.hour = hour;
                start_date.time.minute = minute;
                
                printf("Enter end date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter end time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid end date or time.\n");
                    break;
                }
                
                end_date.date.day = day;
                end_date.date.month = month;
                end_date.date.year = year;
                end_date.time.hour = hour;
                end_date.time.minute = minute;
                
                generate_occupancy_report(&db, start_date, end_date);
                break;
            }
            case 23: {
                int day, month, year, hour, minute;
                DateTime start_date, end_date;
                
                printf("Enter start date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter start time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid start date or time.\n");
                    break;
                }
                
                start_date.date.day = day;
                start_date.date.month = month;
                start_date.date.year = year;
                start_date.time.hour = hour;
                start_date.time.minute = minute;
                
                printf("Enter end date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter end time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid end date or time.\n");
                    break;
                }
                
                end_date.date.day = day;
                end_date.date.month = month;
                end_date.date.year = year;
                end_date.time.hour = hour;
                end_date.time.minute = minute;
                
                generate_revenue_report(&db, start_date, end_date);
                break;
            }
            case 24: {
                int day, month, year, hour, minute;
                DateTime start_date, end_date;
                
                printf("Enter start date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter start time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid start date or time.\n");
                    break;
                }
                
                start_date.date.day = day;
                start_date.date.month = month;
                start_date.date.year = year;
                start_date.time.hour = hour;
                start_date.time.minute = minute;
                
                printf("Enter end date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter end time (HH MM): ");
                scanf("%d %d", &hour, &minute);
                
                if (!is_valid_date(day, month, year) || !is_valid_time(hour, minute)) {
                    printf("Invalid end date or time.\n");
                    break;
                }
                
                end_date.date.day = day;
                end_date.date.month = month;
                end_date.date.year = year;
                end_date.time.hour = hour;
                end_date.time.minute = minute;
                
                generate_service_report(&db, start_date, end_date);
                break;
            }
            case 25: {
                char room_type[20];
                double discount_percent;
                
                printf("Enter room type for promotion: ");
                scanf(" %[^\n]", room_type);
                printf("Enter discount percent (e.g., 15.0 for 15%%): ");
                scanf("%lf", &discount_percent);
                
                apply_promotion(&db, room_type, discount_percent);
                break;
            }
            case 26: {
                int customer_id, points;
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter points to add: ");
                scanf("%d", &points);
                
                update_loyalty_points(&db, customer_id, points);
                break;
            }
            case 27: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 28: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 29: {
                generate_sample_data(&db);
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

void initialize_database(HotelDatabase *db) {
    <｜fim▁hole｜>db->customer_count = 0;
    db->room_count = 0;
    db->reservation_count = 0;
    db->service_count = 0;
    db->bill_item_count = 0;
}

int add_customer(HotelDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 const char *id_number, const char *nationality) {
    if (db->customer_count >= MAX_CUSTOMERS) {
        return -1;
    }
    
    Customer *customer = &db->customers[db->customer_count];
    customer->id = db->customer_count + 1;
    strncpy(customer->first_name, first_name, MAX_NAME_LENGTH - 1);
    customer->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->last_name, last_name, MAX_NAME_LENGTH - 1);
    customer->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->email, email, 49);
    customer->email[49] = '\0';
    strncpy(customer->phone, phone, 14);
    customer->phone[14] = '\0';
    strncpy(customer->address, address, MAX_ADDRESS_LENGTH - 1);
    customer->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    strncpy(customer->id_number, id_number, 19);
    customer->id_number[19] = '\0';
    strncpy(customer->nationality, nationality, 29);
    customer->nationality[29] = '\0';
    
    // Set current date and time
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    customer->created_at.date.day = current_time->tm_mday;
    customer->created_at.date.month = current_time->tm_mon + 1;
    customer->created_at.date.year = current_time->tm_year + 1900;
    customer->created_at.time.hour = current_time->tm_hour;
    customer->created_at.time.minute = current_time->tm_min;
    
    customer->loyalty_tier = 0; // Regular
    customer->points = 0;
    customer->active = 1;
    
    db->customer_count++;
    return customer->id;
}

int add_room(HotelDatabase *db, int room_number, int floor, const char *room_type,
            int bed_count, int capacity, double price_per_night, double price_per_hour,
            const char *amenities, int smoking_allowed) {
    if (db->room_count >= MAX_ROOMS) {
        return -1;
    }
    
    // Check if room number already exists
    if (find_room_by_number(db, room_number) != -1) {
        return -1;
    }
    
    Room *room = &db->rooms[db->room_count];
    room->id = db->room_count + 1;
    room->room_number = room_number;
    room->floor = floor;
    strncpy(room->room_type, room_type, 19);
    room->room_type[19] = '\0';
    room->bed_count = bed_count;
    room->capacity = capacity;
    room->price_per_night = price_per_night;
    room->price_per_hour = price_per_hour;
    strncpy(room->amenities, amenities, 199);
    room->amenities[199] = '\0';
    room->smoking_allowed = smoking_allowed;
    room->active = 1;
    strcpy(room->status, "Available");
    
    db->room_count++;
    return room->id;
}

int create_reservation(HotelDatabase *db, int customer_id, int room_id,
                      DateTime check_in, DateTime check_out, int guests,
                      double deposit, const char *payment_method, const char *special_requests) {
    if (db->reservation_count >= MAX_RESERVATIONS) {
        return -1;
    }
    
    // Validate customer and room exist
    if (find_customer_by_id(db, customer_id) == -1 || find_room_by_id(db, room_id) == -1) {
        return -1;
    }
    
    // Check if room is available for the specified dates
    if (!is_room_available(db, room_id, check_in, check_out)) {
        return -1;
    }
    
    // Calculate nights
    int check_in_timestamp = check_in.date.year * 10000 + check_in.date.month * 100 + check_in.date.day;
    int check_out_timestamp = check_out.date.year * 10000 + check_out.date.month * 100 + check_out.date.day;
    
    if (check_out_timestamp <= check_in_timestamp) {
        return -1; // Check-out must be after check-in
    }
    
    Reservation *reservation = &db->reservations[db->reservation_count];
    reservation->id = db->reservation_count + 1;
    reservation->customer_id = customer_id;
    reservation->room_id = room_id;
    reservation->check_in = check_in;
    reservation->check_out = check_out;
    reservation->nights = check_out_timestamp - check_in_timestamp;
    reservation->guests = guests;
    reservation->deposit = deposit;
    reservation->status[0] = '\0';
    strcpy(reservation->status, "Booked");
    strcpy(reservation->payment_method, payment_method, 19);
    reservation->payment_method[19] = '\0';
    
    // Set booking date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    reservation->booking_date.date.day = current_time->tm_mday;
    reservation->booking_date.date.month = current_time->tm_mon + 1;
    reservation->booking_date.date.year = current_time->tm_year + 1900;
    reservation->booking_date.time.hour = current_time->tm_hour;
    reservation->booking_date.time.minute = current_time->tm_min;
    
    strncpy(reservation->special_requests, special_requests, 199);
    reservation->special_requests[199] = '\0';
    reservation->active = 1;
    
    // Calculate total price
    Room *room = &db->rooms[find_room_by_id(db, room_id)];
    reservation->total_price = room->price_per_night * reservation->nights;
    
    db->reservation_count++;
    return reservation->id;
}

int add_service(HotelDatabase *db, const char *name, const char *description,
               double price, const char *category) {
    if (db->service_count >= MAX_SERVICES) {
        return -1;
    }
    
    Service *service = &db->services[db->service_count];
    service->id = db->service_count + 1;
    strncpy(service->name, name, 49);
    service->name[49] = '\0';
    strncpy(service->description, description, 199);
    service->description[199] = '\0';
    service->price = price;
    strncpy(service->category, category, 29);
    service->category[29] = '\0';
    service->active = 1;
    
    db->service_count++;
    return service->id;
}

int add_bill_item(HotelDatabase *db, int reservation_id, int service_id,
                 DateTime date_time, int quantity, double unit_price, const char *notes) {
    if (db->bill_item_count >= MAX_BILL_ITEMS) {
        return -1;
    }
    
    // Validate reservation and service exist
    if (find_reservation_by_id(db, reservation_id) == -1 || 
        find_service_by_id(db, service_id) == -1) {
        return -1;
    }
    
    BillItem *item = &db->bill_items[db->bill_item_count];
    item->id = db->bill_item_count + 1;
    item->reservation_id = reservation_id;
    item->service_id = service_id;
    item->date_time = date_time;
    item->quantity = quantity;
    item->unit_price = unit_price;
    item->total_price = unit_price * quantity;
    strcpy(item->status, "Ordered");
    strncpy(item->notes, notes, 99);
    item->notes[99] = '\0';
    
    db->bill_item_count++;
    return item->id;
}

void check_in(HotelDatabase *db, int reservation_id) {
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index == -1) {
        printf("Reservation not found.\n");
        return;
    }
    
    Reservation *reservation = &db->reservations[reservation_index];
    
    if (strcmp(reservation->status, "Booked") != 0) {
        printf("Cannot check in. Reservation status is not 'Booked'.\n");
        return;
    }
    
    // Update reservation status
    strcpy(reservation->status, "CheckedIn");
    
    // Update room status
    int room_index = find_room_by_id(db, reservation->room_id);
    if (room_index != -1) {
        strcpy(db->rooms[room_index].status, "Occupied");
    }
    
    printf("Check-in successful for reservation #%d.\n", reservation_id);
}

void check_out(HotelDatabase *db, int reservation_id) {
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index == -1) {
        printf("Reservation not found.\n");
        return;
    }
    
    Reservation *reservation = &db->reservations[reservation_index];
    
    if (strcmp(reservation->status, "CheckedIn") != 0) {
        printf("Cannot check out. Reservation status is not 'CheckedIn'.\n");
        return;
    }
    
    // Process payment
    process_payment(db, reservation_id);
    
    // Update reservation status
    strcpy(reservation->status, "CheckedOut");
    
    // Update room status
    int room_index = find_room_by_id(db, reservation->room_id);
    if (room_index != -1) {
        strcpy(db->rooms[room_index].status, "Available");
    }
    
    // Add loyalty points
    int customer_index = find_customer_by_id(db, reservation->customer_id);
    if (customer_index != -1) {
        // Add 10 points per night stayed
        update_loyalty_points(db, reservation->customer_id, reservation->nights * 10);
    }
    
    printf("Check-out successful for reservation #%d.\n", reservation_id);
}

void cancel_reservation(HotelDatabase *db, int reservation_id) {
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index == -1) {
        printf("Reservation not found.\n");
        return;
    }
    
    Reservation *reservation = &db->reservations[reservation_index];
    
    if (strcmp(reservation->status, "CheckedIn") == 0) {
        printf("Cannot cancel. Guest has already checked in.\n");
        return;
    }
    
    if (strcmp(reservation->status, "Cancelled") == 0) {
        printf("Reservation is already cancelled.\n");
        return;
    }
    
    strcpy(reservation->status, "Cancelled");
    reservation->active = 0;
    
    printf("Reservation #%d cancelled successfully.\n", reservation_id);
    printf("Refund: $%.2f\n", reservation->deposit);
}

void update_room_status(HotelDatabase *db, int room_id, const char *status) {
    int room_index = find_room_by_id(db, room_id);
    if (room_index == -1) {
        printf("Room not found.\n");
        return;
    }
    
    Room *room = &db->rooms[room_index];
    
    strcpy(room->status, status);
    
    printf("Room #%d status updated to '%s'.\n", room->room_number, status);
    printf("Description: %s\n", get_room_status_description(status));
}

void display_customer(const Customer *customer) {
    printf("\nCustomer ID: %d\n", customer->id);
    printf("Name: %s %s\n", customer->first_name, customer->last_name);
    printf("Email: %s\n", customer->email);
    printf("Phone: %s\n", customer->phone);
    printf("Address: %s\n", customer->address);
    printf("ID Number: %s\n", customer->id_number);
    printf("Nationality: %s\n", customer->nationality);
    printf("Loyalty Tier: ");
    
    switch (customer->loyalty_tier) {
        case 0: printf("Regular\n"); break;
        case 1: printf("Silver\n"); break;
        case 2: printf("Gold\n"); break;
        case 3: printf("Platinum\n"); break;
        default: printf("Unknown\n"); break;
    }
    
    printf("Points: %d\n", customer->points);
    printf("Created: %02d/%02d/%d %02d:%02d\n", 
           customer->created_at.date.day, customer->created_at.date.month, customer->created_at.date.year,
           customer->created_at.time.hour, customer->created_at.time.minute);
    printf("Status: %s\n", customer->active ? "Active" : "Inactive");
}

void display_room(const Room *room) {
    printf("\nRoom ID: %d\n", room->id);
    printf("Room Number: %d\n", room->room_number);
    printf("Floor: %d\n", room->floor);
    printf("Room Type: %s\n", room->room_type);
    printf("Bed Count: %d\n", room->bed_count);
    printf("Capacity: %d\n", room->capacity);
    printf("Price per Night: $%.2f\n", room->price_per_night);
    printf("Price per Hour: $%.2f\n", room->price_per_hour);
    printf("Amenities: %s\n", room->amenities);
    printf("Smoking Allowed: %s\n", room->smoking_allowed ? "Yes" : "No");
    printf("Status: %s\n", room->status);
    printf("Status Description: %s\n", get_room_status_description(room->status));
}

void display_reservation(const Reservation *reservation, const HotelDatabase *db) {
    printf("\nReservation ID: %d\n", reservation->id);
    
    // Display customer information
    int customer_index = find_customer_by_id(db, reservation->customer_id);
    if (customer_index != -1) {
        printf("Customer: %s %s (ID: %d)\n", 
               db->customers[customer_index].first_name,
               db->customers[customer_index].last_name,
               reservation->customer_id);
    }
    
    // Display room information
    int room_index = find_room_by_id(db, reservation->room_id);
    if (room_index != -1) {
        printf("Room: #%d - %s (ID: %d)\n", 
               db->rooms[room_index].room_number,
               db->rooms[room_index].room_type,
               reservation->room_id);
    }
    
    printf("Check-in: %02d/%02d/%d %02d:%02d\n", 
           reservation->check_in.date.day, reservation->check_in.date.month, reservation->check_in.date.year,
           reservation->check_in.time.hour, reservation->check_in.time.minute);
    printf("Check-out: %02d/%02d/%d %02d:%02d\n", 
           reservation->check_out.date.day, reservation->check_out.date.month, reservation->check_out.date.year,
           reservation->check_out.time.hour, reservation->check_out.time.minute);
    printf("Nights: %d\n", reservation->nights);
    printf("Guests: %d\n", reservation->guests);
    printf("Total Price: $%.2f\n", reservation->total_price);
    printf("Deposit: $%.2f\n", reservation->deposit);
    printf("Payment Method: %s\n", reservation->payment_method);
    printf("Booking Date: %02d/%02d/%d %02d:%02d\n", 
           reservation->booking_date.date.day, reservation->booking_date.date.month, reservation->booking_date.date.year,
           reservation->booking_date.time.hour, reservation->booking_date.time.minute);
    printf("Special Requests: %s\n", reservation->special_requests);
    printf("Status: %s\n", reservation->status);
    printf("Status Description: %s\n", get_reservation_status_description(reservation->status));
}

void display_service(const Service *service) {
    printf("\nService ID: %d\n", service->id);
    printf("Name: %s\n", service->name);
    printf("Description: %s\n", service->description);
    printf("Price: $%.2f\n", service->price);
    printf("Category: %s\n", service->category);
    printf("Status: %s\n", service->active ? "Active" : "Inactive");
}

void display_bill_item(const BillItem *item, const HotelDatabase *db) {
    printf("\nBill Item ID: %d\n", item->id);
    
    // Display reservation information
    int reservation_index = find_reservation_by_id(db, item->reservation_id);
    if (reservation_index != -1) {
        printf("Reservation ID: %d\n", item->reservation_id);
    }
    
    // Display service information
    int service_index = find_service_by_id(db, item->service_id);
    if (service_index != -1) {
        printf("Service: %s (ID: %d)\n", 
               db->services[service_index].name,
               item->service_id);
    }
    
    printf("Date/Time: %02d/%02d/%d %02d:%02d\n", 
           item->date_time.date.day, item->date_time.date.month, item->date_time.date.year,
           item->date_time.time.hour, item->date_time.time.minute);
    printf("Quantity: %d\n", item->quantity);
    printf("Unit Price: $%.2f\n", item->unit_price);
    printf("Total Price: $%.2f\n", item->total_price);
    printf("Status: %s\n", item->status);
    printf("Notes: %s\n", item->notes);
}

void display_all_customers(const HotelDatabase *db) {
    printf("\n=== All Customers ===\n");
    printf("Total Customers: %d\n\n", db->customer_count);
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            display_customer(&db->customers[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_rooms(const HotelDatabase *db) {
    printf("\n=== All Rooms ===\n");
    printf("Total Rooms: %d\n\n", db->room_count);
    
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].active) {
            display_room(&db->rooms[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_reservations(const HotelDatabase *db) {
    printf("\n=== All Reservations ===\n");
    printf("Total Reservations: %d\n\n", db->reservation_count);
    
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].active) {
            display_reservation(&db->reservations[i], db);
            printf("========================\n");
        }
    }
}

void display_all_services(const HotelDatabase *db) {
    printf("\n=== All Services ===\n");
    printf("Total Services: %d\n\n", db->service_count);
    
    for (int i = 0; i < db->service_count; i++) {
        if (db->services[i].active) {
            display_service(&db->services[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_bill_items(const HotelDatabase *db) {
    printf("\n=== All Bill Items ===\n");
    printf("Total Bill Items: %d\n\n", db->bill_item_count);
    
    for (int i = 0; i < db->bill_item_count; i++) {
        display_bill_item(&db->bill_items[i], db);
        printf("------------------------\n");
    }
}

void search_customers_by_name(const HotelDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active && 
            (strstr(db->customers[i].first_name, name) != NULL || 
             strstr(db->customers[i].last_name, name) != NULL)) {
            display_customer(&db->customers[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No customers found with name containing '%s'.\n", name);
    }
}

void search_rooms_by_type(const HotelDatabase *db, const char *room_type) {
    printf("\nRooms of Type: %s\n", room_type);
    int found = 0;
    
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].active && strstr(db->rooms[i].room_type, room_type) != NULL) {
            display_room(&db->rooms[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No rooms found of this type.\n");
    }
}

void search_reservations_by_status(const HotelDatabase *db, const char *status) {
    printf("\nReservations with Status: %s\n", status);
    int found = 0;
    
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].active && strcmp(db->reservations[i].status, status) == 0) {
            display_reservation(&db->reservations[i], db);
            printf("========================\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No reservations found with this status.\n");
    }
}

void search_available_rooms(HotelDatabase *db, DateTime check_in, DateTime check_out) {
    printf("\nAvailable Rooms for Check-in: %02d/%02d/%d %02d:%02d to Check-out: %02d/%02d/%d %02d:%02d\n",
           check_in.date.day, check_in.date.month, check_in.date.year,
           check_in.time.hour, check_in.time.minute,
           check_out.date.day, check_out.date.month, check_out.date.year,
           check_out.time.hour, check_out.time.minute);
    
    int found = 0;
    
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].active && strcmp(db->rooms[i].status, "Available") == 0) {
            if (is_room_available(db, db->rooms[i].id, check_in, check_out)) {
                display_room(&db->rooms[i]);
                printf("------------------------\n");
                found++;
            }
        }
    }
    
    if (found == 0) {
        printf("No available rooms found for the specified dates.\n");
    }
}

void generate_customer_report(const HotelDatabase *db, int customer_id) {
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    Customer *customer = &db->customers[customer_index];
    
    printf("\n=== Customer Report: %s %s ===\n", customer->first_name, customer->last_name);
    printf("Customer ID: %d\n", customer->id);
    printf("Email: %s\n", customer->email);
    printf("Phone: %s\n", customer->phone);
    printf("Address: %s\n", customer->address);
    printf("ID Number: %s\n", customer->id_number);
    printf("Nationality: %s\n", customer->nationality);
    
    printf("\nLoyalty Information:\n");
    printf("Tier: ");
    switch (customer->loyalty_tier) {
        case 0: printf("Regular\n"); break;
        case 1: printf("Silver\n"); break;
        case 2: printf("Gold\n"); break;
        case 3: printf("Platinum\n"); break;
        default: printf("Unknown\n"); break;
    }
    printf("Points: %d\n", customer->points);
    
    // Reservation history
    printf("\nReservation History:\n");
    printf("%-5s %-20s %-20s %-10s %-10s %-15s %-10s\n", 
           "ID", "Check-in", "Check-out", "Nights", "Guests", "Status", "Total");
    printf("------------------------------------------------------------\n");
    
    int reservation_count = 0;
    double total_spent = 0.0;
    int total_nights = 0;
    
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].active && db->reservations[i].customer_id == customer_id) {
            printf("%-5d %02d/%02d/%d %02d:%02d %02d/%02d/%d %02d:%02d %-10d %-10d %-15s $%-9.2f\n", 
                   db->reservations[i].id,
                   db->reservations[i].check_in.date.day, db->reservations[i].check_in.date.month, db->reservations[i].check_in.date.year,
                   db->reservations[i].check_in.time.hour, db->reservations[i].check_in.time.minute,
                   db->reservations[i].check_out.date.day, db->reservations[i].check_out.date.month, db->reservations[i].check_out.date.year,
                   db->reservations[i].check_out.time.hour, db->reservations[i].check_out.time.minute,
                   db->reservations[i].nights,
                   db->reservations[i].guests,
                   db->reservations[i].status,
                   db->reservations[i].total_price);
            
            reservation_count++;
            total_spent += db->reservations[i].total_price;
            total_nights += db->reservations[i].nights;
        }
    }
    
    printf("------------------------------------------------------------\n");
    printf("Total Reservations: %d\n", reservation_count);
    
    if (reservation_count > 0) {
        printf("Total Spent: $%.2f\n", total_spent);
        printf("Total Nights: %d\n", total_nights);
        printf("Average Spending per Reservation: $%.2f\n", total_spent / reservation_count);
        printf("Average Length of Stay: %.1f nights\n", (float)total_nights / reservation_count);
        
        // Room type preferences
        printf("\nRoom Type Preferences:\n");
        int standard_count = 0;
        int deluxe_count = 0;
        int suite_count = 0;
        int presidential_count = 0;
        
        for (int i = 0; i < db->reservation_count; i++) {
            if (db->reservations[i].active && db->reservations[i].customer_id == customer_id) {
                int room_index = find_room_by_id(db, db->reservations[i].room_id);
                if (room_index != -1) {
                    if (strcmp(db->rooms[room_index].room_type, "Standard") == 0) {
                        standard_count++;
                    } else if (strcmp(db->rooms[room_index].room_type, "Deluxe") == 0) {
                        deluxe_count++;
                    } else if (strcmp(db->rooms[room_index].room_type, "Suite") == 0) {
                        suite_count++;
                    } else if (strcmp(db->rooms[room_index].room_type, "Presidential") == 0) {
                        presidential_count++;
                    }
                }
            }
        }
        
        printf("Standard: %d (%.1f%%)\n", standard_count, (float)standard_count / reservation_count * 100);
        printf("Deluxe: %d (%.1f%%)\n", deluxe_count, (float)deluxe_count / reservation_count * 100);
        printf("Suite: %d (%.1f%%)\n", suite_count, (float)suite_count / reservation_count * 100);
        printf("Presidential: %d (%.1f%%)\n", presidential_count, (float)presidential_count / reservation_count * 100);
    }
}

void generate_room_report(const HotelDatabase *db, int room_id) {
    int room_index = find_room_by_id(db, room_id);
    if (room_index == -1) {
        printf("Room not found.\n");
        return;
    }
    
    Room *room = &db->rooms[room_index];
    
    printf("\n=== Room Report: #%d ===\n", room->room_number);
    printf("Room ID: %d\n", room->id);
    printf("Floor: %d\n", room->floor);
    printf("Room Type: %s\n", room->room_type);
    printf("Bed Count: %d\n", room->bed_count);
    printf("Capacity: %d\n", room->capacity);
    printf("Price per Night: $%.2f\n", room->price_per_night);
    printf("Price per Hour: $%.2f\n", room->price_per_hour);
    printf("Amenities: %s\n", room->amenities);
    printf("Smoking Allowed: %s\n", room->smoking_allowed ? "Yes" : "No");
    printf("Status: %s\n", room->status);
    printf("Status Description: %s\n", get_room_status_description(room->status));
    
    // Reservation history
    printf("\nReservation History:\n");
    printf("%-5s %-20s %-20s %-10s %-10s %-15s %-10s\n", 
           "ID", "Check-in", "Check-out", "Nights", "Guests", "Status", "Total");
    printf("------------------------------------------------------------\n");
    
    int reservation_count = 0;
    double total_revenue = 0.0;
    int total_nights = 0;
    int total_guests = 0;
    
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].active && db->reservations[i].room_id == room_id) {
            printf("%-5d %02d/%02d/%d %02d:%02d %02d/%02d/%d %02d:%02d %-10d %-10d %-15s $%-9.2f\n", 
                   db->reservations[i].id,
                   db->reservations[i].check_in.date.day, db->reservations[i].check_in.date.month, db->reservations[i].check_in.date.year,
                   db->reservations[i].check_in.time.hour, db->reservations[i].check_in.time.minute,
                   db->reservations[i].check_out.date.day, db->reservations[i].check_out.date.month, db->reservations[i].check_out.date.year,
                   db->reservations[i].check_out.time.hour, db->reservations[i].check_out.time.minute,
                   db->reservations[i].nights,
                   db->reservations[i].guests,
                   db->reservations[i].status,
                   db->reservations[i].total_price);
            
            reservation_count++;
            total_revenue += db->reservations[i].total_price;
            total_nights += db->reservations[i].nights;
            total_guests += db->reservations[i].guests;
        }
    }
    
    printf("------------------------------------------------------------\n");
    printf("Total Reservations: %d\n", reservation_count);
    
    if (reservation_count > 0) {
        printf("Total Revenue: $%.2f\n", total_revenue);
        printf("Total Nights Booked: %d\n", total_nights);
        printf("Total Guests: %d\n", total_guests);
        printf("Average Revenue per Reservation: $%.2f\n", total_revenue / reservation_count);
        printf("Average Stay Length: %.1f nights\n", (float)total_nights / reservation_count);
        
        // Calculate occupancy rate (simplified - assumes full year)
        int days_in_year = 365;
        double occupancy_rate = (double)total_nights / days_in_year * 100;
        printf("Estimated Occupancy Rate: %.1f%%\n", occupancy_rate);
    }
    
    // Service usage in this room
    printf("\nService Usage:\n");
    printf("%-20s %-15s %-10s\n", "Service", "Usage Count", "Revenue");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < db->service_count; i++) {
        if (db->services[i].active) {
            int usage_count = 0;
            double service_revenue = 0.0;
            
            for (int j = 0; j < db->bill_item_count; j++) {
                int bill_reservation = db->bill_items[j].reservation_id;
                
                // Check if this bill item is for this room
                int reservation_index = find_reservation_by_id(db, bill_reservation);
                if (reservation_index != -1 && db->reservations[reservation_index].room_id == room_id) {
                    if (db->bill_items[j].service_id == db->services[i].id) {
                        usage_count++;
                        service_revenue += db->bill_items[j].total_price;
                    }
                }
            }
            
            if (usage_count > 0) {
                printf("%-20s %-15d $%-9.2f\n", 
                       db->services[i].name, usage_count, service_revenue);
            }
        }
    }
}

void generate_reservation_report(const HotelDatabase *db, int reservation_id) {
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index == -1) {
        printf("Reservation not found.\n");
        return;
    }
    
    Reservation *reservation = &db->reservations[reservation_index];
    
    printf("\n=== Reservation Report: #%d ===\n", reservation_id);
    
    // Display reservation details
    display_reservation(reservation, db);
    
    // Calculate total bill
    double total_bill = calculate_total_bill(db, reservation_id);
    
    printf("\nBill Summary:\n");
    printf("Room Charges: $%.2f\n", reservation->total_price);
    
    // Display bill items
    printf("\nServices Ordered:\n");
    printf("%-20s %-10s %-10s %-10s\n", "Service", "Quantity", "Unit Price", "Total");
    printf("------------------------------------------------\n");
    
    double service_total = 0.0;
    for (int i = 0; i < db->bill_item_count; i++) {
        if (db->bill_items[i].reservation_id == reservation_id) {
            int service_index = find_service_by_id(db, db->bill_items[i].service_id);
            if (service_index != -1) {
                printf("%-20s %-10d $%-9.2f $%-9.2f\n", 
                       db->services[service_index].name,
                       db->bill_items[i].quantity,
                       db->bill_items[i].unit_price,
                       db->bill_items[i].total_price);
                
                service_total += db->bill_items[i].total_price;
            }
        }
    }
    
    printf("------------------------------------------------\n");
    printf("Service Total: $%.2f\n", service_total);
    printf("Subtotal: $%.2f\n", reservation->total_price + service_total);
    printf("Deposit: -$%.2f\n", reservation->deposit);
    printf("Total Due: $%.2f\n", total_bill);
}

void generate_occupancy_report(const HotelDatabase *db, DateTime start_date, DateTime end_date) {
    printf("\n=== Occupancy Report ===\n");
    printf("Period: %02d/%02d/%d %02d:%02d to %02d/%02d/%d %02d:%02d\n",
           start_date.date.day, start_date.date.month, start_date.date.year,
           start_date.time.hour, start_date.time.minute,
           end_date.date.day, end_date.date.month, end_date.date.year,
           end_date.time.hour, end_date.time.minute);
    
    printf("\nRoom Occupancy:\n");
    printf("%-10s %-20s %-15s %-15s %-15s\n", 
           "Room #", "Type", "Status", "Occupied Days", "Occupancy Rate");
    printf("------------------------------------------------------------\n");
    
    int total_days = 0;
    int total_occupied_days = 0;
    
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].active) {
            int room_occupied_days = 0;
            
            // Calculate occupied days for this room in the period
            for (int j = 0; j < db->reservation_count; j++) {
                if (db->reservations[j].active && db->reservations[j].room_id == db->rooms[i].id) {
                    if (strcmp(db->reservations[j].status, "CheckedIn") == 0 || 
                        strcmp(db->reservations[j].status, "CheckedOut") == 0) {
                        
                        // Check if reservation overlaps with the report period
                        // Simplified calculation - counting full nights
                        room_occupied_days += db->reservations[j].nights;
                    }
                }
            }
            
            // Calculate occupancy rate
            double occupancy_rate = 0.0;
            if (room_occupied_days > 0) {
                // Simplified - assuming 30-day period
                total_days = 30;
                occupancy_rate = (double)room_occupied_days / total_days * 100;
            }
            
            printf("%-10d %-20s %-15s %-15d %-15.1f%%\n", 
                   db->rooms[i].room_number,
                   db->rooms[i].room_type,
                   db->rooms[i].status,
                   room_occupied_days,
                   occupancy_rate);
            
            total_occupied_days += room_occupied_days;
        }
    }
    
    printf("------------------------------------------------------------\n");
    
    // Overall occupancy
    double overall_occupancy = 0.0;
    if (total_days > 0 && db->room_count > 0) {
        overall_occupancy = (double)total_occupied_days / (total_days * db->room_count) * 100;
    }
    
    printf("Total Occupied Days: %d\n", total_occupied_days);
    printf("Total Room Days Available: %d\n", total_days * db->room_count);
    printf("Overall Occupancy Rate: %.1f%%\n", overall_occupancy);
    
    // Room type occupancy
    printf("\nOccupancy by Room Type:\n");
    printf("%-20s %-15s %-15s %-15s\n", 
           "Type", "Total Rooms", "Occupied Days", "Occupancy Rate");
    printf("------------------------------------------------------------\n");
    
    int standard_rooms = 0;
    int standard_occupied = 0;
    int deluxe_rooms = 0;
    int deluxe_occupied = 0;
    int suite_rooms = 0;
    int suite_occupied = 0;
    int presidential_rooms = 0;
    int presidential_occupied = 0;
    
    // Count rooms by type
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].active) {
            if (strcmp(db->rooms[i].room_type, "Standard") == 0) {
                standard_rooms++;
            } else if (strcmp(db->rooms[i].room_type, "Deluxe") == 0) {
                deluxe_rooms++;
            } else if (strcmp(db->rooms[i].room_type, "Suite") == 0) {
                suite_rooms++;
            } else if (strcmp(db->rooms[i].room_type, "Presidential") == 0) {
                presidential_rooms++;
            }
        }
    }
    
    // Count occupied days by room type
    for (int j = 0; j < db->reservation_count; j++) {
        if (db->reservations[j].active) {
            if (strcmp(db->reservations[j].status, "CheckedIn") == 0 || 
                strcmp(db->reservations[j].status, "CheckedOut") == 0) {
                
                int room_index = find_room_by_id(db, db->reservations[j].room_id);
                if (room_index != -1) {
                    if (strcmp(db->rooms[room_index].room_type, "Standard") == 0) {
                        standard_occupied += db->reservations[j].nights;
                    } else if (strcmp(db->rooms[room_index].room_type, "Deluxe") == 0) {
                        deluxe_occupied += db->reservations[j].nights;
                    } else if (strcmp(db->rooms[room_index].room_type, "Suite") == 0) {
                        suite_occupied += db->reservations[j].nights;
                    } else if (strcmp(db->rooms[room_index].room_type, "Presidential") == 0) {
                        presidential_occupied += db->reservations[j].nights;
                    }
                }
            }
        }
    }
    
    printf("%-20s %-15d %-15d %-15.1f%%\n", 
           "Standard", standard_rooms, standard_occupied,
           standard_rooms > 0 ? (double)standard_occupied / (total_days * standard_rooms) * 100 : 0.0);
    printf("%-20s %-15d %-15d %-15.1f%%\n", 
           "Deluxe", deluxe_rooms, deluxe_occupied,
           deluxe_rooms > 0 ? (double)deluxe_occupied / (total_days * deluxe_rooms) * 100 : 0.0);
    printf("%-20s %-15d %-15d %-15.1f%%\n", 
           "Suite", suite_rooms, suite_occupied,
           suite_rooms > 0 ? (double)suite_occupied / (total_days * suite_rooms) * 100 : 0.0);
    printf("%-20s %-15d %-15d %-15.1f%%\n", 
           "Presidential", presidential_rooms, presidential_occupied,
           presidential_rooms > 0 ? (double)presidential_occupied / (total_days * presidential_rooms) * 100 : 0.0);
}

void generate_revenue_report(const HotelDatabase *db, DateTime start_date, DateTime end_date) {
    printf("\n=== Revenue Report ===\n");
    printf("Period: %02d/%02d/%d %02d:%02d to %02d/%02d/%d %02d:%02d\n",
           start_date.date.day, start_date.date.month, start_date.date.year,
           start_date.time.hour, start_date.time.minute,
           end_date.date.day, end_date.date.month, end_date.date.year,
           end_date.time.hour, end_date.time.minute);
    
    printf("\nRevenue Summary:\n");
    printf("%-20s %-15s\n", "Revenue Source", "Amount");
    printf("----------------------------\n");
    
    double room_revenue = 0.0;
    double service_revenue = 0.0;
    double total_revenue = 0.0;
    
    // Calculate room revenue
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].active) {
            // Simplified - including all reservations
            if (strcmp(db->reservations[i].status, "CheckedOut") == 0) {
                room_revenue += db->reservations[i].total_price;
            }
        }
    }
    
    // Calculate service revenue
    for (int i = 0; i < db->bill_item_count; i++) {
        service_revenue += db->bill_items[i].total_price;
    }
    
    total_revenue = room_revenue + service_revenue;
    
    printf("%-20s $%-14.2f\n", "Room Revenue", room_revenue);
    printf("%-20s $%-14.2f\n", "Service Revenue", service_revenue);
    printf("%-20s $%-14.2f\n", "Total Revenue", total_revenue);
    
    // Revenue by room type
    printf("\nRevenue by Room Type:\n");
    printf("%-20s %-15s %-15s\n", "Room Type", "Count", "Revenue");
    printf("-----------------------------------------\n");
    
    int standard_count = 0;
    double standard_revenue = 0.0;
    int deluxe_count = 0;
    double deluxe_revenue = 0.0;
    int suite_count = 0;
    double suite_revenue = 0.0;
    int presidential_count = 0;
    double presidential_revenue = 0.0;
    
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].active && strcmp(db->reservations[i].status, "CheckedOut") == 0) {
            int room_index = find_room_by_id(db, db->reservations[i].room_id);
            if (room_index != -1) {
                if (strcmp(db->rooms[room_index].room_type, "Standard") == 0) {
                    standard_count++;
                    standard_revenue += db->reservations[i].total_price;
                } else if (strcmp(db->rooms[room_index].room_type, "Deluxe") == 0) {
                    deluxe_count++;
                    deluxe_revenue += db->reservations[i].total_price;
                } else if (strcmp(db->rooms[room_index].room_type, "Suite") == 0) {
                    suite_count++;
                    suite_revenue += db->reservations[i].total_price;
                } else if (strcmp(db->rooms[room_index].room_type, "Presidential") == 0) {
                    presidential_count++;
                    presidential_revenue += db->reservations[i].total_price;
                }
            }
        }
    }
    
    printf("%-20s %-15d $%-14.2f\n", "Standard", standard_count, standard_revenue);
    printf("%-20s %-15d $%-14.2f\n", "Deluxe", deluxe_count, deluxe_revenue);
    printf("%-20s %-15d $%-14.2f\n", "Suite", suite_count, suite_revenue);
    printf("%-20s %-15d $%-14.2f\n", "Presidential", presidential_count, presidential_revenue);
    
    // Revenue by service category
    printf("\nRevenue by Service Category:\n");
    printf("%-20s %-15s\n", "Service Category", "Revenue");
    printf("----------------------------\n");
    
    double restaurant_revenue = 0.0;
    double spa_revenue = 0.0;
    double laundry_revenue = 0.0;
    double transport_revenue = 0.0;
    double entertainment_revenue = 0.0;
    
    for (int i = 0; i < db->service_count; i++) {
        if (db->services[i].active) {
            double category_revenue = 0.0;
            
            for (int j = 0; j < db->bill_item_count; j++) {
                if (db->bill_items[j].service_id == db->services[i].id) {
                    category_revenue += db->bill_items[j].total_price;
                }
            }
            
            if (strcmp(db->services[i].category, "Restaurant") == 0) {
                restaurant_revenue += category_revenue;
            } else if (strcmp(db->services[i].category, "Spa") == 0) {
                spa_revenue += category_revenue;
            } else if (strcmp(db->services[i].category, "Laundry") == 0) {
                laundry_revenue += category_revenue;
            } else if (strcmp(db->services[i].category, "Transport") == 0) {
                transport_revenue += category_revenue;
            } else if (strcmp(db->services[i].category, "Entertainment") == 0) {
                entertainment_revenue += category_revenue;
            }
        }
    }
    
    printf("%-20s $%-14.2f\n", "Restaurant", restaurant_revenue);
    printf("%-20s $%-14.2f\n", "Spa", spa_revenue);
    printf("%-20s $%-14.2f\n", "Laundry", laundry_revenue);
    printf("%-20s $%-14.2f\n", "Transport", transport_revenue);
    printf("%-20s $%-14.2f\n", "Entertainment", entertainment_revenue);
}

void generate_service_report(const HotelDatabase *db, DateTime start_date, DateTime end_date) {
    printf("\n=== Service Report ===\n");
    printf("Period: %02d/%02d/%d %02d:%02d to %02d/%02d/%d %02d:%02d\n",
           start_date.date.day, start_date.date.month, start_date.date.year,
           start_date.time.hour, start_date.time.minute,
           end_date.date.day, end_date.date.month, end_date.date.year,
           end_date.time.hour, end_date.time.minute);
    
    printf("\nService Usage:\n");
    printf("%-5s %-20s %-15s %-15s %-10s\n", 
           "ID", "Service", "Category", "Usage Count", "Revenue");
    printf("---------------------------------------------------------\n");
    
    for (int i = 0; i < db->service_count; i++) {
        if (db->services[i].active) {
            int usage_count = 0;
            double revenue = 0.0;
            
            for (int j = 0; j < db->bill_item_count; j++) {
                if (db->bill_items[j].service_id == db->services[i].id) {
                    usage_count++;
                    revenue += db->bill_items[j].total_price;
                }
            }
            
            printf("%-5d %-20s %-15s %-15d $%-9.2f\n", 
                   db->services[i].id,
                   db->services[i].name,
                   db->services[i].category,
                   usage_count,
                   revenue);
        }
    }
    
    // Service usage by category
    printf("\nService Usage by Category:\n");
    printf("%-20s %-15s %-15s\n", 
           "Category", "Usage Count", "Revenue");
    printf("------------------------------------------------\n");
    
    int restaurant_count = 0;
    double restaurant_revenue = 0.0;
    int spa_count = 0;
    double spa_revenue = 0.0;
    int laundry_count = 0;
    double laundry_revenue = 0.0;
    int transport_count = 0;
    double transport_revenue = 0.0;
    int entertainment_count = 0;
    double entertainment_revenue = 0.0;
    
    for (int i = 0; i < db->service_count; i++) {
        if (db->services[i].active) {
            int usage_count = 0;
            double revenue = 0.0;
            
            for (int j = 0; j < db->bill_item_count; j++) {
                if (db->bill_items[j].service_id == db->services[i].id) {
                    usage_count++;
                    revenue += db->bill_items[j].total_price;
                }
            }
            
            if (strcmp(db->services[i].category, "Restaurant") == 0) {
                restaurant_count += usage_count;
                restaurant_revenue += revenue;
            } else if (strcmp(db->services[i].category, "Spa") == 0) {
                spa_count += usage_count;
                spa_revenue += revenue;
            } else if (strcmp(db->services[i].category, "Laundry") == 0) {
                laundry_count += usage_count;
                laundry_revenue += revenue;
            } else if (strcmp(db->services[i].category, "Transport") == 0) {
                transport_count += usage_count;
                transport_revenue += revenue;
            } else if (strcmp(db->services[i].category, "Entertainment") == 0) {
                entertainment_count += usage_count;
                entertainment_revenue += revenue;
            }
        }
    }
    
    printf("%-20s %-15d $%-14.2f\n", "Restaurant", restaurant_count, restaurant_revenue);
    printf("%-20s %-15d $%-14.2f\n", "Spa", spa_count, spa_revenue);
    printf("%-20s %-15d $%-14.2f\n", "Laundry", laundry_count, laundry_revenue);
    printf("%-20s %-15d $%-14.2f\n", "Transport", transport_count, transport_revenue);
    printf("%-20s %-15d $%-14.2f\n", "Entertainment", entertainment_count, entertainment_revenue);
}

int calculate_reservation_total(const HotelDatabase *db, int reservation_id) {
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index == -1) {
        return 0;
    }
    
    double total = db->reservations[reservation_index].total_price;
    
    // Add service charges
    for (int i = 0; i < db->bill_item_count; i++) {
        if (db->bill_items[i].reservation_id == reservation_id) {
            total += db->bill_items[i].total_price;
        }
    }
    
    return total;
}

int calculate_bill_total(const HotelDatabase *db, int reservation_id) {
    double total = calculate_reservation_total(db, reservation_id);
    
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index != -1) {
        total -= db->reservations[reservation_index].deposit;
    }
    
    return total;
}

void apply_promotion(HotelDatabase *db, const char *room_type, double discount_percent) {
    int updated_count = 0;
    
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].active && strcmp(db->rooms[i].room_type, room_type) == 0) {
            // Apply discount to price_per_night
            db->rooms[i].price_per_night *= (1.0 - discount_percent / 100.0);
            updated_count++;
        }
    }
    
    printf("Promotion applied to %d rooms of type '%s'.\n", updated_count, room_type);
    printf("Discount: %.1f%%\n", discount_percent);
}

void update_loyalty_points(HotelDatabase *db, int customer_id, int points) {
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    Customer *customer = &db->customers[customer_index];
    
    customer->points += points;
    
    // Update loyalty tier based on points
    if (customer->points >= 1000) {
        customer->loyalty_tier = 3; // Platinum
    } else if (customer->points >= 500) {
        customer->loyalty_tier = 2; // Gold
    } else if (customer->points >= 100) {
        customer->loyalty_tier = 1; // Silver
    }
    
    printf("Loyalty points updated for customer %s %s.\n", 
           customer->first_name, customer->last_name);
    printf("Points added: %d\n", points);
    printf("Total points: %d\n", customer->points);
    printf("New tier: ");
    
    switch (customer->loyalty_tier) {
        case 0: printf("Regular\n"); break;
        case 1: printf("Silver\n"); break;
        case 2: printf("Gold\n"); break;
        case 3: printf("Platinum\n"); break;
        default: printf("Unknown\n"); break;
    }
}

int is_valid_email(const char *email) {
    if (email == NULL) return 0;
    
    int at_symbol = 0;
    int dot_after_at = 0;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_symbol = 1;
        } else if (at_symbol && email[i] == '.') {
            dot_after_at = 1;
        }
    }
    
    return at_symbol && dot_after_at;
}

int is_valid_phone(const char *phone) {
    if (phone == NULL) return 0;
    
    // Simple validation: check if phone starts with valid prefix and has enough digits
    return (strncmp(phone, "555-", 4) == 0 || 
            strncmp(phone, "(", 1) == 0 || 
            strncmp(phone, "+", 1) == 0);
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int max_days;
    switch (month) {
        case 2:
            // Leap year check
            max_days = ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            max_days = 30;
            break;
        default:
            max_days = 31;
    }
    
    return day >= 1 && day <= max_days;
}

int is_valid_time(int hour, int minute) {
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

int is_valid_room_type(const char *room_type) {
    return strcmp(room_type, "Standard") == 0 || 
           strcmp(room_type, "Deluxe") == 0 || 
           strcmp(room_type, "Suite") == 0 || 
           strcmp(room_type, "Presidential") == 0;
}

int is_valid_payment_method(const char *payment_method) {
    return strcmp(payment_method, "CreditCard") == 0 || 
           strcmp(payment_method, "Cash") == 0 || 
           strcmp(payment_method, "BankTransfer") == 0 || 
           strcmp(payment_method, "Check") == 0;
}

int is_valid_service_category(const char *category) {
    return strcmp(category, "Restaurant") == 0 || 
           strcmp(category, "Spa") == 0 || 
           strcmp(category, "Laundry") == 0 || 
           strcmp(category, "Transport") == 0 || 
           strcmp(category, "Entertainment") == 0;
}

int find_customer_by_id(const HotelDatabase *db, int customer_id) {
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].id == customer_id && db->customers[i].active) {
            return i;
        }
    }
    return -1;
}

int find_room_by_id(const HotelDatabase *db, int room_id) {
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].id == room_id && db->rooms[i].active) {
            return i;
        }
    }
    return -1;
}

int find_reservation_by_id(const HotelDatabase *db, int reservation_id) {
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].id == reservation_id && db->reservations[i].active) {
            return i;
        }
    }
    return -1;
}

int find_service_by_id(const HotelDatabase *db, int service_id) {
    for (int i = 0; i < db->service_count; i++) {
        if (db->services[i].id == service_id && db->services[i].active) {
            return i;
        }
    }
    return -1;
}

int find_bill_item_by_id(const HotelDatabase *db, int bill_item_id) {
    for (int i = 0; i < db->bill_item_count; i++) {
        if (db->bill_items[i].id == bill_item_id) {
            return i;
        }
    }
    return -1;
}

int find_room_by_number(const HotelDatabase *db, int room_number) {
    for (int i = 0; i < db->room_count; i++) {
        if (db->rooms[i].active && db->rooms[i].room_number == room_number) {
            return i;
        }
    }
    return -1;
}

int save_database(const HotelDatabase *db) {
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "wb");
    if (customers_file != NULL) {
        fwrite(&db->customer_count, sizeof(int), 1, customers_file);
        fwrite(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *rooms_file = fopen(FILENAME_ROOMS, "wb");
    if (rooms_file != NULL) {
        fwrite(&db->room_count, sizeof(int), 1, rooms_file);
        fwrite(db->rooms, sizeof(Room), db->room_count, rooms_file);
        fclose(rooms_file);
    } else {
        return 0;
    }
    
    FILE *reservations_file = fopen(FILENAME_RESERVATIONS, "wb");
    if (reservations_file != NULL) {
        fwrite(&db->reservation_count, sizeof(int), 1, reservations_file);
        fwrite(db->reservations, sizeof(Reservation), db->reservation_count, reservations_file);
        fclose(reservations_file);
    } else {
        return 0;
    }
    
    FILE *services_file = fopen(FILENAME_SERVICES, "wb");
    if (services_file != NULL) {
        fwrite(&db->service_count, sizeof(int), 1, services_file);
        fwrite(db->services, sizeof(Service), db->service_count, services_file);
        fclose(services_file);
    } else {
        return 0;
    }
    
    FILE *bills_file = fopen(FILENAME_BILLS, "wb");
    if (bills_file != NULL) {
        fwrite(&db->bill_item_count, sizeof(int), 1, bills_file);
        fwrite(db->bill_items, sizeof(BillItem), db->bill_item_count, bills_file);
        fclose(bills_file);
        return 1;
    }
    
    return 0;
}

int load_database(HotelDatabase *db) {
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "rb");
    if (customers_file != NULL) {
        fread(&db->customer_count, sizeof(int), 1, customers_file);
        fread(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *rooms_file = fopen(FILENAME_ROOMS, "rb");
    if (rooms_file != NULL) {
        fread(&db->room_count, sizeof(int), 1, rooms_file);
        fread(db->rooms, sizeof(Room), db->room_count, rooms_file);
        fclose(rooms_file);
    } else {
        return 0;
    }
    
    FILE *reservations_file = fopen(FILENAME_RESERVATIONS, "rb");
    if (reservations_file != NULL) {
        fread(&db->reservation_count, sizeof(int), 1, reservations_file);
        fread(db->reservations, sizeof(Reservation), db->reservation_count, reservations_file);
        fclose(reservations_file);
    } else {
        return 0;
    }
    
    FILE *services_file = fopen(FILENAME_SERVICES, "rb");
    if (services_file != NULL) {
        fread(&db->service_count, sizeof(int), 1, services_file);
        fread(db->services, sizeof(Service), db->service_count, services_file);
        fclose(services_file);
    } else {
        return 0;
    }
    
    FILE *bills_file = fopen(FILENAME_BILLS, "rb");
    if (bills_file != NULL) {
        fread(&db->bill_item_count, sizeof(int), 1, bills_file);
        fread(db->bill_items, sizeof(BillItem), db->bill_item_count, bills_file);
        fclose(bills_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(HotelDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* nationalities[] = {"American", "British", "Canadian", "Australian", "German",
                             "French", "Spanish", "Italian", "Japanese", "Chinese"};
    
    char* room_types[] = {"Standard", "Deluxe", "Suite", "Presidential"};
    
    char* service_names[] = {"Room Service", "Spa Treatment", "Laundry Service", "Airport Shuttle",
                            "Breakfast Buffet", "Dinner", "Swimming Pool", "Gym Access",
                            "Concierge", "Business Center", "Parking", "Pet Care"};
    
    char* service_categories[] = {"Restaurant", "Spa", "Laundry", "Transport", "Entertainment"};
    
    char* payment_methods[] = {"CreditCard", "Cash", "BankTransfer", "Check"};
    
    char* special_requests[] = {"Late check-out", "Early check-in", "Extra bed", "Baby cot",
                                 "High floor", "Away from elevator", "Quiet room", "Non-smoking"};
    
    srand(time(NULL));
    
    // Add rooms
    for (int i = 0; i < 100 && db->room_count < MAX_ROOMS; i++) {
        int room_number = 100 + i;
        int floor = (room_number / 100) + 1;
        const char* room_type = room_types[rand() % 4];
        int bed_count = 1 + (rand() % 4);
        int capacity = 1 + (rand() % 4);
        double price_per_night = 50.0 + (rand() % 500) + (rand() % 100) / 100.0;
        double price_per_hour = price_per_night / 8; // 8 hours = 1 night
        char amenities[200];
        
        sprintf(amenities, "Air conditioning, TV, Wi-Fi, Mini bar, Safe, %s", 
                rand() % 2 ? "Balcony" : "City view");
        
        int smoking_allowed = rand() % 2;
        
        add_room(db, room_number, floor, room_type, bed_count, capacity,
                price_per_night, price_per_hour, amenities, smoking_allowed);
    }
    
    // Add customers
    for (int i = 0; i < 150 && db->customer_count < MAX_CUSTOMERS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], address[MAX_ADDRESS_LENGTH];
        sprintf(email, "%s.%d@hotel.com", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Hotel Ave, City, State", 100 + rand() % 900);
        
        char id_number[20];
        sprintf(id_number, "ID-%08d", rand() % 100000000);
        
        const char* nationality = nationalities[rand() % 10];
        
        add_customer(db, first_name, last_name, email, phone, address, id_number, nationality);
    }
    
    // Add services
    for (int i = 0; i < 30 && db->service_count < MAX_SERVICES; i++) {
        char description[200];
        sprintf(description, "Premium %s for hotel guests", service_names[i % 12]);
        
        double price = 5.0 + (rand() % 200) + (rand() % 100) / 100.0;
        const char* category = service_categories[rand() % 5];
        
        add_service(db, service_names[i % 12], description, price, category);
    }
    
    // Add reservations
    for (int i = 0; i < 500 && db->reservation_count < MAX_RESERVATIONS; i++) {
        int customer_id = 1 + (rand() % 150);
        int room_id = 1 + (rand() % 100);
        
        DateTime check_in, check_out;
        
        // Random dates within next year
        int days_ahead = rand() % 365;
        int nights = 1 + (rand() % 14); // 1-14 nights
        
        time_t now = time(NULL);
        time_t check_in_time = now + (days_ahead * 24 * 60 * 60);
        time_t check_out_time = check_in_time + (nights * 24 * 60 * 60);
        
        struct tm *check_in_tm = localtime(&check_in_time);
        check_in.date.day = check_in_tm->tm_mday;
        check_in.date.month = check_in_tm->tm_mon + 1;
        check_in.date.year = check_in_tm->tm_year + 1900;
        check_in.time.hour = 14 + rand() % 6; // 2-8 PM check-in
        check_in.time.minute = rand() % 60;
        
        struct tm *check_out_tm = localtime(&check_out_time);
        check_out.date.day = check_out_tm->tm_mday;
        check_out.date.month = check_out_tm->tm_mon + 1;
        check_out.date.year = check_out_tm->tm_year + 1900;
        check_out.time.hour = 10 + rand() % 2; // 10-11 AM check-out
        check_out.time.minute = rand() % 60;
        
        int guests = 1 + (rand() % 4);
        double deposit = 50.0 + (rand() % 200);
        const char* payment_method = payment_methods[rand() % 4];
        const char* special_request = special_requests[rand() % 8];
        
        int reservation_id = create_reservation(db, customer_id, room_id, check_in, check_out,
                                              guests, deposit, payment_method, special_request);
        
        if (reservation_id != -1) {
            // Add bill items for some reservations
            if (rand() % 3 == 0) { // 33% chance
                int service_count = 1 + (rand() % 5);
                for (int j = 0; j < service_count; j++) {
                    int service_id = 1 + (rand() % 30);
                    int quantity = 1 + (rand() % 3);
                    
                    // Get service price
                    int service_index = find_service_by_id(db, service_id);
                    double unit_price = 0.0;
                    if (service_index != -1) {
                        unit_price = db->services[service_index].price;
                    }
                    
                    // Random datetime during the stay
                    time_t service_time = check_in_time + (rand() % nights) * (24 * 60 * 60);
                    struct tm *service_tm = localtime(&service_time);
                    DateTime service_datetime;
                    service_datetime.date.day = service_tm->tm_mday;
                    service_datetime.date.month = service_tm->tm_mon + 1;
                    service_datetime.date.year = service_tm->tm_year + 1900;
                    service_datetime.time.hour = service_tm->tm_hour;
                    service_datetime.time.minute = service_tm->tm_min;
                    
                    char notes[100];
                    sprintf(notes, "Service requested by guest");
                    
                    add_bill_item(db, reservation_id, service_id, service_datetime,
                                 quantity, unit_price, notes);
                }
            }
            
            // Randomly set reservation status
            int status_rand = rand() % 5;
            switch (status_rand) {
                case 0: strcpy(db->reservations[db->reservation_count - 1].status, "Booked"); break;
                case 1: strcpy(db->reservations[db->reservation_count - 1].status, "CheckedIn"); break;
                case 2: strcpy(db->reservations[db->reservation_count - 1].status, "CheckedOut"); break;
                case 3: strcpy(db->reservations[db->reservation_count - 1].status, "Cancelled"); break;
                case 4: strcpy(db->reservations[db->reservation_count - 1].status, "NoShow"); break;
            }
            
            // If checked out, update room status back to available
            if (strcmp(db->reservations[db->reservation_count - 1].status, "CheckedOut") == 0) {
                int room_index = find_room_by_id(db, room_id);
                if (room_index != -1) {
                    strcpy(db->rooms[room_index].status, "Available");
                }
            }
            
            // If checked in, update room status to occupied
            if (strcmp(db->reservations[db->reservation_count - 1].status, "CheckedIn") == 0) {
                int room_index = find_room_by_id(db, room_id);
                if (room_index != -1) {
                    strcpy(db->rooms[room_index].status, "Occupied");
                }
            }
        }
    }
    
    // Update some customers' loyalty points based on their reservations
    for (int i = 0; i < 50; i++) {
        int customer_id = 1 + (rand() % 150);
        int customer_index = find_customer_by_id(db, customer_id);
        
        if (customer_index >= 0 && customer_index < db->customer_count) {
            int points = 10 + (rand() % 1000);
            update_loyalty_points(db, customer_id, points);
        }
    }
}

const char* get_reservation_status_description(const char *status) {
    if (strcmp(status, "Booked") == 0) return "Reservation is confirmed and waiting for check-in";
    if (strcmp(status, "CheckedIn") == 0) return "Guest has checked in and is currently staying";
    if (strcmp(status, "CheckedOut") == 0) return "Guest has checked out and reservation is complete";
    if (strcmp(status, "Cancelled") == 0) return "Reservation has been cancelled by guest or hotel";
    if (strcmp(status, "NoShow") == 0) return "Guest did not arrive for the reservation";
    return "Unknown status";
}

const char* get_room_status_description(const char *status) {
    if (strcmp(status, "Available") == 0) return "Room is available for booking";
    if (strcmp(status, "Occupied") == 0) return "Room is currently occupied by a guest";
    if (strcmp(status, "Maintenance") == 0) return "Room is under maintenance and not available";
    if (strcmp(status, "OutOfService") == 0) return "Room is temporarily out of service";
    return "Unknown status";
}

int is_room_available(const HotelDatabase *db, int room_id, DateTime check_in, DateTime check_out) {
    // Check if room is currently available
    int room_index = find_room_by_id(db, room_id);
    if (room_index == -1) {
        return 0;
    }
    
    if (strcmp(db->rooms[room_index].status, "Available") != 0) {
        return 0;
    }
    
    // Check all existing reservations for this room
    for (int i = 0; i < db->reservation_count; i++) {
        if (db->reservations[i].active && db->reservations[i].room_id == room_id) {
            // Skip cancelled reservations
            if (strcmp(db->reservations[i].status, "Cancelled") == 0 ||
                strcmp(db->reservations[i].status, "CheckedOut") == 0) {
                continue;
            }
            
            // Check for date overlap
            // This is a simplified check - in a real system, you'd need more sophisticated date logic
            int res_check_in = db->reservations[i].check_in.date.year * 10000 + 
                              db->reservations[i].check_in.date.month * 100 + 
                              db->reservations[i].check_in.date.day;
            
            int res_check_out = db->reservations[i].check_out.date.year * 10000 + 
                               db->reservations[i].check_out.date.month * 100 + 
                               db->reservations[i].check_out.date.day;
            
            int new_check_in = check_in.date.year * 10000 + 
                              check_in.date.month * 100 + 
                              check_in.date.day;
            
            int new_check_out = check_out.date.year * 10000 + 
                               check_out.date.month * 100 + 
                               check_out.date.day;
            
            // Check if dates overlap
            if ((new_check_in >= res_check_in && new_check_in < res_check_out) ||
                (new_check_out > res_check_in && new_check_out <= res_check_out) ||
                (new_check_in <= res_check_in && new_check_out >= res_check_out)) {
                return 0; // Not available
            }
        }
    }
    
    return 1; // Available
}

double calculate_total_bill(HotelDatabase *db, int reservation_id) {
    double total = 0.0;
    
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index != -1) {
        total += db->reservations[reservation_index].total_price;
        
        // Add service charges
        for (int i = 0; i < db->bill_item_count; i++) {
            if (db->bill_items[i].reservation_id == reservation_id) {
                total += db->bill_items[i].total_price;
            }
        }
        
        // Subtract deposit
        total -= db->reservations[reservation_index].deposit;
    }
    
    return total;
}

void process_payment(HotelDatabase *db, int reservation_id) {
    int reservation_index = find_reservation_by_id(db, reservation_id);
    if (reservation_index == -1) {
        return;
    }
    
    Reservation *reservation = &db->reservations[reservation_index];
    
    // Calculate total bill
    double total = calculate_total_bill(db, reservation_id);
    
    // Update bill items status to "Paid"
    for (int i = 0; i < db->bill_item_count; i++) {
        if (db->bill_items[i].reservation_id == reservation_id) {
            strcpy(db->bill_items[i].status, "Paid");
        }
    }
    
    printf("Payment processed for reservation #%d.\n", reservation_id);
    printf("Payment method: %s\n", reservation->payment_method);
    printf("Total amount: $%.2f\n", total);
}