#ifndef UV_CHECK_HPP
#define UV_CHECK_HPP

#include <functional>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Check : public Noncopyable
	{
	public:
		inline explicit	Check(uv::Loop &loop);

		inline int		start(std::function<void()> cb);
		inline int		stop();

	private:
		uv_check_t		m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	Check::Check(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_check_init(loop.m_loop_ptr, &m_handle);
	}

	int Check::start(std::function<void()> cb)
	{
		m_startHandler = cb;
		return uv_check_start(&m_handle, [](uv_check_t *handle) {
			auto &check = *reinterpret_cast<uv::Check *>(handle->data);
			check.m_startHandler();
		});
	}

	int Check::stop()
	{
		return uv_check_stop(&m_handle);
	}
}


#endif
