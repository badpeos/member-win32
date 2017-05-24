#ifndef CUSTOMER_H
#define CUSTOMER_H

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

struct Sales
{
	Sales();

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

	void init(struct Info &info);		// init memory & write db

	int read(const Database &db, std::string &id);	// from db, write mem

	int writeInfo(const Database &db);
	int writePoints(const Database &db);
	int writeCoupons(const Database &db);
	int writeSales(const Database &db);

	std::string id;
	Info info;
	long point;
	std::deque<struct Coupons> coupons;
	std::deque<struct Sales> sales;
private :

};

#endif
