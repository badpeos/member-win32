#define _CRT_SECURE_NO_WARNINGS

#include <string>

#include "sqlite3.h"
#include "database.h"

Database::Database() :
	_db(NULL)
{
}

Database::~Database()
{
	sqlite3_close(_db);
}

int Database::init (const std::string &filename)
{
	std::string sql;
	char *msg;

	sqlite3_open(filename.c_str(), &_db);

	sql="pragma synchronous = normal;";
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	sql="pragma temp_store = 2;";
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	sql="pragma journal_mode = memory;";
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	sql="create table info (id text, name text, phone1 text, phone2 text, petname text, remark text);";
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	sql="create table coupons (id text, type text, quantity integer);";
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	sql="create table points (id text, point integer);";
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	sql="create table transactions (id text, category text, type text, quantity integer, price real, remark text);";
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);
	
	return 0;
}

int Database::createCustomer(const Customer &customer)
{
	std::string sql;
	char *msg;
	Customer temp;

	// check id exist
	int count = read(customer.id, temp);
	if (count)
		return -1;

	// info
	sql="insert into info (id, name, phone1, phone2, petname, remark) values (" + 
		std::string("'") + customer.id + std::string("', ") +
		std::string("'") + customer.info.name + std::string("', ") +
		std::string("'") + customer.info.phone1 + std::string("', ") +
		std::string("'") + customer.info.phone2 + std::string("', ") +
		std::string("'") + customer.info.petName + std::string("', ") +
		std::string("'") + customer.info.remark + std::string("'") + 
		std::string(");");
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	// points
	sql="insert into points (id, point) values (" + 
		std::string("'") + customer.id + std::string("', ") + 
		std::string("0);");
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	return 0;
}

int Database::updateInfo(const std::string &id, const struct Info &info)
{
	std::string sql;
	char *msg;
	int rc;

	Customer temp;
	int count = read(id, temp);
	
	if (count == 0)
	{
		temp.info = info;
		temp.id = id;
		rc=createCustomer(temp);
	}
	else
	{
		sql = "update info set " + 
			std::string("name = '") + info.name + std::string("', ") +
			std::string("phone1 = '") + info.phone1 + std::string("', ") +
			std::string("phone2 = '") + info.phone2 + std::string("', ") +
			std::string("petname = '") + info.petName + std::string("', ") +
			std::string("remark = '") + info.remark + std::string("' ") + 
			std::string("where id = '") + std::string("'") + id + std::string("';");
		sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
		if (msg)
			sqlite3_free(msg);
		rc=0;
	}

	return rc;
}

int Database::updatePoints(const std::string &id, const long point)
{
	std::string sql;
	char *msg;
	char buffer[512]={0};

	long oldPoint;
	int count = readPoint(id, oldPoint);
	if (count == 1)
	{
		long newPoint = oldPoint + point;

		// update point
		sprintf (buffer, "%ld", newPoint);
		sql="update points set " +
			std::string("point = ") + std::string(buffer) + 
			std::string("where id = '") + std::string("'") + id + std::string("';");
		sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
		if (msg)
			sqlite3_free(msg);

		// update transaction
		sprintf(buffer, "%ld", point);
		sql="insert into transactions (id, category, type, quantity, price, remark) values (" +
			std::string("'") + id + std::string("', ") +
			std::string("'") + std::string("point change") + std::string("', ") +
			std::string("'") + std::string("") + std::string("', ") +
			std::string("'") + std::string("") + std::string("', ") +
			std::string("'") + std::string(buffer) + std::string("', ") +
			std::string("'") + std::string("") + std::string("');");
		sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
		if (msg)
			sqlite3_free(msg);
	}

	return 0;
}

int Database::updateCoupons(const std::string &id, const struct Coupons &coupons)
{
	//sql="create table coupons (id text, type text, quantity integer);";
	std::string sql;
	char *msg;
	char buffer[512]={0};

	long oldPoint;
	Customer customer;
	int count = read(id, customer);
	if (count == 1)
	{

	}

	return 0;
}

int Database::newTransaction(const std::string &id, const struct Sales &sales)
{
	return 0;
}

int Database::read (std::deque<Customer> customers)
{
	return 0;
}

int Database::read (const std::string &id, Customer &customer)
{
	return 0;
}

int Database::readPoint(const std::string &id, long &point)
{
	point = 0;
	return 0;
}

int Database::search (const std::string &keyword, std::deque<Customer> customers)
{
	return 0;
}
