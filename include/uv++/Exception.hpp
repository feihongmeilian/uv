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
			: m_msg(msg.empty() ? ec.message() : msg), m_code(ec)
		{}

		explicit Exception(std::error_code ec)
			: m_msg(ec.message()), m_code(ec)
		{}

		~Exception() throw() {}

		virtual char const * what() const throw()
		{
			return m_msg.c_str();
		}

		std::error_code code() const throw()
		{
			return m_code;
		}

		const std::string m_msg;
		std::error_code m_code;
	};
}

#endif
