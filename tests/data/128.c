#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_PATIENTS 1000
#define MAX_DOCTORS 200
#define MAX_APPOINTMENTS 2000
#define MAX_DEPARTMENTS 50
#define MAX_MEDICATIONS 500
#define MAX_NAME_LENGTH 50
#define FILENAME_PATIENTS "patients.dat"
#define FILENAME_DOCTORS "doctors.dat"
#define FILENAME_APPOINTMENTS "appointments.dat"
#define FILENAME_DEPARTMENTS "departments.dat"
#define FILENAME_MEDICATIONS "medications.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    char address[100];
    Date birth_date;
    char blood_type[5]; // A+, A-, B+, B-, AB+, AB-, O+, O-
    char gender[10]; // Male, Female, Other
    char emergency_contact[50];
    char emergency_phone[15];
    Date registration_date;
    int active;
    char insurance_id[30];
    char allergies[200];
    char medical_history[500];
} Patient;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    int department_id;
    char specialization[50];
    char license_number[30];
    Date hire_date;
    int active;
    char education[100];
    double hourly_rate;
    char schedule[100]; // e.g., "Mon-Fri 9am-5pm"
    char office_location[50];
} Doctor;

typedef struct {
    int id;
    char name[50];
    char description[200];
    int active;
} Department;

typedef struct {
    int id;
    int patient_id;
    int doctor_id;
    Date appointment_date;
    char time[10]; // HH:MM
    int duration; // in minutes
    char status[20]; // "Scheduled", "Completed", "Cancelled"
    char notes[500];
    char type[30]; // "Consultation", "Follow-up", "Emergency", "Surgery"
    int active;
} Appointment;

typedef struct {
    int id;
    char name[50];
    char description[200];
    char manufacturer[50];
    double price;
    int stock_quantity;
    int reorder_level;
    char dosage[30]; // e.g., "500mg"
    int active;
} Medication;

typedef struct {
    Patient patients[MAX_PATIENTS];
    int patient_count;
    Doctor doctors[MAX_DOCTORS];
    int doctor_count;
    Department departments[MAX_DEPARTMENTS];
    int department_count;
    Appointment appointments[MAX_APPOINTMENTS];
    int appointment_count;
    Medication medications[MAX_MEDICATIONS];
    int medication_count;
} HospitalDatabase;

// Function prototypes
void initialize_database(HospitalDatabase *db);
int add_patient(HospitalDatabase *db, const char *first_name, const char *last_name,
                const char *email, const char *phone, const char *address,
                Date birth_date, const char *blood_type, const char *gender,
                const char *emergency_contact, const char *emergency_phone,
                const char *insurance_id, const char *allergies, const char *medical_history);
int add_doctor(HospitalDatabase *db, const char *first_name, const char *last_name,
               const char *email, const char *phone, int department_id,
               const char *specialization, const char *license_number,
               const char *education, double hourly_rate, const char *schedule, const char *office_location);
int add_department(HospitalDatabase *db, const char *name, const char *description);
int add_appointment(HospitalDatabase *db, int patient_id, int doctor_id,
                  Date appointment_date, const char *time, int duration,
                  const char *type, const char *notes);
int add_medication(HospitalDatabase *db, const char *name, const char *description,
                  const char *manufacturer, double price, int stock_quantity,
                  int reorder_level, const char *dosage);
void update_appointment_status(HospitalDatabase *db, int appointment_id, const char *status);
void prescribe_medication(HospitalDatabase *db, int appointment_id, int medication_id, const char *dosage, int quantity);
void display_patient(const Patient *patient);
void display_doctor(const Doctor *doctor, const HospitalDatabase *db);
void display_appointment(const Appointment *appointment, const HospitalDatabase *db);
void display_department(const Department *department);
void display_medication(const Medication *medication);
void display_all_patients(const HospitalDatabase *db);
void display_all_doctors(const HospitalDatabase *db);
void display_all_appointments(const HospitalDatabase *db);
void display_all_departments(const HospitalDatabase *db);
void display_all_medications(const HospitalDatabase *db);
void search_patients_by_name(const HospitalDatabase *db, const char *name);
void search_doctors_by_specialization(const HospitalDatabase *db, const char *specialization);
void search_appointments_by_date(const HospitalDatabase *db, int day, int month, int year);
void search_appointments_by_doctor(const HospitalDatabase *db, int doctor_id);
void search_appointments_by_patient(const HospitalDatabase *db, int patient_id);
void generate_patient_report(const HospitalDatabase *db, int patient_id);
void generate_doctor_report(const HospitalDatabase *db, int doctor_id);
void generate_department_report(const HospitalDatabase *db, int department_id);
void generate_appointment_report(const HospitalDatabase *db, const Date *start_date, const Date *end_date);
void generate_inventory_report(const HospitalDatabase *db);
int find_patient_by_id(const HospitalDatabase *db, int patient_id);
int find_doctor_by_id(const HospitalDatabase *db, int doctor_id);
int find_appointment_by_id(const HospitalDatabase *db, int appointment_id);
int find_department_by_id(const HospitalDatabase *db, int department_id);
int find_medication_by_id(const HospitalDatabase *db, int medication_id);
int save_database(const HospitalDatabase *db);
int load_database(HospitalDatabase *db);
void generate_sample_data(HospitalDatabase *db);
void update_patient_info(HospitalDatabase *db, int patient_id, const char *email, const char *phone, const char *address);
void update_doctor_info(HospitalDatabase *db, int doctor_id, const char *email, const char *phone, const char *specialization);
int is_valid_email(const char *email);
int is_valid_phone(const char *phone);
int is_valid_blood_type(const char *blood_type);
int is_valid_date(int day, int month, int year);
const char* get_appointment_status_description(const char *status);
const char* get_appointment_type_description(const char *type);

int main() {
    HospitalDatabase db;
    initialize_database(&db);
    
    printf("Hospital Management System\n");
    printf("1. Add Patient\n");
    printf("2. Add Doctor\n");
    printf("3. Add Department\n");
    printf("4. Add Appointment\n");
    printf("5. Add Medication\n");
    printf("6. Update Appointment Status\n");
    printf("7. Prescribe Medication\n");
    printf("8. Update Patient Info\n");
    printf("9. Update Doctor Info\n");
    printf("10. Display All Patients\n");
    printf("11. Display All Doctors\n");
    printf("12. Display All Appointments\n");
    printf("13. Display All Departments\n");
    printf("14. Display All Medications\n");
    printf("15. Search Patients by Name\n");
    printf("16. Search Doctors by Specialization\n");
    printf("17. Search Appointments by Date\n");
    printf("18. Search Appointments by Doctor\n");
    printf("19. Search Appointments by Patient\n");
    printf("20. Generate Patient Report\n");
    printf("21. Generate Doctor Report\n");
    printf("22. Generate Department Report\n");
    printf("23. Generate Appointment Report\n");
    printf("24. Generate Inventory Report\n");
    printf("25. Save Database\n");
    printf("26. Load Database\n");
    printf("27. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[100], blood_type[5], gender[10];
                char emergency_contact[50], emergency_phone[15], insurance_id[30];
                char allergies[200], medical_history[500];
                int day, month, year;
                
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
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter blood type (A+, A-, B+, B-, AB+, AB-, O+, O-): ");
                scanf("%s", blood_type);
                printf("Enter gender (Male, Female, Other): ");
                scanf(" %[^\n]", gender);
                printf("Enter emergency contact: ");
                scanf(" %[^\n]", emergency_contact);
                printf("Enter emergency phone: ");
                scanf("%s", emergency_phone);
                printf("Enter insurance ID: ");
                scanf(" %[^\n]", insurance_id);
                printf("Enter allergies (separated by commas): ");
                scanf(" %[^\n]", allergies);
                printf("Enter medical history: ");
                scanf(" %[^\n]", medical_history);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                if (!is_valid_blood_type(blood_type)) {
                    printf("Invalid blood type. Must be one of: A+, A-, B+, B-, AB+, AB-, O+, O-\n");
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
                
                int patient_id = add_patient(&db, first_name, last_name, email, phone, address,
                                         birth_date, blood_type, gender, emergency_contact, emergency_phone,
                                         insurance_id, allergies, medical_history);
                if (patient_id != -1) {
                    printf("Patient added with ID: %d\n", patient_id);
                } else {
                    printf("Failed to add patient. Database is full.\n");
                }
                break;
            }
            case 2: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], specialization[50], license_number[30];
                char education[100], schedule[100], office_location[50];
                int department_id;
                double hourly_rate;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                printf("Enter specialization: ");
                scanf(" %[^\n]", specialization);
                printf("Enter license number: ");
                scanf(" %[^\n]", license_number);
                printf("Enter education: ");
                scanf(" %[^\n]", education);
                printf("Enter hourly rate: ");
                scanf("%lf", &hourly_rate);
                printf("Enter schedule (e.g., Mon-Fri 9am-5pm): ");
                scanf(" %[^\n]", schedule);
                printf("Enter office location: ");
                scanf(" %[^\n]", office_location);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                int doctor_id = add_doctor(&db, first_name, last_name, email, phone, department_id,
                                         specialization, license_number, education, hourly_rate, 
                                         schedule, office_location);
                if (doctor_id != -1) {
                    printf("Doctor added with ID: %d\n", doctor_id);
                } else {
                    printf("Failed to add doctor. Database is full or invalid department ID.\n");
                }
                break;
            }
            case 3: {
                char name[50], description[200];
                
                printf("Enter department name: ");
                scanf(" %[^\n]", name);
                printf("Enter department description: ");
                scanf(" %[^\n]", description);
                
                int department_id = add_department(db, name, description);
                if (department_id != -1) {
                    printf("Department added with ID: %d\n", department_id);
                } else {
                    printf("Failed to add department. Database is full.\n");
                }
                break;
            }
            case 4: {
                int patient_id, doctor_id, duration;
                char time[10], type[30], notes[500];
                int day, month, year;
                
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                printf("Enter appointment date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter time (HH:MM): ");
                scanf("%s", time);
                printf("Enter duration (minutes): ");
                scanf("%d", &duration);
                printf("Enter type (Consultation, Follow-up, Emergency, Surgery): ");
                scanf(" %[^\n]", type);
                printf("Enter notes: ");
                scanf(" %[^\n]", notes);
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                if (strcmp(type, "Consultation") != 0 && strcmp(type, "Follow-up") != 0 &&
                    strcmp(type, "Emergency") != 0 && strcmp(type, "Surgery") != 0) {
                    printf("Invalid type. Must be one of: Consultation, Follow-up, Emergency, Surgery\n");
                    break;
                }
                
                Date appointment_date = {day, month, year};
                
                int appointment_id = add_appointment(&db, patient_id, doctor_id, appointment_date,
                                               time, duration, type, notes);
                if (appointment_id != -1) {
                    printf("Appointment added with ID: %d\n", appointment_id);
                } else {
                    printf("Failed to add appointment. Database is full or invalid IDs.\n");
                }
                break;
            }
            case 5: {
                char name[50], description[200], manufacturer[50], dosage[30];
                double price;
                int stock_quantity, reorder_level;
                
                printf("Enter medication name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter manufacturer: ");
                scanf(" %[^\n]", manufacturer);
                printf("Enter price: ");
                scanf("%lf", &price);
                printf("Enter stock quantity: ");
                scanf("%d", &stock_quantity);
                printf("Enter reorder level: ");
                scanf("%d", &reorder_level);
                printf("Enter dosage (e.g., 500mg): ");
                scanf(" %[^\n]", dosage);
                
                int medication_id = add_medication(db, name, description, manufacturer, price,
                                                stock_quantity, reorder_level, dosage);
                if (medication_id != -1) {
                    printf("Medication added with ID: %d\n", medication_id);
                } else {
                    printf("Failed to add medication. Database is full.\n");
                }
                break;
            }
            case 6: {
                int appointment_id;
                char status[20];
                
                printf("Enter appointment ID: ");
                scanf("%d", &appointment_id);
                printf("Enter new status (Scheduled, Completed, Cancelled): ");
                scanf(" %[^\n]", status);
                
                if (strcmp(status, "Scheduled") != 0 && strcmp(status, "Completed") != 0 &&
                    strcmp(status, "Cancelled") != 0) {
                    printf("Invalid status. Must be one of: Scheduled, Completed, Cancelled\n");
                    break;
                }
                
                update_appointment_status(&db, appointment_id, status);
                break;
            }
            case 7: {
                int appointment_id, medication_id, quantity;
                char dosage[30];
                
                printf("Enter appointment ID: ");
                scanf("%d", &appointment_id);
                printf("Enter medication ID: ");
                scanf("%d", &medication_id);
                printf("Enter dosage: ");
                scanf(" %[^\n]", dosage);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                
                prescribe_medication(&db, appointment_id, medication_id, dosage, quantity);
                break;
            }
            case 8: {
                int patient_id;
                char email[50], phone[15], address[100];
                
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                printf("Enter new email: ");
                scanf("%s", email);
                printf("Enter new phone: ");
                scanf("%s", phone);
                printf("Enter new address: ");
                scanf(" %[^\n]", address);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                update_patient_info(&db, patient_id, email, phone, address);
                break;
            }
            case 9: {
                int doctor_id;
                char email[50], phone[15], specialization[50];
                
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                printf("Enter new email: ");
                scanf("%s", email);
                printf("Enter new phone: ");
                scanf("%s", phone);
                printf("Enter new specialization: ");
                scanf(" %[^\n]", specialization);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                update_doctor_info(&db, doctor_id, email, phone, specialization);
                break;
            }
            case 10: {
                display_all_patients(&db);
                break;
            }
            case 11: {
                display_all_doctors(&db);
                break;
            }
            case 12: {
                display_all_appointments(&db);
                break;
            }
            case 13: {
                display_all_departments(&db);
                break;
            }
            case 14: {
                display_all_medications(&db);
                break;
            }
            case 15: {
                char name[MAX_NAME_LENGTH];
                printf("Enter patient name to search: ");
                scanf(" %[^\n]", name);
                search_patients_by_name(&db, name);
                break;
            }
            case 16: {
                char specialization[50];
                printf("Enter doctor specialization to search: ");
                scanf(" %[^\n]", specialization);
                search_doctors_by_specialization(&db, specialization);
                break;
            }
            case 17: {
                int day, month, year;
                
                printf("Enter date to search (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                search_appointments_by_date(&db, day, month, year);
                break;
            }
            case 18: {
                int doctor_id;
                
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                search_appointments_by_doctor(&db, doctor_id);
                break;
            }
            case 19: {
                int patient_id;
                
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                search_appointments_by_patient(&db, patient_id);
                break;
            }
            case 20: {
                int patient_id;
                
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                generate_patient_report(&db, patient_id);
                break;
            }
            case 21: {
                int doctor_id;
                
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                generate_doctor_report(&db, doctor_id);
                break;
            }
            case 22: {
                int department_id;
                
                printf("Enter department ID: ");
                scanf("%d", &department_id);
                generate_department_report(&db, department_id);
                break;
            }
            case 23: {
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
                
                generate_appointment_report(&db, &start_date, &end_date);
                break;
            }
            case 24: {
                generate_inventory_report(&db);
                break;
            }
            case 25: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 26: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 27: {
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

void initialize_database(HospitalDatabase *db) {
    <｜fim▁hole｜>db->patient_count = 0;
    db->doctor_count = 0;
    db->department_count = 0;
    db->appointment_count = 0;
    db->medication_count = 0;
}

int add_patient(HospitalDatabase *db, const char *first_name, const char *last_name,
                const char *email, const char *phone, const char *address,
                Date birth_date, const char *blood_type, const char *gender,
                const char *emergency_contact, const char *emergency_phone,
                const char *insurance_id, const char *allergies, const char *medical_history) {
    if (db->patient_count >= MAX_PATIENTS) {
        return -1;
    }
    
    Patient *patient = &db->patients[db->patient_count];
    patient->id = db->patient_count + 1;
    strncpy(patient->first_name, first_name, MAX_NAME_LENGTH - 1);
    patient->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(patient->last_name, last_name, MAX_NAME_LENGTH - 1);
    patient->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(patient->email, email, 49);
    patient->email[49] = '\0';
    strncpy(patient->phone, phone, 14);
    patient->phone[14] = '\0';
    strncpy(patient->address, address, 99);
    patient->address[99] = '\0';
    patient->birth_date = birth_date;
    strncpy(patient->blood_type, blood_type, 4);
    patient->blood_type[4] = '\0';
    strncpy(patient->gender, gender, 9);
    patient->gender[9] = '\0';
    strncpy(patient->emergency_contact, emergency_contact, 49);
    patient->emergency_contact[49] = '\0';
    strncpy(patient->emergency_phone, emergency_phone, 14);
    patient->emergency_phone[14] = '\0';
    
    // Set registration date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    patient->registration_date.day = current_time->tm_mday;
    patient->registration_date.month = current_time->tm_mon + 1;
    patient->registration_date.year = current_time->tm_year + 1900;
    
    patient->active = 1;
    strncpy(patient->insurance_id, insurance_id, 29);
    patient->insurance_id[29] = '\0';
    strncpy(patient->allergies, allergies, 199);
    patient->allergies[199] = '\0';
    strncpy(patient->medical_history, medical_history, 499);
    patient->medical_history[499] = '\0';
    
    db->patient_count++;
    return patient->id;
}

int add_doctor(HospitalDatabase *db, const char *first_name, const char *last_name,
               const char *email, const char *phone, int department_id,
               const char *specialization, const char *license_number,
               const char *education, double hourly_rate, const char *schedule, const char *office_location) {
    if (db->doctor_count >= MAX_DOCTORS) {
        return -1;
    }
    
    // Validate department exists
    if (find_department_by_id(db, department_id) == -1) {
        return -1;
    }
    
    Doctor *doctor = &db->doctors[db->doctor_count];
    doctor->id = db->doctor_count + 1;
    strncpy(doctor->first_name, first_name, MAX_NAME_LENGTH - 1);
    doctor->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(doctor->last_name, last_name, MAX_NAME_LENGTH - 1);
    doctor->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(doctor->email, email, 49);
    doctor->email[49] = '\0';
    strncpy(doctor->phone, phone, 14);
    doctor->phone[14] = '\0';
    doctor->department_id = department_id;
    strncpy(doctor->specialization, specialization, 49);
    doctor->specialization[49] = '\0';
    strncpy(doctor->license_number, license_number, 29);
    doctor->license_number[29] = '\0';
    
    // Set hire date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    doctor->hire_date.day = current_time->tm_mday;
    doctor->hire_date.month = current_time->tm_mon + 1;
    doctor->hire_date.year = current_time->tm_year + 1900;
    
    doctor->active = 1;
    strncpy(doctor->education, education, 99);
    doctor->education[99] = '\0';
    doctor->hourly_rate = hourly_rate;
    strncpy(doctor->schedule, schedule, 99);
    doctor->schedule[99] = '\0';
    strncpy(doctor->office_location, office_location, 49);
    doctor->office_location[49] = '\0';
    
    db->doctor_count++;
    return doctor->id;
}

int add_department(HospitalDatabase *db, const char *name, const char *description) {
    if (db->department_count >= MAX_DEPARTMENTS) {
        return -1;
    }
    
    Department *department = &db->departments[db->department_count];
    department->id = db->department_count + 1;
    strncpy(department->name, name, 49);
    department->name[49] = '\0';
    strncpy(department->description, description, 199);
    department->description[199] = '\0';
    department->active = 1;
    
    db->department_count++;
    return department->id;
}

int add_appointment(HospitalDatabase *db, int patient_id, int doctor_id,
                  Date appointment_date, const char *time, int duration,
                  const char *type, const char *notes) {
    if (db->appointment_count >= MAX_APPOINTMENTS) {
        return -1;
    }
    
    // Validate patient and doctor exist
    if (find_patient_by_id(db, patient_id) == -1 || find_doctor_by_id(db, doctor_id) == -1) {
        return -1;
    }
    
    Appointment *appointment = &db->appointments[db->appointment_count];
    appointment->id = db->appointment_count + 1;
    appointment->patient_id = patient_id;
    appointment->doctor_id = doctor_id;
    appointment->appointment_date = appointment_date;
    strncpy(appointment->time, time, 9);
    appointment->time[9] = '\0';
    appointment->duration = duration;
    strcpy(appointment->status, "Scheduled");
    strncpy(appointment->notes, notes, 499);
    appointment->notes[499] = '\0';
    strncpy(appointment->type, type, 29);
    appointment->type[29] = '\0';
    appointment->active = 1;
    
    db->appointment_count++;
    return appointment->id;
}

int add_medication(HospitalDatabase *db, const char *name, const char *description,
                  const char *manufacturer, double price, int stock_quantity,
                  int reorder_level, const char *dosage) {
    if (db->medication_count >= MAX_MEDICATIONS) {
        return -1;
    }
    
    Medication *medication = &db->medications[db->medication_count];
    medication->id = db->medication_count + 1;
    strncpy(medication->name, name, 49);
    medication->name[49] = '\0';
    strncpy(medication->description, description, 199);
    medication->description[199] = '\0';
    strncpy(medication->manufacturer, manufacturer, 49);
    medication->manufacturer[49] = '\0';
    medication->price = price;
    medication->stock_quantity = stock_quantity;
    medication->reorder_level = reorder_level;
    strncpy(medication->dosage, dosage, 29);
    medication->dosage[29] = '\0';
    medication->active = 1;
    
    db->medication_count++;
    return medication->id;
}

void update_appointment_status(HospitalDatabase *db, int appointment_id, const char *status) {
    int appointment_index = find_appointment_by_id(db, appointment_id);
    if (appointment_index == -1) {
        printf("Appointment not found.\n");
        return;
    }
    
    Appointment *appointment = &db->appointments[appointment_index];
    
    strcpy(appointment->status, status);
    
    printf("Appointment #%d status updated to '%s'.\n", appointment_id, status);
    printf("Description: %s\n", get_appointment_status_description(status));
}

void prescribe_medication(HospitalDatabase *db, int appointment_id, int medication_id, const char *dosage, int quantity) {
    int appointment_index = find_appointment_by_id(db, appointment_id);
    if (appointment_index == -1) {
        printf("Appointment not found.\n");
        return;
    }
    
    int medication_index = find_medication_by_id(db, medication_id);
    if (medication_index == -1) {
        printf("Medication not found.\n");
        return;
    }
    
    // Add medication info to appointment notes (simplified implementation)
    Appointment *appointment = &db->appointments[appointment_index];
    Medication *medication = &db->medications[medication_index];
    
    char new_note[600];
    sprintf(new_note, "%s\nMedication: %s (%s) - %s, Quantity: %d", 
            appointment->notes, medication->name, medication->dosage, dosage, quantity);
    
    strncpy(appointment->notes, new_note, 499);
    appointment->notes[499] = '\0';
    
    // Update medication stock
    if (medication->stock_quantity >= quantity) {
        medication->stock_quantity -= quantity;
        printf("Medication prescribed successfully.\n");
        printf("Medication: %s\n", medication->name);
        printf("Dosage: %s\n", dosage);
        printf("Quantity: %d\n", quantity);
        printf("Remaining stock: %d\n", medication->stock_quantity);
        
        if (medication->stock_quantity <= medication->reorder_level) {
            printf("Warning: Stock level is at or below reorder level.\n");
        }
    } else {
        printf("Insufficient stock. Only %d available, need %d.\n", 
               medication->stock_quantity, quantity);
    }
}

void display_patient(const Patient *patient) {
    printf("\nPatient ID: %d\n", patient->id);
    printf("Name: %s %s\n", patient->first_name, patient->last_name);
    printf("Email: %s\n", patient->email);
    printf("Phone: %s\n", patient->phone);
    printf("Address: %s\n", patient->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           patient->birth_date.day, patient->birth_date.month, patient->birth_date.year);
    printf("Blood Type: %s\n", patient->blood_type);
    printf("Gender: %s\n", patient->gender);
    printf("Emergency Contact: %s\n", patient->emergency_contact);
    printf("Emergency Phone: %s\n", patient->emergency_phone);
    printf("Registration Date: %02d/%02d/%d\n", 
           patient->registration_date.day, patient->registration_date.month, patient->registration_date.year);
    printf("Insurance ID: %s\n", patient->insurance_id);
    printf("Allergies: %s\n", patient->allergies);
    printf("Medical History: %s\n", patient->medical_history);
    printf("Status: %s\n", patient->active ? "Active" : "Inactive");
}

void display_doctor(const Doctor *doctor, const HospitalDatabase *db) {
    printf("\nDoctor ID: %d\n", doctor->id);
    printf("Name: %s %s\n", doctor->first_name, doctor->last_name);
    printf("Email: %s\n", doctor->email);
    printf("Phone: %s\n", doctor->phone);
    
    // Display department name
    int department_index = find_department_by_id(db, doctor->department_id);
    if (department_index != -1) {
        printf("Department: %s\n", db->departments[department_index].name);
    }
    
    printf("Specialization: %s\n", doctor->specialization);
    printf("License Number: %s\n", doctor->license_number);
    printf("Education: %s\n", doctor->education);
    printf("Hourly Rate: $%.2f\n", doctor->hourly_rate);
    printf("Schedule: %s\n", doctor->schedule);
    printf("Office Location: %s\n", doctor->office_location);
    printf("Hire Date: %02d/%02d/%d\n", 
           doctor->hire_date.day, doctor->hire_date.month, doctor->hire_date.year);
    printf("Status: %s\n", doctor->active ? "Active" : "Inactive");
}

void display_appointment(const Appointment *appointment, const HospitalDatabase *db) {
    printf("\nAppointment ID: %d\n", appointment->id);
    
    // Display patient name
    int patient_index = find_patient_by_id(db, appointment->patient_id);
    if (patient_index != -1) {
        printf("Patient: %s %s (ID: %d)\n", 
               db->patients[patient_index].first_name,
               db->patients[patient_index].last_name,
               appointment->patient_id);
    }
    
    // Display doctor name
    int doctor_index = find_doctor_by_id(db, appointment->doctor_id);
    if (doctor_index != -1) {
        printf("Doctor: %s %s (ID: %d)\n", 
               db->doctors[doctor_index].first_name,
               db->doctors[doctor_index].last_name,
               appointment->doctor_id);
    }
    
    printf("Date: %02d/%02d/%d\n", 
           appointment->appointment_date.day, appointment->appointment_date.month, appointment->appointment_date.year);
    printf("Time: %s\n", appointment->time);
    printf("Duration: %d minutes\n", appointment->duration);
    printf("Type: %s\n", appointment->type);
    printf("Status: %s\n", appointment->status);
    printf("Description: %s\n", get_appointment_status_description(appointment->status));
    printf("Type Description: %s\n", get_appointment_type_description(appointment->type));
    printf("Notes: %s\n", appointment->notes);
}

void display_department(const Department *department) {
    printf("\nDepartment ID: %d\n", department->id);
    printf("Name: %s\n", department->name);
    printf("Description: %s\n", department->description);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
}

void display_medication(const Medication *medication) {
    printf("\nMedication ID: %d\n", medication->id);
    printf("Name: %s\n", medication->name);
    printf("Description: %s\n", medication->description);
    printf("Manufacturer: %s\n", medication->manufacturer);
    printf("Price: $%.2f\n", medication->price);
    printf("Stock Quantity: %d\n", medication->stock_quantity);
    printf("Reorder Level: %d\n", medication->reorder_level);
    printf("Dosage: %s\n", medication->dosage);
    printf("Status: %s\n", medication->active ? "Active" : "Inactive");
}

void display_all_patients(const HospitalDatabase *db) {
    printf("\n=== All Patients ===\n");
    printf("Total Patients: %d\n\n", db->patient_count);
    
    for (int i = 0; i < db->patient_count; i++) {
        if (db->patients[i].active) {
            display_patient(&db->patients[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_doctors(const HospitalDatabase *db) {
    printf("\n=== All Doctors ===\n");
    printf("Total Doctors: %d\n\n", db->doctor_count);
    
    for (int i = 0; i < db->doctor_count; i++) {
        if (db->doctors[i].active) {
            display_doctor(&db->doctors[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_appointments(const HospitalDatabase *db) {
    printf("\n=== All Appointments ===\n");
    printf("Total Appointments: %d\n\n", db->appointment_count);
    
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].active) {
            display_appointment(&db->appointments[i], db);
            printf("========================\n");
        }
    }
}

void display_all_departments(const HospitalDatabase *db) {
    printf("\n=== All Departments ===\n");
    printf("Total Departments: %d\n\n", db->department_count);
    
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].active) {
            display_department(&db->departments[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_medications(const HospitalDatabase *db) {
    printf("\n=== All Medications ===\n");
    printf("Total Medications: %d\n\n", db->medication_count);
    
    for (int i = 0; i < db->medication_count; i++) {
        if (db->medications[i].active) {
            display_medication(&db->medications[i]);
            printf("------------------------\n");
        }
    }
}

void search_patients_by_name(const HospitalDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->patient_count; i++) {
        if (db->patients[i].active && 
            (strstr(db->patients[i].first_name, name) != NULL || 
             strstr(db->patients[i].last_name, name) != NULL)) {
            display_patient(&db->patients[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No patients found with name containing '%s'.\n", name);
    }
}

void search_doctors_by_specialization(const HospitalDatabase *db, const char *specialization) {
    printf("\nSearch Results for '%s':\n", specialization);
    int found = 0;
    
    for (int i = 0; i < db->doctor_count; i++) {
        if (db->doctors[i].active && 
            strstr(db->doctors[i].specialization, specialization) != NULL) {
            display_doctor(&db->doctors[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No doctors found with specialization containing '%s'.\n", specialization);
    }
}

void search_appointments_by_date(const HospitalDatabase *db, int day, int month, int year) {
    Date search_date = {day, month, year};
    
    printf("\nSearch Results for %02d/%02d/%d:\n", day, month, year);
    int found = 0;
    
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].active && 
            db->appointments[i].appointment_date.day == day &&
            db->appointments[i].appointment_date.month == month &&
            db->appointments[i].appointment_date.year == year) {
            display_appointment(&db->appointments[i], db);
            printf("========================\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No appointments found for this date.\n");
    }
}

void search_appointments_by_doctor(const HospitalDatabase *db, int doctor_id) {
    int doctor_index = find_doctor_by_id(db, doctor_id);
    if (doctor_index == -1) {
        printf("Doctor not found.\n");
        return;
    }
    
    printf("\nSearch Results for Doctor: %s %s (ID: %d)\n", 
           db->doctors[doctor_index].first_name,
           db->doctors[doctor_index].last_name,
           doctor_id);
    
    int found = 0;
    
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].active && db->appointments[i].doctor_id == doctor_id) {
            display_appointment(&db->appointments[i], db);
            printf("========================\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No appointments found for this doctor.\n");
    }
}

void search_appointments_by_patient(const HospitalDatabase *db, int patient_id) {
    int patient_index = find_patient_by_id(db, patient_id);
    if (patient_index == -1) {
        printf("Patient not found.\n");
        return;
    }
    
    printf("\nSearch Results for Patient: %s %s (ID: %d)\n", 
           db->patients[patient_index].first_name,
           db->patients[patient_index].last_name,
           patient_id);
    
    int found = 0;
    
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].active && db->appointments[i].patient_id == patient_id) {
            display_appointment(&db->appointments[i], db);
            printf("========================\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No appointments found for this patient.\n");
    }
}

void generate_patient_report(const HospitalDatabase *db, int patient_id) {
    int patient_index = find_patient_by_id(db, patient_id);
    if (patient_index == -1) {
        printf("Patient not found.\n");
        return;
    }
    
    Patient *patient = &db->patients[patient_index];
    
    printf("\n=== Patient Report: %s %s ===\n", patient->first_name, patient->last_name);
    printf("Patient ID: %d\n", patient->id);
    printf("Email: %s\n", patient->email);
    printf("Phone: %s\n", patient->phone);
    printf("Address: %s\n", patient->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           patient->birth_date.day, patient->birth_date.month, patient->birth_date.year);
    printf("Blood Type: %s\n", patient->blood_type);
    printf("Gender: %s\n", patient->gender);
    printf("Emergency Contact: %s\n", patient->emergency_contact);
    printf("Emergency Phone: %s\n", patient->emergency_phone);
    printf("Registration Date: %02d/%02d/%d\n", 
           patient->registration_date.day, patient->registration_date.month, patient->registration_date.year);
    printf("Insurance ID: %s\n", patient->insurance_id);
    printf("Allergies: %s\n", patient->allergies);
    printf("Medical History: %s\n", patient->medical_history);
    printf("Status: %s\n", patient->active ? "Active" : "Inactive");
    
    // Calculate age
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_year = current_time->tm_year + 1900;
    int age = current_year - patient->birth_date.year;
    
    // Adjust age if birthday hasn't occurred this year
    if (current_time->tm_mon + 1 < patient->birth_date.month || 
        (current_time->tm_mon + 1 == patient->birth_date.month && 
         current_time->tm_mday < patient->birth_date.day)) {
        age--;
    }
    
    printf("\nAge: %d\n", age);
    
    // Appointment history
    printf("\nAppointment History:\n");
    printf("%-5s %-15s %-15s %-12s %-10s %-15s\n", 
           "ID", "Date", "Doctor", "Time", "Duration", "Type", "Status");
    printf("-----------------------------------------------------------------\n");
    
    int appointment_count = 0;
    
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].active && db->appointments[i].patient_id == patient_id) {
            int doctor_index = find_doctor_by_id(db, db->appointments[i].doctor_id);
            if (doctor_index != -1) {
                printf("%-5d %02d/%02d/%d %-15s %-15s %-12s %-10d %-15s %-15s\n", 
                       db->appointments[i].id,
                       db->appointments[i].appointment_date.day, 
                       db->appointments[i].appointment_date.month, 
                       db->appointments[i].appointment_date.year,
                       db->doctors[doctor_index].first_name,
                       db->appointments[i].time,
                       db->appointments[i].duration,
                       db->appointments[i].type,
                       db->appointments[i].status);
                
                appointment_count++;
            }
        }
    }
    
    printf("-----------------------------------------------------------------\n");
    printf("Total Appointments: %d\n", appointment_count);
    
    if (appointment_count > 0) {
        // Calculate statistics
        int completed_count = 0;
        int cancelled_count = 0;
        int scheduled_count = 0;
        
        for (int i = 0; i < db->appointment_count; i++) {
            if (db->appointments[i].active && db->appointments[i].patient_id == patient_id) {
                if (strcmp(db->appointments[i].status, "Completed") == 0) {
                    completed_count++;
                } else if (strcmp(db->appointments[i].status, "Cancelled") == 0) {
                    cancelled_count++;
                } else if (strcmp(db->appointments[i].status, "Scheduled") == 0) {
                    scheduled_count++;
                }
            }
        }
        
        printf("\nAppointment Statistics:\n");
        printf("Completed: %d (%.1f%%)\n", completed_count, (float)completed_count / appointment_count * 100);
        printf("Cancelled: %d (%.1f%%)\n", cancelled_count, (float)cancelled_count / appointment_count * 100);
        printf("Scheduled: %d (%.1f%%)\n", scheduled_count, (float)scheduled_count / appointment_count * 100);
    }
}

void generate_doctor_report(const HospitalDatabase *db, int doctor_id) {
    int doctor_index = find_doctor_by_id(db, doctor_id);
    if (doctor_index == -1) {
        printf("Doctor not found.\n");
        return;
    }
    
    Doctor *doctor = &db->doctors[doctor_index];
    
    printf("\n=== Doctor Report: %s %s ===\n", doctor->first_name, doctor->last_name);
    printf("Doctor ID: %d\n", doctor->id);
    printf("Email: %s\n", doctor->email);
    printf("Phone: %s\n", doctor->phone);
    
    // Display department name
    int department_index = find_department_by_id(db, doctor->department_id);
    if (department_index != -1) {
        printf("Department: %s\n", db->departments[department_index].name);
    }
    
    printf("Specialization: %s\n", doctor->specialization);
    printf("License Number: %s\n", doctor->license_number);
    printf("Education: %s\n", doctor->education);
    printf("Hourly Rate: $%.2f\n", doctor->hourly_rate);
    printf("Schedule: %s\n", doctor->schedule);
    printf("Office Location: %s\n", doctor->office_location);
    printf("Hire Date: %02d/%02d/%d\n", 
           doctor->hire_date.day, doctor->hire_date.month, doctor->hire_date.year);
    printf("Status: %s\n", doctor->active ? "Active" : "Inactive");
    
    // Appointment summary
    printf("\nAppointment Summary:\n");
    printf("%-5s %-12s %-15s %-10s %-10s %-15s\n", 
           "ID", "Date", "Patient", "Time", "Duration", "Type", "Status");
    printf("---------------------------------------------------------\n");
    
    int appointment_count = 0;
    double total_hours = 0.0;
    
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].active && db->appointments[i].doctor_id == doctor_id) {
            int patient_index = find_patient_by_id(db, db->appointments[i].patient_id);
            if (patient_index != -1) {
                printf("%-5d %02d/%02d/%d %-15s %-10s %-10d %-15s %-15s\n", 
                       db->appointments[i].id,
                       db->appointments[i].appointment_date.day, 
                       db->appointments[i].appointment_date.month, 
                       db->appointments[i].appointment_date.year,
                       db->patients[patient_index].first_name,
                       db->appointments[i].time,
                       db->appointments[i].duration,
                       db->appointments[i].type,
                       db->appointments[i].status);
                
                appointment_count++;
                total_hours += db->appointments[i].duration / 60.0;
            }
        }
    }
    
    printf("---------------------------------------------------------\n");
    printf("Total Appointments: %d\n", appointment_count);
    
    if (appointment_count > 0) {
        printf("Total Hours: %.1f\n", total_hours);
        printf("Average Appointment Duration: %.1f minutes\n", total_hours * 60 / appointment_count);
        printf("Potential Earnings: $%.2f\n", total_hours * doctor->hourly_rate);
        
        // Calculate statistics
        int completed_count = 0;
        int cancelled_count = 0;
        int scheduled_count = 0;
        
        for (int i = 0; i < db->appointment_count; i++) {
            if (db->appointments[i].active && db->appointments[i].doctor_id == doctor_id) {
                if (strcmp(db->appointments[i].status, "Completed") == 0) {
                    completed_count++;
                } else if (strcmp(db->appointments[i].status, "Cancelled") == 0) {
                    cancelled_count++;
                } else if (strcmp(db->appointments[i].status, "Scheduled") == 0) {
                    scheduled_count++;
                }
            }
        }
        
        printf("\nAppointment Statistics:\n");
        printf("Completed: %d (%.1f%%)\n", completed_count, (float)completed_count / appointment_count * 100);
        printf("Cancelled: %d (%.1f%%)\n", cancelled_count, (float)cancelled_count / appointment_count * 100);
        printf("Scheduled: %d (%.1f%%)\n", scheduled_count, (float)scheduled_count / appointment_count * 100);
    }
}

void generate_department_report(const HospitalDatabase *db, int department_id) {
    int department_index = find_department_by_id(db, department_id);
    if (department_index == -1) {
        printf("Department not found.\n");
        return;
    }
    
    Department *department = &db->departments[department_index];
    
    printf("\n=== Department Report: %s ===\n", department->name);
    printf("Department ID: %d\n", department->id);
    printf("Description: %s\n", department->description);
    printf("Status: %s\n", department->active ? "Active" : "Inactive");
    
    // Doctor summary
    printf("\nDoctor Summary:\n");
    printf("%-5s %-20s %-20s %-20s %-15s\n", 
           "ID", "Name", "Specialization", "Email", "Phone");
    printf("-------------------------------------------------------\n");
    
    int doctor_count = 0;
    
    for (int i = 0; i < db->doctor_count; i++) {
        if (db->doctors[i].active && db->doctors[i].department_id == department_id) {
            printf("%-5d %-20s %-20s %-20s %-15s\n", 
                   db->doctors[i].id,
                   db->doctors[i].first_name,
                   db->doctors[i].specialization,
                   db->doctors[i].email,
                   db->doctors[i].phone);
            
            doctor_count++;
        }
    }
    
    printf("-------------------------------------------------------\n");
    printf("Total Doctors: %d\n", doctor_count);
    
    // Department statistics
    if (doctor_count > 0) {
        printf("\nDepartment Statistics:\n");
        
        // Calculate average hourly rate
        double total_rate = 0.0;
        for (int i = 0; i < db->doctor_count; i++) {
            if (db->doctors[i].active && db->doctors[i].department_id == department_id) {
                total_rate += db->doctors[i].hourly_rate;
            }
        }
        
        printf("Average Hourly Rate: $%.2f\n", total_rate / doctor_count);
        
        // Calculate total appointments
        int total_appointments = 0;
        for (int i = 0; i < db->appointment_count; i++) {
            if (db->appointments[i].active) {
                int doctor_index = find_doctor_by_id(db, db->appointments[i].doctor_id);
                if (doctor_index != -1 && db->doctors[doctor_index].department_id == department_id) {
                    total_appointments++;
                }
            }
        }
        
        printf("Total Appointments: %d\n", total_appointments);
        printf("Average Appointments per Doctor: %.1f\n", (float)total_appointments / doctor_count);
    }
}

void generate_appointment_report(const HospitalDatabase *db, const Date *start_date, const Date *end_date) {
    printf("\n=== Appointment Report ===\n");
    printf("Period: %02d/%02d/%d to %02d/%02d/%d\n", 
           start_date->day, start_date->month, start_date->year,
           end_date->day, end_date->month, end_date->year);
    
    printf("\nAppointments within date range:\n");
    printf("%-5s %-15s %-15s %-12s %-15s %-15s\n", 
           "ID", "Date", "Patient", "Doctor", "Type", "Status");
    printf("-----------------------------------------------------------------\n");
    
    int appointment_count = 0;
    int completed_count = 0;
    int cancelled_count = 0;
    int scheduled_count = 0;
    
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].active) {
            // Check if appointment date is within range
            int app_timestamp = db->appointments[i].appointment_date.year * 10000 + 
                              db->appointments[i].appointment_date.month * 100 + 
                              db->appointments[i].appointment_date.day;
            
            int start_timestamp = start_date->year * 10000 + 
                                start_date->month * 100 + 
                                start_date->day;
            
            int end_timestamp = end_date->year * 10000 + 
                              end_date->month * 100 + 
                              end_date->day;
            
            if (app_timestamp >= start_timestamp && app_timestamp <= end_timestamp) {
                int patient_index = find_patient_by_id(db, db->appointments[i].patient_id);
                int doctor_index = find_doctor_by_id(db, db->appointments[i].doctor_id);
                
                if (patient_index != -1 && doctor_index != -1) {
                    printf("%-5d %02d/%02d/%d %-15s %-15s %-15s %-15s\n", 
                           db->appointments[i].id,
                           db->appointments[i].appointment_date.day, 
                           db->appointments[i].appointment_date.month, 
                           db->appointments[i].appointment_date.year,
                           db->patients[patient_index].first_name,
                           db->doctors[doctor_index].first_name,
                           db->appointments[i].type,
                           db->appointments[i].status);
                    
                    appointment_count++;
                    
                    if (strcmp(db->appointments[i].status, "Completed") == 0) {
                        completed_count++;
                    } else if (strcmp(db->appointments[i].status, "Cancelled") == 0) {
                        cancelled_count++;
                    } else if (strcmp(db->appointments[i].status, "Scheduled") == 0) {
                        scheduled_count++;
                    }
                }
            }
        }
    }
    
    printf("-----------------------------------------------------------------\n");
    printf("Total Appointments: %d\n", appointment_count);
    
    if (appointment_count > 0) {
        printf("\nAppointment Statistics:\n");
        printf("Completed: %d (%.1f%%)\n", completed_count, (float)completed_count / appointment_count * 100);
        printf("Cancelled: %d (%.1f%%)\n", cancelled_count, (float)cancelled_count / appointment_count * 100);
        printf("Scheduled: %d (%.1f%%)\n", scheduled_count, (float)scheduled_count / appointment_count * 100);
        
        // Type breakdown
        int consultation_count = 0;
        int followup_count = 0;
        int emergency_count = 0;
        int surgery_count = 0;
        
        for (int i = 0; i < db->appointment_count; i++) {
            if (db->appointments[i].active) {
                // Check if appointment date is within range
                int app_timestamp = db->appointments[i].appointment_date.year * 10000 + 
                                  db->appointments[i].appointment_date.month * 100 + 
                                  db->appointments[i].appointment_date.day;
                
                int start_timestamp = start_date->year * 10000 + 
                                    start_date->month * 100 + 
                                    start_date->day;
                
                int end_timestamp = end_date->year * 10000 + 
                                  end_date->month * 100 + 
                                  end_date->day;
                
                if (app_timestamp >= start_timestamp && app_timestamp <= end_timestamp) {
                    if (strcmp(db->appointments[i].type, "Consultation") == 0) {
                        consultation_count++;
                    } else if (strcmp(db->appointments[i].type, "Follow-up") == 0) {
                        followup_count++;
                    } else if (strcmp(db->appointments[i].type, "Emergency") == 0) {
                        emergency_count++;
                    } else if (strcmp(db->appointments[i].type, "Surgery") == 0) {
                        surgery_count++;
                    }
                }
            }
        }
        
        printf("\nAppointment Type Breakdown:\n");
        printf("Consultation: %d (%.1f%%)\n", consultation_count, (float)consultation_count / appointment_count * 100);
        printf("Follow-up: %d (%.1f%%)\n", followup_count, (float)followup_count / appointment_count * 100);
        printf("Emergency: %d (%.1f%%)\n", emergency_count, (float)emergency_count / appointment_count * 100);
        printf("Surgery: %d (%.1f%%)\n", surgery_count, (float)surgery_count / appointment_count * 100);
    }
}

void generate_inventory_report(const HospitalDatabase *db) {
    printf("\n=== Inventory Report ===\n");
    
    int total_medications = 0;
    int total_stock = 0;
    int low_stock_items = 0;
    double total_value = 0.0;
    
    printf("%-5s %-20s %-20s %-10s %-10s %-15s\n", 
           "ID", "Name", "Manufacturer", "Stock", "Reorder", "Value");
    printf("-----------------------------------------------------------------------\n");
    
    for (int i = 0; i < db->medication_count; i++) {
        if (db->medications[i].active) {
            double item_value = db->medications[i].price * db->medications[i].stock_quantity;
            
            printf("%-5d %-20s %-20s %-10d %-10d $%-14.2f\n", 
                   db->medications[i].id,
                   db->medications[i].name,
                   db->medications[i].manufacturer,
                   db->medications[i].stock_quantity,
                   db->medications[i].reorder_level,
                   item_value);
            
            total_medications++;
            total_stock += db->medications[i].stock_quantity;
            total_value += item_value;
            
            if (db->medications[i].stock_quantity <= db->medications[i].reorder_level) {
                low_stock_items++;
            }
        }
    }
    
    printf("-----------------------------------------------------------------------\n");
    printf("Total Medications: %d\n", total_medications);
    printf("Total Stock: %d\n", total_stock);
    printf("Total Value: $%.2f\n", total_value);
    printf("Low Stock Items: %d\n", low_stock_items);
    printf("Average Stock per Medication: %.1f\n", total_medications > 0 ? (float)total_stock / total_medications : 0.0);
    
    // Low stock warning
    if (low_stock_items > 0) {
        printf("\nLow Stock Items:\n");
        printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Stock", "Reorder");
        printf("------------------------------------------------\n");
        
        for (int i = 0; i < db->medication_count; i++) {
            if (db->medications[i].active && 
                db->medications[i].stock_quantity <= db->medications[i].reorder_level) {
                printf("%-5d %-20s %-10d %-10d\n", 
                       db->medications[i].id,
                       db->medications[i].name,
                       db->medications[i].stock_quantity,
                       db->medications[i].reorder_level);
            }
        }
    }
}

int find_patient_by_id(const HospitalDatabase *db, int patient_id) {
    for (int i = 0; i < db->patient_count; i++) {
        if (db->patients[i].id == patient_id && db->patients[i].active) {
            return i;
        }
    }
    return -1;
}

int find_doctor_by_id(const HospitalDatabase *db, int doctor_id) {
    for (int i = 0; i < db->doctor_count; i++) {
        if (db->doctors[i].id == doctor_id && db->doctors[i].active) {
            return i;
        }
    }
    return -1;
}

int find_appointment_by_id(const HospitalDatabase *db, int appointment_id) {
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].id == appointment_id && db->appointments[i].active) {
            return i;
        }
    }
    return -1;
}

int find_department_by_id(const HospitalDatabase *db, int department_id) {
    for (int i = 0; i < db->department_count; i++) {
        if (db->departments[i].id == department_id && db->departments[i].active) {
            return i;
        }
    }
    return -1;
}

int find_medication_by_id(const HospitalDatabase *db, int medication_id) {
    for (int i = 0; i < db->medication_count; i++) {
        if (db->medications[i].id == medication_id && db->medications[i].active) {
            return i;
        }
    }
    return -1;
}

int save_database(const HospitalDatabase *db) {
    FILE *patients_file = fopen(FILENAME_PATIENTS, "wb");
    if (patients_file != NULL) {
        fwrite(&db->patient_count, sizeof(int), 1, patients_file);
        fwrite(db->patients, sizeof(Patient), db->patient_count, patients_file);
        fclose(patients_file);
    } else {
        return 0;
    }
    
    FILE *doctors_file = fopen(FILENAME_DOCTORS, "wb");
    if (doctors_file != NULL) {
        fwrite(&db->doctor_count, sizeof(int), 1, doctors_file);
        fwrite(db->doctors, sizeof(Doctor), db->doctor_count, doctors_file);
        fclose(doctors_file);
    } else {
        return 0;
    }
    
    FILE *departments_file = fopen(FILENAME_DEPARTMENTS, "wb");
    if (departments_file != NULL) {
        fwrite(&db->department_count, sizeof(int), 1, departments_file);
        fwrite(db->departments, sizeof(Department), db->department_count, departments_file);
        fclose(departments_file);
    } else {
        return 0;
    }
    
    FILE *appointments_file = fopen(FILENAME_APPOINTMENTS, "wb");
    if (appointments_file != NULL) {
        fwrite(&db->appointment_count, sizeof(int), 1, appointments_file);
        fwrite(db->appointments, sizeof(Appointment), db->appointment_count, appointments_file);
        fclose(appointments_file);
    } else {
        return 0;
    }
    
    FILE *medications_file = fopen(FILENAME_MEDICATIONS, "wb");
    if (medications_file != NULL) {
        fwrite(&db->medication_count, sizeof(int), 1, medications_file);
        fwrite(db->medications, sizeof(Medication), db->medication_count, medications_file);
        fclose(medications_file);
        return 1;
    }
    
    return 0;
}

int load_database(HospitalDatabase *db) {
    FILE *patients_file = fopen(FILENAME_PATIENTS, "rb");
    if (patients_file != NULL) {
        fread(&db->patient_count, sizeof(int), 1, patients_file);
        fread(db->patients, sizeof(Patient), db->patient_count, patients_file);
        fclose(patients_file);
    } else {
        return 0;
    }
    
    FILE *doctors_file = fopen(FILENAME_DOCTORS, "rb");
    if (doctors_file != NULL) {
        fread(&db->doctor_count, sizeof(int), 1, doctors_file);
        fread(db->doctors, sizeof(Doctor), db->doctor_count, doctors_file);
        fclose(doctors_file);
    } else {
        return 0;
    }
    
    FILE *departments_file = fopen(FILENAME_DEPARTMENTS, "rb");
    if (departments_file != NULL) {
        fread(&db->department_count, sizeof(int), 1, departments_file);
        fread(db->departments, sizeof(Department), db->department_count, departments_file);
        fclose(departments_file);
    } else {
        return 0;
    }
    
    FILE *appointments_file = fopen(FILENAME_APPOINTMENTS, "rb");
    if (appointments_file != NULL) {
        fread(&db->appointment_count, sizeof(int), 1, appointments_file);
        fread(db->appointments, sizeof(Appointment), db->appointment_count, appointments_file);
        fclose(appointments_file);
    } else {
        return 0;
    }
    
    FILE *medications_file = fopen(FILENAME_MEDICATIONS, "rb");
    if (medications_file != NULL) {
        fread(&db->medication_count, sizeof(int), 1, medications_file);
        fread(db->medications, sizeof(Medication), db->medication_count, medications_file);
        fclose(medications_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(HospitalDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* department_names[] = {"Emergency", "Cardiology", "Neurology", "Pediatrics", 
                               "Surgery", "Radiology", "Oncology", "Orthopedics",
                               "Psychiatry", "Gynecology"};
    
    char* specializations[] = {"General Medicine", "Cardiologist", "Neurologist", "Pediatrician",
                               "Surgeon", "Radiologist", "Oncologist", "Orthopedist",
                               "Psychiatrist", "Gynecologist", "Anesthesiologist", "Pathologist"};
    
    char* medication_names[] = {"Aspirin", "Ibuprofen", "Paracetamol", "Amoxicillin", 
                                "Lisinopril", "Metformin", "Atorvastatin", "Omeprazole",
                                "Sertraline", "Albuterol", "Insulin", "Warfarin"};
    
    char* manufacturers[] = {"Pfizer", "Johnson & Johnson", "Novartis", "Roche",
                           "Merck", "GlaxoSmithKline", "Sanofi", "AstraZeneca"};
    
    char* allergies[] = {"None", "Penicillin", "Latex", "Pollen", "Dust", "Nuts",
                        "Dairy", "Eggs", "Shellfish", "Soy", "Gluten"};
    
    srand(time(NULL));
    
    // Add departments
    for (int i = 0; i < 10 && db->department_count < MAX_DEPARTMENTS; i++) {
        add_department(db, department_names[i], "Department responsible for specialized medical care");
    }
    
    // Add doctors
    for (int i = 0; i < 50 && db->doctor_count < MAX_DOCTORS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], license_number[30], education[100];
        sprintf(email, "%s.%d@hospital.com", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(license_number, "MD%05d", 10000 + i);
        sprintf(education, "MD from %s University", department_names[rand() % 10]);
        
        int department_id = 1 + (rand() % 10);
        double hourly_rate = 50.0 + (rand() % 100) + (rand() % 100) / 100.0;
        char schedule[100], office_location[50];
        sprintf(schedule, "Mon-Fri %dam-%dpm", 8 + (rand() % 3), 17 + (rand() % 3));
        sprintf(office_location, "Room %d, Floor %d", 100 + rand() % 100, 1 + (rand() % 10));
        
        add_doctor(db, first_name, last_name, email, phone, department_id,
                  specializations[rand() % 12], license_number, education, hourly_rate, 
                  schedule, office_location);
    }
    
    // Add patients
    for (int i = 0; i < 500 && db->patient_count < MAX_PATIENTS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], address[100], blood_type[5], gender[10];
        char emergency_contact[50], emergency_phone[15], insurance_id[30];
        char patient_allergies[200], medical_history[500];
        
        sprintf(email, "%s.%d@patient.com", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Patient St, City, State", 100 + rand() % 900);
        
        const char* blood_types[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
        strcpy(blood_type, blood_types[rand() % 8]);
        
        const char* genders[] = {"Male", "Female", "Other"};
        strcpy(gender, genders[rand() % 3]);
        
        sprintf(emergency_contact, "%s %s", first_names[rand() % 24], last_names[rand() % 18]);
        sprintf(emergency_phone, "555-%04d", rand() % 10000);
        sprintf(insurance_id, "INS%08d", 10000000 + i);
        
        strcpy(patient_allergies, allergies[rand() % 11]);
        sprintf(medical_history, "History includes %s and %s", 
                allergies[rand() % 11], allergies[rand() % 11]);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1950 + (rand() % 70);
        Date birth_date = {day, month, year};
        
        add_patient(db, first_name, last_name, email, phone, address, birth_date, 
                  blood_type, gender, emergency_contact, emergency_phone, insurance_id,
                  patient_allergies, medical_history);
    }
    
    // Add medications
    for (int i = 0; i < 100 && db->medication_count < MAX_MEDICATIONS; i++) {
        char description[200], dosage[30];
        sprintf(description, "Medication for treating various conditions");
        sprintf(dosage, "%dmg", 50 + (rand() % 450));
        
        double price = 5.0 + (rand() % 100) + (rand() % 100) / 100.0;
        int stock_quantity = 10 + (rand() % 1000);
        int reorder_level = 5 + (rand() % 50);
        
        add_medication(db, medication_names[rand() % 12], description, 
                     manufacturers[rand() % 8], price, stock_quantity, reorder_level, dosage);
    }
    
    // Add appointments
    for (int i = 0; i < 1000 && db->appointment_count < MAX_APPOINTMENTS; i++) {
        int patient_id = 1 + (rand() % 500);
        int doctor_id = 1 + (rand() % 50);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 2023 + (rand() % 2);
        Date appointment_date = {day, month, year};
        
        char time[10];
        sprintf(time, "%02d:%02d", 8 + (rand() % 10), rand() % 60);
        
        int duration = 15 + (rand() % 165); // 15-180 minutes
        const char* types[] = {"Consultation", "Follow-up", "Emergency", "Surgery"};
        const char* type = types[rand() % 4];
        
        char notes[500];
        sprintf(notes, "Patient complaint: %s", allergies[rand() % 11]);
        
        add_appointment(db, patient_id, doctor_id, appointment_date, time, duration, type, notes);
    }
    
    // Update some appointment statuses
    for (int i = 0; i < 200; i++) {
        int appointment_index = rand() % db->appointment_count;
        if (db->appointments[appointment_index].active) {
            const char* statuses[] = {"Scheduled", "Completed", "Cancelled"};
            int status_index = rand() % 3;
            
            // Convert to lowercase for comparison
            if (status_index == 0) strcpy(db->appointments[appointment_index].status, "Scheduled");
            else if (status_index == 1) strcpy(db->appointments[appointment_index].status, "Completed");
            else strcpy(db->appointments[appointment_index].status, "Cancelled");
        }
    }
}

void update_patient_info(HospitalDatabase *db, int patient_id, const char *email, const char *phone, const char *address) {
    int patient_index = find_patient_by_id(db, patient_id);
    if (patient_index == -1) {
        printf("Patient not found.\n");
        return;
    }
    
    Patient *patient = &db->patients[patient_index];
    
    strncpy(patient->email, email, 49);
    patient->email[49] = '\0';
    strncpy(patient->phone, phone, 14);
    patient->phone[14] = '\0';
    strncpy(patient->address, address, 99);
    patient->address[99] = '\0';
    
    printf("Patient information updated.\n");
    printf("Patient: %s %s\n", patient->first_name, patient->last_name);
    printf("New Email: %s\n", patient->email);
    printf("New Phone: %s\n", patient->phone);
    printf("New Address: %s\n", patient->address);
}

void update_doctor_info(HospitalDatabase *db, int doctor_id, const char *email, const char *phone, const char *specialization) {
    int doctor_index = find_doctor_by_id(db, doctor_id);
    if (doctor_index == -1) {
        printf("Doctor not found.\n");
        return;
    }
    
    Doctor *doctor = &db->doctors[doctor_index];
    
    strncpy(doctor->email, email, 49);
    doctor->email[49] = '\0';
    strncpy(doctor->phone, phone, 14);
    doctor->phone[14] = '\0';
    strncpy(doctor->specialization, specialization, 49);
    doctor->specialization[49] = '\0';
    
    printf("Doctor information updated.\n");
    printf("Doctor: %s %s\n", doctor->first_name, doctor->last_name);
    printf("New Email: %s\n", doctor->email);
    printf("New Phone: %s\n", doctor->phone);
    printf("New Specialization: %s\n", doctor->specialization);
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

int is_valid_blood_type(const char *blood_type) {
    if (blood_type == NULL) return 0;
    
    return (strcmp(blood_type, "A+") == 0 || strcmp(blood_type, "A-") == 0 ||
            strcmp(blood_type, "B+") == 0 || strcmp(blood_type, "B-") == 0 ||
            strcmp(blood_type, "AB+") == 0 || strcmp(blood_type, "AB-") == 0 ||
            strcmp(blood_type, "O+") == 0 || strcmp(blood_type, "O-") == 0);
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

const char* get_appointment_status_description(const char *status) {
    if (strcmp(status, "Scheduled") == 0) return "Appointment is scheduled and waiting to happen";
    if (strcmp(status, "Completed") == 0) return "Appointment has been completed successfully";
    if (strcmp(status, "Cancelled") == 0) return "Appointment was cancelled by patient or doctor";
    return "Unknown status";
}

const char* get_appointment_type_description(const char *type) {
    if (strcmp(type, "Consultation") == 0) return "General consultation with a doctor";
    if (strcmp(type, "Follow-up") == 0) return "Follow-up appointment after initial consultation";
    if (strcmp(type, "Emergency") == 0) return "Emergency medical appointment";
    if (strcmp(type, "Surgery") == 0) return "Scheduled surgical procedure";
    return "Unknown type";
}