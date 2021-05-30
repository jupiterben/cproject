
#pragma once

#include "../maxHeaders.h"

class FString
{
protected:
	std::string data;
public:
	typedef std::string value_type;
	FString() {}
	FString(const TSTR &s);
	FString(const TCHAR *s);
	FString(const char *s);
	FString(size_t len, char s) { this->resize(len, s); }
	size_t size() const { return data.size(); }
	void resize(size_t l, char ch);
	const char *c_str() const { return data.c_str(); }
	const bool operator < (const FString& other)const { return data < other.data; }
	const bool operator == (const FString& other)const { return data == other.data; }
	FString& append(const char* s, size_t len);
	FString& push_back(char s) { data.push_back(s); return *this; }
	char& operator[](size_t i) { return data[i]; }
	const char& operator[](size_t i)const { return data[i]; }
	inline const char& back()const { return data.back(); }
	size_t hash()const { return std::hash<std::string>()(data); }
};

namespace std {
	template <>
	struct hash<FString>
	{
		std::size_t operator()(const FString& k) const
		{
			return k.hash();
		}
	};

	// template <>
	// struct less<FString>
	// {
	// 	bool operator()(const FString& k1, const FString& k2) const
	// 	{
	// 		return k1 < k2;
	// 	}
	// };
}