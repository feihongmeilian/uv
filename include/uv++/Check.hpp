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
		explicit		Check(uv::Loop &loop);

		void			start(std::function<void()> cb, uv::Error &err);
		void			start(std::function<void()> cb);
		void			stop(uv::Error &err);
		void			stop();

	private:
		uv_check_t		m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	inline Check::Check(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_check_init(loop.m_loop_ptr, &m_handle);
	}

	inline void Check::start(std::function<void()> cb, uv::Error &err)
	{
		m_startHandler = cb;
		err.m_error = uv_check_start(&m_handle, [](uv_check_t *handle) {
			auto &check = *reinterpret_cast<uv::Check *>(handle->data);
			check.m_startHandler();
		});
	}

	inline void Check::start(std::function<void()> cb)
	{
		uv::Error err;

		start(cb, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Check::stop(uv::Error &err)
	{
		err.m_error = uv_check_stop(&m_handle);
	}

	inline void Check::stop()
	{
		uv::Error err;

		stop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
