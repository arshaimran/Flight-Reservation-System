#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>

using namespace std;

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
bool loginUser()
{
    string email, password;

    cout << "Enter your email: ";
    cin >> email;

    cout << "Enter your password: ";
    cin >> password;

    // Check if the email exists in the stored map
    if (users.find(email) != users.end())
    {
        // Hash the entered password
        string hashedPassword = hashPassword(password);

        // Compare the stored password with the entered password
        if (users[email].first == hashedPassword)
        {
            cout << "Login successful! Welcome to GIKI Airlines, " << users[email].second << "!\n";

            // Now check the user's role (staff or admin)
            string roleChoice;
            cout << "Please select your role:\n";
            cout << "1. Airline Staff\n2. Admin\n";
            cout << "Enter the number corresponding to your role: ";
            cin >> roleChoice;

            if (roleChoice == "1")
            { // If it's an Airline Staff member
                string staffKey;
                cout << "Enter Staff Key: ";
                cin >> staffKey;

                if (staffKey == "0000")
                {
                    cout << "Staff login successful!\n";
                    return true;
                }
                else
                {
                    cout << "Incorrect Staff Key!\n";
                    return false;
                }
            }
            else if (roleChoice == "2")
            { // If it's an Admin
                string adminKey;
                cout << "Enter Admin Key: ";
                cin >> adminKey;

                if (adminKey == "1234")
                {
                    cout << "Admin login successful!\n";
                    return true;
                }
                else
                {
                    cout << "Incorrect Admin Key!\n";
                    return false;
                }
            }
            else
            {
                cout << "Invalid role selection.\n";
                return false;
            }
        }
    }

    cout << "Login failed! Incorrect email or password.\n";
    return false;
}

// Function to register a new user
void registerUser()
{
    string name, email, password;

    // Ask for user details
    cout << "Enter your name: ";
    cin.ignore(); // To ignore the leftover newline character
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

    // Store the user details in the map (email is the key, pair of password and name)
    users[email] = {hashedPassword, name};

    // Write to file to persist data
    ofstream outFile("users.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << email << " " << hashedPassword << " " << name << endl;
        outFile.close();
        cout << "Registration successful!\n";
    }
    else
    {
        cout << "Error writing to file.\n";
    }

    // After registration, prompt to either login or exit
    int choice;
    cout << "Do you want to login now?\n";
    cout << "1. Login\n2. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        loginUser();
    }
    else
    {
        cout << "Exiting...\n";
    }
}

// Function to show the main menu
void mainMenu()
{
    string roleChoice;

    cout << "Welcome to GIKI Airlines!\n";
    cout << "Please select your role:\n";
    cout << "1. Passenger\n2. Airline Staff\n3. Admin\n";
    cout << "Enter the number corresponding to your role: ";
    cin >> roleChoice;

    // Based on role, either register or login
    if (roleChoice == "1")
    {
        cout << "You selected Passenger.\n";
        cout << "Please select an option:\n";
        cout << "1. Register\n2. Login\n";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            registerUser();
        }
        else if (choice == 2)
        {
            if (loginUser())
            {
                cout << "Welcome Passenger!\n";
            }
        }
        else
        {
            cout << "Invalid option. Exiting...\n";
        }
    }
    else if (roleChoice == "2")
    {
        cout << "You selected Airline Staff.\n";
        cout << "Please select an option:\n";
        cout << "1. Register\n2. Login\n";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            registerUser();
        }
        else if (choice == 2)
        {
            if (loginUser())
            {
                cout << "Welcome Airline Staff Member!\n";
            }
        }
        else
        {
            cout << "Invalid option. Exiting...\n";
        }
    }
    else if (roleChoice == "3")
    {
        cout << "You selected Admin.\n";
        cout << "Please select an option:\n";
        cout << "1. Register\n2. Login\n";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            registerUser();
        }
        else if (choice == 2)
        {
            if (loginUser())
            {
                cout << "Welcome Admin!\n";
            }
        }
        else
        {
            cout << "Invalid option. Exiting...\n";
        }
    }
    else
    {
        cout << "Invalid role selected. Exiting...\n";
    }
}

int main()
{
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
    mainMenu();

    return 0;
}