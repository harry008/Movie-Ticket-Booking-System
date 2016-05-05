#include "inc\sqlite3.h"
#include <string.h>
#pragma once
class SQLiteConnection {
	sqlite3 * conn;
public:
	SQLiteConnection() {
		conn = NULL;
	}
	~SQLiteConnection() {
		sqlite3_close(conn);
	}	
	int connect(char const * dbName) {
		
		int res = sqlite3_open(dbName, &conn);

		if (SQLITE_OK != res) {
			printf("%s\n", sqlite3_errmsg(conn));
		    return res;
		}
		return res;
	}
	sqlite3 * getConn() {
		return conn;
	}
};

class Statement {
	sqlite3_stmt * stmt;
public:
	Statement() {
		stmt = NULL;
	}
	int prepare(sqlite3 *,char *);
	int bind_param_int(sqlite3 *, int, int);
	int bind_param_text(sqlite3 *, int, char const *);
	int bind_param_double(sqlite3 *, int, double);
	bool step();
	int reset();
	unsigned char const * getColText(int idx);
	void finalize() {
		sqlite3_finalize(stmt);
	}
};
int Statement::prepare(sqlite3 * conn, char *sql) {
	int result;
	result = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
	
	if (SQLITE_OK != result) {
		sqlite3_errmsg(conn);
		return 0;			
	}
	return SQLITE_OK;
}
int Statement::bind_param_int(sqlite3 * conn,int idx, int val) {
	int res;
	res = sqlite3_bind_int(stmt, idx, val);
	if (SQLITE_OK != res) {
		sqlite3_errmsg(conn);
		return 0;
	}
	return SQLITE_OK;
	
}

int Statement::bind_param_text(sqlite3 * conn, int idx, char const * val) {
	int res;
	res = sqlite3_bind_text(stmt, idx, val, strlen(val)+1, SQLITE_STATIC);
	if (SQLITE_OK != res) {
		sqlite3_errmsg(conn);
		return 0;
	}
	return SQLITE_OK;
}
int Statement::bind_param_double(sqlite3 * conn , int idx, double val){
	int res;
	res = sqlite3_bind_double(stmt, idx, val);
	if (SQLITE_OK != res) {
		sqlite3_errmsg(conn);
		return 0;
	}
	return SQLITE_OK;
}
bool Statement::step() {
	int res = sqlite3_step(stmt);
	if (SQLITE_DONE == res) return true;
	if (SQLITE_ROW == res) return true;
	return false;
}
int Statement::reset() {
	int res = sqlite3_reset(stmt);
	if (SQLITE_OK == res) return res;
	return 0;
}
unsigned char const * Statement::getColText(int idx) {
	return sqlite3_column_text(stmt, idx);
}