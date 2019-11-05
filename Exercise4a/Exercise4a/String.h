#include <iostream>
#include <cstring>

class String
{
	friend std::ostream& operator << (std::ostream& os, const String& string);

public:
	String(const char* string = "");
	String(const String& s);
	~String();
	void list();
	String operator + (const String& string) const;
	String& operator++();
	String operator++(int);
	const String& operator = (const String& string);
	char& operator[](int i);
private:
	char* c_string;
};