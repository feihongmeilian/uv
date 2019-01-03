#ifndef UV_EXCEPTION_HPP
#define UV_EXCEPTION_HPP

#include <stdexcept>
#include <string>

#include <uv.h>

#include "Error.hpp"

namespace uv
{
	class Exception : public std::exception
	{
	public:
		Exception(const std::string  &msg, std::error_code ec = makeErrorCode(0))
			: msg_(msg.empty() ? ec.message() : msg), code_(ec)
		{}

		explicit Exception(std::error_code ec)
			: msg_(ec.message()), code_(ec)
		{}

		~Exception() throw() {}

		virtual char const * what() const throw()
		{
			return msg_.c_str();
		}

		std::error_code code() const throw()
		{
			return code_;
		}

		const std::string msg_;
		std::error_code code_;
	};
}

#endif
