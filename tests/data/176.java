/**
 * Hospital Management System
 * 
 * This class implements a comprehensive hospital management system
 * that can handle patients, doctors, appointments, medical records,
 * and billing information.
 * 
 * Features:
 * - Patient registration and management
 * - Doctor and staff management
 * - Appointment scheduling
 * - Medical record management
 * - Room and equipment management
 * - Billing and insurance processing
 * - Reporting and analytics
 */

import java.util.*;
import java.text.*;
import java.io.*;

/**
 * Patient class represents a hospital patient
 */
class Patient {
    private String patientId;
    private String firstName;
    private String lastName;
    private Date birthDate;
    private String gender;
    private String address;
    private String phone;
    private String email;
    private String bloodType;
    private String allergies;
    private String medicalHistory;
    private Date registrationDate;
    private String emergencyContact;
    private String insuranceProvider;
    private String insuranceNumber;
    private List<String> medications;
    private List<String> diagnoses;
    private List<String> treatments;
    
    /**
     * Constructor for Patient
     * @param patientId Patient ID
     * @param firstName First name
     * @param lastName Last name
     * @param birthDate Birth date
     * @param gender Gender
     * @param address Address
     * @param phone Phone number
     * @param email Email address
     * @param bloodType Blood type
     * @param allergies Allergies
     * @param medicalHistory Medical history
     * @param emergencyContact Emergency contact
     * @param insuranceProvider Insurance provider
     * @param insuranceNumber Insurance number
     */
    public Patient(String patientId, String firstName, String lastName, Date birthDate,
                  String gender, String address, String phone, String email,
                  String bloodType, String allergies, String medicalHistory,
                  String emergencyContact, String insuranceProvider, String insuranceNumber) {
        this.patientId = patientId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.birthDate = birthDate;
        this.gender = gender;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.bloodType = bloodType;
        this.allergies = allergies;
        this.medicalHistory = medicalHistory;
        this.registrationDate = new Date();
        this.emergencyContact = emergencyContact;
        this.insuranceProvider = insuranceProvider;
        this.insuranceNumber = insuranceNumber;
        this.medications = new ArrayList<>();
        this.diagnoses = new ArrayList<>();
        this.treatments = new ArrayList<>();
    }
    
    /**
     * Get patient ID
     * @return Patient ID
     */
    public String getPatientId() {
        return patientId;
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
     * Get birth date
     * @return Birth date
     */
    public Date getBirthDate() {
        return birthDate;
    }
    
    /**
     * Get age
     * @return Age in years
     */
    public int getAge() {
        Calendar now = Calendar.getInstance();
        Calendar birth = Calendar.getInstance();
        now.setTime(new Date());
        birth.setTime(birthDate);
        
        int age = now.get(Calendar.YEAR) - birth.get(Calendar.YEAR);
        
        // Adjust if birthday hasn't occurred this year
        if (now.get(Calendar.DAY_OF_YEAR) < birth.get(Calendar.DAY_OF_YEAR)) {
            age--;
        }
        
        return age;
    }
    
    /**
     * Get gender
     * @return Gender
     */
    public String getGender() {
        return gender;
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
     * Get blood type
     * @return Blood type
     */
    public String getBloodType() {
        return bloodType;
    }
    
    /**
     * Get allergies
     * @return Allergies
     */
    public String getAllergies() {
        return allergies;
    }
    
    /**
     * Get medical history
     * @return Medical history
     */
    public String getMedicalHistory() {
        return medicalHistory;
    }
    
    /**
     * Get registration date
     * @return Registration date
     */
    public Date getRegistrationDate() {
        return registrationDate;
    }
    
    /**
     * Get emergency contact
     * @return Emergency contact
     */
    public String getEmergencyContact() {
        return emergencyContact;
    }
    
    /**
     * Get insurance provider
     * @return Insurance provider
     */
    public String getInsuranceProvider() {
        return insuranceProvider;
    }
    
    /**
     * Get insurance number
     * @return Insurance number
     */
    public String getInsuranceNumber() {
        return insuranceNumber;
    }
    
    /**
     * Get medications
     * @return List of medications
     */
    public List<String> getMedications() {
        return medications;
    }
    
    /**
     * Get diagnoses
     * @return List of diagnoses
     */
    public List<String> getDiagnoses() {
        return diagnoses;
    }
    
    /**
     * Get treatments
     * @return List of treatments
     */
    public List<String> getTreatments() {
        return treatments;
    }
    
    /**
     * Add medication
     * @param medication Medication to add
     * @return True if added successfully, false otherwise
     */
    public boolean addMedication(String medication) {
        if (medication == null || medication.trim().isEmpty()) {
            return false;
        }
        
        medications.add(medication);
        return true;
    }
    
    /**
     * Add diagnosis
     * @param diagnosis Diagnosis to add
     * @return True if added successfully, false otherwise
     */
    public boolean addDiagnosis(String diagnosis) {
        if (diagnosis == null || diagnosis.trim().isEmpty()) {
            return false;
        }
        
        diagnoses.add(diagnosis);
        return true;
    }
    
    /**
     * Add treatment
     * @param treatment Treatment to add
     * @return True if added successfully, false otherwise
     */
    public boolean addTreatment(String treatment) {
        if (treatment == null || treatment.trim().isEmpty()) {
            return false;
        }
        
        treatments.add(treatment);
        return true;
    }
    
    /**
     * Update address
     * @param address New address
     * @return True if updated successfully, false otherwise
     */
    public boolean updateAddress(String address) {
        if (address == null || address.trim().isEmpty()) {
            return false;
        }
        
        this.address = address;
        return true;
    }
    
    /**
     * Update phone number
     * @param phone New phone number
     * @return True if updated successfully, false otherwise
     */
    public boolean updatePhone(String phone) {
        if (phone == null || phone.trim().isEmpty()) {
            return false;
        }
        
        this.phone = phone;
        return true;
    }
    
    /**
     * Update email address
     * @param email New email address
     * @return True if updated successfully, false otherwise
     */
    public boolean updateEmail(String email) {
        if (email == null || email.trim().isEmpty()) {
            return false;
        }
        
        this.email = email;
        return true;
    }
    
    /**
     * Update insurance information
     * @param provider Insurance provider
     * @param number Insurance number
     * @return True if updated successfully, false otherwise
     */
    public boolean updateInsurance(String provider, String number) {
        if (provider == null || provider.trim().isEmpty() || 
            number == null || number.trim().isEmpty()) {
            return false;
        }
        
        this.insuranceProvider = provider;
        this.insuranceNumber = number;
        return true;
    }
    
    /**
     * Get patient summary
     * @return Patient summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Patient ID: ").append(patientId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Age: ").append(getAge()).append(" years\n");
        sb.append("Gender: ").append(gender).append("\n");
        sb.append("Blood Type: ").append(bloodType).append("\n");
        sb.append("Allergies: ").append(allergies).append("\n");
        sb.append("Medical History: ").append(medicalHistory).append("\n");
        sb.append("Registration Date: ").append(registrationDate).append("\n");
        sb.append("Address: ").append(address).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Emergency Contact: ").append(emergencyContact).append("\n");
        sb.append("Insurance Provider: ").append(insuranceProvider).append("\n");
        sb.append("Insurance Number: ").append(insuranceNumber).append("\n");
        sb.append("Medications: ").append(medications.size()).append("\n");
        sb.append("Diagnoses: ").append(diagnoses.size()).append("\n");
        sb.append("Treatments: ").append(treatments.size()).append("\n");
        
        return sb.toString();
    }
}

/**
 * Doctor class represents a hospital doctor
 */
class Doctor {
    private String doctorId;
    private String firstName;
    private String lastName;
    private String specialization;
    private String department;
    private Date hireDate;
    private String phone;
    private String email;
    private String address;
    private String licenseNumber;
    private String education;
    private int yearsOfExperience;
    private boolean active;
    private List<String> availableDays;
    private double consultationFee;
    private List<String> qualifications;
    private List<String> certifications;
    private List<String> publications;
    
    /**
     * Constructor for Doctor
     * @param doctorId Doctor ID
     * @param firstName First name
     * @param lastName Last name
     * @param specialization Medical specialization
     * @param department Department
     * @param hireDate Hire date
     * @param phone Phone number
     * @param email Email address
     * @param address Address
     * @param licenseNumber License number
     * @param education Education background
     * @param yearsOfExperience Years of experience
     * @param active Active status
     */
    public Doctor(String doctorId, String firstName, String lastName, String specialization,
                String department, Date hireDate, String phone, String email,
                String address, String licenseNumber, String education,
                int yearsOfExperience, boolean active) {
        this.doctorId = doctorId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.specialization = specialization;
        this.department = department;
        this.hireDate = hireDate;
        this.phone = phone;
        this.email = email;
        this.address = address;
        this.licenseNumber = licenseNumber;
        this.education = education;
        this.yearsOfExperience = yearsOfExperience;
        this.active = active;
        this.availableDays = Arrays.asList("Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday");
        this.consultationFee = 100.0;
        this.qualifications = new ArrayList<>();
        this.certifications = new ArrayList<>();
        this.publications = new ArrayList<>();
    }
    
    /**
     * Get doctor ID
     * @return Doctor ID
     */
    public String getDoctorId() {
        return doctorId;
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
        return "Dr. " + firstName + " " + lastName;
    }
    
    /**
     * Get specialization
     * @return Medical specialization
     */
    public String getSpecialization() {
        return specialization;
    }
    
    /**
     * Get department
     * @return Department
     */
    public String getDepartment() {
        return department;
    }
    
    /**
     * Get hire date
     * @return Hire date
     */
    public Date getHireDate() {
        return hireDate;
    }
    
    /**
     * Get years of experience
     * @return Years of experience
     */
    public int getYearsOfExperience() {
        return yearsOfExperience;
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
     * Get address
     * @return Address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get license number
     * @return License number
     */
    public String getLicenseNumber() {
        return licenseNumber;
    }
    
    /**
     * Get education
     * @return Education background
     */
    public String getEducation() {
        return education;
    }
    
    /**
     * Get available days
     * @return List of available days
     */
    public List<String> getAvailableDays() {
        return availableDays;
    }
    
    /**
     * Get consultation fee
     * @return Consultation fee
     */
    public double getConsultationFee() {
        return consultationFee;
    }
    
    /**
     * Get qualifications
     * @return List of qualifications
     */
    public List<String> getQualifications() {
        return qualifications;
    }
    
    /**
     * Get certifications
     * @return List of certifications
     */
    public List<String> getCertifications() {
        return certifications;
    }
    
    /**
     * Get publications
     * @return List of publications
     */
    public List<String> getPublications() {
        return publications;
    }
    
    /**
     * Check if doctor is active
     * @return True if active, false otherwise
     */
    public boolean isActive() {
        return active;
    }
    
    /**
     * Add qualification
     * @param qualification Qualification to add
     * @return True if added successfully, false otherwise
     */
    public boolean addQualification(String qualification) {
        if (qualification == null || qualification.trim().isEmpty()) {
            return false;
        }
        
        qualifications.add(qualification);
        return true;
    }
    
    /**
     * Add certification
     * @param certification Certification to add
     * @return True if added successfully, false otherwise
     */
    public boolean addCertification(String certification) {
        if (certification == null || certification.trim().isEmpty()) {
            return false;
        }
        
        certifications.add(certification);
        return true;
    }
    
    /**
     * Add publication
     * @param publication Publication to add
     * @return True if added successfully, false otherwise
     */
    public boolean addPublication(String publication) {
        if (publication == null || publication.trim().isEmpty()) {
            return false;
        }
        
        publications.add(publication);
        return true;
    }
    
    /**
     * Update consultation fee
     * @param fee New consultation fee
     * @return True if updated successfully, false otherwise
     */
    public boolean updateConsultationFee(double fee) {
        if (fee < 0) {
            return false;
        }
        
        this.consultationFee = fee;
        return true;
    }
    
    /**
     * Set active status
     * @param active Active status
     */
    public void setActive(boolean active) {
        this.active = active;
    }
    
    /**
     * Get doctor summary
     * @return Doctor summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Doctor ID: ").append(doctorId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Specialization: ").append(specialization).append("\n");
        sb.append("Department: ").append(department).append("\n");
        sb.append("Years of Experience: ").append(yearsOfExperience).append("\n");
        sb.append("Hire Date: ").append(hireDate).append("\n");
        sb.append("License Number: ").append(licenseNumber).append("\n");
        sb.append("Education: ").append(education).append("\n");
        sb.append("Available Days: ").append(String.join(", ", availableDays)).append("\n");
        sb.append("Consultation Fee: $").append(String.format("%.2f", consultationFee)).append("\n");
        sb.append("Qualifications: ").append(qualifications.size()).append("\n");
        sb.append("Certifications: ").append(certifications.size()).append("\n");
        sb.append("Publications: ").append(publications.size()).append("\n");
        sb.append("Active: ").append(active ? "Yes" : "No").append("\n");
        
        return sb.toString();
    }
}

/**
 * Appointment class represents a hospital appointment
 */
class Appointment {
    private String appointmentId;
    private String patientId;
    private String doctorId;
    private Date appointmentDate;
    private Date appointmentTime;
    private String department;
    private String type;  // CONSULTATION, FOLLOW_UP, SURGERY, etc.
    private String status;  // SCHEDULED, COMPLETED, CANCELLED, RESCHEDULED
    private String notes;
    private String roomNumber;
    private double consultationFee;
    
    /**
     * Constructor for Appointment
     * @param appointmentId Appointment ID
     * @param patientId Patient ID
     * @param doctorId Doctor ID
     * @param appointmentDate Appointment date
     * @param appointmentTime Appointment time
     * @param department Department
     * @param type Appointment type
     * @param status Appointment status
     * @param notes Appointment notes
     * @param roomNumber Room number
     * @param consultationFee Consultation fee
     */
    public Appointment(String appointmentId, String patientId, String doctorId, Date appointmentDate,
                       Date appointmentTime, String department, String type,
                       String status, String notes, String roomNumber,
                       double consultationFee) {
        this.appointmentId = appointmentId;
        this.patientId = patientId;
        this.doctorId = doctorId;
        this.appointmentDate = appointmentDate;
        this.appointmentTime = appointmentTime;
        this.department = department;
        this.type = type;
        this.status = status;
        this.notes = notes;
        this.roomNumber = roomNumber;
        this.consultationFee = consultationFee;
    }
    
    /**
     * Get appointment ID
     * @return Appointment ID
     */
    public String getAppointmentId() {
        return appointmentId;
    }
    
    /**
     * Get patient ID
     * @return Patient ID
     */
    public String getPatientId() {
        return patientId;
    }
    
    /**
     * Get doctor ID
     * @return Doctor ID
     */
    public String getDoctorId() {
        return doctorId;
    }
    
    /**
     * Get appointment date
     * @return Appointment date
     */
    public Date getAppointmentDate() {
        return appointmentDate;
    }
    
    /**
     * Get appointment time
     * @return Appointment time
     */
    public Date getAppointmentTime() {
        return appointmentTime;
    }
    
    /**
     * Get department
     * @return Department
     */
    public String getDepartment() {
        return department;
    }
    
    /**
     * Get appointment type
     * @return Appointment type
     */
    public String getType() {
        return type;
    }
    
    /**
     * Get status
     * @return Appointment status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get notes
     * @return Appointment notes
     */
    public String getNotes() {
        return notes;
    }
    
    /**
     * Get room number
     * @return Room number
     */
    public String getRoomNumber() {
        return roomNumber;
    }
    
    /**
     * Get consultation fee
     * @return Consultation fee
     */
    public double getConsultationFee() {
        return consultationFee;
    }
    
    /**
     * Update status
     * @param status New status
     * @return True if updated successfully, false otherwise
     */
    public boolean updateStatus(String status) {
        List<String> validStatuses = Arrays.asList("SCHEDULED", "COMPLETED", "CANCELLED", "RESCHEDULED");
        
        if (validStatuses.contains(status)) {
            this.status = status;
            return true;
        }
        
        return false;
    }
    
    /**
     * Update notes
     * @param notes New notes
     * @return True if updated successfully, false otherwise
     */
    public boolean updateNotes(String notes) {
        if (notes == null) {
            this.notes = "";
            return true;
        }
        
        this.notes = notes;
        return true;
    }
    
    /**
     * Reschedule appointment
     * @param newDate New date
     * @param newTime New time
     * @return True if rescheduled successfully, false otherwise
     */
    public boolean reschedule(Date newDate, Date newTime) {
        this.appointmentDate = newDate;
        this.appointmentTime = newTime;
        this.status = "RESCHEDULED";
        
        // Add notes about rescheduling
        String originalDateTime = this.appointmentDate + " " + this.appointmentTime;
        this.notes = (this.notes == null ? "" : this.notes) + "\n\nRescheduled from: " + originalDateTime +
                           " to: " + newDate + " " + newTime;
        
        return true;
    }
    
    /**
     * Cancel appointment
     * @return True if cancelled successfully, false otherwise
     */
    public boolean cancel() {
        this.status = "CANCELLED";
        
        // Add notes about cancellation
        this.notes = (this.notes == null ? "" : this.notes) + "\n\nCancelled on: " + new Date();
        
        return true;
    }
    
    /**
     * Complete appointment
     * @return True if completed successfully, false otherwise
     */
    public boolean complete() {
        this.status = "COMPLETED";
        this.notes = (this.notes == null ? "" : this.notes) + "\n\nCompleted on: " + new Date();
        
        return true;
    }
    
    /**
     * Get appointment summary
     * @return Appointment summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Appointment ID: ").append(appointmentId).append("\n");
        sb.append("Patient ID: ").append(patientId).append("\n");
        sb.append("Doctor ID: ").append(doctorId).append("\n");
        sb.append("Date: ").append(appointmentDate).append("\n");
        sb.append("Time: ").append(appointmentTime).append("\n");
        sb.append("Department: ").append(department).append("\n");
        sb.append("Type: ").append(type).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Room Number: ").append(roomNumber).append("\n");
        sb.append("Consultation Fee: $").append(String.format("%.2f", consultationFee)).append("\n");
        sb.append("Notes: ").append(notes).append("\n");
        
        return sb.toString();
    }
}

/**
 * MedicalRecord class represents a patient's medical record
 */
class MedicalRecord {
    private String recordId;
    private String patientId;
    private String doctorId;
    private Date recordDate;
    private String diagnosis;
    private String prescription;
    private String treatment;
    private String followUpNotes;
    private Date followUpDate;
    private double cost;
    private String insuranceClaimStatus;  // PENDING, APPROVED, DENIED, PAID
    
    /**
     * Constructor for MedicalRecord
     * @param recordId Medical record ID
     * @param patientId Patient ID
     * @param doctorId Doctor ID
     * @param recordDate Record date
     * @param diagnosis Diagnosis
     * @param prescription Prescription
     * @param treatment Treatment
     * @param followUpNotes Follow-up notes
     * @param followUpDate Follow-up date
     * @param cost Treatment cost
     * @param insuranceClaimStatus Insurance claim status
     */
    public MedicalRecord(String recordId, String patientId, String doctorId, Date recordDate,
                        String diagnosis, String prescription, String treatment,
                        String followUpNotes, Date followUpDate, double cost,
                        String insuranceClaimStatus) {
        this.recordId = recordId;
        this.patientId = patientId;
        this.doctorId = doctorId;
        this.recordDate = recordDate;
        this.diagnosis = diagnosis;
        this.prescription = prescription;
        this.treatment = treatment;
        this.followUpNotes = followUpNotes;
        this.followUpDate = followUpDate;
        this.cost = cost;
        this.insuranceClaimStatus = insuranceClaimStatus;
    }
    
    /**
     * Get medical record ID
     * @return Medical record ID
     */
    public String getRecordId() {
        return recordId;
    }
    
    /**
     * Get patient ID
     * @return Patient ID
     */
    public String getPatientId() {
        return patientId;
    }
    
    /**
     * Get doctor ID
     * @return Doctor ID
     */
    public String getDoctorId() {
        return doctorId;
    }
    
    /**
     * Get record date
     * @return Record date
     */
    public Date getRecordDate() {
        return recordDate;
    }
    
    /**
     * Get diagnosis
     * @return Diagnosis
     */
    public String getDiagnosis() {
        return diagnosis;
    }
    
    /**
     * Get prescription
     * @return Prescription
     */
    public String getPrescription() {
        return prescription;
    }
    
    /**
     * Get treatment
     * @return Treatment
     */
    public String getTreatment() {
        return treatment;
    }
    
    /**
     * Get follow-up notes
     * @return Follow-up notes
     */
    public String getFollowUpNotes() {
        return followUpNotes;
    }
    
    /**
     * Get follow-up date
     * @return Follow-up date
     */
    public Date getFollowUpDate() {
        return followUpDate;
    }
    
    /**
     * Get cost
     * @return Treatment cost
     */
    public double getCost() {
        return cost;
    }
    
    /**
     * Get insurance claim status
     * @return Insurance claim status
     */
    public String getInsuranceClaimStatus() {
        return insuranceClaimStatus;
    }
    
    /**
     * Update follow-up date
     * @param followUpDate New follow-up date
     * @return True if updated successfully, false otherwise
     */
    public boolean updateFollowUpDate(Date followUpDate) {
        this.followUpDate = followUpDate;
        return true;
    }
    
    /**
     * Update insurance claim status
     * @param status New insurance claim status
     * @return True if updated successfully, false otherwise
     */
    public boolean updateInsuranceClaimStatus(String status) {
        List<String> validStatuses = Arrays.asList("PENDING", "APPROVED", "DENIED", "PAID");
        
        if (validStatuses.contains(status)) {
            this.insuranceClaimStatus = status;
            return true;
        }
        
        return false;
    }
    
    /**
     * Get medical record summary
     * @return Medical record summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Medical Record ID: ").append(recordId).append("\n");
        sb.append("Patient ID: ").append(patientId).append("\n");
        sb.append("Doctor ID: ").append(doctorId).append("\n");
        sb.append("Record Date: ").append(recordDate).append("\n");
        sb.append("Diagnosis: ").append(diagnosis).append("\n");
        sb.append("Prescription: ").append(prescription).append("\n");
        sb.append("Treatment: ").append(treatment).append("\n");
        sb.append("Cost: $").append(String.format("%.2f", cost)).append("\n");
        sb.append("Insurance Claim Status: ").append(insuranceClaimStatus).append("\n");
        sb.append("Follow-up Date: ").append(followUpDate != null ? followUpDate : "N/A").append("\n");
        sb.append("Follow-up Notes: ").append(followUpNotes).append("\n");
        
        return sb.toString();
    }
}

/**
 * HospitalManagementSystem class manages the entire hospital
 */
public class HospitalManagementSystem {
    private String hospitalName;
    private String address;
    private String phone;
    private List<Patient> patients;
    private Map<String, Patient> patientMap;
    private List<Doctor> doctors;
    private Map<String, Doctor> doctorMap;
    private List<Appointment> appointments;
    private Map<String, Appointment> appointmentMap;
    private List<MedicalRecord> medicalRecords;
    private Map<String, MedicalRecord> medicalRecordMap;
    private List<String> departments;
    private List<String> availableRooms;
    private Map<String, Double> departmentBudgets;
    
    /**
     * Constructor for HospitalManagementSystem
     * @param hospitalName Hospital name
     * @param address Hospital address
     * @param phone Hospital phone
     */
    public HospitalManagementSystem(String hospitalName, String address, String phone) {
        this.hospitalName = hospitalName;
        this.address = address;
        this.phone = phone;
        this.patients = new ArrayList<>();
        this.patientMap = new HashMap<>();
        this.doctors = new ArrayList<>();
        this.doctorMap = new HashMap<>();
        this.appointments = new ArrayList<>();
        this.appointmentMap = new HashMap<>();
        this.medicalRecords = new ArrayList<>();
        this.medicalRecordMap = new HashMap<>();
        this.departments = Arrays.asList("Cardiology", "Neurology", "Orthopedics", "Pediatrics", "Psychiatry", "General Medicine", "Surgery", "Emergency", "Radiology", "Pathology", "Dermatology", "Gynecology", "Urology", "Ophthalmology", "ENT");
        this.availableRooms = new ArrayList<>();
        this.departmentBudgets = new HashMap<>();
        
        // Initialize department budgets
        initializeDepartmentBudgets();
        
        // Initialize available rooms
        initializeAvailableRooms();
    }
    
    /**
     * Initialize department budgets
     */
    private void initializeDepartmentBudgets() {
        departmentBudgets.put("Cardiology", 500000.00);
        departmentBudgets.put("Neurology", 600000.00);
        departmentBudgets.put("Orthopedics", 450000.00);
        departmentBudgets.put("Pediatrics", 350000.00);
        departmentBudgets.put("Psychiatry", 300000.00);
        departmentBudgets.put("General Medicine", 400000.00);
        departmentBudgets.put("Surgery", 800000.00);
        departmentBudgets.put("Emergency", 700000.00);
        departmentBudgets.put("Radiology", 550000.00);
        departmentBudgets.put("Pathology", 400000.00);
        departmentBudgets.put("Dermatology", 350000.00);
        departmentBudgets.put("Gynecology", 400000.00);
        departmentBudgets.put("Urology", 450000.00);
        departmentBudgets.put("Ophthalmology", 500000.00);
        departmentBudgets.put("ENT", 400000.00);
    }
    
    /**
     * Initialize available rooms
     */
    private void initializeAvailableRooms() {
        for (int floor = 1; floor <= 3; floor++) {
            for (int room = 1; room <= 10; room++) {
                availableRooms.add("Floor " + floor + "-Room " + room);
            }
        }
    }
    
    /**
     * Get hospital name
     * @return Hospital name
     */
    public String getHospitalName() {
        return hospitalName;
    }
    
    /**
     * Get hospital address
     * @return Hospital address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get hospital phone
     * @return Hospital phone
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get patients
     * @return List of patients
     */
    public List<Patient> getPatients() {
        return patients;
    }
    
    /**
     * Get doctors
     * @return List of doctors
     */
    public List<Doctor> getDoctors() {
        return doctors;
    }
    
    /**
     * Get appointments
     * @return List of appointments
     */
    public List<Appointment> getAppointments() {
        return appointments;
    }
    
    /**
     * Get medical records
     * @return List of medical records
     */
    public List<MedicalRecord> getMedicalRecords() {
        return medicalRecords;
    }
    
    /**
     * Get departments
     * @return List of departments
     */
    public List<String> getDepartments() {
        return departments;
    }
    
    /**
     * Get available rooms
     * @return List of available rooms
     */
    public List<String> getAvailableRooms() {
        return availableRooms;
    }
    
    /**
     * Get department budgets
     * @return Map of department budgets
     */
    public Map<String, Double> getDepartmentBudgets() {
        return departmentBudgets;
    }
    
    /**
     * Add a patient
     * @param patient Patient to add
     * @return True if added successfully, false otherwise
     */
    public boolean addPatient(Patient patient) {
        if (patientMap.containsKey(patient.getPatientId())) {
            return false;
        }
        
        patients.add(patient);
        patientMap.put(patient.getPatientId(), patient);
        return true;
    }
    
    /**
     * Get a patient by ID
     * @param patientId Patient ID
     * @return Patient if found, null otherwise
     */
    public Patient getPatient(String patientId) {
        return patientMap.get(patientId);
    }
    
    /**
     * Add a doctor
     * @param doctor Doctor to add
     * @return True if added successfully, false otherwise
     */
    public boolean addDoctor(Doctor doctor) {
        if (doctorMap.containsKey(doctor.getDoctorId())) {
            return false;
        }
        
        doctors.add(doctor);
        doctorMap.put(doctor.getDoctorId(), doctor);
        return true;
    }
    
    /**
     * Get a doctor by ID
     * @param doctorId Doctor ID
     * @return Doctor if found, null otherwise
     */
    public Doctor getDoctor(String doctorId) {
        return doctorMap.get(doctorId);
    }
    
    /**
     * Schedule an appointment
     * @param appointment Appointment to schedule
     * @return True if scheduled successfully, false otherwise
     */
    public boolean scheduleAppointment(Appointment appointment) {
        if (appointmentMap.containsKey(appointment.getAppointmentId())) {
            return false;
        }
        
        // Check if patient and doctor exist
        if (!patientMap.containsKey(appointment.getPatientId())) {
            return false;
        }
        
        if (!doctorMap.containsKey(appointment.getDoctorId())) {
            return false;
        }
        
        // Check if room is available
        if (!availableRooms.contains(appointment.getRoomNumber())) {
            return false;
        }
        
        appointments.add(appointment);
        appointmentMap.put(appointment.getAppointmentId(), appointment);
        
        // Remove room from available rooms
        availableRooms.remove(appointment.getRoomNumber());
        
        return true;
    }
    
    /**
     * Cancel an appointment
     * @param appointmentId Appointment ID
     * @return True if cancelled successfully, false otherwise
     */
    public boolean cancelAppointment(String appointmentId) {
        Appointment appointment = appointmentMap.get(appointmentId);
        if (appointment == null) {
            return false;
        }
        
        if (!appointment.cancel()) {
            return false;
        }
        
        // Add room back to available rooms
        availableRooms.add(appointment.getRoomNumber());
        
        return true;
    }
    
    /**
     * Complete an appointment
     * @param appointmentId Appointment ID
     * @return True if completed successfully, false otherwise
     */
    public boolean completeAppointment(String appointmentId) {
        Appointment appointment = appointmentMap.get(appointmentId);
        if (appointment == null) {
            return false;
        }
        
        if (!appointment.complete()) {
            return false;
        }
        
        // Add room back to available rooms
        availableRooms.add(appointment.getRoomNumber());
        
        return true;
    }
    
    /**
     * Add medical record
     * @param medicalRecord Medical record to add
     * @return True if added successfully, false otherwise
     */
    public boolean addMedicalRecord(MedicalRecord medicalRecord) {
        if (medicalRecordMap.containsKey(medicalRecord.getRecordId())) {
            return false;
        }
        
        medicalRecords.add(medicalRecord);
        medicalRecordMap.put(medicalRecord.getRecordId(), medicalRecord);
        return true;
    }
    
    /**
     * Get a medical record by ID
     * @param recordId Medical record ID
     * @return Medical record if found, null otherwise
     */
    public MedicalRecord getMedicalRecord(String recordId) {
        return medicalRecordMap.get(recordId);
    }
    
    /**
     * Get available doctors by specialization
     * @param specialization Medical specialization
     * @return List of available doctors
     */
    public List<Doctor> getAvailableDoctorsBySpecialization(String specialization) {
        List<Doctor> availableDoctors = new ArrayList<>();
        
        for (Doctor doctor : doctors) {
            if (doctor.isActive() && doctor.getSpecialization().equals(specialization)) {
                availableDoctors.add(doctor);
            }
        }
        
        return availableDoctors;
    }
    
    /**
     * Get available rooms
     * @param department Department for room check
     * @return List of available rooms
     */
    public List<String> getAvailableRoomsByDepartment(String department) {
        // In a real implementation, you would check which rooms are available
        // for specific departments based on hospital layout
        // This is a simplified implementation
        
        List<String> availableRooms = new ArrayList<>();
        
        for (String room : this.availableRooms) {
            availableRooms.add(room);
        }
        
        return availableRooms;
    }
    
    /**
     * Get total patients
     * @return Total number of patients
     */
    public int getTotalPatients() {
        return patients.size();
    }
    
    /**
     * Get total doctors
     * @return Total number of doctors
     */
    public int getTotalDoctors() {
        return doctors.size();
    }
    
    /**
     * Get total appointments
     * @return Total number of appointments
     */
    public int getTotalAppointments() {
        return appointments.size();
    }
    
    /**
     * Get total medical records
     * @return Total number of medical records
     */
    public int getTotalMedicalRecords() {
        return medicalRecords.size();
    }
    
    /**
     * Get total department budget
     * @return Total department budget
     */
    public double getTotalDepartmentBudget() {
        double totalBudget = 0.0;
        
        for (Double budget : departmentBudgets.values()) {
            totalBudget += budget;
        }
        
        return totalBudget;
    }
    
    /**
     * Generate hospital report
     * @return Hospital report string
     */
    public String generateHospitalReport() {
        StringBuilder sb = new StringBuilder();
        sb.append("=======================================\n");
        sb.append("          HOSPITAL REPORT\n");
        sb.append("=======================================\n");
        sb.append("Hospital Name: ").append(hospitalName).append("\n");
        sb.append("Address: ").append(address).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Report Date: ").append(new Date()).append("\n\n");
        
        // Summary statistics
        sb.append("SUMMARY STATISTICS\n");
        sb.append("--------------------\n");
        sb.append("Total Patients: ").append(getTotalPatients()).append("\n");
        sb.append("Total Doctors: ").append(getTotalDoctors()).append("\n");
        sb.append("Total Appointments: ").append(getTotalAppointments()).append("\n");
        sb.append("Total Medical Records: ").append(getTotalMedicalRecords()).append("\n");
        sb.append("Total Department Budget: $").append(String.format("%.2f", getTotalDepartmentBudget())).append("\n\n");
        
        // Department statistics
        sb.append("DEPARTMENT STATISTICS\n");
        sb.append("-----------------------\n");
        for (String department : departments) {
            int deptDoctors = 0;
            int deptPatients = 0;
            int deptAppointments = 0;
            int deptMedicalRecords = 0;
            
            for (Doctor doctor : doctors) {
                if (doctor.getDepartment().equals(department)) {
                    deptDoctors++;
                }
            }
            
            for (Patient patient : patients) {
                // In a real implementation, you would check department assignments
                // This is a simplified implementation
                deptPatients++;
            }
            
            for (Appointment appointment : appointments) {
                if (appointment.getDepartment().equals(department)) {
                    deptAppointments++;
                }
            }
            
            for (MedicalRecord record : medicalRecords) {
                // In a real implementation, you would check doctor's department
                // This is a simplified implementation
                deptMedicalRecords++;
            }
            
            sb.append(department).append(":\n");
            sb.append("  Doctors: ").append(deptDoctors).append("\n");
            sb.append("  Patients: ").append(deptPatients).append("\n");
            sb.append("  Appointments: ").append(deptAppointments).append("\n");
            sb.append("  Medical Records: ").append(deptMedicalRecords).append("\n");
            sb.append("  Budget: $").append(String.format("%.2f", departmentBudgets.getOrDefault(department, 0.0))).append("\n");
        }
        
        // Top doctors by department
        sb.append("\nTOP DOCTORS BY DEPARTMENT\n");
        sb.append("-------------------------\n");
        for (String department : departments) {
            List<Doctor> deptDoctors = getAvailableDoctorsBySpecialization(department);
            deptDoctors.sort((a, b) -> a.getFullName().compareTo(b.getFullName()));
            
            sb.append(department).append(":\n");
            for (int i = 0; i < Math.min(3, deptDoctors.size()); i++) {
                Doctor doctor = deptDoctors.get(i);
                sb.append("  ").append(i + 1).append(". ").append(doctor.getFullName()).append(" (")
                        .append(doctor.getSpecialization()).append(")\n");
            }
        }
        
        // Available rooms
        sb.append("\nAVAILABLE ROOMS\n");
        sb.append("---------------\n");
        for (String room : availableRooms) {
            sb.append(room).append("\n");
        }
        
        return sb.toString();
    }
    
    /**
     * Save hospital data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("HOSPITAL DATA EXPORT");
            writer.println("==================");
            writer.println("Hospital Name: " + hospitalName);
            writer.println("Address: " + address);
            writer.println("Phone: " + phone);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export patients
            writer.println("PATIENTS");
            writer.println("--------");
            for (Patient patient : patients) {
                writer.println("ID: " + patient.getPatientId());
                writer.println("Name: " + patient.getFullName());
                writer.println("Age: " + patient.getAge() + " years");
                writer.println("Gender: " + patient.getGender());
                writer.println("Blood Type: " + patient.getBloodType());
                writer.println("Allergies: " + patient.getAllergies());
                writer.println("Medical History: " + patient.getMedicalHistory());
                writer.println("Registration Date: " + patient.getRegistrationDate());
                writer.println("Address: " + patient.getAddress());
                writer.println("Phone: " + patient.getPhone());
                writer.println("Email: " + patient.getEmail());
                writer.println("Emergency Contact: " + patient.getEmergencyContact());
                writer.println("Insurance Provider: " + patient.getInsuranceProvider());
                writer.println("Insurance Number: " + patient.getInsuranceNumber());
                writer.println("Medications: " + String.join(", ", patient.getMedications()));
                writer.println("Diagnoses: " + String.join(", ", patient.getDiagnoses()));
                writer.println("Treatments: " + String.join(", ", patient.getTreatments()));
                writer.println();
            }
            
            // Export doctors
            writer.println("DOCTORS");
            writer.println("--------");
            for (Doctor doctor : doctors) {
                writer.println("ID: " + doctor.getDoctorId());
                writer.println("Name: " + doctor.getFullName());
                writer.println("Specialization: " + doctor.getSpecialization());
                writer.println("Department: " + doctor.getDepartment());
                writer.println("Years of Experience: " + doctor.getYearsOfExperience());
                writer.println("License Number: " + doctor.getLicenseNumber());
                writer.println("Education: " + doctor.getEducation());
                writer.println("Consultation Fee: $" + String.format("%.2f", doctor.getConsultationFee()));
                writer.println("Available Days: " + String.join(", ", doctor.getAvailableDays()));
                writer.println("Qualifications: " + String.join(", ", doctor.getQualifications()));
                writer.println("Certifications: " + String.join(", ", doctor.getCertifications()));
                writer.println("Publications: " + String.join(", ", doctor.getPublications()));
                writer.println("Active: " + (doctor.isActive() ? "Yes" : "No"));
                writer.println();
            }
            
            // Export appointments
            writer.println("APPOINTMENTS");
            writer.println("-----------");
            for (Appointment appointment : appointments) {
                writer.println("ID: " + appointment.getAppointmentId());
                writer.println("Patient ID: " + appointment.getPatientId());
                writer.println("Doctor ID: " + appointment.getDoctorId());
                writer.println("Date: " + appointment.getAppointmentDate());
                writer.println("Time: " + appointment.getAppointmentTime());
                writer.println("Department: " + appointment.getDepartment());
                writer.println("Type: " + appointment.getType());
                writer.println("Status: " + appointment.getStatus());
                writer.println("Room: " + appointment.getRoomNumber());
                writer.println("Consultation Fee: $" + String.format("%.2f", appointment.getConsultationFee()));
                writer.println("Notes: " + appointment.getNotes());
                writer.println();
            }
            
            // Export medical records
            writer.println("MEDICAL RECORDS");
            writer.println("---------------");
            for (MedicalRecord record : medicalRecords) {
                writer.println("ID: " + record.getRecordId());
                writer.println("Patient ID: " + record.getPatientId());
                writer.println("Doctor ID: " + record.getDoctorId());
                writer.println("Record Date: " + record.getRecordDate());
                writer.println("Diagnosis: " + record.getDiagnosis());
                writer.println("Prescription: " + record.getPrescription());
                writer.println("Treatment: " + record.getTreatment());
                writer.println("Follow-up Date: " + (record.getFollowUpDate() != null ? record.getFollowUpDate() : "N/A"));
                writer.println("Follow-up Notes: " + record.getFollowUpNotes());
                writer.println("Cost: $" + String.format("%.2f", record.getCost()));
                writer.println("Insurance Claim Status: " + record.getInsuranceClaimStatus());
                writer.println();
            }
            
            // Export departments and budgets
            writer.println("DEPARTMENTS AND BUDGETS");
            writer.println("-------------------------");
            for (String department : departments) {
                writer.println("Department: " + department);
                writer.println("Budget: $" + String.format("%.2f", departmentBudgets.getOrDefault(department, 0.0)));
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
        // Create a new hospital management system
        HospitalManagementSystem hospital = new HospitalManagementSystem("City General Hospital", "123 Main St, Anytown, USA", "(555) 123-4567");
        
        // Create some doctors
        Doctor doctor1 = new Doctor("D001", "John", "Smith", "Cardiology", "Cardiology", 
                                  new Date(2010, 5, 15), "(555) 123-4567", 
                                  "john.smith@hospital.com", "456 Oak Ave, Anytown, USA", 
                                  "MD12345", "MD from Johns Hopkins", 15, true);
        
        Doctor doctor2 = new Doctor("D002", "Sarah", "Johnson", "Neurology", "Neurology", 
                                  new Date(2012, 3, 20), "(555) 987-6543", 
                                  "sarah.johnson@hospital.com", "789 Pine Rd, Anytown, USA", 
                                  "MD98765", "MD from Harvard", 10, true);
        
        Doctor doctor3 = new Doctor("D003", "Michael", "Davis", "Orthopedics", "Orthopedics", 
                                  new Date(2015, 6, 10), "(555) 246-8135", 
                                  "michael.davis@hospital.com", "321 Elm Blvd, Anytown, USA", 
                                  "MD65432", "MD from Stanford", 12, true);
        
        Doctor doctor4 = new Doctor("D004", "Emily", "Wilson", "Pediatrics", "Pediatrics", 
                                  new Date(2018, 2, 5), "(555) 135-7924", 
                                  "emily.wilson@hospital.com", "654 Maple Dr, Anytown, USA", 
                                  "MD98765", "MD from Stanford", 8, true);
        
        Doctor doctor5 = new Doctor("D005", "Robert", "Brown", "Emergency", "Emergency", 
                                  new Date(2010, 8, 15), "(555) 246-8135", 
                                  "robert.brown@hospital.com", "321 Elm Blvd, Anytown, USA", 
                                  "MD65432", "MD from UCLA", 20, true);
        
        // Add doctors to hospital
        hospital.addDoctor(doctor1);
        hospital.addDoctor(doctor2);
        hospital.addDoctor(doctor3);
        hospital.addDoctor(doctor4);
        hospital.addDoctor(doctor5);
        
        // Create some patients
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
        
        Patient patient1 = new Patient("P001", "Alice", "Johnson", 
                                         dateFormat.parse("1985-03-12"), "Female", 
                                         "123 Main St, Anytown, USA", 
                                         "(555) 123-4567", "alice.johnson@email.com",
                                         "O Negative", "O Positive", "Hypertension", 
                                         "John Johnson", "555-987-6543",
                                         dateFormat.parse("2020-01-15"), true);
        
        Patient patient2 = new Patient("P002", "Bob", "Smith", 
                                         dateFormat.parse("1978-06-05"), "Male", 
                                         "456 Oak Ave, Anytown, USA", 
                                         "(555) 987-6543", "bob.smith@email.com",
                                         "No known allergies", "Childhood asthma", 
                                         "Jane Smith", "555-987-6543",
                                         dateFormat.parse("2021-03-10"), true);
        
        Patient patient3 = new Patient("P003", "Charlie", "Brown", 
                                         dateFormat.parse("1990-11-15"), "Male", 
                                         "789 Pine Rd, Anytown, USA", 
                                         "(555) 246-8135", "Penicillin", "Diabetes", 
                                         "Robert Brown", "555-246-8135",
                                         dateFormat.parse("2015-02-20"), true);
        
        Patient patient4 = new Patient("P004", "Diana", "Miller", 
                                         dateFormat.parse("1995-08-20"), "Female", 
                                         "321 Elm Blvd, Anytown, USA", 
                                         "(555) 135-7924", "N/A", "None", 
                                         "Diana Miller", "555-135-7924",
                                         dateFormat.parse("2018-04-05"), true);
        
        Patient patient5 = new Patient("P005", "Edward", "Williams", 
                                         dateFormat.parse("2000-12-30"), "Male", 
                                         "654 Maple Dr, Anytown, USA", 
                                         "(555) 987-6543", "Latex", "Hypertension", 
                                         "Susan Williams", "555-987-6543",
                                         dateFormat.parse("2019-01-10"), true);
        
        // Add patients to hospital
        hospital.addPatient(patient1);
        hospital.addPatient(patient2);
        hospital.addPatient(patient3);
        hospital.addPatient(patient4);
        hospital.addPatient(patient5);
        
        // Schedule some appointments
        Date today = new Date();
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(today);
        
        // Schedule appointment for tomorrow
        calendar.add(Calendar.DAY_OF_MONTH, 1);
        Date tomorrow = calendar.getTime();
        
        Appointment appointment1 = new Appointment("A001", "P001", "D001", tomorrow,
                                             new Date(0, 0, 0, 0, 1970, 1, 1), 
                                             "CONSULTATION", "Cardiology", "SCHEDULED",
                                             "Routine checkup", "Room 101", 150.0);
        
        Appointment appointment2 = new Appointment("A002", "P002", "D002", tomorrow,
                                             new Date(0, 0, 0, 0, 1970, 2, 0), 
                                             "FOLLOW_UP", "Neurology", "SCHEDULED",
                                             "Follow-up consultation", "Room 102", 200.0);
        
        // Add appointments to hospital
        hospital.scheduleAppointment(appointment1);
        hospital.scheduleAppointment(appointment2);
        
        // Create some medical records
        MedicalRecord record1 = new MedicalRecord("MR001", "P001", "D001", today,
                                               "Hypertension, mild, well controlled with medication", 
                                               "Lisinopril 20mg daily", "Continue with current medication and lifestyle changes", 
                                               "Low salt diet", new Date(), 150.0, "PAID");
        
        MedicalRecord record2 = new MedicalRecord("MR002", "P002", "D002", today,
                                               "Diabetes, type 2, needs insulin management", 
                                               "Metformin 500mg twice daily with meals", 
                                               "Low carbohydrate diet", new Date(), 200.0, "PENDING");
        
        // Add medical records to hospital
        hospital.addMedicalRecord(record1);
        hospital.addMedicalRecord(record2);
        
        // Print hospital information
        System.out.println("Hospital Management System");
        System.out.println("=========================");
        System.out.println("Hospital: " + hospital.getHospitalName());
        System.out.println("Total Patients: " + hospital.getTotalPatients());
        System.out.println("Total Doctors: " + hospital.getTotalDoctors());
        System.out.println("Total Appointments: " + hospital.getTotalAppointments());
        System.out.println("Total Medical Records: " + hospital.getTotalMedicalRecords());
        System.out.println("Total Department Budget: $" + String.format("%.2f", hospital.getTotalDepartmentBudget()));
        System.out.println();
        
        // Print patient summaries
        System.out.println("Patient Summaries");
        System.out.println("===============");
        for (Patient patient : hospital.getPatients()) {
            System.out.println(patient.getSummary());
            System.out.println();
        }
        
        // Print doctor summaries
        System.out.println("Doctor Summaries");
        System.out.println("===============");
        for (Doctor doctor : hospital.getDoctors()) {
            System.out.println(doctor.getSummary());
            System.out.println();
        }
        
        // Print appointment summaries
        System.out.println("Appointment Summaries");
        System.out.println("=================");
        for (Appointment appointment : hospital.getAppointments()) {
            System.out.println(appointment.getSummary());
            System.out.println();
        }
        
        // Print medical record summaries
        System.out.println("Medical Record Summaries");
        System.out.println("======================");
        for (MedicalRecord record : hospital.getMedicalRecords()) {
            System.out.println(record.getSummary());
            System.out.println();
        }
        
        // Print hospital report
        System.out.println(hospital.generateHospitalReport());
        
        // Save data to file
        hospital.saveToFile("hospital_data_export.txt");
        
        System.out.println("\n\nHospital Management System demonstration completed successfully!");
    }
}
<｜fim▁hole｜>