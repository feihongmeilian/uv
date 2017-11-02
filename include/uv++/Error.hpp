#ifndef UV_ERROR_HPP
#define UV_ERROR_HPP

#include <uv.h>

namespace uv
{
	class Pipe;
	class Tty;
	class Tcp;
	class Udp;
	class Check;
	class Timer;
	class Signal;
	class Prepare;
	class Poll;
	class Idle;
	class Async;
	class Work;
	class Loop;
	class FileStream;
	class FileStreamPoll;
	class FileStreamEvent;
	class GetAddrInfo;

	template<typename T> class Stream;
	template<typename T> class Handle;
	class Error
	{
	public:
		Error() = default;
		explicit		Error(int error);
						Error(const Error &error);
		Error			&operator=(const Error &error);

		operator		bool() const;
		const char		*toString() const;
		const char		*name() const;

	private:
		friend class Pipe;
		friend class Tcp;
		friend class Udp;
		friend class Check;
		friend class Timer;
		friend class Signal;
		friend class Prepare;
		friend class Poll;
		friend class Idle;
		friend class Tty;
		friend class Async;
		friend class Work;
		friend class FileStream;
		friend class FileStreamPoll;
		friend class FileStreamEvent;
		friend class GetAddrInfo;
		friend class Loop;

		template<typename T> friend class Stream;
		template<typename T> friend class Handle;
	private:
		int				m_error = 0;
	};







	inline Error::Error(int error)
	{
		m_error = error;
	}

	inline Error::Error(const Error &error)
	{
		m_error = error.m_error;
	}

	inline Error &Error::operator=(const Error &error)
	{
		if (this == &error) return *this;

		m_error = error.m_error;

		return *this;
	}
	
	inline Error::operator bool() const
	{
		return m_error != 0;
	}

	inline const char *Error::toString() const
	{
		return uv_strerror(m_error);
	}
	
	inline const char *Error::name() const
	{
		return uv_err_name(m_error);
	}
}


#endif
