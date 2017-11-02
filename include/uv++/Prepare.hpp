#ifndef UV_PREPARE_HPP
#define UV_PREPARE_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Prepare : public Noncopyable
	{
	public:
		explicit		Prepare(uv::Loop &loop);

		void			start(std::function<void()> cb, uv::Error &err);
		void			start(std::function<void()> cb);
		void			stop(uv::Error &err);
		void			stop();

	private:
		uv_prepare_t	m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	inline Prepare::Prepare(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_prepare_init(loop.m_loop_ptr, &m_handle);
	}

	inline void Prepare::start(std::function<void()> cb, uv::Error &err)
	{
		m_startHandler = cb;
		err.m_error = uv_prepare_start(&m_handle, [](uv_prepare_t *handle) {
			auto &prepare = *reinterpret_cast<uv::Prepare *>(handle->data);
			prepare.m_startHandler();
		});
	}

	inline void Prepare::start(std::function<void()> cb)
	{
		uv::Error err;

		start(cb, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Prepare::stop(uv::Error &err)
	{
		err.m_error = uv_prepare_stop(&m_handle);
	}

	inline void Prepare::stop()
	{
		uv::Error err;

		stop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
