#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <deque>
#include <map>

#include "customer.h"
#include "sqlite3.h"

class Database
{
public :
	Database();
	virtual ~Database();

	int init (const std::string &filename);

	int createCustomer(const Customer &customer);

	int updateInfo(const std::string &id, const struct Info &info);
	int updatePoints(const std::string &id, const long point);
	int updateCoupons(const std::string &id, const std::string &type, const long quantity, const double total);
	int updateBath(const std::string &id, const std::string &type, const long quantity, const double total);
	int updateShopping(const std::string &id, const double price);
	int updateDaycare(const std::string &id, const std::string &type, const long quantity, const double total);
	//int newTransaction(const std::string &id, const struct Sales &sales);

	int read (std::deque<Customer> &customers);
	int read (const std::string &id, Customer &customer);
	int readPoint(const std::string &id, long &point);
	int readCoupons(const std::string &id, const std::string &type, long &oldQty);
	int readCoupons(const std::string &id, std::deque<struct Coupons> &coupons);
	int readTransactions(const std::string &id, std::deque<struct Transactions> &transactions);
	int search (const std::string &keyword, std::deque<Customer> &customers);

private :
	sqlite3 *_db;
};

#endif
