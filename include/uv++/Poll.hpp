#ifndef UV_POLL_HPP
#define UV_POLL_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "private/FileHandle.hpp"

namespace uv
{
	class Poll : public FileHandle<uv_poll_t>
	{
	public:
		Poll();

		void			init(uv::Loop &loop, int fd, std::error_code &ec);
		void			init(uv::Loop &loop, int fd);
		void			initSocket(uv::Loop &loop, uv_os_sock_t socket, std::error_code &ec);
		void			initSocket(uv::Loop &loop, uv_os_sock_t socket);
		void			start(std::error_code &ec);
		void			start();
		void			stop(std::error_code &ec);
		void			stop();

		void			onReadable(const std::function<void(const std::error_code &ec)> &handler);
		void			onWritable(const std::function<void(const std::error_code &ec)> &handler);
		void			onDisconnect(const std::function<void(const std::error_code &ec)> &handler);
		void			onPrioritized(const std::function<void(const std::error_code &ec)> &handler);

	private:
		int			m_events = 0;
		std::function<void(const std::error_code &ec)>	m_readableHandler = [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	m_writableHandler = [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	m_disconnectHandler = [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	m_prioritizedHandler = [](const std::error_code &ec) {};
	};





	inline Poll::Poll()
	{
		m_handle.data = this;
	}
	
	inline void Poll::init(uv::Loop &loop, int fd, std::error_code &ec)
	{
		auto status = uv_poll_init(loop.value(), &m_handle, fd);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::init(uv::Loop &loop, int fd)
	{
		std::error_code ec;

		init(loop, fd, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::initSocket(uv::Loop &loop, uv_os_sock_t socket, std::error_code &ec)
	{
		auto status = uv_poll_init_socket(loop.value(), &m_handle, socket);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::initSocket(uv::Loop &loop, uv_os_sock_t socket)
	{
		std::error_code ec;

		initSocket(loop, socket, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::start(std::error_code &ec)
	{
		auto status = uv_poll_start(&m_handle, m_events, [](uv_poll_t* handle, int status, int events) {
			auto &poll = *reinterpret_cast<uv::Poll *>(handle->data);
			if (events & UV_READABLE) {
				poll.m_readableHandler(makeErrorCode(status));
			}
			if (events & UV_WRITABLE) {
				poll.m_writableHandler(makeErrorCode(status));
			}
			if (events & UV_DISCONNECT) {
				poll.m_disconnectHandler(makeErrorCode(status));
			}
			if (events & UV_PRIORITIZED) {
				poll.m_prioritizedHandler(makeErrorCode(status));
			}
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::start()
	{
		std::error_code ec;

		start(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::stop(std::error_code &ec)
	{
		auto status = uv_poll_stop(&m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::onReadable(const std::function<void(const std::error_code &ec)> &handler)
	{
		m_events += UV_READABLE;
		m_readableHandler = handler;
	}

	inline void Poll::onWritable(const std::function<void(const std::error_code &ec)> &handler)
	{
		m_events += UV_WRITABLE;
		m_writableHandler = handler;
	}

	inline void Poll::onDisconnect(const std::function<void(const std::error_code &ec)> &handler)
	{
		m_events += UV_DISCONNECT;
		m_disconnectHandler = handler;
	}

	inline void Poll::onPrioritized(const std::function<void(const std::error_code &ec)> &handler)
	{
		m_events += UV_PRIORITIZED;
		m_prioritizedHandler = handler;
	}
}


#endif
