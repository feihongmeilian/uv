#ifndef UV_POLL_HPP
#define UV_POLL_HPP

#include <functional>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Poll : public Noncopyable
	{
	public:
		inline Poll(uv::Loop &loop, int fd);
		inline Poll(uv::Loop &loop, uv_os_sock_t socket);

		inline int		start(int events, std::function<void(uv::Poll &)> cb);
		inline int		stop();

	private:
		uv_poll_t		m_handle;

	private:
		std::function<void(uv::Poll &)>	m_startHandler;
	};





	Poll::Poll(uv::Loop &loop, int fd)
	{
		m_handle.data = this;
		uv_poll_init(&loop.m_loop, &m_handle, fd);
	}

	Poll::Poll(uv::Loop &loop, uv_os_sock_t socket)
	{
		m_handle.data = this;
		uv_poll_init_socket(&loop.m_loop, &m_handle, socket);
	}

	int Poll::start(int events, std::function<void(uv::Poll &)> cb)
	{
		m_startHandler = cb;
		return uv_poll_start(&m_handle, events, [](uv_poll_t* handle, int status, int events) {
			if (status) return;

			auto &poll = *reinterpret_cast<uv::Poll *>(handle->data);
			poll.m_startHandler(poll);
		});
	}

	int Poll::stop()
	{
		return uv_poll_stop(&m_handle);
	}
}


#endif
