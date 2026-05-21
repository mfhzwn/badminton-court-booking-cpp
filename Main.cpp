#define ENTER 13
#define BCKSPC 8

#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h> //getch()
#include <mysql.h> //libmysql
#include <ctime>
#include <thread>
#include <chrono>
#include <sstream>

using namespace std;

void Login();
void Register();

string checkCourt(string bc, string strStime, string strFtime, string chosen_year, string chosen_month, string chosen_day);
string checkCourt(string bc, string strStime, string strFtime, string date);

string morphDate(string receive);
string Payment(double price, string id);
int numberOfDays(int month, int year);
bool isCurrentDate(string date);

void Customer(string id);
void Booking(string id);
void DeleteBooking(string id);
void EditBooking(string id);
void ViewBooking(string id);

void Staff(string id);
void CustomerReport(string id);
void SalesReport(string id);
void CourtReport(string id);
void CourtStatus(string id);
void SearchBooking(string id);

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;

time_t now = time(nullptr);
tm* local_time = localtime(&now);

int day = local_time->tm_mday;
int month = local_time->tm_mon + 1;
int year = local_time->tm_year + 1900;
int hour = local_time->tm_hour;
int min = local_time->tm_min;
int sec = local_time->tm_sec;
string strhour = to_string(hour);

const int COURT_SIZE = 20;

class db_response //to check with db 
{
public:
    static void ConnectionFunction()
    {
        conn = mysql_init(0);
        /*if (conn)
            cout << "Database Connected" << endl;
        else
            cout << "Failed To Connect!" << endl;*/

        conn = mysql_real_connect(conn, "localhost", "root", "", "ebcbs", 3306, NULL, 0);
        /*if (conn)
            cout << "Database Connected To MySql" << endl;
        else
            cout << "Failed To Connect!" << endl;*/
    }
};

int main()
{
    system("cls");
    system("title Badminton Court Booking");
    system("color 8E");
    db_response::ConnectionFunction();
    cout << "Current date and time: " << asctime(local_time) << endl;

    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(60) << setfill(' ') << right << "Welcome to EcoSports Badminton Booking" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << "Please choose a menu by entering the number\n" << endl;
    cout << "   (1) Register" << endl;
    cout << "   (2) Login" << endl;
    cout << "   (3) Exit" << endl;
    cout << "   Choose: ";
    string menu;
    cin >> menu;

    if (menu == "1")
        Register();
    else if (menu == "2")
        Login();
    else if (menu == "3")
    {
        cout << "   \nThank you for using EcoSport Booking System. Exiting program..." << endl;
        exit(0);
    }
    else
    {
        cout << "Please choose between 1 - 3. ";
        system("pause"); //pause console
        system("cls");
        main();
    }
    return 0;
}

void Register()
{
    system("cls");
    int i_new_cust_id;
    string choice, choice2;
    size_t length_counter;
    string cust_name, email, password, phone, pay_id, last_cust_id, new_cust_id;

    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(50) << setfill(' ') << right << "ECOSPORTS MEMBER REGISTRATION" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;

    cout << "\nPlease fill in all the informations to register." << endl;
    cout << "\nEnter Email: ";
    cin.ignore(1, '\n');
    getline(cin >> ws, email);

    string checkUser_query = "SELECT * FROM customer WHERE CUST_EMAIL = '" + email + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
        TryAgain:
            cout << "\nERROR: Email already exist. Please proceed to login or use another email." << endl;
            cout << "\n   (1) Try again" << endl;
            cout << "   (2) Login" << endl;
            cout << "   Choose: ";
            cin >> choice2;

            if (choice2 == "1")
            {
                Register();
            }
            else if (choice2 == "2")
                Login();
            else
            {
                cout << "Please choose 1 or 2 only. Try again. ";
                system("pause");
                system("cls");
                goto TryAgain;
            }
        }
        else
        {
            cout << "Enter Password: ";
            getline(cin >> ws, password);
            cout << "Enter Name: ";
            getline(cin >> ws, cust_name);
            RetryPhone:
            cout << "Enter Phone Number: ";
            getline(cin >> ws, phone);

            char* p;
            long converted = strtol(phone.c_str(), &p, 10);
            if (*p) {
                cout << "\nPhone number can only contain numbers. Please try again.\n" << endl;
                goto RetryPhone;
            }

            string checkUser_query = "SELECT CUST_ID FROM customer ORDER BY CUST_ID DESC LIMIT 0,1";
            const char* cu = checkUser_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (res->row_count == 1)
                {
                    while (row = mysql_fetch_row(res))
                        last_cust_id = row[0];
                }
            }
            else
                cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

            last_cust_id[0] = '0';
            i_new_cust_id = stoi(last_cust_id) + 1;
            new_cust_id = to_string(i_new_cust_id);
            length_counter = 5 - new_cust_id.length();
            for (int i = 0; i < length_counter; i++)
            {
                new_cust_id = "0" + new_cust_id;
            }
            new_cust_id[0] = 'C';

            string insertCustomer_query = "INSERT INTO customer (CUST_ID, CUST_EMAIL, CUST_PASS, CUST_NAME, CUST_HP) values ('" + new_cust_id + "', '" + email + "', '" + password + "', '" + cust_name + "', '" + phone + "')";
            const char* q = insertCustomer_query.c_str();
            qstate = mysql_query(conn, q);

            if (!qstate)
            {
                cout << endl << "You have been registered. Please proceed to login. ";
                system("pause");
                main();
            }
            else
            {
                cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
            }
        }
    }
}

void Login()
{
    system("cls");
    string email, id, s;
    stringstream ss;
    char ch, password[30];
    int i = 0;

    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(50) << setfill(' ') << right << "ECOSPORTS LOGIN" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << "\n   Email: ";
    getline(cin >> ws, email);
    cout << "\n   Password: ";
    //getline(cin >> ws, password);
    /*while ((ch = _getch()) != 13)
    {
        password += ch;
        cout << '*';
    }*/

    while(1)
    {
        ch = _getch();
        if (ch == ENTER)
        {
            password[i] = NULL;
            break;
        }
        else if (ch == BCKSPC)
        {
            if (i > 0)
            {
                i--;
                cout << ("\b \b");
            }
        }
        else
        {
            password[i] = ch;
            i++;
            cout << ("*");
        }
    }
    ss<<password;
    ss>>s;

    //customer
    string checkUser_query = "SELECT * FROM customer WHERE CUST_EMAIL = '" + email + "' AND CUST_PASS = '" + password + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
            {
                id = row[0];
            }
            Customer(id);
        }

        else
        {
            string checkUser_query = "SELECT * FROM staff WHERE STAFF_EMAIL = '" + email + "' AND STAFF_PASS = '" + password + "'";
            const char* cu = checkUser_query.c_str();
            qstate = mysql_query(conn, cu);
            //STAFF
            res = mysql_store_result(conn);
            if (res->row_count == 1)
            {
                while (row = mysql_fetch_row(res))
                {
                    id = row[0];
                }
                Staff(id);
            }

            else
            {
                static int counter = 0;
                if (counter < 2)
                {
                    cout << "\n\n   Invalid username or password. " << 2 - counter << " more attempts. ";
                    counter++;
                    system("pause");
                    Login();
                }
                else
                {
                    cout << "No user found. Please go to register. ";
                    counter = 0;
                    system("pause");
                    main();
                }
            }
        }
    }
    else
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

int numberOfDays(int month, int year) 
{
    month = month - 1;
    switch (month) {
    case 0:
    case 2:
    case 4:
    case 6:
    case 7:
    case 9:
    case 11: return(31);
        break;
    case 1:
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
            return (29);
        else
            return (28);
        break;
    case 3:
    case 5:
    case 8:
    case 10: return(30);
        break;
    }
}

string Payment(double price, string id)
{
    int min = local_time->tm_min;

    system("cls");
    int i_new_pay_id;
    size_t length_counter;
    int seconds = 3;
    string new_pay_id, last_pay_id, choice;

    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(50) << setfill(' ') << right << "PAYMENT CONFIRMATION" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << "Price: " << price << endl;
    cout << "\n(1) Yes, confirm booking" << endl;
    cout << "(2) No, cancel booking" << endl;
    cout << "Please choose: ";
    cin >> choice;

    if (choice == "1")
    {
        string checkUser_query = "SELECT PAY_ID FROM payment ORDER BY PAY_ID DESC LIMIT 0,1";
        const char* cu = checkUser_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            if (res->row_count == 1)
            {
                while (row = mysql_fetch_row(res))
                    last_pay_id = row[0];
            }
            else
            {
                cout << "Data not found.";
            }
        }
        else
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

        last_pay_id[0] = '0';
        i_new_pay_id = stoi(last_pay_id) + 1;
        new_pay_id = to_string(i_new_pay_id);
        length_counter = 8 - new_pay_id.length();
        for (int i = 0; i < length_counter; i++)
        {
            new_pay_id = "0" + new_pay_id;
        }
        new_pay_id[0] = 'P';

        string s_price = to_string(price);
        string s_day = to_string(day);
        string s_month = to_string(month);
        string s_year = to_string(year);
        string s_hour = to_string(hour);
        string s_min = to_string(min);
        string s_sec = to_string(sec);

        string insert_query = "INSERT INTO payment (PAY_ID, PAY_PRICE, PAY_DATE, PAY_TIME) VALUES ('" + new_pay_id + "', '" + s_price + "', '" + s_year + "-" + s_month + "-" + s_day + "', '" + s_hour + ":" + s_min + ":" + s_sec + "')";
        const char* q = insert_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << endl << "Payment SUCCESSFUL." << endl;
            while (seconds > 0)
            {
                cout << "System will continue to booking in " << seconds << endl;
                this_thread::sleep_for(chrono::seconds(1));
                seconds--;
            }
            system("cls");
            return new_pay_id;
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
        }
    }
    else if (choice == "2")
    {
        cout << "Payment FAILED. Press ENTER to go to main menu..." << endl;
        system("pause");
        Customer(id);
    }
    else
    {
        cout << "Invalid input." << endl;
        while (seconds > 0)
        {
            cout << "Please try again in " << seconds << endl;
            this_thread::sleep_for(chrono::seconds(1));
            seconds--;
        }
        Payment(price, id);
    }
}

string checkCourt(string bc, string strStime, string strFtime, string chosen_year, string chosen_month, string chosen_day)
{
    string status = "NA";
    string checkCourt_query = "SELECT * FROM court WHERE COURT_ID = 'BC" + bc + "' AND COURT_STATUS = 'OFF'";
    const char* cu = checkCourt_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count >= 1)
        {
            status = "NA";
        }
        else
        {
            string checkCourt_query = "SELECT * FROM booking B, checkcourt CC, court C WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + chosen_year + "-" + chosen_month + "-" + chosen_day + "' AND B.BOOKED_STIME >= '" + strStime + ":00:00' AND B.BOOKED_STIME < '" + strFtime + ":00:00'";
            const char* cu = checkCourt_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (res->row_count >= 1)
                {
                    status = "NA";
                }
                else
                {
                    string checkCourt_query = "SELECT * FROM booking B, checkcourt CC WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + chosen_year + "-" + chosen_month + "-" + chosen_day + "' AND '" + strFtime + ":00:00' > B.BOOKED_STIME AND '" + strFtime + ":00:00' <= B.BOOKED_ETIME";
                    const char* cu = checkCourt_query.c_str();
                    qstate = mysql_query(conn, cu);

                    if (!qstate)
                    {
                        res = mysql_store_result(conn);
                        if (res->row_count >= 1)
                        {
                            status = "NA";
                        }
                        else
                        {
                            string checkCourt_query = "SELECT * FROM booking B, checkcourt CC WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + chosen_year + "-" + chosen_month + "-" + chosen_day + "' AND B.BOOKED_STIME >= '" + strStime + ":00:00' AND B.BOOKED_STIME < '" + strFtime + ":00:00'";
                            const char* cu = checkCourt_query.c_str();
                            qstate = mysql_query(conn, cu);

                            if (!qstate)
                            {
                                res = mysql_store_result(conn);
                                if (res->row_count >= 1)
                                {
                                    status = "NA";
                                }
                                else
                                {
                                    string checkCourt_query = "SELECT * FROM booking B, checkcourt CC WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + chosen_year + "-" + chosen_month + "-" + chosen_day + "' AND B.BOOKED_ETIME > '" + strStime + ":00:00' AND B.BOOKED_ETIME <= '" + strFtime + ":00:00'";
                                    const char* cu = checkCourt_query.c_str();
                                    qstate = mysql_query(conn, cu);

                                    if (!qstate)
                                    {
                                        res = mysql_store_result(conn);
                                        if (res->row_count >= 1)
                                        {
                                            status = "NA";
                                        }
                                        else
                                        {
                                            status = bc;
                                        }

                                    }
                                    else
                                    {
                                        cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
                                    }
                                }

                            }
                            else
                            {
                                cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
                            }
                        }

                    }
                    else
                    {
                        cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
                    }
                }

            }
            else
            {
                cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
            }
        }
    }
    return status;
}

string checkCourt(string bc, string strStime, string strFtime, string date)
{
    string status = "NA";
    string checkCourt_query = "SELECT * FROM court WHERE COURT_ID = 'BC" + bc + "' AND COURT_STATUS = 'OFF'";
    const char* cu = checkCourt_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count >= 1)
        {
            status = "NA";
        }
        else
        {
            string checkCourt_query = "SELECT * FROM booking B, checkcourt CC WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + date + "' AND '" + strStime + ":00:00' >= B.BOOKED_STIME AND '" + strStime + ":00:00' < B.BOOKED_ETIME";
            const char* cu = checkCourt_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (res->row_count >= 1)
                {
                    status = "NA";
                }
                else
                {
                    string checkCourt_query = "SELECT * FROM booking B, checkcourt CC WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + date + "' AND '" + strFtime + ":00:00' > B.BOOKED_STIME AND '" + strFtime + ":00:00' <= B.BOOKED_ETIME";
                    const char* cu = checkCourt_query.c_str();
                    qstate = mysql_query(conn, cu);

                    if (!qstate)
                    {
                        res = mysql_store_result(conn);
                        if (res->row_count >= 1)
                        {
                            status = "NA";
                        }
                        else
                        {
                            string checkCourt_query = "SELECT * FROM booking B, checkcourt CC WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + date + "' AND B.BOOKED_STIME >= '" + strStime + ":00:00' AND B.BOOKED_STIME < '" + strFtime + ":00:00'";
                            const char* cu = checkCourt_query.c_str();
                            qstate = mysql_query(conn, cu);

                            if (!qstate)
                            {
                                res = mysql_store_result(conn);
                                if (res->row_count >= 1)
                                {
                                    status = "NA";
                                }
                                else
                                {
                                    string checkCourt_query = "SELECT * FROM booking B, checkcourt CC WHERE CC.COURT_ID = 'BC" + bc + "' AND B.BOOKED_ID = CC.BOOKED_ID AND B.BOOKED_DATE = '" + date + "' AND B.BOOKED_ETIME > '" + strStime + ":00:00' AND B.BOOKED_ETIME <= '" + strFtime + ":00:00'";
                                    const char* cu = checkCourt_query.c_str();
                                    qstate = mysql_query(conn, cu);

                                    if (!qstate)
                                    {
                                        res = mysql_store_result(conn);
                                        if (res->row_count >= 1)
                                        {
                                            status = "NA";
                                        }
                                        else
                                        {
                                            status = bc;
                                        }

                                    }
                                    else
                                    {
                                        cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
                                    }
                                }

                            }
                            else
                            {
                                cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
                            }
                        }

                    }
                    else
                    {
                        cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
                    }
                }

            }
            else
            {
                cout << "Query Execution Problem! Check court availability function" << mysql_errno(conn) << endl;
            }
        }
    }
    return status;
}

string morphDate(string anything)
{
    string x = anything;
    
    if (x == "1")
        x = "01";
    else if (x == "2")
        x = "02";
    else if (x == "3")
        x = "03";
    else if (x == "4")
        x = "04";
    else if (x == "5")
        x = "05";
    else if (x == "6")
        x = "06";
    else if (x == "7")
        x = "07";
    else if (x == "8")
        x = "08";
    else if (x == "9")
        x = "09";
    else
        return x;
    return x;
}

bool isCurrentDate(string date)
{
    string stryear = to_string(year);
    string strmonth = to_string(month);
    string strday = to_string(day);

    strmonth = morphDate(strmonth);
    strday = morphDate(strday);

    string current = stryear + "-" + strmonth + "-" + strday;
    if (current == date)
        return true;
    else
        return false;
}

void Customer(string id)
{
    string choice;

cust_menu:
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(50) << setfill(' ') << right << "CUSTOMER PAGE" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << "\n   (1) Book Court" << endl;
    cout << "   (2) CHANGE Booking" << endl;
    cout << "   (3) CANCEL Booking" << endl;
    cout << "   (4) View active Bookings" << endl;
    cout << "   (5) Log out" << endl;
    cout << "   Choose: ";
    cin >> choice;

    if (choice == "1")
        Booking(id);
    else if (choice == "2")
        EditBooking(id);
    else if (choice == "3")
        DeleteBooking(id);
    else if (choice == "4")
        ViewBooking(id);
    else if (choice == "5")
    {
        cout << "LOG OUT SUCCESSFUL" << endl;
        system("pause");
        main();
    }
    else
    {
        cout << "Please choose between 1 - 5 only. ";
        system("pause"); //pause console
        system("cls");
        goto cust_menu;
    }
}

void Booking(string id)
{
    int day_in_month, book_day[7], book_month[7], book_year[7], Stime, Ftime, intchosen_day, duration;
    int next_month = 0, next_year = 0, multi = 0;
    string phone, booked_date, Cdate, Ctime, strduration, choice, choice2, strStime, strFtime, chosen_day, chosen_month, chosen_year, BookingID, PayID, DBprice, checker, custName;
    string Ccourt[COURT_SIZE];
    double price = 0;

    int i_new_booked_id;
    size_t length_counter;
    string new_booked_id, last_booked_id;

    day_in_month = numberOfDays(month, year);

    if (hour == 23)
    {
        day++;
    }

    book_day[0] = day;
    book_month[0] = month;
    book_year[0] = year;

    for (int i = 1; i < 7; i++)
    {
        book_day[i] = book_day[i - 1] + 1;
        if (book_day[i] > day_in_month)
        {
            book_month[i] = book_month[i - 1] + 1;
            book_day[i] = 1;
        }
        else
        {
            book_month[i] = book_month[i - 1];
        }

        if (book_month[i] == 13)
        {
            book_month[i] = 1;
            next_year = 1;
        }
        book_year[i] = year + next_year;
    }

    string findCustomer_query = "SELECT * FROM customer WHERE CUST_ID = '" + id + "'";
    const char* cu = findCustomer_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
                custName = row[3];
        }
        else
        {
            cout << "Customer not found" << endl;
        }
    }
    else
        cout << "Query Execution Problem! find customer" << mysql_errno(conn) << endl;

RCdate:
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(60) << setfill(' ') << right << "ECOSPORTS BADMINTON BOOKING" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << "\nNOTE: Booking date can only made 7 days earlier" << endl;
    cout << setw(45) << setfill('-') << right << "BOOKING INFO" << setw(45)<< "-" << endl;
    cout << "Name: " << custName << endl << endl;
    cout << "Choose a date by entering the number." << endl;
    for (int i = 0; i < 7; i++)
        cout << "   (" << i + 1 << ") Date: " << book_day[i] << "-" << book_month[i] << "-" << book_year[i] << endl;
    cout << "\n   (0) BACK" << endl << endl;
    cout << "Enter number: ";
    cin >> Cdate;
    if (Cdate == "1")
    {
        chosen_day = to_string(book_day[0]);
        chosen_month = to_string(book_month[0]);
        chosen_year = to_string(book_year[0]);
    }
    else if (Cdate == "2")
    {
        chosen_day = to_string(book_day[1]);
        chosen_month = to_string(book_month[1]);
        chosen_year = to_string(book_year[1]);
    }
    else if (Cdate == "3")
    {
        chosen_day = to_string(book_day[2]);
        chosen_month = to_string(book_month[2]);
        chosen_year = to_string(book_year[2]);
    }
    else if (Cdate == "4")
    {
        chosen_day = to_string(book_day[3]);
        chosen_month = to_string(book_month[3]);
        chosen_year = to_string(book_year[3]);
    }
    else if (Cdate == "5")
    {
        chosen_day = to_string(book_day[4]);
        chosen_month = to_string(book_month[4]);
        chosen_year = to_string(book_year[4]);
    }
    else if (Cdate == "6")
    {
        chosen_day = to_string(book_day[5]);
        chosen_month = to_string(book_month[5]);
        chosen_year = to_string(book_year[5]);
    }
    else if (Cdate == "7")
    {
        chosen_day = to_string(book_day[6]);
        chosen_month = to_string(book_month[6]);
        chosen_year = to_string(book_year[6]);
    }
    else if (Cdate == "0")
    {
        Customer(id);
    }
    else
    {
        cout << "Please choose between 1 - 7. ";
        system("pause"); //pause console
        system("cls");
        goto RCdate;
    }

RCtime:
    system("cls");
    cout << setw(45) << setfill('-') << right << "BOOKING INFO" << setw(45) << "-" << endl;
    cout << "Booking Date: " << chosen_day << "-" << chosen_month << "-" << chosen_year << endl << endl;
    cout << "NOTE: The starting hour using the 24-hour format." << endl;
    cout << "Start time:" << endl;
    cout << left << setfill(' ') << setw(13) << "(1) 08:00" << right << setw(13) << "(9) 16:00" << endl;
    cout << left << setfill(' ') << setw(13) << "(2) 09:00" << right << setw(13) << "(10) 17:00" << endl;
    cout << left << setfill(' ') << setw(13) << "(3) 10:00" << right << setw(13) << "(11) 18:00" << endl;
    cout << left << setfill(' ') << setw(13) << "(4) 11:00" << right << setw(13) << "(12) 19:00" << endl;
    cout << left << setfill(' ') << setw(13) << "(5) 12:00" << right << setw(13) << "(13) 20:00" << endl;
    cout << left << setfill(' ') << setw(13) << "(6) 13:00" << right << setw(13) << "(14) 21:00" << endl;
    cout << left << setfill(' ') << setw(13) << "(7) 14:00" << right << setw(13) << "(15) 22:00" << endl;
    cout << left << setfill(' ') << setw(13) << "(8) 15:00" << right << setw(13) << "(16) 23:00" << endl;
    cout << "\n(0) BACK" << endl;

    cout << "Select a starting time: ";
    cin >> Ctime;
    if (Ctime == "1" || Ctime == "2" || Ctime == "3" || Ctime == "4" || Ctime == "5" || Ctime == "6" || Ctime == "7" || Ctime == "8" ||
        Ctime == "9" || Ctime == "10" || Ctime == "11" || Ctime == "12" || Ctime == "13" || Ctime == "14" || Ctime == "15" || Ctime == "16")
    {
        Stime = stoi(Ctime);
        Stime = Stime + 7;
        intchosen_day = stoi(chosen_day);
        if (intchosen_day != day)
        {
        RCduration:
            system("cls");
            cout << setw(45) << setfill('-') << right << "BOOKING INFO" << setw(45) << "-" << endl;
            cout << "Name: " << custName << endl << endl;
            cout << "Booking Date: " << chosen_day << "-" << chosen_month << "-" << chosen_year << endl << endl;
            cout << "Start time: " << Stime << ":00" << endl << endl;
            cout << "How many hours would you like to play? (minimum: 1 hour and maximum: 6 hours)" << endl;
            cout << "Price per hour: RM 16" << endl;
            cout << "Operation time: 8 AM until 12 AM" << endl;
            cout << "\n(0) BACK" << endl;
            cout << "Enter duration: ";
            cin >> strduration;

            if (strduration == "1" || strduration == "2" || strduration == "3" || strduration == "4" || strduration == "5" || strduration == "6")
            {
                duration = stoi(strduration);
                Ftime = Stime + duration;
                if (Ftime > 24)
                {

                    cout << "\nERROR: You can only have a maximum duration of " << 24 - Stime << " hour. The court closes at 12 AM." << endl;
                    cout << "Would you like to choose another start time or change another duration?" << endl;
                    cout << "   (1) Change start time" << endl;
                    cout << "   (2) Change duration" << endl;
                RCchoice:
                    cout << "   Choose: ";
                    cin >> choice;
                    if (choice == "1")
                    {
                        goto RCtime;
                    }
                    else if (choice == "2")
                    {
                        goto RCduration;
                    }
                    else
                    {
                        cout << "Please choose 1 or 2 only." << endl;
                        system("pause");
                        cout << endl;
                        goto RCchoice;
                    }
                }
                else
                {
                    if (Ftime == 24)
                    {
                        Ftime = 0;
                    }
                    strStime = to_string(Stime);
                    strFtime = to_string(Ftime);
                RCcourt:
                    system("cls");
                    cout << setw(45) << setfill('-') << right << "BOOKING INFO" << setw(45) << "-" << endl;
                    cout << "Name: " << custName << endl << endl;
                    cout << "Booking Date: " << chosen_day << "-" << chosen_month << "-" << chosen_year << endl << endl;
                    cout << "Start time: " << Stime << ":00" << endl << endl;
                    cout << "Finish time: " << Ftime << ":00" << endl << endl;
                    cout << "Duration: " << duration << " hours" << endl << endl;
                    cout << "Court layout:" << endl;
                    cout << "___________________________________________          _______________________________________________" << endl;
                    cout << "|                                         / ENTRANCE \\                                             |" << endl;
                    cout << "|                                                                                                  |" << endl;
                    cout << "|       " << checkCourt("01", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("02", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("03", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("04", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("05", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("06", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("07", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("08", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("09", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "        " << checkCourt("10", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       |" << endl;
                    cout << "|                                                                                                  |" << endl;
                    cout << "|       " << checkCourt("11", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("12", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("13", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("14", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("15", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("16", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("17", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("18", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       " << checkCourt("19", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "        " << checkCourt("20", strStime, strFtime, chosen_year, chosen_month, chosen_day) << "       |" << endl;
                    cout << "|                                                                                                  |" << endl;
                    cout << "____________________________________________________________________________________________________" << endl;
                    cout << "\nNA: Not Available" << endl;
                    cout << "\nChoose which court would you like to book by entering the number: ";
                    cin >> Ccourt[multi];
                    Ccourt[multi] = morphDate(Ccourt[multi]);

                    if (Ccourt[multi] == "01" || Ccourt[multi] == "02" || Ccourt[multi] == "03" || Ccourt[multi] == "04" || Ccourt[multi] == "05" ||
                        Ccourt[multi] == "06" || Ccourt[multi] == "07" || Ccourt[multi] == "08" || Ccourt[multi] == "09" || Ccourt[multi] == "10" ||
                        Ccourt[multi] == "11" || Ccourt[multi] == "12" || Ccourt[multi] == "13" || Ccourt[multi] == "14" || Ccourt[multi] == "15" ||
                        Ccourt[multi] == "16" || Ccourt[multi] == "17" || Ccourt[multi] == "18" || Ccourt[multi] == "19" || Ccourt[multi] == "20")
                    {
                        for (int check = 0; check < multi; check++)
                        {
                            if (Ccourt[multi] == Ccourt[check])
                            {
                                cout << "You have already book Court " << Ccourt[multi] << ". Please select another court." << endl;
                                system("pause");
                                goto RCcourt;
                            }
                        }

                        checker = checkCourt(Ccourt[multi], strStime, strFtime, chosen_year, chosen_month, chosen_day);
                        if (checker == "NA")
                        {
                            cout << "Court " << Ccourt[multi] << " not available to book. Please choose another court." << endl;
                            system("pause");
                            goto RCcourt;
                        }
                        else
                        {
                        RCAskMulti:
                            system("cls");
                            cout << setw(45) << setfill('-') << right << "BOOKING INFO" << setw(45) << "-" << endl;
                            cout << "Name: " << custName << endl << endl;
                            cout << "Booking Date: " << chosen_day << "-" << chosen_month << "-" << chosen_year << endl << endl;
                            cout << "Start time: " << Stime << ":00" << endl << endl;
                            cout << "Finish time: " << Ftime << ":00" << endl << endl;
                            cout << "Duration: " << duration << " hours" << endl << endl;
                            cout << "Court: ";
                            for (int i = 0; i <= multi; i++)
                            {
                                cout << Ccourt[i] << " ";
                            }
                            cout << endl << endl;
                            cout << "Court Booked Successful! Would you like to: " << endl;
                            cout << "   (1) Add more court" << endl;
                            cout << "   (2) Continue to payment" << endl;
                            cout << "   Choose: ";
                            cin >> choice;
                            if (choice == "1")
                            {
                                multi++;
                                goto RCcourt;
                            }
                            else if (choice == "2")
                            {
                                for (int i = 0; i <= multi; i++)
                                {
                                    string checkUser_query = "SELECT * FROM court WHERE COURT_ID = 'BC" + Ccourt[i] + "'";
                                    const char* cu = checkUser_query.c_str();
                                    qstate = mysql_query(conn, cu);

                                    if (!qstate)
                                    {
                                        res = mysql_store_result(conn);
                                        if (res->row_count == 1)
                                        {
                                            while (row = mysql_fetch_row(res))
                                            {
                                                DBprice = row[1];
                                                price = price + atof(DBprice.c_str()) * duration;
                                            }
                                        }
                                        else
                                        {
                                            cout << "COURT NOT FOUND" << endl;
                                            system("pause");
                                        }
                                    }
                                    else
                                        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

                                }

                                PayID = Payment(price, id);

                                string checkUser_query = "SELECT BOOKED_ID FROM booking ORDER BY BOOKED_ID DESC LIMIT 0,1";
                                const char* cu = checkUser_query.c_str();
                                qstate = mysql_query(conn, cu);

                                if (!qstate)
                                {
                                    res = mysql_store_result(conn);
                                    if (res->row_count == 1)
                                    {
                                        while (row = mysql_fetch_row(res))
                                            last_booked_id = row[0];
                                    }
                                    else
                                    {
                                        cout << "Data not found.";
                                    }
                                }
                                else
                                {
                                    cout << "Query Execution Problem! find booking id!!" << mysql_errno(conn) << endl;
                                }

                                last_booked_id[0] = '0';
                                i_new_booked_id = stoi(last_booked_id) + 1;
                                new_booked_id = to_string(i_new_booked_id);
                                length_counter = 8 - new_booked_id.length();
                                for (int i = 0; i < length_counter; i++)
                                {
                                    new_booked_id = "0" + new_booked_id;
                                }
                                new_booked_id[0] = 'B';

                                string insertBookedCourt_query = "INSERT INTO booking (BOOKED_ID, CUST_ID , PAY_ID, BOOKED_DATE, BOOKED_STIME, BOOKED_ETIME, BOOKED_DURATION) values ('" + new_booked_id + "', '" + id + "', '" + PayID + "', '" + chosen_year + "-" + chosen_month + "-" + chosen_day + "', '" + strStime + ":00:00', '" + strFtime + ":00:00', '" + strduration + "')";
                                const char* q = insertBookedCourt_query.c_str();
                                qstate = mysql_query(conn, q);

                                if (!qstate)
                                {
                                    for (int i = 0; i <= multi; i++)
                                    {
                                        string insertCheckCourt_query = "INSERT INTO checkcourt (BOOKED_ID, COURT_ID) values ('" + new_booked_id + "', 'BC" + Ccourt[i] + "')";
                                        const char* q = insertCheckCourt_query.c_str();
                                        qstate = mysql_query(conn, q);
                                    }
                                    if (!qstate)
                                    {
                                        system("cls");
                                        cout << setw(90) << setfill('=') << "=" << endl;
                                        cout << setw(60) << setfill(' ') << right << "ECOSPORTS BADMINTON BOOKING" << endl;
                                        cout << setw(90) << setfill('=') << "=" << endl << endl;
                                        cout << setw(45) << setfill('-') << right << "BOOKING INFO" << setw(45) << "-" << endl;
                                        cout << "Name: " << custName << endl << endl;
                                        cout << "Booking Date: " << chosen_day << "-" << chosen_month << "-" << chosen_year << endl << endl;
                                        cout << "Start time: " << Stime << ":00" << endl << endl;
                                        cout << "Finish time: " << Ftime << ":00" << endl << endl;
                                        cout << "Duration: " << duration << " hours" << endl << endl;
                                        cout << "Court: ";
                                        for (int i = 0; i <= multi; i++)
                                        {
                                            cout << "BC" << Ccourt[i] << " ";
                                        }
                                        cout << endl;
                                        cout << setw(90) << setfill('-') << "-" << endl;
                                        cout << "BOOKING SUCCESSFUL! Return to main menu." << endl;
                                        cout << endl << endl;
                                        system("pause");
                                        Customer(id);
                                    }
                                    else
                                    {
                                        cout << "Query Execution Problem! insert check court" << mysql_errno(conn) << endl; //error
                                    }
                                }
                                else
                                {
                                    cout << "Query Execution Problem! insert booking" << mysql_errno(conn) << endl; //error
                                }
                            }
                            else
                            {
                                cout << "Please choose 1 or 2 only." << endl;
                                system("pause");
                                cout << endl;
                                goto RCAskMulti;
                            }
                        }
                    }
                    else
                    {
                        cout << "Enter the court number from 01 - 20 only. Please try again." << endl;
                        system("pause");
                        goto RCcourt;
                    }
                }
            }
            else if (strduration == "0")
            {
                goto RCtime;
            }
            else
            {
                cout << "Minimum duration is 1 hour and maximum duration is 6 hours. Please try again." << endl;
                system("pause");
                goto RCduration;
            }
        }
        else
        {
            if (Stime <= hour)
            {
                cout << "The starting time chosen has already passed. You can choose hour " << hour + 1 << ":00 and upwards. Please try again." << endl;
                system("pause");
                goto RCtime;
            }
            else
            {
                goto RCduration;
            }
        }
    }
    else if (Ctime == "0")
    {
        goto RCdate;
    }
    else
    {
        cout << "Please choose between 1 - 16. ";
        system("pause"); //pause console
        system("cls");
        goto RCtime;
    }
}

void DeleteBooking(string id)
{
    string strday = to_string(day);
    string strmonth = to_string(month);
    string stryear = to_string(year);

    int counter = 0, court_counter = 0, intchoice, i = 0;
    string date[50], stime[50], ftime[50], bookID[50], court[50], choice, Ccancel;

    for (int i = 0; i < 50; i++)
    {
        bookID[i] = "0";
    }

    string checkUser_query = "SELECT * FROM booking WHERE CUST_ID = '" + id + "' AND BOOKED_DATE >= '" + stryear + "-" + strmonth + "-" + strday + "' AND (BOOKED_DATE, BOOKED_ETIME, BOOKED_STIME) NOT IN (SELECT BOOKED_DATE, BOOKED_ETIME, BOOKED_STIME FROM booking WHERE BOOKED_DATE = '" + stryear + "-" + strmonth + "-" + strday + "' AND BOOKED_ETIME <= '" + strhour + ":00:00' OR BOOKED_DATE = '" + stryear + "-" + strmonth + "-" + strday + "' AND BOOKED_STIME <= '" + strhour + ":00:00') ORDER BY BOOKED_DATE ASC, BOOKED_STIME ASC";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count >= 1)
        {
        RCindex:
            system("cls");
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << setw(50) << setfill(' ') << right << "BOOKING CANCELLATION" << endl;
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << "Your active books:" << endl;
            cout << "       Date            Start Time       Finish Time      Court Number" << endl;
            while (row = mysql_fetch_row(res)) 
            {
                bookID[counter] = row[0];
                date[counter] = row[3];
                stime[counter] = row[4];
                ftime[counter] = row[5];
                counter++;
            }

        repeat:
            string checkUser_query = "SELECT * FROM checkcourt WHERE BOOKED_ID = '" + bookID[i] + "'";
            const char* cu = checkUser_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (res->row_count >= 1)
                {
                    cout << "  (" << i + 1 << ")" << setfill(' ') << setw(12) << date[i] << setw(14) << stime[i] << setw(17) << ftime[i] << setw(13);
                    while (row = mysql_fetch_row(res))
                    {
                        cout << row[1] << " ";
                    }
                    cout << endl;
                }
                else
                {
                    cout << "Data not found." << endl;
                }

                if (bookID[i + 1] != "0")
                {
                    i++;
                    goto repeat;
                }
                else
                {
                    cout << "\n  (0) BACK" << endl;
                    cout << "\n   Please choose which booking you would like CANCEL" << endl;
                    cout << "   Choose index: ";
                    cin >> choice;

                    if (choice == "1" || choice == "2" || choice == "3" || choice == "4" || choice == "5" ||
                        choice == "6" || choice == "7" || choice == "8" || choice == "9" || choice == "01" || choice == "02" || choice == "03" || choice == "04" || choice == "05" ||
                        choice == "06" || choice == "07" || choice == "08" || choice == "09" || choice == "10" ||
                        choice == "11" || choice == "12" || choice == "13" || choice == "14" || choice == "15" ||
                        choice == "16" || choice == "17" || choice == "18" || choice == "19" || choice == "20")
                    {
                        intchoice = stoi(choice);
                        if (intchoice > counter || intchoice < 1)
                        {
                            cout << "Invalid number, maximum number you can choose is " << counter << endl;
                            system("pause"); //pause console
                            goto RCindex;
                        }
                        else
                        {
                            string delete_query = "DELETE FROM checkcourt WHERE BOOKED_ID = '" + bookID[intchoice - 1] + "'";
                            const char* q = delete_query.c_str();
                            qstate = mysql_query(conn, q);

                            delete_query = "DELETE FROM booking WHERE BOOKED_ID = '" + bookID[intchoice - 1] + "'";
                            q = delete_query.c_str();
                            qstate = mysql_query(conn, q);

                        RCcancel:
                            system("cls");
                            cout << setw(90) << setfill('=') << "=" << endl;
                            cout << setw(50) << setfill(' ') << right << "BOOKING CANCELLATION" << endl;
                            cout << setw(90) << setfill('=') << "=" << endl;
                            cout << "   Cancel SUCCESSFUL! Would you like to cancel other booking?" << endl;
                            cout << "   (1) Yes, cancel more" << endl;
                            cout << "   (2) No, return to main menu" << endl;
                            cout << "   Choose: ";
                            cin >> Ccancel;

                            if (Ccancel == "1")
                                DeleteBooking(id);
                            else if (Ccancel == "2")
                                Customer(id);
                            else
                            {
                                cout << "Please choose 1 or 2 only. ";
                                system("pause"); //pause console
                                system("cls");
                                goto RCcancel;
                            }
                        }
                    }
                    else if (choice == "0")
                    {
                        Customer(id);
                    }
                    else
                    {
                        cout << "Invalid number, maximum number you can choose is " << counter << endl;
                        system("pause"); //pause console
                        system("cls");
                        goto RCindex;
                    }
                }
            }
            else
            {
                cout << "Print court error" << mysql_errno(conn) << endl;
            }
        }
        else
        {
            system("cls");
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << setw(50) << setfill(' ') << right << "BOOKING CANCELLATION" << endl;
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << "\nYOU HAVE NO ACTIVE BOOKING. RETURNING TO MAIN MENU" << endl;
            system("pause");
            Customer(id);
        }
    }
    else
    {
        cout << "Query Execution Problem! view booking" << mysql_errno(conn) << endl;
    }
}

void EditBooking(string id)
{
    string strday = to_string(day);
    string strmonth = to_string(month);
    string stryear = to_string(year);

    chrono::system_clock::time_point currentDate = chrono::system_clock::now();

    int counter, intchoice, intchoice2, chosen = 0, court_counter = 0, Stime, Ftime;
    string date[50], stime[50], ftime[50], bookID[50], court[50], duration[50], choice, Ccancel, choice1, choice2, Ccourt, checker, Ctime, strStime, strFtime;

    for (int i = 0; i < 50; i++)
    {
        bookID[i] = "0";
    }
    for (int i = 0; i < 50; i++)
    {
        court[i] = "0";
    }

    counter = 0;
    string checkUser_query = "SELECT * FROM booking WHERE CUST_ID = '" + id + "' AND BOOKED_DATE >= '" + stryear + "-" + strmonth + "-" + strday + "' AND (BOOKED_DATE, BOOKED_ETIME, BOOKED_STIME) NOT IN (SELECT BOOKED_DATE, BOOKED_ETIME, BOOKED_STIME FROM booking WHERE BOOKED_DATE = '" + stryear + "-" + strmonth + "-" + strday + "' AND BOOKED_ETIME <= '" + strhour + ":00:00' OR BOOKED_DATE = '" + stryear + "-" + strmonth + "-" + strday + "' AND BOOKED_STIME <= '" + strhour + ":00:00') ORDER BY BOOKED_DATE ASC, BOOKED_STIME ASC";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count >= 1)
        {
            system("cls");
            cout << "Current date and time: " << asctime(local_time) << endl;
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << setw(50) << setfill(' ') << right << "CHANGE BOOKING" << endl;
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << "Your active bookings:" << endl;
            cout << "       Date            Start Time       Finish Time      Court Number" << endl;
            while (row = mysql_fetch_row(res))
            {
                bookID[counter] = row[0];
                date[counter] = row[3];
                stime[counter] = row[4];
                ftime[counter] = row[5];
                duration[counter] = row[6];
                counter++;
            }

        repeat:
            string checkUser_query = "SELECT * FROM checkcourt WHERE BOOKED_ID = '" + bookID[chosen] + "'";
            const char* cu = checkUser_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (res->row_count >= 1)
                {
                    cout << "  (" << chosen + 1 << ")" << setfill(' ') << setw(12) << date[chosen] << setw(14) << stime[chosen] << setw(17) << ftime[chosen] << setw(13);
                    while (row = mysql_fetch_row(res))
                    {
                        cout << row[1] << " ";
                    }
                    cout << endl;
                }
                else
                {
                    cout << "Data not found." << endl;
                }

                if (bookID[chosen + 1] != "0")
                {
                    chosen++;
                    goto repeat;
                }
                else
                {
                    cout << "\n  (0) BACK" << endl;
                    cout << "\n   Please choose which booking you would like CHANGE" << endl;
                RCindex:
                    cout << "   Choose index: ";
                    cin >> choice; //bookID, date, time

                    if (choice == "1" || choice == "2" || choice == "3" || choice == "4" || choice == "5" ||
                        choice == "6" || choice == "7" || choice == "8" || choice == "9" || choice == "01" || choice == "02" || choice == "03" || choice == "04" || choice == "05" ||
                        choice == "06" || choice == "07" || choice == "08" || choice == "09" || choice == "10" ||
                        choice == "11" || choice == "12" || choice == "13" || choice == "14" || choice == "15" ||
                        choice == "16" || choice == "17" || choice == "18" || choice == "19" || choice == "20")
                    {
                        intchoice = stoi(choice);
                        if (intchoice > counter || intchoice < 1)
                        {
                            cout << "Invalid number, maximum number you can choose is " << counter << endl;
                            system("pause");
                            cout << endl;
                            goto RCindex;
                        }
                        else
                        {
                            string getCourt_query = "SELECT * FROM checkcourt WHERE BOOKED_ID = '" + bookID[intchoice - 1] + "'";
                            cu = getCourt_query.c_str();
                            qstate = mysql_query(conn, cu);

                            if (!qstate)
                            {
                                res = mysql_store_result(conn);
                                if (res->row_count >= 1)
                                {
                                    while (row = mysql_fetch_row(res))
                                    {
                                        court[court_counter] = row[1];
                                        court_counter++;
                                    }
                                }
                                else
                                {
                                    cout << "Court not found." << endl;
                                }
                            }
                            else
                                cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

                            //bookID[intchoice-1]
                            //court[intchoice2-1]
                        RCtime:
                            system("cls");
                            cout << "----------------BOOKING INFO----------------\n" << endl;
                            cout << "Booking Date: " << date[intchoice - 1] << endl << endl;
                            cout << "PREVIOUS Start time: " << stime[intchoice - 1] << endl << endl;
                            cout << "NOTE: The starting hour using the 24-hour format." << endl;
                            cout << "NEW or MAINTAIN Start time:" << endl;
                            cout << left << setfill(' ') << setw(13) << "(1) 08:00" << right << setw(13) << "(9) 16:00" << endl;
                            cout << left << setw(13) << "(2) 09:00" << right << setw(13) << "(10) 17:00" << endl;
                            cout << left << setw(13) << "(3) 10:00" << right << setw(13) << "(11) 18:00" << endl;
                            cout << left << setw(13) << "(4) 11:00" << right << setw(13) << "(12) 19:00" << endl;
                            cout << left << setw(13) << "(5) 12:00" << right << setw(13) << "(13) 20:00" << endl;
                            cout << left << setw(13) << "(6) 13:00" << right << setw(13) << "(14) 21:00" << endl;
                            cout << left << setw(13) << "(7) 14:00" << right << setw(13) << "(15) 22:00" << endl;
                            cout << left << setw(13) << "(8) 15:00" << right << setw(13) << "(16) 23:00" << endl;

                            cout << "Select a starting time: ";
                            cin >> Ctime;
                            if (Ctime == "1" || Ctime == "2" || Ctime == "3" || Ctime == "4" || Ctime == "5" || Ctime == "6" || Ctime == "7" || Ctime == "8" ||
                                Ctime == "9" || Ctime == "10" || Ctime == "11" || Ctime == "12" || Ctime == "13" || Ctime == "14" || Ctime == "15" || Ctime == "16")
                            {
                                Stime = stoi(Ctime);
                                Stime = Stime + 7;
                                
                                //run to trigger and test
                                if (isCurrentDate(date[intchoice - 1]) == true && Stime <= hour)
                                {
                                    cout << "The starting time chosen has already passed. You can choose hour " << hour + 1 << ":00 and upwards. Please try again." << endl;
                                    system("pause");
                                    goto RCtime;
                                }
                                else
                                {
                                RCupdatecourt:
                                    system("cls");
                                    cout << "Current date and time: " << asctime(local_time) << endl;
                                    cout << setw(90) << setfill('=') << "=" << endl;
                                    cout << setw(50) << setfill(' ') << right << "CHANGE BOOKING" << endl;
                                    cout << setw(90) << setfill('=') << "=" << endl;
                                    court_counter = 0;
                                RGetCourt:
                                    cout << "   (" << court_counter + 1 << ") " << court[court_counter];
                                    if (court[court_counter + 1] != "0")
                                    {
                                        court_counter++;
                                        cout << endl;
                                        goto RGetCourt;
                                    }
                                    else
                                    {
                                        cout << "\n   (0) MAINTAIN" << endl;
                                        cout << "\n   Choose which court to change: ";
                                        cin >> choice2; //court number
                                        if (choice2 == "0" || choice2 == "1" || choice2 == "2" || choice2 == "3" || choice2 == "4" || choice2 == "5" ||
                                            choice2 == "6" || choice2 == "7" || choice2 == "8" || choice2 == "9" || choice2 == "01" || choice2 == "02" || choice2 == "03" || choice2 == "04" || choice2 == "05" ||
                                            choice2 == "06" || choice2 == "07" || choice2 == "08" || choice2 == "09" || choice2 == "10" ||
                                            choice2 == "11" || choice2 == "12" || choice2 == "13" || choice2 == "14" || choice2 == "15" ||
                                            choice2 == "16" || choice2 == "17" || choice2 == "18" || choice2 == "19" || choice2 == "20")
                                        {
                                            if (choice2 == "0")
                                            {
                                                Ccourt = court[intchoice2 - 1];
                                                Ftime = Stime + stoi(duration[intchoice - 1]);
                                                strStime = to_string(Stime);
                                                strFtime = to_string(Ftime);
                                                goto skipCourt;
                                            }

                                            intchoice2 = stoi(choice2);
                                            if (intchoice2 > court_counter+1 || intchoice2 < 1)
                                            {
                                                cout << "Invalid number, maximum number you can choose is " << court_counter << endl;
                                                system("pause");
                                                goto RCupdatecourt;
                                            }
                                            else
                                            {
                                            RCcourt:
                                                Ftime = Stime + stoi(duration[intchoice - 1]);
                                                strStime = to_string(Stime);
                                                strFtime = to_string(Ftime);

                                                system("cls");
                                                cout << setw(90) << setfill('=') << "=" << endl;
                                                cout << setw(50) << setfill(' ') << right << "CHANGE BOOKING" << endl;
                                                cout << setw(90) << setfill('=') << "=" << endl;
                                                cout << R"(Booking Date: )" << date[intchoice - 1] << endl << endl;
                                                cout << R"(Start time: )" << Stime << ":00" << endl << endl;
                                                cout << R"(Finish time: )" << Ftime << ":00" << endl << endl;
                                                cout << R"(Duration: )" << duration[intchoice - 1] << " hours" << endl << endl;
                                                cout << "Court layout:" << endl;
                                                cout << "___________________________________________          _______________________________________________" << endl;
                                                cout << "|                                         / ENTRANCE \\                                             |" << endl;
                                                cout << "|                                                                                                  |" << endl;
                                                cout << "|       " << checkCourt("01", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("02", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("03", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("04", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("05", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("06", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("07", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("08", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("09", strStime, strFtime, date[intchoice - 1]) << "        " << checkCourt("10", strStime, strFtime, date[intchoice - 1]) << "       |" << endl;
                                                cout << "|                                                                                                  |" << endl;
                                                cout << "|       " << checkCourt("11", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("12", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("13", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("14", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("15", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("16", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("17", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("18", strStime, strFtime, date[intchoice - 1]) << "       " << checkCourt("19", strStime, strFtime, date[intchoice - 1]) << "        " << checkCourt("20", strStime, strFtime, date[intchoice - 1]) << "       |" << endl;
                                                cout << "|                                                                                                  |" << endl;
                                                cout << "____________________________________________________________________________________________________" << endl;
                                                cout << "\nNA: Not Available" << endl;
                                                cout << "\nChoose which court would you like to book by entering the number: ";
                                                cin >> Ccourt;
                                                Ccourt = morphDate(Ccourt);

                                                if (Ccourt == "01" || Ccourt == "02" || Ccourt == "03" || Ccourt == "04" || Ccourt == "05" ||
                                                    Ccourt == "06" || Ccourt == "07" || Ccourt == "08" || Ccourt == "09" || Ccourt == "10" ||
                                                    Ccourt == "11" || Ccourt == "12" || Ccourt == "13" || Ccourt == "14" || Ccourt == "15" ||
                                                    Ccourt == "16" || Ccourt == "17" || Ccourt == "18" || Ccourt == "19" || Ccourt == "20")
                                                {
                                                    checker = checkCourt(Ccourt, strStime, strFtime, date[intchoice - 1]);
                                                    if (checker == "NA")
                                                    {
                                                        cout << "Court " << Ccourt << " not available to book. Please choose another court." << endl;
                                                        system("pause");
                                                        goto RCcourt;
                                                    }
                                                    else
                                                    {
                                                    skipCourt:
                                                        string update_query = "UPDATE checkcourt SET COURT_ID = 'BC" + Ccourt + "' WHERE BOOKED_ID = '" + bookID[intchoice - 1] + "' AND COURT_ID = '" + court[intchoice2 - 1] + "'";
                                                        const char* q = update_query.c_str();
                                                        qstate = mysql_query(conn, q);

                                                        update_query = "UPDATE booking SET BOOKED_STIME = '" + strStime + ":00:00', BOOKED_ETIME = '" + strFtime + ":00:00' WHERE BOOKED_ID = '" + bookID[intchoice - 1] + "'";
                                                        q = update_query.c_str();
                                                        qstate = mysql_query(conn, q);

                                                        cout << "   UPDATE SUCCESSFUL!" << endl;
                                                        system("pause");
                                                        Customer(id);
                                                    }
                                                }
                                                else
                                                {
                                                    cout << "Enter the court number from 01 - 20 only. Please try again." << endl;
                                                    system("pause");
                                                    goto RCcourt;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            cout << "Invalid Number. Try again." << endl;
                                            system("pause");
                                            goto RCupdatecourt;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                cout << "Enter the start time from 1 - 16 only. Please try again." << endl;
                                system("pause");
                                goto RCtime;
                            }
                        }
                    }
                    else if (choice == "0")
                    {
                        Customer(id);
                    }
                    else
                    {
                        cout << "Invalid number, maximum number you can choose is " << counter << endl;
                        system("pause"); //pause console
                        system("cls");
                        goto RCindex;
                    }
                }
            }
            else
            {
                cout << "Print court error" << mysql_errno(conn) << endl;
            }
        }
        else
        {
            system("cls");
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << setw(50) << setfill(' ') << right << "CHANGE BOOKING" << endl;
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << "\nYOU HAVE NO ACTIVE BOOKING. RETURNING TO MAIN MENU" << endl;
            system("pause");
            Customer(id);
        }
    }
    else
    {
        cout << "Query Execution Problem! view booking" << mysql_errno(conn) << endl;
    }
}

void ViewBooking(string id)
{
    int min = local_time->tm_min;
    
    string strday = to_string(day);
    string strmonth = to_string(month);
    string stryear = to_string(year);

    string strhour = to_string(hour);

    int counter = 0, court_counter = 0, i = 0;
    string date[50], stime[50], ftime[50], bookID[50], court[50], choice, Ccancel, custName;

    for (int i = 0; i < 50; i++)
    {
        bookID[i] = "0";
    }

    string checkCustomer_query = "SELECT * FROM customer WHERE CUST_ID = '" + id + "'";
    const char* cu = checkCustomer_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
                custName = row[3];
        }
        else
        {
            cout << "Customer not found" << endl;
        }
    }
    else
        cout << "Query Execution Problem! find customer" << mysql_errno(conn) << endl;


    string checkUser_query = "SELECT * FROM booking WHERE CUST_ID = '" + id + "' AND BOOKED_DATE >= '" + stryear + "-" + strmonth + "-" + strday + "' AND (BOOKED_DATE, BOOKED_ETIME) NOT IN (SELECT BOOKED_DATE, BOOKED_ETIME FROM booking WHERE BOOKED_DATE = '" + stryear + "-" + strmonth + "-" + strday + "' AND BOOKED_ETIME < '" + strhour + ":00:00') ORDER BY BOOKED_DATE ASC, BOOKED_STIME ASC";
    cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count >= 1)
        {
            system("cls");
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << setw(40) << setfill(' ') << right << "VIEW BOOKINGS FOR " << custName << endl;
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << "Your active bookings:" << endl;
            cout << "       Date            Start Time       Finish Time      Court Number" << endl;
            while (row = mysql_fetch_row(res))
            {
                bookID[counter] = row[0];
                date[counter] = row[3];
                stime[counter] = row[4];
                ftime[counter] = row[5];
                counter++;
            }

        repeat:
            string checkUser_query = "SELECT * FROM checkcourt WHERE BOOKED_ID = '" + bookID[i] + "'";
            const char* cu = checkUser_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (res->row_count >= 1)
                {
                    cout << "   " << i + 1 << "." << setfill(' ') << setw(12) << date[i] << setw(14) << stime[i] << setw(17) << ftime[i] << setw(13);
                    while (row = mysql_fetch_row(res))
                    {
                        cout << row[1] << " ";
                    }
                    cout << endl;
                }
                else
                {
                    cout << "Data not found." << endl;
                }

                if (bookID[i + 1] != "0")
                {
                    i++;
                    goto repeat;
                }
                else
                {
                    system("pause");
                    Customer(id);
                }
            }
            else
            {
                cout << "Print court error" << mysql_errno(conn) << endl;
            }
        }
        else
        {
            system("cls");
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << setw(50) << setfill(' ') << right << "VIEW BOOKINGS" << endl;
            cout << setw(90) << setfill('=') << "=" << endl;
            cout << "\nYOU HAVE NO ACTIVE BOOKING. RETURNING TO MAIN MENU" << endl;
            system("pause");
            Customer(id);
        }
    }
    else
    {
        cout << "Query Execution Problem! view booking" << mysql_errno(conn) << endl;
    }
}

void Staff(string id)
{
    string choice;

staff_menu:
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(50) << setfill(' ') << right << "STAFF PAGE" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << "\n   (1) Customer With No Booking" << endl;
    cout << "   (2) Monthly Sales" << endl;
    cout << "   (3) Frequently Booked Court" << endl;
    cout << "   (4) Turn ON/OFF Court for Maintenance" << endl;
    cout << "   (5) Search Booking" << endl;
    cout << "   (6) Log out" << endl;
    cout << "   Choose: ";
    cin >> choice;

    if (choice == "1")
        CustomerReport(id);
    else if (choice == "2")
        SalesReport(id);
    else if (choice == "3")
        CourtReport(id);
    else if (choice == "4")
        CourtStatus(id);
    else if (choice == "5")
        SearchBooking(id);
    else if (choice == "6")
    {
        cout << "LOG OUT SUCCESSFUL" << endl;
        system("pause");
        main();
    }
    else
    {
        cout << "Please choose between 1 - 4 only. ";
        system("pause"); //pause console
        system("cls");
        goto staff_menu;
    }
}

void CustomerReport(string id)
{
    int counter = 0;
    
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(60) << setfill(' ') << right << "List Of Customers With No Booking" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;

    string checkUser_query = "SELECT * FROM customer WHERE CUST_ID NOT IN (SELECT CUST_ID FROM booking)";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (!qstate)
        {
            cout << left << setw(12) << setfill(' ') << "ID" << "| " << setw(20) << "NAME" << "| " << setw(35) << "EMAIL" << "| " << setw(16) << "PHONE NUMBER" << "|" << endl;
            cout << setw(90) << setfill('-') << "-" << endl;
            while (row = mysql_fetch_row(res))
            {
                cout << left << setw(12) << setfill(' ') << row[0] << "| " << setw(20) << fixed << setprecision(2) << row[3] << "| " << setw(35) << row[1] << "| " << setw(16) << row[4] << "|" << endl;
                cout << setw(90) << setfill('-') << "-" << endl;
                counter++;
            }
            cout << setw(30) << setfill(' ') <<"Number of customer with no booking: " << setw(53) << counter << "|" << endl;
            cout << setw(90) << setfill('-') << "-" << endl;
        }
    }
    cout << endl;
    system("pause");
    Staff(id);
}

void SalesReport(string id)
{
    double total[12] = {0};
    string stryear = to_string(year);
    string max, strmonth, strprice;
    const char* cu;
    string checkPayment_query;
    
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(50) << right << setfill(' ') << "MONTHLY SALES FOR YEAR " << left << year << endl;
    cout << setw(90) << setfill('=') << "=" << endl;

    for (int i = 0; i < 12; i++)
    {
        max = to_string(numberOfDays(i + 1, year));
        strmonth = to_string(i + 1);

        checkPayment_query = "SELECT * FROM payment WHERE PAY_DATE >= '" + stryear + "-" + strmonth + "-1' AND PAY_DATE <= '" + stryear + "-" + strmonth + "-" + max + "'";
        cu = checkPayment_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            if (res->row_count >= 1)
            {
                while (row = mysql_fetch_row(res))
                {
                    strprice = row[1];
                    total[i] = total[i] + stod(strprice);
                }
            }
            else
            {
                total[i] = total[i] + 0;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
        }
    }
    cout << left << setw(35) << setfill(' ') << "MONTH" << "| " << setw(52) << fixed << setprecision(2) << "TOTAL" << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "JANUARY" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[0] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "FEBRUARY" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[1] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "MARCH" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[2] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "APRIL" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[3] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "MAY" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[4] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "JUNE" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[5] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "JULY" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[6] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "AUGUST" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[7] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "SEPTEMBER" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[8] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "OCTOBER" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[9] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "NOVEMBER" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[10] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    cout << left << setw(35) << setfill(' ') << "DECEMBER" << "| " << setw(2) << "RM " << setw(49) << fixed << setprecision(2) << total[11] << "| " << endl;
    cout << setw(90) << setfill('-') << "-" << endl;
    system("pause");
    Staff(id);
}

void CourtReport(string id)
{
    int freqcourt[COURT_SIZE] = {0}, count = 0, total = 0;
    string court[COURT_SIZE];

    const char* cu;
    string countCourt_query, getCourt_query;
    
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(56) << setfill(' ') << "FREQUENTLY CHOSEN COURTS" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;

    getCourt_query = "SELECT * FROM court";
    cu = getCourt_query.c_str();
    qstate = mysql_query(conn, cu);
    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count >= 1)
        {
            while (row = mysql_fetch_row(res))
            {
                court[count] = row[0];
                count++;
            }
        }
        else
            cout << "Data Not Found" << endl;
    }
    else
        cout << "Query Execution Problem! get court" << mysql_errno(conn) << endl;

    for (int i = 0; i < COURT_SIZE; i++)
    {
        countCourt_query = "SELECT * FROM checkCourt WHERE COURT_ID = '" + court[i] + "'";
        cu = countCourt_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            if (res->row_count >= 1)
            {
                while (row = mysql_fetch_row(res))
                {
                    freqcourt[i] = freqcourt[i] + 1;
                    total = total + 1;
                }
            }
        }
    }

    for (int i = 0; i < COURT_SIZE; i++)
    {
        cout << left << setw(10) << setfill(' ') << court[i] << "| ";
        for (int j = 0; j < freqcourt[i]; j++)
        {
            cout << "**";
        }
        cout << endl;
    }
    cout << setw(90) << setfill('_') << "_" << endl;
    cout << "CUMULATIVE COURT BOOKINGS ALL TIME: " << total << endl << endl;
    system("pause");
    Staff(id);
}

void CourtStatus(string id)
{
    string court, status;
    
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(56) << setfill(' ') << "TOGGLE COURT STATUS FOR MAINTENANCE" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;

    string checkCourt_query = "SELECT * FROM court";
    const char* cu = checkCourt_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (!qstate)
        {
            cout << left << setw(45) << setfill(' ') << "COURT ID" << "| " << setw(42) << "STATUS" << "| " << endl;
            cout << setw(90) << setfill('-') << "-" << endl;
            while (row = mysql_fetch_row(res))
            {
                cout << left << setw(45) << setfill(' ') << row[0] << "| " << setw(42) << row[2] << "| " << endl;
                cout << setw(90) << setfill('-') << "-" << endl;
            }
        }
    }
    cout << "(0) BACK" << endl;
    cout << "Enter the Court ID that you would like to toggle the status: ";
    cin >> court;

    if (court == "0")
    {
        Staff(id);
    }
    else
    {
        string checkCourt_query = "SELECT * FROM court WHERE COURT_ID = '" + court + "'";
        const char* cu = checkCourt_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            if (res->row_count == 1)
            {
                while (row = mysql_fetch_row(res))
                    status = row[2];

                if (status == "ON")
                {
                    string update_query = "UPDATE court SET COURT_STATUS = 'OFF' WHERE COURT_ID = '" + court + "'";
                    const char* q = update_query.c_str();
                    qstate = mysql_query(conn, q);
                }
                else
                {
                    string update_query = "UPDATE court SET COURT_STATUS = 'ON' WHERE COURT_ID = '" + court + "'";
                    const char* q = update_query.c_str();
                    qstate = mysql_query(conn, q);
                }
                CourtStatus(id);
            }
            else
            {
                cout << "Invalid Court ID, please try again." << endl;
                system("pause");
                CourtStatus(id);
            }
        }
        else
            cout << "Query Execution Problem!" << "select court" << mysql_errno(conn) << endl;
    }
}

void SearchBooking(string id)
{
    string date, choice;
    
    system("cls");
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << setw(56) << setfill(' ') << "SEARCH BOOKING USING DATE" << endl;
    cout << setw(90) << setfill('=') << "=" << endl;
    cout << "(0) BACK" << endl;
    cout << "Please enter the booking date you would like to search(YYYY-MM-DD): ";
    cin >> date;
    if (date == "0")
        Staff(id);

    string checkUser_query = "SELECT A.CUST_NAME, B.BOOKED_STIME, B.BOOKED_ETIME, B.BOOKED_DURATION, C.COURT_ID FROM booking AS B INNER JOIN customer AS A ON B.CUST_ID = A.CUST_ID INNER JOIN checkcourt AS C ON B.BOOKED_ID = C.BOOKED_ID WHERE B.BOOKED_DATE = '" + date + "' ORDER BY B.BOOKED_STIME ASC";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count >= 1)
        {
            cout << endl;
            cout << setw(90) << setfill('-') << "-" << endl;
            cout << left << setw(30) << setfill(' ') << "Customer Name" << "| " << setw(13) << "Start time" << "| " << setw(13) << "Finish Time" << "| " << setw(10) << "Duration" << "| " << setw(15) << "Court" << "| " << endl;
            cout << setw(90) << setfill('-') << "-" << endl;
            while (row = mysql_fetch_row(res))
            {
                cout << left << setw(30) << setfill(' ') << row[0] << "| " << setw(13) << row[1] << "| " << setw(13) << row[2] << "| " << setw(10) << row[3] << "| " << setw(15) << row[4] << "| " << endl;
                cout << setw(90) << setfill('-') << "-" << endl;
            }
            system("pause");
            Staff(id);
        }
        else
        {
            cout << "   No Booking found on " << date << "." << endl;
            cout << "   (1) Try Again" << endl;
            cout << "   (2) Main Menu" << endl;
        RCchoice:
            cout << "   Choose: " << endl;
            cin >> choice;
            if (choice == "1")
            {
                SearchBooking(id);
            }
            else if (choice == "2")
            {
                Staff(id);
            }
            else
            {
                cout << "Please choose between 1 - 2 only. ";
                system("pause"); //pause console
                goto RCchoice;
            }
        }
    }
}