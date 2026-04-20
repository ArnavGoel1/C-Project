# Electricity Billing System (C++)

## About the Project
This is a console-based Electricity Billing System made in C++.  
The main idea behind this project is to manage customer records and generate electricity bills automatically based on units consumed.

It uses basic concepts of C++ like classes, file handling, and a menu-driven system.

---

## Features

- Add new customers  
- Update existing customer details  
- Delete customer records  
- Generate electricity bills  
- Search records:
  - By Customer ID  
  - By Bill ID  
  - By Date  
- Export records (by date) to a text file  
- Data is stored in files so it is saved even after closing the program  

---

## How Billing Works

The bill is calculated based on units consumed:

| Units | Rate |
|------|------|
| 0 – 200 | ₹6/unit |
| 201 – 400 | ₹7.7/unit |
| Above 400 | ₹9/unit |

Additional charges:
- Fixed charge: ₹1000  
- GST: 5%  
- Due date is set to 15 days from billing  

---

## How to Run

1. Compile the code:
g++ main.cpp -o billing

2. Run the program:
./billing

---

## Login Details

Username: admin  
Password: a12345  

---

## Files Used

main.cpp        -> source code  
Bill.txt        -> stores customer and billing data  
Customer.txt    -> used when exporting records  

---

## Limitations

- Console-based program (no GUI)  
- Uses file storage instead of a database  
- Tariff rates are fixed in the code  
- Single-user system  

---

## Future Improvements

- Add a GUI (Qt or web-based)  
- Use a database like MySQL or SQLite  
- Add user roles (admin/user)  
- Add online payment system  

---

## Author

Arnav Goel  
BCA Student  


