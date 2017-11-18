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
		Idle();

		void			init(uv::Loop &loop, std::error_code &ec);
		void			init(uv::Loop &loop);
		void			start(const std::function<void()> &handler, std::error_code &ec);
		void			start(const std::function<void()> &handler);
		void			stop(std::error_code &ec);
		void			stop();

	private:
		uv_idle_t		m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	inline Idle::Idle()
	{
		m_handle.data = this;
	}

	inline void Idle::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_idle_init(loop.value(), &m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Idle::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Idle::start(const std::function<void()> &handler, std::error_code &ec)
	{
		m_startHandler = handler;
		auto status = uv_idle_start(&m_handle, [](uv_idle_t *handle) {
			auto &idle = *reinterpret_cast<uv::Idle *>(handle->data);
			idle.m_startHandler();
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Idle::start(const std::function<void()> &handler)
	{
		std::error_code ec;

		start(handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Idle::stop(std::error_code &ec)
	{
		auto status = uv_idle_stop(&m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Idle::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
