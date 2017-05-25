#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <map>
#include <deque>

class Database;

struct Info
{
	Info();

	std::string name;
	std::string phone1;
	std::string phone2;
	std::string petName;
	std::string remark;
};

struct Coupons
{
	Coupons();

	std::string type;
	long quantity;
};

struct Transactions
{
	Transactions();

	std::string category;
	std::string type;
	long quantity;
	double price;
	std::string remark;
	std::string date;
};

class Customer
{
public :
	Customer();
	virtual ~Customer();

	void init(const std::string &id, struct Info &info);

	int read(const Database &db, std::string &id);	// from db, write mem

	int writeInfo(const Database &db);
	int writePoints(const Database &db);
	int writeCoupons(const Database &db);
	int writeSales(const Database &db);

	std::string id;

	Info info;
	long point;
	std::map<std::string, long> coupons;
	std::deque<struct Transactions> transactions;
private :

};

#endif
