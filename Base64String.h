#pragma once
#ifndef _BASE64_STRING_H_
#define _BASE64_STRING_H_

#ifndef _BUILD_DLL
#define DLL_API __declspec(dllimport)
#else
#define DLL_API __declspec(dllexport)
#endif // !_BUILD_DLL

#include <string>
#include <stdexcept>

typedef unsigned char byte;

class DLL_API Base64String
{
public:
	static std::string encode(const std::string& _data);
	static std::string decode(const std::string& _data);

private:
	static byte check(const byte _value);

	static void encodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret);
	static void decodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret);
	static void encodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret, const size_t _remainingBytes);
	static void decodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret, const size_t _remainingBytes);

private:
	enum { TwoCharactersRemaining = 2, ThreeCharactersRemaining };

	static const byte base64EncodeMap[];
	static const byte base64DecodeMap[];

	static constexpr int INVALID_VALUE = 255;
	static constexpr char COMPLEMENT_CHARACTER = '=';
};

#endif // !_BASE64_STRING_H_