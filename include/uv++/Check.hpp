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
		Check();

		void			init(uv::Loop &loop, std::error_code &ec);
		void			init(uv::Loop &loop);
		void			start(std::function<void()> cb, std::error_code &ec);
		void			start(std::function<void()> cb);
		void			stop(std::error_code &ec);
		void			stop();

	private:
		uv_check_t		m_handle;

	private:
		std::function<void()>	m_startHandler = []() {};
	};





	inline Check::Check()
	{
		m_handle.data = this;
	}

	inline void Check::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_check_init(loop.value(), &m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Check::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Check::start(std::function<void()> cb, std::error_code &ec)
	{
		m_startHandler = cb;
		auto status = uv_check_start(&m_handle, [](uv_check_t *handle) {
			auto &check = *reinterpret_cast<uv::Check *>(handle->data);
			check.m_startHandler();
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Check::start(std::function<void()> cb)
	{
		std::error_code ec;

		start(cb, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Check::stop(std::error_code &ec)
	{
		auto status = uv_check_stop(&m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Check::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
