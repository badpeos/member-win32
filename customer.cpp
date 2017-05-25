#pragma warning (disable : 4786)

#include <string>
#include <deque>

#include "customer.h"

Info::Info() :
	name(""),
	petName(""),
	phone1(""),
	phone2(""),
	remark("")
{
}

Coupons::Coupons() :
	type(""),
	quantity(0)
{
}

Transactions::Transactions() :
	category(""),
	type(""),
	quantity(0),
	price(0.0),
	remark(""),
	date("")
{
}

Customer::Customer() :
	id(""),
	point(0)
{
}

Customer::~Customer()
{
}

void Customer::init(const std::string &id, struct Info &info)
{
}

int Customer::read(const Database &db, std::string &id)
{
	return 0;
}

int Customer::writeInfo(const Database &db)
{
	return 0;
}

int Customer::writeCoupons(const Database &db)
{
	return 0;
}

int Customer::writePoints(const Database &db)
{
	return 0;
}

int Customer::writeSales(const Database &db)
{
	return 0;
}
