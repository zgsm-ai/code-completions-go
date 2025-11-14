#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_FLIGHTS 500
#define MAX_PASSENGERS 2000
#define MAX_BOOKINGS 2000
#define MAX_AIRLINES 20
#define MAX_NAME_LENGTH 50
#define FILENAME_FLIGHTS "flights.dat"
#define FILENAME_PASSENGERS "passengers.dat"
#define FILENAME_BOOKINGS "bookings.dat"
#define FILENAME_AIRLINES "airlines.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char flight_number[20];
    char origin[50];
    char destination[50];
    Date departure_date;
    char departure_time[10]; // HH:MM
    Date arrival_date;
    char arrival_time[10]; // HH:MM
    char airline[50];
    int available_seats;
    double price;
    int active;
} Flight;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    Date birth_date;
    char gender[10]; // Male, Female, Other
    char passport_number[20];
    char nationality[30];
    int active;
} Passenger;

typedef struct {
    int id;
    int flight_id;
    int passenger_id;
    Date booking_date;
    int seat_number;
    double price_paid;
    char status[20]; // "Confirmed", "Cancelled", "Completed"
    int active;
} Booking;

typedef struct {
    int id;
    char name[50];
    char iata_code[5]; // 2-3 letter code
    char description[200];
    int active;
} Airline;

typedef struct {
    Flight flights[MAX_FLIGHTS];
    int flight_count;
    Passenger passengers[MAX_PASSENGERS];
    int passenger_count;
    Booking bookings[MAX_BOOKINGS];
    int booking_count;
    Airline airlines[MAX_AIRLINES];
    int airline_count;
} FlightBookingSystem;

// Function prototypes
void initialize_system(FlightBookingSystem *system);
int add_flight(FlightBookingSystem *system, const char *flight_number, const char *origin,
               const char *destination, Date departure_date, const char *departure_time,
               Date arrival_date, const char *arrival_time, const char *airline,
               int available_seats, double price);
int add_passenger(FlightBookingSystem *system, const char *first_name, const char *last_name,
                   const char *email, const char *phone, Date birth_date,
                   const char *gender, const char *passport_number,
                   const char *nationality);
int add_airline(FlightBookingSystem *system, const char *name, const char *iata_code,
                 const char *description);
int book_flight(FlightBookingSystem *system, int flight_id, int passenger_id,
               int seat_number);
void cancel_booking(FlightBookingSystem *system, int booking_id);
void update_booking_status(FlightBookingSystem *system, int booking_id, const char *status);
void display_flight(const Flight *flight);
void display_passenger(const Passenger *passenger);
void display_booking(const Booking *booking, const FlightBookingSystem *system);
void display_airline(const Airline *airline);
void display_all_flights(const FlightBookingSystem *system);
void display_all_passengers(const FlightBookingSystem *system);
void display_all_bookings(const FlightBookingSystem *system);
void display_all_airlines(const FlightBookingSystem *system);
void search_flights_by_destination(const FlightBookingSystem *system, const char *destination);
void search_flights_by_date(const FlightBookingSystem *system, Date date);
void search_flights_by_airline(const FlightBookingSystem *system, const char *airline);
void search_passengers_by_name(const FlightBookingSystem *system, const char *name);
void search_bookings_by_passenger(const FlightBookingSystem *system, int passenger_id);
void generate_passenger_report(const FlightBookingSystem *system, int passenger_id);
void generate_flight_report(const FlightBookingSystem *system, int flight_id);
void generate_booking_report(const FlightBookingSystem *system, const Date *start_date,
                           const Date *end_date);
void generate_revenue_report(const FlightBookingSystem *system);
int find_flight_by_id(const FlightBookingSystem *system, int flight_id);
int find_passenger_by_id(const FlightBookingSystem *system, int passenger_id);
int find_booking_by_id(const FlightBookingSystem *system, int booking_id);
int find_airline_by_id(const FlightBookingSystem *system, int airline_id);
int save_system(const FlightBookingSystem *system);
int load_system(FlightBookingSystem *system);
void generate_sample_data(FlightBookingSystem *system);
void check_flight_availability(FlightBookingSystem *system, int flight_id);
int is_valid_email(const char *email);
int is_valid_phone(const char *phone);
int is_valid_date(int day, int month, int year);
const char* get_booking_status_description(const char *status);

int main() {
    FlightBookingSystem system;
    initialize_system(&system);
    
    printf("Flight Booking System\n");
    printf("1. Add Flight\n");
    printf("2. Add Passenger\n");
    printf("3. Add Airline\n");
    printf("4. Book Flight\n");
    printf("5. Cancel Booking\n");
    printf("6. Update Booking Status\n");
    printf("7. Display All Flights\n");
    printf("8. Display All Passengers\n");
    printf("9. Display All Bookings\n");
    printf("10. Display All Airlines\n");
    printf("11. Search Flights by Destination\n");
    printf("12. Search Flights by Date\n");
    printf("13. Search Flights by Airline\n");
    printf("14. Search Passengers by Name\n");
    printf("15. Search Bookings by Passenger\n");
    printf("16. Generate Passenger Report\n");
    printf("17. Generate Flight Report\n");
    printf("18. Generate Booking Report\n");
    printf("19. Generate Revenue Report\n");
    printf("20. Check Flight Availability\n");
    printf("21. Save System\n");
    printf("22. Load System\n");
    printf("23. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char flight_number[20], origin[50], destination[50];
                char departure_time[10], arrival_time[10], airline[50];
                int dep_day, dep_month, dep_year;
                int arr_day, arr_month, arr_year;
                int available_seats;
                double price;
                
                printf("Enter flight number: ");
                scanf("%s", flight_number);
                printf("Enter origin: ");
                scanf(" %[^\n]", origin);
                printf("Enter destination: ");
                scanf(" %[^\n]", destination);
                printf("Enter departure date (DD MM YYYY): ");
                scanf("%d %d %d", &dep_day, &dep_month, &dep_year);
                printf("Enter departure time (HH:MM): ");
                scanf("%s", departure_time);
                printf("Enter arrival date (DD MM YYYY): ");
                scanf("%d %d %d", &arr_day, &arr_month, &arr_year);
                printf("Enter arrival time (HH:MM): ");
                scanf("%s", arrival_time);
                printf("Enter airline: ");
                scanf(" %[^\n]", airline);
                printf("Enter available seats: ");
                scanf("%d", &available_seats);
                printf("Enter price: ");
                scanf("%lf", &price);
                
                if (!is_valid_date(dep_day, dep_month, dep_year) || 
                    !is_valid_date(arr_day, arr_month, arr_year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                Date departure_date = {dep_day, dep_month, dep_year};
                Date arrival_date = {arr_day, arr_month, arr_year};
                
                int flight_id = add_flight(&system, flight_number, origin, destination,
                                       departure_date, departure_time, arrival_date,
                                       arrival_time, airline, available_seats, price);
                if (flight_id != -1) {
                    printf("Flight added with ID: %d\n", flight_id);
                } else {
                    printf("Failed to add flight. System is full.\n");
                }
                break;
            }
            case 2: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], gender[10];
                char passport_number[20], nationality[30];
                int day, month, year;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter gender (Male, Female, Other): ");
                scanf(" %[^\n]", gender);
                printf("Enter passport number: ");
                scanf(" %[^\n]", passport_number);
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
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                if (strcmp(gender, "Male") != 0 && strcmp(gender, "Female") != 0 && strcmp(gender, "Other") != 0) {
                    printf("Invalid gender. Must be one of: Male, Female, Other\n");
                    break;
                }
                
                Date birth_date = {day, month, year};
                
                int passenger_id = add_passenger(&system, first_name, last_name, email, phone,
                                             birth_date, gender, passport_number, nationality);
                if (passenger_id != -1) {
                    printf("Passenger added with ID: %d\n", passenger_id);
                } else {
                    printf("Failed to add passenger. System is full.\n");
                }
                break;
            }
            case 3: {
                char name[50], iata_code[5], description[200];
                
                printf("Enter airline name: ");
                scanf(" %[^\n]", name);
                printf("Enter IATA code: ");
                scanf("%s", iata_code);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                
                int airline_id = add_airline(&system, name, iata_code, description);
                if (airline_id != -1) {
                    printf("Airline added with ID: %d\n", airline_id);
                } else {
                    printf("Failed to add airline. System is full.\n");
                }
                break;
            }
            case 4: {
                int flight_id, passenger_id, seat_number;
                
                printf("Enter flight ID: ");
                scanf("%d", &flight_id);
                printf("Enter passenger ID: ");
                scanf("%d", &passenger_id);
                printf("Enter seat number: ");
                scanf("%d", &seat_number);
                
                if (book_flight(&system, flight_id, passenger_id, seat_number)) {
                    printf("Flight booked successfully.\n");
                } else {
                    printf("Failed to book flight. Check IDs and seat availability.\n");
                }
                break;
            }
            case 5: {
                int booking_id;
                
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                
                cancel_booking(&system, booking_id);
                break;
            }
            case 6: {
                int booking_id;
                char status[20];
                
                printf("Enter booking ID: ");
                scanf("%d", &booking_id);
                printf("Enter new status (Confirmed, Cancelled, Completed): ");
                scanf(" %[^\n]", status);
                
                if (strcmp(status, "Confirmed") != 0 && strcmp(status, "Cancelled") != 0 &&
                    strcmp(status, "Completed") != 0) {
                    printf("Invalid status. Must be one of: Confirmed, Cancelled, Completed\n");
                    break;
                }
                
                update_booking_status(&system, booking_id, status);
                break;
            }
            case 7: {
                display_all_flights(&system);
                break;
            }
            case 8: {
                display_all_passengers(&system);
                break;
            }
            case 9: {
                display_all_bookings(&system);
                break;
            }
            case 10: {
                display_all_airlines(&system);
                break;
            }
            case 11: {
                char destination[50];
                printf("Enter destination to search: ");
                scanf(" %[^\n]", destination);
                search_flights_by_destination(&system, destination);
                break;
            }
            case 12: {
                int day, month, year;
                
                printf("Enter date to search (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                Date date = {day, month, year};
                search_flights_by_date(&system, date);
                break;
            }
            case 13: {
                char airline[50];
                printf("Enter airline to search: ");
                scanf(" %[^\n]", airline);
                search_flights_by_airline(&system, airline);
                break;
            }
            case 14: {
                char name[MAX_NAME_LENGTH];
                printf("Enter passenger name to search: ");
                scanf(" %[^\n]", name);
                search_passengers_by_name(&system, name);
                break;
            }
            case 15: {
                int passenger_id;
                printf("Enter passenger ID: ");
                scanf("%d", &passenger_id);
                search_bookings_by_passenger(&system, passenger_id);
                break;
            }
            case 16: {
                int passenger_id;
                printf("Enter passenger ID: ");
                scanf("%d", &passenger_id);
                generate_passenger_report(&system, passenger_id);
                break;
            }
            case 17: {
                int flight_id;
                printf("Enter flight ID: ");
                scanf("%d", &flight_id);
                generate_flight_report(&system, flight_id);
                break;
            }
            case 18: {
                int start_day, start_month, start_year;
                int end_day, end_month, end_year;
                
                printf("Enter start date (DD MM YYYY): ");
                scanf("%d %d %d", &start_day, &start_month, &start_year);
                printf("Enter end date (DD MM YYYY): ");
                scanf("%d %d %d", &end_day, &end_month, &end_year);
                
                if (!is_valid_date(start_day, start_month, start_year) || 
                    !is_valid_date(end_day, end_month, end_year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                Date start_date = {start_day, start_month, start_year};
                Date end_date = {end_day, end_month, end_year};
                
                generate_booking_report(&system, &start_date, &end_date);
                break;
            }
            case 19: {
                generate_revenue_report(&system);
                break;
            }
            case 20: {
                int flight_id;
                printf("Enter flight ID: ");
                scanf("%d", &flight_id);
                check_flight_availability(&system, flight_id);
                break;
            }
            case 21: {
                if (save_system(&system)) {
                    printf("System saved successfully.\n");
                } else {
                    printf("Failed to save system.\n");
                }
                break;
            }
            case 22: {
                if (load_system(&system)) {
                    printf("System loaded successfully.\n");
                } else {
                    printf("Failed to load system.\n");
                }
                break;
            }
            case 23: {
                generate_sample_data(&system);
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

void initialize_system(FlightBookingSystem *system) {
    <｜fim▁hole｜>system->flight_count = 0;
    system->passenger_count = 0;
    system->booking_count = 0;
    system->airline_count = 0;
}

int add_flight(FlightBookingSystem *system, const char *flight_number, const char *origin,
               const char *destination, Date departure_date, const char *departure_time,
               Date arrival_date, const char *arrival_time, const char *airline,
               int available_seats, double price) {
    if (system->flight_count >= MAX_FLIGHTS) {
        return -1;
    }
    
    Flight *flight = &system->flights[system->flight_count];
    flight->id = system->flight_count + 1;
    strncpy(flight->flight_number, flight_number, 19);
    flight->flight_number[19] = '\0';
    strncpy(flight->origin, origin, 49);
    flight->origin[49] = '\0';
    strncpy(flight->destination, destination, 49);
    flight->destination[49] = '\0';
    flight->departure_date = departure_date;
    strncpy(flight->departure_time, departure_time, 9);
    flight->departure_time[9] = '\0';
    flight->arrival_date = arrival_date;
    strncpy(flight->arrival_time, arrival_time, 9);
    flight->arrival_time[9] = '\0';
    strncpy(flight->airline, airline, 49);
    flight->airline[49] = '\0';
    flight->available_seats = available_seats;
    flight->price = price;
    flight->active = 1;
    
    system->flight_count++;
    return flight->id;
}

int add_passenger(FlightBookingSystem *system, const char *first_name, const char *last_name,
                   const char *email, const char *phone, Date birth_date,
                   const char *gender, const char *passport_number,
                   const char *nationality) {
    if (system->passenger_count >= MAX_PASSENGERS) {
        return -1;
    }
    
    Passenger *passenger = &system->passengers[system->passenger_count];
    passenger->id = system->passenger_count + 1;
    strncpy(passenger->first_name, first_name, MAX_NAME_LENGTH - 1);
    passenger->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(passenger->last_name, last_name, MAX_NAME_LENGTH - 1);
    passenger->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(passenger->email, email, 49);
    passenger->email[49] = '\0';
    strncpy(passenger->phone, phone, 14);
    passenger->phone[14] = '\0';
    passenger->birth_date = birth_date;
    strncpy(passenger->gender, gender, 9);
    passenger->gender[9] = '\0';
    strncpy(passenger->passport_number, passport_number, 19);
    passenger->passport_number[19] = '\0';
    strncpy(passenger->nationality, nationality, 29);
    passenger->nationality[29] = '\0';
    passenger->active = 1;
    
    system->passenger_count++;
    return passenger->id;
}

int add_airline(FlightBookingSystem *system, const char *name, const char *iata_code,
                 const char *description) {
    if (system->airline_count >= MAX_AIRLINES) {
        return -1;
    }
    
    Airline *airline = &system->airlines[system->airline_count];
    airline->id = system->airline_count + 1;
    strncpy(airline->name, name, 49);
    airline->name[49] = '\0';
    strncpy(airline->iata_code, iata_code, 4);
    airline->iata_code[4] = '\0';
    strncpy(airline->description, description, 199);
    airline->description[199] = '\0';
    airline->active = 1;
    
    system->airline_count++;
    return airline->id;
}

int book_flight(FlightBookingSystem *system, int flight_id, int passenger_id,
               int seat_number) {
    if (system->booking_count >= MAX_BOOKINGS) {
        return 0;
    }
    
    // Validate flight and passenger exist
    if (find_flight_by_id(system, flight_id) == -1 || find_passenger_by_id(system, passenger_id) == -1) {
        return 0;
    }
    
    // Check flight availability
    int flight_index = find_flight_by_id(system, flight_id);
    if (flight_index == -1) {
        return 0;
    }
    
    // Check if passenger is already booked on this flight
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].flight_id == flight_id && 
            system->bookings[i].passenger_id == passenger_id &&
            system->bookings[i].active) {
            return 0;
        }
    }
    
    // Check seat availability
    if (system->flights[flight_index].available_seats <= 0) {
        return 0;
    }
    
    // Create booking
    Booking *booking = &system->bookings[system->booking_count];
    booking->id = system->booking_count + 1;
    booking->flight_id = flight_id;
    booking->passenger_id = passenger_id;
    
    // Set booking date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    booking->booking_date.day = current_time->tm_mday;
    booking->booking_date.month = current_time->tm_mon + 1;
    booking->booking_date.year = current_time->tm_year + 1900;
    
    booking->seat_number = seat_number;
    booking->price_paid = system->flights[flight_index].price;
    strcpy(booking->status, "Confirmed");
    booking->active = 1;
    
    // Update available seats
    system->flights[flight_index].available_seats--;
    
    system->booking_count++;
    return 1;
}

void cancel_booking(FlightBookingSystem *system, int booking_id) {
    int booking_index = find_booking_by_id(system, booking_id);
    if (booking_index == -1) {
        printf("Booking not found.\n");
        return;
    }
    
    Booking *booking = &system->bookings[booking_index];
    
    strcpy(booking->status, "Cancelled");
    
    // Return seat to available pool
    int flight_index = find_flight_by_id(system, booking->flight_id);
    if (flight_index != -1) {
        system->flights[flight_index].available_seats++;
    }
    
    printf("Booking #%d cancelled successfully.\n", booking_id);
    printf("Refund: $%.2f\n", booking->price_paid);
}

void update_booking_status(FlightBookingSystem *system, int booking_id, const char *status) {
    int booking_index = find_booking_by_id(system, booking_id);
    if (booking_index == -1) {
        printf("Booking not found.\n");
        return;
    }
    
    Booking *booking = &system->bookings[booking_index];
    
    strcpy(booking->status, status);
    
    printf("Booking #%d status updated to '%s'.\n", booking_id, status);
    printf("Description: %s\n", get_booking_status_description(status));
}

void display_flight(const Flight *flight) {
    printf("\nFlight ID: %d\n", flight->id);
    printf("Flight Number: %s\n", flight->flight_number);
    printf("Origin: %s\n", flight->origin);
    printf("Destination: %s\n", flight->destination);
    printf("Departure: %02d/%02d/%d %s\n", 
           flight->departure_date.day, flight->departure_date.month, flight->departure_date.year,
           flight->departure_time);
    printf("Arrival: %02d/%02d/%d %s\n", 
           flight->arrival_date.day, flight->arrival_date.month, flight->arrival_date.year,
           flight->arrival_time);
    printf("Airline: %s\n", flight->airline);
    printf("Available Seats: %d\n", flight->available_seats);
    printf("Price: $%.2f\n", flight->price);
    printf("Status: %s\n", flight->active ? "Active" : "Inactive");
}

void display_passenger(const Passenger *passenger) {
    printf("\nPassenger ID: %d\n", passenger->id);
    printf("Name: %s %s\n", passenger->first_name, passenger->last_name);
    printf("Email: %s\n", passenger->email);
    printf("Phone: %s\n", passenger->phone);
    printf("Birth Date: %02d/%02d/%d\n", 
           passenger->birth_date.day, passenger->birth_date.month, passenger->birth_date.year);
    printf("Gender: %s\n", passenger->gender);
    printf("Passport Number: %s\n", passenger->passport_number);
    printf("Nationality: %s\n", passenger->nationality);
    printf("Status: %s\n", passenger->active ? "Active" : "Inactive");
}

void display_booking(const Booking *booking, const FlightBookingSystem *system) {
    printf("\nBooking ID: %d\n", booking->id);
    
    // Display flight information
    int flight_index = find_flight_by_id(system, booking->flight_id);
    if (flight_index != -1) {
        printf("Flight: %s from %s to %s\n", 
               system->flights[flight_index].flight_number,
               system->flights[flight_index].origin,
               system->flights[flight_index].destination);
        printf("Departure: %02d/%02d/%d %s\n", 
               system->flights[flight_index].departure_date.day,
               system->flights[flight_index].departure_date.month,
               system->flights[flight_index].departure_date.year,
               system->flights[flight_index].departure_time);
        printf("Arrival: %02d/%02d/%d %s\n", 
               system->flights[flight_index].arrival_date.day,
               system->flights[flight_index].arrival_date.month,
               system->flights[flight_index].arrival_date.year,
               system->flights[flight_index].arrival_time);
    }
    
    // Display passenger information
    int passenger_index = find_passenger_by_id(system, booking->passenger_id);
    if (passenger_index != -1) {
        printf("Passenger: %s %s (ID: %d)\n", 
               system->passengers[passenger_index].first_name,
               system->passengers[passenger_index].last_name,
               booking->passenger_id);
    }
    
    printf("Booking Date: %02d/%02d/%d\n", 
           booking->booking_date.day, booking->booking_date.month, booking->booking_date.year);
    printf("Seat Number: %d\n", booking->seat_number);
    printf("Price Paid: $%.2f\n", booking->price_paid);
    printf("Status: %s\n", booking->status);
    printf("Description: %s\n", get_booking_status_description(booking->status));
    printf("Status: %s\n", booking->active ? "Active" : "Inactive");
}

void display_airline(const Airline *airline) {
    printf("\nAirline ID: %d\n", airline->id);
    printf("Name: %s\n", airline->name);
    printf("IATA Code: %s\n", airline->iata_code);
    printf("Description: %s\n", airline->description);
    printf("Status: %s\n", airline->active ? "Active" : "Inactive");
}

void display_all_flights(const FlightBookingSystem *system) {
    printf("\n=== All Flights ===\n");
    printf("Total Flights: %d\n\n", system->flight_count);
    
    for (int i = 0; i < system->flight_count; i++) {
        if (system->flights[i].active) {
            display_flight(&system->flights[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_passengers(const FlightBookingSystem *system) {
    printf("\n=== All Passengers ===\n");
    printf("Total Passengers: %d\n\n", system->passenger_count);
    
    for (int i = 0; i < system->passenger_count; i++) {
        if (system->passengers[i].active) {
            display_passenger(&system->passengers[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_bookings(const FlightBookingSystem *system) {
    printf("\n=== All Bookings ===\n");
    printf("Total Bookings: %d\n\n", system->booking_count);
    
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active) {
            display_booking(&system->bookings[i], system);
            printf("========================\n");
        }
    }
}

void display_all_airlines(const FlightBookingSystem *system) {
    printf("\n=== All Airlines ===\n");
    printf("Total Airlines: %d\n\n", system->airline_count);
    
    for (int i = 0; i < system->airline_count; i++) {
        if (system->airlines[i].active) {
            display_airline(&system->airlines[i]);
            printf("------------------------\n");
        }
    }
}

void search_flights_by_destination(const FlightBookingSystem *system, const char *destination) {
    printf("\nSearch Results for flights to '%s':\n", destination);
    int found = 0;
    
    for (int i = 0; i < system->flight_count; i++) {
        if (system->flights[i].active && strstr(system->flights[i].destination, destination) != NULL) {
            display_flight(&system->flights[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No flights found to this destination.\n");
    }
}

void search_flights_by_date(const FlightBookingSystem *system, Date date) {
    printf("\nSearch Results for flights on %02d/%02d/%d:\n", 
           date.day, date.month, date.year);
    int found = 0;
    
    for (int i = 0; i < system->flight_count; i++) {
        if (system->flights[i].active && 
            system->flights[i].departure_date.day == date.day &&
            system->flights[i].departure_date.month == date.month &&
            system->flights[i].departure_date.year == date.year) {
            display_flight(&system->flights[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No flights found on this date.\n");
    }
}

void search_flights_by_airline(const FlightBookingSystem *system, const char *airline) {
    printf("\nSearch Results for flights with airline '%s':\n", airline);
    int found = 0;
    
    for (int i = 0; i < system->flight_count; i++) {
        if (system->flights[i].active && strstr(system->flights[i].airline, airline) != NULL) {
            display_flight(&system->flights[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No flights found with this airline.\n");
    }
}

void search_passengers_by_name(const FlightBookingSystem *system, const char *name) {
    printf("\nSearch Results for passengers with name containing '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < system->passenger_count; i++) {
        if (system->passengers[i].active && 
            (strstr(system->passengers[i].first_name, name) != NULL || 
             strstr(system->passengers[i].last_name, name) != NULL)) {
            display_passenger(&system->passengers[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No passengers found with name containing '%s'.\n", name);
    }
}

void search_bookings_by_passenger(const FlightBookingSystem *system, int passenger_id) {
    int passenger_index = find_passenger_by_id(system, passenger_id);
    if (passenger_index == -1) {
        printf("Passenger not found.\n");
        return;
    }
    
    printf("\nSearch Results for bookings of passenger %s %s (ID: %d):\n", 
           system->passengers[passenger_index].first_name,
           system->passengers[passenger_index].last_name,
           passenger_id);
    
    int found = 0;
    
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active && system->bookings[i].passenger_id == passenger_id) {
            display_booking(&system->bookings[i], system);
            printf("========================\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No bookings found for this passenger.\n");
    }
}

void generate_passenger_report(const FlightBookingSystem *system, int passenger_id) {
    int passenger_index = find_passenger_by_id(system, passenger_id);
    if (passenger_index == -1) {
        printf("Passenger not found.\n");
        return;
    }
    
    Passenger *passenger = &system->passengers[passenger_index];
    
    printf("\n=== Passenger Report: %s %s ===\n", passenger->first_name, passenger->last_name);
    printf("Passenger ID: %d\n", passenger->id);
    printf("Email: %s\n", passenger->email);
    printf("Phone: %s\n", passenger->phone);
    printf("Birth Date: %02d/%02d/%d\n", 
           passenger->birth_date.day, passenger->birth_date.month, passenger->birth_date.year);
    printf("Gender: %s\n", passenger->gender);
    printf("Passport Number: %s\n", passenger->passport_number);
    printf("Nationality: %s\n", passenger->nationality);
    printf("Status: %s\n", passenger->active ? "Active" : "Inactive");
    
    // Calculate age
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_year = current_time->tm_year + 1900;
    int age = current_year - passenger->birth_date.year;
    
    // Adjust age if birthday hasn't occurred this year
    if (current_time->tm_mon + 1 < passenger->birth_date.month || 
        (current_time->tm_mon + 1 == passenger->birth_date.month && 
         current_time->tm_mday < passenger->birth_date.day)) {
        age--;
    }
    
    printf("Age: %d\n", age);
    
    // Booking history
    printf("\nBooking History:\n");
    printf("%-5s %-15s %-15s %-12s %-10s %-10s %-10s\n", 
           "ID", "Flight", "Origin", "Destination", "Date", "Price", "Status");
    printf("-----------------------------------------------------------------\n");
    
    int booking_count = 0;
    double total_spent = 0.0;
    
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active && system->bookings[i].passenger_id == passenger_id) {
            int flight_index = find_flight_by_id(system, system->bookings[i].flight_id);
            if (flight_index != -1) {
                printf("%-5d %-15s %-15s %-12s %-10s $%-9.2f %-10s\n", 
                       system->bookings[i].id,
                       system->flights[flight_index].flight_number,
                       system->flights[flight_index].origin,
                       system->flights[flight_index].destination,
                       system->bookings[i].booking_date.day, system->bookings[i].booking_date.month,
                       system->bookings[i].booking_date.year,
                       system->bookings[i].price_paid,
                       system->bookings[i].status);
                
                booking_count++;
                total_spent += system->bookings[i].price_paid;
            }
        }
    }
    
    printf("-----------------------------------------------------------------\n");
    printf("Total Bookings: %d\n", booking_count);
    printf("Total Spent: $%.2f\n", total_spent);
    
    if (booking_count > 0) {
        printf("Average Price per Booking: $%.2f\n", total_spent / booking_count);
        
        // Calculate booking statistics
        int confirmed_count = 0;
        int cancelled_count = 0;
        int completed_count = 0;
        
        for (int i = 0; i < system->booking_count; i++) {
            if (system->bookings[i].active && system->bookings[i].passenger_id == passenger_id) {
                if (strcmp(system->bookings[i].status, "Confirmed") == 0) {
                    confirmed_count++;
                } else if (strcmp(system->bookings[i].status, "Cancelled") == 0) {
                    cancelled_count++;
                } else if (strcmp(system->bookings[i].status, "Completed") == 0) {
                    completed_count++;
                }
            }
        }
        
        printf("\nBooking Statistics:\n");
        printf("Confirmed: %d (%.1f%%)\n", confirmed_count, (float)confirmed_count / booking_count * 100);
        printf("Cancelled: %d (%.1f%%)\n", cancelled_count, (float)cancelled_count / booking_count * 100);
        printf("Completed: %d (%.1f%%)\n", completed_count, (float)completed_count / booking_count * 100);
    }
}

void generate_flight_report(const FlightBookingSystem *system, int flight_id) {
    int flight_index = find_flight_by_id(system, flight_id);
    if (flight_index == -1) {
        printf("Flight not found.\n");
        return;
    }
    
    Flight *flight = &system->flights[flight_index];
    
    printf("\n=== Flight Report: %s ===\n", flight->flight_number);
    printf("Flight ID: %d\n", flight->id);
    printf("Origin: %s\n", flight->origin);
    printf("Destination: %s\n", flight->destination);
    printf("Departure: %02d/%02d/%d %s\n", 
           flight->departure_date.day, flight->departure_date.month, flight->departure_date.year,
           flight->departure_time);
    printf("Arrival: %02d/%02d/%d %s\n", 
           flight->arrival_date.day, flight->arrival_date.month, flight->arrival_date.year,
           flight->arrival_time);
    printf("Airline: %s\n", flight->airline);
    printf("Available Seats: %d\n", flight->available_seats);
    printf("Price: $%.2f\n", flight->price);
    printf("Status: %s\n", flight->active ? "Active" : "Inactive");
    
    // Booking summary
    printf("\nBooking Summary:\n");
    printf("%-5s %-20s %-15s %-10s %-10s\n", 
           "ID", "Passenger Name", "Seat Number", "Price", "Status");
    printf("--------------------------------------------------------\n");
    
    int booking_count = 0;
    double total_revenue = 0.0;
    
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active && system->bookings[i].flight_id == flight_id) {
            int passenger_index = find_passenger_by_id(system, system->bookings[i].passenger_id);
            if (passenger_index != -1) {
                printf("%-5d %-20s %-15d %-10s $%-9.2f %-10s\n", 
                       system->bookings[i].id,
                       system->passengers[passenger_index].first_name,
                       system->passengers[passenger_index].last_name,
                       system->bookings[i].seat_number,
                       system->bookings[i].price_paid,
                       system->bookings[i].status);
                
                booking_count++;
                
                if (strcmp(system->bookings[i].status, "Cancelled") != 0) { // Not cancelled
                    total_revenue += system->bookings[i].price_paid;
                }
            }
        }
    }
    
    printf("--------------------------------------------------------\n");
    printf("Total Bookings: %d\n", booking_count);
    
    if (booking_count > 0) {
        printf("Total Revenue: $%.2f\n", total_revenue);
        
        // Calculate revenue based on flight price and bookings
        int potential_revenue = (flight->available_seats + booking_count) * flight->price;
        printf("Potential Revenue: $%.2f\n", potential_revenue);
        printf("Revenue Utilization: %.1f%%\n", (total_revenue / potential_revenue) * 100);
    }
}

void generate_booking_report(const FlightBookingSystem *system, const Date *start_date,
                           const Date *end_date) {
    printf("\n=== Booking Report ===\n");
    printf("Period: %02d/%02d/%d to %02d/%02d/%d\n", 
           start_date->day, start_date->month, start_date->year,
           end_date->day, end_date->month, end_date->year);
    
    printf("\nBookings within date range:\n");
    printf("%-5s %-15s %-15s %-15s %-10s %-10s\n", 
           "ID", "Passenger Name", "Flight", "Destination", "Price", "Status");
    printf("-----------------------------------------------------------------\n");
    
    int booking_count = 0;
    double total_revenue = 0.0;
    
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active) {
            // Check if booking date is within range
            int booking_timestamp = system->bookings[i].booking_date.year * 10000 + 
                                  system->bookings[i].booking_date.month * 100 + 
                                  system->bookings[i].booking_date.day;
            
            int start_timestamp = start_date->year * 10000 + 
                                start_date->month * 100 + 
                                start_date->day;
            
            int end_timestamp = end_date->year * 10000 + 
                              end_date->month * 100 + 
                              end_date->day;
            
            if (booking_timestamp >= start_timestamp && booking_timestamp <= end_timestamp) {
                int passenger_index = find_passenger_by_id(system, system->bookings[i].passenger_id);
                int flight_index = find_flight_by_id(system, system->bookings[i].flight_id);
                
                if (passenger_index != -1 && flight_index != -1) {
                    printf("%-5d %-15s %-15s %-15s $%-10.2f %-10s\n", 
                           system->bookings[i].id,
                           system->passengers[passenger_index].first_name,
                           system->passengers[passenger_index].last_name,
                           system->flights[flight_index].flight_number,
                           system->flights[flight_index].destination,
                           system->bookings[i].price_paid,
                           system->bookings[i].status);
                    
                    booking_count++;
                    
                    if (strcmp(system->bookings[i].status, "Cancelled") != 0) { // Not cancelled
                        total_revenue += system->bookings[i].price_paid;
                    }
                }
            }
        }
    }
    
    printf("-----------------------------------------------------------------\n");
    printf("Total Bookings: %d\n", booking_count);
    
    if (booking_count > 0) {
        printf("Total Revenue: $%.2f\n", total_revenue);
        printf("Average Revenue per Booking: $%.2f\n", total_revenue / booking_count);
        
        // Calculate booking statistics
        int confirmed_count = 0;
        int cancelled_count = 0;
        int completed_count = 0;
        
        for (int i = 0; i < system->booking_count; i++) {
            if (system->bookings[i].active) {
                // Check if booking date is within range
                int booking_timestamp = system->bookings[i].booking_date.year * 10000 + 
                                      system->bookings[i].booking_date.month * 100 + 
                                      system->bookings[i].booking_date.day;
                
                int start_timestamp = start_date->year * 10000 + 
                                    start_date->month * 100 + 
                                    start_date->day;
                
                int end_timestamp = end_date->year * 10000 + 
                                  end_date->month * 100 + 
                                  end_date->day;
                
                if (booking_timestamp >= start_timestamp && booking_timestamp <= end_timestamp) {
                    if (strcmp(system->bookings[i].status, "Confirmed") == 0) {
                        confirmed_count++;
                    } else if (strcmp(system->bookings[i].status, "Cancelled") == 0) {
                        cancelled_count++;
                    } else if (strcmp(system->bookings[i].status, "Completed") == 0) {
                        completed_count++;
                    }
                }
            }
        }
        
        printf("\nBooking Statistics:\n");
        printf("Confirmed: %d (%.1f%%)\n", confirmed_count, (float)confirmed_count / booking_count * 100);
        printf("Cancelled: %d (%.1f%%)\n", cancelled_count, (float)cancelled_count / booking_count * 100);
        printf("Completed: %d (%.1f%%)\n", completed_count, (float)completed_count / booking_count * 100);
    }
}

void generate_revenue_report(const FlightBookingSystem *system) {
    printf("\n=== Revenue Report ===\n");
    
    // Overall booking statistics
    int total_bookings = 0;
    int confirmed_bookings = 0;
    int cancelled_bookings = 0;
    int completed_bookings = 0;
    double total_revenue = 0.0;
    double total_refunds = 0.0;
    
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active) {
            total_bookings++;
            
            if (strcmp(system->bookings[i].status, "Confirmed") == 0) {
                confirmed_bookings++;
                total_revenue += system->bookings[i].price_paid;
            } else if (strcmp(system->bookings[i].status, "Cancelled") == 0) {
                cancelled_bookings++;
                total_refunds += system->bookings[i].price_paid;
            } else if (strcmp(system->bookings[i].status, "Completed") == 0) {
                completed_bookings++;
                total_revenue += system->bookings[i].price_paid;
            }
        }
    }
    
    printf("Overall Statistics:\n");
    printf("Total Bookings: %d\n", total_bookings);
    printf("Confirmed: %d (%.1f%%)\n", confirmed_bookings, (float)confirmed_bookings / total_bookings * 100);
    printf("Cancelled: %d (%.1f%%)\n", cancelled_bookings, (float)cancelled_bookings / total_bookings * 100);
    printf("Completed: %d (%.1f%%)\n", completed_bookings, (float)completed_bookings / total_bookings * 100);
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("Total Refunds: $%.2f\n", total_refunds);
    printf("Net Revenue: $%.2f\n", total_revenue - total_refunds);
    printf("Average Booking Value: $%.2f\n", total_bookings > 0 ? total_revenue / total_bookings : 0.0);
    
    // Revenue by destination
    printf("\nRevenue by Destination:\n");
    
    typedef struct {
        char destination[50];
        double revenue;
        int count;
    } DestinationRevenue;
    
    DestinationRevenue destinations[MAX_FLIGHTS];
    int destination_count = 0;
    
    for (int i = 0; i < system->flight_count; i++) {
        if (system->flights[i].active) {
            int found = 0;
            for (int j = 0; j < destination_count; j++) {
                if (strcmp(destinations[j].destination, system->flights[i].destination) == 0) {
                    found = 1;
                    break;
                }
            }
            
            if (!found && destination_count < MAX_FLIGHTS) {
                strcpy(destinations[destination_count].destination, system->flights[i].destination);
                destinations[destination_count].revenue = 0.0;
                destinations[destination_count].count = 0;
                destination_count++;
            }
        }
    }
    
    // Calculate revenue for each destination
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active) {
            int flight_index = find_flight_by_id(system, system->bookings[i].flight_id);
            if (flight_index != -1) {
                for (int j = 0; j < destination_count; j++) {
                    if (strcmp(destinations[j].destination, system->flights[flight_index].destination) == 0) {
                        if (strcmp(system->bookings[i].status, "Confirmed") == 0 ||
                            strcmp(system->bookings[i].status, "Completed") == 0) {
                            destinations[j].revenue += system->bookings[i].price_paid;
                        }
                        destinations[j].count++;
                        break;
                    }
                }
            }
        }
    }
    
    // Sort destinations by revenue (descending)
    for (int i = 0; i < destination_count - 1; i++) {
        for (int j = 0; j < destination_count - i - 1; j++) {
            if (destinations[j].revenue < destinations[j + 1].revenue) {
                DestinationRevenue temp = destinations[j];
                destinations[j] = destinations[j + 1];
                destinations[j + 1] = temp;
            }
        }
    }
    
    // Display top 10 destinations by revenue
    printf("%-15s %-10s %-15s\n", "Destination", "Bookings", "Revenue");
    printf("-------------------------------------------------\n");
    
    int display_count = destination_count < 10 ? destination_count : 10;
    for (int i = 0; i < display_count; i++) {
        if (destinations[i].count > 0) {
            printf("%-15s %-10d $%-14.2f\n", 
                   destinations[i].destination,
                   destinations[i].count,
                   destinations[i].revenue);
        }
    }
    
    if (destination_count > display_count) {
        printf("... and %d more destinations\n", destination_count - display_count);
    }
}

void check_flight_availability(FlightBookingSystem *system, int flight_id) {
    int flight_index = find_flight_by_id(system, flight_id);
    if (flight_index == -1) {
        printf("Flight not found.\n");
        return;
    }
    
    Flight *flight = &system->flights[flight_index];
    
    printf("\n=== Flight Availability ===\n");
    printf("Flight Number: %s\n", flight->flight_number);
    printf("Origin: %s\n", flight->origin);
    printf("Destination: %s\n", flight->destination);
    printf("Departure: %02d/%02d/%d %s\n", 
           flight->departure_date.day, flight->departure_date.month, flight->departure_date.year,
           flight->departure_time);
    printf("Arrival: %02d/%02d/%d %s\n", 
           flight->arrival_date.day, flight->arrival_date.month, flight->arrival_date.year,
           flight->arrival_time);
    printf("Airline: %s\n", flight->airline);
    printf("Available Seats: %d\n", flight->available_seats);
    printf("Price: $%.2f\n", flight->price);
    printf("Status: %s\n", flight->active ? "Active" : "Inactive");
    
    if (!flight->active) {
        printf("\nWarning: This flight is not active.\n");
    } else if (flight->available_seats <= 0) {
        printf("\nWarning: No seats available on this flight.\n");
    } else {
        printf("\nThis flight is available for booking.\n");
    }
    
    // Show current bookings for this flight
    printf("\nCurrent Bookings:\n");
    printf("%-5s %-20s %-10s %-10s\n", 
           "ID", "Passenger Name", "Seat Number", "Status");
    printf("-------------------------------------------------\n");
    
    int booking_count = 0;
    
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].active && system->bookings[i].flight_id == flight_id) {
            int passenger_index = find_passenger_by_id(system, system->bookings[i].passenger_id);
            if (passenger_index != -1) {
                printf("%-5d %-20s %-10d %-10s\n", 
                       system->bookings[i].id,
                       system->passengers[passenger_index].first_name,
                       system->passengers[passenger_index].last_name,
                       system->bookings[i].seat_number,
                       system->bookings[i].status);
                
                booking_count++;
            }
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("Current Bookings: %d\n", booking_count);
    printf("Remaining Seats: %d\n", flight->available_seats);
}

int find_flight_by_id(const FlightBookingSystem *system, int flight_id) {
    for (int i = 0; i < system->flight_count; i++) {
        if (system->flights[i].id == flight_id && system->flights[i].active) {
            return i;
        }
    }
    return -1;
}

int find_passenger_by_id(const FlightBookingSystem *system, int passenger_id) {
    for (int i = 0; i < system->passenger_count; i++) {
        if (system->passengers[i].id == passenger_id && system->passengers[i].active) {
            return i;
        }
    }
    return -1;
}

int find_booking_by_id(const FlightBookingSystem *system, int booking_id) {
    for (int i = 0; i < system->booking_count; i++) {
        if (system->bookings[i].id == booking_id && system->bookings[i].active) {
            return i;
        }
    }
    return -1;
}

int find_airline_by_id(const FlightBookingSystem *system, int airline_id) {
    for (int i = 0; i < system->airline_count; i++) {
        if (system->airlines[i].id == airline_id && system->airlines[i].active) {
            return i;
        }
    }
    return -1;
}

int save_system(const FlightBookingSystem *system) {
    FILE *flights_file = fopen(FILENAME_FLIGHTS, "wb");
    if (flights_file != NULL) {
        fwrite(&system->flight_count, sizeof(int), 1, flights_file);
        fwrite(system->flights, sizeof(Flight), system->flight_count, flights_file);
        fclose(flights_file);
    } else {
        return 0;
    }
    
    FILE *passengers_file = fopen(FILENAME_PASSENGERS, "wb");
    if (passengers_file != NULL) {
        fwrite(&system->passenger_count, sizeof(int), 1, passengers_file);
        fwrite(system->passengers, sizeof(Passenger), system->passenger_count, passengers_file);
        fclose(passengers_file);
    } else {
        return 0;
    }
    
    FILE *bookings_file = fopen(FILENAME_BOOKINGS, "wb");
    if (bookings_file != NULL) {
        fwrite(&system->booking_count, sizeof(int), 1, bookings_file);
        fwrite(system->bookings, sizeof(Booking), system->booking_count, bookings_file);
        fclose(bookings_file);
    } else {
        return 0;
    }
    
    FILE *airlines_file = fopen(FILENAME_AIRLINES, "wb");
    if (airlines_file != NULL) {
        fwrite(&system->airline_count, sizeof(int), 1, airlines_file);
        fwrite(system->airlines, sizeof(Airline), system->airline_count, airlines_file);
        fclose(airlines_file);
        return 1;
    }
    
    return 0;
}

int load_system(FlightBookingSystem *system) {
    FILE *flights_file = fopen(FILENAME_FLIGHTS, "rb");
    if (flights_file != NULL) {
        fread(&system->flight_count, sizeof(int), 1, flights_file);
        fread(system->flights, sizeof(Flight), system->flight_count, flights_file);
        fclose(flights_file);
    } else {
        return 0;
    }
    
    FILE *passengers_file = fopen(FILENAME_PASSENGERS, "rb");
    if (passengers_file != NULL) {
        fread(&system->passenger_count, sizeof(int), 1, passengers_file);
        fread(system->passengers, sizeof(Passenger), system->passenger_count, passengers_file);
        fclose(passengers_file);
    } else {
        return 0;
    }
    
    FILE *bookings_file = fopen(FILENAME_BOOKINGS, "rb");
    if (bookings_file != NULL) {
        fread(&system->booking_count, sizeof(int), 1, bookings_file);
        fread(system->bookings, sizeof(Booking), system->booking_count, bookings_file);
        fclose(bookings_file);
    } else {
        return 0;
    }
    
    FILE *airlines_file = fopen(FILENAME_AIRLINES, "rb");
    if (airlines_file != NULL) {
        fread(&system->airline_count, sizeof(int), 1, airlines_file);
        fread(system->airlines, sizeof(Airline), system->airline_count, airlines_file);
        fclose(airlines_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(FlightBookingSystem *system) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* cities[] = {"New York", "Los Angeles", "Chicago", "Houston", "Phoenix",
                     "Philadelphia", "San Antonio", "San Diego", "Dallas", "San Jose",
                     "Austin", "Jacksonville", "Fort Worth", "Columbus", "Charlotte",
                     "San Francisco", "Indianapolis", "Seattle", "Denver", "Boston"};
    
    char* airlines[] = {"Delta Air Lines", "American Airlines", "United Airlines",
                       "Southwest Airlines", "JetBlue Airways", "Alaska Airlines",
                       "Hawaiian Airlines", "Frontier Airlines", "Spirit Airlines",
                       "Allegiant Air"};
    
    char* iata_codes[] = {"DL", "AA", "UA", "WN", "B6", "AS", "HA", "F9", "NK", "G4"};
    
    char* nationalities[] = {"United States", "Canada", "United Kingdom", "Germany",
                           "France", "Italy", "Spain", "Japan", "China", "Australia",
                           "India", "Brazil", "Mexico", "Russia", "South Korea"};
    
    srand(time(NULL));
    
    // Add airlines
    for (int i = 0; i < 10 && system->airline_count < MAX_AIRLINES; i++) {
        char description[200];
        sprintf(description, "Major international and domestic carrier with comprehensive route network");
        
        add_airline(system, airlines[i], iata_codes[i], description);
    }
    
    // Add flights
    for (int i = 0; i < 100 && system->flight_count < MAX_FLIGHTS; i++) {
        char flight_number[20], origin[50], destination[50];
        sprintf(flight_number, "%s%d", airlines[rand() % 10], 100 + i);
        
        strcpy(origin, cities[rand() % 20]);
        strcpy(destination, cities[rand() % 20]);
        
        int dep_day = 1 + (rand() % 28);
        int dep_month = 1 + (rand() % 12);
        int dep_year = 2023 + (rand() % 2);
        Date departure_date = {dep_day, dep_month, dep_year};
        
        char departure_time[10];
        sprintf(departure_time, "%02d:%02d", 6 + (rand() % 18), rand() % 60);
        
        int arr_day = dep_day + (rand() % 7);
        int arr_month = dep_month;
        int arr_year = dep_year;
        if (arr_day > 28) {
            arr_day = arr_day - 28;
            arr_month = dep_month + 1;
            if (arr_month > 12) {
                arr_month = 1;
                arr_year++;
            }
        }
        Date arrival_date = {arr_day, arr_month, arr_year};
        
        char arrival_time[10];
        sprintf(arrival_time, "%02d:%02d", 8 + (rand() % 14), rand() % 60);
        
        int available_seats = 50 + (rand() % 200); // 50-250 seats
        double price = 100.0 + (rand() % 900) + (rand() % 100) / 100.0; // $100-$999
        
        add_flight(system, flight_number, origin, destination, departure_date, departure_time,
                 arrival_date, arrival_time, airlines[rand() % 10], available_seats, price);
    }
    
    // Add passengers
    for (int i = 0; i < 500 && system->passenger_count < MAX_PASSENGERS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], gender[10];
        char passport_number[20], nationality[30];
        sprintf(email, "%s.%d@passenger.com", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        
        const char* genders[] = {"Male", "Female", "Other"};
        strcpy(gender, genders[rand() % 3]);
        
        sprintf(passport_number, "PP%08d", 10000000 + i);
        strcpy(nationality, nationalities[rand() % 15]);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1960 + (rand() % 60);
        Date birth_date = {day, month, year};
        
        add_passenger(system, first_name, last_name, email, phone, birth_date,
                    gender, passport_number, nationality);
    }
    
    // Add bookings
    for (int i = 0; i < 1000 && system->booking_count < MAX_BOOKINGS; i++) {
        int flight_id = 1 + (rand() % 100);
        int passenger_id = 1 + (rand() % 500);
        int seat_number = 1 + (rand() % 250);
        
        if (book_flight(system, flight_id, passenger_id, seat_number)) {
            // Randomly set some bookings to cancelled or completed
            int booking_index = system->booking_count - 1;
            if (rand() % 10 < 2) { // 20% chance of cancelled
                strcpy(system->bookings[booking_index].status, "Cancelled");
            } else if (rand() % 10 < 3) { // 30% chance of completed
                strcpy(system->bookings[booking_index].status, "Completed");
            }
        }
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

const char* get_booking_status_description(const char *status) {
    if (strcmp(status, "Confirmed") == 0) return "Booking is confirmed and awaiting flight";
    if (strcmp(status, "Cancelled") == 0) return "Booking has been cancelled and will be refunded";
    if (strcmp(status, "Completed") == 0) return "Flight has been completed";
    return "Unknown status";
}