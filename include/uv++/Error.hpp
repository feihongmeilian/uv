#ifndef UV_ERROR_HPP
#define UV_ERROR_HPP

#include <uv.h>

namespace uv
{

	class Error
	{
	public:
		inline explicit	Error(int status);

		inline operator	bool() const;
		inline const char	*toString();
		inline const char	*name();

	private:
		int				m_error;
	};





	Error::Error(int status)
	{
		m_error = status;
	}
	
	Error::operator bool() const
	{
		return m_error != 0;
	}

	const char *Error::toString()
	{
		return uv_strerror(m_error);
	}
	
	const char *Error::name()
	{
		return uv_err_name(m_error);
	}
}


#endif
