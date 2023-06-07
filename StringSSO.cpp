#include <iostream>
#include "StringSSO.h"
#pragma warning (disable : 4996)

bool StringSSO::isSmall() const{
	return((smallStr[sizeof(bigStr) + sizeof(bigStrLen) - 1] & MASK) == 0);
}

StringSSO::StringSSO() {
	smallStr[0] = '\0';
}

StringSSO::StringSSO(const char* str) {
	size_t currSize = strlen(str);
	if (currSize <= LENGTH) {
		smallStr[LENGTH] = LENGTH - currSize;
		strcpy(smallStr, str);
	}
	else {
		bigStrLen = strlen(str);
		bigStr = new char[bigStrLen + 1];
		strcpy(bigStr, str);
		smallStr[LENGTH] |= MASK;
	}
}

void StringSSO::free() {
	if (!isSmall()) {
		delete[] bigStr;
		bigStr = nullptr;
		bigStrLen = 0;
	}
}

StringSSO::~StringSSO() {
	free();
}

void StringSSO::copyFrom(const StringSSO& other) {

	if (other.isSmall()) {
		size_t len = strlen(other.smallStr);
		smallStr[LENGTH] = LENGTH - len;
		strcpy(smallStr, other.smallStr);
	}
	else {
		//smallStr[0]= '\0' ;
		bigStrLen = strlen(other.bigStr);
		bigStr = new char[bigStrLen + 1];
		strcpy(bigStr, other.bigStr);
		smallStr[LENGTH] |= MASK;
	}
}

StringSSO::StringSSO(const StringSSO& other) {
	copyFrom(other);
}

StringSSO& StringSSO::operator=(const StringSSO& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}


const char* StringSSO::c_str() const {
	if (isSmall()) {
		return smallStr;
	}
	else {
		return bigStr;
	}
}

size_t StringSSO::getLength() const {
	if (isSmall())
	{
		return LENGTH - smallStr[LENGTH];
	}
	else {
		size_t currLen = bigStrLen;
		return (currLen &= (~MASK2));
	}
}

char& StringSSO::operator[](size_t index) {
	size_t len = getLength();
	if (index <= len) {
		if (isSmall())
			return smallStr[index - 1];
		return bigStr[index - 1];
	}
	throw std::invalid_argument("this character does not exist");
}
char StringSSO::operator[](size_t index) const {
	size_t len = getLength();
	if (index <= len) {
		if (isSmall())
			return smallStr[index - 1];
		return bigStr[index - 1];
	}
	throw std::invalid_argument("this character does not exist");
}


std::ostream& operator<<(std::ostream& os, const StringSSO& str) {
	return os << str.c_str();
}


std::istream& operator>>(std::istream& is, StringSSO& str) {
	char buffer[LENGTH2];
	is >> buffer;
	str.free();
	size_t currSize = strlen(buffer);
	if (currSize <= LENGTH) {
		str.smallStr[LENGTH] = LENGTH - currSize;
		strcpy(str.smallStr, buffer);
		return is;
	}
	str.bigStrLen = strlen(buffer);
	str.bigStr = new char[str.bigStrLen + 1];
	strcpy(str.bigStr, buffer);
	str.smallStr[LENGTH] |= MASK;
}

StringSSO& StringSSO::operator+=(const StringSSO& other) {
	size_t currSize = getLength() + other.getLength();
	if (isSmall()) {
		if (currSize <= LENGTH) {
			smallStr[LENGTH] = LENGTH - currSize;
			strcat(smallStr, other.smallStr);
		}
		else {
			char temp[LENGTH + 1];
			strcpy(temp, smallStr);
			bigStrLen = currSize;
			bigStr = new char[currSize + 1];
			bigStr[0] = '\0';
			strcat(bigStr, temp);
			strcat(bigStr, other.c_str());
		}
		return *this;
	}
	char* result = new char[currSize + 1];
	result[0] = '\0';
	strcat(result, bigStr);
	strcat(result, other.c_str());
	delete bigStr;
	bigStr = result;
	return *this;
}

StringSSO operator+(const StringSSO& lhs, const  StringSSO& rhs) {
	size_t len = lhs.getLength() + rhs.getLength();
	if (len <= LENGTH) {
		char temp1[LENGTH + 1]{ '\0' };
		strcpy(temp1, lhs.c_str());
		strcat(temp1, rhs.c_str());
		StringSSO res1;
		res1.smallStr[LENGTH] = LENGTH - len;
		strcpy(res1.smallStr, temp1);
		return res1;
	}
	char* temp2 = new char[len + 1];
	strcpy(temp2, lhs.c_str());
	strcat(temp2, rhs.c_str());
	StringSSO res2(temp2);
	return res2;
}

/*
* 
StringSSO operator+(const StringSSO& lhs, const  StringSSO& rhs) {
	StringSSO result;
	size_t currSize = lhs.getLength() + rhs.getLength();
	if (currSize <= LENGTH) {
		result.smallStr[LENGTH] = LENGTH - currSize;
		strcpy(result.smallStr, lhs.c_str());
		strcat(result.smallStr, rhs.c_str());
	}
	else {

		result.bigStrLen = currSize;
		result.bigStr = new char[currSize + 1];
		//result.smallStr[0] = '\0';
		result.bigStr[0] = '\0';
		strcat(result.bigStr, lhs.c_str());
		strcat(result.bigStr, rhs.c_str());
		result.smallStr[LENGTH] |= MASK;
	}
	return result;
	}
*/


bool operator<(const StringSSO& lhs, const StringSSO& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}
bool operator<=(const StringSSO& lhs, const StringSSO& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) <= 0;
}
bool operator>=(const StringSSO& lhs, const StringSSO& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) <= 0;
}
bool operator>(const StringSSO& lhs, const StringSSO& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}
bool operator==(const StringSSO& lhs, const StringSSO& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}
bool operator!=(const StringSSO& lhs, const StringSSO& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) != 0;
}