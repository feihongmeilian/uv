#ifndef UV_IDLE_HPP
#define UV_IDLE_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Idle : public Noncopyable
	{
	public:
		explicit		Idle(uv::Loop &loop);

		void			start(std::function<void()> cb, uv::Error &err);
		void			start(std::function<void()> cb);
		void			stop(uv::Error &err);
		void			stop();

	private:
		uv_idle_t		m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	inline Idle::Idle(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_idle_init(loop.m_loop_ptr, &m_handle);
	}

	inline void Idle::start(std::function<void()> cb, uv::Error &err)
	{
		m_startHandler = cb;
		err.m_error = uv_idle_start(&m_handle, [](uv_idle_t *handle) {
			auto &idle = *reinterpret_cast<uv::Idle *>(handle->data);
			idle.m_startHandler();
		});
	}

	inline void Idle::start(std::function<void()> cb)
	{
		uv::Error err;

		start(cb, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Idle::stop(uv::Error &err)
	{
		err.m_error = uv_idle_stop(&m_handle);
	}

	inline void Idle::stop()
	{
		uv::Error err;

		stop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
