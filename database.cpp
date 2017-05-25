#pragma warning (disable : 4786)

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <algorithm>

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
			std::string("where id = '") + id + std::string("';");
		sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
		if (msg)
			sqlite3_free(msg);

		// update transaction
		sprintf(buffer, "%ld", point);
		sql="insert into transactions (id, category, type, quantity, price, remark) values (" +
			std::string("'") + id + std::string("', ") +
			std::string("'") + std::string("point change") + std::string("', ") +
			std::string("'") + std::string("") + std::string("', ") +
			std::string("'") + std::string(buffer) + std::string("', ") +
			std::string("'") + std::string("") + std::string("', ") +
			std::string("'") + std::string("") + std::string("');");
		sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
		if (msg)
			sqlite3_free(msg);
	}

	return 0;
}

int Database::updateCoupons(const std::string &id, const std::string &type, const long quantity, const double total)
{
	//sql="create table coupons (id text, type text, quantity integer);";
	std::string sql;
	char *msg;
	char buffer[512]={0};

	Customer customer;
	long oldQty = 0;
	int count = readCoupons(id, type, oldQty);
	long newQty = oldQty+quantity;
	sprintf (buffer, "%ld", newQty);
	if (count == 1)
	{
		
		sql="update coupons set " +
			std::string("quantity = ") + std::string(buffer) + 
			std::string("where id = '") + id + std::string("';");
		sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
		if (msg)
			sqlite3_free(msg);
	}
	else
	{
		sql="insert into coupons (id, type, quantity) values (" +
			std::string("'") + id + std::string("', ") +
			std::string("'") + type + std::string("', ") + 
			std::string(buffer) +
			std::string(");");
		sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
		if (msg)
			sqlite3_free(msg);
	}

	// update transaction
	sprintf(buffer, "%ld", quantity);
	sql="insert into transactions (id, category, type, quantity, price, remark) values (" +
		std::string("'") + id + std::string("', ") +
		std::string("'") + std::string("coupons change") + std::string("', ") +
		std::string("'") + type + std::string("', ") +
		std::string("'") + std::string(buffer) + std::string("', ") +
		std::string("'") + std::string("") + std::string("', ") +
		std::string("'") + std::string("") + std::string("');");
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	return 0;
}

int Database::updateBath(const std::string &id, const std::string &type, const long quantity, const double total)
{
	std::string sql;
	char qtyBuf[512]={0};
	char totBuf[512]={0};
	char *msg;

	sprintf(qtyBuf, "%ld", quantity);
	sprintf(totBuf, "%.1lf", total);

	sql="insert into transactions (id, category, type, quantity, price, remark) values (" +
		std::string("'") + id + std::string("', ") + 
		std::string("'") + std::string("single bath") + std::string("', ") +
		std::string("'") + type + std::string("', ") +
		std::string("'") + std::string(qtyBuf) + std::string("', ") +
		std::string("'") + std::string(totBuf) + std::string("', ") + 
		std::string("'") + std::string("") + std::string("'") +
		std::string(");");
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	updatePoints(id, total);

	return 0;
}

int Database::updateShopping(const std::string &id, const double price)
{
	std::string sql;
	char totBuf[512]={0};
	char *msg;

	sprintf(totBuf, "%.1lf", price);

	sql="insert into transactions (id, category, type, quantity, price, remark) values (" +
		std::string("'") + id + std::string("', ") + 
		std::string("'") + std::string("sales") + std::string("', ") +
		std::string("'") + std::string("") + std::string("', ") +
		std::string("'") + std::string("") + std::string("', ") +
		std::string("'") + std::string(totBuf) + std::string("', ") + 
		std::string("'") + std::string("") + std::string("'") +
		std::string(");");
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	updatePoints(id, (long)price);

	return 0;
}

int Database::updateDaycare(const std::string &id, const std::string &type, const long quantity, const double total)
{
	std::string sql;
	char totBuf[512]={0};
	char qtyBuf[512]={0};
	char *msg;

	sprintf(qtyBuf, "%ld", quantity);
	sprintf(totBuf, "%.1lf", total);

	sql="insert into transactions (id, category, type, quantity, price, remark) values (" +
		std::string("'") + id + std::string("', ") + 
		std::string("'") + std::string("day care") + std::string("', ") +
		std::string("'") + type + std::string("', ") +
		std::string("'") + std::string(qtyBuf) + std::string("', ") +
		std::string("'") + std::string(totBuf) + std::string("', ") + 
		std::string("'") + std::string("") + std::string("'") +
		std::string(");");
	sqlite3_exec(_db, sql.c_str(), NULL, NULL, &msg);
	if (msg)
		sqlite3_free(msg);

	updatePoints(id, (long)total);
	return 0;
}

static int cbInfo (void *pt, int count, char **data, char **column)
{
	std::string id;
	struct Info info;
	int i;

	for (i = 0; i<count; i++)
	{
		std::string colName = column[i];

		if (colName == "id")
		{
			id=data[i];
		}
		else if (colName == "name")
		{
			info.name = data[i];
		}
		else if (colName == "phone1")
		{
			info.phone1 = data[i];
		}
		else if (colName == "phone2")
		{
			info.phone2 = data[i];
		}
		else if (colName == "petname")
		{
			info.petName = data[i];
		}
		else if (colName == "remark")
		{
			info.remark = data[i];
		}
	}

	std::map<std::string, struct Info> *result;
	result = (std::map<std::string, struct Info> *) pt;
	(*result)[id] = info;
	return 0;
}

static int cbCoupons(void *pt, int count, char **data, char **column)
{
	//sql="create table coupons (id text, type text, quantity integer);";
	int i;

	std::string id;
	std::string type;
	long qty;
	for (i = 0; i<count; i++)
	{
		std::string colName = column[i];

		if (colName == "id")
		{
			id=data[i];
		}
		else if (colName == "type")
		{
			type = data[i];
		}
		else if (colName == "quantity")
		{
			qty = atol(data[i]);
		}
	}

	std::map< std::string, std::map<std::string, long> > *result;
	result = (std::map< std::string, std::map<std::string, long> > *) pt;
	(*result)[id][type] = qty;
	return 0;
}

static int cbPoints(void *pt, int count, char **data, char **column)
{
	//sql="create table points (id text, point integer);";
	int i;
	std::string id;
	long point;

	for (i = 0; i<count; i++)
	{
		std::string colName = column[i];

		if (colName == "id")
		{
			id = data[i];
		}
		else if (colName == "point")
		{
			point = atol(data[i]);
		}
	}

	long *pnt = (long *) pt;
	*pnt = point;

	return 0;
}

static int cbTransactions(void *pt, int count, char **data, char **column)
{
	//sql="create table transactions (id text, category text, type text, quantity integer, price real, remark text);";
	std::string id;
	struct Transactions transaction;
	int i;

	for (i = 0; i<count; i++)
	{
		std::string colName = column[i];

		if (colName == "id")
		{
			id = data[i];
		}
		else if (colName == "category")
		{
			transaction.category = data[i];
		}
		else if (colName == "type")
		{
			transaction.type = data[i];
		}
		else if (colName == "quantity")
		{
			transaction.quantity = atol(data[i]);
		}
		else if (colName == "price")
		{
			transaction.price = atof(data[i]);
		}
		else if (colName == "remark")
		{
			transaction.remark = data[i];
		}
		else if (colName == "date")
		{
			transaction.date = data[i];
		}
	}

	std::map<std::string, std::deque<struct Transactions> > *result;
	(*result)[id].push_back(transaction);

	return 0;
}

int Database::read (std::deque<Customer> &customers)
{
	char *msg;

	std::map<std::string, struct Info> infos;

	// select all from info
	std::string sql;
	sql="select id from info;";
	sqlite3_exec(_db, sql.c_str(), cbInfo, &infos, &msg);
	if (msg)
		sqlite3_free(msg);

	std::map<std::string, struct Info>::iterator iter = infos.begin();
	while (iter != infos.end())
	{
		std::string id=iter->first;
		Customer customer;
		read(id, customer);

		customers.push_back(customer);
		iter++;
	}

	return 0;
}

int Database::read (const std::string &id, Customer &customer)
{
	long point;
	std::deque<struct Coupons> coupons;
	std::deque<struct Transactions> transactions;
	std::map<std::string, struct Info> infos;
	std::string sql;
	char *msg;

	sql="select * from info where id = " + std::string("'") + id + std::string("';");
	sqlite3_exec(_db, sql.c_str(), cbInfo, &infos, &msg);
	if (msg)
		sqlite3_free(msg);

	// read tables except info
	readPoint(id, point);					// points
	readCoupons(id, coupons);				// coupons
	readTransactions(id, transactions);		// transactions

	std::map<std::string, struct Info>::iterator iter = infos.begin();
	customer.init(id, iter->second);
	customer.point = point;		

	std::deque<struct Coupons>::iterator couponIter;
	couponIter = coupons.begin();		
	while (couponIter != coupons.end())
	{
		customer.coupons[couponIter->type] = couponIter->quantity;
		couponIter++;
	}

	std::deque<struct Transactions>::iterator tranIter;
	tranIter = transactions.begin();
	while (tranIter != transactions.end())
	{
		customer.transactions.push_back(*tranIter);
		tranIter++;
	}

	return 0;
}

int Database::readPoint(const std::string &id, long &point)
{
	// select all from points where id = id

	point = 0;
	return 0;
}

int Database::readCoupons(const std::string &id, const std::string &type, long &oldQty)
{
	oldQty = 0;

	// select * from coupons where id = id

	return 0;
}

int Database::readCoupons(const std::string &id, std::deque<struct Coupons> &coupons)
{
	return 0;
}

int Database::readTransactions(const std::string &id, std::deque<struct Transactions> &transactions)
{
	return 0;
}

int Database::search (const std::string &keyword, std::deque<Customer> &customers)
{
	// select id from info where id = keyword or phone1 = keyword or phone2 = keyword
	// for each id in return set, do read(id, customer) and push back to deque
	return 0;
}
