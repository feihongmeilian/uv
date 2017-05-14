#ifndef UV_IDLE_HPP
#define UV_IDLE_HPP

#include <functional>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Idle : public Noncopyable
	{
	public:
		inline explicit	Idle(uv::Loop &loop);

		inline int		start(std::function<void(uv::Idle &)> cb);
		inline int		stop();

	private:
		uv_idle_t		m_handle;

	private:
		std::function<void(uv::Idle &)>	m_startHandler;
	};





	Idle::Idle(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_idle_init(&loop.m_loop, &m_handle);
	}

	int Idle::start(std::function<void(uv::Idle &)> cb)
	{
		m_startHandler = cb;
		return uv_idle_start(&m_handle, [](uv_idle_t *handle) {
			auto &idle = *reinterpret_cast<uv::Idle *>(handle->data);
			idle.m_startHandler(idle);
		});
	}

	int Idle::stop()
	{
		return uv_idle_stop(&m_handle);
	}
}


#endif
