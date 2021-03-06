#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <deque>
#include <map>

#include "customer.h"
#include "sqlite3.h"

struct Point
{
	std::string name;
	int point;
};

class Database
{
public :
	Database();
	virtual ~Database();

	int init (const std::string &filename);

	int createCustomer(const Customer &customer, const std::string &date);

	int updateInfo(const std::string &id, const struct Info &info, const std::string &date);
	int updatePoints(const std::string &id, const long point, const std::string &date);
	int updateCoupons(const std::string &id, const std::string &type, const long quantity, const double total, const std::string &remark, const std::string &date);
	int updateBath(const std::string &id, const std::string &type, const long quantity, const double total, const std::string &remark, const std::string &date);
	int updateShopping(const std::string &id, const double price, const std::string &remark, const std::string &date);
	int updateDaycare(const std::string &id, const std::string &type, const long quantity, const double total, const std::string &remark, const std::string &date);
	//int newTransaction(const std::string &id, const struct Sales &sales);

	int read (std::deque<Customer> &customers);
	int read (const std::string &id, Customer &customer);
	int readPoint(const std::string &id, long &point);
	int readCoupons(const std::string &id, const std::string &type, long &oldQty);
	int readCoupons(const std::string &id, std::deque<struct Coupons> &coupons);
	int readTransactions(const std::string &id, std::deque<struct Transactions> &transactions);
	int search (const std::string &keyword, std::deque<Customer> &customers);

	int readTransactionAll(const std::string &id, std::deque<struct Transactions> &transactions);
	int readTransactionCoupons(const std::string &id, std::deque<struct Transactions> &transactions);
	int readTransactionSingle(const std::string &id, std::deque<struct Transactions> &transactions);
	int readTransactionSales(const std::string &id, std::deque<struct Transactions> &transactions);
	int readTransactionDay(const std::string &id, std::deque<struct Transactions> &transactions);
	int readTransactionPoints(const std::string &id, std::deque<struct Transactions> &transactions);

	bool resetPoint();
	bool readPoint(std::map<std::string, struct Point> &memberPoint);

private :
	sqlite3 *_db;
};

#endif
