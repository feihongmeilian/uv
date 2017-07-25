#ifndef UV_PREPARE_HPP
#define UV_PREPARE_HPP

#include <functional>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Prepare : public Noncopyable
	{
	public:
		inline explicit	Prepare(uv::Loop &loop);

		inline int		start(std::function<void()> cb);
		inline int		stop();

	private:
		uv_prepare_t		m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	Prepare::Prepare(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_prepare_init(&loop.m_loop, &m_handle);
	}

	int Prepare::start(std::function<void()> cb)
	{
		m_startHandler = cb;
		return uv_prepare_start(&m_handle, [](uv_prepare_t *handle) {
			auto &prepare = *reinterpret_cast<uv::Prepare *>(handle->data);
			prepare.m_startHandler();
		});
	}

	int Prepare::stop()
	{
		return uv_prepare_stop(&m_handle);
	}
}


#endif
