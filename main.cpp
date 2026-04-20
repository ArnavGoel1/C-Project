#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;

/*
    CLASS: person
    Stores basic customer details
*/
class person {
public:
    long long int p;     // Phone number (10 digits)
    static int cid;      // Static counter for customer IDs
    char n[50];          // Customer name
};
int person::cid = 1;

/*
    CLASS: bill (inherits person)
    Stores billing-related information
*/
class bill : public person {
public:
    int custid;          // Unique customer ID
    int billid;          // Unique bill ID (max 999)
    static int bid;      // Static counter for bill IDs
    char date[20];       // Bill date
    char datetime[100];  // Full timestamp
    float total;         // Total bill amount (changed to float for accuracy)
    char ddate[20];      // Due date

    /*
        FUNCTION: input()
        Takes customer input and validates phone number
    */
    void input() {
        cout << "Enter your name\n";
        cin.ignore();
        cin.getline(n, 50);

        // Validate phone number
        do {
            cout << "Enter your phone no.\n";
            cin >> p;
            if (p < 1000000000 || p > 9999999999)
                cout << "Invalid phone number\n";
        } while (p < 1000000000 || p > 9999999999);

        // Get current date & time
        time_t now = time(0);
        tm* ltm = localtime(&now);

        strftime(datetime, sizeof(datetime), "%Y %b %d %H:%M:%S", ltm);
        strftime(date, sizeof(date), "%d %b %Y", ltm);
    }

    /*
        FUNCTION: display()
        Displays customer and billing details
    */
    void display() {
        cout << "\nName: " << n << endl;
        cout << "Phone Number: " << p << endl;
        cout << "Customer ID: " << custid << endl;
        cout << "Bill ID: " << setfill('0') << setw(3) << billid << endl;
        cout << "Bill Date: " << datetime << endl;
        cout << "Total Bill Amount: Rs." << fixed << setprecision(2) << total << endl;
        cout << "Due Date: " << ddate << endl;
    }
};

int bill::bid = 1;

// Function declarations
void addrec();
void updaterec();
void deleterec();
void disrec();
void genbill();
void searec();
void SearchbyDate();

/*
    MAIN FUNCTION
    Handles login + menu-driven system
*/
int main() {
    int c;
    char usern[20], passw[20];

    // Login system
    cout << "Enter your username\n";
    cin >> usern;
    cout << "Enter your password\n";
    cin >> passw;

    // Validate login
    if (strcmp(usern, "admin") == 0 && strcmp(passw, "a12345") == 0) {
        cout << "Login Successful\n";

        // Restore last used IDs from file
        ifstream fin("Bill.txt", ios::binary);
        if (fin) {
            bill b;
            int last_custid = 0, last_billid = 0;

            while (fin.read((char*)&b, sizeof(b))) {
                last_custid = b.custid;
                if (b.billid > last_billid)
                    last_billid = b.billid;
            }

            person::cid = last_custid + 1;
            bill::bid = last_billid + 1;
            fin.close();
        }
    } else {
        cout << "Invalid credentials\n";
        exit(0);
    }

    // Menu loop
    do {
        cout << "\n*********Electricity Bill System*********\n";
        cout << "1. Add Customer\n";
        cout << "2. Update Customer\n";
        cout << "3. Delete Customer\n";
        cout << "4. Search Customer\n";
        cout << "5. Generate Bill\n";
        cout << "6. Display All Customers\n";
        cout << "7. Exit\n";

        cin >> c;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (c) {
            case 1: addrec(); break;
            case 2: updaterec(); break;
            case 3: deleterec(); break;
            case 4: searec(); break;
            case 5: genbill(); break;
            case 6: disrec(); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice\n";
        }

    } while (c != 7);

    return 0;
}

/*
    FUNCTION: addrec()
    Adds a new customer record
*/
void addrec() {
    bill b;
    ofstream fileout("Bill.txt", ios::binary | ios::app);

    b.input();
    b.custid = person::cid++;

    cout << "Customer ID: " << b.custid << endl;

    fileout.write((char*)&b, sizeof(b));
    fileout.close();

    cout << "Record added successfully\n";
}

/*
    FUNCTION: SearchbyDate()
    Searches records based on bill date and optionally exports them
*/
void SearchbyDate() {
    char dateInput[20];
    cout << "Enter date (dd Mon yyyy): ";
    cin.ignore();
    cin.getline(dateInput, 20);

    bill b;
    bool found = false;
    fstream in("Bill.txt", ios::in | ios::binary);

    while (in.read((char*)&b, sizeof(b))) {
        if (strcmp(b.date, dateInput) == 0) {
            b.display();
            found = true;
        }
    }

    if (!found)
        cout << "No records found\n";

    // Export option
    char choice[10];
    cout << "Export to Customer.txt? (yes/NO): ";
    cin >> choice;

    if (strcmp(choice, "yes") == 0 || strcmp(choice, "YES") == 0) {
        in.clear();
        in.seekg(0);
        ofstream outFile("Customer.txt", ios::app);

        while (in.read((char*)&b, sizeof(b))) {
            if (strcmp(b.date, dateInput) == 0) {
                outFile << "Customer ID: " << b.custid << "\n";
                outFile << "Bill ID: " << b.billid << "\n";
                outFile << "Name: " << b.n << "\n";
                outFile << "Total: Rs." << b.total << "\n\n";
            }
        }

        outFile.close();
        cout << "Exported successfully\n";
    }

    in.close();
}

/*
    FUNCTION: genbill()
    Generates electricity bill using slab-based tariff
*/
void genbill() {
    bill b;
    int id, units;
    char due_date[20];
    bool found = false;

    int fixedc = 1000;
    float tariff, cost, gst, total;

    fstream fin("Bill.txt", ios::in | ios::out | ios::binary);

    cout << "Enter customer ID: ";
    cin >> id;

    while (fin.read((char*)&b, sizeof(b))) {
        if (b.custid == id) {
            found = true;

            // Calculate due date
            time_t due = time(0) + (15 * 24 * 60 * 60);
            tm* dtm = localtime(&due);
            strftime(due_date, sizeof(due_date), "%Y %b %d", dtm);

            cout << "Enter units consumed: ";
            cin >> units;

            // Tariff logic
            if (units <= 200) tariff = 6;
            else if (units <= 400) tariff = 7.7;
            else tariff = 9;

            cost = (units * tariff) + fixedc;
            gst = cost * 0.05;
            total = cost + gst;

            // Bill ID generation
            if (bill::bid <= 999)
                b.billid = bill::bid++;
            else {
                cout << "Bill ID limit reached!\n";
                return;
            }

            b.total = total;
            strcpy(b.ddate, due_date);

            b.display();

            // Update record in file
            streampos pos = fin.tellg();
            fin.seekp(pos - (streampos)sizeof(b));
            fin.write((char*)&b, sizeof(b));

            break;
        }
    }

    if (!found)
        cout << "Customer not found\n";

    fin.close();
}

/*
    FUNCTION: updaterec()
    Updates existing customer record
*/
void updaterec() {
    int id;
    bill b;
    bool found = false;

    cout << "Enter customer ID: ";
    cin >> id;

    fstream file("Bill.txt", ios::binary | ios::in | ios::out);

    while (file.read((char*)&b, sizeof(b))) {
        if (b.custid == id) {
            streampos pos = file.tellg();

            cout << "Old Record:\n";
            b.display();

            cout << "Enter new details:\n";
            b.input();
            b.custid = id;

            file.seekp(pos - (streampos)sizeof(b));
            file.write((char*)&b, sizeof(b));

            cout << "Updated successfully\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Customer not found\n";

    file.close();
}

/*
    FUNCTION: deleterec()
    Deletes a customer record
*/
void deleterec() {
    int id;
    bill b;
    bool found = false;

    ifstream infile("Bill.txt", ios::binary);
    ofstream outfile("temp.txt", ios::binary);

    cout << "Enter customer ID: ";
    cin >> id;

    while (infile.read((char*)&b, sizeof(b))) {
        if (b.custid != id)
            outfile.write((char*)&b, sizeof(b));
        else
            found = true;
    }

    infile.close();
    outfile.close();

    remove("Bill.txt");
    rename("temp.txt", "Bill.txt");

    if (found)
        cout << "Deleted successfully\n";
    else
        cout << "Customer not found\n";
}

/*
    FUNCTION: searec()
    Searches customer by ID, Bill ID, or Date
*/
void searec() {
    bill b;
    bool found = false;
    int choice;

    ifstream infile("Bill.txt", ios::binary);

    cout << "1. Customer ID\n2. Bill ID\n3. Date\n";
    cin >> choice;

    if (choice == 1) {
        int id;
        cin >> id;

        while (infile.read((char*)&b, sizeof(b))) {
            if (b.custid == id) {
                b.display();
                found = true;
                break;
            }
        }
    }
    else if (choice == 2) {
        int id;
        cin >> id;

        while (infile.read((char*)&b, sizeof(b))) {
            if (b.billid == id) {
                b.display();
                found = true;
                break;
            }
        }
    }
    else if (choice == 3) {
        SearchbyDate();
        found = true;
    }

    if (!found)
        cout << "Record not found\n";

    infile.close();
}

/*
    FUNCTION: disrec()
    Displays all customer records
*/
void disrec() {
    bill b;
    ifstream file("Bill.txt", ios::binary);

    if (!file) {
        cout << "No records found\n";
        return;
    }

    while (file.read((char*)&b, sizeof(b))) {
        b.display();
        cout << "----------------------\n";
    }

    file.close();
}