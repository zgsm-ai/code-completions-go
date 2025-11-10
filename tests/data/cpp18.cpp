#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <memory>
#include <cmath>
#include <queue>
#include <stack>
#include <limits>
#include <random>

using namespace std;

#define MAX_PATIENTS 1000
#define MAX_DOCTORS 100
#define MAX_APPOINTMENTS 2000
#define MAX_PRESCRIPTIONS 3000

struct date {
    int year;
    int month;
    int day;
};

struct time {
    int hour;
    int minute;
};

class Doctor;
class Patient;
class Appointment;
class Prescription;
class Medication;

enum Gender {
    MALE = 1,
    FEMALE = 2,
    OTHER = 3
};

enum AppointmentStatus {
    SCHEDULED = 1,
    COMPLETED = 2,
    CANCELLED = 3,
    NO_SHOW = 4
};

class Medication {
private:
    int medicationId;
    string name;
    string description;
    string dosage;
    string sideEffects;
    
public:
    Medication(int medicationId = 0, const string& name = "", const string& description = "",
              const string& dosage = "", const string& sideEffects = "")
        : medicationId(medicationId), name(name), description(description),
          dosage(dosage), sideEffects(sideEffects) {}
    
    int getMedicationId() const { return medicationId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    string getDosage() const { return dosage; }
    string getSideEffects() const { return sideEffects; }
    
    void setMedicationId(int id) { medicationId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setDosage(const string& dosage) { this->dosage = dosage; }
    void setSideEffects(const string& sideEffects) { this->sideEffects = sideEffects; }
    
    void display() const {
        cout << "Medication ID: " << medicationId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Dosage: " << dosage << endl;
        cout << "Side Effects: " << sideEffects << endl;
    }
};

class Patient {
private:
    int patientId;
    string firstName;
    string lastName;
    date dateOfBirth;
    string address;
    string phone;
    string email;
    Gender gender;
    string bloodType;
    string allergies;
    string emergencyContactName;
    string emergencyContactPhone;
    date registrationDate;
    vector<int> appointmentIds;
    vector<int> prescriptionIds;
    
public:
    Patient(int patientId = 0, const string& firstName = "", const string& lastName = "",
            date dateOfBirth = {0, 0, 0}, const string& address = "", 
            const string& phone = "", const string& email = "", Gender gender = MALE,
            const string& bloodType = "", const string& allergies = "",
            const string& emergencyContactName = "", const string& emergencyContactPhone = "",
            date registrationDate = {0, 0, 0})
        : patientId(patientId), firstName(firstName), lastName(lastName), dateOfBirth(dateOfBirth),
          address(address), phone(phone), email(email), gender(gender),
          bloodType(bloodType), allergies(allergies), emergencyContactName(emergencyContactName),
          emergencyContactPhone(emergencyContactPhone), registrationDate(registrationDate) {}
    
    int getPatientId() const { return patientId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    date getDateOfBirth() const { return dateOfBirth; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    Gender getGender() const { return gender; }
    string getBloodType() const { return bloodType; }
    string getAllergies() const { return allergies; }
    string getEmergencyContactName() const { return emergencyContactName; }
    string getEmergencyContactPhone() const { return emergencyContactPhone; }
    date getRegistrationDate() const { return registrationDate; }
    vector<int> getAppointmentIds() const { return appointmentIds; }
    vector<int> getPrescriptionIds() const { return prescriptionIds; }
    
    void setPatientId(int id) { patientId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setDateOfBirth(date date) { dateOfBirth = date; }
    void setAddress(const string& address) { this->address = address; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setEmail(const string& email) { this->email = email; }
    void setGender(Gender gender) { this->gender = gender; }
    void setBloodType(const string& bloodType) { this->bloodType = bloodType; }
    void setAllergies(const string& allergies) { this->allergies = allergies; }
    void setEmergencyContactName(const string& name) { emergencyContactName = name; }
    void setEmergencyContactPhone(const string& phone) { emergencyContactPhone = phone; }
    void setRegistrationDate(date date) { registrationDate = date; }
    
    void addAppointment(int appointmentId) {
        if (find(appointmentIds.begin(), appointmentIds.end(), appointmentId) == appointmentIds.end()) {
            appointmentIds.push_back(appointmentId);
        }
    }
    
    void removeAppointment(int appointmentId) {
        auto it = find(appointmentIds.begin(), appointmentIds.end(), appointmentId);
        if (it != appointmentIds.end()) {
            appointmentIds.erase(it);
        }
    }
    
    void addPrescription(int prescriptionId) {
        if (find(prescriptionIds.begin(), prescriptionIds.end(), prescriptionId) == prescriptionIds.end()) {
            prescriptionIds.push_back(prescriptionId);
        }
    }
    
    void removePrescription(int prescriptionId) {
        auto it = find(prescriptionIds.begin(), prescriptionIds.end(), prescriptionId);
        if (it != prescriptionIds.end()) {
            prescriptionIds.erase(it);
        }
    }
    
    int getAge() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        int age = today.year - dateOfBirth.year;
        if (today.month < dateOfBirth.month || 
            (today.month == dateOfBirth.month && today.day < dateOfBirth.day)) {
            age--;
        }
        
        return age;
    }
    
    string getGenderString() const {
        switch (gender) {
            case MALE: return "Male";
            case FEMALE: return "Female";
            case OTHER: return "Other";
            default: return "Unknown";
        }
    }
    
    void display() const {
        cout << "Patient ID: " << patientId << endl;
        cout << "Name: " << getFullName() << endl;
        
        // Format date of birth
        tm tm = {};
        tm.tm_year = dateOfBirth.year - 1900;
        tm.tm_mon = dateOfBirth.month - 1;
        tm.tm_mday = dateOfBirth.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Date of Birth: " << dateStr << " (Age: " << getAge() << ")" << endl;
        
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "Gender: " << getGenderString() << endl;
        cout << "Blood Type: " << bloodType << endl;
        cout << "Allergies: " << allergies << endl;
        cout << "Emergency Contact: " << emergencyContactName << " (" << emergencyContactPhone << ")" << endl;
        
        // Format registration date
        tm.tm_year = registrationDate.year - 1900;
        tm.tm_mon = registrationDate.month - 1;
        tm.tm_mday = registrationDate.day;
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Registration Date: " << dateStr << endl;
        
        cout << "Appointments: " << appointmentIds.size() << endl;
        cout << "Prescriptions: " << prescriptionIds.size() << endl;
    }
};

class Doctor {
private:
    int doctorId;
    string firstName;
    string lastName;
    string specialization;
    string licenseNumber;
    string phone;
    string email;
    date hireDate;
    bool isActive;
    vector<int> appointmentIds;
    
public:
    Doctor(int doctorId = 0, const string& firstName = "", const string& lastName = "",
           const string& specialization = "", const string& licenseNumber = "",
           const string& phone = "", const string& email = "", date hireDate = {0, 0, 0},
           bool isActive = true)
        : doctorId(doctorId), firstName(firstName), lastName(lastName), specialization(specialization),
          licenseNumber(licenseNumber), phone(phone), email(email), hireDate(hireDate), isActive(isActive) {}
    
    int getDoctorId() const { return doctorId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getSpecialization() const { return specialization; }
    string getLicenseNumber() const { return licenseNumber; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    date getHireDate() const { return hireDate; }
    bool getIsActive() const { return isActive; }
    vector<int> getAppointmentIds() const { return appointmentIds; }
    
    void setDoctorId(int id) { doctorId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setSpecialization(const string& specialization) { this->specialization = specialization; }
    void setLicenseNumber(const string& licenseNumber) { this->licenseNumber = licenseNumber; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setEmail(const string& email) { this->email = email; }
    void setHireDate(date date) { hireDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    
    void addAppointment(int appointmentId) {
        if (find(appointmentIds.begin(), appointmentIds.end(), appointmentId) == appointmentIds.end()) {
            appointmentIds.push_back(appointmentId);
        }
    }
    
    void removeAppointment(int appointmentId) {
        auto it = find(appointmentIds.begin(), appointmentIds.end(), appointmentId);
        if (it != appointmentIds.end()) {
            appointmentIds.erase(it);
        }
    }
    
    int getYearsOfService() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        int years = today.year - hireDate.year;
        if (today.month < hireDate.month || 
            (today.month == hireDate.month && today.day < hireDate.day)) {
            years--;
        }
        
        return years;
    }
    
    void display() const {
        cout << "Doctor ID: " << doctorId << endl;
        cout << "Name: " << getFullName() << endl;
        cout << "Specialization: " << specialization << endl;
        cout << "License Number: " << licenseNumber << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        
        // Format hire date
        tm tm = {};
        tm.tm_year = hireDate.year - 1900;
        tm.tm_mon = hireDate.month - 1;
        tm.tm_mday = hireDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Hire Date: " << dateStr << endl;
        
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "Years of Service: " << getYearsOfService() << endl;
        cout << "Appointments: " << appointmentIds.size() << endl;
    }
};

class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;
    date appointmentDate;
    time appointmentTime;
    string reason;
    AppointmentStatus status;
    string notes;
    date creationDate;
    
public:
    Appointment(int appointmentId = 0, int patientId = 0, int doctorId = 0,
               date appointmentDate = {0, 0, 0}, time appointmentTime = {0, 0},
               const string& reason = "", AppointmentStatus status = SCHEDULED,
               const string& notes = "", date creationDate = {0, 0, 0})
        : appointmentId(appointmentId), patientId(patientId), doctorId(doctorId),
          appointmentDate(appointmentDate), appointmentTime(appointmentTime), reason(reason),
          status(status), notes(notes), creationDate(creationDate) {}
    
    int getAppointmentId() const { return appointmentId; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }
    date getAppointmentDate() const { return appointmentDate; }
    time getAppointmentTime() const { return appointmentTime; }
    string getReason() const { return reason; }
    AppointmentStatus getStatus() const { return status; }
    string getNotes() const { return notes; }
    date getCreationDate() const { return creationDate; }
    
    void setAppointmentId(int id) { appointmentId = id; }
    void setPatientId(int patientId) { this->patientId = patientId; }
    void setDoctorId(int doctorId) { this->doctorId = doctorId; }
    void setAppointmentDate(date date) { appointmentDate = date; }
    void setAppointmentTime(time time) { appointmentTime = time; }
    void setReason(const string& reason) { this->reason = reason; }
    void setStatus(AppointmentStatus status) { this->status = status; }
    void setNotes(const string& notes) { this->notes = notes; }
    void setCreationDate(date date) { creationDate = date; }
    
    string getStatusString() const {
        switch (status) {
            case SCHEDULED: return "Scheduled";
            case COMPLETED: return "Completed";
            case CANCELLED: return "Cancelled";
            case NO_SHOW: return "No Show";
            default: return "Unknown";
        }
    }
    
    bool isPast() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        if (today.year > appointmentDate.year) {
            return true;
        } else if (today.year == appointmentDate.year && today.month > appointmentDate.month) {
            return true;
        } else if (today.year == appointmentDate.year && today.month == appointmentDate.month && 
                   today.day > appointmentDate.day) {
            return true;
        } else if (today.year == appointmentDate.year && today.month == appointmentDate.month && 
                   today.day == appointmentDate.day) {
            return (timeinfo->tm_hour > appointmentTime.hour || 
                   (timeinfo->tm_hour == appointmentTime.hour && timeinfo->tm_min > appointmentTime.minute));
        }
        
        return false;
    }
    
    void display() const {
        cout << "Appointment ID: " << appointmentId << endl;
        cout << "Patient ID: " << patientId << endl;
        cout << "Doctor ID: " << doctorId << endl;
        
        // Format appointment date
        tm tm = {};
        tm.tm_year = appointmentDate.year - 1900;
        tm.tm_mon = appointmentDate.month - 1;
        tm.tm_mday = appointmentDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Appointment Date: " << dateStr << endl;
        
        cout << "Appointment Time: " << setfill('0') << setw(2) << appointmentTime.hour 
             << ":" << setw(2) << appointmentTime.minute << endl;
        
        cout << "Reason: " << reason << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Notes: " << notes << endl;
        
        // Format creation date
        tm.tm_year = creationDate.year - 1900;
        tm.tm_mon = creationDate.month - 1;
        tm.tm_mday = creationDate.day;
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Creation Date: " << dateStr << endl;
    }
};

class Prescription {
private:
    int prescriptionId;
    int patientId;
    int doctorId;
    date prescriptionDate;
    string diagnosis;
    vector<pair<int, string>> medications; // medicationId, dosage instructions
    string notes;
    bool isFilled;
    date filledDate;
    string pharmacyName;
    
public:
    Prescription(int prescriptionId = 0, int patientId = 0, int doctorId = 0,
                 date prescriptionDate = {0, 0, 0}, const string& diagnosis = "",
                 bool isFilled = false, date filledDate = {0, 0, 0},
                 const string& pharmacyName = "", const string& notes = "")
        : prescriptionId(prescriptionId), patientId(patientId), doctorId(doctorId),
          prescriptionDate(prescriptionDate), diagnosis(diagnosis), isFilled(isFilled),
          filledDate(filledDate), pharmacyName(pharmacyName), notes(notes) {}
    
    int getPrescriptionId() const { return prescriptionId; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }
    date getPrescriptionDate() const { return prescriptionDate; }
    string getDiagnosis() const { return diagnosis; }
    vector<pair<int, string>> getMedications() const { return medications; }
    string getNotes() const { return notes; }
    bool getIsFilled() const { return isFilled; }
    date getFilledDate() const { return filledDate; }
    string getPharmacyName() const { return pharmacyName; }
    
    void setPrescriptionId(int id) { prescriptionId = id; }
    void setPatientId(int patientId) { this->patientId = patientId; }
    void setDoctorId(int doctorId) { this->doctorId = doctorId; }
    void setPrescriptionDate(date date) { prescriptionDate = date; }
    void setDiagnosis(const string& diagnosis) { this->diagnosis = diagnosis; }
    void setNotes(const string& notes) { this->notes = notes; }
    void setIsFilled(bool isFilled) { this->isFilled = isFilled; }
    void setFilledDate(date date) { filledDate = date; }
    void setPharmacyName(const string& pharmacyName) { this->pharmacyName = pharmacyName; }
    
    void addMedication(int medicationId, const string& dosageInstructions) {
        medications.push_back({medicationId, dosageInstructions});
    }
    
    void removeMedication(int medicationId) {
        for (auto it = medications.begin(); it != medications.end(); ++it) {
            if (it->first == medicationId) {
                medications.erase(it);
                break;
            }
        }
    }
    
    void display(const vector<Medication>& allMedications) const {
        cout << "Prescription ID: " << prescriptionId << endl;
        cout << "Patient ID: " << patientId << endl;
        cout << "Doctor ID: " << doctorId << endl;
        
        // Format prescription date
        tm tm = {};
        tm.tm_year = prescriptionDate.year - 1900;
        tm.tm_mon = prescriptionDate.month - 1;
        tm.tm_mday = prescriptionDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Prescription Date: " << dateStr << endl;
        
        cout << "Diagnosis: " << diagnosis << endl;
        cout << "Medications:" << endl;
        
        for (const auto& medPair : medications) {
            cout << "  Medication ID: " << medPair.first;
            
            // Find medication name
            for (const auto& med : allMedications) {
                if (med.getMedicationId() == medPair.first) {
                    cout << " (" << med.getName() << ")";
                    break;
                }
            }
            
            cout << ", Dosage Instructions: " << medPair.second << endl;
        }
        
        cout << "Notes: " << notes << endl;
        cout << "Filled: " << (isFilled ? "Yes" : "No") << endl;
        
        if (isFilled) {
            // Format filled date
            tm.tm_year = filledDate.year - 1900;
            tm.tm_mon = filledDate.month - 1;
            tm.tm_mday = filledDate.day;
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
            cout << "Filled Date: " << dateStr << endl;
            cout << "Pharmacy: " << pharmacyName << endl;
        }
    }
};

class HospitalManagementSystem {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Medication> medications;
    vector<Appointment> appointments;
    vector<Prescription> prescriptions;
    
    int nextPatientId;
    int nextDoctorId;
    int nextMedicationId;
    int nextAppointmentId;
    int nextPrescriptionId;
    
public:
    HospitalManagementSystem() {
        nextPatientId = 1;
        nextDoctorId = 1;
        nextMedicationId = 1;
        nextAppointmentId = 1;
        nextPrescriptionId = 1;
    }
    
    // Patient management
    Patient* findPatient(int patientId) {
        for (auto& patient : patients) {
            if (patient.getPatientId() == patientId) {
                return &patient;
            }
        }
        return nullptr;
    }
    
    bool addPatient(const Patient& patient) {
        if (patients.size() >= MAX_PATIENTS) {
            return false;
        }
        
        patients.push_back(patient);
        return true;
    }
    
    bool removePatient(int patientId) {
        for (auto it = patients.begin(); it != patients.end(); ++it) {
            if (it->getPatientId() == patientId) {
                patients.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Patient*> getAllPatients() {
        vector<Patient*> result;
        for (auto& patient : patients) {
            result.push_back(&patient);
        }
        return result;
    }
    
    vector<Patient*> searchPatientsByName(const string& name) {
        vector<Patient*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& patient : patients) {
            string fullName = patient.getFullName();
            transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            
            if (fullName.find(lowerName) != string::npos) {
                result.push_back(&patient);
            }
        }
        return result;
    }
    
    vector<Patient*> getPatientsByAge(int minAge, int maxAge) {
        vector<Patient*> result;
        
        for (auto& patient : patients) {
            int age = patient.getAge();
            if (age >= minAge && age <= maxAge) {
                result.push_back(&patient);
            }
        }
        return result;
    }
    
    int getNextPatientId() { return nextPatientId++; }
    
    // Doctor management
    Doctor* findDoctor(int doctorId) {
        for (auto& doctor : doctors) {
            if (doctor.getDoctorId() == doctorId) {
                return &doctor;
            }
        }
        return nullptr;
    }
    
    bool addDoctor(const Doctor& doctor) {
        if (doctors.size() >= MAX_DOCTORS) {
            return false;
        }
        
        doctors.push_back(doctor);
        return true;
    }
    
    bool removeDoctor(int doctorId) {
        for (auto it = doctors.begin(); it != doctors.end(); ++it) {
            if (it->getDoctorId() == doctorId) {
                doctors.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Doctor*> getAllDoctors() {
        vector<Doctor*> result;
        for (auto& doctor : doctors) {
            result.push_back(&doctor);
        }
        return result;
    }
    
    vector<Doctor*> searchDoctorsByName(const string& name) {
        vector<Doctor*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& doctor : doctors) {
            string fullName = doctor.getFullName();
            transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            
            if (fullName.find(lowerName) != string::npos) {
                result.push_back(&doctor);
            }
        }
        return result;
    }
    
    vector<Doctor*> getDoctorsBySpecialization(const string& specialization) {
        vector<Doctor*> result;
        string lowerSpec = specialization;
        transform(lowerSpec.begin(), lowerSpec.end(), lowerSpec.begin(), ::tolower);
        
        for (auto& doctor : doctors) {
            string doctorSpec = doctor.getSpecialization();
            transform(doctorSpec.begin(), doctorSpec.end(), doctorSpec.begin(), ::tolower);
            
            if (doctorSpec.find(lowerSpec) != string::npos) {
                result.push_back(&doctor);
            }
        }
        return result;
    }
    
    vector<Doctor*> getActiveDoctors() {
        vector<Doctor*> result;
        
        for (auto& doctor : doctors) {
            if (doctor.getIsActive()) {
                result.push_back(&doctor);
            }
        }
        return result;
    }
    
    int getNextDoctorId() { return nextDoctorId++; }
    
    // Medication management
    Medication* findMedication(int medicationId) {
        for (auto& medication : medications) {
            if (medication.getMedicationId() == medicationId) {
                return &medication;
            }
        }
        return nullptr;
    }
    
    bool addMedication(const Medication& medication) {
        medications.push_back(medication);
        return true;
    }
    
    bool removeMedication(int medicationId) {
        for (auto it = medications.begin(); it != medications.end(); ++it) {
            if (it->getMedicationId() == medicationId) {
                medications.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Medication*> getAllMedications() {
        vector<Medication*> result;
        for (auto& medication : medications) {
            result.push_back(&medication);
        }
        return result;
    }
    
    vector<Medication*> searchMedicationsByName(const string& name) {
        vector<Medication*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& medication : medications) {
            string medicationName = medication.getName();
            transform(medicationName.begin(), medicationName.end(), medicationName.begin(), ::tolower);
            
            if (medicationName.find(lowerName) != string::npos) {
                result.push_back(&medication);
            }
        }
        return result;
    }
    
    int getNextMedicationId() { return nextMedicationId++; }
    
    // Appointment management
    Appointment* findAppointment(int appointmentId) {
        for (auto& appointment : appointments) {
            if (appointment.getAppointmentId() == appointmentId) {
                return &appointment;
            }
        }
        return nullptr;
    }
    
    bool addAppointment(const Appointment& appointment) {
        if (appointments.size() >= MAX_APPOINTMENTS) {
            return false;
        }
        
        appointments.push_back(appointment);
        return true;
    }
    
    bool removeAppointment(int appointmentId) {
        for (auto it = appointments.begin(); it != appointments.end(); ++it) {
            if (it->getAppointmentId() == appointmentId) {
                appointments.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Appointment*> getAllAppointments() {
        vector<Appointment*> result;
        for (auto& appointment : appointments) {
            result.push_back(&appointment);
        }
        return result;
    }
    
    vector<Appointment*> getAppointmentsByPatient(int patientId) {
        vector<Appointment*> result;
        for (auto& appointment : appointments) {
            if (appointment.getPatientId() == patientId) {
                result.push_back(&appointment);
            }
        }
        return result;
    }
    
    vector<Appointment*> getAppointmentsByDoctor(int doctorId) {
        vector<Appointment*> result;
        for (auto& appointment : appointments) {
            if (appointment.getDoctorId() == doctorId) {
                result.push_back(&appointment);
            }
        }
        return result;
    }
    
    vector<Appointment*> getAppointmentsByDate(int year, int month, int day) {
        vector<Appointment*> result;
        for (auto& appointment : appointments) {
            if (appointment.getAppointmentDate().year == year &&
                appointment.getAppointmentDate().month == month &&
                appointment.getAppointmentDate().day == day) {
                result.push_back(&appointment);
            }
        }
        return result;
    }
    
    vector<Appointment*> getAppointmentsByStatus(AppointmentStatus status) {
        vector<Appointment*> result;
        for (auto& appointment : appointments) {
            if (appointment.getStatus() == status) {
                result.push_back(&appointment);
            }
        }
        return result;
    }
    
    vector<Appointment*> getPastAppointments() {
        vector<Appointment*> result;
        for (auto& appointment : appointments) {
            if (appointment.isPast()) {
                result.push_back(&appointment);
            }
        }
        return result;
    }
    
    vector<Appointment*> getUpcomingAppointments() {
        vector<Appointment*> result;
        for (auto& appointment : appointments) {
            if (!appointment.isPast()) {
                result.push_back(&appointment);
            }
        }
        return result;
    }
    
    int getNextAppointmentId() { return nextAppointmentId++; }
    
    // Prescription management
    Prescription* findPrescription(int prescriptionId) {
        for (auto& prescription : prescriptions) {
            if (prescription.getPrescriptionId() == prescriptionId) {
                return &prescription;
            }
        }
        return nullptr;
    }
    
    bool addPrescription(const Prescription& prescription) {
        if (prescriptions.size() >= MAX_PRESCRIPTIONS) {
            return false;
        }
        
        prescriptions.push_back(prescription);
        return true;
    }
    
    bool removePrescription(int prescriptionId) {
        for (auto it = prescriptions.begin(); it != prescriptions.end(); ++it) {
            if (it->getPrescriptionId() == prescriptionId) {
                prescriptions.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Prescription*> getAllPrescriptions() {
        vector<Prescription*> result;
        for (auto& prescription : prescriptions) {
            result.push_back(&prescription);
        }
        return result;
    }
    
    vector<Prescription*> getPrescriptionsByPatient(int patientId) {
        vector<Prescription*> result;
        for (auto& prescription : prescriptions) {
            if (prescription.getPatientId() == patientId) {
                result.push_back(&prescription);
            }
        }
        return result;
    }
    
    vector<Prescription*> getPrescriptionsByDoctor(int doctorId) {
        vector<Prescription*> result;
        for (auto& prescription : prescriptions) {
            if (prescription.getDoctorId() == doctorId) {
                result.push_back(&prescription);
            }
        }
        return result;
    }
    
    vector<Prescription*> getFilledPrescriptions() {
        vector<Prescription*> result;
        for (auto& prescription : prescriptions) {
            if (prescription.getIsFilled()) {
                result.push_back(&prescription);
            }
        }
        return result;
    }
    
    vector<Prescription*> getUnfilledPrescriptions() {
        vector<Prescription*> result;
        for (auto& prescription : prescriptions) {
            if (!prescription.getIsFilled()) {
                result.push_back(&prescription);
            }
        }
        return result;
    }
    
    int getNextPrescriptionId() { return nextPrescriptionId++; }
    
    // Business operations
    bool scheduleAppointment(int patientId, int doctorId, const date& appointmentDate, 
                           const time& appointmentTime, const string& reason) {
        // Check if patient and doctor exist
        Patient* patient = findPatient(patientId);
        Doctor* doctor = findDoctor(doctorId);
        
        if (!patient || !doctor || !doctor->getIsActive()) {
            return false;
        }
        
        // Get current date for creation date
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date creationDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        Appointment appointment(getNextAppointmentId(), patientId, doctorId, 
                             appointmentDate, appointmentTime, reason, SCHEDULED, 
                             "", creationDate);
        
        if (!addAppointment(appointment)) {
            return false;
        }
        
        // Add appointment to patient and doctor
        patient->addAppointment(appointment.getAppointmentId());
        doctor->addAppointment(appointment.getAppointmentId());
        
        return true;
    }
    
    bool completeAppointment(int appointmentId, const string& notes) {
        Appointment* appointment = findAppointment(appointmentId);
        if (!appointment || appointment->getStatus() != SCHEDULED) {
            return false;
        }
        
        appointment->setStatus(COMPLETED);
        appointment->setNotes(notes);
        
        return true;
    }
    
    bool cancelAppointment(int appointmentId, const string& reason) {
        Appointment* appointment = findAppointment(appointmentId);
        if (!appointment || appointment->getStatus() != SCHEDULED) {
            return false;
        }
        
        appointment->setStatus(CANCELLED);
        appointment->setNotes(reason);
        
        return true;
    }
    
    bool markNoShow(int appointmentId, const string& notes) {
        Appointment* appointment = findAppointment(appointmentId);
        if (!appointment || appointment->getStatus() != SCHEDULED) {
            return false;
        }
        
        appointment->setStatus(NO_SHOW);
        appointment->setNotes(notes);
        
        return true;
    }
    
    bool createPrescription(int patientId, int doctorId, const string& diagnosis,
                         const vector<pair<int, string>>& medications, const string& notes) {
        // Check if patient and doctor exist
        Patient* patient = findPatient(patientId);
        Doctor* doctor = findDoctor(doctorId);
        
        if (!patient || !doctor) {
            return false;
        }
        
        // Get current date for prescription date
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date prescriptionDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        Prescription prescription(getNextPrescriptionId(), patientId, doctorId, 
                               prescriptionDate, diagnosis, false, {0, 0, 0}, 
                               "", notes);
        
        // Add medications to prescription
        for (const auto& medPair : medications) {
            prescription.addMedication(medPair.first, medPair.second);
        }
        
        if (!addPrescription(prescription)) {
            return false;
        }
        
        // Add prescription to patient and doctor
        patient->addPrescription(prescription.getPrescriptionId());
        
        return true;
    }
    
    bool fillPrescription(int prescriptionId, const string& pharmacyName) {
        Prescription* prescription = findPrescription(prescriptionId);
        if (!prescription || prescription->getIsFilled()) {
            return false;
        }
        
        prescription->setIsFilled(true);
        prescription->setPharmacyName(pharmacyName);
        
        // Get current date for filled date
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date filledDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        prescription->setFilledDate(filledDate);
        
        return true;
    }
    
    // Reporting
    void generatePatientReport(int patientId) {
        Patient* patient = findPatient(patientId);
        if (!patient) {
            cout << "Patient not found." << endl;
            return;
        }
        
        cout << "Patient Report" << endl;
        cout << "==============" << endl;
        patient->display();
        cout << endl;
        
        // Display appointments
        vector<Appointment*> patientAppointments = getAppointmentsByPatient(patientId);
        if (!patientAppointments.empty()) {
            cout << "Appointments (" << patientAppointments.size() << "):" << endl;
            for (Appointment* appointment : patientAppointments) {
                cout << "  ID: " << appointment->getAppointmentId()
                     << ", Date: " << appointment->getAppointmentDate().year 
                     << "-" << appointment->getAppointmentDate().month 
                     << "-" << appointment->getAppointmentDate().day
                     << ", Time: " << setfill('0') << setw(2) << appointment->getAppointmentTime().hour 
                     << ":" << setw(2) << appointment->getAppointmentTime().minute
                     << ", Status: " << appointment->getStatusString() << endl;
            }
        } else {
            cout << "No appointments found." << endl;
        }
        
        cout << endl;
        
        // Display prescriptions
        vector<Prescription*> patientPrescriptions = getPrescriptionsByPatient(patientId);
        if (!patientPrescriptions.empty()) {
            cout << "Prescriptions (" << patientPrescriptions.size() << "):" << endl;
            for (Prescription* prescription : patientPrescriptions) {
                cout << "  ID: " << prescription->getPrescriptionId()
                     << ", Date: " << prescription->getPrescriptionDate().year 
                     << "-" << prescription->getPrescriptionDate().month 
                     << "-" << prescription->getPrescriptionDate().day
                     << ", Diagnosis: " << prescription->getDiagnosis()
                     << ", Filled: " << (prescription->getIsFilled() ? "Yes" : "No") << endl;
            }
        } else {
            cout << "No prescriptions found." << endl;
        }
    }
    
    void generateDoctorReport(int doctorId) {
        Doctor* doctor = findDoctor(doctorId);
        if (!doctor) {
            cout << "Doctor not found." << endl;
            return;
        }
        
        cout << "Doctor Report" << endl;
        cout << "=============" << endl;
        doctor->display();
        cout << endl;
        
        // Display appointments
        vector<Appointment*> doctorAppointments = getAppointmentsByDoctor(doctorId);
        if (!doctorAppointments.empty()) {
            // Group appointments by status
            map<AppointmentStatus, int> statusCounts;
            
            for (Appointment* appointment : doctorAppointments) {
                statusCounts[appointment->getStatus()]++;
            }
            
            cout << "Appointment Summary:" << endl;
            for (const auto& pair : statusCounts) {
                Appointment dummyAppointment;
                cout << "  " << dummyAppointment.getStatusStringStatic(pair.first) 
                     << ": " << pair.second << endl;
            }
            
            cout << endl << "Recent Appointments:" << endl;
            // Sort appointments by date and time
            sort(doctorAppointments.begin(), doctorAppointments.end(), 
                [](const Appointment* a, const Appointment* b) {
                    if (a->getAppointmentDate().year != b->getAppointmentDate().year) {
                        return a->getAppointmentDate().year > b->getAppointmentDate().year;
                    } else if (a->getAppointmentDate().month != b->getAppointmentDate().month) {
                        return a->getAppointmentDate().month > b->getAppointmentDate().month;
                    } else if (a->getAppointmentDate().day != b->getAppointmentDate().day) {
                        return a->getAppointmentDate().day > b->getAppointmentDate().day;
                    } else {
                        if (a->getAppointmentTime().hour != b->getAppointmentTime().hour) {
                            return a->getAppointmentTime().hour > b->getAppointmentTime().hour;
                        } else {
                            return a->getAppointmentTime().minute > b->getAppointmentTime().minute;
                        }
                    }
                });
            
            // Display the most recent 5 appointments
            int displayCount = min(5, static_cast<int>(doctorAppointments.size()));
            for (int i = 0; i < displayCount; i++) {
                Appointment* appointment = doctorAppointments[i];
                cout << "  ID: " << appointment->getAppointmentId()
                     << ", Patient ID: " << appointment->getPatientId()
                     << ", Date: " << appointment->getAppointmentDate().year 
                     << "-" << appointment->getAppointmentDate().month 
                     << "-" << appointment->getAppointmentDate().day
                     << ", Time: " << setfill('0') << setw(2) << appointment->getAppointmentTime().hour 
                     << ":" << setw(2) << appointment->getAppointmentTime().minute
                     << ", Status: " << appointment->getStatusString() << endl;
            }
        } else {
            cout << "No appointments found." << endl;
        }
        
        cout << endl;
        
        // Display prescriptions
        vector<Prescription*> doctorPrescriptions = getPrescriptionsByDoctor(doctorId);
        if (!doctorPrescriptions.empty()) {
            cout << "Prescriptions (" << doctorPrescriptions.size() << "):" << endl;
            
            // Count filled vs unfilled prescriptions
            int filledCount = 0;
            for (Prescription* prescription : doctorPrescriptions) {
                if (prescription->getIsFilled()) {
                    filledCount++;
                }
            }
            
            cout << "  Filled: " << filledCount << endl;
            cout << "  Unfilled: " << (doctorPrescriptions.size() - filledCount) << endl;
        } else {
            cout << "No prescriptions found." << endl;
        }
    }
    
    void generateAppointmentReport(int year, int month) {
        cout << "Appointment Report for " << year << "-" << month << endl;
        cout << "============================" << endl;
        
        vector<Appointment*> monthlyAppointments = getAppointmentsByDate(year, month, 0); // Use 0 for day to get all appointments in the month
        
        // This would need to be implemented properly to filter by month only
        // For now, we'll just get all appointments and manually filter
        monthlyAppointments.clear();
        for (auto& appointment : appointments) {
            if (appointment.getAppointmentDate().year == year && 
                appointment.getAppointmentDate().month == month) {
                monthlyAppointments.push_back(&appointment);
            }
        }
        
        if (monthlyAppointments.empty()) {
            cout << "No appointments found for this month." << endl;
            return;
        }
        
        // Group appointments by status
        map<AppointmentStatus, int> statusCounts;
        
        for (Appointment* appointment : monthlyAppointments) {
            statusCounts[appointment->getStatus()]++;
        }
        
        cout << "Appointment Summary:" << endl;
        for (const auto& pair : statusCounts) {
            Appointment dummyAppointment;
            cout << "  " << dummyAppointment.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        // Group appointments by doctor
        map<int, int> doctorCounts;
        for (Appointment* appointment : monthlyAppointments) {
            doctorCounts[appointment->getDoctorId()]++;
        }
        
        cout << endl << "Appointment Distribution by Doctor:" << endl;
        for (const auto& pair : doctorCounts) {
            Doctor* doctor = findDoctor(pair.first);
            if (doctor) {
                cout << "  " << doctor->getFullName() << " (ID: " << pair.first 
                     << "): " << pair.second << " appointments" << endl;
            }
        }
        
        // Group appointments by day
        map<int, vector<Appointment*>> dayGroups;
        for (Appointment* appointment : monthlyAppointments) {
            dayGroups[appointment->getAppointmentDate().day].push_back(appointment);
        }
        
        cout << endl << "Daily Breakdown:" << endl;
        for (const auto& pair : dayGroups) {
            cout << "  " << year << "-" << month << "-" << pair.first << ": "
                 << pair.second.size() << " appointments" << endl;
        }
    }
    
    void generatePrescriptionReport() {
        cout << "Prescription Report" << endl;
        cout << "===================" << endl;
        
        vector<Prescription*> allPrescriptions = getAllPrescriptions();
        if (allPrescriptions.empty()) {
            cout << "No prescriptions found." << endl;
            return;
        }
        
        // Count filled vs unfilled prescriptions
        int filledCount = 0;
        map<string, int> pharmacyCounts;
        
        for (Prescription* prescription : allPrescriptions) {
            if (prescription->getIsFilled()) {
                filledCount++;
                pharmacyCounts[prescription->getPharmacyName()]++;
            }
        }
        
        cout << "Prescription Summary:" << endl;
        cout << "  Total: " << allPrescriptions.size() << endl;
        cout << "  Filled: " << filledCount << endl;
        cout << "  Unfilled: " << (allPrescriptions.size() - filledCount) << endl;
        
        cout << endl << "Pharmacy Distribution:" << endl;
        for (const auto& pair : pharmacyCounts) {
            cout << "  " << pair.first << ": " << pair.second << " prescriptions" << endl;
        }
        
        // Analyze common diagnoses
        map<string, int> diagnosisCounts;
        for (Prescription* prescription : allPrescriptions) {
            diagnosisCounts[prescription->getDiagnosis()]++;
        }
        
        cout << endl << "Top Diagnoses:" << endl;
        
        // Sort diagnoses by count
        vector<pair<string, int>> sortedDiagnoses(diagnosisCounts.begin(), diagnosisCounts.end());
        sort(sortedDiagnoses.begin(), sortedDiagnoses.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        int displayCount = min(5, static_cast<int>(sortedDiagnoses.size()));
        for (int i = 0; i < displayCount; i++) {
            cout << "  " << i + 1 << ". " << sortedDiagnoses[i].first 
                 << ": " << sortedDiagnoses[i].second << " prescriptions" << endl;
        }
        
        // Analyze medication usage
        map<int, int> medicationCounts;
        for (Prescription* prescription : allPrescriptions) {
            for (const auto& medPair : prescription->getMedications()) {
                medicationCounts[medPair.first]++;
            }
        }
        
        cout << endl << "Most Prescribed Medications:" << endl;
        
        // Sort medications by count
        vector<pair<int, int>> sortedMedications(medicationCounts.begin(), medicationCounts.end());
        sort(sortedMedications.begin(), sortedMedications.end(), 
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.second > b.second;
             });
        
        displayCount = min(5, static_cast<int>(sortedMedications.size()));
        for (int i = 0; i < displayCount; i++) {
            Medication* medication = findMedication(sortedMedications[i].first);
            if (medication) {
                cout << "  " << i + 1 << ". " << medication->getName() 
                     << ": " << sortedMedications[i].second << " times" << endl;
            }
        }
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        // Save next IDs
        outFile << "NEXT_IDS" << endl;
        outFile << nextPatientId << "|" << nextDoctorId << "|" << nextMedicationId << "|" 
                << nextAppointmentId << "|" << nextPrescriptionId << endl;
        
        // Save patients
        outFile << "PATIENTS" << endl;
        for (const auto& patient : patients) {
            outFile << patient.getPatientId() << "|" 
                    << patient.getFirstName() << "|" 
                    << patient.getLastName() << "|" 
                    << patient.getDateOfBirth().year << "|" 
                    << patient.getDateOfBirth().month << "|" 
                    << patient.getDateOfBirth().day << "|" 
                    << patient.getAddress() << "|" 
                    << patient.getPhone() << "|" 
                    << patient.getEmail() << "|" 
                    << static_cast<int>(patient.getGender()) << "|" 
                    << patient.getBloodType() << "|" 
                    << patient.getAllergies() << "|" 
                    << patient.getEmergencyContactName() << "|" 
                    << patient.getEmergencyContactPhone() << "|" 
                    << patient.getRegistrationDate().year << "|" 
                    << patient.getRegistrationDate().month << "|" 
                    << patient.getRegistrationDate().day << "|";
            
            // Save appointment IDs
            vector<int> appointmentIds = patient.getAppointmentIds();
            outFile << appointmentIds.size();
            for (int appointmentId : appointmentIds) {
                outFile << "," << appointmentId;
            }
            outFile << "|";
            
            // Save prescription IDs
            vector<int> prescriptionIds = patient.getPrescriptionIds();
            outFile << prescriptionIds.size();
            for (int prescriptionId : prescriptionIds) {
                outFile << "," << prescriptionId;
            }
            outFile << endl;
        }
        
        // Save doctors
        outFile << "DOCTORS" << endl;
        for (const auto& doctor : doctors) {
            outFile << doctor.getDoctorId() << "|" 
                    << doctor.getFirstName() << "|" 
                    << doctor.getLastName() << "|" 
                    << doctor.getSpecialization() << "|" 
                    << doctor.getLicenseNumber() << "|" 
                    << doctor.getPhone() << "|" 
                    << doctor.getEmail() << "|" 
                    << doctor.getHireDate().year << "|" 
                    << doctor.getHireDate().month << "|" 
                    << doctor.getHireDate().day << "|" 
                    << (doctor.getIsActive() ? "1" : "0") << "|";
            
            // Save appointment IDs
            vector<int> appointmentIds = doctor.getAppointmentIds();
            outFile << appointmentIds.size();
            for (int appointmentId : appointmentIds) {
                outFile << "," << appointmentId;
            }
            outFile << endl;
        }
        
        // Save medications
        outFile << "MEDICATIONS" << endl;
        for (const auto& medication : medications) {
            outFile << medication.getMedicationId() << "|" 
                    << medication.getName() << "|" 
                    << medication.getDescription() << "|" 
                    << medication.getDosage() << "|" 
                    << medication.getSideEffects() << endl;
        }
        
        // Save appointments
        outFile << "APPOINTMENTS" << endl;
        for (const auto& appointment : appointments) {
            outFile << appointment.getAppointmentId() << "|" 
                    << appointment.getPatientId() << "|" 
                    << appointment.getDoctorId() << "|" 
                    << appointment.getAppointmentDate().year << "|" 
                    << appointment.getAppointmentDate().month << "|" 
                    << appointment.getAppointmentDate().day << "|" 
                    << appointment.getAppointmentTime().hour << "|" 
                    << appointment.getAppointmentTime().minute << "|" 
                    << appointment.getReason() << "|" 
                    << static_cast<int>(appointment.getStatus()) << "|" 
                    << appointment.getNotes() << "|" 
                    << appointment.getCreationDate().year << "|" 
                    << appointment.getCreationDate().month << "|" 
                    << appointment.getCreationDate().day << endl;
        }
        
        // Save prescriptions
        outFile << "PRESCRIPTIONS" << endl;
        for (const auto& prescription : prescriptions) {
            outFile << prescription.getPrescriptionId() << "|" 
                    << prescription.getPatientId() << "|" 
                    << prescription.getDoctorId() << "|" 
                    << prescription.getPrescriptionDate().year << "|" 
                    << prescription.getPrescriptionDate().month << "|" 
                    << prescription.getPrescriptionDate().day << "|" 
                    << prescription.getDiagnosis() << "|" 
                    << (prescription.getIsFilled() ? "1" : "0") << "|" 
                    << prescription.getFilledDate().year << "|" 
                    << prescription.getFilledDate().month << "|" 
                    << prescription.getFilledDate().day << "|" 
                    << prescription.getPharmacyName() << "|" 
                    << prescription.getNotes() << "|";
            
            // Save medications
            vector<pair<int, string>> medPairs = prescription.getMedications();
            outFile << medPairs.size();
            for (const auto& medPair : medPairs) {
                outFile << "," << medPair.first << ":" << medPair.second;
            }
            outFile << endl;
        }
        
        outFile.close();
        return true;
    }
    
    bool loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            return false;
        }
        
        string line;
        string section = "";
        
        while (getline(inFile, line)) {
            if (line == "NEXT_IDS") {
                section = "NEXT_IDS";
                continue;
            } else if (line == "PATIENTS") {
                section = "PATIENTS";
                continue;
            } else if (line == "DOCTORS") {
                section = "DOCTORS";
                continue;
            } else if (line == "MEDICATIONS") {
                section = "MEDICATIONS";
                continue;
            } else if (line == "APPOINTMENTS") {
                section = "APPOINTMENTS";
                continue;
            } else if (line == "PRESCRIPTIONS") {
                section = "PRESCRIPTIONS";
                continue;
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "NEXT_IDS" && tokens.size() >= 5) {
                nextPatientId = stoi(tokens[0]);
                nextDoctorId = stoi(tokens[1]);
                nextMedicationId = stoi(tokens[2]);
                nextAppointmentId = stoi(tokens[3]);
                nextPrescriptionId = stoi(tokens[4]);
            } else if (section == "PATIENTS" && tokens.size() >= 20) {
                int patientId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                
                date dateOfBirth;
                dateOfBirth.year = stoi(tokens[3]);
                dateOfBirth.month = stoi(tokens[4]);
                dateOfBirth.day = stoi(tokens[5]);
                
                string address = tokens[6];
                string phone = tokens[7];
                string email = tokens[8];
                Gender gender = static_cast<Gender>(stoi(tokens[9]));
                string bloodType = tokens[10];
                string allergies = tokens[11];
                string emergencyContactName = tokens[12];
                string emergencyContactPhone = tokens[13];
                
                date registrationDate;
                registrationDate.year = stoi(tokens[14]);
                registrationDate.month = stoi(tokens[15]);
                registrationDate.day = stoi(tokens[16]);
                
                bool isActive = (tokens[17] == "1");
                
                Patient patient(patientId, firstName, lastName, dateOfBirth, address, phone, 
                              email, gender, bloodType, allergies, emergencyContactName, 
                              emergencyContactPhone, registrationDate);
                
                // Parse appointment IDs
                int appointmentCount = stoi(tokens[18]);
                if (appointmentCount > 0) {
                    stringstream appointmentSs(tokens[19].substr(1)); // Skip first character
                    string appointmentToken;
                    while (getline(appointmentSs, appointmentToken, ',')) {
                        patient.addAppointment(stoi(appointmentToken));
                    }
                }
                
                // Parse prescription IDs
                if (tokens.size() > 20) {
                    int prescriptionCount = stoi(tokens[20]);
                    if (prescriptionCount > 0) {
                        stringstream prescriptionSs(tokens[21].substr(1)); // Skip first character
                        string prescriptionToken;
                        while (getline(prescriptionSs, prescriptionToken, ',')) {
                            patient.addPrescription(stoi(prescriptionToken));
                        }
                    }
                }
                
                patients.push_back(patient);
            } else if (section == "DOCTORS" && tokens.size() >= 12) {
                int doctorId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string specialization = tokens[3];
                string licenseNumber = tokens[4];
                string phone = tokens[5];
                string email = tokens[6];
                
                date hireDate;
                hireDate.year = stoi(tokens[7]);
                hireDate.month = stoi(tokens[8]);
                hireDate.day = stoi(tokens[9]);
                
                bool isActive = (tokens[10] == "1");
                
                Doctor doctor(doctorId, firstName, lastName, specialization, licenseNumber, 
                               phone, email, hireDate, isActive);
                
                // Parse appointment IDs
                int appointmentCount = stoi(tokens[11]);
                if (appointmentCount > 0) {
                    stringstream appointmentSs(tokens[12].substr(1)); // Skip first character
                    string appointmentToken;
                    while (getline(appointmentSs, appointmentToken, ',')) {
                        doctor.addAppointment(stoi(appointmentToken));
                    }
                }
                
                doctors.push_back(doctor);
            } else if (section == "MEDICATIONS" && tokens.size() >= 5) {
                int medicationId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                string dosage = tokens[3];
                string sideEffects = tokens[4];
                
                Medication medication(medicationId, name, description, dosage, sideEffects);
                medications.push_back(medication);
            } else if (section == "APPOINTMENTS" && tokens.size() >= 14) {
                int appointmentId = stoi(tokens[0]);
                int patientId = stoi(tokens[1]);
                int doctorId = stoi(tokens[2]);
                
                date appointmentDate;
                appointmentDate.year = stoi(tokens[3]);
                appointmentDate.month = stoi(tokens[4]);
                appointmentDate.day = stoi(tokens[5]);
                
                time appointmentTime;
                appointmentTime.hour = stoi(tokens[6]);
                appointmentTime.minute = stoi(tokens[7]);
                
                string reason = tokens[8];
                AppointmentStatus status = static_cast<AppointmentStatus>(stoi(tokens[9]));
                string notes = tokens[10];
                
                date creationDate;
                creationDate.year = stoi(tokens[11]);
                creationDate.month = stoi(tokens[12]);
                creationDate.day = stoi(tokens[13]);
                
                Appointment appointment(appointmentId, patientId, doctorId, appointmentDate, 
                                   appointmentTime, reason, status, notes, creationDate);
                appointments.push_back(appointment);
            } else if (section == "PRESCRIPTIONS" && tokens.size() >= 11) {
                int prescriptionId = stoi(tokens[0]);
                int patientId = stoi(tokens[1]);
                int doctorId = stoi(tokens[2]);
                
                date prescriptionDate;
                prescriptionDate.year = stoi(tokens[3]);
                prescriptionDate.month = stoi(tokens[4]);
                prescriptionDate.day = stoi(tokens[5]);
                
                string diagnosis = tokens[6];
                bool isFilled = (tokens[7] == "1");
                
                date filledDate;
                filledDate.year = stoi(tokens[8]);
                filledDate.month = stoi(tokens[9]);
                filledDate.day = stoi(tokens[10]);
                
                string pharmacyName = tokens[11];
                string notes = "";
                
                Prescription prescription(prescriptionId, patientId, doctorId, prescriptionDate, 
                                       diagnosis, isFilled, filledDate, pharmacyName, notes);
                
                // Parse medications
                if (tokens.size() > 12) {
                    int medicationCount = stoi(tokens[12]);
                    if (medicationCount > 0) {
                        stringstream medicationSs(tokens[13].substr(1)); // Skip first character
                        string medicationToken;
                        while (getline(medicationSs, medicationToken, ',')) {
                            size_t colonPos = medicationToken.find(':');
                            if (colonPos != string::npos) {
                                int medId = stoi(medicationToken.substr(0, colonPos));
                                string dosageInstructions = medicationToken.substr(colonPos + 1);
                                prescription.addMedication(medId, dosageInstructions);
                            }
                        }
                    }
                }
                
                prescriptions.push_back(prescription);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Sample data generation
    void generateSampleData() {
        // Clear existing data
        patients.clear();
        doctors.clear();
        medications.clear();
        appointments.clear();
        prescriptions.clear();
        
        nextPatientId = 1;
        nextDoctorId = 1;
        nextMedicationId = 1;
        nextAppointmentId = 1;
        nextPrescriptionId = 1;
        
        // Create sample medications
        vector<Medication> sampleMedications = {
            Medication(getNextMedicationId(), "Amoxicillin", "Antibiotic", "500mg, 3 times daily", "Nausea, diarrhea, allergic reactions"),
            Medication(getNextMedicationId(), "Ibuprofen", "Anti-inflammatory", "400mg, as needed", "Stomach upset, headache"),
            Medication(getNextMedicationId(), "Acetaminophen", "Pain reliever", "500mg, as needed", "Liver damage in high doses"),
            Medication(getNextMedicationId(), "Lisinopril", "ACE inhibitor", "10mg, once daily", "Cough, dizziness, headache"),
            Medication(getNextMedicationId(), "Metformin", "Diabetes medication", "500mg, twice daily", "Diarrhea, nausea, stomach upset")
        };
        
        for (const auto& medication : sampleMedications) {
            addMedication(medication);
        }
        
        // Create sample patients
        vector<Patient> samplePatients = {
            Patient(getNextPatientId(), "John", "Doe", {1980, 5, 15}, "123 Main St", "555-1234", "john.doe@email.com", MALE, "O+", "Penicillin", "Jane Doe", "555-5678", {2023, 1, 10}),
            Patient(getNextPatientId(), "Jane", "Smith", {1985, 8, 22}, "456 Oak Ave", "555-2345", "jane.smith@email.com", FEMALE, "A+", "None", "John Smith", "555-3456", {2023, 2, 15}),
            Patient(getNextPatientId(), "Robert", "Johnson", {1990, 3, 10}, "789 Pine Rd", "555-4567", "robert.johnson@email.com", MALE, "B+", "Pollen", "Mary Johnson", "555-5678", {2023, 3, 5}),
            Patient(getNextPatientId(), "Emily", "Davis", {1975, 11, 30}, "321 Elm St", "555-6789", "emily.davis@email.com", FEMALE, "AB+", "Latex", "William Davis", "555-7890", {2023, 1, 20}),
            Patient(getNextPatientId(), "Michael", "Wilson", {1988, 7, 12}, "654 Maple Dr", "555-8901", "michael.wilson@email.com", MALE, "O-", "Shellfish", "Sarah Wilson", "555-9012", {2023, 2, 28})
        };
        
        for (const auto& patient : samplePatients) {
            addPatient(patient);
        }
        
        // Create sample doctors
        vector<Doctor> sampleDoctors = {
            Doctor(getNextDoctorId(), "Dr. Alice", "Brown", "General Practice", "MD123456", "555-1111", "alice.brown@hospital.com", {2010, 1, 15}, true),
            Doctor(getNextDoctorId(), "Dr. Charles", "White", "Cardiology", "MD789012", "555-2222", "charles.white@hospital.com", {2008, 3, 10}, true),
            Doctor(getNextDoctorId(), "Dr. Elizabeth", "Green", "Pediatrics", "MD345678", "555-3333", "elizabeth.green@hospital.com", {2012, 6, 20}, true),
            Doctor(getNextDoctorId(), "Dr. James", "Black", "Orthopedics", "MD901234", "555-4444", "james.black@hospital.com", {2015, 9, 5}, true)
        };
        
        for (const auto& doctor : sampleDoctors) {
            addDoctor(doctor);
        }
        
        // Create sample appointments
        vector<Appointment> sampleAppointments = {
            Appointment(getNextAppointmentId(), 1, 1, {2023, 10, 15}, {9, 30}, "Regular checkup", SCHEDULED, "Annual checkup", {2023, 9, 15}),
            Appointment(getNextAppointmentId(), 2, 1, {2023, 10, 20}, {14, 0}, "Follow-up visit", SCHEDULED, "Follow-up on previous treatment", {2023, 9, 20}),
            Appointment(getNextAppointmentId(), 3, 2, {2023, 10, 25}, {10, 15}, "Cardiac evaluation", SCHEDULED, "Initial cardiac consultation", {2023, 9, 25}),
            Appointment(getNextAppointmentId(), 1, 3, {2023, 10, 30}, {11, 0}, "Vaccination", SCHEDULED, "Child's vaccination", {2023, 10, 1}),
            Appointment(getNextAppointmentId(), 4, 1, {2023, 11, 5}, {8, 45}, "Comprehensive physical", SCHEDULED, "Full health check", {2023, 10, 20}),
            Appointment(getNextAppointmentId(), 5, 2, {2023, 11, 10}, {13, 30}, "Developmental assessment", SCHEDULED, "Child development evaluation", {2023, 10, 30})
        };
        
        for (const auto& appointment : sampleAppointments) {
            addAppointment(appointment);
            
            // Add appointment to patient and doctor
            findPatient(appointment.getPatientId())->addAppointment(appointment.getAppointmentId());
            findDoctor(appointment.getDoctorId())->addAppointment(appointment.getAppointmentId());
        }
        
        // Create sample prescriptions
        vector<Prescription> samplePrescriptions = {
            Prescription(getNextPrescriptionId(), 1, 1, {2023, 10, 15}, "Hypertension", false, {0, 0, 0}, "", "Patient presents with elevated blood pressure"),
            Prescription(getNextPrescriptionId(), 2, 2, {2023, 10, 20}, "Arrhythmia", false, {0, 0, 0}, "", "Abnormal heart rhythm detected in ECG"),
            Prescription(getNextPrescriptionId(), 3, 3, {2023, 10, 25}, "Type 2 Diabetes", false, {0, 0, 0}, "", "Patient with high blood sugar levels"),
            Prescription(getNextPrescriptionId(), 4, 1, {2023, 11, 5}, "Upper respiratory infection", true, {2023, 11, 5}, "City Pharmacy", "Symptoms improved with antibiotics"),
            Prescription(getNextPrescriptionId(), 5, 3, {2023, 11, 10}, "Developmental delay", false, {0, 0, 0}, "", "Refer to developmental specialist")
        };
        
        // Add medications to prescriptions
        samplePrescriptions[0].addMedication(4, "Take one tablet daily with food");
        samplePrescriptions[1].addMedication(1, "Take one tablet twice daily as needed for pain");
        samplePrescriptions[2].addMedication(5, "Take one tablet with meals");
        samplePrescriptions[3].addMedication(1, "Take one tablet three times daily for 7 days");
        
        for (const auto& prescription : samplePrescriptions) {
            addPrescription(prescription);
            
            // Add prescription to patient and doctor
            findPatient(prescription.getPatientId())->addPrescription(prescription.getPrescriptionId());
            findDoctor(prescription.getDoctorId())->addPrescription(prescription.getPrescriptionId());
        }
        
        cout << "Sample data generated successfully:" << endl;
        cout << "Medications: " << medications.size() << endl;
        cout << "Patients: " << patients.size() << endl;
        cout << "Doctors: " << doctors.size() << endl;
        cout << "Appointments: " << appointments.size() << endl;
        cout << "Prescriptions: " << prescriptions.size() << endl;
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(HospitalManagementSystem& system, int choice);
void patientMenu(HospitalManagementSystem& system);
void doctorMenu(HospitalManagementSystem& system);
void appointmentMenu(HospitalManagementSystem& system);
void prescriptionMenu(HospitalManagementSystem& system);
void reportMenu(HospitalManagementSystem& system);
void fileMenu(HospitalManagementSystem& system);

int main() {
    hospitalManagementSystem system;
    
    cout << "Hospital Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("hospital_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("hospital_data.txt");
        cout << "Sample data generated and saved." << endl;
    } else {
        cout << "Existing data loaded successfully." << endl;
    }
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleMenuChoice(system, choice);
        
        cout << endl;
        system("pause");
        system("cls");
    } while (choice != 0);
    
    // Save data before exiting
    system.saveToFile("hospital_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "Hospital Management System" << endl;
    cout << "1. Patient Management" << endl;
    cout << "2. Doctor Management" << endl;
    cout << "3. Appointment Management" << endl;
    cout << "4. Prescription Management" << endl;
    cout << "5. Reports" << endl;
    cout << "6. File Operations" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(hospitalManagementSystem& system, int choice) {
    switch (choice) {
        case 1:
            patientMenu(system);
            break;
        case 2:
            doctorMenu(system);
            break;
        case 3:
            appointmentMenu(system);
            break;
        case 4:
            prescriptionMenu(system);
            break;
        case 5:
            reportMenu(system);
            break;
        case 6:
            fileMenu(system);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void patientMenu(hospitalManagementSystem& system) {
    int choice;
    do {
        cout << "Patient Management" << endl;
        cout << "1. Add Patient" << endl;
        cout << "2. Remove Patient" << endl;
        cout << "3. List All Patients" << endl;
        cout << "4. Search Patients by Name" << endl;
        cout << "5. Search Patients by Age Range" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string firstName, lastName, address, phone, email, bloodType, allergies;
                string emergencyContactName, emergencyContactPhone;
                int genderChoice;
                int year, month, day;
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter Date of Birth (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                cout << "Enter Address: ";
                getline(cin, address);
                
                cout << "Enter Phone: ";
                getline(cin, phone);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter Gender (1-Male, 2-Female, 3-Other): ";
                cin >> genderChoice;
                
                cout << "Enter Blood Type: ";
                getline(cin, bloodType);
                
                cout << "Enter Allergies: ";
                getline(cin, allergies);
                
                cout << "Enter Emergency Contact Name: ";
                getline(cin, emergencyContactName);
                
                cout << "Enter Emergency Contact Phone: ";
                getline(cin, emergencyContactPhone);
                
                // Get current date for registration date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date registrationDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                date dateOfBirth = {year, month, day};
                Gender gender = static_cast<Gender>(genderChoice);
                
                Patient patient(system.getNextPatientId(), firstName, lastName, dateOfBirth, 
                                 address, phone, email, gender, bloodType, allergies, 
                                 emergencyContactName, emergencyContactPhone, registrationDate);
                
                if (system.addPatient(patient)) {
                    cout << "Patient added successfully." << endl;
                } else {
                    cout << "Failed to add patient. Maximum number of patients reached." << endl;
                }
                break;
            }
            case 2: {
                int patientId;
                cout << "Enter Patient ID to remove: ";
                cin >> patientId;
                
                if (system.removePatient(patientId)) {
                    cout << "Patient removed successfully." << endl;
                } else {
                    cout << "Failed to remove patient. Patient ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Patient*> patients = system.getAllPatients();
                if (patients.empty()) {
                    cout << "No patients found." << endl;
                } else {
                    cout << "Patients (" << patients.size() << "):" << endl;
                    for (Patient* patient : patients) {
                        cout << "ID: " << patient->getPatientId()
                             << ", Name: " << patient->getFullName()
                             << ", Age: " << patient->getAge()
                             << ", Gender: " << patient->getGenderString() << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter name to search: ";
                getline(cin, name);
                
                vector<Patient*> patients = system.searchPatientsByName(name);
                if (patients.empty()) {
                    cout << "No patients found matching name." << endl;
                } else {
                    cout << "Patients matching '" << name << "' (" << patients.size() << "):" << endl;
                    for (Patient* patient : patients) {
                        cout << "ID: " << patient->getPatientId()
                             << ", Name: " << patient->getFullName()
                             << ", Age: " << patient->getAge() << endl;
                    }
                }
                break;
            }
            case 5: {
                int minAge, maxAge;
                cout << "Enter Minimum Age: ";
                cin >> minAge;
                
                cout << "Enter Maximum Age: ";
                cin >> maxAge;
                
                vector<Patient*> patients = system.getPatientsByAge(minAge, maxAge);
                if (patients.empty()) {
                    cout << "No patients found in this age range." << endl;
                } else {
                    cout << "Patients aged " << minAge << " to " << maxAge << " (" << patients.size() << "):" << endl;
                    for (Patient* patient : patients) {
                        cout << "ID: " << patient->getPatientId()
                             << ", Name: " << patient->getFullName()
                             << ", Age: " << patient->getAge() << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void doctorMenu(hospitalManagementSystem& system) {
    int choice;
    do {
        cout << "Doctor Management" << endl;
        cout << "1. Add Doctor" << endl;
        cout << "2. Remove Doctor" << endl;
        cout << "3. List All Doctors" << endl;
        cout << "4. Search Doctors by Name" << endl;
        cout << "5. Search Doctors by Specialization" << endl;
        cout << "6. List Active Doctors" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string firstName, lastName, specialization, licenseNumber, phone, email;
                int year, month, day;
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter Specialization: ";
                getline(cin, specialization);
                
                cout << "Enter License Number: ";
                getline(cin, licenseNumber);
                
                cout << "Enter Phone: ";
                getline(cin, phone);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter Hire Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                // Get current date for hire date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date hireDate = {year, month, day};
                
                Doctor doctor(system.getNextDoctorId(), firstName, lastName, specialization, 
                                 licenseNumber, phone, email, hireDate, true);
                
                if (system.addDoctor(doctor)) {
                    cout << "Doctor added successfully." << endl;
                } else {
                    cout << "Failed to add doctor. Maximum number of doctors reached." << endl;
                }
                break;
            }
            case 2: {
                int doctorId;
                cout << "Enter Doctor ID to remove: ";
                cin >> doctorId;
                
                if (system.removeDoctor(doctorId)) {
                    cout << "Doctor removed successfully." << endl;
                } else {
                    cout << "Failed to remove doctor. Doctor ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Doctor*> doctors = system.getAllDoctors();
                if (doctors.empty()) {
                    cout << "No doctors found." << endl;
                } else {
                    cout << "Doctors (" << doctors.size() << "):" << endl;
                    for (Doctor* doctor : doctors) {
                        cout << "ID: " << doctor->getDoctorId()
                             << ", Name: " << doctor->getFullName()
                             << ", Specialization: " << doctor->getSpecialization()
                             << ", Status: " << (doctor->getIsActive() ? "Active" : "Inactive") << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter name to search: ";
                getline(cin, name);
                
                vector<Doctor*> doctors = system.searchDoctorsByName(name);
                if (doctors.empty()) {
                    cout << "No doctors found matching name." << endl;
                } else {
                    cout << "Doctors matching '" << name << "' (" << doctors.size() << "):" << endl;
                    for (Doctor* doctor : doctors) {
                        cout << "ID: " << doctor->getDoctorId()
                             << ", Name: " << doctor->getFullName()
                             << ", Specialization: " << doctor->getSpecialization() << endl;
                    }
                }
                break;
            }
            case 5: {
                string specialization;
                cout << "Enter specialization to search: ";
                getline(cin, specialization);
                
                vector<Doctor*> doctors = system.getDoctorsBySpecialization(specialization);
                if (doctors.empty()) {
                    cout << "No doctors found with this specialization." << endl;
                } else {
                    cout << "Doctors with specialization '" << specialization << "' (" << doctors.size() << "):" << endl;
                    for (Doctor* doctor : doctors) {
                        cout << "ID: " << doctor->getDoctorId()
                             << ", Name: " << doctor->getFullName()
                             << ", Status: " << (doctor->getIsActive() ? "Active" : "Inactive") << endl;
                    }
                }
                break;
            }
            case 6: {
                vector<Doctor*> doctors = system.getActiveDoctors();
                if (doctors.empty()) {
                    cout << "No active doctors found." << endl;
                } else {
                    cout << "Active Doctors (" << doctors.size() << "):" << endl;
                    for (Doctor* doctor : doctors) {
                        cout << "ID: " << doctor->getDoctorId()
                             << ", Name: " << doctor->getFullName()
                             << ", Specialization: " << doctor->getSpecialization() << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void appointmentMenu(hospitalManagementSystem& system) {
    int choice;
    do {
        cout << "Appointment Management" << endl;
        cout << "1. Schedule Appointment" << endl;
        cout << "2. Complete Appointment" << endl;
        cout << "3. Cancel Appointment" << endl;
        cout << "4. Mark No Show" << endl;
        cout << "5. List All Appointments" << endl;
        cout << "6. List Appointments by Patient" << endl;
        cout << "7. List Appointments by Doctor" << endl;
        cout << "8. List Appointments by Date" << endl;
        cout << "9. List Appointments by Status" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int patientId, doctorId, year, month, day, hour, minute;
                string reason;
                
                cout << "Enter Patient ID: ";
                cin >> patientId;
                
                cout << "Enter Doctor ID: ";
                cin >> doctorId;
                
                cout << "Enter Appointment Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                cout << "Enter Appointment Time (HH MM): ";
                cin >> hour >> minute;
                
                cout << "Enter Reason: ";
                getline(cin, reason);
                
                date appointmentDate = {year, month, day};
                time appointmentTime = {hour, minute};
                
                if (system.scheduleAppointment(patientId, doctorId, appointmentDate, appointmentTime, reason)) {
                    cout << "Appointment scheduled successfully." << endl;
                } else {
                    cout << "Failed to schedule appointment. Patient or doctor may not exist, or doctor may not be active." << endl;
                }
                break;
            }
            case 2: {
                int appointmentId;
                string notes;
                
                cout << "Enter Appointment ID: ";
                cin >> appointmentId;
                cin.ignore();
                
                cout << "Enter Notes: ";
                getline(cin, notes);
                
                if (system.completeAppointment(appointmentId, notes)) {
                    cout << "Appointment completed successfully." << endl;
                } else {
                    cout << "Failed to complete appointment. Appointment ID may not exist or appointment is not scheduled." << endl;
                }
                break;
            }
            case 3: {
                int appointmentId;
                string reason;
                
                cout << "Enter Appointment ID: ";
                cin >> appointmentId;
                cin.ignore();
                
                cout << "Enter Cancellation Reason: ";
                getline(cin, reason);
                
                if (system.cancelAppointment(appointmentId, reason)) {
                    cout << "Appointment cancelled successfully." << endl;
                } else {
                    cout << "Failed to cancel appointment. Appointment ID may not exist or appointment is not scheduled." << endl;
                }
                break;
            }
            case 4: {
                int appointmentId;
                string notes;
                
                cout << "Enter Appointment ID: ";
                cin >> appointmentId;
                cin.ignore();
                
                cout << "Enter Notes: ";
                getline(cin, notes);
                
                if (system.markNoShow(appointmentId, notes)) {
                    cout << "Appointment marked as no show successfully." << endl;
                } else {
                    cout << "Failed to mark appointment as no show. Appointment ID may not exist or appointment is not scheduled." << endl;
                }
                break;
            }
            case 5: {
                vector<Appointment*> appointments = system.getAllAppointments();
                if (appointments.empty()) {
                    cout << "No appointments found." << endl;
                } else {
                    cout << "Appointments (" << appointments.size() << "):" << endl;
                    for (Appointment* appointment : appointments) {
                        cout << "ID: " << appointment->getAppointmentId()
                             << ", Patient ID: " << appointment->getPatientId()
                             << ", Doctor ID: " << appointment->getDoctorId()
                             << ", Date: " << appointment->getAppointmentDate().year 
                             << "-" << appointment->getAppointmentDate().month 
                             << "-" << appointment->getAppointmentDate().day
                             << ", Status: " << appointment->getStatusString() << endl;
                    }
                }
                break;
            }
            case 6: {
                int patientId;
                cout << "Enter Patient ID: ";
                cin >> patientId;
                
                vector<Appointment*> appointments = system.getAppointmentsByPatient(patientId);
                if (appointments.empty()) {
                    cout << "No appointments found for this patient." << endl;
                } else {
                    cout << "Appointments for Patient " << patientId << " (" << appointments.size() << "):" << endl;
                    for (Appointment* appointment : appointments) {
                        cout << "ID: " << appointment->getAppointmentId()
                             << ", Doctor ID: " << appointment->getDoctorId()
                             << ", Date: " << appointment->getAppointmentDate().year 
                             << "-" << appointment->getAppointmentDate().month 
                             << "-" << appointment->getAppointmentDate().day
                             << ", Status: " << appointment->getStatusString() << endl;
                    }
                }
                break;
            }
            case 7: {
                int doctorId;
                cout << "Enter Doctor ID: ";
                cin >> doctorId;
                
                vector<Appointment*> appointments = system.getAppointmentsByDoctor(doctorId);
                if (appointments.empty()) {
                    cout << "No appointments found for this doctor." << endl;
                } else {
                    cout << "Appointments for Doctor " << doctorId << " (" << appointments.size() << "):" << endl;
                    for (Appointment* appointment : appointments) {
                        cout << "ID: " << appointment->getAppointmentId()
                             << ", Patient ID: " << appointment->getPatientId()
                             << ", Date: " << appointment->getAppointmentDate().year 
                             << "-" << appointment->getAppointmentDate().month 
                             << "-" << appointment->getAppointmentDate().day
                             << ", Status: " << appointment->getStatusString() << endl;
                    }
                }
                break;
            }
            case 8: {
                int year, month, day;
                cout << "Enter Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                vector<Appointment*> appointments = system.getAppointmentsByDate(year, month, day);
                if (appointments.empty()) {
                    cout << "No appointments found for this date." << endl;
                } else {
                    cout << "Appointments for " << year << "-" << month << "-" << day << " (" << appointments.size() << "):" << endl;
                    for (Appointment* appointment : appointments) {
                        cout << "ID: " << appointment->getAppointmentId()
                             << ", Patient ID: " << appointment->getPatientId()
                             << ", Doctor ID: " << appointment->getDoctorId()
                             << ", Time: " << setfill('0') << setw(2) << appointment->getAppointmentTime().hour 
                             << ":" << setw(2) << appointment->getAppointmentTime().minute
                             << ", Status: " << appointment->getStatusString() << endl;
                    }
                }
                break;
            }
            case 9: {
                int statusChoice;
                cout << "Select Status:" << endl;
                cout << "1. Scheduled" << endl;
                cout << "2. Completed" << endl;
                cout << "3. Cancelled" << endl;
                cout << "4. No Show" << endl;
                cout << "Enter choice: ";
                cin >> statusChoice;
                
                AppointmentStatus status = static_cast<AppointmentStatus>(statusChoice);
                vector<Appointment*> appointments = system.getAppointmentsByStatus(status);
                
                string statusStr;
                switch (status) {
                    case SCHEDULED: statusStr = "Scheduled"; break;
                    case COMPLETED: statusStr = "Completed"; break;
                    case CANCELLED: statusStr = "Cancelled"; break;
                    case NO_SHOW: statusStr = "No Show"; break;
                }
                
                if (appointments.empty()) {
                    cout << "No " << statusStr << " appointments found." << endl;
                } else {
                    cout << statusStr << " Appointments (" << appointments.size() << "):" << endl;
                    for (Appointment* appointment : appointments) {
                        cout << "ID: " << appointment->getAppointmentId()
                             << ", Patient ID: " << appointment->getPatientId()
                             << ", Doctor ID: " << appointment->getDoctorId()
                             << ", Date: " << appointment->getAppointmentDate().year 
                             << "-" << appointment->getAppointmentDate().month 
                             << "-" << appointment->getAppointmentDate().day << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void prescriptionMenu(hospitalManagementSystem& system) {
    int choice;
    do {
        cout << "Prescription Management" << endl;
        cout << "1. Create Prescription" << endl;
        cout << "2. Fill Prescription" << endl;
        cout << "3. List All Prescriptions" << endl;
        cout << "4. List Prescriptions by Patient" << endl;
        cout << "5. List Prescriptions by Doctor" << endl;
        cout << "6. List Filled Prescriptions" << endl;
        cout << "7. List Unfilled Prescriptions" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int patientId, doctorId, year, month, day, medicationCount;
                string diagnosis, notes;
                
                cout << "Enter Patient ID: ";
                cin >> patientId;
                
                cout << "Enter Doctor ID: ";
                cin >> doctorId;
                
                cout << "Enter Prescription Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                cout << "Enter Diagnosis: ";
                getline(cin, diagnosis);
                
                cout << "Enter Number of Medications: ";
                cin >> medicationCount;
                
                vector<pair<int, string>> medications;
                for (int i = 0; i < medicationCount; i++) {
                    int medicationId;
                    string dosageInstructions;
                    
                    cout << "Enter Medication ID " << (i + 1) << ": ";
                    cin >> medicationId;
                    cin.ignore();
                    
                    cout << "Enter Dosage Instructions: ";
                    getline(cin, dosageInstructions);
                    
                    medications.push_back({medicationId, dosageInstructions});
                }
                
                cout << "Enter Notes: ";
                getline(cin, notes);
                
                date prescriptionDate = {year, month, day};
                
                if (system.createPrescription(patientId, doctorId, diagnosis, medications, notes)) {
                    cout << "Prescription created successfully." << endl;
                } else {
                    cout << "Failed to create prescription. Patient or doctor may not exist." << endl;
                }
                break;
            }
            case 2: {
                int prescriptionId;
                string pharmacyName;
                
                cout << "Enter Prescription ID: ";
                cin >> prescriptionId;
                cin.ignore();
                
                cout << "Enter Pharmacy Name: ";
                getline(cin, pharmacyName);
                
                if (system.fillPrescription(prescriptionId, pharmacyName)) {
                    cout << "Prescription filled successfully." << endl;
                } else {
                    cout << "Failed to fill prescription. Prescription ID may not exist or prescription is already filled." << endl;
                }
                break;
            }
            case 3: {
                vector<Prescription*> prescriptions = system.getAllPrescriptions();
                if (prescriptions.empty()) {
                    cout << "No prescriptions found." << endl;
                } else {
                    cout << "Prescriptions (" << prescriptions.size() << "):" << endl;
                    for (Prescription* prescription : prescriptions) {
                        cout << "ID: " << prescription->getPrescriptionId()
                             << ", Patient ID: " << prescription->getPatientId()
                             << ", Doctor ID: " << prescription->getDoctorId()
                             << ", Date: " << prescription->getPrescriptionDate().year 
                             << "-" << prescription->getPrescriptionDate().month 
                             << "-" << prescription->getPrescriptionDate().day
                             << ", Filled: " << (prescription->getIsFilled() ? "Yes" : "No") << endl;
                    }
                }
                break;
            }
            case 4: {
                int patientId;
                cout << "Enter Patient ID: ";
                cin >> patientId;
                
                vector<Prescription*> prescriptions = system.getPrescriptionsByPatient(patientId);
                if (prescriptions.empty()) {
                    cout << "No prescriptions found for this patient." << endl;
                } else {
                    cout << "Prescriptions for Patient " << patientId << " (" << prescriptions.size() << "):" << endl;
                    for (Prescription* prescription : prescriptions) {
                        cout << "ID: " << prescription->getPrescriptionId()
                             << ", Doctor ID: " << prescription->getDoctorId()
                             << ", Date: " << prescription->getPrescriptionDate().year 
                             << "-" << prescription->getPrescriptionDate().month 
                             << "-" << prescription->getPrescriptionDate().day
                             << ", Diagnosis: " << prescription->getDiagnosis()
                             << ", Filled: " << (prescription->getIsFilled() ? "Yes" : "No") << endl;
                    }
                }
                break;
            }
            case 5: {
                int doctorId;
                cout << "Enter Doctor ID: ";
                cin >> doctorId;
                
                vector<Prescription*> prescriptions = system.getPrescriptionsByDoctor(doctorId);
                if (prescriptions.empty()) {
                    cout << "No prescriptions found for this doctor." << endl;
                } else {
                    cout << "Prescriptions for Doctor " << doctorId << " (" << prescriptions.size() << "):" << endl;
                    for (Prescription* prescription : prescriptions) {
                        cout << "ID: " << prescription->getPrescriptionId()
                             << ", Patient ID: " << prescription->getPatientId()
                             << ", Date: " << prescription->getPrescriptionDate().year 
                             << "-" << prescription->getPrescriptionDate().month 
                             << "-" << prescription->getPrescriptionDate().day
                             << ", Diagnosis: " << prescription->getDiagnosis()
                             << ", Filled: " << (prescription->getIsFilled() ? "Yes" : "No") << endl;
                    }
                }
                break;
            }
            case 6: {
                vector<Prescription*> prescriptions = system.getFilledPrescriptions();
                if (prescriptions.empty()) {
                    cout << "No filled prescriptions found." << endl;
                } else {
                    cout << "Filled Prescriptions (" << prescriptions.size() << "):" << endl;
                    for (Prescription* prescription : prescriptions) {
                        cout << "ID: " << prescription->getPrescriptionId()
                             << ", Patient ID: " << prescription->getPatientId()
                             << ", Doctor ID: " << prescription->getDoctorId()
                             << ", Date: " << prescription->getPrescriptionDate().year 
                             << "-" << prescription->getPrescriptionDate().month 
                             << "-" << prescription->getPrescriptionDate().day
                             << ", Pharmacy: " << prescription->getPharmacyName() << endl;
                    }
                }
                break;
            }
            case 7: {
                vector<Prescription*> prescriptions = system.getUnfilledPrescriptions();
                if (prescriptions.empty()) {
                    cout << "No unfilled prescriptions found." << endl;
                } else {
                    cout << "Unfilled Prescriptions (" << prescriptions.size() << "):" << endl;
                    for (Prescription* prescription : prescriptions) {
                        cout << "ID: " << prescription->getPrescriptionId()
                             << ", Patient ID: " << prescription->getPatientId()
                             << ", Doctor ID: " << prescription->getDoctorId()
                             << ", Date: " << prescription->getPrescriptionDate().year 
                             << "-" << prescription->getPrescriptionDate().month 
                             << "-" << prescription->getPrescriptionDate().day
                             << ", Diagnosis: " << prescription->getDiagnosis() << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void reportMenu(hospitalManagementSystem& system) {
    int choice;
    do {
        cout << "Reports" << endl;
        cout << "1. Generate Patient Report" << endl;
        cout << "2. Generate Doctor Report" << endl;
        cout << "3. Generate Appointment Report" << endl;
        cout << "4. Generate Prescription Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int patientId;
                cout << "Enter Patient ID: ";
                cin >> patientId;
                
                system.generatePatientReport(patientId);
                break;
            }
            case 2: {
                int doctorId;
                cout << "Enter Doctor ID: ";
                cin >> doctorId;
                
                system.generateDoctorReport(doctorId);
                break;
            }
            case 3: {
                int year, month;
                cout << "Enter Year: ";
                cin >> year;
                
                cout << "Enter Month: ";
                cin >> month;
                
                system.generateAppointmentReport(year, month);
                break;
            }
            case 4: {
                system.generatePrescriptionReport();
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void fileMenu(hospitalManagementSystem& system) {
    int choice;
    do {
        cout << "File Operations" << endl;
        cout << "1. Save Data to File" << endl;
        cout << "2. Load Data from File" << endl;
        cout << "3. Generate Sample Data" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter filename to save: ";
                getline(cin, filename);
                
                if (filename.empty()) {
                    filename = "hospital_data.txt";
                }
                
                if (system.saveToFile(filename)) {
                    cout << "Data saved successfully to " << filename << endl;
                } else {
                    cout << "Failed to save data." << endl;
                }
                break;
            }
            case 2: {
                string filename;
                cout << "Enter filename to load: ";
                getline(cin, filename);
                
                if (filename.empty()) {
                    filename = "hospital_data.txt";
                }
                
                if (system.loadFromFile(filename)) {
                    cout << "Data loaded successfully from " << filename << endl;
                } else {
                    cout << "Failed to load data." << endl;
                }
                break;
            }
            case 3: {
                system.generateSampleData();
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}
<｜fim▁hole｜>