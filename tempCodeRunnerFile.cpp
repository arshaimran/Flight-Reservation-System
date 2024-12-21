
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <thread> // Required for sleep_for
#include <chrono> // Required for chrono

using namespace std;

// To store payment history for auditing purposes
unordered_map<string, string> paymentHistory;

unordered_map<string, pair<string, string>> users; // stores email, password pair;

void addUser(const string &role);
void removeUser();
void mainMenu(FlightBST &flightBST, BookingLinkedList &bookingList);

struct Passenger
{
    string name;
    int flightID;
    string flightDate;
    string flightTime;
    double fare;
    bool isPaid;          // To track whether the passenger has paid
    double paymentAmount; // The actual amount paid by the passenger

    Passenger(string _name, int _flightID, string _flightDate, string _flightTime, double _fare)
        : name(_name), flightID(_flightID), flightDate(_flightDate), flightTime(_flightTime), fare(_fare), isPaid(false), paymentAmount(0) {}
};

struct PassengerNode
{
    Passenger passenger; // The actual passenger data
    PassengerNode *next; // Pointer to the next node in the list

    PassengerNode(Passenger _passenger) : passenger(_passenger), next(nullptr) {}
};

class BookingLinkedList
{
private:
    PassengerNode *head;

public:
    BookingLinkedList() : head(nullptr) {}

    // Add a new booking to the linked list
    void addBooking(const Passenger &passenger)
    {
        PassengerNode *newNode = new PassengerNode(passenger);
        if (!head)
        {
            head = newNode;
        }
        else
        {
            PassengerNode *current = head;
            while (current->next)
            {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Display all bookings
    void displayBookingsWithPayments()
    {
        if (!head)
        {
            cout << "No bookings found.\n";
            return;
        }

        PassengerNode *current = head;
        while (current)
        {
            // Accessing Passenger object fields through current->passenger
            cout << "Passenger: " << current->passenger.name
                 << ", Flight ID: " << current->passenger.flightID
                 << ", Date: " << current->passenger.flightDate
                 << ", Time: " << current->passenger.flightTime
                 << ", Fare: " << current->passenger.fare << " PKR"
                 << ", Paid: " << (current->passenger.isPaid ? "Yes" : "No")
                 << ", Amount Paid: " << current->passenger.paymentAmount << " PKR\n";
            current = current->next;
        }
    }
};

struct Flight
{
    int flightID;
    string origin;
    string destination;
    string date;
    string time;
    double fare;
    int availableSeats;

    Flight() : flightID(0), fare(0.0), availableSeats(0) {}
    Flight(int id, string o, string d, string da, string t, double f, int s)
        : flightID(id), origin(o), destination(d), date(da), time(t), fare(f), availableSeats(s) {}
};

// Node for BST
struct BSTNode
{
    Flight flight;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Flight f) : flight(f), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for flight management
class FlightBST
{
private:
    BSTNode *root;
    int flightIDCounter;

    // Insert into BST
    BSTNode *insert(BSTNode *node, Flight flight)
    {
        if (!node)
            return new BSTNode(flight);
        if (flight.time < node->flight.time)
            node->left = insert(node->left, flight);
        else
            node->right = insert(node->right, flight);
        return node;
    }

    // In-order traversal to display all flights
    void inOrder(BSTNode *node)
    {
        if (!node)
            return;
        inOrder(node->left);
        displayFlightDetails(node->flight);
        inOrder(node->right);
    }

    // Helper to display flight details
    void displayFlightDetails(const Flight &flight)
    {
        cout << "Flight ID: " << flight.flightID << ", Origin: " << flight.origin
             << ", Destination: " << flight.destination << ", Date: " << flight.date
             << ", Time: " << flight.time << ", Fare: RS" << flight.fare
             << ", Available Seats: " << flight.availableSeats << endl;
    }

    // Helper to search for flights based on criteria
    void searchFlightsHelper(BSTNode *node, const string &origin, const string &destination, const string &date)
    {
        if (!node)
            return;

        if ((origin.empty() || node->flight.origin == origin) &&
            (destination.empty() || node->flight.destination == destination) &&
            (date.empty() || node->flight.date == date))
        {
            displayFlightDetails(node->flight);
        }

        searchFlightsHelper(node->left, origin, destination, date);
        searchFlightsHelper(node->right, origin, destination, date);
    }

    bool bookSeatsHelper(BSTNode *node, int flightID, int numSeats)
    {
        if (!node)
            return false;

        if (node->flight.flightID == flightID)
        {
            if (node->flight.availableSeats >= numSeats)
            {
                node->flight.availableSeats -= numSeats;
                return true;
            }
            else
            {
                cout << "Not enough seats available on Flight ID: " << flightID << endl;
                return false;
            }
        }

        return bookSeatsHelper(node->left, flightID, numSeats) ||
               bookSeatsHelper(node->right, flightID, numSeats);
    }

public:
    FlightBST() : root(nullptr), flightIDCounter(1) {}

    // Insert a flight into BST
    void insertFlight(const Flight &flight)
    {
        root = insert(root, flight);
    }

    // Display all available flights
    void displayAllFlights()
    {
        if (!root)
        {
            cout << "No flights available.\n";
            return;
        }
        cout << "All Available Flights:\n";
        inOrder(root);
    }

    // Add flight to the BST with auto-generated ID
    void addFlight(string origin, string destination, string date, string time, double fare, int seats)
    {
        Flight newFlight(flightIDCounter++, origin, destination, date, time, fare, seats);
        insertFlight(newFlight);
    }

    // Search for flights based on criteria
    void searchFlights(const string &origin, const string &destination, const string &date)
    {
        cout << "Search Results:\n";
        searchFlightsHelper(root, origin, destination, date);
    }

    bool bookSeats(int flightID, int numSeats)
    {
        return bookSeatsHelper(root, flightID, numSeats);
    }

    bool updateFlight(int flightID, const string &newTime, double newFare, int newSeats)
    {
        BSTNode *node = root;
        while (node)
        {
            if (node->flight.flightID == flightID)
            {
                node->flight.time = newTime;
                node->flight.fare = newFare;
                node->flight.availableSeats = newSeats;
                return true;
            }
            else if (flightID < node->flight.flightID)
                node = node->left;
            else
                node = node->right;
        }
        return false; // Flight not found
    }

    BSTNode *removeFlight(BSTNode *node, int flightID)
    {
        if (!node)
            return node;

        if (flightID < node->flight.flightID)
            node->left = removeFlight(node->left, flightID);
        else if (flightID > node->flight.flightID)
            node->right = removeFlight(node->right, flightID);
        else
        {
            if (!node->left)
            {
                BSTNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                BSTNode *temp = node->left;
                delete node;
                return temp;
            }

            BSTNode *temp = findMin(node->right);
            node->flight = temp->flight;
            node->right = removeFlight(node->right, temp->flight.flightID);
        }
        return node;
    }

    Flight getFlightByID(int flightID)
    {
        BSTNode *node = root;
        while (node)
        {
            if (node->flight.flightID == flightID)
            {
                return node->flight;
            }
            else if (flightID < node->flight.flightID)
                node = node->left;
            else
                node = node->right;
        }

        // If the flight is not found, we can throw an exception or return a default Flight object.
        throw runtime_error("Flight not found");
    }

    // Helper to find the minimum node
    BSTNode *findMin(BSTNode *node)
    {
        while (node && node->left != nullptr)
            node = node->left;
        return node;
    }

    // Public function to remove a flight
    bool removeFlight(int flightID)
    {
        root = removeFlight(root, flightID);
        return true;
    }
};

// Function to process payment (using the original fare set when adding/updating flights)
bool processPayment(const string &userName, int fare)
{
    string bankName, cardHolder, cardNumber, expiryDate, cvv;

    // Collect payment details
    cout << "Enter Bank Name: ";
    cin.ignore();
    getline(cin, bankName);

    cout << "Enter Cardholder Name: ";
    getline(cin, cardHolder);

    cout << "Enter Card Number (16 digits): ";
    cin >> cardNumber;
    while (cardNumber.length() != 16 || !all_of(cardNumber.begin(), cardNumber.end(), ::isdigit))
    {
        cout << "Invalid card number. Please enter a 16-digit card number: ";
        cin >> cardNumber;
    }

    cout << "Enter Expiry Date (MM/YY): ";
    cin >> expiryDate;

    cout << "Enter CVV (3 digits): ";
    cin >> cvv;
    while (cvv.length() != 3 || !all_of(cvv.begin(), cvv.end(), ::isdigit))
    {
        cout << "Invalid CVV. Please enter a 3-digit CVV: ";
        cin >> cvv;
    }

    // Simulate a delay to process payment
    cout << "Processing payment...\n";
    (std::chrono::seconds(3)); // 3 seconds delay

    // Always make payment successful
    cout << "Payment successful!\n";

    // Save payment history for auditing
    paymentHistory[userName] = "Paid " + to_string(fare) + " PKR.";
    return true;
}

// Booking function to use the original fare for payment
void bookFlight(FlightBST &flightBST, BookingLinkedList &bookingList)
{
    int flightID, numSeats;
    string passengerName;
    vector<string> passengerNames;

    cout << "Enter Flight ID to book: ";
    cin >> flightID;

    cout << "Enter number of seats to book: ";
    cin >> numSeats;

    // Collect passenger names
    for (int i = 1; i <= numSeats; ++i)
    {
        cout << "Enter passenger " << i << "'s name: ";
        cin.ignore();
        getline(cin, passengerName);
        passengerNames.push_back(passengerName);
    }

    // Attempt to book the flight
    if (flightBST.bookSeats(flightID, numSeats))
    {
        cout << "Booking successful! The following passengers are booked on Flight ID: " << flightID << endl;
        for (const auto &name : passengerNames)
        {
            cout << "- " << name << endl;
        }

        // Use the original fare for payment
        Flight bookedFlight = flightBST.getFlightByID(flightID); // Retrieve the flight details
        double fare = bookedFlight.fare;                         // Fare in PKR

        // Add each passenger to the booking linked list
        for (const auto &name : passengerNames)
        {
            Passenger newPassenger(name, flightID, bookedFlight.time, bookedFlight.date, fare);
            bookingList.addBooking(newPassenger);
        }

        cout << "Passengers added to booking list.\n";

        // Proceed to payment
        cout << "Proceeding to payment...\n";
        if (processPayment(passengerNames[0], static_cast<int>(fare))) // Use first passenger for payment
        {
            cout << "Payment and booking successfully completed. Thank you for choosing GIKI Airlines.\n";
        }
    }
    else
    {
        cout << "Booking failed. Please try again.\n";
    }
}

/*void displayPassengerBookings(BookingLinkedList &bookingList)
{
    bookingList.displayBookings();
}*/

// Add default flights
void addDefaultFlights(FlightBST &flightBST)
{
    flightBST.addFlight("Lahore", "Islamabad", "2024-12-15", "08:00", 18400.0, 50);
    flightBST.addFlight("Islamabad", "Karachi", "2024-12-15", "12:00", 45150.0, 60);
    flightBST.addFlight("Karachi", "Lahore", "2024-12-15", "16:00", 67120.0, 40);
}

void mainMenu(FlightBST &flightBST, BookingLinkedList &bookingList)
{
    string roleChoice;
    cout << "Welcome to GIKI Airlines!\n";
    cout << "Please select your role:\n";
    cout << "1. Passenger\n2. Airline Staff\n3. Admin\n";
    cin >> roleChoice;

    if (roleChoice == "1")
    {
        string subChoice;
        cout << "1. Register\n2. Login\nEnter your choice: ";
        cin >> subChoice;

        if (subChoice == "1")
        {
            registerUser(flightBST, bookingList, "Passenger");
        }
        else if (subChoice == "2")
        {
            string emailOut, nameOut;
            if (loginUser(emailOut, nameOut))
                mainMenuPassenger(flightBST, bookingList);
        }
    }
    else if (roleChoice == "2")
    {
        string subChoice;
        cout << "1. Register\n2. Login\nEnter your choice: ";
        cin >> subChoice;

        if (subChoice == "1")
        {
            registerUser(flightBST, bookingList, "Airline Staff");
        }
        else if (subChoice == "2")
        {
            string emailOut, nameOut;
            if (loginUser(emailOut, nameOut))
                mainMenuStaff(flightBST, bookingList);
        }
    }
    else if (roleChoice == "3")
    {
        string subChoice;
        cout << "1. Register\n2. Login\nEnter your choice: ";
        cin >> subChoice;

        if (subChoice == "1")
        {
            registerUser(flightBST, bookingList, "Admin");
        }
        else if (subChoice == "2")
        {
            string emailOut, nameOut;
            if (loginUser(emailOut, nameOut))
                mainMenuAdmin(flightBST, bookingList);
        }
    }
    else
    {
        cout << "Invalid role selected. Exiting...\n";
    }
}

// Main menu for the passenger
void mainMenuPassenger(FlightBST &flightBST, BookingLinkedList &bookingList)
{
    while (true)
    {
        int choice;
        cout << "\nWelcome to GIKI Flights!\n";
        cout << "1. View all available flights\n";
        cout << "2. Search for flights\n";
        cout << "3. Book a flight\n"; // New option
        cout << "4. Return to Main Menu\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            flightBST.displayAllFlights();
            break;
        case 2:
        {
            string origin, destination, date;
            cin.ignore();
            cout << "Enter Origin (or leave empty to skip): ";
            getline(cin, origin);

            cout << "Enter Destination (or leave empty to skip): ";
            getline(cin, destination);

            cout << "Enter Date (YYYY-MM-DD, or leave empty to skip): ";
            getline(cin, date);

            flightBST.searchFlights(origin, destination, date);
            break;
        }
        case 3:
            bookFlight(flightBST, bookingList); // Call the new booking function
            break;

        case 4:
            mainMenu(flightBST, bookingList); // //return to menu
            break;
        case 5:
            cout << "Thank you for using GIKI Airlines. Goodbye!\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
            mainMenu(flightBST, bookingList);
        }
    }
}

void mainMenuStaff(FlightBST &flightBST, BookingLinkedList &bookingList)
{
    while (true)
    {
        int choice;
        cout << "\nWelcome, Airline Staff!\n";
        cout << "1. View all available flights\n";
        cout << "2. Add a new flight\n";
        cout << "3. Update a flight\n";
        cout << "4. Remove a flight\n";
        cout << "5. Return to Main Menu\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            flightBST.displayAllFlights(); // Reuse existing function to display flights
            break;
        case 2:
        {
            string origin, destination, date, time;
            double fare;
            int seats;

            cout << "Enter Origin: ";
            cin.ignore();
            getline(cin, origin);

            cout << "Enter Destination: ";
            getline(cin, destination);

            cout << "Enter Date (YYYY-MM-DD): ";
            getline(cin, date);

            cout << "Enter Time (HH:MM): ";
            getline(cin, time);

            cout << "Enter Fare: ";
            cin >> fare;

            cout << "Enter Available Seats: ";
            cin >> seats;

            flightBST.addFlight(origin, destination, date, time, fare, seats);
            cout << "Flight added successfully!\n";
            break;
        }
        case 3:
        {
            int flightID;
            string newTime;
            double newFare;
            int newSeats;

            cout << "Enter Flight ID to update: ";
            cin >> flightID;

            cout << "Enter new Time (HH:MM): ";
            cin.ignore();
            getline(cin, newTime);

            cout << "Enter new Fare: ";
            cin >> newFare;

            cout << "Enter new Available Seats: ";
            cin >> newSeats;

            if (flightBST.updateFlight(flightID, newTime, newFare, newSeats))
                cout << "Flight updated successfully!\n";
            else
                cout << "Flight not found!\n";
            break;
        }
        case 4:
        {
            int flightID;
            cout << "Enter Flight ID to remove: ";
            cin >> flightID;

            if (flightBST.removeFlight(flightID))
                cout << "Flight removed successfully!\n";
            else
                cout << "Flight not found!\n";
            break;
        }
        case 5:
        {
            cout << "Returning to Main Menu\n";
            mainMenu(flightBST, bookingList);
        }
        case 6:
        {
            cout << "Exiting, Goodbye!";
            return;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
            mainMenuStaff(flightBST, bookingList);
        }
    }
}

void mainMenuAdmin(FlightBST &flightBST, BookingLinkedList &bookingList)
{
    while (true)
    { // Menu loop
        int choice;
        cout << "\n==== Admin Controls ====\n";
        cout << "1. Add a new user\n";
        cout << "2. Remove a user\n";
        cout << "3. View system-wide booking and payment data\n";
        cout << "4. Go back to main menu\n";
        cout << "5. Exit the program\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string role;
            cout << "Enter role (Passenger/Staff/Admin): ";
            cin >> role;
            addUser(role); // Ensure this function is implemented
            break;
        }
        case 2:
        {
            removeUser(); // Ensure this function is implemented
            break;
        }
        case 3:
        {
            bookingList.displayBookingsWithPayments(); // Display booking and payment details
            break;
        }
        case 4:
        {
            cout << "Returning to main menu...\n";
            mainMenu(flightBST, bookingList);

            // Exit the admin menu and go back
        }
        case 5:
        {
            cout << "Exiting the program. Goodbye!\n";
            exit(0); // Terminate the program
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Function to validate email format (simple regex for basic validation)
bool isValidEmail(const string &email)
{
    regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, emailPattern);
}

// Function to validate the password (must be at least 8 characters and include a number)
bool isValidPassword(const string &password)
{
    if (password.length() < 8)
    {
        return false;
    }

    regex passwordPattern(R"((.*\d.*))"); // password must include at least one digit
    return regex_match(password, passwordPattern);
}

// Function to hash the password (a simple example for demonstration)
string hashPassword(const string &password)
{
    string hashedPassword = password;
    for (auto &c : hashedPassword)
    {
        c += 1; // simple shift encryption for demo
    }
    return hashedPassword;
}

// Function to authenticate user login
bool loginUser(string &emailOut, string &nameOut)
{
    string email, password;

    cout << "Enter your email: ";
    cin >> email;

    cout << "Enter your password: ";
    cin >> password;

    // Check if the email exists in the stored map
    if (users.find(email) != users.end())
    {
        string hashedPassword = hashPassword(password);

        if (users[email].first == hashedPassword)
        {
            cout << "Login successful! Welcome, " << users[email].second << "!\n";

            // Pass the email and name back to the caller
            emailOut = email;
            nameOut = users[email].second;
            return true;
        }
    }

    cout << "Login failed! Incorrect email or password.\n";
    return false;
}

// Function to register a new user
void registerUser(FlightBST &flightBST, BookingLinkedList &bookingList, const string &role)
{
    string name, email, password;

    // Ask for user details
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter your email: ";
    cin >> email;

    // Validate email format
    while (!isValidEmail(email))
    {
        cout << "Invalid email format. Please enter a valid email: ";
        cin >> email;
    }

    // Ask for password and validate it
    do
    {
        cout << "Enter your password (must be at least 8 characters and include a number): ";
        cin >> password;

        if (!isValidPassword(password))
        {
            cout << "Password does not meet the requirements. Please try again.\n";
        }

    } while (!isValidPassword(password));

    // Hash the password before storing
    string hashedPassword = hashPassword(password);

    // Store the user details with role
    users[email] = {hashedPassword, name + ":" + role};

    // Write to file to persist data
    ofstream outFile("users.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << email << " " << hashedPassword << " " << name << " " << role << endl;
        outFile.close();
        cout << "Registration successful as " << role << "!\n";
    }
    else
    {
        cout << "Error writing to file.\n";
    }

    // After registration, prompt to login or exit
    int choice;
    cout << "Do you want to login now?\n";
    cout << "1. Login\n2. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        string emailOut, nameOut;
        if (loginUser(emailOut, nameOut))
        {
            if (role == "Passenger")
                mainMenuPassenger(flightBST, bookingList);
            else if (role == "Airline Staff")
                mainMenuStaff(flightBST, bookingList);
            else if (role == "Admin")
                mainMenuAdmin(flightBST, bookingList);
        }
    }
    else
    {
        cout << "Exiting...\n";
    }
}

void addUser(const string &role)
{
    string name, email, password;
    cout << "Enter user name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter user email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    // Validate email and password, hash password, and store user
    string hashedPassword = hashPassword(password);
    users[email] = {hashedPassword, name + ":" + role};

    // Write to file
    ofstream outFile("users.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << email << " " << hashedPassword << " " << name << " " << role << endl;
        outFile.close();
    }
    cout << "User added successfully.\n";
}

void removeUser()
{
    string email;
    cout << "Enter the email of the user to remove: ";
    cin >> email;

    if (users.find(email) != users.end())
    {
        users.erase(email);

        // Remove from file (you would need to rewrite the file without this user)
        cout << "User removed successfully.\n";
    }
    else
    {
        cout << "User not found.\n";
    }
}

int main()
{

    FlightBST flightBST;
    BookingLinkedList bookingList;
    addDefaultFlights(flightBST);

    // Load the users from file into the map (for persistent storage)
    ifstream inFile("users.txt");
    if (inFile.is_open())
    {
        string email, password, name;
        while (inFile >> email >> password >> name)
        {
            users[email] = {password, name}; // Load stored user data
        }
        inFile.close();
    }

    // Display the main menu
    mainMenu(flightBST, bookingList);

    return 0;
}