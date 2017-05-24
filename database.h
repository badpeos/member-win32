#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <deque>

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
	int updateCoupons(const std::string &id, const struct Coupons &coupons);
	int newTransaction(const std::string &id, const struct Sales &sales);

	int read (std::deque<Customer> customers);
	int read (const std::string &id, Customer &customer);
	int readPoint(const std::string &id, long &point);
	int search (const std::string &keyword, std::deque<Customer> customers);

private :
	sqlite3 *_db;
};

#endif
