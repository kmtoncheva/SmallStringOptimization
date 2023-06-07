#pragma once
#include <iostream>

const size_t LENGTH = 15;
const size_t MASK = (1 << 7);
const size_t MASK2 = (1 << 31);
const size_t LENGTH2 = 1024;

class StringSSO {
	union {
		struct {
			char* bigStr;
			size_t bigStrLen;
		};
		char smallStr[sizeof(bigStr) + sizeof(bigStrLen)]{'\0'};
	};

	void copyFrom(const StringSSO&);
	void free();
	bool isSmall() const;

public:
	StringSSO();
	StringSSO(const char*);
	StringSSO(const StringSSO&);
	StringSSO& operator=(const StringSSO&);
	~StringSSO();
	
	const char* c_str() const;
	size_t getLength() const;
	char& operator[](size_t index);
	char operator[](size_t index) const;

	StringSSO& operator+=(const StringSSO&);

	friend std::ostream& operator<<(std::ostream&, const StringSSO&);
	friend std::istream& operator>>(std::istream&, StringSSO&);
	friend StringSSO operator+(const StringSSO&, const StringSSO&);
};

std::ostream& operator<<(std::ostream& os, const StringSSO& str);
std::istream& operator>>(std::istream&, StringSSO&);
StringSSO operator+(const StringSSO&, const StringSSO&);


bool operator<(const StringSSO& lhs, const StringSSO& rhs);
bool operator<=(const StringSSO& lhs, const StringSSO& rhs);
bool operator>=(const StringSSO& lhs, const StringSSO& rhs);
bool operator>(const StringSSO& lhs, const StringSSO& rhs);
bool operator==(const StringSSO& lhs, const StringSSO& rhs);
bool operator!=(const StringSSO& lhs, const StringSSO& rhs);