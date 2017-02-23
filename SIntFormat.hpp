
#ifndef	_SIGNED_INT_FORMAT_HPP_
#define	_SIGNED_INT_FORMAT_HPP_

#include <string>

class SIntFormat
{
private:
	const std::string str_sign = "+-";
	const std::string str_digit = "0123456789";

	bool		valid;
	std::string	digits;
	int		sign;

	int int_simple(const std::string& num) {
		valid = false;
		sign = 1;	/* Default to positive '+' */

		size_t pSign  = num.find_first_of( str_sign );
		size_t pValue = num.find_first_not_of( str_sign );

		if (pSign == std::string::npos) {
			/* No sign: default */
		} else if (pSign + 1 != pValue) {
			/* Wrong format */
			if (pValue < pSign) {
				/* [:digit:][+-] */
			} else {
				/* [+-]+[:digit:] */
			}
			return -EINVAL;
		} else {
			/* pSign < pValue */
			if (num.at(pSign) == '-')
				sign = -1;

			/* If it's not '-', it must be '+' : find_first_of() */
			/* We don't handle '+' case, as it's default case */
		}

		digits = num.substr(pValue);	/* [[:digit:]] */
		if (digits.find_first_not_of( str_digit ) != std::string::npos) {
			/* [+-][[:digit:]]\w */
			return -EINVAL;
		}

		valid = true;
		return 0;
	}

public:
	SIntFormat(const std::string& num) {
		// int_regex(num);
		int_simple( num );
	};
	~SIntFormat() {};

	bool isValid() {
		return valid;
	}

	int getSign() {
		return sign;
	}

	std::string& getDigits() {
		return digits;
	}
};

#endif	/* _SIGNED_INT_FORMAT_HPP_ */
