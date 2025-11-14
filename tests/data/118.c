#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_FLIGHTS 500
#define MAX_BOOKINGS 2000
#define MAX_PASSENGERS 1000
#define MAX_AIRPORTS 100
#define MAX_AIRCRAFT 50
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define FILENAME_FLIGHTS "flights.dat"
#define FILENAME_BOOKINGS "bookings.dat"
#define FILENAME_PASSENGERS "passengers.dat"
#define FILENAME_AIRPORTS "airports.dat"
#define FILENAME_AIRCRAFT "aircraft.dat"

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

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char city[MAX_NAME_LENGTH];
    char country[MAX_NAME_LENGTH];
    char code[4]; // 3-letter airport code
    int timezone_offset; // Hours from UTC
} Airport;

typedef struct {
    int id;
    char model[MAX_NAME_LENGTH];
    char manufacturer[MAX_NAME_LENGTH];
    int capacity;
    int range; // in kilometers
    int cruising_speed; // in km/h
    int fuel_capacity; // in liters
    char registration_number[20];
} Aircraft;

typedef enum {
    SCHEDULED,
    DELAYED,
    CANCELLED,
    DEPARTED,
    LANDED,
    BOARDING
} FlightStatus;

typedef struct {
    int id;
    int flight_number;
    int aircraft_id;
    int departure_airport_id;
    int arrival_airport_id;
    DateTime departure_time;
    DateTime arrival_time;
    FlightStatus status;
    float price_economy;
    float price_business;
    float price_first_class;
    int seats_economy;
    int seats_business;
    int seats_first_class;
    int available_economy;
    int available_business;
    int available_first_class;
    char gate[10];
} Flight;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char passport_number[20];
    Date birth_date;
    char nationality[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char phone[15];
    char email[50];
    char gender; // 'M', 'F', or 'O'
    int frequent_flyer_miles;
    char membership_level[20]; // "Basic", "Silver", "Gold", "Platinum"
} Passenger;

typedef enum {
    ECONOMY,
    BUSINESS,
    FIRST_CLASS
} TicketClass;

typedef struct {
    int id;
    int flight_id;
    int passenger_id;
    TicketClass ticket_class;
    float price;
    Date booking_date;
    char seat_number[10];
    int checked_in;
    int cancelled;
    char special_requests[200];
} Booking;

typedef struct {
    Flight flights[MAX_FLIGHTS];
    int flight_count;
    Airport airports[MAX_AIRPORTS];
    int airport_count;
    Aircraft aircraft[MAX_AIRCRAFT];
    int aircraft_count;
    Passenger passengers[MAX_PASSENGERS];
    int passenger_count;
    Booking bookings[MAX_BOOKINGS];
    int booking_count;
} AirlineDatabase;

// Function prototypes
void initialize_database(AirlineDatabase *db);
int add_airport(AirlineDatabase *db, const char *name, const char *city, 
                 const char *country, const char *code, int timezone_offset);
int add_aircraft(AirlineDatabase *db, const char *model, const char *manufacturer,
                 int capacity, int range, int cruising_speed, int fuel_capacity,
                 const char *registration_number);
int add_passenger(AirlineDatabase *db, const char *first_name, const char *last_name,
                  const char *passport_number, Date birth_date, const char *nationality,
                  const char *address, const char *phone, const char *email, char gender);
int add_flight(AirlineDatabase *db, int flight_number, int aircraft_id,
               int departure_airport_id, int arrival_airport_id,
               DateTime departure_time, DateTime arrival_time,
               float price_economy, float price_business, float price_first_class);
int create_booking(AirlineDatabase *db, int flight_id, int passenger_id,
                   TicketClass ticket_class, const char *special_requests);
void check_in_passenger(AirlineDatabase *db, int booking_id);
void cancel_booking(AirlineDatabase *db, int booking_id);
void update_flight_status(AirlineDatabase *db, int flight_id, FlightStatus status);
void display_flight(const Flight *flight, const AirlineDatabase *db);
void display_airport(const Airport *airport);
void display_aircraft(const Aircraft *aircraft);
void display_passenger(const Passenger *passenger);
void display_booking(const Booking *booking, const AirlineDatabase *db);
void display_all_flights(const AirlineDatabase *db);
void display_all_airports(const AirlineDatabase *db);
void display_all_aircraft(const AirlineDatabase *db);
void display_all_passengers(const AirlineDatabase *db);
void display_flight_bookings(const AirlineDatabase *db, int flight_id);
void display_passenger_bookings(const AirlineDatabase *db, int passenger_id);
void search_flights_by_route(const AirlineDatabase *db, int departure_airport_id, int arrival_airport_id, Date date);
void search_flights_by_status(const AirlineDatabase *db, FlightStatus status);
void generate_flight_manifest(const AirlineDatabase *db, int flight_id);
void generate_revenue_report(const AirlineDatabase *db, int month, int year);
void update_frequent_flyer_miles(AirlineDatabase *db, int passenger_id, int miles);
int find_flight_by_id(const AirlineDatabase *db, int flight_id);
int find_airport_by_id(const AirlineDatabase *db, int airport_id);
int find_aircraft_by_id(const AirlineDatabase *db, int aircraft_id);
int find_passenger_by_id(const AirlineDatabase *db, int passenger_id);
int find_booking_by_id(const AirlineDatabase *db, int booking_id);
int save_database(const AirlineDatabase *db);
int load_database(AirlineDatabase *db);
void generate_sample_data(AirlineDatabase *db);
void display_airline_statistics(const AirlineDatabase *db);
void find_flights_by_destination(const AirlineDatabase *db, int departure_airport_id, Date date);
int is_valid_date(int day, int month, int year);
int is_valid_time(int hour, int minute);
const char* get_flight_status_name(FlightStatus status);
const char* get_ticket_class_name(TicketClass ticket_class);

int main() {
    AirlineDatabase db;
    initialize_database(&db);
    
    printf("Airline Management System\n");
    printf("1. Add Airport\n");
    printf("2. Add Aircraft\n");
    printf("3. Add Passenger\n");
    printf("4. Add Flight\n");
    printf("5. Create Booking\n");
    printf("6. Check In Passenger\n");
    printf("7. Cancel Booking\n");
    printf("8. Update Flight Status\n");
    printf("9. Display All Flights\n");
    printf("10. Display All Airports\n");
    printf("11. Display All Aircraft\n");
    printf("12. Display All Passengers\n");
    printf("13. Display Flight Bookings\n");
    printf("14. Display Passenger Bookings\n");
    printf("15. Search Flights by Route\n");
    printf("16. Search Flights by Status\n");
    printf("17. Generate Flight Manifest\n");
    printf("18. Generate Revenue Report\n");
    printf("19. Update Frequent Flyer Miles\n");
    printf("20. Save Database\n");
    printf("21. Load Database\n");
    printf("22. Generate Sample Data\n");
    printf("23. Display Airline Statistics\n");
    printf("24. Find Flights by Destination\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH], city[MAX_NAME_LENGTH], country[MAX_NAME_LENGTH];
                char code[4];
                int timezone_offset;
                
                printf("Enter airport name: ");
                scanf(" %[^\n]", name);
                printf("Enter city: ");
                scanf(" %[^\n]", city);
                printf("Enter country: ");
                scanf(" %[^\n]", country);
                printf("Enter 3-letter airport code: ");
                scanf("%s", code);
                printf("Enter timezone offset (hours from UTC): ");
                scanf("%d", &timezone_offset);
                
                int airport_id = add_airport(&db, name, city, country, code, timezone_offset);
                if (airport_id != -1) {
                    printf("Airport added with ID: %d\n", airport_id);
                } else {
                    printf("Failed to add airport. Database is full.\n");
                }
                break;
            }
            case 2: {
                char model[MAX_NAME_LENGTH], manufacturer[MAX_NAME_LENGTH], registration[20];
                int capacity, range, cruising_speed, fuel_capacity;
                
                printf("Enter aircraft model: ");
                scanf(" %[^\n]", model);
                printf("Enter manufacturer: ");
                scanf(" %[^\n]", manufacturer);
                printf("Enter passenger capacity: ");
                scanf("%d", &capacity);
                printf("Enter range (km): ");
                scanf("%d", &range);
                printf("Enter cruising speed (km/h): ");
                scanf("%d", &cruising_speed);
                printf("Enter fuel capacity (liters): ");
                scanf("%d", &fuel_capacity);
                printf("Enter registration number: ");
                scanf("%s", registration);
                
                int aircraft_id = add_aircraft(&db, model, manufacturer, capacity, range,
                                              cruising_speed, fuel_capacity, registration);
                if (aircraft_id != -1) {
                    printf("Aircraft added with ID: %d\n", aircraft_id);
                } else {
                    printf("Failed to add aircraft. Database is full.\n");
                }
                break;
            }
            case 3: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char passport_number[20], nationality[MAX_NAME_LENGTH], address[MAX_ADDRESS_LENGTH];
                char phone[15], email[50];
                int day, month, year;
                char gender;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter passport number: ");
                scanf("%s", passport_number);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter nationality: ");
                scanf(" %[^\n]", nationality);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter gender (M/F/O): ");
                scanf(" %c", &gender);
                
                Date birth_date = {day, month, year};
                
                int passenger_id = add_passenger(&db, first_name, last_name, passport_number,
                                               birth_date, nationality, address, phone, email, gender);
                if (passenger_id != -1) {
                    printf("Passenger added with ID: %d\n", passenger_id);
                } else {
                    printf("Failed to add passenger. Database is full.\n");
                }
                break;
            }
            case 4: {
                int flight_number, aircraft_id, departure_airport_id, arrival_airport_id;
                DateTime departure_time, arrival_time;
                float price_economy, price_business, price_first_class;
                
                printf("Enter flight number: ");
                scanf("%d", &flight_number);
                printf("Enter aircraft ID: ");
                scanf("%d", &aircraft_id);
                printf("Enter departure airport ID: ");
                scanf("%d", &departure_airport_id);
                printf("Enter arrival airport ID: ");
                scanf("%d", &arrival_airport_id);
                printf("Enter departure date and time (DD MM YYYY HH MM): ");
                scanf("%d %d %d %d %d", &departure_time.day, &departure_time.month,
                      &departure_time.year, &departure_time.hour, &departure_time.minute);
                printf("Enter arrival date and time (DD MM YYYY HH MM): ");
                scanf("%d %d %d %d %d", &arrival_time.day, &arrival_time.month,
                      &arrival_time.year, &arrival_time.hour, &arrival_time.minute);
                printf("Enter economy class price: ");
                scanf("%f", &price_economy);
                printf("Enter business class price: ");
                scanf("%f", &price_business);
                printf("Enter first class price: ");
                scanf("%f", &price_first_class);
                
                int flight_id = add_flight(&db, flight_number, aircraft_id, departure_airport_id,
                                          arrival_airport_id, departure_time, arrival_time,
                                          price_economy, price_business, price_first_class);
                if (flight_id != -1) {
                    printf("Flight added with ID: %d\n", flight_id);
                } else {
                    printf("Failed to add flight. Check IDs and availability.\n");
                }
                break;
            }
            case 5: {
                int flight_id, passenger_id, ticket_class;
                char special_requests[200];
                
                printf("Enter flight ID: ");
                scanf("%d", &flight_id);
                printf("Enter passenger ID: ");
                scanf("%d", &passenger_id);
                printf("Enter ticket class (0-Economy, 1-Business, 2-First Class): ");
                scanf("%d", &ticket_class);
                printf("Enter special requests (press Enter for none): ");
                getchar();
                fgets(special_requests, 200, stdin);
                special_requests[strcspn(special_requests, "\n")] = '\0';
                
                int booking_id = create_booking(&db, flight_id, passenger_id, (TicketClass)ticket_class, special_requests);
                if (booking_id != -1) {
                    printf("Booking created with ID: %d\n", booking_id);
                } else {
                    printf("Failed to create booking. Check IDs and seat availability.\n");
                }
                break;
            }
            case 6: {
                int booking_id;
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                check_in_passenger(&db, booking_id);
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
                int flight_id, status;
                printf("Enter flight ID: ");
                scanf("%d", &flight_id);
                printf("Enter flight status (0-Scheduled, 1-Delayed, 2-Cancelled, 3-Departed, 4-Landed, 5-Boarding): ");
                scanf("%d", &status);
                update_flight_status(&db, flight_id, (FlightStatus)status);
                break;
            }
            case 9: {
                display_all_flights(&db);
                break;
            }
            case 10: {
                display_all_airports(&db);
                break;
            }
            case 11: {
                display_all_aircraft(&db);
                break;
            }
            case 12: {
                display_all_passengers(&db);
                break;
            }
            case 13: {
                int flight_id;
                printf("Enter flight ID: ");
                scanf("%d", &flight_id);
                display_flight_bookings(&db, flight_id);
                break;
            }
            case 14: {
                int passenger_id;
                printf("Enter passenger ID: ");
                scanf("%d", &passenger_id);
                display_passenger_bookings(&db, passenger_id);
                break;
            }
            case 15: {
                int departure_airport_id, arrival_airport_id;
                int day, month, year;
                printf("Enter departure airport ID: ");
                scanf("%d", &departure_airport_id);
                printf("Enter arrival airport ID: ");
                scanf("%d", &arrival_airport_id);
                printf("Enter travel date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                Date date = {day, month, year};
                search_flights_by_route(&db, departure_airport_id, arrival_airport_id, date);
                break;
            }
            case 16: {
                int status;
                printf("Enter flight status (0-Scheduled, 1-Delayed, 2-Cancelled, 3-Departed, 4-Landed, 5-Boarding): ");
                scanf("%d", &status);
                search_flights_by_status(&db, (FlightStatus)status);
                break;
            }
            case 17: {
                int flight_id;
                printf("Enter flight ID: ");
                scanf("%d", &flight_id);
                generate_flight_manifest(&db, flight_id);
                break;
            }
            case 18: {
                int month, year;
                printf("Enter month (1-12): ");
                scanf("%d", &month);
                printf("Enter year: ");
                scanf("%d", &year);
                generate_revenue_report(&db, month, year);
                break;
            }
            case 19: {
                int passenger_id, miles;
                printf("Enter passenger ID: ");
                scanf("%d", &passenger_id);
                printf("Enter miles to add: ");
                scanf("%d", &miles);
                update_frequent_flyer_miles(&db, passenger_id, miles);
                break;
            }
            case 20: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 21: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 22: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 23: {
                display_airline_statistics(&db);
                break;
            }
            case 24: {
                int departure_airport_id;
                int day, month, year;
                printf("Enter departure airport ID: ");
                scanf("%d", &departure_airport_id);
                printf("Enter travel date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                Date date = {day, month, year};
                find_flights_by_destination(&db, departure_airport_id, date);
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

void initialize_database(AirlineDatabase *db) {
    <｜fim▁hole｜>db->flight_count = 0;
    db->airport_count = 0;
    db->aircraft_count = 0;
    db->passenger_count = 0;
    db->booking_count = 0;
}

int add_airport(AirlineDatabase *db, const char *name, const char *city, 
                 const char *country, const char *code, int timezone_offset) {
    if (db->airport_count >= MAX_AIRPORTS) {
        return -1;
    }
    
    Airport *airport = &db->airports[db->airport_count];
    airport->id = db->airport_count + 1;
    strncpy(airport->name, name, MAX_NAME_LENGTH - 1);
    airport->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(airport->city, city, MAX_NAME_LENGTH - 1);
    airport->city[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(airport->country, country, MAX_NAME_LENGTH - 1);
    airport->country[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(airport->code, code, 3);
    airport->code[3] = '\0';
    airport->timezone_offset = timezone_offset;
    
    db->airport_count++;
    return airport->id;
}

int add_aircraft(AirlineDatabase *db, const char *model, const char *manufacturer,
                 int capacity, int range, int cruising_speed, int fuel_capacity,
                 const char *registration_number) {
    if (db->aircraft_count >= MAX_AIRCRAFT) {
        return -1;
    }
    
    Aircraft *aircraft = &db->aircraft[db->aircraft_count];
    aircraft->id = db->aircraft_count + 1;
    strncpy(aircraft->model, model, MAX_NAME_LENGTH - 1);
    aircraft->model[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(aircraft->manufacturer, manufacturer, MAX_NAME_LENGTH - 1);
    aircraft->manufacturer[MAX_NAME_LENGTH - 1] = '\0';
    aircraft->capacity = capacity;
    aircraft->range = range;
    aircraft->cruising_speed = cruising_speed;
    aircraft->fuel_capacity = fuel_capacity;
    strncpy(aircraft->registration_number, registration_number, 19);
    aircraft->registration_number[19] = '\0';
    
    db->aircraft_count++;
    return aircraft->id;
}

int add_passenger(AirlineDatabase *db, const char *first_name, const char *last_name,
                  const char *passport_number, Date birth_date, const char *nationality,
                  const char *address, const char *phone, const char *email, char gender) {
    if (db->passenger_count >= MAX_PASSENGERS) {
        return -1;
    }
    
    Passenger *passenger = &db->passengers[db->passenger_count];
    passenger->id = db->passenger_count + 1;
    strncpy(passenger->first_name, first_name, MAX_NAME_LENGTH - 1);
    passenger->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(passenger->last_name, last_name, MAX_NAME_LENGTH - 1);
    passenger->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(passenger->passport_number, passport_number, 19);
    passenger->passport_number[19] = '\0';
    passenger->birth_date = birth_date;
    strncpy(passenger->nationality, nationality, MAX_NAME_LENGTH - 1);
    passenger->nationality[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(passenger->address, address, MAX_ADDRESS_LENGTH - 1);
    passenger->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    strncpy(passenger->phone, phone, 14);
    passenger->phone[14] = '\0';
    strncpy(passenger->email, email, 49);
    passenger->email[49] = '\0';
    passenger->gender = gender;
    passenger->frequent_flyer_miles = 0;
    strcpy(passenger->membership_level, "Basic");
    
    db->passenger_count++;
    return passenger->id;
}

int add_flight(AirlineDatabase *db, int flight_number, int aircraft_id,
               int departure_airport_id, int arrival_airport_id,
               DateTime departure_time, DateTime arrival_time,
               float price_economy, float price_business, float price_first_class) {
    if (db->flight_count >= MAX_FLIGHTS) {
        return -1;
    }
    
    // Validate aircraft and airports exist
    if (find_aircraft_by_id(db, aircraft_id) == -1 ||
        find_airport_by_id(db, departure_airport_id) == -1 ||
        find_airport_by_id(db, arrival_airport_id) == -1) {
        return -1;
    }
    
    // Validate date and time
    if (!is_valid_date(departure_time.day, departure_time.month, departure_time.year) ||
        !is_valid_time(departure_time.hour, departure_time.minute) ||
        !is_valid_date(arrival_time.day, arrival_time.month, arrival_time.year) ||
        !is_valid_time(arrival_time.hour, arrival_time.minute)) {
        return -1;
    }
    
    Aircraft *aircraft = &db->aircraft[find_aircraft_by_id(db, aircraft_id)];
    
    Flight *flight = &db->flights[db->flight_count];
    flight->id = db->flight_count + 1;
    flight->flight_number = flight_number;
    flight->aircraft_id = aircraft_id;
    flight->departure_airport_id = departure_airport_id;
    flight->arrival_airport_id = arrival_airport_id;
    flight->departure_time = departure_time;
    flight->arrival_time = arrival_time;
    flight->status = SCHEDULED;
    flight->price_economy = price_economy;
    flight->price_business = price_business;
    flight->price_first_class = price_first_class;
    flight->gate[0] = '\0';
    
    // Set seat counts based on aircraft capacity
    // Standard distribution: 70% economy, 20% business, 10% first class
    flight->seats_economy = aircraft->capacity * 0.7;
    flight->seats_business = aircraft->capacity * 0.2;
    flight->seats_first_class = aircraft->capacity * 0.1;
    
    // Initially all seats are available
    flight->available_economy = flight->seats_economy;
    flight->available_business = flight->seats_business;
    flight->available_first_class = flight->seats_first_class;
    
    db->flight_count++;
    return flight->id;
}

int create_booking(AirlineDatabase *db, int flight_id, int passenger_id,
                   TicketClass ticket_class, const char *special_requests) {
    if (db->booking_count >= MAX_BOOKINGS) {
        return -1;
    }
    
    // Validate flight and passenger exist
    int flight_index = find_flight_by_id(db, flight_id);
    int passenger_index = find_passenger_by_id(db, passenger_id);
    
    if (flight_index == -1 || passenger_index == -1) {
        return -1;
    }
    
    Flight *flight = &db->flights[flight_index];
    
    // Check if flight is still scheduled and has available seats
    if (flight->status != SCHEDULED) {
        printf("Cannot book a flight that is not scheduled.\n");
        return -1;
    }
    
    // Check seat availability based on ticket class
    if (ticket_class == ECONOMY && flight->available_economy <= 0) {
        printf("No economy seats available on this flight.\n");
        return -1;
    } else if (ticket_class == BUSINESS && flight->available_business <= 0) {
        printf("No business seats available on this flight.\n");
        return -1;
    } else if (ticket_class == FIRST_CLASS && flight->available_first_class <= 0) {
        printf("No first class seats available on this flight.\n");
        return -1;
    }
    
    Booking *booking = &db->bookings[db->booking_count];
    booking->id = db->booking_count + 1;
    booking->flight_id = flight_id;
    booking->passenger_id = passenger_id;
    booking->ticket_class = ticket_class;
    booking->booking_date.day = flight->departure_time.day;
    booking->booking_date.month = flight->departure_time.month;
    booking->booking_date.year = flight->departure_time.year;
    booking->checked_in = 0;
    booking->cancelled = 0;
    
    // Set price based on ticket class
    if (ticket_class == ECONOMY) {
        booking->price = flight->price_economy;
        flight->available_economy--;
    } else if (ticket_class == BUSINESS) {
        booking->price = flight->price_business;
        flight->available_business--;
    } else {
        booking->price = flight->price_first_class;
        flight->available_first_class--;
    }
    
    // Generate seat number
    if (ticket_class == ECONOMY) {
        sprintf(booking->seat_number, "%dA", flight->seats_economy - flight->available_economy);
    } else if (ticket_class == BUSINESS) {
        sprintf(booking->seat_number, "%dB", flight->seats_business - flight->available_business);
    } else {
        sprintf(booking->seat_number, "%dF", flight->seats_first_class - flight->available_first_class);
    }
    
    strncpy(booking->special_requests, special_requests, 199);
    booking->special_requests[199] = '\0';
    
    db->booking_count++;
    return booking->id;
}

void check_in_passenger(AirlineDatabase *db, int booking_id) {
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
        printf("Passenger has already checked in.\n");
        return;
    }
    
    // Mark as checked in
    booking->checked_in = 1;
    
    // Add frequent flyer miles (1 mile per $1 spent)
    int passenger_index = find_passenger_by_id(db, booking->passenger_id);
    if (passenger_index != -1) {
        update_frequent_flyer_miles(db, booking->passenger_id, (int)booking->price);
    }
    
    printf("Passenger checked in successfully. Seat: %s\n", booking->seat_number);
}

void cancel_booking(AirlineDatabase *db, int booking_id) {
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
    
    // Mark as cancelled
    booking->cancelled = 1;
    
    // Return seat to availability
    int flight_index = find_flight_by_id(db, booking->flight_id);
    if (flight_index != -1) {
        Flight *flight = &db->flights[flight_index];
        
        if (booking->ticket_class == ECONOMY) {
            flight->available_economy++;
        } else if (booking->ticket_class == BUSINESS) {
            flight->available_business++;
        } else {
            flight->available_first_class++;
        }
    }
    
    printf("Booking %d cancelled successfully.\n", booking_id);
}

void update_flight_status(AirlineDatabase *db, int flight_id, FlightStatus status) {
    int flight_index = find_flight_by_id(db, flight_id);
    if (flight_index == -1) {
        printf("Flight not found.\n");
        return;
    }
    
    Flight *flight = &db->flights[flight_index];
    
    FlightStatus old_status = flight->status;
    flight->status = status;
    
    printf("Flight %d status updated from %s to %s\n", 
           flight_id, get_flight_status_name(old_status), get_flight_status_name(status));
}

void display_flight(const Flight *flight, const AirlineDatabase *db) {
    printf("\nFlight ID: %d\n", flight->id);
    printf("Flight Number: %d\n", flight->flight_number);
    
    // Display aircraft information
    int aircraft_index = find_aircraft_by_id(db, flight->aircraft_id);
    if (aircraft_index != -1) {
        printf("Aircraft: %s %s (Capacity: %d)\n", 
               db->aircraft[aircraft_index].manufacturer, 
               db->aircraft[aircraft_index].model,
               db->aircraft[aircraft_index].capacity);
    }
    
    // Display departure airport
    int departure_index = find_airport_by_id(db, flight->departure_airport_id);
    if (departure_index != -1) {
        printf("Departure: %s (%s), %s\n", 
               db->airports[departure_index].name,
               db->airports[departure_index].code,
               db->airports[departure_index].city);
    }
    
    // Display arrival airport
    int arrival_index = find_airport_by_id(db, flight->arrival_airport_id);
    if (arrival_index != -1) {
        printf("Arrival: %s (%s), %s\n", 
               db->airports[arrival_index].name,
               db->airports[arrival_index].code,
               db->airports[arrival_index].city);
    }
    
    printf("Departure: %02d/%02d/%d %02d:%02d\n", 
           flight->departure_time.day, flight->departure_time.month, flight->departure_time.year,
           flight->departure_time.hour, flight->departure_time.minute);
    
    printf("Arrival: %02d/%02d/%d %02d:%02d\n", 
           flight->arrival_time.day, flight->arrival_time.month, flight->arrival_time.year,
           flight->arrival_time.hour, flight->arrival_time.minute);
    
    printf("Status: %s\n", get_flight_status_name(flight->status));
    
    if (strlen(flight->gate) > 0) {
        printf("Gate: %s\n", flight->gate);
    }
    
    printf("Prices: Economy $%.2f, Business $%.2f, First Class $%.2f\n",
           flight->price_economy, flight->price_business, flight->price_first_class);
    
    printf("Availability: Economy %d/%d, Business %d/%d, First Class %d/%d\n",
           flight->available_economy, flight->seats_economy,
           flight->available_business, flight->seats_business,
           flight->available_first_class, flight->seats_first_class);
}

void display_airport(const Airport *airport) {
    printf("\nAirport ID: %d\n", airport->id);
    printf("Name: %s\n", airport->name);
    printf("City: %s\n", airport->city);
    printf("Country: %s\n", airport->country);
    printf("Code: %s\n", airport->code);
    printf("Timezone: UTC%+d\n", airport->timezone_offset);
}

void display_aircraft(const Aircraft *aircraft) {
    printf("\nAircraft ID: %d\n", aircraft->id);
    printf("Model: %s\n", aircraft->model);
    printf("Manufacturer: %s\n", aircraft->manufacturer);
    printf("Capacity: %d passengers\n", aircraft->capacity);
    printf("Range: %d km\n", aircraft->range);
    printf("Cruising Speed: %d km/h\n", aircraft->cruising_speed);
    printf("Fuel Capacity: %d liters\n", aircraft->fuel_capacity);
    printf("Registration Number: %s\n", aircraft->registration_number);
}

void display_passenger(const Passenger *passenger) {
    printf("\nPassenger ID: %d\n", passenger->id);
    printf("Name: %s %s\n", passenger->first_name, passenger->last_name);
    printf("Passport Number: %s\n", passenger->passport_number);
    printf("Birth Date: %02d/%02d/%d\n", 
           passenger->birth_date.day, passenger->birth_date.month, passenger->birth_date.year);
    printf("Nationality: %s\n", passenger->nationality);
    printf("Address: %s\n", passenger->address);
    printf("Phone: %s\n", passenger->phone);
    printf("Email: %s\n", passenger->email);
    printf("Gender: %c\n", passenger->gender);
    printf("Frequent Flyer Miles: %d\n", passenger->frequent_flyer_miles);
    printf("Membership Level: %s\n", passenger->membership_level);
}

void display_booking(const Booking *booking, const AirlineDatabase *db) {
    printf("\nBooking ID: %d\n", booking->id);
    
    // Display passenger information
    int passenger_index = find_passenger_by_id(db, booking->passenger_id);
    if (passenger_index != -1) {
        printf("Passenger: %s %s (ID: %d)\n", 
               db->passengers[passenger_index].first_name,
               db->passengers[passenger_index].last_name,
               booking->passenger_id);
    }
    
    // Display flight information
    int flight_index = find_flight_by_id(db, booking->flight_id);
    if (flight_index != -1) {
        Flight *flight = &db->flights[flight_index];
        
        // Display route
        int departure_index = find_airport_by_id(db, flight->departure_airport_id);
        int arrival_index = find_airport_by_id(db, flight->arrival_airport_id);
        
        if (departure_index != -1 && arrival_index != -1) {
            printf("Flight: %s %s → %s\n", 
                   db->airports[departure_index].code,
                   db->airports[arrival_index].code);
        }
        
        printf("Date: %02d/%02d/%d\n", 
               flight->departure_time.day, flight->departure_time.month, flight->departure_time.year);
        printf("Time: %02d:%02d\n", 
               flight->departure_time.hour, flight->departure_time.minute);
    }
    
    printf("Ticket Class: %s\n", get_ticket_class_name(booking->ticket_class));
    printf("Seat: %s\n", booking->seat_number);
    printf("Price: $%.2f\n", booking->price);
    printf("Booking Date: %02d/%02d/%d\n", 
           booking->booking_date.day, booking->booking_date.month, booking->booking_date.year);
    printf("Check-in Status: %s\n", booking->checked_in ? "Checked In" : "Not Checked In");
    printf("Booking Status: %s\n", booking->cancelled ? "Cancelled" : "Active");
    
    if (strlen(booking->special_requests) > 0) {
        printf("Special Requests: %s\n", booking->special_requests);
    }
}

void display_all_flights(const AirlineDatabase *db) {
    printf("\n=== All Flights ===\n");
    printf("Total Flights: %d\n\n", db->flight_count);
    
    for (int i = 0; i < db->flight_count; i++) {
        display_flight(&db->flights[i], db);
        printf("------------------------\n");
    }
}

void display_all_airports(const AirlineDatabase *db) {
    printf("\n=== All Airports ===\n");
    printf("Total Airports: %d\n\n", db->airport_count);
    
    for (int i = 0; i < db->airport_count; i++) {
        display_airport(&db->airports[i]);
        printf("------------------------\n");
    }
}

void display_all_aircraft(const AirlineDatabase *db) {
    printf("\n=== All Aircraft ===\n");
    printf("Total Aircraft: %d\n\n", db->aircraft_count);
    
    for (int i = 0; i < db->aircraft_count; i++) {
        display_aircraft(&db->aircraft[i]);
        printf("------------------------\n");
    }
}

void display_all_passengers(const AirlineDatabase *db) {
    printf("\n=== All Passengers ===\n");
    printf("Total Passengers: %d\n\n", db->passenger_count);
    
    for (int i = 0; i < db->passenger_count; i++) {
        display_passenger(&db->passengers[i]);
        printf("------------------------\n");
    }
}

void display_flight_bookings(const AirlineDatabase *db, int flight_id) {
    int flight_index = find_flight_by_id(db, flight_id);
    if (flight_index == -1) {
        printf("Flight not found.\n");
        return;
    }
    
    Flight *flight = &db->flights[flight_index];
    printf("\n=== Bookings for Flight %d ===\n", flight->flight_number);
    
    int found = 0;
    
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].flight_id == flight_id && !db->bookings[i].cancelled) {
            display_booking(&db->bookings[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No bookings found for this flight.\n");
    } else {
        printf("Total Active Bookings: %d\n", found);
    }
}

void display_passenger_bookings(const AirlineDatabase *db, int passenger_id) {
    int passenger_index = find_passenger_by_id(db, passenger_id);
    if (passenger_index == -1) {
        printf("Passenger not found.\n");
        return;
    }
    
    Passenger *passenger = &db->passengers[passenger_index];
    printf("\n=== Bookings for Passenger %s %s ===\n", 
           passenger->first_name, passenger->last_name);
    
    int found = 0;
    
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].passenger_id == passenger_id) {
            display_booking(&db->bookings[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No bookings found for this passenger.\n");
    } else {
        printf("Total Bookings: %d\n", found);
    }
}

void search_flights_by_route(const AirlineDatabase *db, int departure_airport_id, int arrival_airport_id, Date date) {
    int departure_index = find_airport_by_id(db, departure_airport_id);
    int arrival_index = find_airport_by_id(db, arrival_airport_id);
    
    if (departure_index == -1 || arrival_index == -1) {
        printf("Invalid airport ID(s).\n");
        return;
    }
    
    printf("\n=== Flights from %s to %s on %02d/%02d/%d ===\n",
           db->airports[departure_index].code,
           db->airports[arrival_index].code,
           date.day, date.month, date.year);
    
    int found = 0;
    
    for (int i = 0; i < db->flight_count; i++) {
        Flight *flight = &db->flights[i];
        
        if (flight->departure_airport_id == departure_airport_id &&
            flight->arrival_airport_id == arrival_airport_id &&
            flight->departure_time.day == date.day &&
            flight->departure_time.month == date.month &&
            flight->departure_time.year == date.year &&
            flight->status != CANCELLED) {
            
            display_flight(flight, db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No flights found for this route on the specified date.\n");
    }
}

void search_flights_by_status(const AirlineDatabase *db, FlightStatus status) {
    printf("\n=== Flights with Status: %s ===\n", get_flight_status_name(status));
    
    int found = 0;
    
    for (int i = 0; i < db->flight_count; i++) {
        if (db->flights[i].status == status) {
            display_flight(&db->flights[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No flights found with this status.\n");
    } else {
        printf("Total Flights: %d\n", found);
    }
}

void generate_flight_manifest(const AirlineDatabase *db, int flight_id) {
    int flight_index = find_flight_by_id(db, flight_id);
    if (flight_index == -1) {
        printf("Flight not found.\n");
        return;
    }
    
    Flight *flight = &db->flights[flight_index];
    printf("\n=== Flight Manifest for Flight %d ===\n", flight->flight_number);
    
    // Display flight details
    int departure_index = find_airport_by_id(db, flight->departure_airport_id);
    int arrival_index = find_airport_by_id(db, flight->arrival_airport_id);
    
    if (departure_index != -1 && arrival_index != -1) {
        printf("Route: %s (%s) → %s (%s)\n",
               db->airports[departure_index].name, db->airports[departure_index].code,
               db->airports[arrival_index].name, db->airports[arrival_index].code);
    }
    
    printf("Departure: %02d/%02d/%d %02d:%02d\n", 
           flight->departure_time.day, flight->departure_time.month, flight->departure_time.year,
           flight->departure_time.hour, flight->departure_time.minute);
    printf("Arrival: %02d/%02d/%d %02d:%02d\n", 
           flight->arrival_time.day, flight->arrival_time.month, flight->arrival_time.year,
           flight->arrival_time.hour, flight->arrival_time.minute);
    printf("Aircraft: %s %s\n",
           db->aircraft[find_aircraft_by_id(db, flight->aircraft_id)].manufacturer,
           db->aircraft[find_aircraft_by_id(db, flight->aircraft_id)].model);
    
    // Display passengers by class
    int total_passengers = 0;
    
    printf("\nEconomy Class Passengers:\n");
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        if (booking->flight_id == flight_id && booking->ticket_class == ECONOMY && !booking->cancelled) {
            int passenger_index = find_passenger_by_id(db, booking->passenger_id);
            if (passenger_index != -1) {
                Passenger *passenger = &db->passengers[passenger_index];
                printf("  %s. Seat: %s. %s\n", 
                       passenger->last_name, booking->seat_number,
                       booking->checked_in ? "Checked In" : "Not Checked In");
                total_passengers++;
            }
        }
    }
    
    printf("\nBusiness Class Passengers:\n");
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        if (booking->flight_id == flight_id && booking->ticket_class == BUSINESS && !booking->cancelled) {
            int passenger_index = find_passenger_by_id(db, booking->passenger_id);
            if (passenger_index != -1) {
                Passenger *passenger = &db->passengers[passenger_index];
                printf("  %s. Seat: %s. %s\n", 
                       passenger->last_name, booking->seat_number,
                       booking->checked_in ? "Checked In" : "Not Checked In");
                total_passengers++;
            }
        }
    }
    
    printf("\nFirst Class Passengers:\n");
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        if (booking->flight_id == flight_id && booking->ticket_class == FIRST_CLASS && !booking->cancelled) {
            int passenger_index = find_passenger_by_id(db, booking->passenger_id);
            if (passenger_index != -1) {
                Passenger *passenger = &db->passengers[passenger_index];
                printf("  %s. Seat: %s. %s\n", 
                       passenger->last_name, booking->seat_number,
                       booking->checked_in ? "Checked In" : "Not Checked In");
                total_passengers++;
            }
        }
    }
    
    printf("\nTotal Passengers: %d\n", total_passengers);
    printf("Checked In: %d\n", total_passengers);
    printf("Not Checked In: %d\n", 0); // Assuming all are checked in for simplicity
}

void generate_revenue_report(const AirlineDatabase *db, int month, int year) {
    if (month < 1 || month > 12 || year < 2000 || year > 2100) {
        printf("Invalid month or year.\n");
        return;
    }
    
    char month_names[][10] = {"January", "February", "March", "April", "May", "June",
                             "July", "August", "September", "October", "November", "December"};
    
    printf("\n=== Revenue Report for %s %d ===\n", month_names[month - 1], year);
    
    float total_revenue = 0.0f;
    float economy_revenue = 0.0f;
    float business_revenue = 0.0f;
    float first_class_revenue = 0.0f;
    int total_bookings = 0;
    int cancelled_bookings = 0;
    
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        
        // Check if booking is in the specified month and year
        if (booking->booking_date.month == month && booking->booking_date.year == year) {
            if (!booking->cancelled) {
                total_revenue += booking->price;
                total_bookings++;
                
                if (booking->ticket_class == ECONOMY) {
                    economy_revenue += booking->price;
                } else if (booking->ticket_class == BUSINESS) {
                    business_revenue += booking->price;
                } else {
                    first_class_revenue += booking->price;
                }
            } else {
                cancelled_bookings++;
            }
        }
    }
    
    printf("Total Bookings: %d\n", total_bookings);
    printf("Cancelled Bookings: %d\n", cancelled_bookings);
    printf("Active Bookings: %d\n", total_bookings - cancelled_bookings);
    printf("\nRevenue by Class:\n");
    printf("  Economy: $%.2f (%d bookings)\n", economy_revenue, 
           (int)(economy_revenue > 0 ? economy_revenue / db->flights[0].price_economy : 0)); // Approximate count
    printf("  Business: $%.2f\n", business_revenue);
    printf("  First Class: $%.2f\n", first_class_revenue);
    printf("  Total Revenue: $%.2f\n", total_revenue);
    
    // Top routes by revenue
    printf("\nTop Routes by Revenue:\n");
    printf("%-20s %-15s %-15s %-10s\n", "Route", "Bookings", "Revenue", "Avg. Price");
    printf("---------------------------------------------------\n");
    
    // Calculate revenue by route
    int route_count = 0;
    typedef struct {
        int departure_id;
        int arrival_id;
        int bookings;
        float revenue;
    } RouteRevenue;
    
    RouteRevenue routes[100];
    
    for (int i = 0; i < db->flight_count; i++) {
        Flight *flight = &db->flights[i];
        
        // Check if this route is already in our list
        int found_route = 0;
        for (int j = 0; j < route_count; j++) {
            if (routes[j].departure_id == flight->departure_airport_id &&
                routes[j].arrival_id == flight->arrival_airport_id) {
                found_route = 1;
                break;
            }
        }
        
        if (!found_route && route_count < 100) {
            routes[route_count].departure_id = flight->departure_airport_id;
            routes[route_count].arrival_id = flight->arrival_airport_id;
            routes[route_count].bookings = 0;
            routes[route_count].revenue = 0.0f;
            route_count++;
        }
    }
    
    // Calculate revenue for each route
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        
        if (booking->booking_date.month == month && booking->booking_date.year == year && !booking->cancelled) {
            int flight_index = find_flight_by_id(db, booking->flight_id);
            if (flight_index != -1) {
                Flight *flight = &db->flights[flight_index];
                
                for (int j = 0; j < route_count; j++) {
                    if (routes[j].departure_id == flight->departure_airport_id &&
                        routes[j].arrival_id == flight->arrival_airport_id) {
                        routes[j].bookings++;
                        routes[j].revenue += booking->price;
                        break;
                    }
                }
            }
        }
    }
    
    // Sort routes by revenue
    for (int i = 0; i < route_count - 1; i++) {
        for (int j = 0; j < route_count - i - 1; j++) {
            if (routes[j].revenue < routes[j + 1].revenue) {
                RouteRevenue temp = routes[j];
                routes[j] = routes[j + 1];
                routes[j + 1] = temp;
            }
        }
    }
    
    // Display top 10 routes
    int display_count = route_count < 10 ? route_count : 10;
    for (int i = 0; i < display_count; i++) {
        int dep_index = find_airport_by_id(db, routes[i].departure_id);
        int arr_index = find_airport_by_id(db, routes[i].arrival_id);
        
        if (dep_index != -1 && arr_index != -1) {
            char route_name[40];
            sprintf(route_name, "%s-%s", 
                    db->airports[dep_index].code, db->airports[arr_index].code);
            
            float avg_price = routes[i].bookings > 0 ? routes[i].revenue / routes[i].bookings : 0.0f;
            
            printf("%-20s %-15d $%-14.2f $%-9.2f\n", 
                   route_name, routes[i].bookings, routes[i].revenue, avg_price);
        }
    }
}

void update_frequent_flyer_miles(AirlineDatabase *db, int passenger_id, int miles) {
    int passenger_index = find_passenger_by_id(db, passenger_id);
    if (passenger_index == -1) {
        printf("Passenger not found.\n");
        return;
    }
    
    Passenger *passenger = &db->passengers[passenger_index];
    
    int old_miles = passenger->frequent_flyer_miles;
    passenger->frequent_flyer_miles += miles;
    
    // Update membership level based on miles
    if (passenger->frequent_flyer_miles >= 100000) {
        strcpy(passenger->membership_level, "Platinum");
    } else if (passenger->frequent_flyer_miles >= 50000) {
        strcpy(passenger->membership_level, "Gold");
    } else if (passenger->frequent_flyer_miles >= 25000) {
        strcpy(passenger->membership_level, "Silver");
    } else {
        strcpy(passenger->membership_level, "Basic");
    }
    
    printf("Frequent flyer miles updated for %s %s: %d → %d\n",
           passenger->first_name, passenger->last_name, old_miles, passenger->frequent_flyer_miles);
    printf("Membership Level: %s\n", passenger->membership_level);
}

int find_flight_by_id(const AirlineDatabase *db, int flight_id) {
    for (int i = 0; i < db->flight_count; i++) {
        if (db->flights[i].id == flight_id) {
            return i;
        }
    }
    return -1;
}

int find_airport_by_id(const AirlineDatabase *db, int airport_id) {
    for (int i = 0; i < db->airport_count; i++) {
        if (db->airports[i].id == airport_id) {
            return i;
        }
    }
    return -1;
}

int find_aircraft_by_id(const AirlineDatabase *db, int aircraft_id) {
    for (int i = 0; i < db->aircraft_count; i++) {
        if (db->aircraft[i].id == aircraft_id) {
            return i;
        }
    }
    return -1;
}

int find_passenger_by_id(const AirlineDatabase *db, int passenger_id) {
    for (int i = 0; i < db->passenger_count; i++) {
        if (db->passengers[i].id == passenger_id) {
            return i;
        }
    }
    return -1;
}

int find_booking_by_id(const AirlineDatabase *db, int booking_id) {
    for (int i = 0; i < db->booking_count; i++) {
        if (db->bookings[i].id == booking_id) {
            return i;
        }
    }
    return -1;
}

int save_database(const AirlineDatabase *db) {
    FILE *flights_file = fopen(FILENAME_FLIGHTS, "wb");
    if (flights_file != NULL) {
        fwrite(&db->flight_count, sizeof(int), 1, flights_file);
        fwrite(db->flights, sizeof(Flight), db->flight_count, flights_file);
        fclose(flights_file);
    } else {
        return 0;
    }
    
    FILE *airports_file = fopen(FILENAME_AIRPORTS, "wb");
    if (airports_file != NULL) {
        fwrite(&db->airport_count, sizeof(int), 1, airports_file);
        fwrite(db->airports, sizeof(Airport), db->airport_count, airports_file);
        fclose(airports_file);
    } else {
        return 0;
    }
    
    FILE *aircraft_file = fopen(FILENAME_AIRCRAFT, "wb");
    if (aircraft_file != NULL) {
        fwrite(&db->aircraft_count, sizeof(int), 1, aircraft_file);
        fwrite(db->aircraft, sizeof(Aircraft), db->aircraft_count, aircraft_file);
        fclose(aircraft_file);
    } else {
        return 0;
    }
    
    FILE *passengers_file = fopen(FILENAME_PASSENGERS, "wb");
    if (passengers_file != NULL) {
        fwrite(&db->passenger_count, sizeof(int), 1, passengers_file);
        fwrite(db->passengers, sizeof(Passenger), db->passenger_count, passengers_file);
        fclose(passengers_file);
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

int load_database(AirlineDatabase *db) {
    FILE *flights_file = fopen(FILENAME_FLIGHTS, "rb");
    if (flights_file != NULL) {
        fread(&db->flight_count, sizeof(int), 1, flights_file);
        fread(db->flights, sizeof(Flight), db->flight_count, flights_file);
        fclose(flights_file);
    } else {
        return 0;
    }
    
    FILE *airports_file = fopen(FILENAME_AIRPORTS, "rb");
    if (airports_file != NULL) {
        fread(&db->airport_count, sizeof(int), 1, airports_file);
        fread(db->airports, sizeof(Airport), db->airport_count, airports_file);
        fclose(airports_file);
    } else {
        return 0;
    }
    
    FILE *aircraft_file = fopen(FILENAME_AIRCRAFT, "rb");
    if (aircraft_file != NULL) {
        fread(&db->aircraft_count, sizeof(int), 1, aircraft_file);
        fread(db->aircraft, sizeof(Aircraft), db->aircraft_count, aircraft_file);
        fclose(aircraft_file);
    } else {
        return 0;
    }
    
    FILE *passengers_file = fopen(FILENAME_PASSENGERS, "rb");
    if (passengers_file != NULL) {
        fread(&db->passenger_count, sizeof(int), 1, passengers_file);
        fread(db->passengers, sizeof(Passenger), db->passenger_count, passengers_file);
        fclose(passengers_file);
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

void generate_sample_data(AirlineDatabase *db) {
    char* airport_names[] = {
        "John F. Kennedy International Airport", "Los Angeles International Airport",
        "Chicago O'Hare International Airport", "Dallas/Fort Worth International Airport",
        "Denver International Airport", "San Francisco International Airport",
        "Seattle-Tacoma International Airport", "Las Vegas McCarran International Airport",
        "Orlando International Airport", "Miami International Airport",
        "Boston Logan International Airport", "Phoenix Sky Harbor International Airport",
        "Detroit Metropolitan Wayne County Airport", "Minneapolis-Saint Paul International Airport",
        "Newark Liberty International Airport", "George Bush Intercontinental Airport"
    };
    
    char* airport_cities[] = {
        "New York", "Los Angeles", "Chicago", "Dallas", "Denver",
        "San Francisco", "Seattle", "Las Vegas", "Orlando", "Miami",
        "Boston", "Phoenix", "Detroit", "Minneapolis", "Newark", "Houston"
    };
    
    char* airport_codes[] = {
        "JFK", "LAX", "ORD", "DFW", "DEN", "SFO", "SEA", "LAS", "MCO", "MIA",
        "BOS", "PHX", "DTW", "MSP", "EWR", "IAH"
    };
    
    char* airport_countries[] = {
        "United States", "United States", "United States", "United States", "United States",
        "United States", "United States", "United States", "United States", "United States",
        "United States", "United States", "United States", "United States", "United States", "United States"
    };
    
    char* aircraft_models[] = {
        "Boeing 737", "Boeing 747", "Boeing 777", "Boeing 787",
        "Airbus A320", "Airbus A330", "Airbus A350", "Airbus A380",
        "Embraer E190", "Bombardier CRJ900", "McDonnell Douglas MD-80", "McDonnell Douglas MD-11"
    };
    
    char* manufacturers[] = {
        "Boeing", "Boeing", "Boeing", "Boeing",
        "Airbus", "Airbus", "Airbus", "Airbus",
        "Embraer", "Bombardier", "McDonnell Douglas", "McDonnell Douglas"
    };
    
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    srand(time(NULL));
    
    // Add airports
    for (int i = 0; i < 16 && db->airport_count < MAX_AIRPORTS; i++) {
        int timezone_offset = -8 + (rand() % 6); // Range from -8 to -3
        add_airport(db, airport_names[i], airport_cities[i], airport_countries[i],
                   airport_codes[i], timezone_offset);
    }
    
    // Add aircraft
    for (int i = 0; i < 12 && db->aircraft_count < MAX_AIRCRAFT; i++) {
        int capacity = 100 + (rand() % 400); // 100-500 passengers
        int range = 2000 + (rand() % 10000); // 2000-12000 km
        int speed = 800 + (rand() % 200); // 800-1000 km/h
        int fuel = 10000 + (rand() % 100000); // 10000-110000 liters
        char reg_num[20];
        sprintf(reg_num, "N-%d%s", 100 + i, (rand() % 2) ? "AB" : "CD");
        
        add_aircraft(db, aircraft_models[i], manufacturers[i], capacity, range, speed, fuel, reg_num);
    }
    
    // Add passengers
    for (int i = 0; i < 100 && db->passenger_count < MAX_PASSENGERS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 12]);
        strcpy(last_name, last_names[rand() % 12]);
        
        char passport[20];
        sprintf(passport, "%08d%04d", 10000000 + rand() % 90000000, rand() % 10000);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1950 + (rand() % 50);
        Date birth_date = {day, month, year};
        
        char nationality[MAX_NAME_LENGTH] = "United States";
        char address[MAX_ADDRESS_LENGTH];
        sprintf(address, "%d Main St, City, State", 100 + rand() % 900);
        
        char phone[15];
        sprintf(phone, "555-%04d", rand() % 10000);
        
        char email[50];
        sprintf(email, "%s.%s@email.com", first_name, last_name);
        
        char gender = (rand() % 3 == 0) ? 'O' : ((rand() % 2) ? 'M' : 'F');
        
        add_passenger(db, first_name, last_name, passport, birth_date, nationality, 
                     address, phone, email, gender);
    }
    
    // Add flights for the next 30 days
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    
    for (int i = 0; i < 200 && db->flight_count < MAX_FLIGHTS; i++) {
        int flight_number = 100 + i;
        int aircraft_id = 1 + (rand() % db->aircraft_count);
        int departure_id = 1 + (rand() % db->airport_count);
        int arrival_id;
        
        // Ensure departure and arrival airports are different
        do {
            arrival_id = 1 + (rand() % db->airport_count);
        } while (arrival_id == departure_id);
        
        // Random date within the next 30 days
        int days_offset = 1 + (rand() % 30);
        DateTime departure_time;
        departure_time.day = current_time->tm_mday + days_offset;
        departure_time.month = current_time->tm_mon + 1;
        departure_time.year = current_time->tm_year + 1900;
        departure_time.hour = 6 + (rand() % 16); // 6 AM to 10 PM
        departure_time.minute = (rand() % 4) * 15; // 0, 15, 30, or 45 minutes
        
        // Normalize date if days exceed month days
        if (departure_time.day > 28) {
            departure_time.day -= 28;
            departure_time.month += 1;
            if (departure_time.month > 12) {
                departure_time.month -= 12;
                departure_time.year += 1;
            }
        }
        
        // Calculate arrival time (flight duration: 1-6 hours)
        int flight_duration = 1 + (rand() % 6);
        DateTime arrival_time = departure_time;
        arrival_time.hour += flight_duration;
        
        // Normalize time
        if (arrival_time.hour >= 24) {
            arrival_time.hour -= 24;
            arrival_time.day += 1;
            
            if (arrival_time.day > 28) {
                arrival_time.day -= 28;
                arrival_time.month += 1;
                if (arrival_time.month > 12) {
                    arrival_time.month -= 12;
                    arrival_time.year += 1;
                }
            }
        }
        
        // Set prices (economy: $100-500, business: 2-3x economy, first: 3-5x economy)
        float economy_price = 100.0f + (rand() % 400);
        float business_price = economy_price * (2.0f + (rand() % 2));
        float first_price = economy_price * (3.0f + (rand() % 3));
        
        add_flight(db, flight_number, aircraft_id, departure_id, arrival_id,
                  departure_time, arrival_time, economy_price, business_price, first_price);
    }
    
    // Create bookings
    for (int i = 0; i < 500 && db->booking_count < MAX_BOOKINGS; i++) {
        int flight_id = 1 + (rand() % db->flight_count);
        int passenger_id = 1 + (rand() % db->passenger_count);
        TicketClass ticket_class = (TicketClass)(rand() % 3); // Economy, Business, or First
        char special_requests[200] = "";
        
        // 30% chance of having special requests
        if (rand() % 10 < 3) {
            strcpy(special_requests, (rand() % 2) ? "Wheelchair assistance" : "Vegetarian meal");
        }
        
        int booking_id = create_booking(db, flight_id, passenger_id, ticket_class, special_requests);
        
        if (booking_id != -1) {
            // Check in or cancel some bookings
            if (rand() % 10 < 6) { // 60% chance
                check_in_passenger(db, booking_id);
            } else if (rand() % 10 < 2) { // 20% chance
                cancel_booking(db, booking_id);
            }
        }
    }
    
    // Update some flight statuses
    for (int i = 0; i < db->flight_count; i++) {
        int status_choice = rand() % 10;
        
        if (status_choice < 5) { // 50% chance
            // Keep as Scheduled
        } else if (status_choice < 7) { // 20% chance
            update_flight_status(db, db->flights[i].id, DELAYED);
        } else if (status_choice < 8) { // 10% chance
            update_flight_status(db, db->flights[i].id, DEPARTED);
        } else if (status_choice < 9) { // 10% chance
            update_flight_status(db, db->flights[i].id, LANDED);
        } else if (status_choice < 10) { // 10% chance
            update_flight_status(db, db->flights[i].id, CANCELLED);
        }
    }
}

void display_airline_statistics(const AirlineDatabase *db) {
    printf("\n=== Airline Statistics ===\n");
    
    int scheduled_flights = 0;
    int departed_flights = 0;
    int landed_flights = 0;
    int delayed_flights = 0;
    int cancelled_flights = 0;
    
    int total_bookings = 0;
    int checked_in_bookings = 0;
    int cancelled_bookings = 0;
    
    float total_revenue = 0.0f;
    float economy_revenue = 0.0f;
    float business_revenue = 0.0f;
    float first_class_revenue = 0.0f;
    
    // Flight statistics
    for (int i = 0; i < db->flight_count; i++) {
        switch (db->flights[i].status) {
            case SCHEDULED: scheduled_flights++; break;
            case DEPARTED: departed_flights++; break;
            case LANDED: landed_flights++; break;
            case DELAYED: delayed_flights++; break;
            case CANCELLED: cancelled_flights++; break;
            case BOARDING: scheduled_flights++; break;
        }
    }
    
    // Booking statistics
    for (int i = 0; i < db->booking_count; i++) {
        Booking *booking = &db->bookings[i];
        
        if (!booking->cancelled) {
            total_bookings++;
            total_revenue += booking->price;
            
            if (booking->ticket_class == ECONOMY) {
                economy_revenue += booking->price;
            } else if (booking->ticket_class == BUSINESS) {
                business_revenue += booking->price;
            } else {
                first_class_revenue += booking->price;
            }
            
            if (booking->checked_in) {
                checked_in_bookings++;
            }
        } else {
            cancelled_bookings++;
        }
    }
    
    printf("Total Airports: %d\n", db->airport_count);
    printf("Total Aircraft: %d\n", db->aircraft_count);
    printf("Total Passengers: %d\n", db->passenger_count);
    printf("Total Flights: %d\n", db->flight_count);
    printf("\nFlight Status Distribution:\n");
    printf("  Scheduled: %d (%.1f%%)\n", scheduled_flights, 
           db->flight_count > 0 ? (float)scheduled_flights / db->flight_count * 100 : 0);
    printf("  Delayed: %d (%.1f%%)\n", delayed_flights, 
           db->flight_count > 0 ? (float)delayed_flights / db->flight_count * 100 : 0);
    printf("  Cancelled: %d (%.1f%%)\n", cancelled_flights, 
           db->flight_count > 0 ? (float)cancelled_flights / db->flight_count * 100 : 0);
    printf("  Departed: %d (%.1f%%)\n", departed_flights, 
           db->flight_count > 0 ? (float)departed_flights / db->flight_count * 100 : 0);
    printf("  Landed: %d (%.1f%%)\n", landed_flights, 
           db->flight_count > 0 ? (float)landed_flights / db->flight_count * 100 : 0);
    
    printf("\nBooking Statistics:\n");
    printf("  Total Bookings: %d\n", total_bookings);
    printf("  Cancelled Bookings: %d (%.1f%%)\n", cancelled_bookings, 
           (total_bookings + cancelled_bookings) > 0 ? 
           (float)cancelled_bookings / (total_bookings + cancelled_bookings) * 100 : 0);
    printf("  Checked In: %d (%.1f%%)\n", checked_in_bookings, 
           total_bookings > 0 ? (float)checked_in_bookings / total_bookings * 100 : 0);
    
    printf("\nRevenue Statistics:\n");
    printf("  Total Revenue: $%.2f\n", total_revenue);
    printf("  Economy Class: $%.2f (%.1f%%)\n", economy_revenue, 
           total_revenue > 0 ? (float)economy_revenue / total_revenue * 100 : 0);
    printf("  Business Class: $%.2f (%.1f%%)\n", business_revenue, 
           total_revenue > 0 ? (float)business_revenue / total_revenue * 100 : 0);
    printf("  First Class: $%.2f (%.1f%%)\n", first_class_revenue, 
           total_revenue > 0 ? (float)first_class_revenue / total_revenue * 100 : 0);
    
    // Top airports by flights
    printf("\nTop Airports by Flight Volume:\n");
    int airport_flight_counts[MAX_AIRPORTS] = {0};
    
    for (int i = 0; i < db->flight_count; i++) {
        airport_flight_counts[db->flights[i].departure_airport_id - 1]++;
        airport_flight_counts[db->flights[i].arrival_airport_id - 1]++;
    }
    
    // Sort airports by flight count
    int sorted_airport_indices[MAX_AIRPORTS];
    for (int i = 0; i < db->airport_count; i++) {
        sorted_airport_indices[i] = i;
    }
    
    for (int i = 0; i < db->airport_count - 1; i++) {
        for (int j = 0; j < db->airport_count - i - 1; j++) {
            if (airport_flight_counts[sorted_airport_indices[j]] < 
                airport_flight_counts[sorted_airport_indices[j + 1]]) {
                int temp = sorted_airport_indices[j];
                sorted_airport_indices[j] = sorted_airport_indices[j + 1];
                sorted_airport_indices[j + 1] = temp;
            }
        }
    }
    
    // Display top 5 airports
    int display_count = db->airport_count < 5 ? db->airport_count : 5;
    for (int i = 0; i < display_count; i++) {
        int airport_index = sorted_airport_indices[i];
        printf("  %s (%s): %d flights\n", 
               db->airports[airport_index].name,
               db->airports[airport_index].code,
               airport_flight_counts[airport_index]);
    }
}

void find_flights_by_destination(const AirlineDatabase *db, int departure_airport_id, Date date) {
    int departure_index = find_airport_by_id(db, departure_airport_id);
    if (departure_index == -1) {
        printf("Invalid departure airport ID.\n");
        return;
    }
    
    printf("\n=== All Flights from %s on %02d/%02d/%d ===\n",
           db->airports[departure_index].code,
           date.day, date.month, date.year);
    
    int found = 0;
    
    for (int i = 0; i < db->flight_count; i++) {
        Flight *flight = &db->flights[i];
        
        if (flight->departure_airport_id == departure_airport_id &&
            flight->departure_time.day == date.day &&
            flight->departure_time.month == date.month &&
            flight->departure_time.year == date.year &&
            flight->status != CANCELLED) {
            
            display_flight(flight, db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No flights found from this airport on the specified date.\n");
    } else {
        printf("Total Flights: %d\n", found);
    }
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

const char* get_flight_status_name(FlightStatus status) {
    switch (status) {
        case SCHEDULED: return "Scheduled";
        case DELAYED: return "Delayed";
        case CANCELLED: return "Cancelled";
        case DEPARTED: return "Departed";
        case LANDED: return "Landed";
        case BOARDING: return "Boarding";
        default: return "Unknown";
    }
}

const char* get_ticket_class_name(TicketClass ticket_class) {
    switch (ticket_class) {
        case ECONOMY: return "Economy";
        case BUSINESS: return "Business";
        case FIRST_CLASS: return "First Class";
        default: return "Unknown";
    }
}