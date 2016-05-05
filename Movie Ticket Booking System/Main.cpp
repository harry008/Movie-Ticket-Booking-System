#include <iostream>
//#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <wincon.h>
#include <iomanip>
#include <string.h>
#include <string>
using namespace std;

/*
* SQLite3 header file
* for performing database operations.
*/

#include "inc\sqlite3.h"

// My wrapper but with exceptions
// #include "SQLite.h"

/*
* Constants
*/

// DB File Name
#define DB_NAME "./mtbs.db"

// Error / Success codes
#define SUCCESS 200
#define FAIL 500
//#define NOT_FOUND 404

/**
* Globals
*/

sqlite3 * conn = NULL;
sqlite3_stmt * stmt = NULL;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void showErrAndCloseConn() {
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("\n%s\n", sqlite3_errmsg(conn));
	sqlite3_finalize(stmt);
	sqlite3_close(conn);
	SetConsoleTextAttribute(h, EVENT_SYSTEM_FOREGROUND | FOREGROUND_INTENSITY);

}

void printline(int l = 30, char c = '-') {
	cout << setw(1) << "+";
	for (int i = 1; i <= l; i++)
		cout << c;
	cout << "+";
}

void printHeader() {
	cout << endl;

	SetConsoleTextAttribute(h, BACKGROUND_GREEN | FOREGROUND_INTENSITY);
	printline(39);
	cout << endl;
	cout << setfill(' ') << setw(1) << "|" << "\tMovie Ticket Booking System" << setw(6)<< right << " |" << endl;
	printline(39);
	SetConsoleTextAttribute(h, EVENT_SYSTEM_FOREGROUND | FOREGROUND_INTENSITY);
	cout << endl;
}

void MainMenu();
void adminMenu();
void MovieMenu();



/***************************************************
* Admin Class
*********************************************************/
class Admin {
	long int id;
	char * username;
	
public:
	int login();
	long int getId() {
		return id;
	}
	int changeCategoryPrices();
	int logout();
};

int Admin::login() {
	char username[50], password[50];
	string pass;
	char c;
	printHeader();
	cout << setw(20) << "Enter Username: ";
	cin >> username;
	cout << setw(20) << "Enter Password: ";
	c = _getch();
	while (c != 13)
	{
		pass.push_back(c);
		cout << "*";
		c = _getch();
	}
	int res = sqlite3_open(DB_NAME, &conn);

	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return res;
	}

	char * sql = "select id, username from Admin where username = ? and password = ?";
	res = sqlite3_prepare_v2(conn,
		sql,
		-1,
		&stmt,
		NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_text(stmt, 1, username, strlen(username), SQLITE_STATIC);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	strcpy_s(password, pass.c_str());
	res = sqlite3_bind_text(stmt, 2, password, strlen(password), NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	if (SQLITE_ROW == sqlite3_step(stmt)) {
		cout << endl << "Login Success\n";
		id = sqlite3_column_int(stmt, 0);
		
		//cout<< endl << sqlite3_column_text(stmt, 0) << "\t" << sqlite3_column_text(stmt, 1) << endl;
	} else {
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << endl;
		cout << setfill (' ') << setw(5) << " " <<  setw(20) << "Wrong username or password. Exiting now" << endl;
		return FAIL;
	}
	sqlite3_finalize(stmt);
	sqlite3_close(conn);
	return SUCCESS;
}
int Admin::logout() {
	system("cls");
	cout << endl << setw(20) << "\tLogged out succesfully. Exiting in 3 seconds..";
	Sleep(3000);
	exit(0);
}

int Admin::changeCategoryPrices() {
	int id;
	char * sql = "SELECT id, name FROM Category;";
	int res = sqlite3_open(DB_NAME, &conn);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	system("cls");
	printHeader();
	cout << endl;
	printline();
	cout << endl;
	cout << setfill(' ') << setw(1) << left <<  "|" << setw(5) << right << "ID" << setw(2) << right << " " << setw(1) << right << "|" << setw(20) << right << "Category Name"<< setw(2) << " " << setw(1)<< "|"<< endl;
	printline();
	cout << endl;
	while (SQLITE_ROW == sqlite3_step(stmt)) {		
		cout << setfill(' ')<< setw(1) << left << "|" << setw(5) << right <<  sqlite3_column_int(stmt, 0) << setw(2) << " " <<  setw(1) << right << "|" << setw(20) << right << sqlite3_column_text(stmt, 1) << setw(2) << " " << setw(1) << "|" << endl;
		printline();
		cout << endl;
	}
	sqlite3_finalize(stmt);
	cout << endl;
	cout << setw(15) <<  "Enter ID of category you want to update price : ";
	cin >> id;

	if (id > 0 && id <= 4) {
		double price;
		cout << "Enter New Price: ";
		cin >> price;

		sql = "UPDATE Category SET price=? WHERE id=?";
		res = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_bind_double(stmt, 1, price);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_bind_int(stmt, 2, id);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_step(stmt);
		if (SQLITE_DONE == res) {
			cout << endl << "Price Changed Successfully.\n";
		}
		else {
			showErrAndCloseConn();
			return FAIL;
		}
		sqlite3_finalize(stmt);
		sqlite3_close(conn);
		return SUCCESS;
	}
	else {
		cout << "\nEnter Proper ID of the category. Category of this ID does not exist in this system.\n";
		return FAIL;
	}
}

/***********************	Admin Over    **********************/


/************************** 	Movie Module    **********************************/
class Movie {
	long int id;
	string title;
	string release_date;
	int len;
	int insertInDB(const char *, const char *, const int);
public:
	void init();
	long int getId() {
		return id;
	}
	char const * getMovieTitle() {
		return title.c_str();
	}
	int addNewMovie();
	int changeMovie();
	int updateMovie();
	
	friend int operator >> (istream &, Movie &);
	friend int operator << (ostream &, Movie &);
};
void Movie::init() {

	int res = sqlite3_open(DB_NAME, &conn);
	int movieCount = 0;
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return;
	}
	char * sqlCountMovies = "SELECT COUNT(*) from Movies;";
	res = sqlite3_prepare_v2(conn, sqlCountMovies, -1, &stmt, NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return;
	}
	res = sqlite3_step(stmt);
	if (SQLITE_ROW == res) {
		movieCount = sqlite3_column_int(stmt, 0);
	}
	else {
		showErrAndCloseConn();
		return;
	}
	if (movieCount == 0) {
		char c;
		system("cls");
		printHeader();
		cout << endl << "There are no movies in the system. Please enter the movie details.";
		cout << endl << "Want to enter a new movie detail? (y/n): ";
		c = _getch();
		if (c == 'y' || c == 'Y') {
			sqlite3_finalize(stmt);
			sqlite3_close(conn);
			addNewMovie();
		}
	}
	else {
		sqlite3_finalize(stmt);
		char * sql = "select * from Movies ORDER BY id DESC LIMIT 0,1;";
		res = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return;
		}
		res = sqlite3_step(stmt);
		if (SQLITE_ROW == res) {
			id = sqlite3_column_int(stmt, 0);
			title = string(
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
				);
			release_date = string(
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
				);
			len = sqlite3_column_int(stmt, 3);
		}
		sqlite3_finalize(stmt);
		sqlite3_close(conn);
	}
	sqlite3_close(conn);
}


int operator >> (istream &din, Movie &m) {
	fflush(stdin);
	system("cls");
	printHeader();
	cout << endl << "Enter Movie Title: ";
	din >> m.title;
	cout << "Enter Release Date (DD/MM/YYYY): ";
	din >> m.release_date;
	cout << "Enter Movie Length (in minutes): ";
	din >> m.len;
	return 1;
}
int operator << (ostream & dout, Movie &c) {
	dout << endl;
	printline(44, '-');
	dout << endl;
	dout << setfill(' ') << setw(1) << left << "|" << setw(15) << "Title: " << setw(15) << c.title << setw(15) << right << "|" << endl;
	dout << setfill(' ') << setw(1) << left << "|" << setw(15) << "Release Date: " << setw(15)<< c.release_date << setw(15) << right << "|" << endl;
	dout << setfill(' ') << setw(1) << left << "|" << setw(15) << "Length: " << setw(2) <<  c.len << " minutes" << setw(19) << right << "|" << endl;
	printline(44, '-');
	cout << endl;
	return 1;
}
int Movie::addNewMovie() {
	system("cls");
	string t , reldate;
	int len;
	fflush(stdin);
	printHeader();
	cout << endl;
	cin.ignore();
	cout << setw(15) << "Enter Movie Title: ";
	getline (cin, t);
	cout << setw(15) << "Enter Release Date (YYYY-MM-DD): ";
	getline(cin, reldate);
	fflush(stdin);
	cout << "Enter Movie Length (in minutes): ";
	cin >> len;
	fflush(stdin);

	const char *title = t.c_str();
	const char *release_date = reldate.c_str();

	if (SUCCESS != insertInDB(title, release_date, len)) {
		cout << "\nThere was some problem. Could not add new movie.\n";
		return FAIL;
	}
	return SUCCESS;
}

int Movie::updateMovie() {
	system("cls");
	printHeader();
	cout << setw(15)<< "Update The Current Movie Details\n";
	cin >> *this;	
	int res = sqlite3_open(DB_NAME, &conn);
	if (SQLITE_OK != res) {
		printf("\n%s\n", sqlite3_errmsg(conn));
		sqlite3_close(conn);
		return FAIL;
	}
	char const * sql = "UPDATE Movies SET title = ? AND release_date = ? AND len = ? WHERE id = ?;";
	res = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_text(stmt, 1, title.c_str(), strlen(title.c_str()), NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_text(stmt, 2, release_date.c_str(), strlen(release_date.c_str()), NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 3, len);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 4, id);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_step(stmt);
	if (SQLITE_DONE != res) {
		showErrAndCloseConn();
		cout << endl << "Some error occurred";
		return FAIL;
	}
	cout << endl << "Movie Details Updated Successfully."<< endl;
	sqlite3_finalize(stmt);
	sqlite3_close(conn);
	return SUCCESS;
}
int Movie::insertInDB(const char *t, const char *rel_date, const int l) {
	int res = sqlite3_open(DB_NAME, &conn);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	char * sql = "INSERT INTO movies (title, release_date, length) VALUES (?, ?, ?)";
	res = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_text(stmt, 1, t, strlen(t), NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}

	res = sqlite3_bind_text(stmt, 2, rel_date, strlen(rel_date), NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 3, l);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}	
	res = sqlite3_step(stmt);
	if (SQLITE_DONE != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	cout << endl << "Movie Inserted in DB" << endl;
	id = (int)sqlite3_last_insert_rowid(conn);
	sqlite3_finalize(stmt);
	sqlite3_close(conn);
	return SUCCESS;
}
int Movie::changeMovie() {
	int ch;
	system("cls");
	printHeader();
	cout << endl;
	cout << setw(15) << "New Movie or Old(1/2): ";
	cin >> ch;
	if (ch == 1) {
		if (SUCCESS != addNewMovie()) {
			cout << "\nThere was some problem.";
			return FAIL;
		}
		return SUCCESS;
	}
	if (ch == 2) {
		int res = sqlite3_open(DB_NAME, &conn);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		char * sql = "SELECT id, title FROM movies ORDER BY id DESC LIMIT 0, 3";
		res = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		int i = 1;
		while (SQLITE_ROW == sqlite3_step(stmt)) {
			if (i == 1) {
				cout << endl;
				printline();
				cout << endl;
				cout << setfill(' ') << setw(1) << left << "| "  << "Movie ID " << setw(10) <<"Movie Title" << setw(15)<< right <<  setw(1) <<" |" << endl;
				printline();
				cout << endl;
				cout << setfill(' ')<< left << sqlite3_column_int(stmt, 0)<< " " << setw(10) << sqlite3_column_text(stmt, 1) << right << endl;
				printline();
				cout << endl;
			} else {

				cout << left << sqlite3_column_int(stmt, 0) << " " << setw(10) << sqlite3_column_text(stmt, 1) << right << endl;
				printline();
				cout << endl;
			}
			i++;
		}
		sqlite3_finalize(stmt);
		
		int mv_id;
		cout << endl << "Enter ID of Movie from above list: ";
		cin >> mv_id;
		
		sql = "SELECT * FROM movies WHERE id=?";
		res = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_bind_int(stmt, 1, mv_id);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_step(stmt);
		if (SQLITE_ROW == res) {
			id = sqlite3_column_int(stmt, 0);
			title = string(
				reinterpret_cast<const char *>((stmt, 1)));
			release_date = string(
				reinterpret_cast<const char *>((stmt, 2)));
			len = sqlite3_column_int(stmt, 3);
		}
		sqlite3_finalize(stmt);
		sqlite3_close(conn);
		return SUCCESS;
	}
	return FAIL;
}

/*******************************   Movie Module over    *******************************/

/*****************************
* Ticket Module
******************************/

/** Maximum seat allocation per category  **/
#define MAX_SEATS_CAT1 30
#define MAX_SEATS_CAT2 30
#define MAX_SEATS_CAT3 30
#define MAX_SEATS_CAT4 30

class  Ticket {
	Movie m;
	int id;
	int seat_no;
	int category;
    double price;
	int show_num;
	double total;
	int screen;
	string catName;
	string date;
	void print_ticket();
public:
	Ticket(){}
	Ticket(Movie movie) {
		m = movie;
	}
	int book_ticket();
	friend int operator >>(istream &, Ticket &);
};
int operator >> (istream &din, Ticket &t) {
	system("cls");
	printHeader();

	cout << endl;
	cout << setw(15) << "Enter Ticket Details" << endl;
	
	cout << endl;
	cout << "Categories\n";
	cout << "1. Silver\n2. Gold\n3. Diamond\n4. Balcony\n";
	cout << setw(15) << "Enter Category: ";
	din >> t.category;
	if (t.category <= 0 || t.category > 4) {
		cout << "\nInvalid Category\n";
		return 0;
	}
	cout << setw(15) << "Enter Screen Number: ";
	din >> t.screen;
	if (t.screen >= 3 || t.screen <= 0)
	{
		cout << "\nInvalid Screen Number\n";
		return 0;
	}
	cout << "\nShow Number and Timings";
	cout << "\n1. 12-3pm\n2. 3-6pm\n3. 6-9pm\n4. 9-12am\n";
	cout << setw(15) << "Enter Show Number: ";
	din >> t.show_num;
	if(t.show_num <= 0 || t.show_num > 5){
		cout << "\nInvalid Show Number\n";
		return 0;
	}
	return 1;
}
int Ticket::book_ticket() {
	seat_no = 0;
	int n, cnt = 0;
	std::cout << "How Many Tickets you want to buy: ";
	cin >> n;
	if (n <= 0) return FAIL;

	// close database even if its close or open then again open it
	sqlite3_close(conn);

	int res = sqlite3_open(DB_NAME, &conn);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	// get number of seats occupied
	const char * countSeatsSQL = "SELECT COUNT(*), DATE('now') AS date from Ticket_Master WHERE movie_id= ? AND category_id= ? AND screen_id = ? AND show_num = ? AND date=DATE('now');";
	res = sqlite3_prepare_v2(conn, countSeatsSQL, -1, &stmt, NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 1, m.getId());
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 2, category);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 3, screen);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 4, show_num);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_step(stmt);
	if (SQLITE_ROW == res) {
		cnt = sqlite3_column_int(stmt, 0);
		date = string(
			reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))
			);
		if (category == 1) {
			if ((MAX_SEATS_CAT1 + 1) < (n + cnt)) {
				std::cout << "\nSorry there are only " << (MAX_SEATS_CAT1 - cnt) << " tickets available in this cateogry today.\n";
				return FAIL;
			}
		}
		if (category == 2) {
			if ((MAX_SEATS_CAT2 + 1) < (n + cnt)) {
				std::cout << "\nSorry there are only " << (MAX_SEATS_CAT2 - cnt) << " tickets available in this cateogry today.\n";
				return FAIL;
			}
		}
		if (category == 3) {
			if ((MAX_SEATS_CAT3 + 1) < (n + cnt)) {
				std::cout << "\nSorry there are only " << (MAX_SEATS_CAT3 - cnt) << " tickets available in this category today.\n";
				return FAIL;
			}
		}
		if (category == 4) {
			if ((MAX_SEATS_CAT4 + 1) < (n + cnt)) {
				std::cout << "\nSorry there are only " << cnt << " tickets available in this category today.\n";
				return FAIL;
			}
		}
	}
	else {
		showErrAndCloseConn();
		return FAIL;
	}
	sqlite3_finalize(stmt);
	const char * getPriceSQL = "SELECT name, price from Category WHERE id = ?";
	res = sqlite3_prepare_v2(conn, getPriceSQL, -1, &stmt, NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	res = sqlite3_bind_int(stmt, 1, category);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	if (SQLITE_ROW == sqlite3_step(stmt)) {
		catName = string(
			reinterpret_cast<const char*> (sqlite3_column_text(stmt, 0))
			);
		price = sqlite3_column_double(stmt, 1);
	}
	else {
		showErrAndCloseConn();
		return FAIL;
	}
	sqlite3_finalize(stmt);

	// Book the ticket
	char * SQLInsert = "INSERT INTO Ticket_Master (movie_id, category_id, screen_id, show_num, seat_num, total, date) VALUES (? , ? , ? , ? ,? , ? , DATE('now'));";
	res = sqlite3_prepare_v2(conn, SQLInsert, -1, &stmt, NULL);
	if (SQLITE_OK != res) {
		showErrAndCloseConn();
		return FAIL;
	}
	total = 0;
	for (int i = 1; i <= n; i++) {
		seat_no = ++cnt;
		res = sqlite3_bind_int(stmt, 1, m.getId());
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_bind_int(stmt, 2, category);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_bind_int(stmt, 3, screen);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_bind_int(stmt, 4, show_num);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}
		res = sqlite3_bind_int(stmt, 5, seat_no);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}

		res = sqlite3_bind_double(stmt, 6, price);
		if (SQLITE_OK != res) {
			showErrAndCloseConn();
			return FAIL;
		}

		res = sqlite3_step(stmt);
		if (SQLITE_DONE != res) {
			cout << "\nError Occurred, Could not book a ticket. Please try again\n";
			showErrAndCloseConn();
			return FAIL;
		}
		total += price;
		id = (int)sqlite3_last_insert_rowid(conn);

		sqlite3_reset(stmt);
		print_ticket();
		cout << endl << endl;
	}
	cout << endl;
	printline(45);
	cout << endl;
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << setfill(' ') << setw(1) << left << "|" << setw(7) << " " << setw(15) << "Total Amount Payable: " << total << setw(1) << right << " " << setw(13) << "|" << endl;
	cout << setfill(' ') << setw(1) << left << "|" << setw(5) << " " << setw(15) << "Have a great show, Thank you!" << setw(1) << right << " " << setw(11) << "|" << endl;

	SetConsoleTextAttribute(h, EVENT_SYSTEM_FOREGROUND | FOREGROUND_INTENSITY);
	printline(45);
	cout << endl << endl;
	return SUCCESS;
}
void Ticket::print_ticket() {
	int length = strlen(m.getMovieTitle());
	
	cout << setfill('-') << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(length + 5) << "-" << setw(1) << "+" << setw(12) << "-" << setw(1) << "+" << setw(12) << "-" << setw(12) << setw(1) << "+" << setw(12) << "-" << setw(1) << "+" << setw(11) << "-" << setw(1) << "+" << setw(8) << "-" << setw(1) << "+" << std::endl;
	cout << setfill(' ') << setw(1) << "| " << setw(5) << left << "Seat No." << setw(1) << " | " << setw(length + 3) << left << "Movie Title" << setw(1) << " | " << setw(10) << left << "Date" << setw(1) << " | " << setw(10) << left << "Category" << setw(1) << " | " << setw(10) << left << "Screen" << setw(1) << " | " << setw(9) << left << "Show No." << setw(1) << " | " << setw(6) << left << "Price" << setw(1) << " | " << endl;
	cout << setfill('-') << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(length + 5) << "-" << setw(1) << "+" << setw(12) << "-" << setw(1) << "+" << setw(12) << "-" << setw(12) << setw(1) << "+" << setw(12) << "-" << setw(1) << "+" << setw(11) << "-" << setw(1) << "+" << setw(8) << "-" << setw(1) << "+" << std::endl;
	// ticket info
	cout << setfill(' ') << setw(1) << "| " << setw(8) << left << seat_no << setw(1) << " | " << setw(length + 3) << left << m.getMovieTitle() << setw(1) << " | " << setw(7) << left << date << setw(1) << " | " << setw(10) << left << catName << setw(1) << " | " << setw(10) << left << screen << setw(1) << " | " << setw(9) << left << show_num << setw(1) << " | " << setw(6) << price << setw(1) << " | " << endl;

	// border
	cout << setfill('-') << setw(1) << "+" << setw(10) << "-" << setw(1) << "+" << setw(length + 5) << "-" << setw(1) << "+" << setw(12) << "-" << setw(1) << "+" << setw(12) << "-" << setw(12) << setw(1) << "+" << setw(12) << "-" << setw(1) << "+" << setw(11) << "-" << setw(1) << "+" << setw(8) << "-" << setw(1) << "+" << std::endl;

}

/*******************  Ticket Module Over ************************/

/************************
* Main App
************************/
Admin admin;
Movie movie;

void MovieMenu() {
	system("cls");
	int ch;
	fflush(stdin);
	printHeader();
	cout << endl << "1. Add New Movie";
	cout << endl << "2. Display Current Movie";
	cout << endl << "3. Update Current Movie Details";
	cout << endl << "4. Change Movie";
	cout << endl << "5. Main Menu";
	cout << endl << "Enter your choice : ";
	cin >> ch;

	switch (ch) {
	case 1: 
		if (SUCCESS != movie.addNewMovie()) {
			cout << "There was problem in adding new movie";
		}
		break;
	case 2:
		cout << movie; break;
	case 3: 
		if (SUCCESS != movie.updateMovie()) {
			cout << "\nThere was some problem, could not update the movie details.\n";
			sqlite3_close(conn);
		} 
		break;
	case 4:
		if (SUCCESS != movie.changeMovie()) {
			cout << "\nCould not change the movie.\n";
			sqlite3_close(conn);
		}
		break;
	case 5: MainMenu(); break;
	default:
		cout << "\nInvalid Choice...\n";
	}
}
void adminMenu() {
	int ch;
	system("cls");
	printHeader();
	cout << endl;
	cout << setw(15) << left << "1. Change Price for category" << endl;
	cout << setw(15) << left << "2. Logout" << endl;
	cout << setw(15) << left << "3. Main Menu" << endl;
	cout << setw(15) << left << "Enter choice : ";
	cin >> ch;
	switch (ch) {
	case 1: 
		if (SUCCESS != admin.changeCategoryPrices()) {
			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << setw(5) << "\n Could not update the price.\n";
		}
		break;
	case 2: admin.logout(); break;
	case 3: MainMenu(); break;
	default:

		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << setw(15) << "Invalid Choice..." << endl;
		SetConsoleTextAttribute(h, EVENT_SYSTEM_FOREGROUND | FOREGROUND_INTENSITY);
		break;
	}
}

void MainMenu() {
	int ch;
	system("cls");
	printHeader();

	movie.init();
	Ticket t(movie);
	printline();
	cout << endl;
	cout << setfill(' ') << setw(1) << "| " << setw(5) << "1. Admin Menu" << setw(15) << " " << setw(1) << left << " | "<< endl;
	cout << setfill(' ') << setw(1) << "| " << setw(5) << "2. Movie Menu" << setw(15) << " " << setw(1) << left << " | " << endl;
	cout << setfill(' ') << setw(1) << "| " << setw(5) << "3. Book Ticket"<< setw(14) << " " << setw(1) << left << " | " << endl;
	cout << setfill(' ') << setw(1) << "| " << setw(5) << "0. Exit" << setw(21) << " " << setw(1) << left << " | " << endl;
	printline();
	cout << endl << endl;
	cout << setw(15) << "Enter Your Choice: ";
	cin >> ch;
	fflush(stdin);
	switch (ch)	{
	case 0: exit(0); break;
	case 1: adminMenu(); break;
	case 2: MovieMenu(); break;
	case 3: 
		if (1 != (cin >> t)) break;
		if (SUCCESS != t.book_ticket()) {

			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "\nCould not book ticket(s)\n";
			SetConsoleTextAttribute(h, EVENT_SYSTEM_FOREGROUND | FOREGROUND_INTENSITY);
			sqlite3_close(conn);
		}
		break;
	default:
		SetConsoleTextAttribute(h,FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\nInvalid Choice....\n";
		break;
	}
}


void splash_screen() {

	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << endl << endl<< endl;
	cout << "\t" << ";;;       ;;;  ;;;;;;;  ;;       ;;  ;;;;;;;;;  ;;;;;;;;"<<endl;
	cout << "\t" << ";;;;;   ;;;;;  ;;   ;;  ;;       ;;     ;;       ;;"<< endl;
	cout << "\t" << ";;;  ;;   ;;;  ;;   ;;   ;;     ;;      ;;       ;;;;;;"<<endl;
	cout << "\t" << ";;;       ;;;  ;;   ;;    ;;   ;;       ;;       ;;" << endl;
	cout << "\t" << ";;;       ;;;  ;;;;;;;     ;;;;;     ;;;;;;;;;  ;;;;;;;;" << endl ;
	cout << endl << endl;
	cout << ";;;;;;;;;;;  ;;;;;;;;;  ;;;;;;;;;;  ;;    ;;   ;;;;;;;;  ;;;;;;;;;;" << endl;
	cout << "    ;;          ;;      ;;          ;;   ;;     ;;           ;;" << endl;
	cout << "    ;;          ;;      ;;          ;; ;;       ;;;;;;       ;;" << endl;
	cout << "    ;;          ;;      ;;          ;;    ;;    ;;           ;;" << endl;
	cout << "    ;;       ;;;;;;;;   ;;;;;;;;;   ;;    ;;   ;;;;;;;;      ;;" << endl;
	cout << endl << endl;
	cout << ";;;;;;;;    ;;;;;;;   ;;;;;;;   ;;   ;;   ;;;;;;;;  ;;     ;;  ;;;;;;;;" << endl;
	cout << ";;     ))   ;;   ;;   ;;   ;;   ;;  ;;       ;;     ;;;;   ;;  ;;" << endl;
	cout << ";;;;;;;     ;;   ;;   ;;   ;;   ;;;;         ;;     ;;  ;  ;;  ;;  ;;;;" << endl;
	cout << ";;     ))   ;;   ;;   ;;   ;;   ;;  ;;       ;;     ;;   ; ;;  ;;    ;;" << endl;
	cout << ";;;;;;;;    ;;;;;;;   ;;;;;;;   ;;   ;;   ;;;;;;;;  ;;    ;;;  ;;;;;;;;" << endl;

	cout << endl<< endl;
	
	cout << " ;;;;;;;;   ;;      ;;   ;;;;;;;  ;;;;;;;;;;  ;;;;;;;;;;  ;;;       ;;;" << endl;
	cout << ";;           ;;    ;;   ;;            ;;        ;;        ;;;;;   ;;;;;" << endl;
	cout << " ;;;;;;;;	;;       ;;;;;;;      ;;        ;;;;;     ;;;   ;;  ;;;" << endl;
	cout << "        ;;      ;;             ;;     ;;        ;;        ;;;       ;;;" << endl;
	cout << " ;;;;;;;;       ;;      ;;;;;;;;      ;;       ;;;;;;;;;  ;;;       ;;;" << endl;
	

	Sleep(1500);

	SetConsoleTextAttribute(h, EVENT_SYSTEM_FOREGROUND | FOREGROUND_INTENSITY);
	cout << endl<< endl << "Press Any Key To Enter Into The System";
	_getch();
	system("cls");

}

int main() {
	system("cls");
	SetConsoleTitle(L"Movie Ticket Booking System");
	splash_screen();
	
	if(SUCCESS != admin.login()){
		Sleep(3000);
		sqlite3_close(conn);
		return 1;
	}
	char c;
	do {
		MainMenu();
		cout << "DO YOU WANT TO CONTINUE? (y/n) : ";
		cin >> c;
	} while (c == 'y' || c == 'Y');
	cout << endl<<  setw(15) <<"Press Any Key to Exit.";
	sqlite3_close(conn);
	_getch();
    return 0;
}