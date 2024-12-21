

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>

using namespace std;

unordered_map<string, string> paymentHistory; // stores user payment history

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
             << ", Time: " << flight.time << ", Fare: $" << flight.fare
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
    // Method to book seats on a flight

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

    // Helper method to traverse BST and update seat count
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
};

// Add default flights
void addDefaultFlights(FlightBST &flightBST)
{
    flightBST.addFlight("Lahore", "Islamabad", "2024-12-15", "08:00", 100.0, 50);
    flightBST.addFlight("Islamabad", "Karachi", "2024-12-15", "12:00", 150.0, 60);
    flightBST.addFlight("Karachi", "Lahore", "2024-12-15", "16:00", 120.0, 40);
}

// Main menu for the passenger
void mainMenuPassenger(FlightBST &flightBST)
{
    while (true)
    {
        int choice;
        cout << "\nWelcome to GIKI Flights!\n";
        cout << "1. View all available flights\n";
        cout << "2. Search for flights\n";
        cout << "3. Book a flight\n"; // New option for booking
        cout << "4. Exit\n";
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
        case 3: // Call bookFlight function
            bookFlight(flightBST);
            break;
        case 4:
            cout << "Thank you for using GIKI Airlines. Goodbye!\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void mainMenuAdmin(FlightBST &flightBST) { cout << "hehe"; };
void mainMenuStaff(FlightBST &flightBST) { cout << "huhu"; };

unordered_map<string, pair<string, string>> users; // stores email, password pair

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
void registerUser(FlightBST &flightBST, const string &role)
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
                mainMenuPassenger(flightBST);
            else if (role == "Airline Staff")
                mainMenuStaff(flightBST);
            else if (role == "Admin")
                mainMenuAdmin(flightBST);
        }
    }
    else
    {
        cout << "Exiting...\n";
    }
}

void mainMenu(FlightBST &flightBST)
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
            registerUser(flightBST, "Passenger");
        }
        else if (subChoice == "2")
        {
            string emailOut, nameOut;
            if (loginUser(emailOut, nameOut))
                mainMenuPassenger(flightBST);
        }
    }
    else if (roleChoice == "2")
    {
        string subChoice;
        cout << "1. Register\n2. Login\nEnter your choice: ";
        cin >> subChoice;

        if (subChoice == "1")
        {
            registerUser(flightBST, "Airline Staff");
        }
        else if (subChoice == "2")
        {
            string emailOut, nameOut;
            if (loginUser(emailOut, nameOut))
                mainMenuStaff(flightBST);
        }
    }
    else if (roleChoice == "3")
    {
        string subChoice;
        cout << "1. Register\n2. Login\nEnter your choice: ";
        cin >> subChoice;

        if (subChoice == "1")
        {
            registerUser(flightBST, "Admin");
        }
        else if (subChoice == "2")
        {
            string emailOut, nameOut;
            if (loginUser(emailOut, nameOut))
                mainMenuAdmin(flightBST);
        }
    }
    else
    {
        cout << "Invalid role selected. Exiting...\n";
    }
}

int main()
{

    FlightBST flightBST;
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
    mainMenu(flightBST);

    return 0;
}
