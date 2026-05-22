# Badminton Court Booking System (C++)

## Project Overview
This project is a console-based Badminton Court Booking System developed using C++ with MySQL database integration through phpMyAdmin.

The system simulates a real-world badminton court reservation management process with two user roles:
- Customer
- Staff

Customers can manage their court bookings, while staff members can monitor bookings, maintenance status, and booking analytics.

The project was developed as a university project to strengthen understanding in database-driven system development using C++.

---

## Features

### Customer Functions
- Register customer account
- Login system
- Book badminton court
- Change booking details
- Cancel booking
- View active/upcoming bookings
- Logout account

### Staff Functions
- View customers with no bookings after registration
- View monthly sales report
- View frequently booked courts
- Turn court ON/OFF for maintenance
- Search booking records
- Logout account

---

## Database Integration

This system requires a local MySQL database connection to function properly.

The database was managed using phpMyAdmin and contains several tables including:
- Booking details
- Court details
- Customer details
- Staff details

The system performs database operations such as:
- Insert booking records
- Update booking information
- Delete booking records
- Retrieve booking and customer data

---

## Technologies Used
- C++
- MySQL
- phpMyAdmin
- SQL Queries
- Console-Based User Interface

---

## Project Purpose
This project was developed for academic purposes to improve understanding in:
- Database connectivity
- SQL operations
- CRUD functionality
- User role management
- Console-based application development

---

## Limitations

This project requires:
- Local MySQL server
- phpMyAdmin database setup
- Proper database configuration

The database is not hosted online, therefore the project may not run directly without configuring the local database environment.

This project is intended for educational and demonstration purposes only.
