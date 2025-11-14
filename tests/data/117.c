#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_PATIENTS 500
#define MAX_DOCTORS 100
#define MAX_APPOINTMENTS 1000
#define MAX_TREATMENTS 2000
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define FILENAME_PATIENTS "patients.dat"
#define FILENAME_DOCTORS "doctors.dat"
#define FILENAME_APPOINTMENTS "appointments.dat"
#define FILENAME_TREATMENTS "treatments.dat"

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
    char address[MAX_ADDRESS_LENGTH];
    char phone[15];
    char email[50];
    Date birth_date;
    char gender; // 'M', 'F', or 'O'
    char blood_type[5]; // A+, A-, B+, B-, AB+, AB-, O+, O-
    char medical_history[500];
    char allergies[200];
    char insurance_info[100];
    int emergency_contact_id;
    Date registration_date;
    int active;
} Patient;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char specialization[50];
    char phone[15];
    char email[50];
    char address[MAX_ADDRESS_LENGTH];
    int years_of_experience;
    char education[200];
    char license_number[30];
    float consultation_fee;
    int available_days[7]; // 0 = Sunday, 1 = Monday, etc.
    int available_start_time[2]; // hour, minute
    int available_end_time[2]; // hour, minute
    int active;
} Doctor;

typedef enum {
    SCHEDULED,
    COMPLETED,
    CANCELLED,
    NO_SHOW
} AppointmentStatus;

typedef struct {
    int id;
    int patient_id;
    int doctor_id;
    DateTime appointment_time;
    int duration_minutes;
    AppointmentStatus status;
    char notes[200];
    float fee_paid;
} Appointment;

typedef enum {
    CONSULTATION,
    TEST,
    SURGERY,
    MEDICATION,
    THERAPY,
    VACCINATION,
    CHECKUP,
    EMERGENCY
} TreatmentType;

typedef struct {
    int id;
    int patient_id;
    int doctor_id;
    int appointment_id; // 0 if not associated with an appointment
    Date treatment_date;
    TreatmentType type;
    char description[300];
    char medication[200];
    char instructions[200];
    float cost;
    int paid;
} Treatment;

typedef struct {
    Patient patients[MAX_PATIENTS];
    int patient_count;
    Doctor doctors[MAX_DOCTORS];
    int doctor_count;
    Appointment appointments[MAX_APPOINTMENTS];
    int appointment_count;
    Treatment treatments[MAX_TREATMENTS];
    int treatment_count;
} HospitalDatabase;

// Function prototypes
void initialize_database(HospitalDatabase *db);
int add_patient(HospitalDatabase *db, const char *name, const char *address, 
                 const char *phone, const char *email, Date birth_date, 
                 char gender, const char *blood_type, const char *medical_history,
                 const char *allergies, const char *insurance_info);
int add_doctor(HospitalDatabase *db, const char *name, const char *specialization,
                const char *phone, const char *email, const char *address,
                int years_of_experience, const char *education,
                const char *license_number, float consultation_fee);
int schedule_appointment(HospitalDatabase *db, int patient_id, int doctor_id,
                        DateTime appointment_time, int duration_minutes);
int add_treatment(HospitalDatabase *db, int patient_id, int doctor_id,
                  int appointment_id, TreatmentType type, const char *description,
                  const char *medication, const char *instructions, float cost);
void complete_appointment(HospitalDatabase *db, int appointment_id);
void cancel_appointment(HospitalDatabase *db, int appointment_id);
void display_patient(const Patient *patient);
void display_doctor(const Doctor *doctor);
void display_appointment(const Appointment *appointment, const HospitalDatabase *db);
void display_treatment(const Treatment *treatment, const HospitalDatabase *db);
void display_all_patients(const HospitalDatabase *db);
void display_all_doctors(const HospitalDatabase *db);
void display_all_appointments(const HospitalDatabase *db);
void display_patient_appointments(const HospitalDatabase *db, int patient_id);
void display_doctor_appointments(const HospitalDatabase *db, int doctor_id);
void display_patient_treatments(const HospitalDatabase *db, int patient_id);
int find_patient_by_id(const HospitalDatabase *db, int patient_id);
int find_doctor_by_id(const HospitalDatabase *db, int doctor_id);
int find_appointment_by_id(const HospitalDatabase *db, int appointment_id);
void search_patients_by_name(const HospitalDatabase *db, const char *name);
void search_doctors_by_specialization(const HospitalDatabase *db, const char *specialization);
void generate_daily_schedule(const HospitalDatabase *db, int doctor_id, Date date);
void generate_patient_report(const HospitalDatabase *db, int patient_id);
void generate_financial_report(const HospitalDatabase *db, int month, int year);
int save_database(const HospitalDatabase *db);
int load_database(HospitalDatabase *db);
void generate_sample_data(HospitalDatabase *db);
void display_patient_summary(const HospitalDatabase *db);
void display_doctor_summary(const HospitalDatabase *db);
void find_available_slots(const HospitalDatabase *db, int doctor_id, Date date);
int is_valid_date(int day, int month, int year);
int is_valid_time(int hour, int minute);
const char* get_appointment_status_name(AppointmentStatus status);
const char* get_treatment_type_name(TreatmentType type);

int main() {
    HospitalDatabase db;
    initialize_database(&db);
    
    printf("Hospital Management System\n");
    printf("1. Add Patient\n");
    printf("2. Add Doctor\n");
    printf("3. Schedule Appointment\n");
    printf("4. Add Treatment\n");
    printf("5. Complete Appointment\n");
    printf("6. Cancel Appointment\n");
    printf("7. Display All Patients\n");
    printf("8. Display All Doctors\n");
    printf("9. Display All Appointments\n");
    printf("10. Display Patient Appointments\n");
    printf("11. Display Doctor Appointments\n");
    printf("12. Display Patient Treatments\n");
    printf("13. Search Patients by Name\n");
    printf("14. Search Doctors by Specialization\n");
    printf("15. Generate Daily Schedule\n");
    printf("16. Generate Patient Report\n");
    printf("17. Generate Financial Report\n");
    printf("18. Save Database\n");
    printf("19. Load Database\n");
    printf("20. Generate Sample Data\n");
    printf("21. Display Patient Summary\n");
    printf("22. Display Doctor Summary\n");
    printf("23. Find Available Slots\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH], address[MAX_ADDRESS_LENGTH], phone[15], email[50];
                char blood_type[5], medical_history[500], allergies[200], insurance_info[100];
                int day, month, year;
                char gender;
                
                printf("Enter patient name: ");
                scanf(" %[^\n]", name);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter gender (M/F/O): ");
                scanf(" %c", &gender);
                printf("Enter blood type (e.g., A+, O-): ");
                scanf("%s", blood_type);
                printf("Enter medical history (press Enter for none): ");
                getchar();
                fgets(medical_history, 500, stdin);
                medical_history[strcspn(medical_history, "\n")] = '\0';
                printf("Enter allergies (press Enter for none): ");
                fgets(allergies, 200, stdin);
                allergies[strcspn(allergies, "\n")] = '\0';
                printf("Enter insurance information: ");
                fgets(insurance_info, 100, stdin);
                insurance_info[strcspn(insurance_info, "\n")] = '\0';
                
                Date birth_date = {day, month, year};
                
                int patient_id = add_patient(&db, name, address, phone, email, birth_date,
                                            gender, blood_type, medical_history, allergies, insurance_info);
                if (patient_id != -1) {
                    printf("Patient added with ID: %d\n", patient_id);
                } else {
                    printf("Failed to add patient. Database is full.\n");
                }
                break;
            }
            case 2: {
                char name[MAX_NAME_LENGTH], specialization[50], phone[15], email[50];
                char address[MAX_ADDRESS_LENGTH], education[200], license_number[30];
                int years_of_experience;
                float consultation_fee;
                
                printf("Enter doctor name: ");
                scanf(" %[^\n]", name);
                printf("Enter specialization: ");
                scanf(" %[^\n]", specialization);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter years of experience: ");
                scanf("%d", &years_of_experience);
                printf("Enter education: ");
                scanf(" %[^\n]", education);
                printf("Enter license number: ");
                scanf("%s", license_number);
                printf("Enter consultation fee: ");
                scanf("%f", &consultation_fee);
                
                int doctor_id = add_doctor(&db, name, specialization, phone, email, address,
                                          years_of_experience, education, license_number, consultation_fee);
                if (doctor_id != -1) {
                    printf("Doctor added with ID: %d\n", doctor_id);
                    
                    // Set availability
                    printf("Set availability for Dr. %s (1=available, 0=unavailable):\n", name);
                    for (int i = 0; i < 7; i++) {
                        const char* day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                                                  "Thursday", "Friday", "Saturday"};
                        printf("%s: ", day_names[i]);
                        scanf("%d", &db.doctors[doctor_id - 1].available_days[i]);
                    }
                    
                    printf("Enter available start time (hour minute): ");
                    scanf("%d %d", &db.doctors[doctor_id - 1].available_start_time[0],
                              &db.doctors[doctor_id - 1].available_start_time[1]);
                    printf("Enter available end time (hour minute): ");
                    scanf("%d %d", &db.doctors[doctor_id - 1].available_end_time[0],
                              &db.doctors[doctor_id - 1].available_end_time[1]);
                } else {
                    printf("Failed to add doctor. Database is full.\n");
                }
                break;
            }
            case 3: {
                int patient_id, doctor_id, duration_minutes;
                DateTime appointment_time;
                
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                printf("Enter appointment date and time (DD MM YYYY HH MM): ");
                scanf("%d %d %d %d %d", &appointment_time.day, &appointment_time.month,
                      &appointment_time.year, &appointment_time.hour, &appointment_time.minute);
                printf("Enter duration in minutes: ");
                scanf("%d", &duration_minutes);
                
                int appointment_id = schedule_appointment(&db, patient_id, doctor_id,
                                                      appointment_time, duration_minutes);
                if (appointment_id != -1) {
                    printf("Appointment scheduled with ID: %d\n", appointment_id);
                } else {
                    printf("Failed to schedule appointment. Check IDs and availability.\n");
                }
                break;
            }
            case 4: {
                int patient_id, doctor_id, appointment_id, type;
                char description[300], medication[200], instructions[200];
                float cost;
                Date treatment_date;
                
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                printf("Enter appointment ID (0 if not associated): ");
                scanf("%d", &appointment_id);
                printf("Enter treatment date (DD MM YYYY): ");
                scanf("%d %d %d", &treatment_date.day, &treatment_date.month, &treatment_date.year);
                printf("Enter treatment type (0-Consultation, 1-Test, 2-Surgery, 3-Medication, 4-Therapy, 5-Vaccination, 6-Checkup, 7-Emergency): ");
                scanf("%d", &type);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter medication (press Enter for none): ");
                getchar();
                fgets(medication, 200, stdin);
                medication[strcspn(medication, "\n")] = '\0';
                printf("Enter instructions: ");
                fgets(instructions, 200, stdin);
                instructions[strcspn(instructions, "\n")] = '\0';
                printf("Enter cost: ");
                scanf("%f", &cost);
                
                int treatment_id = add_treatment(&db, patient_id, doctor_id, appointment_id,
                                              (TreatmentType)type, description, medication,
                                              instructions, cost);
                if (treatment_id != -1) {
                    printf("Treatment added with ID: %d\n", treatment_id);
                } else {
                    printf("Failed to add treatment. Check IDs.\n");
                }
                break;
            }
            case 5: {
                int appointment_id;
                printf("Enter appointment ID: ");
                scanf("%d", &appointment_id);
                complete_appointment(&db, appointment_id);
                break;
            }
            case 6: {
                int appointment_id;
                printf("Enter appointment ID: ");
                scanf("%d", &appointment_id);
                cancel_appointment(&db, appointment_id);
                break;
            }
            case 7: {
                display_all_patients(&db);
                break;
            }
            case 8: {
                display_all_doctors(&db);
                break;
            }
            case 9: {
                display_all_appointments(&db);
                break;
            }
            case 10: {
                int patient_id;
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                display_patient_appointments(&db, patient_id);
                break;
            }
            case 11: {
                int doctor_id;
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                display_doctor_appointments(&db, doctor_id);
                break;
            }
            case 12: {
                int patient_id;
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                display_patient_treatments(&db, patient_id);
                break;
            }
            case 13: {
                char name[MAX_NAME_LENGTH];
                printf("Enter patient name to search: ");
                scanf(" %[^\n]", name);
                search_patients_by_name(&db, name);
                break;
            }
            case 14: {
                char specialization[50];
                printf("Enter specialization to search: ");
                scanf(" %[^\n]", specialization);
                search_doctors_by_specialization(&db, specialization);
                break;
            }
            case 15: {
                int doctor_id;
                Date date;
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &date.day, &date.month, &date.year);
                generate_daily_schedule(&db, doctor_id, date);
                break;
            }
            case 16: {
                int patient_id;
                printf("Enter patient ID: ");
                scanf("%d", &patient_id);
                generate_patient_report(&db, patient_id);
                break;
            }
            case 17: {
                int month, year;
                printf("Enter month (1-12): ");
                scanf("%d", &month);
                printf("Enter year: ");
                scanf("%d", &year);
                generate_financial_report(&db, month, year);
                break;
            }
            case 18: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 19: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 20: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 21: {
                display_patient_summary(&db);
                break;
            }
            case 22: {
                display_doctor_summary(&db);
                break;
            }
            case 23: {
                int doctor_id;
                Date date;
                printf("Enter doctor ID: ");
                scanf("%d", &doctor_id);
                printf("Enter date (DD MM YYYY): ");
                scanf("%d %d %d", &date.day, &date.month, &date.year);
                find_available_slots(&db, doctor_id, date);
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
    db->appointment_count = 0;
    db->treatment_count = 0;
}

int add_patient(HospitalDatabase *db, const char *name, const char *address, 
                 const char *phone, const char *email, Date birth_date, 
                 char gender, const char *blood_type, const char *medical_history,
                 const char *allergies, const char *insurance_info) {
    if (db->patient_count >= MAX_PATIENTS) {
        return -1;
    }
    
    Patient *patient = &db->patients[db->patient_count];
    patient->id = db->patient_count + 1;
    strncpy(patient->name, name, MAX_NAME_LENGTH - 1);
    patient->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(patient->address, address, MAX_ADDRESS_LENGTH - 1);
    patient->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    strncpy(patient->phone, phone, 14);
    patient->phone[14] = '\0';
    strncpy(patient->email, email, 49);
    patient->email[49] = '\0';
    patient->birth_date = birth_date;
    patient->gender = gender;
    strncpy(patient->blood_type, blood_type, 4);
    patient->blood_type[4] = '\0';
    strncpy(patient->medical_history, medical_history, 499);
    patient->medical_history[499] = '\0';
    strncpy(patient->allergies, allergies, 199);
    patient->allergies[199] = '\0';
    strncpy(patient->insurance_info, insurance_info, 99);
    patient->insurance_info[99] = '\0';
    patient->emergency_contact_id = 0; // No emergency contact initially
    patient->active = 1;
    
    // Set registration date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    patient->registration_date.day = current_time->tm_mday;
    patient->registration_date.month = current_time->tm_mon + 1;
    patient->registration_date.year = current_time->tm_year + 1900;
    
    db->patient_count++;
    return patient->id;
}

int add_doctor(HospitalDatabase *db, const char *name, const char *specialization,
                const char *phone, const char *email, const char *address,
                int years_of_experience, const char *education,
                const char *license_number, float consultation_fee) {
    if (db->doctor_count >= MAX_DOCTORS) {
        return -1;
    }
    
    Doctor *doctor = &db->doctors[db->doctor_count];
    doctor->id = db->doctor_count + 1;
    strncpy(doctor->name, name, MAX_NAME_LENGTH - 1);
    doctor->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(doctor->specialization, specialization, 49);
    doctor->specialization[49] = '\0';
    strncpy(doctor->phone, phone, 14);
    doctor->phone[14] = '\0';
    strncpy(doctor->email, email, 49);
    doctor->email[49] = '\0';
    strncpy(doctor->address, address, MAX_ADDRESS_LENGTH - 1);
    doctor->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    doctor->years_of_experience = years_of_experience;
    strncpy(doctor->education, education, 199);
    doctor->education[199] = '\0';
    strncpy(doctor->license_number, license_number, 29);
    doctor->license_number[29] = '\0';
    doctor->consultation_fee = consultation_fee;
    
    // Initialize availability
    for (int i = 0; i < 7; i++) {
        doctor->available_days[i] = 0; // Initially unavailable
    }
    
    // Set default availability time (9 AM to 5 PM)
    doctor->available_start_time[0] = 9;
    doctor->available_start_time[1] = 0;
    doctor->available_end_time[0] = 17;
    doctor->available_end_time[1] = 0;
    
    doctor->active = 1;
    
    db->doctor_count++;
    return doctor->id;
}

int schedule_appointment(HospitalDatabase *db, int patient_id, int doctor_id,
                        DateTime appointment_time, int duration_minutes) {
    if (db->appointment_count >= MAX_APPOINTMENTS) {
        return -1;
    }
    
    // Validate patient and doctor
    int patient_index = find_patient_by_id(db, patient_id);
    int doctor_index = find_doctor_by_id(db, doctor_id);
    
    if (patient_index == -1 || doctor_index == -1) {
        return -1;
    }
    
    // Validate date and time
    if (!is_valid_date(appointment_time.day, appointment_time.month, appointment_time.year) ||
        !is_valid_time(appointment_time.hour, appointment_time.minute)) {
        return -1;
    }
    
    Doctor *doctor = &db->doctors[doctor_index];
    
    // Check if doctor is available on that day of the week
    struct tm tm_time = {0};
    tm_time.tm_year = appointment_time.year - 1900;
    tm_time.tm_mon = appointment_time.month - 1;
    tm_time.tm_mday = appointment_time.day;
    mktime(&tm_time); // Normalize the date
    
    int day_of_week = tm_time.tm_wday; // 0 = Sunday, 1 = Monday, etc.
    
    if (!doctor->available_days[day_of_week]) {
        printf("Doctor is not available on this day.\n");
        return -1;
    }
    
    // Check if appointment time is within doctor's available hours
    int appointment_total_minutes = appointment_time.hour * 60 + appointment_time.minute;
    int start_total_minutes = doctor->available_start_time[0] * 60 + doctor->available_start_time[1];
    int end_total_minutes = doctor->available_end_time[0] * 60 + doctor->available_end_time[1];
    
    if (appointment_total_minutes < start_total_minutes || 
        appointment_total_minutes >= end_total_minutes) {
        printf("Appointment time is outside doctor's available hours.\n");
        return -1;
    }
    
    // Check for overlapping appointments
    for (int i = 0; i < db->appointment_count; i++) {
        Appointment *existing = &db->appointments[i];
        
        if (existing->doctor_id == doctor_id && existing->status != CANCELLED) {
            // Check if dates match
            if (existing->appointment_time.day == appointment_time.day &&
                existing->appointment_time.month == appointment_time.month &&
                existing->appointment_time.year == appointment_time.year) {
                
                int existing_start = existing->appointment_time.hour * 60 + existing->appointment_time.minute;
                int existing_end = existing_start + existing->duration_minutes;
                int new_start = appointment_time.hour * 60 + appointment_time.minute;
                int new_end = new_start + duration_minutes;
                
                // Check for overlap
                if ((new_start >= existing_start && new_start < existing_end) ||
                    (new_end > existing_start && new_end <= existing_end) ||
                    (new_start <= existing_start && new_end >= existing_end)) {
                    printf("Appointment conflicts with an existing appointment.\n");
                    return -1;
                }
            }
        }
    }
    
    // Create the appointment
    Appointment *appointment = &db->appointments[db->appointment_count];
    appointment->id = db->appointment_count + 1;
    appointment->patient_id = patient_id;
    appointment->doctor_id = doctor_id;
    appointment->appointment_time = appointment_time;
    appointment->duration_minutes = duration_minutes;
    appointment->status = SCHEDULED;
    strcpy(appointment->notes, "");
    appointment->fee_paid = 0.0f;
    
    db->appointment_count++;
    return appointment->id;
}

int add_treatment(HospitalDatabase *db, int patient_id, int doctor_id,
                  int appointment_id, TreatmentType type, const char *description,
                  const char *medication, const char *instructions, float cost) {
    if (db->treatment_count >= MAX_TREATMENTS) {
        return -1;
    }
    
    // Validate patient and doctor
    int patient_index = find_patient_by_id(db, patient_id);
    int doctor_index = find_doctor_by_id(db, doctor_id);
    
    if (patient_index == -1 || doctor_index == -1) {
        return -1;
    }
    
    // Validate appointment (if provided)
    if (appointment_id != 0 && find_appointment_by_id(db, appointment_id) == -1) {
        return -1;
    }
    
    Treatment *treatment = &db->treatments[db->treatment_count];
    treatment->id = db->treatment_count + 1;
    treatment->patient_id = patient_id;
    treatment->doctor_id = doctor_id;
    treatment->appointment_id = appointment_id;
    
    // Set treatment date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    treatment->treatment_date.day = current_time->tm_mday;
    treatment->treatment_date.month = current_time->tm_mon + 1;
    treatment->treatment_date.year = current_time->tm_year + 1900;
    
    treatment->type = type;
    strncpy(treatment->description, description, 299);
    treatment->description[299] = '\0';
    strncpy(treatment->medication, medication, 199);
    treatment->medication[199] = '\0';
    strncpy(treatment->instructions, instructions, 199);
    treatment->instructions[199] = '\0';
    treatment->cost = cost;
    treatment->paid = 0;
    
    db->treatment_count++;
    return treatment->id;
}

void complete_appointment(HospitalDatabase *db, int appointment_id) {
    int appointment_index = find_appointment_by_id(db, appointment_id);
    if (appointment_index == -1) {
        printf("Appointment not found.\n");
        return;
    }
    
    Appointment *appointment = &db->appointments[appointment_index];
    
    if (appointment->status != SCHEDULED) {
        printf("Only scheduled appointments can be completed.\n");
        return;
    }
    
    // Update appointment status
    appointment->status = COMPLETED;
    
    // Mark fee as paid
    int doctor_index = find_doctor_by_id(db, appointment->doctor_id);
    if (doctor_index != -1) {
        appointment->fee_paid = db->doctors[doctor_index].consultation_fee;
    }
    
    printf("Appointment %d has been completed.\n", appointment_id);
}

void cancel_appointment(HospitalDatabase *db, int appointment_id) {
    int appointment_index = find_appointment_by_id(db, appointment_id);
    if (appointment_index == -1) {
        printf("Appointment not found.\n");
        return;
    }
    
    Appointment *appointment = &db->appointments[appointment_index];
    
    if (appointment->status != SCHEDULED) {
        printf("Only scheduled appointments can be cancelled.\n");
        return;
    }
    
    // Update appointment status
    appointment->status = CANCELLED;
    
    printf("Appointment %d has been cancelled.\n", appointment_id);
}

void display_patient(const Patient *patient) {
    printf("\nPatient ID: %d\n", patient->id);
    printf("Name: %s\n", patient->name);
    printf("Address: %s\n", patient->address);
    printf("Phone: %s\n", patient->phone);
    printf("Email: %s\n", patient->email);
    printf("Birth Date: %02d/%02d/%d\n", 
           patient->birth_date.day, patient->birth_date.month, patient->birth_date.year);
    printf("Gender: %c\n", patient->gender);
    printf("Blood Type: %s\n", patient->blood_type);
    printf("Medical History: %s\n", strlen(patient->medical_history) > 0 ? patient->medical_history : "None");
    printf("Allergies: %s\n", strlen(patient->allergies) > 0 ? patient->allergies : "None");
    printf("Insurance Information: %s\n", patient->insurance_info);
    printf("Registration Date: %02d/%02d/%d\n", 
           patient->registration_date.day, patient->registration_date.month, patient->registration_date.year);
    printf("Status: %s\n", patient->active ? "Active" : "Inactive");
}

void display_doctor(const Doctor *doctor) {
    printf("\nDoctor ID: %d\n", doctor->id);
    printf("Name: %s\n", doctor->name);
    printf("Specialization: %s\n", doctor->specialization);
    printf("Phone: %s\n", doctor->phone);
    printf("Email: %s\n", doctor->email);
    printf("Address: %s\n", doctor->address);
    printf("Years of Experience: %d\n", doctor->years_of_experience);
    printf("Education: %s\n", doctor->education);
    printf("License Number: %s\n", doctor->license_number);
    printf("Consultation Fee: $%.2f\n", doctor->consultation_fee);
    
    // Display availability
    const char* day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                              "Thursday", "Friday", "Saturday"};
    printf("Availability:\n");
    for (int i = 0; i < 7; i++) {
        printf("  %s: %s (%02d:%02d - %02d:%02d)\n", 
               day_names[i], 
               doctor->available_days[i] ? "Available" : "Unavailable",
               doctor->available_start_time[0], doctor->available_start_time[1],
               doctor->available_end_time[0], doctor->available_end_time[1]);
    }
    
    printf("Status: %s\n", doctor->active ? "Active" : "Inactive");
}

void display_appointment(const Appointment *appointment, const HospitalDatabase *db) {
    printf("\nAppointment ID: %d\n", appointment->id);
    
    // Display patient information
    int patient_index = find_patient_by_id(db, appointment->patient_id);
    if (patient_index != -1) {
        printf("Patient: %s (ID: %d)\n", 
               db->patients[patient_index].name, appointment->patient_id);
    }
    
    // Display doctor information
    int doctor_index = find_doctor_by_id(db, appointment->doctor_id);
    if (doctor_index != -1) {
        printf("Doctor: %s (ID: %d)\n", 
               db->doctors[doctor_index].name, appointment->doctor_id);
    }
    
    printf("Date and Time: %02d/%02d/%d %02d:%02d\n", 
           appointment->appointment_time.day, appointment->appointment_time.month, 
           appointment->appointment_time.year, 
           appointment->appointment_time.hour, appointment->appointment_time.minute);
    printf("Duration: %d minutes\n", appointment->duration_minutes);
    printf("Status: %s\n", get_appointment_status_name(appointment->status));
    
    if (strlen(appointment->notes) > 0) {
        printf("Notes: %s\n", appointment->notes);
    }
    
    if (appointment->fee_paid > 0) {
        printf("Fee Paid: $%.2f\n", appointment->fee_paid);
    }
}

void display_treatment(const Treatment *treatment, const HospitalDatabase *db) {
    printf("\nTreatment ID: %d\n", treatment->id);
    
    // Display patient information
    int patient_index = find_patient_by_id(db, treatment->patient_id);
    if (patient_index != -1) {
        printf("Patient: %s (ID: %d)\n", 
               db->patients[patient_index].name, treatment->patient_id);
    }
    
    // Display doctor information
    int doctor_index = find_doctor_by_id(db, treatment->doctor_id);
    if (doctor_index != -1) {
        printf("Doctor: %s (ID: %d)\n", 
               db->doctors[doctor_index].name, treatment->doctor_id);
    }
    
    if (treatment->appointment_id != 0) {
        printf("Appointment ID: %d\n", treatment->appointment_id);
    }
    
    printf("Treatment Date: %02d/%02d/%d\n", 
           treatment->treatment_date.day, treatment->treatment_date.month, treatment->treatment_date.year);
    printf("Type: %s\n", get_treatment_type_name(treatment->type));
    printf("Description: %s\n", treatment->description);
    
    if (strlen(treatment->medication) > 0) {
        printf("Medication: %s\n", treatment->medication);
    }
    
    if (strlen(treatment->instructions) > 0) {
        printf("Instructions: %s\n", treatment->instructions);
    }
    
    printf("Cost: $%.2f\n", treatment->cost);
    printf("Payment Status: %s\n", treatment->paid ? "Paid" : "Unpaid");
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
            display_doctor(&db->doctors[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_appointments(const HospitalDatabase *db) {
    printf("\n=== All Appointments ===\n");
    printf("Total Appointments: %d\n\n", db->appointment_count);
    
    for (int i = 0; i < db->appointment_count; i++) {
        display_appointment(&db->appointments[i], db);
        printf("------------------------\n");
    }
}

void display_patient_appointments(const HospitalDatabase *db, int patient_id) {
    int patient_index = find_patient_by_id(db, patient_id);
    if (patient_index == -1) {
        printf("Patient not found.\n");
        return;
    }
    
    Patient *patient = &db->patients[patient_index];
    printf("\n=== Appointments for Patient %s ===\n", patient->name);
    
    int found = 0;
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].patient_id == patient_id) {
            display_appointment(&db->appointments[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No appointments found for this patient.\n");
    }
}

void display_doctor_appointments(const HospitalDatabase *db, int doctor_id) {
    int doctor_index = find_doctor_by_id(db, doctor_id);
    if (doctor_index == -1) {
        printf("Doctor not found.\n");
        return;
    }
    
    Doctor *doctor = &db->doctors[doctor_index];
    printf("\n=== Appointments for Doctor %s ===\n", doctor->name);
    
    int found = 0;
    for (int i = 0; i < db->appointment_count; i++) {
        if (db->appointments[i].doctor_id == doctor_id) {
            display_appointment(&db->appointments[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No appointments found for this doctor.\n");
    }
}

void display_patient_treatments(const HospitalDatabase *db, int patient_id) {
    int patient_index = find_patient_by_id(db, patient_id);
    if (patient_index == -1) {
        printf("Patient not found.\n");
        return;
    }
    
    Patient *patient = &db->patients[patient_index];
    printf("\n=== Treatments for Patient %s ===\n", patient->name);
    
    int found = 0;
    float total_cost = 0.0f;
    
    for (int i = 0; i < db->treatment_count; i++) {
        if (db->treatments[i].patient_id == patient_id) {
            display_treatment(&db->treatments[i], db);
            printf("------------------------\n");
            total_cost += db->treatments[i].cost;
            found++;
        }
    }
    
    if (found == 0) {
        printf("No treatments found for this patient.\n");
    } else {
        printf("Total Cost: $%.2f\n", total_cost);
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
        if (db->appointments[i].id == appointment_id) {
            return i;
        }
    }
    return -1;
}

void search_patients_by_name(const HospitalDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->patient_count; i++) {
        if (db->patients[i].active && strstr(db->patients[i].name, name) != NULL) {
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
    printf("\nSearch Results for specialization '%s':\n", specialization);
    int found = 0;
    
    for (int i = 0; i < db->doctor_count; i++) {
        if (db->doctors[i].active && strstr(db->doctors[i].specialization, specialization) != NULL) {
            display_doctor(&db->doctors[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No doctors found with specialization containing '%s'.\n", specialization);
    }
}

void generate_daily_schedule(const HospitalDatabase *db, int doctor_id, Date date) {
    int doctor_index = find_doctor_by_id(db, doctor_id);
    if (doctor_index == -1) {
        printf("Doctor not found.\n");
        return;
    }
    
    Doctor *doctor = &db->doctors[doctor_index];
    
    // Check if doctor is available on that day of the week
    struct tm tm_date = {0};
    tm_date.tm_year = date.year - 1900;
    tm_date.tm_mon = date.month - 1;
    tm_date.tm_mday = date.day;
    mktime(&tm_date); // Normalize the date
    
    int day_of_week = tm_date.tm_wday; // 0 = Sunday, 1 = Monday, etc.
    const char* day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                              "Thursday", "Friday", "Saturday"};
    
    printf("\n=== Daily Schedule for Dr. %s ===\n", doctor->name);
    printf("Date: %s, %02d/%02d/%d\n", 
           day_names[day_of_week], date.day, date.month, date.year);
    
    if (!doctor->available_days[day_of_week]) {
        printf("Doctor is not available on this day.\n");
        return;
    }
    
    printf("Working Hours: %02d:%02d - %02d:%02d\n\n", 
           doctor->available_start_time[0], doctor->available_start_time[1],
           doctor->available_end_time[0], doctor->available_end_time[1]);
    
    int found = 0;
    for (int i = 0; i < db->appointment_count; i++) {
        Appointment *appointment = &db->appointments[i];
        
        if (appointment->doctor_id == doctor_id && 
            appointment->appointment_time.day == date.day &&
            appointment->appointment_time.month == date.month &&
            appointment->appointment_time.year == date.year) {
            
            // Display patient information
            int patient_index = find_patient_by_id(db, appointment->patient_id);
            if (patient_index != -1) {
                printf("%02d:%02d - %02d:%02d: %s (ID: %d) - %s\n", 
                       appointment->appointment_time.hour, appointment->appointment_time.minute,
                       appointment->appointment_time.hour + appointment->duration_minutes / 60,
                       (appointment->appointment_time.minute + appointment->duration_minutes % 60) % 60,
                       db->patients[patient_index].name, appointment->patient_id,
                       get_appointment_status_name(appointment->status));
            }
            
            found++;
        }
    }
    
    if (found == 0) {
        printf("No appointments scheduled for this day.\n");
    }
}

void generate_patient_report(const HospitalDatabase *db, int patient_id) {
    int patient_index = find_patient_by_id(db, patient_id);
    if (patient_index == -1) {
        printf("Patient not found.\n");
        return;
    }
    
    Patient *patient = &db->patients[patient_index];
    printf("\n=== Patient Report for %s ===\n", patient->name);
    printf("Patient ID: %d\n", patient->id);
    printf("Phone: %s\n", patient->phone);
    printf("Email: %s\n", patient->email);
    printf("Blood Type: %s\n", patient->blood_type);
    printf("Allergies: %s\n", strlen(patient->allergies) > 0 ? patient->allergies : "None");
    printf("Medical History: %s\n", strlen(patient->medical_history) > 0 ? patient->medical_history : "None");
    
    // Display recent appointments
    printf("\nRecent Appointments:\n");
    int appointment_count = 0;
    for (int i = 0; i < db->appointment_count && appointment_count < 5; i++) {
        if (db->appointments[i].patient_id == patient_id) {
            Appointment *appointment = &db->appointments[i];
            
            // Display doctor information
            int doctor_index = find_doctor_by_id(db, appointment->doctor_id);
            if (doctor_index != -1) {
                printf("  - %02d/%02d/%d %02d:%02d: Dr. %s (%s)\n",
                       appointment->appointment_time.day, appointment->appointment_time.month,
                       appointment->appointment_time.year,
                       appointment->appointment_time.hour, appointment->appointment_time.minute,
                       db->doctors[doctor_index].name,
                       get_appointment_status_name(appointment->status));
            }
            
            appointment_count++;
        }
    }
    
    if (appointment_count == 0) {
        printf("  No appointments found.\n");
    }
    
    // Display recent treatments
    printf("\nRecent Treatments:\n");
    int treatment_count = 0;
    float total_cost = 0.0f;
    
    for (int i = 0; i < db->treatment_count && treatment_count < 5; i++) {
        if (db->treatments[i].patient_id == patient_id) {
            Treatment *treatment = &db->treatments[i];
            
            // Display doctor information
            int doctor_index = find_doctor_by_id(db, treatment->doctor_id);
            if (doctor_index != -1) {
                printf("  - %02d/%02d/%d: Dr. %s - %s ($%.2f)\n",
                       treatment->treatment_date.day, treatment->treatment_date.month,
                       treatment->treatment_date.year,
                       db->doctors[doctor_index].name,
                       get_treatment_type_name(treatment->type),
                       treatment->cost);
                
                total_cost += treatment->cost;
            }
            
            treatment_count++;
        }
    }
    
    if (treatment_count == 0) {
        printf("  No treatments found.\n");
    } else {
        printf("  Total Cost of Recent Treatments: $%.2f\n", total_cost);
    }
}

void generate_financial_report(const HospitalDatabase *db, int month, int year) {
    if (month < 1 || month > 12 || year < 2000 || year > 2100) {
        printf("Invalid month or year.\n");
        return;
    }
    
    char month_names[][10] = {"January", "February", "March", "April", "May", "June",
                             "July", "August", "September", "October", "November", "December"};
    
    printf("\n=== Financial Report for %s %d ===\n", month_names[month - 1], year);
    
    float appointment_revenue = 0.0f;
    float treatment_revenue = 0.0f;
    int completed_appointments = 0;
    int total_treatments = 0;
    
    // Calculate appointment revenue
    for (int i = 0; i < db->appointment_count; i++) {
        Appointment *appointment = &db->appointments[i];
        
        if (appointment->status == COMPLETED &&
            appointment->appointment_time.month == month &&
            appointment->appointment_time.year == year) {
            
            appointment_revenue += appointment->fee_paid;
            completed_appointments++;
        }
    }
    
    // Calculate treatment revenue
    for (int i = 0; i < db->treatment_count; i++) {
        Treatment *treatment = &db->treatments[i];
        
        if (treatment->paid &&
            treatment->treatment_date.month == month &&
            treatment->treatment_date.year == year) {
            
            treatment_revenue += treatment->cost;
            total_treatments++;
        }
    }
    
    float total_revenue = appointment_revenue + treatment_revenue;
    
    // Doctor performance
    printf("\nDoctor Performance:\n");
    printf("%-20s %-10s %-15s %-15s\n", "Doctor", "Appointments", "Revenue", "Treatments");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < db->doctor_count; i++) {
        if (!db->doctors[i].active) continue;
        
        int doctor_appointments = 0;
        float doctor_revenue = 0.0f;
        int doctor_treatments = 0;
        
        // Count doctor's appointments
        for (int j = 0; j < db->appointment_count; j++) {
            Appointment *appointment = &db->appointments[j];
            
            if (appointment->doctor_id == db->doctors[i].id &&
                appointment->status == COMPLETED &&
                appointment->appointment_time.month == month &&
                appointment->appointment_time.year == year) {
                
                doctor_appointments++;
                doctor_revenue += appointment->fee_paid;
            }
        }
        
        // Count doctor's treatments
        for (int j = 0; j < db->treatment_count; j++) {
            Treatment *treatment = &db->treatments[j];
            
            if (treatment->doctor_id == db->doctors[i].id &&
                treatment->paid &&
                treatment->treatment_date.month == month &&
                treatment->treatment_date.year == year) {
                
                doctor_revenue += treatment->cost;
                doctor_treatments++;
            }
        }
        
        printf("%-20s %-10d $%-14.2f %-15d\n", 
               db->doctors[i].name, doctor_appointments, doctor_revenue, doctor_treatments);
    }
    
    printf("------------------------------------------------------------\n");
    
    // Treatment type breakdown
    printf("\nTreatment Type Breakdown:\n");
    printf("%-20s %-10s %-15s\n", "Type", "Count", "Revenue");
    printf("--------------------------------------\n");
    
    int type_counts[8] = {0}; // CONSULTATION to EMERGENCY
    float type_revenue[8] = {0.0f};
    
    for (int i = 0; i < db->treatment_count; i++) {
        Treatment *treatment = &db->treatments[i];
        
        if (treatment->paid &&
            treatment->treatment_date.month == month &&
            treatment->treatment_date.year == year) {
            
            type_counts[treatment->type]++;
            type_revenue[treatment->type] += treatment->cost;
        }
    }
    
    const char* type_names[] = {"Consultation", "Test", "Surgery", "Medication", 
                                "Therapy", "Vaccination", "Checkup", "Emergency"};
    
    for (int i = 0; i < 8; i++) {
        if (type_counts[i] > 0) {
            printf("%-20s %-10d $%-14.2f\n", 
                   type_names[i], type_counts[i], type_revenue[i]);
        }
    }
    
    printf("\nSummary:\n");
    printf("Completed Appointments: %d\n", completed_appointments);
    printf("Appointment Revenue: $%.2f\n", appointment_revenue);
    printf("Total Treatments: %d\n", total_treatments);
    printf("Treatment Revenue: $%.2f\n", treatment_revenue);
    printf("Total Revenue: $%.2f\n", total_revenue);
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
    
    FILE *appointments_file = fopen(FILENAME_APPOINTMENTS, "wb");
    if (appointments_file != NULL) {
        fwrite(&db->appointment_count, sizeof(int), 1, appointments_file);
        fwrite(db->appointments, sizeof(Appointment), db->appointment_count, appointments_file);
        fclose(appointments_file);
    } else {
        return 0;
    }
    
    FILE *treatments_file = fopen(FILENAME_TREATMENTS, "wb");
    if (treatments_file != NULL) {
        fwrite(&db->treatment_count, sizeof(int), 1, treatments_file);
        fwrite(db->treatments, sizeof(Treatment), db->treatment_count, treatments_file);
        fclose(treatments_file);
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
    
    FILE *appointments_file = fopen(FILENAME_APPOINTMENTS, "rb");
    if (appointments_file != NULL) {
        fread(&db->appointment_count, sizeof(int), 1, appointments_file);
        fread(db->appointments, sizeof(Appointment), db->appointment_count, appointments_file);
        fclose(appointments_file);
    } else {
        return 0;
    }
    
    FILE *treatments_file = fopen(FILENAME_TREATMENTS, "rb");
    if (treatments_file != NULL) {
        fread(&db->treatment_count, sizeof(int), 1, treatments_file);
        fread(db->treatments, sizeof(Treatment), db->treatment_count, treatments_file);
        fclose(treatments_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(HospitalDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    char* specializations[] = {"General Practice", "Cardiology", "Pediatrics", "Orthopedics",
                               "Dermatology", "Psychiatry", "Obstetrics", "Radiology",
                               "Neurology", "Oncology", "Ophthalmology", "ENT"};
    
    char* blood_types[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    
    srand(time(NULL));
    
    // Add doctors
    for (int i = 0; i < 15 && db->doctor_count < MAX_DOCTORS; i++) {
        char name[60], phone[15], email[50], address[100], education[200], license[30];
        sprintf(name, "Dr. %s %s", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(email, "dr.%s.%s@hospital.com", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(address, "%d Medical Center Blvd, City, State", 100 + rand() % 900);
        
        const char* specialization = specializations[rand() % 12];
        int years = 5 + (rand() % 30);
        sprintf(education, "MD from %s University", 
                (rand() % 2) ? "Harvard" : ((rand() % 2) ? "Johns Hopkins" : "Stanford"));
        sprintf(license, "MD%08d", 10000000 + rand() % 90000000);
        float fee = 50.0f + (rand() % 200);
        
        int doctor_id = add_doctor(db, name, specialization, phone, email, address,
                                  years, education, license, fee);
        
        // Set availability
        for (int j = 0; j < 7; j++) {
            // Most doctors work weekdays, some work weekends
            if (j >= 1 && j <= 5) {
                db.doctors[doctor_id - 1].available_days[j] = (rand() % 10 < 8); // 80% chance
            } else {
                db.doctors[doctor_id - 1].available_days[j] = (rand() % 10 < 2); // 20% chance
            }
        }
        
        // Set working hours
        db.doctors[doctor_id - 1].available_start_time[0] = 7 + (rand() % 3); // 7-9 AM
        db.doctors[doctor_id - 1].available_start_time[1] = 0;
        db.doctors[doctor_id - 1].available_end_time[0] = 16 + (rand() % 3); // 4-6 PM
        db.doctors[doctor_id - 1].available_end_time[1] = 0;
    }
    
    // Add patients
    for (int i = 0; i < 100 && db->patient_count < MAX_PATIENTS; i++) {
        char name[60], phone[15], email[50], address[100], insurance[100];
        sprintf(name, "%s %s", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(email, "%s.%s@email.com", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(address, "%d Patient St, City, State", 100 + rand() % 900);
        sprintf(insurance, "%s Insurance Plan", 
                (rand() % 3) ? "Premium" : ((rand() % 2) ? "Standard" : "Basic"));
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1940 + (rand() % 80);
        Date birth_date = {day, month, year};
        
        char gender = (rand() % 3 == 0) ? 'O' : ((rand() % 2) ? 'M' : 'F');
        const char* blood_type = blood_types[rand() % 8];
        
        add_patient(db, name, address, phone, email, birth_date, gender,
                   blood_type, "None", "None", insurance);
    }
    
    // Schedule appointments
    for (int i = 0; i < 200 && db->appointment_count < MAX_APPOINTMENTS; i++) {
        int patient_id = 1 + (rand() % db->patient_count);
        int doctor_id = 1 + (rand() % db->doctor_count);
        
        // Generate a date within the next 30 days
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);
        
        DateTime appointment_time;
        appointment_time.day = 1 + (rand() % 28);
        appointment_time.month = 1 + (rand() % 12);
        appointment_time.year = current_time->tm_year + 1900;
        appointment_time.hour = 8 + (rand() % 10); // 8 AM - 6 PM
        appointment_time.minute = (rand() % 4) * 15; // 0, 15, 30, or 45 minutes
        
        int duration = 15 + (rand() % 4) * 15; // 15, 30, 45, or 60 minutes
        
        if (schedule_appointment(db, patient_id, doctor_id, appointment_time, duration) != -1) {
            // Complete or cancel some appointments
            if (rand() % 10 < 7) { // 70% chance
                complete_appointment(&db, db->appointment_count);
            } else if (rand() % 10 < 2) { // 20% chance
                cancel_appointment(&db, db->appointment_count);
            }
        }
    }
    
    // Add treatments
    for (int i = 0; i < 300 && db->treatment_count < MAX_TREATMENTS; i++) {
        int patient_id = 1 + (rand() % db->patient_count);
        int doctor_id = 1 + (rand() % db->doctor_count);
        int appointment_id = (rand() % 10 < 8) ? (1 + (rand() % db->appointment_count)) : 0; // 80% chance
        TreatmentType type = (TreatmentType)(rand() % 8);
        
        char description[300], medication[200], instructions[200];
        sprintf(description, "Treatment for %s condition", 
                (rand() % 2) ? "acute" : "chronic");
        
        if (type == MEDICATION) {
            sprintf(medication, "%s %d mg", 
                    (rand() % 2) ? "Aspirin" : ((rand() % 2) ? "Ibuprofen" : "Paracetamol"),
                    100 + (rand() % 900));
            strcpy(instructions, "Take with food as needed");
        } else {
            strcpy(medication, "None");
            strcpy(instructions, "Follow up in 1 week");
        }
        
        float cost = 50.0f + (rand() % 1000);
        
        add_treatment(db, patient_id, doctor_id, appointment_id, type, description,
                     medication, instructions, cost);
        
        // Mark some treatments as paid
        if (rand() % 10 < 7) { // 70% chance
            db->treatments[db->treatment_count - 1].paid = 1;
        }
    }
}

void display_patient_summary(const HospitalDatabase *db) {
    printf("\n=== Patient Summary ===\n");
    
    int active_patients = 0;
    int male_patients = 0;
    int female_patients = 0;
    int other_patients = 0;
    float age_sum = 0.0f;
    int age_count = 0;
    
    for (int i = 0; i < db->patient_count; i++) {
        if (db->patients[i].active) {
            active_patients++;
            
            if (db->patients[i].gender == 'M') male_patients++;
            else if (db->patients[i].gender == 'F') female_patients++;
            else other_patients++;
            
            // Calculate age
            time_t now = time(NULL);
            struct tm *current_time = localtime(&now);
            int current_year = current_time->tm_year + 1900;
            int age = current_year - db->patients[i].birth_date.year;
            
            age_sum += age;
            age_count++;
        }
    }
    
    printf("Total Active Patients: %d\n", active_patients);
    printf("Male Patients: %d (%.1f%%)\n", male_patients, 
           active_patients > 0 ? (float)male_patients / active_patients * 100 : 0);
    printf("Female Patients: %d (%.1f%%)\n", female_patients,
           active_patients > 0 ? (float)female_patients / active_patients * 100 : 0);
    printf("Other Patients: %d (%.1f%%)\n", other_patients,
           active_patients > 0 ? (float)other_patients / active_patients * 100 : 0);
    printf("Average Age: %.1f years\n", age_count > 0 ? age_sum / age_count : 0);
    
    // Blood type distribution
    int blood_type_counts[8] = {0}; // A+, A-, B+, B-, AB+, AB-, O+, O-
    const char* blood_types[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    
    for (int i = 0; i < db->patient_count; i++) {
        if (db->patients[i].active) {
            for (int j = 0; j < 8; j++) {
                if (strcmp(db->patients[i].blood_type, blood_types[j]) == 0) {
                    blood_type_counts[j]++;
                    break;
                }
            }
        }
    }
    
    printf("\nBlood Type Distribution:\n");
    for (int i = 0; i < 8; i++) {
        if (blood_type_counts[i] > 0) {
            printf("  %s: %d (%.1f%%)\n", blood_types[i], blood_type_counts[i],
                   active_patients > 0 ? (float)blood_type_counts[i] / active_patients * 100 : 0);
        }
    }
}

void display_doctor_summary(const HospitalDatabase *db) {
    printf("\n=== Doctor Summary ===\n");
    
    int active_doctors = 0;
    float avg_experience = 0.0f;
    float avg_fee = 0.0f;
    
    // Specialization count
    int specialization_counts[12] = {0}; // General Practice to ENT
    const char* specializations[] = {"General Practice", "Cardiology", "Pediatrics", "Orthopedics",
                                   "Dermatology", "Psychiatry", "Obstetrics", "Radiology",
                                   "Neurology", "Oncology", "Ophthalmology", "ENT"};
    
    for (int i = 0; i < db->doctor_count; i++) {
        if (db->doctors[i].active) {
            active_doctors++;
            avg_experience += db->doctors[i].years_of_experience;
            avg_fee += db->doctors[i].consultation_fee;
            
            // Count specialization
            for (int j = 0; j < 12; j++) {
                if (strcmp(db->doctors[i].specialization, specializations[j]) == 0) {
                    specialization_counts[j]++;
                    break;
                }
            }
        }
    }
    
    printf("Total Active Doctors: %d\n", active_doctors);
    printf("Average Years of Experience: %.1f\n", 
           active_doctors > 0 ? avg_experience / active_doctors : 0);
    printf("Average Consultation Fee: $%.2f\n", 
           active_doctors > 0 ? avg_fee / active_doctors : 0);
    
    printf("\nSpecialization Distribution:\n");
    for (int i = 0; i < 12; i++) {
        if (specialization_counts[i] > 0) {
            printf("  %s: %d (%.1f%%)\n", specializations[i], specialization_counts[i],
                   active_doctors > 0 ? (float)specialization_counts[i] / active_doctors * 100 : 0);
        }
    }
}

void find_available_slots(const HospitalDatabase *db, int doctor_id, Date date) {
    int doctor_index = find_doctor_by_id(db, doctor_id);
    if (doctor_index == -1) {
        printf("Doctor not found.\n");
        return;
    }
    
    Doctor *doctor = &db->doctors[doctor_index];
    
    // Check if doctor is available on that day of the week
    struct tm tm_date = {0};
    tm_date.tm_year = date.year - 1900;
    tm_date.tm_mon = date.month - 1;
    tm_date.tm_mday = date.day;
    mktime(&tm_date); // Normalize the date
    
    int day_of_week = tm_date.tm_wday; // 0 = Sunday, 1 = Monday, etc.
    const char* day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                              "Thursday", "Friday", "Saturday"};
    
    printf("\n=== Available Slots for Dr. %s ===\n", doctor->name);
    printf("Date: %s, %02d/%02d/%d\n", 
           day_names[day_of_week], date.day, date.month, date.year);
    
    if (!doctor->available_days[day_of_week]) {
        printf("Doctor is not available on this day.\n");
        return;
    }
    
    int start_hour = doctor->available_start_time[0];
    int start_minute = doctor->available_start_time[1];
    int end_hour = doctor->available_end_time[0];
    int end_minute = doctor->available_end_time[1];
    
    printf("Working Hours: %02d:%02d - %02d:%02d\n", 
           start_hour, start_minute, end_hour, end_minute);
    
    // Get all appointments for that day
    int appointment_count = 0;
    Appointment day_appointments[50];
    
    for (int i = 0; i < db->appointment_count; i++) {
        Appointment *appointment = &db->appointments[i];
        
        if (appointment->doctor_id == doctor_id && 
            appointment->appointment_time.day == date.day &&
            appointment->appointment_time.month == date.month &&
            appointment->appointment_time.year == date.year &&
            appointment->status == SCHEDULED) {
            
            day_appointments[appointment_count++] = *appointment;
        }
    }
    
    // Sort appointments by time
    for (int i = 0; i < appointment_count - 1; i++) {
        for (int j = 0; j < appointment_count - i - 1; j++) {
            int time1 = day_appointments[j].appointment_time.hour * 60 + day_appointments[j].appointment_time.minute;
            int time2 = day_appointments[j + 1].appointment_time.hour * 60 + day_appointments[j + 1].appointment_time.minute;
            
            if (time1 > time2) {
                Appointment temp = day_appointments[j];
                day_appointments[j] = day_appointments[j + 1];
                day_appointments[j + 1] = temp;
            }
        }
    }
    
    // Find available slots
    printf("\nAvailable Slots:\n");
    
    int current_time = start_hour * 60 + start_minute;
    int end_time = end_hour * 60 + end_minute;
    
    for (int i = 0; i <= appointment_count; i++) {
        int next_appointment_time = end_time;
        
        if (i < appointment_count) {
            next_appointment_time = day_appointments[i].appointment_time.hour * 60 + 
                                 day_appointments[i].appointment_time.minute;
        }
        
        if (current_time < next_appointment_time) {
            int slot_duration = next_appointment_time - current_time;
            
            if (slot_duration >= 15) { // Only show slots that are at least 15 minutes
                printf("  %02d:%02d - %02d:%02d (%d minutes available)\n",
                       current_time / 60, current_time % 60,
                       next_appointment_time / 60, next_appointment_time % 60,
                       slot_duration);
                
                // Show some standard appointment options (15, 30, 45, 60 minutes)
                if (slot_duration >= 15) {
                    printf("    - %02d:%02d (15 min)\n",
                           current_time / 60, current_time % 60);
                }
                
                if (slot_duration >= 30) {
                    printf("    - %02d:%02d (30 min)\n",
                           current_time / 60, current_time % 60);
                }
                
                if (slot_duration >= 45) {
                    printf("    - %02d:%02d (45 min)\n",
                           current_time / 60, current_time % 60);
                }
                
                if (slot_duration >= 60) {
                    printf("    - %02d:%02d (60 min)\n",
                           current_time / 60, current_time % 60);
                }
            }
        }
        
        if (i < appointment_count) {
            // Move to after the current appointment
            current_time = day_appointments[i].appointment_time.hour * 60 + 
                          day_appointments[i].appointment_time.minute +
                          day_appointments[i].duration_minutes;
        }
    }
    
    if (appointment_count == 0) {
        printf("  Entire day is available from %02d:%02d to %02d:%02d\n",
               start_hour, start_minute, end_hour, end_minute);
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

const char* get_appointment_status_name(AppointmentStatus status) {
    switch (status) {
        case SCHEDULED: return "Scheduled";
        case COMPLETED: return "Completed";
        case CANCELLED: return "Cancelled";
        case NO_SHOW: return "No Show";
        default: return "Unknown";
    }
}

const char* get_treatment_type_name(TreatmentType type) {
    switch (type) {
        case CONSULTATION: return "Consultation";
        case TEST: return "Test";
        case SURGERY: return "Surgery";
        case MEDICATION: return "Medication";
        case THERAPY: return "Therapy";
        case VACCINATION: return "Vaccination";
        case CHECKUP: return "Checkup";
        case EMERGENCY: return "Emergency";
        default: return "Unknown";
    }
}