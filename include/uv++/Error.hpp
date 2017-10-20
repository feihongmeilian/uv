#ifndef UV_ERROR_HPP
#define UV_ERROR_HPP

#include <uv.h>

namespace uv
{

	class Error
	{
	public:
		inline explicit	Error(int status);
		inline explicit	Error(const Error &error);
		inline Error	&operator=(const Error &error);

		inline operator	bool() const;
		inline const char	*toString() const;
		inline const char	*name() const;

	private:
		int				m_error;
	};





	Error::Error(int status)
	{
		m_error = status;
	}

	Error::Error(const Error &error)
	{
		m_error = error.m_error;
	}

	Error &Error::operator=(const Error &error)
	{
		if (this == &error) return *this;

		m_error = error.m_error;

		return *this;
	}
	
	Error::operator bool() const
	{
		return m_error != 0;
	}

	const char *Error::toString() const
	{
		return uv_strerror(m_error);
	}
	
	const char *Error::name() const
	{
		return uv_err_name(m_error);
	}
}


#endif
