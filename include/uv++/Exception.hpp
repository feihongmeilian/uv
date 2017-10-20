#ifndef UV_EXCEPTION_HPP
#define UV_EXCEPTION_HPP

#include <stdexcept>

#include <uv.h>

#include "Error.hpp"

namespace uv
{
	class Exception :public std::exception
	{
	public:
		inline explicit Exception(const uv::Error &error);
		inline virtual const char *what() const throw();

	private:
		uv::Error m_error;
	};





	Exception::Exception(const uv::Error &error)
		:m_error(error)
	{
	}

	const char *Exception::what() const throw()
	{
		return m_error.toString();
	}
}

#endif
