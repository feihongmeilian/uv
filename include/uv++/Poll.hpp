#ifndef UV_POLL_HPP
#define UV_POLL_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Poll : public Noncopyable
	{
	public:
		Poll(uv::Loop &loop, int fd);
#ifdef _MSC_VER
		Poll(uv::Loop &loop, uv_os_sock_t socket);
#endif
		void			start(int events, std::function<void(const Error &error, int events)> cb, uv::Error &er);
		void			start(int events, std::function<void(const Error &error, int events)> cb);
		void			stop(uv::Error &er);
		void			stop();

	public:
		static const int READABLE		= UV_READABLE;
		static const int WRITABLE		= UV_WRITABLE;
		static const int DISCONNECT	= UV_DISCONNECT;

	private:
		uv_poll_t		m_handle;
		std::function<void(const Error &error, int events)>	m_startHandler
			= [](const Error &error, int events) {};
	};





	inline Poll::Poll(uv::Loop &loop, int fd)
	{
		m_handle.data = this;
		uv_poll_init(loop.m_loop_ptr, &m_handle, fd);
	}

#ifdef _MSC_VER
	inline Poll::Poll(uv::Loop &loop, uv_os_sock_t socket)
	{
		m_handle.data = this;
		uv_poll_init_socket(loop.m_loop_ptr, &m_handle, socket);
	}
#endif

	inline void Poll::start(int events, std::function<void(const Error &error, int events)> cb, uv::Error &er)
	{
		m_startHandler = cb;
		er.m_error = uv_poll_start(&m_handle, events, [](uv_poll_t* handle, int status, int events) {

			auto &poll = *reinterpret_cast<uv::Poll *>(handle->data);
			poll.m_startHandler(Error(status), events);
		});
	}

	inline void Poll::start(int events, std::function<void(const Error&error, int events)> cb)
	{
		uv::Error er;

		start(events, cb, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Poll::stop(uv::Error &er)
	{
		er.m_error = uv_poll_stop(&m_handle);
	}

	inline void Poll::stop()
	{
		uv::Error er;

		stop(er);
		if (er) {
			throw uv::Exception(er);
		}
	}
}


#endif
