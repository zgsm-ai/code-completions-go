/**
 * Hotel Management System
 * 
 * This class implements a comprehensive hotel management system
 * that can handle reservations, room management, guest services, and billing.
 * 
 * Features:
 * - Room and suite management
 * - Guest registration and management
 * - Reservation booking and tracking
 * - Check-in and check-out processing
 * - Billing and payment management
 * - Staff management and scheduling
 * - Inventory and amenities tracking
 * - Reporting and analytics
 */

import java.util.*;
import java.text.*;
import java.io.*;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import java.util.stream.Collectors;

/**
 * Room class represents a hotel room
 */
class Room {
    private String roomId;
    private String roomNumber;
    private String roomType;
    private String floor;
    private int capacity;
    private double basePrice;
    private String currency;
    private String bedType;
    private String view;
    private List<String> amenities;
    private String status;  // AVAILABLE, OCCUPIED, MAINTENANCE, OUT_OF_ORDER, CLEANING
    private String description;
    private double size;
    private List<String> images;
    private boolean smoking;
    private boolean petFriendly;
    private String lastCleanedDate;
    private String housekeepingNotes;
    
    /**
     * Constructor for Room
     * @param roomId Room ID
     * @param roomNumber Room number
     * @param roomType Room type
     * @param floor Floor number
     * @param capacity Room capacity
     * @param basePrice Base price per night
     * @param currency Currency code
     * @param bedType Bed type
     * @param view Room view
     */
    public Room(String roomId, String roomNumber, String roomType, String floor,
               int capacity, double basePrice, String currency,
               String bedType, String view) {
        this.roomId = roomId;
        this.roomNumber = roomNumber;
        this.roomType = roomType;
        this.floor = floor;
        this.capacity = capacity;
        this.basePrice = basePrice;
        this.currency = currency;
        this.bedType = bedType;
        this.view = view;
        this.amenities = new ArrayList<>();
        this.status = "AVAILABLE";
        this.description = "";
        this.size = 0.0;
        this.images = new ArrayList<>();
        this.smoking = false;
        this.petFriendly = false;
        this.lastCleanedDate = "";
        this.housekeepingNotes = "";
    }
    
    /**
     * Get room ID
     * @return Room ID
     */
    public String getRoomId() {
        return roomId;
    }
    
    /**
     * Get room number
     * @return Room number
     */
    public String getRoomNumber() {
        return roomNumber;
    }
    
    /**
     * Get room type
     * @return Room type
     */
    public String getRoomType() {
        return roomType;
    }
    
    /**
     * Get floor
     * @return Floor number
     */
    public String getFloor() {
        return floor;
    }
    
    /**
     * Get capacity
     * @return Room capacity
     */
    public int getCapacity() {
        return capacity;
    }
    
    /**
     * Get base price
     * @return Base price per night
     */
    public double getBasePrice() {
        return basePrice;
    }
    
    /**
     * Get currency
     * @return Currency code
     */
    public String getCurrency() {
        return currency;
    }
    
    /**
     * Get bed type
     * @return Bed type
     */
    public String getBedType() {
        return bedType;
    }
    
    /**
     * Get view
     * @return Room view
     */
    public String getView() {
        return view;
    }
    
    /**
     * Get amenities
     * @return List of room amenities
     */
    public List<String> getAmenities() {
        return amenities;
    }
    
    /**
     * Get status
     * @return Room status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get description
     * @return Room description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get size
     * @return Room size in square meters
     */
    public double getSize() {
        return size;
    }
    
    /**
     * Get images
     * @return List of room images
     */
    public List<String> getImages() {
        return images;
    }
    
    /**
     * Check if room allows smoking
     * @return True if smoking is allowed, false otherwise
     */
    public boolean isSmoking() {
        return smoking;
    }
    
    /**
     * Check if room is pet-friendly
     * @return True if pets are allowed, false otherwise
     */
    public boolean isPetFriendly() {
        return petFriendly;
    }
    
    /**
     * Get last cleaned date
     * @return Last cleaned date
     */
    public String getLastCleanedDate() {
        return lastCleanedDate;
    }
    
    /**
     * Get housekeeping notes
     * @return Housekeeping notes
     */
    public String getHousekeepingNotes() {
        return housekeepingNotes;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update base price
     * @param price New base price
     */
    public void updateBasePrice(double price) {
        this.basePrice = price;
    }
    
    /**
     * Update description
     * @param description New description
     */
    public void updateDescription(String description) {
        this.description = description;
    }
    
    /**
     * Update size
     * @param size New room size
     */
    public void updateSize(double size) {
        this.size = size;
    }
    
    /**
     * Set smoking policy
     * @param smoking Smoking policy
     */
    public void setSmoking(boolean smoking) {
        this.smoking = smoking;
    }
    
    /**
     * Set pet-friendly policy
     * @param petFriendly Pet-friendly policy
     */
    public void setPetFriendly(boolean petFriendly) {
        this.petFriendly = petFriendly;
    }
    
    /**
     * Update last cleaned date
     * @param date Last cleaned date
     */
    public void updateLastCleanedDate(String date) {
        this.lastCleanedDate = date;
    }
    
    /**
     * Update housekeeping notes
     * @param notes Housekeeping notes
     */
    public void updateHousekeepingNotes(String notes) {
        this.housekeepingNotes = notes;
    }
    
    /**
     * Add amenity
     * @param amenity Amenity to add
     */
    public void addAmenity(String amenity) {
        if (!amenities.contains(amenity)) {
            amenities.add(amenity);
        }
    }
    
    /**
     * Remove amenity
     * @param amenity Amenity to remove
     */
    public void removeAmenity(String amenity) {
        amenities.remove(amenity);
    }
    
    /**
     * Add image
     * @param imageUrl Image URL to add
     */
    public void addImage(String imageUrl) {
        if (!images.contains(imageUrl)) {
            images.add(imageUrl);
        }
    }
    
    /**
     * Remove image
     * @param imageUrl Image URL to remove
     */
    public void removeImage(String imageUrl) {
        images.remove(imageUrl);
    }
    
    /**
     * Check if room is available
     * @return True if room is available, false otherwise
     */
    public boolean isAvailable() {
        return status.equals("AVAILABLE");
    }
    
    /**
     * Check if room is occupied
     * @return True if room is occupied, false otherwise
     */
    public boolean isOccupied() {
        return status.equals("OCCUPIED");
    }
    
    /**
     * Check if room needs cleaning
     * @return True if room needs cleaning, false otherwise
     */
    public boolean needsCleaning() {
        return status.equals("CLEANING");
    }
    
    /**
     * Get room summary
     * @return Room summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        
        sb.append("Room ID: ").append(roomId).append("\n");
        sb.append("Room Number: ").append(roomNumber).append("\n");
        sb.append("Room Type: ").append(roomType).append("\n");
        sb.append("Floor: ").append(floor).append("\n");
        sb.append("Capacity: ").append(capacity).append(" guests\n");
        sb.append("Base Price: ").append(currency).append(String.format("%.2f", basePrice)).append(" per night\n");
        sb.append("Bed Type: ").append(bedType).append("\n");
        sb.append("View: ").append(view).append("\n");
        sb.append("Status: ").append(status).append("\n");
        
        if (size > 0) {
            sb.append("Size: ").append(size).append(" sq. m\n");
        }
        
        if (!description.trim().isEmpty()) {
            sb.append("Description: ").append(description).append("\n");
        }
        
        if (!amenities.isEmpty()) {
            sb.append("Amenities: ").append(String.join(", ", amenities)).append("\n");
        }
        
        sb.append("Smoking: ").append(smoking ? "Yes" : "No").append("\n");
        sb.append("Pet Friendly: ").append(petFriendly ? "Yes" : "No").append("\n");
        
        if (!images.isEmpty()) {
            sb.append("Images: ").append(images.size()).append("\n");
        }
        
        if (!lastCleanedDate.trim().isEmpty()) {
            sb.append("Last Cleaned: ").append(lastCleanedDate).append("\n");
        }
        
        if (!housekeepingNotes.trim().isEmpty()) {
            sb.append("Housekeeping Notes: ").append(housekeepingNotes).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Guest class represents a hotel guest
 */
class Guest {
    private String guestId;
    private String firstName;
    private String lastName;
    private String email;
    private String phone;
    private String address;
    private String city;
    private String state;
    private String postalCode;
    private String country;
    private Date registrationDate;
    private String status;  // ACTIVE, INACTIVE, VIP
    private String idType;
    private String idNumber;
    private Date birthDate;
    private String nationality;
    private List<String> preferences;
    private List<String> specialRequests;
    private String membershipProgram;
    private String membershipNumber;
    private String emergencyContact;
    private String emergencyPhone;
    private boolean smoking;
    private String dietaryRestrictions;
    
    /**
     * Constructor for Guest
     * @param guestId Guest ID
     * @param firstName First name
     * @param lastName Last name
     * @param email Email address
     * @param phone Phone number
     * @param idType ID type
     * @param idNumber ID number
     * @param birthDate Birth date
     */
    public Guest(String guestId, String firstName, String lastName, String email,
                String phone, String idType, String idNumber, Date birthDate) {
        this.guestId = guestId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phone = phone;
        this.address = "";
        this.city = "";
        this.state = "";
        this.postalCode = "";
        this.country = "";
        this.registrationDate = new Date();
        this.status = "ACTIVE";
        this.idType = idType;
        this.idNumber = idNumber;
        this.birthDate = birthDate;
        this.nationality = "";
        this.preferences = new ArrayList<>();
        this.specialRequests = new ArrayList<>();
        this.membershipProgram = "";
        this.membershipNumber = "";
        this.emergencyContact = "";
        this.emergencyPhone = "";
        this.smoking = false;
        this.dietaryRestrictions = "";
    }
    
    /**
     * Get guest ID
     * @return Guest ID
     */
    public String getGuestId() {
        return guestId;
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
     * Get email
     * @return Email address
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get phone
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get address
     * @return Street address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get city
     * @return City
     */
    public String getCity() {
        return city;
    }
    
    /**
     * Get state
     * @return State
     */
    public String getState() {
        return state;
    }
    
    /**
     * Get postal code
     * @return Postal code
     */
    public String getPostalCode() {
        return postalCode;
    }
    
    /**
     * Get country
     * @return Country
     */
    public String getCountry() {
        return country;
    }
    
    /**
     * Get registration date
     * @return Registration date
     */
    public Date getRegistrationDate() {
        return registrationDate;
    }
    
    /**
     * Get status
     * @return Guest status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get ID type
     * @return ID type
     */
    public String getIdType() {
        return idType;
    }
    
    /**
     * Get ID number
     * @return ID number
     */
    public String getIdNumber() {
        return idNumber;
    }
    
    /**
     * Get birth date
     * @return Birth date
     */
    public Date getBirthDate() {
        return birthDate;
    }
    
    /**
     * Get nationality
     * @return Nationality
     */
    public String getNationality() {
        return nationality;
    }
    
    /**
     * Get preferences
     * @return List of guest preferences
     */
    public List<String> getPreferences() {
        return preferences;
    }
    
    /**
     * Get special requests
     * @return List of special requests
     */
    public List<String> getSpecialRequests() {
        return specialRequests;
    }
    
    /**
     * Get membership program
     * @return Membership program
     */
    public String getMembershipProgram() {
        return membershipProgram;
    }
    
    /**
     * Get membership number
     * @return Membership number
     */
    public String getMembershipNumber() {
        return membershipNumber;
    }
    
    /**
     * Get emergency contact
     * @return Emergency contact name
     */
    public String getEmergencyContact() {
        return emergencyContact;
    }
    
    /**
     * Get emergency phone
     * @return Emergency contact phone
     */
    public String getEmergencyPhone() {
        return emergencyPhone;
    }
    
    /**
     * Check if guest prefers smoking
     * @return True if smoking is preferred, false otherwise
     */
    public boolean isSmoking() {
        return smoking;
    }
    
    /**
     * Get dietary restrictions
     * @return Dietary restrictions
     */
    public String getDietaryRestrictions() {
        return dietaryRestrictions;
    }
    
    /**
     * Update first name
     * @param firstName New first name
     */
    public void updateFirstName(String firstName) {
        this.firstName = firstName;
    }
    
    /**
     * Update last name
     * @param lastName New last name
     */
    public void updateLastName(String lastName) {
        this.lastName = lastName;
    }
    
    /**
     * Update email
     * @param email New email
     */
    public void updateEmail(String email) {
        this.email = email;
    }
    
    /**
     * Update phone
     * @param phone New phone
     */
    public void updatePhone(String phone) {
        this.phone = phone;
    }
    
    /**
     * Update address
     * @param address New street address
     */
    public void updateAddress(String address) {
        this.address = address;
    }
    
    /**
     * Update city
     * @param city New city
     */
    public void updateCity(String city) {
        this.city = city;
    }
    
    /**
     * Update state
     * @param state New state
     */
    public void updateState(String state) {
        this.state = state;
    }
    
    /**
     * Update postal code
     * @param postalCode New postal code
     */
    public void updatePostalCode(String postalCode) {
        this.postalCode = postalCode;
    }
    
    /**
     * Update country
     * @param country New country
     */
    public void updateCountry(String country) {
        this.country = country;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update nationality
     * @param nationality New nationality
     */
    public void updateNationality(String nationality) {
        this.nationality = nationality;
    }
    
    /**
     * Set membership program
     * @param program Membership program
     * @param number Membership number
     */
    public void setMembership(String program, String number) {
        this.membershipProgram = program;
        this.membershipNumber = number;
    }
    
    /**
     * Set emergency contact
     * @param contact Emergency contact name
     * @param phone Emergency contact phone
     */
    public void setEmergencyContact(String contact, String phone) {
        this.emergencyContact = contact;
        this.emergencyPhone = phone;
    }
    
    /**
     * Set smoking preference
     * @param smoking Smoking preference
     */
    public void setSmoking(boolean smoking) {
        this.smoking = smoking;
    }
    
    /**
     * Set dietary restrictions
     * @param restrictions Dietary restrictions
     */
    public void setDietaryRestrictions(String restrictions) {
        this.dietaryRestrictions = restrictions;
    }
    
    /**
     * Add preference
     * @param preference Preference to add
     */
    public void addPreference(String preference) {
        if (!preferences.contains(preference)) {
            preferences.add(preference);
        }
    }
    
    /**
     * Remove preference
     * @param preference Preference to remove
     */
    public void removePreference(String preference) {
        preferences.remove(preference);
    }
    
    /**
     * Add special request
     * @param request Special request to add
     */
    public void addSpecialRequest(String request) {
        if (!specialRequests.contains(request)) {
            specialRequests.add(request);
        }
    }
    
    /**
     * Remove special request
     * @param request Special request to remove
     */
    public void removeSpecialRequest(String request) {
        specialRequests.remove(request);
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
     * Get guest summary
     * @return Guest summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Guest ID: ").append(guestId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        
        if (!address.trim().isEmpty()) {
            sb.append("Address: ").append(address).append("\n");
        }
        
        if (!city.trim().isEmpty()) {
            sb.append("City: ").append(city).append("\n");
        }
        
        if (!state.trim().isEmpty()) {
            sb.append("State: ").append(state).append("\n");
        }
        
        if (!postalCode.trim().isEmpty()) {
            sb.append("Postal Code: ").append(postalCode).append("\n");
        }
        
        if (!country.trim().isEmpty()) {
            sb.append("Country: ").append(country).append("\n");
        }
        
        sb.append("Registration Date: ").append(sdf.format(registrationDate)).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("ID: ").append(idType).append(" - ").append(idNumber).append("\n");
        sb.append("Birth Date: ").append(sdf.format(birthDate)).append("\n");
        sb.append("Age: ").append(getAge()).append("\n");
        
        if (!nationality.trim().isEmpty()) {
            sb.append("Nationality: ").append(nationality).append("\n");
        }
        
        if (!membershipProgram.trim().isEmpty()) {
            sb.append("Membership: ").append(membershipProgram)
              .append(" (").append(membershipNumber).append(")\n");
        }
        
        if (!emergencyContact.trim().isEmpty()) {
            sb.append("Emergency Contact: ").append(emergencyContact)
              .append(" (").append(emergencyPhone).append(")\n");
        }
        
        if (!preferences.isEmpty()) {
            sb.append("Preferences: ").append(String.join(", ", preferences)).append("\n");
        }
        
        if (!specialRequests.isEmpty()) {
            sb.append("Special Requests: ").append(String.join(", ", specialRequests)).append("\n");
        }
        
        sb.append("Smoking Preference: ").append(smoking ? "Yes" : "No").append("\n");
        
        if (!dietaryRestrictions.trim().isEmpty()) {
            sb.append("Dietary Restrictions: ").append(dietaryRestrictions).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Reservation class represents a hotel reservation
 */
class Reservation {
    private String reservationId;
    private String guestId;
    private String roomId;
    private Date checkInDate;
    private Date checkOutDate;
    private String status;  // CONFIRMED, CHECKED_IN, CHECKED_OUT, CANCELLED, NO_SHOW
    private double totalAmount;
    private String currency;
    private int adults;
    private int children;
    private List<String> specialRequests;
    private String paymentStatus;  // PENDING, PAID, REFUNDED
    private String paymentMethod;
    private String confirmationCode;
    private Date bookingDate;
    private String notes;
    private boolean guaranteed;
    private double depositAmount;
    private double cancellationFee;
    private Date cancellationDate;
    
    /**
     * Constructor for Reservation
     * @param reservationId Reservation ID
     * @param guestId Guest ID
     * @param roomId Room ID
     * @param checkInDate Check-in date
     * @param checkOutDate Check-out date
     * @param totalAmount Total amount
     * @param currency Currency code
     * @param adults Number of adults
     * @param children Number of children
     */
    public Reservation(String reservationId, String guestId, String roomId,
                     Date checkInDate, Date checkOutDate, double totalAmount,
                     String currency, int adults, int children) {
        this.reservationId = reservationId;
        this.guestId = guestId;
        this.roomId = roomId;
        this.checkInDate = checkInDate;
        this.checkOutDate = checkOutDate;
        this.status = "CONFIRMED";
        this.totalAmount = totalAmount;
        this.currency = currency;
        this.adults = adults;
        this.children = children;
        this.specialRequests = new ArrayList<>();
        this.paymentStatus = "PENDING";
        this.paymentMethod = "";
        this.confirmationCode = "";
        this.bookingDate = new Date();
        this.notes = "";
        this.guaranteed = false;
        this.depositAmount = 0.0;
        this.cancellationFee = 0.0;
        this.cancellationDate = null;
    }
    
    /**
     * Get reservation ID
     * @return Reservation ID
     */
    public String getReservationId() {
        return reservationId;
    }
    
    /**
     * Get guest ID
     * @return Guest ID
     */
    public String getGuestId() {
        return guestId;
    }
    
    /**
     * Get room ID
     * @return Room ID
     */
    public String getRoomId() {
        return roomId;
    }
    
    /**
     * Get check-in date
     * @return Check-in date
     */
    public Date getCheckInDate() {
        return checkInDate;
    }
    
    /**
     * Get check-out date
     * @return Check-out date
     */
    public Date getCheckOutDate() {
        return checkOutDate;
    }
    
    /**
     * Get status
     * @return Reservation status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get total amount
     * @return Total amount
     */
    public double getTotalAmount() {
        return totalAmount;
    }
    
    /**
     * Get currency
     * @return Currency code
     */
    public String getCurrency() {
        return currency;
    }
    
    /**
     * Get number of adults
     * @return Number of adults
     */
    public int getAdults() {
        return adults;
    }
    
    /**
     * Get number of children
     * @return Number of children
     */
    public int getChildren() {
        return children;
    }
    
    /**
     * Get total number of guests
     * @return Total number of guests
     */
    public int getTotalGuests() {
        return adults + children;
    }
    
    /**
     * Get special requests
     * @return List of special requests
     */
    public List<String> getSpecialRequests() {
        return specialRequests;
    }
    
    /**
     * Get payment status
     * @return Payment status
     */
    public String getPaymentStatus() {
        return paymentStatus;
    }
    
    /**
     * Get payment method
     * @return Payment method
     */
    public String getPaymentMethod() {
        return paymentMethod;
    }
    
    /**
     * Get confirmation code
     * @return Confirmation code
     */
    public String getConfirmationCode() {
        return confirmationCode;
    }
    
    /**
     * Get booking date
     * @return Booking date
     */
    public Date getBookingDate() {
        return bookingDate;
    }
    
    /**
     * Get notes
     * @return Reservation notes
     */
    public String getNotes() {
        return notes;
    }
    
    /**
     * Check if reservation is guaranteed
     * @return True if guaranteed, false otherwise
     */
    public boolean isGuaranteed() {
        return guaranteed;
    }
    
    /**
     * Get deposit amount
     * @return Deposit amount
     */
    public double getDepositAmount() {
        return depositAmount;
    }
    
    /**
     * Get cancellation fee
     * @return Cancellation fee
     */
    public double getCancellationFee() {
        return cancellationFee;
    }
    
    /**
     * Get cancellation date
     * @return Cancellation date
     */
    public Date getCancellationDate() {
        return cancellationDate;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update total amount
     * @param amount New total amount
     */
    public void updateTotalAmount(double amount) {
        this.totalAmount = amount;
    }
    
    /**
     * Set as guaranteed
     * @param depositAmount Deposit amount
     */
    public void setAsGuaranteed(double depositAmount) {
        this.guaranteed = true;
        this.depositAmount = depositAmount;
    }
    
    /**
     * Update payment status
     * @param status New payment status
     */
    public void updatePaymentStatus(String status) {
        this.paymentStatus = status;
    }
    
    /**
     * Set payment method
     * @param method Payment method
     */
    public void setPaymentMethod(String method) {
        this.paymentMethod = method;
    }
    
    /**
     * Set confirmation code
     * @param code Confirmation code
     */
    public void setConfirmationCode(String code) {
        this.confirmationCode = code;
    }
    
    /**
     * Add note
     * @param note Note to add
     */
    public void addNote(String note) {
        if (notes.trim().isEmpty()) {
            notes = note;
        } else {
            notes += "; " + note;
        }
    }
    
    /**
     * Add special request
     * @param request Special request to add
     */
    public void addSpecialRequest(String request) {
        if (!specialRequests.contains(request)) {
            specialRequests.add(request);
        }
    }
    
    /**
     * Remove special request
     * @param request Special request to remove
     */
    public void removeSpecialRequest(String request) {
        specialRequests.remove(request);
    }
    
    /**
     * Cancel reservation
     * @param cancellationFee Cancellation fee
     */
    public void cancel(double cancellationFee) {
        this.status = "CANCELLED";
        this.cancellationFee = cancellationFee;
        this.cancellationDate = new Date();
    }
    
    /**
     * Mark as no-show
     */
    public void markAsNoShow() {
        this.status = "NO_SHOW";
        this.cancellationDate = new Date();
    }
    
    /**
     * Calculate nights
     * @return Number of nights
     */
    public int calculateNights() {
        long diff = checkOutDate.getTime() - checkInDate.getTime();
        return (int) (diff / (24 * 60 * 60 * 1000));
    }
    
    /**
     * Check if dates overlap with another reservation
     * @param otherCheckIn Check-in date of other reservation
     * @param otherCheckOut Check-out date of other reservation
     * @return True if dates overlap, false otherwise
     */
    public boolean datesOverlap(Date otherCheckIn, Date otherCheckOut) {
        // Check if this reservation starts before the other ends
        // and this reservation ends after the other starts
        return !checkOutDate.before(otherCheckIn) && !checkInDate.after(otherCheckOut);
    }
    
    /**
     * Get reservation summary
     * @return Reservation summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Reservation ID: ").append(reservationId).append("\n");
        sb.append("Guest ID: ").append(guestId).append("\n");
        sb.append("Room ID: ").append(roomId).append("\n");
        sb.append("Check-in Date: ").append(sdf.format(checkInDate)).append("\n");
        sb.append("Check-out Date: ").append(sdf.format(checkOutDate)).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Total Amount: ").append(currency).append(String.format("%.2f", totalAmount)).append("\n");
        sb.append("Adults: ").append(adults).append("\n");
        sb.append("Children: ").append(children).append("\n");
        sb.append("Nights: ").append(calculateNights()).append("\n");
        sb.append("Booking Date: ").append(sdf.format(bookingDate)).append("\n");
        sb.append("Payment Status: ").append(paymentStatus).append("\n");
        
        if (!paymentMethod.trim().isEmpty()) {
            sb.append("Payment Method: ").append(paymentMethod).append("\n");
        }
        
        if (!confirmationCode.trim().isEmpty()) {
            sb.append("Confirmation Code: ").append(confirmationCode).append("\n");
        }
        
        if (guaranteed) {
            sb.append("Guaranteed Reservation: Yes\n");
            sb.append("Deposit Amount: ").append(currency).append(String.format("%.2f", depositAmount)).append("\n");
        }
        
        if (!specialRequests.isEmpty()) {
            sb.append("Special Requests: ").append(String.join(", ", specialRequests)).append("\n");
        }
        
        if (!notes.trim().isEmpty()) {
            sb.append("Notes: ").append(notes).append("\n");
        }
        
        if (status.equals("CANCELLED")) {
            sb.append("Cancellation Date: ").append(sdf.format(cancellationDate)).append("\n");
            sb.append("Cancellation Fee: ").append(currency).append(String.format("%.2f", cancellationFee)).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Staff class represents a hotel staff member
 */
class Staff {
    private String staffId;
    private String firstName;
    private String lastName;
    private String email;
    private String phone;
    private String position;
    private String department;
    private Date hireDate;
    private String status;  // ACTIVE, INACTIVE, ON_LEAVE
    private double salary;
    private String currency;
    private String workSchedule;
    private List<String> skills;
    private List<String> certifications;
    private String emergencyContact;
    private String emergencyPhone;
    private List<String> assignedTasks;
    private List<String> assignedShifts;
    
    /**
     * Constructor for Staff
     * @param staffId Staff ID
     * @param firstName First name
     * @param lastName Last name
     * @param email Email address
     * @param phone Phone number
     * @param position Position
     * @param department Department
     * @param hireDate Hire date
     * @param salary Salary
     * @param currency Currency code
     * @param workSchedule Work schedule
     */
    public Staff(String staffId, String firstName, String lastName, String email,
                String phone, String position, String department, Date hireDate,
                double salary, String currency, String workSchedule) {
        this.staffId = staffId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phone = phone;
        this.position = position;
        this.department = department;
        this.hireDate = hireDate;
        this.status = "ACTIVE";
        this.salary = salary;
        this.currency = currency;
        this.workSchedule = workSchedule;
        this.skills = new ArrayList<>();
        this.certifications = new ArrayList<>();
        this.emergencyContact = "";
        this.emergencyPhone = "";
        this.assignedTasks = new ArrayList<>();
        this.assignedShifts = new ArrayList<>();
    }
    
    /**
     * Get staff ID
     * @return Staff ID
     */
    public String getStaffId() {
        return staffId;
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
     * Get email
     * @return Email address
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get phone
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get position
     * @return Position
     */
    public String getPosition() {
        return position;
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
     * Get status
     * @return Staff status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get salary
     * @return Salary
     */
    public double getSalary() {
        return salary;
    }
    
    /**
     * Get currency
     * @return Currency code
     */
    public String getCurrency() {
        return currency;
    }
    
    /**
     * Get work schedule
     * @return Work schedule
     */
    public String getWorkSchedule() {
        return workSchedule;
    }
    
    /**
     * Get skills
     * @return List of skills
     */
    public List<String> getSkills() {
        return skills;
    }
    
    /**
     * Get certifications
     * @return List of certifications
     */
    public List<String> getCertifications() {
        return certifications;
    }
    
    /**
     * Get emergency contact
     * @return Emergency contact name
     */
    public String getEmergencyContact() {
        return emergencyContact;
    }
    
    /**
     * Get emergency phone
     * @return Emergency contact phone
     */
    public String getEmergencyPhone() {
        return emergencyPhone;
    }
    
    /**
     * Get assigned tasks
     * @return List of assigned tasks
     */
    public List<String> getAssignedTasks() {
        return assignedTasks;
    }
    
    /**
     * Get assigned shifts
     * @return List of assigned shifts
     */
    public List<String> getAssignedShifts() {
        return assignedShifts;
    }
    
    /**
     * Update first name
     * @param firstName New first name
     */
    public void updateFirstName(String firstName) {
        this.firstName = firstName;
    }
    
    /**
     * Update last name
     * @param lastName New last name
     */
    public void updateLastName(String lastName) {
        this.lastName = lastName;
    }
    
    /**
     * Update email
     * @param email New email
     */
    public void updateEmail(String email) {
        this.email = email;
    }
    
    /**
     * Update phone
     * @param phone New phone
     */
    public void updatePhone(String phone) {
        this.phone = phone;
    }
    
    /**
     * Update position
     * @param position New position
     */
    public void updatePosition(String position) {
        this.position = position;
    }
    
    /**
     * Update department
     * @param department New department
     */
    public void updateDepartment(String department) {
        this.department = department;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update salary
     * @param salary New salary
     */
    public void updateSalary(double salary) {
        this.salary = salary;
    }
    
    /**
     * Update work schedule
     * @param schedule New work schedule
     */
    public void updateWorkSchedule(String schedule) {
        this.workSchedule = schedule;
    }
    
    /**
     * Set emergency contact
     * @param contact Emergency contact name
     * @param phone Emergency contact phone
     */
    public void setEmergencyContact(String contact, String phone) {
        this.emergencyContact = contact;
        this.emergencyPhone = phone;
    }
    
    /**
     * Add skill
     * @param skill Skill to add
     */
    public void addSkill(String skill) {
        if (!skills.contains(skill)) {
            skills.add(skill);
        }
    }
    
    /**
     * Remove skill
     * @param skill Skill to remove
     */
    public void removeSkill(String skill) {
        skills.remove(skill);
    }
    
    /**
     * Add certification
     * @param certification Certification to add
     */
    public void addCertification(String certification) {
        if (!certifications.contains(certification)) {
            certifications.add(certification);
        }
    }
    
    /**
     * Remove certification
     * @param certification Certification to remove
     */
    public void removeCertification(String certification) {
        certifications.remove(certification);
    }
    
    /**
     * Assign task
     * @param taskId Task ID to assign
     */
    public void assignTask(String taskId) {
        if (!assignedTasks.contains(taskId)) {
            assignedTasks.add(taskId);
        }
    }
    
    /**
     * Remove task assignment
     * @param taskId Task ID to remove
     */
    public void removeTaskAssignment(String taskId) {
        assignedTasks.remove(taskId);
    }
    
    /**
     * Assign shift
     * @param shiftId Shift ID to assign
     */
    public void assignShift(String shiftId) {
        if (!assignedShifts.contains(shiftId)) {
            assignedShifts.add(shiftId);
        }
    }
    
    /**
     * Remove shift assignment
     * @param shiftId Shift ID to remove
     */
    public void removeShiftAssignment(String shiftId) {
        assignedShifts.remove(shiftId);
    }
    
    /**
     * Get years of service
     * @return Years of service
     */
    public int getYearsOfService() {
        Calendar now = Calendar.getInstance();
        Calendar hire = Calendar.getInstance();
        now.setTime(new Date());
        hire.setTime(hireDate);
        
        return now.get(Calendar.YEAR) - hire.get(Calendar.YEAR);
    }
    
    /**
     * Get staff summary
     * @return Staff summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Staff ID: ").append(staffId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Position: ").append(position).append("\n");
        sb.append("Department: ").append(department).append("\n");
        sb.append("Hire Date: ").append(sdf.format(hireDate)).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Salary: ").append(currency).append(String.format("%.2f", salary)).append("\n");
        sb.append("Work Schedule: ").append(workSchedule).append("\n");
        sb.append("Years of Service: ").append(getYearsOfService()).append("\n");
        
        if (!skills.isEmpty()) {
            sb.append("Skills: ").append(String.join(", ", skills)).append("\n");
        }
        
        if (!certifications.isEmpty()) {
            sb.append("Certifications: ").append(String.join(", ", certifications)).append("\n");
        }
        
        if (!emergencyContact.trim().isEmpty()) {
            sb.append("Emergency Contact: ").append(emergencyContact)
              .append(" (").append(emergencyPhone).append(")\n");
        }
        
        if (!assignedTasks.isEmpty()) {
            sb.append("Assigned Tasks: ").append(String.join(", ", assignedTasks)).append("\n");
        }
        
        if (!assignedShifts.isEmpty()) {
            sb.append("Assigned Shifts: ").append(String.join(", ", assignedShifts)).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * HotelManagementSystem class manages the entire hotel management system
 */
public class HotelManagementSystem {
    private String hotelName;
    private String hotelCode;
    private String address;
    private String phone;
    private String email;
    private List<Room> rooms;
    private Map<String, Room> roomMap;
    private List<Guest> guests;
    private Map<String, Guest> guestMap;
    private List<Reservation> reservations;
    private Map<String, Reservation> reservationMap;
    private List<Staff> staff;
    private Map<String, Staff> staffMap;
    private List<String> roomTypes;
    private List<String> departments;
    private Map<String, Double> roomTypePriceModifiers;
    
    /**
     * Constructor for HotelManagementSystem
     * @param hotelName Hotel name
     * @param hotelCode Hotel code
     * @param address Hotel address
     * @param phone Hotel phone
     * @param email Hotel email
     */
    public HotelManagementSystem(String hotelName, String hotelCode,
                            String address, String phone, String email) {
        this.hotelName = hotelName;
        this.hotelCode = hotelCode;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.rooms = new ArrayList<>();
        this.roomMap = new HashMap<>();
        this.guests = new ArrayList<>();
        this.guestMap = new HashMap<>();
        this.reservations = new ArrayList<>();
        this.reservationMap = new HashMap<>();
        this.staff = new ArrayList<>();
        this.staffMap = new HashMap<>();
        this.roomTypes = new ArrayList<>();
        this.departments = new ArrayList<>();
        this.roomTypePriceModifiers = new HashMap<>();
        
        // Initialize room types
        roomTypes.add("STANDARD");
        roomTypes.add("DELUXE");
        roomTypes.add("SUITE");
        roomTypes.add("PRESIDENTIAL");
        
        // Initialize departments
        departments.add("FRONT_DESK");
        departments.add("HOUSEKEEPING");
        departments.add("MAINTENANCE");
        departments.add("FOOD_AND_BEVERAGE");
        departments.add("SECURITY");
        departments.add("MANAGEMENT");
        departments.add("CONCIERGE");
        departments.add("SALES_AND_MARKETING");
        
        // Initialize room type price modifiers
        roomTypePriceModifiers.put("STANDARD", 1.0);
        roomTypePriceModifiers.put("DELUXE", 1.5);
        roomTypePriceModifiers.put("SUITE", 2.0);
        roomTypePriceModifiers.put("PRESIDENTIAL", 3.0);
    }
    
    /**
     * Get hotel name
     * @return Hotel name
     */
    public String getHotelName() {
        return hotelName;
    }
    
    /**
     * Get hotel code
     * @return Hotel code
     */
    public String getHotelCode() {
        return hotelCode;
    }
    
    /**
     * Get address
     * @return Hotel address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get phone
     * @return Hotel phone
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get email
     * @return Hotel email
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get rooms
     * @return List of rooms
     */
    public List<Room> getRooms() {
        return rooms;
    }
    
    /**
     * Get guests
     * @return List of guests
     */
    public List<Guest> getGuests() {
        return guests;
    }
    
    /**
     * Get reservations
     * @return List of reservations
     */
    public List<Reservation> getReservations() {
        return reservations;
    }
    
    /**
     * Get staff
     * @return List of staff
     */
    public List<Staff> getStaff() {
        return staff;
    }
    
    /**
     * Get room types
     * @return List of room types
     */
    public List<String> getRoomTypes() {
        return roomTypes;
    }
    
    /**
     * Get departments
     * @return List of departments
     */
    public List<String> getDepartments() {
        return departments;
    }
    
    /**
     * Add a room
     * @param room Room to add
     * @return True if added successfully, false otherwise
     */
    public boolean addRoom(Room room) {
        if (roomMap.containsKey(room.getRoomId())) {
            return false;
        }
        
        rooms.add(room);
        roomMap.put(room.getRoomId(), room);
        return true;
    }
    
    /**
     * Add a guest
     * @param guest Guest to add
     * @return True if added successfully, false otherwise
     */
    public boolean addGuest(Guest guest) {
        if (guestMap.containsKey(guest.getGuestId())) {
            return false;
        }
        
        guests.add(guest);
        guestMap.put(guest.getGuestId(), guest);
        return true;
    }
    
    /**
     * Add a reservation
     * @param reservation Reservation to add
     * @return True if added successfully, false otherwise
     */
    public boolean addReservation(Reservation reservation) {
        if (reservationMap.containsKey(reservation.getReservationId())) {
            return false;
        }
        
        reservations.add(reservation);
        reservationMap.put(reservation.getReservationId(), reservation);
        return true;
    }
    
    /**
     * Add a staff member
     * @param staffMember Staff member to add
     * @return True if added successfully, false otherwise
     */
    public boolean addStaff(Staff staffMember) {
        if (staffMap.containsKey(staffMember.getStaffId())) {
            return false;
        }
        
        staff.add(staffMember);
        staffMap.put(staffMember.getStaffId(), staffMember);
        return true;
    }
    
    /**
     * Get room by ID
     * @param roomId Room ID
     * @return Room if found, null otherwise
     */
    public Room getRoomById(String roomId) {
        return roomMap.get(roomId);
    }
    
    /**
     * Get guest by ID
     * @param guestId Guest ID
     * @return Guest if found, null otherwise
     */
    public Guest getGuestById(String guestId) {
        return guestMap.get(guestId);
    }
    
    /**
     * Get reservation by ID
     * @param reservationId Reservation ID
     * @return Reservation if found, null otherwise
     */
    public Reservation getReservationById(String reservationId) {
        return reservationMap.get(reservationId);
    }
    
    /**
     * Get staff by ID
     * @param staffId Staff ID
     * @return Staff member if found, null otherwise
     */
    public Staff getStaffById(String staffId) {
        return staffMap.get(staffId);
    }
    
    /**
     * Get rooms by type
     * @param roomType Room type
     * @return List of rooms of specified type
     */
    public List<Room> getRoomsByType(String roomType) {
        List<Room> result = new ArrayList<>();
        
        for (Room room : rooms) {
            if (room.getRoomType().equals(roomType)) {
                result.add(room);
            }
        }
        
        return result;
    }
    
    /**
     * Get available rooms
     * @return List of available rooms
     */
    public List<Room> getAvailableRooms() {
        List<Room> result = new ArrayList<>();
        
        for (Room room : rooms) {
            if (room.isAvailable()) {
                result.add(room);
            }
        }
        
        return result;
    }
    
    /**
     * Get rooms by capacity
     * @param capacity Minimum capacity
     * @return List of rooms with at least the specified capacity
     */
    public List<Room> getRoomsByCapacity(int capacity) {
        List<Room> result = new ArrayList<>();
        
        for (Room room : rooms) {
            if (room.getCapacity() >= capacity) {
                result.add(room);
            }
        }
        
        return result;
    }
    
    /**
     * Get active guests
     * @return List of active guests
     */
    public List<Guest> getActiveGuests() {
        List<Guest> result = new ArrayList<>();
        
        for (Guest guest : guests) {
            if (guest.getStatus().equals("ACTIVE")) {
                result.add(guest);
            }
        }
        
        return result;
    }
    
    /**
     * Get guests by status
     * @param status Guest status
     * @return List of guests with specified status
     */
    public List<Guest> getGuestsByStatus(String status) {
        List<Guest> result = new ArrayList<>();
        
        for (Guest guest : guests) {
            if (guest.getStatus().equals(status)) {
                result.add(guest);
            }
        }
        
        return result;
    }
    
    /**
     * Get active reservations
     * @return List of active reservations
     */
    public List<Reservation> getActiveReservations() {
        List<Reservation> result = new ArrayList<>();
        
        for (Reservation reservation : reservations) {
            if (reservation.getStatus().equals("CONFIRMED") || 
                reservation.getStatus().equals("CHECKED_IN")) {
                result.add(reservation);
            }
        }
        
        return result;
    }
    
    /**
     * Get reservations by status
     * @param status Reservation status
     * @return List of reservations with specified status
     */
    public List<Reservation> getReservationsByStatus(String status) {
        List<Reservation> result = new ArrayList<>();
        
        for (Reservation reservation : reservations) {
            if (reservation.getStatus().equals(status)) {
                result.add(reservation);
            }
        }
        
        return result;
    }
    
    /**
     * Get active staff
     * @return List of active staff
     */
    public List<Staff> getActiveStaff() {
        List<Staff> result = new ArrayList<>();
        
        for (Staff staffMember : staff) {
            if (staffMember.getStatus().equals("ACTIVE")) {
                result.add(staffMember);
            }
        }
        
        return result;
    }
    
    /**
     * Get staff by department
     * @param department Department name
     * @return List of staff in specified department
     */
    public List<Staff> getStaffByDepartment(String department) {
        List<Staff> result = new ArrayList<>();
        
        for (Staff staffMember : staff) {
            if (staffMember.getDepartment().equals(department)) {
                result.add(staffMember);
            }
        }
        
        return result;
    }
    
    /**
     * Get staff by position
     * @param position Position name
     * @return List of staff with specified position
     */
    public List<Staff> getStaffByPosition(String position) {
        List<Staff> result = new ArrayList<>();
        
        for (Staff staffMember : staff) {
            if (staffMember.getPosition().equals(position)) {
                result.add(staffMember);
            }
        }
        
        return result;
    }
    
    /**
     * Get available rooms for dates
     * @param checkIn Check-in date
     * @param checkOut Check-out date
     * @return List of available rooms for the date range
     */
    public List<Room> getAvailableRoomsForDates(Date checkIn, Date checkOut) {
        List<Room> result = new ArrayList<>();
        
        for (Room room : rooms) {
            boolean isAvailable = true;
            
            // Check if room is in a suitable status
            if (!room.isAvailable()) {
                isAvailable = false;
            }
            
            // Check if room is not reserved for the date range
            for (Reservation reservation : reservations) {
                if (reservation.getRoomId().equals(room.getRoomId()) && 
                    (reservation.getStatus().equals("CONFIRMED") || 
                     reservation.getStatus().equals("CHECKED_IN")) &&
                    reservation.datesOverlap(checkIn, checkOut)) {
                    isAvailable = false;
                    break;
                }
            }
            
            if (isAvailable) {
                result.add(room);
            }
        }
        
        return result;
    }
    
    /**
     * Get reservations by guest
     * @param guestId Guest ID
     * @return List of reservations for the guest
     */
    public List<Reservation> getReservationsByGuest(String guestId) {
        List<Reservation> result = new ArrayList<>();
        
        for (Reservation reservation : reservations) {
            if (reservation.getGuestId().equals(guestId)) {
                result.add(reservation);
            }
        }
        
        return result;
    }
    
    /**
     * Calculate room price for a reservation
     * @param room Room
     * @param checkIn Check-in date
     * @param checkOut Check-out date
     * @param adults Number of adults
     * @param children Number of children
     * @return Total price
     */
    public double calculateRoomPrice(Room room, Date checkIn, Date checkOut, int adults, int children) {
        // Calculate base price
        double basePrice = room.getBasePrice();
        double typeModifier = roomTypePriceModifiers.getOrDefault(room.getRoomType(), 1.0);
        
        // Calculate nights
        long diff = checkOut.getTime() - checkIn.getTime();
        int nights = (int) (diff / (24 * 60 * 60 * 1000));
        
        // Apply type modifier
        double totalPrice = basePrice * typeModifier * nights;
        
        // Apply weekend premium
        for (Date date = checkIn; date.before(checkOut); date = new Date(date.getTime() + 24 * 60 * 60 * 1000)) {
            Calendar cal = Calendar.getInstance();
            cal.setTime(date);
            int dayOfWeek = cal.get(Calendar.DAY_OF_WEEK);
            
            if (dayOfWeek == Calendar.FRIDAY || dayOfWeek == Calendar.SATURDAY) {
                totalPrice += basePrice * typeModifier * 0.25;  // 25% weekend premium
            }
        }
        
        // Add extra guest charges
        int totalGuests = adults + children;
        if (totalGuests > room.getCapacity()) {
            totalPrice += (totalGuests - room.getCapacity()) * 30.0;  // $30 per extra guest
        }
        
        return totalPrice;
    }
    
    /**
     * Make a reservation
     * @param guestId Guest ID
     * @param roomId Room ID
     * @param checkIn Check-in date
     * @param checkOut Check-out date
     * @param adults Number of adults
     * @param children Number of children
     * @return Reservation ID if successful, null otherwise
     */
    public String makeReservation(String guestId, String roomId, Date checkIn, Date checkOut, int adults, int children) {
        Guest guest = getGuestById(guestId);
        Room room = getRoomById(roomId);
        
        if (guest == null || room == null) {
            return null;
        }
        
        // Check if guest is active
        if (!guest.getStatus().equals("ACTIVE")) {
            return null;
        }
        
        // Check if room is suitable for guests
        if (room.getCapacity() < adults + children) {
            return null;
        }
        
        // Check if room is available for the date range
        if (!getAvailableRoomsForDates(checkIn, checkOut).contains(room)) {
            return null;
        }
        
        // Calculate price
        double totalPrice = calculateRoomPrice(room, checkIn, checkOut, adults, children);
        
        // Create reservation
        String reservationId = "RES" + (reservations.size() + 1);
        Reservation reservation = new Reservation(
            reservationId, guestId, roomId, checkIn, checkOut,
            totalPrice, room.getCurrency(), adults, children
        );
        
        // Generate confirmation code
        reservation.setConfirmationCode("HTL" + hotelCode + reservationId.substring(3));
        
        // Add reservation to system
        addReservation(reservation);
        
        return reservationId;
    }
    
    /**
     * Check in a guest
     * @param reservationId Reservation ID
     * @return True if checked in successfully, false otherwise
     */
    public boolean checkIn(String reservationId) {
        Reservation reservation = getReservationById(reservationId);
        
        if (reservation == null || !reservation.getStatus().equals("CONFIRMED")) {
            return false;
        }
        
        Room room = getRoomById(reservation.getRoomId());
        if (room == null) {
            return false;
        }
        
        // Update room status
        room.updateStatus("OCCUPIED");
        
        // Update reservation status
        reservation.updateStatus("CHECKED_IN");
        
        return true;
    }
    
    /**
     * Check out a guest
     * @param reservationId Reservation ID
     * @return True if checked out successfully, false otherwise
     */
    public boolean checkOut(String reservationId) {
        Reservation reservation = getReservationById(reservationId);
        
        if (reservation == null || !reservation.getStatus().equals("CHECKED_IN")) {
            return false;
        }
        
        Room room = getRoomById(reservation.getRoomId());
        if (room == null) {
            return false;
        }
        
        // Update room status
        room.updateStatus("CLEANING");
        room.updateLastCleanedDate(new SimpleDateFormat("yyyy-MM-dd").format(new Date()));
        
        // Update reservation status
        reservation.updateStatus("CHECKED_OUT");
        
        return true;
    }
    
    /**
     * Cancel a reservation
     * @param reservationId Reservation ID
     * @return Cancellation fee
     */
    public double cancelReservation(String reservationId) {
        Reservation reservation = getReservationById(reservationId);
        
        if (reservation == null || reservation.getStatus().equals("CANCELLED")) {
            return 0.0;
        }
        
        // Calculate cancellation fee based on policy
        double fee = 0.0;
        Date now = new Date();
        Date checkIn = reservation.getCheckInDate();
        long diff = checkIn.getTime() - now.getTime();
        int daysUntilCheckIn = (int) (diff / (24 * 60 * 60 * 1000));
        
        if (daysUntilCheckIn >= 30) {
            fee = 0.0;  // No fee if cancelled 30+ days in advance
        } else if (daysUntilCheckIn >= 14) {
            fee = reservation.getTotalAmount() * 0.25;  // 25% fee if cancelled 14-29 days in advance
        } else if (daysUntilCheckIn >= 7) {
            fee = reservation.getTotalAmount() * 0.5;   // 50% fee if cancelled 7-13 days in advance
        } else if (daysUntilCheckIn >= 1) {
            fee = reservation.getTotalAmount() * 0.75;   // 75% fee if cancelled 1-6 days in advance
        } else {
            fee = reservation.getTotalAmount();  // Full fee if cancelled on or after check-in date
        }
        
        // Cancel reservation
        reservation.cancel(fee);
        
        return fee;
    }
    
    /**
     * Process payment for a reservation
     * @param reservationId Reservation ID
     * @param paymentMethod Payment method
     * @return True if payment processed successfully, false otherwise
     */
    public boolean processPayment(String reservationId, String paymentMethod) {
        Reservation reservation = getReservationById(reservationId);
        
        if (reservation == null) {
            return false;
        }
        
        // Update payment information
        reservation.setPaymentMethod(paymentMethod);
        reservation.updatePaymentStatus("PAID");
        
        return true;
    }
    
    /**
     * Generate hotel report
     * @return Hotel report string
     */
    public String generateHotelReport() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("=====================================\n");
        sb.append("         HOTEL REPORT\n");
        sb.append("=====================================\n");
        sb.append("Hotel: ").append(hotelName).append("\n");
        sb.append("Code: ").append(hotelCode).append("\n");
        sb.append("Address: ").append(address).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Report Date: ").append(sdf.format(new Date())).append("\n\n");
        
        // Room statistics
        sb.append("ROOM STATISTICS\n");
        sb.append("---------------\n");
        sb.append("Total Rooms: ").append(rooms.size()).append("\n");
        
        Map<String, Integer> roomsByType = new HashMap<>();
        Map<String, Integer> roomsByStatus = new HashMap<>();
        int totalCapacity = 0;
        
        for (Room room : rooms) {
            String type = room.getRoomType();
            roomsByType.put(type, roomsByType.getOrDefault(type, 0) + 1);
            
            String status = room.getStatus();
            roomsByStatus.put(status, roomsByStatus.getOrDefault(status, 0) + 1);
            
            totalCapacity += room.getCapacity();
        }
        
        sb.append("Total Capacity: ").append(totalCapacity).append(" guests\n");
        
        sb.append("\nRooms by Type:\n");
        for (Map.Entry<String, Integer> entry : roomsByType.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        sb.append("\nRooms by Status:\n");
        for (Map.Entry<String, Integer> entry : roomsByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Guest statistics
        sb.append("\nGUEST STATISTICS\n");
        sb.append("-----------------\n");
        sb.append("Total Guests: ").append(guests.size()).append("\n");
        
        Map<String, Integer> guestsByStatus = new HashMap<>();
        int vipCount = 0;
        
        for (Guest guest : guests) {
            String status = guest.getStatus();
            guestsByStatus.put(status, guestsByStatus.getOrDefault(status, 0) + 1);
            
            if (guest.getStatus().equals("VIP")) {
                vipCount++;
            }
        }
        
        sb.append("VIP Guests: ").append(vipCount).append("\n");
        
        sb.append("\nGuests by Status:\n");
        for (Map.Entry<String, Integer> entry : guestsByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Reservation statistics
        sb.append("\nRESERVATION STATISTICS\n");
        sb.append("-----------------------\n");
        sb.append("Total Reservations: ").append(reservations.size()).append("\n");
        
        Map<String, Integer> reservationsByStatus = new HashMap<>();
        double totalRevenue = 0.0;
        int totalNights = 0;
        
        for (Reservation reservation : reservations) {
            String status = reservation.getStatus();
            reservationsByStatus.put(status, reservationsByStatus.getOrDefault(status, 0) + 1);
            
            if (reservation.getStatus().equals("CHECKED_OUT")) {
                totalRevenue += reservation.getTotalAmount();
                totalNights += reservation.calculateNights();
            }
        }
        
        sb.append("Total Revenue: ").append("$").append(String.format("%.2f", totalRevenue)).append("\n");
        sb.append("Total Nights Booked: ").append(totalNights).append("\n");
        if (totalNights > 0) {
            sb.append("Average Daily Rate: ").append("$").append(String.format("%.2f", totalRevenue / totalNights)).append("\n");
        }
        
        sb.append("\nReservations by Status:\n");
        for (Map.Entry<String, Integer> entry : reservationsByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Staff statistics
        sb.append("\nSTAFF STATISTICS\n");
        sb.append("----------------\n");
        sb.append("Total Staff: ").append(staff.size()).append("\n");
        
        Map<String, Integer> staffByDepartment = new HashMap<>();
        Map<String, Integer> staffByStatus = new HashMap<>();
        double totalSalaryCost = 0.0;
        
        for (Staff staffMember : staff) {
            String department = staffMember.getDepartment();
            staffByDepartment.put(department, staffByDepartment.getOrDefault(department, 0) + 1);
            
            String status = staffMember.getStatus();
            staffByStatus.put(status, staffByStatus.getOrDefault(status, 0) + 1);
            
            totalSalaryCost += staffMember.getSalary();
        }
        
        sb.append("Total Monthly Salary Cost: ").append("$").append(String.format("%.2f", totalSalaryCost)).append("\n");
        
        sb.append("\nStaff by Department:\n");
        for (Map.Entry<String, Integer> entry : staffByDepartment.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        sb.append("\nStaff by Status:\n");
        for (Map.Entry<String, Integer> entry : staffByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Occupancy rate for the current month
        Calendar cal = Calendar.getInstance();
        int year = cal.get(Calendar.YEAR);
        int month = cal.get(Calendar.MONTH);
        
        cal.set(year, month, 1, 0, 0, 0);
        Date monthStart = cal.getTime();
        cal.add(Calendar.MONTH, 1);
        Date monthEnd = cal.getTime();
        
        int totalPossibleNights = 0;
        int occupiedNights = 0;
        
        for (Reservation reservation : reservations) {
            if (reservation.getStatus().equals("CHECKED_OUT")) {
                // Calculate nights that fall within current month
                Date resStart = reservation.getCheckInDate();
                Date resEnd = reservation.getCheckOutDate();
                
                // Calculate overlap with current month
                Date start = resStart.before(monthStart) ? monthStart : resStart;
                Date end = resEnd.after(monthEnd) ? monthEnd : resEnd;
                
                if (start.before(end)) {
                    long diff = end.getTime() - start.getTime();
                    int nights = (int) (diff / (24 * 60 * 60 * 1000));
                    occupiedNights += nights;
                }
            }
        }
        
        // Calculate total possible nights
        for (Room room : rooms) {
            long diff = monthEnd.getTime() - monthStart.getTime();
            int daysInMonth = (int) (diff / (24 * 60 * 60 * 1000));
            totalPossibleNights += daysInMonth;
        }
        
        double occupancyRate = totalPossibleNights > 0 ? 
                           (double) occupiedNights / totalPossibleNights * 100 : 0.0;
        
        sb.append("\nCURRENT MONTH OCCUPANCY RATE\n");
        sb.append("------------------------------\n");
        sb.append("Occupancy Rate: ").append(String.format("%.1f", occupancyRate)).append("%\n");
        
        return sb.toString();
    }
    
    /**
     * Save hotel data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("HOTEL DATA EXPORT");
            writer.println("==================");
            writer.println("Hotel: " + hotelName);
            writer.println("Code: " + hotelCode);
            writer.println("Address: " + address);
            writer.println("Phone: " + phone);
            writer.println("Email: " + email);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export rooms
            writer.println("ROOMS");
            writer.println("-----");
            for (Room room : rooms) {
                writer.println(room.getSummary());
                writer.println();
            }
            
            // Export guests
            writer.println("GUESTS");
            writer.println("------");
            for (Guest guest : guests) {
                writer.println(guest.getSummary());
                writer.println();
            }
            
            // Export reservations
            writer.println("RESERVATIONS");
            writer.println("------------");
            for (Reservation reservation : reservations) {
                writer.println(reservation.getSummary());
                writer.println();
            }
            
            // Export staff
            writer.println("STAFF");
            writer.println("-----");
            for (Staff staffMember : staff) {
                writer.println(staffMember.getSummary());
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
        // Create a new hotel management system
        HotelManagementSystem hotel = new HotelManagementSystem(
            "Grand Plaza Hotel", "GPH001",
            "123 Main St, City, State, Country", "(555) 123-4567", "info@grandplaza.com"
        );
        
        // Create rooms
        Room room1 = new Room("R001", "101", "STANDARD", "1",
                              2, 120.0, "USD", "Double Bed", "City View");
        room1.addAmenity("Free Wi-Fi");
        room1.addAmenity("Mini Bar");
        room1.addAmenity("Air Conditioning");
        room1.addAmenity("Flat-screen TV");
        room1.updateSize(25.0);
        room1.addImage("https://example.com/room101-1.jpg");
        room1.addImage("https://example.com/room101-2.jpg");
        
        Room room2 = new Room("R002", "201", "DELUXE", "2",
                              2, 180.0, "USD", "King Bed", "Ocean View");
        room2.addAmenity("Free Wi-Fi");
        room2.addAmenity("Mini Bar");
        room2.addAmenity("Air Conditioning");
        room2.addAmenity("Flat-screen TV");
        room2.addAmenity("Balcony");
        room2.addAmenity("Coffee Maker");
        room2.updateSize(35.0);
        room2.setPetFriendly(true);
        room2.addImage("https://example.com/room201-1.jpg");
        room2.addImage("https://example.com/room201-2.jpg");
        room2.addImage("https://example.com/room201-3.jpg");
        
        Room room3 = new Room("R003", "301", "SUITE", "3",
                              4, 350.0, "USD", "King Bed + Sofa Bed", "Ocean View");
        room3.addAmenity("Free Wi-Fi");
        room3.addAmenity("Mini Bar");
        room3.addAmenity("Air Conditioning");
        room3.addAmenity("Flat-screen TV");
        room3.addAmenity("Balcony");
        room3.addAmenity("Coffee Maker");
        room3.addAmenity("Living Room");
        room3.addAmenity("Kitchenette");
        room3.addAmenity("Jacuzzi");
        room3.updateSize(60.0);
        room3.setPetFriendly(true);
        room3.addImage("https://example.com/suite301-1.jpg");
        room3.addImage("https://example.com/suite301-2.jpg");
        room3.addImage("https://example.com/suite301-3.jpg");
        
        Room room4 = new Room("R004", "102", "STANDARD", "1",
                              2, 130.0, "USD", "Twin Beds", "Garden View");
        room4.addAmenity("Free Wi-Fi");
        room4.addAmenity("Air Conditioning");
        room4.addAmenity("Flat-screen TV");
        room4.updateSize(25.0);
        room4.setSmoking(true);
        room4.addImage("https://example.com/room102-1.jpg");
        room4.addImage("https://example.com/room102-2.jpg");
        
        Room room5 = new Room("R005", "401", "PRESIDENTIAL", "4",
                              6, 600.0, "USD", "King Bed + Sofa Bed + Twin Beds", "Ocean View");
        room5.addAmenity("Free Wi-Fi");
        room5.addAmenity("Mini Bar");
        room5.addAmenity("Air Conditioning");
        room5.addAmenity("Flat-screen TV");
        room5.addAmenity("Balcony");
        room5.addAmenity("Coffee Maker");
        room5.addAmenity("Living Room");
        room5.addAmenity("Kitchen");
        room5.addAmenity("Dining Room");
        room5.addAmenity("Jacuzzi");
        room5.addAmenity("Butler Service");
        room5.updateSize(120.0);
        room5.setPetFriendly(true);
        room5.addImage("https://example.com/pres401-1.jpg");
        room5.addImage("https://example.com/pres401-2.jpg");
        room5.addImage("https://example.com/pres401-3.jpg");
        room5.addImage("https://example.com/pres401-4.jpg");
        
        // Add rooms to hotel
        hotel.addRoom(room1);
        hotel.addRoom(room2);
        hotel.addRoom(room3);
        hotel.addRoom(room4);
        hotel.addRoom(room5);
        
        // Create guests
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        Guest guest1 = new Guest("G001", "John", "Doe", "john.doe@email.com",
                                  "(555) 111-2222", "Driver's License", "D12345678",
                                  sdf.parse("1980-05-15"));
        guest1.updateAddress("123 Main St");
        guest1.updateCity("Anytown");
        guest1.updateState("California");
        guest1.updatePostalCode("12345");
        guest1.updateCountry("USA");
        guest1.updateStatus("VIP");
        guest1.addPreference("High Floor");
        guest1.addPreference("Room Away from Elevator");
        guest1.addSpecialRequest("Extra Towels");
        guest1.setMembership("Marriott Bonvoy", "12345678");
        guest1.setEmergencyContact("Jane Doe", "(555) 999-8888");
        guest1.setDietaryRestrictions("None");
        
        Guest guest2 = new Guest("G002", "Jane", "Smith", "jane.smith@email.com",
                                  "(555) 333-4444", "Passport", "A987654321",
                                  sdf.parse("1985-08-20"));
        guest2.updateAddress("456 Oak Ave");
        guest2.updateCity("Otherville");
        guest2.updateState("New York");
        guest2.updatePostalCode("54321");
        guest2.updateCountry("USA");
        guest2.addPreference("Non-Smoking Room");
        guest2.addSpecialRequest("Late Check-out");
        guest2.setDietaryRestrictions("Vegetarian");
        
        Guest guest3 = new Guest("G003", "Robert", "Johnson", "robert.johnson@email.com",
                                  "(555) 555-6666", "Driver's License", "B56789012",
                                  sdf.parse("1975-11-10"));
        guest3.updateAddress("789 Pine Rd");
        guest3.updateCity("Smalltown");
        guest3.updateState("Texas");
        guest3.updatePostalCode("98765");
        guest3.updateCountry("USA");
        guest3.addPreference("King Bed");
        guest3.addPreference("Early Check-in");
        guest3.setSmoking(true);
        guest3.setDietaryRestrictions("Gluten-Free");
        
        Guest guest4 = new Guest("G004", "Emily", "Williams", "emily.williams@email.com",
                                  "(555) 777-8888", "Passport", "C345678901",
                                  sdf.parse("1990-03-25"));
        guest4.updateAddress("321 Elm Blvd");
        guest4.updateCity("Big City");
        guest4.updateState("Florida");
        guest4.updatePostalCode("13579");
        guest4.updateCountry("USA");
        guest4.addPreference("Pool View");
        guest4.addSpecialRequest("Extra Pillows");
        guest4.addSpecialRequest("Connecting Room");
        guest4.setDietaryRestrictions("Nut Allergy");
        
        Guest guest5 = new Guest("G005", "Michael", "Brown", "michael.brown@email.com",
                                  "(555) 999-0000", "Driver's License", "D23456789",
                                  sdf.parse("1972-07-08"));
        guest5.updateAddress("654 Maple Dr");
        guest5.updateCity("Coastal City");
        guest5.updateState("California");
        guest5.updatePostalCode("24680");
        guest5.updateCountry("USA");
        guest5.addPreference("Suite");
        guest5.addSpecialRequest("Pet-Friendly Room");
        guest5.setMembership("Hilton Honors", "87654321");
        guest5.setEmergencyContact("Lisa Brown", "(555) 111-2222");
        guest5.setDietaryRestrictions("None");
        
        // Add guests to hotel
        hotel.addGuest(guest1);
        hotel.addGuest(guest2);
        hotel.addGuest(guest3);
        hotel.addGuest(guest4);
        hotel.addGuest(guest5);
        
        // Create reservations
        try {
            Date today = sdf.parse("2023-06-15");
            Date tomorrow = sdf.parse("2023-06-16");
            Date nextWeek = sdf.parse("2023-06-22");
            Date nextWeekPlus5 = sdf.parse("2023-06-27");
            
            String reservation1 = hotel.makeReservation("G001", "R001", today, nextWeek, 2, 0);
            String reservation2 = hotel.makeReservation("G002", "R002", tomorrow, nextWeekPlus5, 2, 1);
            String reservation3 = hotel.makeReservation("G003", "R004", sdf.parse("2023-07-01"), sdf.parse("2023-07-05"), 1, 0);
            String reservation4 = hotel.makeReservation("G004", "R005", sdf.parse("2023-08-10"), sdf.parse("2023-08-15"), 2, 2);
            
            // Process payment for some reservations
            if (reservation1 != null) {
                hotel.processPayment(reservation1, "Credit Card");
                hotel.checkIn(reservation1);
            }
            
            if (reservation2 != null) {
                hotel.processPayment(reservation2, "PayPal");
            }
            
            if (reservation3 != null) {
                hotel.processPayment(reservation3, "Cash");
            }
            
            // Create staff
            Staff staff1 = new Staff("S001", "David", "Wilson", "david.wilson@hotel.com",
                                      "(555) 123-9876", "Front Desk Manager", "FRONT_DESK",
                                      sdf.parse("2015-03-15"), 3500.0, "USD", "Mon-Fri, 9AM-5PM");
            staff1.addSkill("Customer Service");
            staff1.addSkill("Hotel Management");
            staff1.addCertification("Front Desk Operations");
            staff1.setEmergencyContact("Laura Wilson", "(555) 987-6543");
            
            Staff staff2 = new Staff("S002", "Sarah", "Johnson", "sarah.johnson@hotel.com",
                                      "(555) 234-5678", "Housekeeping Supervisor", "HOUSEKEEPING",
                                      sdf.parse("2018-06-20"), 2800.0, "USD", "Mon-Fri, 8AM-4PM");
            staff2.addSkill("Housekeeping");
            staff2.addSkill("Team Management");
            staff2.setEmergencyContact("Tom Johnson", "(555) 876-5432");
            
            Staff staff3 = new Staff("S003", "Robert", "Miller", "robert.miller@hotel.com",
                                      "(555) 345-6789", "Maintenance Technician", "MAINTENANCE",
                                      sdf.parse("2019-09-10"), 2500.0, "USD", "Shift Work");
            staff3.addSkill("HVAC Maintenance");
            staff3.addSkill("Electrical Work");
            staff3.addSkill("Plumbing");
            staff3.addCertification("HVAC Technician");
            staff3.setEmergencyContact("Susan Miller", "(555) 765-4321");
            
            Staff staff4 = new Staff("S004", "Emily", "Davis", "emily.davis@hotel.com",
                                      "(555) 456-7890", "Restaurant Manager", "FOOD_AND_BEVERAGE",
                                      sdf.parse("2017-04-05"), 3200.0, "USD", "Varied Hours");
            staff4.addSkill("Restaurant Management");
            staff4.addSkill("Food & Beverage");
            staff4.addCertification("Food Safety");
            staff4.setEmergencyContact("James Davis", "(555) 654-3210");
            
            Staff staff5 = new Staff("S005", "Michael", "Brown", "michael.brown@hotel.com",
                                      "(555) 567-8901", "General Manager", "MANAGEMENT",
                                      sdf.parse("2012-11-12"), 6000.0, "USD", "Mon-Fri, 8AM-6PM");
            staff5.addSkill("Hotel Management");
            staff5.addSkill("Business Development");
            staff5.addSkill("Team Leadership");
            staff5.addCertification("Hospitality Management");
            staff5.setEmergencyContact("Anna Brown", "(555) 543-2109");
            
            // Add staff to hotel
            hotel.addStaff(staff1);
            hotel.addStaff(staff2);
            hotel.addStaff(staff3);
            hotel.addStaff(staff4);
            hotel.addStaff(staff5);
            
            // Print hotel information
            System.out.println("Hotel Management System");
            System.out.println("=======================");
            System.out.println("Hotel: " + hotel.getHotelName());
            System.out.println("Code: " + hotel.getHotelCode());
            System.out.println("Address: " + hotel.getAddress());
            System.out.println("Phone: " + hotel.getPhone());
            System.out.println("Email: " + hotel.getEmail());
            System.out.println("Total Rooms: " + hotel.getRooms().size());
            System.out.println("Total Guests: " + hotel.getGuests().size());
            System.out.println("Total Reservations: " + hotel.getReservations().size());
            System.out.println("Total Staff: " + hotel.getStaff().size());
            System.out.println();
            
            // Print room summaries
            System.out.println("Room Summaries");
            System.out.println("===============");
            for (Room room : hotel.getRooms()) {
                System.out.println(room.getSummary());
                System.out.println();
            }
            
            // Print guest summaries
            System.out.println("Guest Summaries");
            System.out.println("================");
            for (Guest guest : hotel.getGuests()) {
                System.out.println(guest.getSummary());
                System.out.println();
            }
            
            // Print reservation summaries
            System.out.println("Reservation Summaries");
            System.out.println("=======================");
            for (Reservation reservation : hotel.getReservations()) {
                System.out.println(reservation.getSummary());
                System.out.println();
            }
            
            // Print staff summaries
            System.out.println("Staff Summaries");
            System.out.println("================");
            for (Staff staffMember : hotel.getStaff()) {
                System.out.println(staffMember.getSummary());
                System.out.println();
            }
            
            // Check out a reservation
            if (reservation1 != null) {
                System.out.println("Checking out reservation " + reservation1);
                hotel.checkOut(reservation1);
            }
            
            // Cancel a reservation
            if (reservation2 != null) {
                System.out.println("Cancelling reservation " + reservation2);
                double cancellationFee = hotel.cancelReservation(reservation2);
                System.out.println("Cancellation fee: $" + String.format("%.2f", cancellationFee));
            }
            
            // Generate and print hotel report
            String report = hotel.generateHotelReport();
            System.out.println("\n" + report);
            
            // Save data to file
            hotel.saveToFile("hotel_data_export.txt");
            
            System.out.println("\nHotel Management System demonstration completed successfully!");
            
        } catch (ParseException e) {
            System.err.println("Date parsing error: " + e.getMessage());
        }
    }
}
<｜fim▁hole｜>