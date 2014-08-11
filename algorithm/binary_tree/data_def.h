#ifndef DATA_DEF_HEADER
#define DATA_DEF_HEADER

#include <iostream>
#include <string>

class DataStruct1
{
public:
	typedef int key_type;
	typedef std::string data_type;

	DataStruct1() : _key(0), _data(std::string()) {};
	DataStruct1(const key_type& k, const data_type& d) : _key(k), _data(d) {};

	const key_type get_key(void) const { return _key; };
	const data_type get_data(void) const { return _data; };
	void set_data(const data_type& d) { _data = d; };

	bool operator < (const DataStruct1& a) const { return _key < a._key; };
	bool operator == (const DataStruct1& a) const { return _key == a._key; };

	friend std::ostream& operator << (std::ostream&  os, const DataStruct1& d);

private:
	key_type _key;
	data_type _data;
};

std::ostream& operator << (std::ostream& os, const DataStruct1& d)
{
	os << "[" << d.get_key() << ", " << d.get_data() << "]";
	return os;
}

class DataStruct2
{
public:
	typedef double key_type;
	typedef double data_type;

	DataStruct2() : dbl(0.0) {};
	DataStruct2(double d) : dbl(d) {};

	const key_type get_key(void) const { return dbl; };
	const data_type get_data(void) const { return dbl; };
	void set_data(double d) { dbl = d; };

	//bool operator < (const DataStruct2& a) const { return dbl<a.dbl; };
	bool operator == (const DataStruct2& a) const { return dbl == a.dbl; };

	friend std::ostream& operator << (std::ostream&  os, const DataStruct2& d);

private:
	double dbl;
};

std::ostream& operator << (std::ostream& os, const DataStruct2& d)
{
	os << "[" << d.get_data() << "]";
	return os;
}

#endif
