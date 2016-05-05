#include <iostream>
#include <conio.h>
#include "inc\sqlite3.h"

class Admin {
	long int id;
	char * username;
public:
	int login();
	long int getId() {
		return id;
	}
	int logout();
};
int Admin::login() {
	char username[50], password[50];
	std::string pass;
	char c;
	std::cout << "Enter Username: ";
	std::cin >> username;
	std::cout << "Enter Password: ";
	c = _getch();
	while (c != 13)
	{
		pass.push_back(c);
		std::cout << "*";
		c = _getch();
	}

	sqlite3 * conn = NULL;

	int res = sqlite3_open("./mtbs.db", &conn);

	if (SQLITE_OK != res) {
		printf("%s\n", sqlite3_errmsg(conn));
		return res;
	}

	sqlite3_stmt * stmt = NULL;
	char * sql = "select id, username from Admin where username = ? and password = ?";
	res = sqlite3_prepare_v2(conn,
		sql,
		-1,
		&stmt,
		NULL);

	if (SQLITE_OK != res) {
		printf("\n%s\n", sqlite3_errmsg(conn));
		sqlite3_close(conn);
		return res;
	}

	res = sqlite3_bind_text(stmt, 1, username, strlen(username), SQLITE_STATIC);
	if (SQLITE_OK != res) {
		printf("\n%s\n", sqlite3_errmsg(conn));
		sqlite3_close(conn);
		return res;
	}

	strcpy_s(password, pass.c_str());

	res = sqlite3_bind_text(stmt, 2, password, strlen(password), NULL);
	if (SQLITE_OK != res) {
		printf("\n%s\n", sqlite3_errmsg(conn));
		sqlite3_close(conn);
		return res;
	}

	if (SQLITE_ROW == sqlite3_step(stmt)) {
		std::cout << std::endl << "Login Success\n";
		res = 1;
		id = sqlite3_column_int(stmt, 0);
		//std::cout<< std::endl << sqlite3_column_text(stmt, 0) << "\t" << sqlite3_column_text(stmt, 1) << std::endl;
	}
	else {
		std::cout << std::endl << "Wrong username or password.\nExiting now\n";
		res = 0;
	}
	sqlite3_finalize(stmt);
	sqlite3_close(conn);
	return res;
}
int Admin::logout() {
	return 1;
}
