#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;

class person {
public:
    long long int p;
    static int cid;
    char n[50];
};
int person::cid = 1;

class bill : public person {
public:
    int custid;
    int billid;
    static int bid;
    char date[20];
    char datetime[100];
    int total;
    char ddate[20];

    void input() {
        cout << "Enter your name" << endl;
        cin.ignore();
        cin.getline(n, 50);
        do {
            cout << "Enter your phone no." << endl;
            cin >> p;
            if (p < 1000000000 || p > 9999999999) {
                cout << "Invalid phone number" << endl;
            }
        } while (p < 1000000000 || p > 9999999999);

        time_t now = time(0);
        tm* ltm = localtime(&now);
        strftime(datetime, sizeof(datetime), " %Y %b %d %H:%M:%S", ltm);
        strftime(date, sizeof(date), "%d %b %Y", ltm);
    }

    void display() {
        cout << " Name: " << n << endl;
        cout << " Phone Number: " << p << endl;
        cout << " The customer ID: " << custid << endl;
        cout << " The bill ID is " << setfill('0') << setw(3) << billid << endl;
        cout << " The bill date is " << datetime << endl;
        cout<< " Total Bill amount is Rs."<< fixed << setprecision(2)<<total<<endl;
        cout<< "Due date is "<<ddate<<endl;
    }
};
int bill::bid = 1;

void addrec();
void updaterec();
void deleterec();
void disrec();
void genbill();
void searec();
void SearchbyDate();

int main() {
    int c;
    char usern[20], passw[20];
    cout << "Enter your username" << endl;
    cin >> usern;
    cout << "Enter your password" << endl;
    cin >> passw;

    if (strcmp(usern, "admin") == 0 && strcmp(passw, "a12345") == 0) {
        cout << "Login Successful" << endl;

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
    }
    else {
        cout << "Invalid credentials" << endl;
        exit(0);
    }

    do {
        cout << "\n*********Electricity bill*********\n" << endl;
        cout << "1 to Add the customer " << endl;
        cout << "2 to Update the customer " << endl;
        cout << "3 to Delete the customer " << endl;
        cout << "4 to Search the customer " << endl;
        cout << "5 to Generate the bill" << endl;
        cout << "6 to Display all the customer" << endl;
        cout << "7 to Exit" << endl;
        cin >> c;

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
        case 7: cout << "Exitting"; break;
        default: cout << "Invalid choice" << endl;
        }
    } while (c != 7);

    return 0;
}

void addrec() {
    bill b;
    ofstream fileout("Bill.txt", ios::binary | ios::app);
    b.input();
    b.custid = person::cid++;
    cout << " The customer ID: " << b.custid << endl;
    fileout.write((char*)&b, sizeof(b));
    fileout.close();
    cout << "New record has been added\n";
}

void SearchbyDate() {
    char dateInput[20];
    cout << "Enter date to search (format: dd Mon yyyy, e.g., 07 Jul 2025): ";
    cin.ignore();
    cin.getline(dateInput, 20);

    bill b;
    bool found = false;
    fstream in("Bill.txt", ios::in | ios::binary);
    while (in.read((char*)&b, sizeof(b))) {
        if (strcmp(b.date, dateInput) == 0) {
            cout << "\nBill Found:\n";
            b.display();
            found = true;
        }
    }

    if (!found) {
        cout << "\nNo bill found for the given date.\n";
    }

    char choice[10];
    cout << "\nDo you want to export to Customer.txt? (yes/NO): ";
    cin >> choice;

    if (strcmp(choice, "yes") == 0 || strcmp(choice, "YES") == 0) {
        in.clear();
        in.seekg(0);
        ofstream outFile("Customer.txt", ios::app);
        while (in.read((char*)&b, sizeof(b))) {
            if (strcmp(b.date, dateInput) == 0) {
                outFile << "Date & Time: " << b.datetime << "\n";
                outFile << "Customer ID: " << b.custid << "\n";
                outFile << "Bill ID: " << b.billid << "\n";
                outFile << "Customer Name: " << b.n << "\n";
                outFile << "Phone Number: " << b.p << "\n";
                outFile << "Total Bill Amount: Rs." << b.total << "\n";
                outFile << "Due Date: " << b.ddate << "\n\n";
            }
        }
        outFile.close();
        cout << "\nBills saved to 'Customer.txt'.\n";
    }

    in.close();
}


void genbill() {
    bill b;
    int id, u2;
    char due_date[20];
    bool found = false; 
    int fixedc = 1000;
    float tariff, tcost, cost, gst;


    fstream fin("Bill.txt", ios::in | ios::out | ios::binary);
    if (!fin) {
        cout << "File not found.\n";
        return;
    }

    cout << "\n==== Bill=======\n";
    cout << "Enter the customer ID for the bill: ";
    cin >> id;

    while (fin.read((char*)&b, sizeof(b))) {
        if (b.custid == id) {
            found = true;
           
            time_t due = time(0) + (15 * 24 * 60 * 60);
            tm* dtm = localtime(&due);
            strftime(due_date, sizeof(due_date), "%Y %b %d", dtm);

            cout << "Date: " << b.datetime << endl;
            cout << "Enter your this month's electricity consumed in KiloWattHour(kWh): ";
            cin >> u2;

            if (u2 <= 200 && u2 >= 0)
                tariff = 6;
            else if (u2 >= 201 && u2 <= 400)
                tariff = 7.7;
            else
                tariff = 9;

            cost = (u2 * tariff) + fixedc;
            gst = cost * 0.05;
            tcost = cost + gst;

            if (bill::bid <= 999) {
                b.billid = bill::bid++;
            }
            else {
                cout << "Cannot generate bill. Bill ID limit (999) reached.\n" << endl;
                fin.close();
                bill::bid = 1;
                return;
            }
            b.total= tcost;
            strcpy(b.ddate,due_date);

            b.display();
            cout << "The units consumed is " << u2 << " kWh" << endl;
            cout << "The tariff is " << tariff << endl;
            cout << "The cost is " << cost << endl;
            cout << "The fixed cost is " << fixedc << endl;
            cout << "The GST is " << gst << endl;
            cout << "The total cost is ₹" << tcost << endl;
            cout << "The bill due date is " << due_date << endl;

            streampos pos = fin.tellg();
            fin.seekp(pos - (streampos)sizeof(b));
            fin.write((char*)&b, sizeof(b));
            break;
        }
    }

    if (!found)
        cout << "No customer is there" << endl;

    fin.close();
}

void updaterec() {
    int i;
    bill b;
    bool found = false;
    cout << "Enter customer ID to update" << endl;
    cin >> i;
    fstream fileout("Bill.txt", ios::binary | ios::in | ios::out);
    while (fileout.read((char*)&b, sizeof(b))) {
        if (b.custid == i) {
            streampos pos = fileout.tellg();
            cout << "\nOld Record:\n" << endl;
            b.display();
            cout << "\nEnter New Details:\n";
            b.input();
            b.custid = i;
            fileout.seekp(pos - (streampos)sizeof(b), ios::beg);
            fileout.write((char*)&b, sizeof(b));
            cout << "Customer Updated Successfully" << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Customer not found" << endl;
    }
    fileout.close();
}

void deleterec() {
    int i;
    bill b;
    bool found = false;
    ifstream infile("Bill.txt", ios::binary);
    ofstream outfile("temp.txt", ios::binary);
    cout << "Enter customer ID" << endl;
    cin >> i;
    while (infile.read((char*)&b, sizeof(b))) {
        if (b.custid != i) {
            outfile.write((char*)&b, sizeof(b));
        }
        else {
            found = true;
        }
    }
    infile.close();
    outfile.close();
    remove("Bill.txt");
    rename("temp.txt", "Bill.txt");
    if (found) {
        cout << "Customer deleted successfully" << endl;
    }
    else {
        cout << "Customer not found!" << endl;
    }
}

void searec() {
    bool found = false;
    bill b;
    int choice;
    ifstream infile("Bill.txt", ios::binary);
    cout << "Search by:\n1. Customer ID\n2. Bill ID\n3. Date\n";
    cin >> choice;
    if (choice == 1) {
        int i;
        cout << "Enter customer ID" << endl;
        cin >> i;
        while (infile.read((char*)&b, sizeof(b))) {
            if (b.custid == i) {
                cout << "\n Customer Found\n";
                b.display();
                found = true;
                break;
            }
        }
    }
    else if (choice == 2) {
        int i;
        cout << "Enter the bill ID" << endl;
        cin >> i;
        while (infile.read((char*)&b, sizeof(b))) {
            if (b.billid == i) {
                cout << "\nBill Found: \n" << endl;
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
    else {
        cout << "Invalid Choice" << endl;
        infile.close();
        return;
    }

    if (!found) {
        cout << " Record Not found" << endl;
    }
    infile.close();
}

void disrec() {
    bill b;
    ifstream file("Bill.txt", ios::binary);
    if (!file) {
        cout << "No customer records found" << endl;
        return;
    }
    cout << "\n======List of Customer======\n\n";
    bool found = false;
    while (file.read((char*)&b, sizeof(b))) {
        b.display();
        cout << "\n----------------------------\n" << endl;
        found = true;
    }
    if (!found) {
        cout << "No records\n";
    }
    file.close();
}