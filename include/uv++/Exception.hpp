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
		explicit			Exception(const uv::Error &err);
		virtual const char	*what() const throw() override;

	private:
		uv::Error			m_error;
	};





	inline Exception::Exception(const uv::Error &err)
		:m_error(err)
	{
	}

	inline const char *Exception::what() const throw()
	{
		return m_error.toString();
	}
}

#endif
