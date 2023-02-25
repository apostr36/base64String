#include "base64String.h"

const byte Base64String::base64EncodeMap[] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

const byte Base64String::base64DecodeMap[] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,  255, 255, 255, 63,
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255, 255, 255, 255, 255,
	255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
	15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  255, 255, 255, 255, 255,
	255, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
	41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

std::string Base64String::encode(const std::string& _data)
{
	if (_data.empty())
	{
		throw std::invalid_argument("invalid argument");
	}

	std::string ret;
	auto remainingBytes = _data.size();
	auto dataIterator = _data.cbegin();
	for (; 3 <= remainingBytes;
		dataIterator += 3, remainingBytes -= 3)
	{
		encodeOperation(dataIterator, ret);
	}

	if (0 != remainingBytes)
	{
		encodeOperation(dataIterator, ret, remainingBytes);
	}

	return ret;
}

std::string Base64String::decode(const std::string& _data)
{
	if ((_data.empty()) || (0 != (_data.size() % 4)))
	{
		throw std::invalid_argument("invalid argument");
	}

	std::string ret;
	size_t remainingBytes = _data.size();
	if (COMPLEMENT_CHARACTER == _data.at(_data.size() - 2))
	{
		remainingBytes -= 2;
	}
	else if (COMPLEMENT_CHARACTER == _data.at(_data.size() - 1))
	{
		remainingBytes -= 1;
	}
	auto dataIterator = _data.cbegin();
	for (; 4 <= remainingBytes;
		dataIterator += 4, remainingBytes -= 4)
	{
		decodeOperation(dataIterator, ret);
	}

	if (0 != remainingBytes)
	{
		decodeOperation(dataIterator, ret, remainingBytes);
	}

	return ret;
}

byte Base64String::check(const byte _value)
{
	if (INVALID_VALUE == _value)
	{
		throw std::invalid_argument("invalid argument");
	}
	
	return _value;
}

void Base64String::encodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret)
{
	_ret.push_back(base64EncodeMap[(static_cast<byte>(*(_startPoint)) >> 2)]);
	_ret.push_back(base64EncodeMap[(((static_cast<byte>(*(_startPoint)) << 4) & 0x30) |
		(static_cast<byte>(*(_startPoint + 1)) >> 4))]);
	_ret.push_back(base64EncodeMap[(((static_cast<byte>(*(_startPoint + 1)) & 0xF) << 2) |
		(static_cast<byte>(*(_startPoint + 2)) >> 6))]);
	_ret.push_back(base64EncodeMap[(static_cast<byte>(*(_startPoint + 2)) & 0x3F)]);
}

void Base64String::decodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret)
{
	_ret.push_back((check(base64DecodeMap[static_cast<byte>(*(_startPoint))]) << 2) |
		(check(base64DecodeMap[static_cast<byte>(*(_startPoint + 1))]) >> 4));
	_ret.push_back((check(base64DecodeMap[static_cast<byte>(*(_startPoint + 1))]) << 4) |
		(check(base64DecodeMap[static_cast<byte>(*(_startPoint + 2))]) >> 2));
	_ret.push_back((check(base64DecodeMap[static_cast<byte>(*(_startPoint + 2))]) << 6) |
		(check(base64DecodeMap[static_cast<byte>(*(_startPoint + 3))])));
}

void Base64String::encodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret, const size_t _remainingBytes)
{
	_ret.push_back(base64EncodeMap[(static_cast<byte>(*(_startPoint)) >> 2)]);

	if (TwoCharactersRemaining == _remainingBytes)
	{
		_ret.push_back(base64EncodeMap[(((static_cast<byte>(*(_startPoint)) << 4) & 0x30) |
			(static_cast<byte>(*(_startPoint + 1)) >> 4))]);
		_ret.push_back(base64EncodeMap[((static_cast<byte>(*(_startPoint + 1)) << 2) & 0x3C)]);
	}
	else
	{
		_ret.push_back(base64EncodeMap[((static_cast<byte>(*(_startPoint)) << 4) & 0x30)]);
		_ret.push_back(COMPLEMENT_CHARACTER);
	}

	_ret.push_back(COMPLEMENT_CHARACTER);
}

void Base64String::decodeOperation(const std::string::const_iterator& _startPoint, std::string& _ret, const size_t _remainingBytes)
{
	_ret.push_back((check(base64DecodeMap[static_cast<byte>(*(_startPoint))]) << 2) |
		(check(base64DecodeMap[static_cast<byte>(*(_startPoint + 1))]) >> 4));

	if (ThreeCharactersRemaining == _remainingBytes)
	{
		_ret.push_back((check(base64DecodeMap[static_cast<byte>(*(_startPoint + 1))]) << 4) |
			(check(base64DecodeMap[static_cast<byte>(*(_startPoint + 2))]) >> 2));
	}
}