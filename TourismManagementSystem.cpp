#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;

// Global variables for destinations and packages
const int MAX_DESTINATIONS = 10;
const int MAX_BOOKINGS = 50;

struct Destination {
    string name; // Name of the destination
    string description; //  description
    double price; // price
    string available_packages[4]; // Array of available packages
};

// Structure for storing booking information
struct Booking {
    int booking_id; // Unique booking identifier
    string customer_name; // Customer's name
    string destination; // Chosen destination
    string package; // Selected package
    int num_travelers; // Number of travelers
    double total_cost; // Total booking cost
};

// Global arrays
Destination destinations[MAX_DESTINATIONS] = {
    {"Paris", "City of Love", 1200.00, {"Basic", "Premium", "Luxury", "All-Inclusive"}},
    {"Tokyo", "Modern Metropolis", 1500.00, {"Cultural", "Adventure", "Food Tour", "Complete"}},
    {"Bali", "Tropical Paradise", 800.00, {"Beach", "Nature", "Spiritual", "Family"}},
    {"New York", "The Big Apple", 1000.00, {"City Tour", "Shopping", "Arts", "Entertainment"}},
    {"Safari", "African Adventure", 2000.00, {"Wildlife", "Camping", "Photography", "Expedition"}}
};

// Array to store bookings
Booking bookings[MAX_BOOKINGS];
int current_booking_count = 0;

// Function declarations
void displayMenu();
void login();
void viewDestinations();
void makeBooking();
void viewBookings();
void cancelBooking();
void saveBookingToFile(const Booking& booking);
void loadBookingsFromFile();

// Login functionality
void login() {
    string username, password;
    int attempts = 0;
    const string CORRECT_USERNAME = "admin";
    const string CORRECT_PASSWORD = "1234";

    do {
            // Clear screen and prompt for credentials
        system("cls");
        cout << "\n============= TOURISM MANAGEMENT SYSTEM LOGIN =============\n";
        cout << "\nUsername: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        // Check credentials
        if (username == CORRECT_USERNAME && password == CORRECT_PASSWORD) {
            cout << "\nLogin successful! Welcome to Tourism Management System.";
            system("pause");
            return; // if true
        } else {
            cout << "\nInvalid username or password! Please try again.";
            attempts++; // if false
        }

        // Exit after 3 failed attempts
        if (attempts == 3) {
            cout << "\nToo many failed attempts. System will now exit.";
            exit(1);
        }
    } while (true);
}

// Display available destinations
void viewDestinations() {
    system("cls"); // Clear screen
    cout << "\n============= AVAILABLE DESTINATIONS =============\n";

    // Loop through destinations array
    for (int i = 0; i < MAX_DESTINATIONS && !destinations[i].name.empty(); i++) {
            // Display destination details
        cout << "\n[" << i + 1 << "] " << destinations[i].name;
        cout << "\nDescription: " << destinations[i].description;
        cout << "\nBase Price: $" << destinations[i].price;
        // Display available packages
        cout << "\nAvailable Packages:";
        for (int j = 0; j < 4; j++) {
            cout << "\n   - " << destinations[i].available_packages[j];
        }
        cout << "\n------------------------------------------";
    }
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// Make a new booking
void makeBooking() {
    system("cls");
    cout << "\n============= MAKE A NEW BOOKING =============\n";

    // Check if booking limit reached
    if (current_booking_count >= MAX_BOOKINGS) {
        cout << "\nBooking system is currently full. Please try again later.";
        return;
    }

    // Get customer details
    Booking new_booking;
    new_booking.booking_id = current_booking_count + 1;

    cout << "\nEnter customer name: ";
    cin.ignore();
    getline(cin, new_booking.customer_name);

    // Show available destinations
    viewDestinations();

    int dest_choice;
    cout << "\nSelect destination number: ";
    cin >> dest_choice;

    // Validate choice and set destination
    if (dest_choice < 1 || dest_choice > MAX_DESTINATIONS) {
        cout << "\nInvalid destination choice!";
        return;
    }

    new_booking.destination = destinations[dest_choice - 1].name;

    // Show available packages
    cout << "\nAvailable packages for " << new_booking.destination << ":";
    for (int i = 0; i < 4; i++) {
        cout << "\n[" << i + 1 << "] " << destinations[dest_choice - 1].available_packages[i];
    }

    int package_choice;
    cout << "\nSelect package number: ";
    cin >> package_choice;

    if (package_choice < 1 || package_choice > 4) {
        cout << "\nInvalid package choice!";
        return;
    }

    new_booking.package = destinations[dest_choice - 1].available_packages[package_choice - 1];

    cout << "\nEnter number of travelers: ";
    cin >> new_booking.num_travelers;

    // Calculate total cost (base price * number of travelers + package premium)
    double package_premium = (package_choice - 1) * 200.00; // Each upgrade adds $200
    new_booking.total_cost = (destinations[dest_choice - 1].price + package_premium) * new_booking.num_travelers;

    // Save booking
    bookings[current_booking_count] = new_booking;
    current_booking_count++;
    saveBookingToFile(new_booking);

    cout << "\nBooking confirmed!";
    cout << "\nTotal cost: $" << new_booking.total_cost;
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// View all bookings
void viewBookings() {
    system("cls");
    cout << "\n============= CURRENT BOOKINGS =============\n";

    if (current_booking_count == 0) {
        cout << "\nNo bookings found.";
        cout << "\nPress any key to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    for (int i = 0; i < current_booking_count; i++) {
        cout << "\nBooking ID: " << bookings[i].booking_id;
        cout << "\nCustomer: " << bookings[i].customer_name;
        cout << "\nDestination: " << bookings[i].destination;
        cout << "\nPackage: " << bookings[i].package;
        cout << "\nTravelers: " << bookings[i].num_travelers;
        cout << "\nTotal Cost: $" << bookings[i].total_cost;
        cout << "\n------------------------------------------";
    }

    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// Cancel a booking
void cancelBooking() {
    system("cls");
    cout << "\n============= CANCEL BOOKING =============\n";

    int booking_id;
    cout << "\nEnter booking ID to cancel: ";
    cin >> booking_id;

    bool found = false;
    for (int i = 0; i < current_booking_count; i++) {
        if (bookings[i].booking_id == booking_id) {
            // Shift remaining bookings
            for (int j = i; j < current_booking_count - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            current_booking_count--;
            found = true;
            cout << "\nBooking cancelled successfully!";
            break;
        }
    }

    if (!found) {
        cout << "\nBooking not found!";
    }

    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// Save booking to file
void saveBookingToFile(const Booking& booking) {
    ofstream file("bookings.txt", ios::app);
    if (file.is_open()) {
        file << booking.booking_id << ","
             << booking.customer_name << ","
             << booking.destination << ","
             << booking.package << ","
             << booking.num_travelers << ","
             << booking.total_cost << "\n";
        file.close();
    }
}

// Load bookings from file
void loadBookingsFromFile() {
    ifstream file("bookings.txt");
    string line;

    while (getline(file, line) && current_booking_count < MAX_BOOKINGS) {
        size_t pos = 0;
        string token;
        string delimiter = ",";
        Booking booking;

        // Parse booking ID
        pos = line.find(delimiter);
        booking.booking_id = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        // Parse customer name
        pos = line.find(delimiter);
        booking.customer_name = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Parse destination
        pos = line.find(delimiter);
        booking.destination = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Parse package
        pos = line.find(delimiter);
        booking.package = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Parse number of travelers
        pos = line.find(delimiter);
        booking.num_travelers = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        // Parse total cost
        booking.total_cost = stod(line);

        bookings[current_booking_count++] = booking;
    }

    file.close();
}

// Main menu
void displayMenu() {
    int choice;
    do {
        system("cls");
        cout << "\n============= TOURISM MANAGEMENT SYSTEM =============\n";
        cout << "\n[1] View Destinations";
        cout << "\n[2] Make Booking";
        cout << "\n[3] View Bookings";
        cout << "\n[4] Cancel Booking";
        cout << "\n[5] Exit";
        cout << "\n\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewDestinations();
                break;
            case 2:
                makeBooking();
                break;
            case 3:
                viewBookings();
                break;
            case 4:
                cancelBooking();
                break;
            case 5:
                cout << "\nThank you for using Tourism Management System!\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.";
                cin.ignore();
                cin.get();
        }
    } while (choice != 5);
}

int main() {
    loadBookingsFromFile();
    login();
    displayMenu();
    return 0;
}

