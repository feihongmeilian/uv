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
		void			onReadable(std::function<void(const Error &error)> cb);
		void			onWritable(std::function<void(const Error &error)> cb);
		void			onDisconnect(std::function<void(const Error &error)> cb);
		void			onPrioritized(std::function<void(const Error &error)> cb);
		void			start(uv::Error &er);
		void			start();
		void			stop(uv::Error &er);
		void			stop();

	private:
		uv_poll_t		m_handle;
		int				m_events = 0;
		std::function<void(const Error &error)>	m_readableHandler = [](const Error &error) {};
		std::function<void(const Error &error)>	m_writableHandler = [](const Error &error) {};
		std::function<void(const Error &error)>	m_disconnectHandler = [](const Error &error) {};
		std::function<void(const Error &error)>	m_prioritizedHandler = [](const Error &error) {};
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

	inline void Poll::onReadable(std::function<void(const Error&error)> cb)
	{
		m_events += UV_READABLE;
		m_readableHandler = cb;
	}

	inline void Poll::onWritable(std::function<void(const Error&error)> cb)
	{
		m_events += UV_WRITABLE;
		m_writableHandler = cb;
	}

	inline void Poll::onDisconnect(std::function<void(const Error&error)> cb)
	{
		m_events += UV_DISCONNECT;
		m_disconnectHandler = cb;
	}

	inline void Poll::onPrioritized(std::function<void(const Error&error)> cb)
	{
		m_events += UV_PRIORITIZED;
		m_prioritizedHandler = cb;
	}

	inline void Poll::start(uv::Error &er)
	{
		er.m_error = uv_poll_start(&m_handle, m_events, [](uv_poll_t* handle, int status, int events) {
			auto &poll = *reinterpret_cast<uv::Poll *>(handle->data);
			if (events & UV_READABLE) {
				poll.m_readableHandler(Error(status));
			}
			if (events & UV_WRITABLE) {
				poll.m_writableHandler(Error(status));
			}
			if (events & UV_DISCONNECT) {
				poll.m_disconnectHandler(Error(status));
			}
			if (events & UV_PRIORITIZED) {
				poll.m_prioritizedHandler(Error(status));
			}
		});
	}

	inline void Poll::start()
	{
		uv::Error er;

		start(er);
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
