#ifndef UV_CHECK_HPP
#define UV_CHECK_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Check : public Noncopyable
	{
	public:
		explicit	Check(uv::Loop &loop);

		void			start(std::function<void()> cb, uv::Error &er);
		void			start(std::function<void()> cb);
		void			stop(uv::Error &er);
		void			stop();

	private:
		uv_check_t	m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	inline Check::Check(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_check_init(loop.m_loop_ptr, &m_handle);
	}

	inline void Check::start(std::function<void()> cb, uv::Error &er)
	{
		m_startHandler = cb;
		er.m_error = uv_check_start(&m_handle, [](uv_check_t *handle) {
			auto &check = *reinterpret_cast<uv::Check *>(handle->data);
			check.m_startHandler();
		});
	}

	inline void Check::start(std::function<void()> cb)
	{
		uv::Error er;

		start(cb, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Check::stop(uv::Error &er)
	{
		er.m_error = uv_check_stop(&m_handle);
	}

	inline void Check::stop()
	{
		uv::Error er;

		stop(er);
		if (er) {
			throw uv::Exception(er);
		}
	}
}


#endif
