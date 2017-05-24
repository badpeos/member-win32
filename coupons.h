#ifndef COUPONS_H
#define COUPONS_H

class Database;

class Coupons
{
public :
	Coupons (const Database& database);
	virtual ~Coupons();

private :
	Coupons ();

	long id;

};

#endif
