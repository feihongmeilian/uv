#ifndef UV_IDLE_HPP
#define UV_IDLE_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Handle.hpp"

namespace uv
{
	class Idle : public Handle<uv_idle_t>
	{
	public:
		Idle();

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        start(const std::function<void()> &handler, std::error_code &ec);
		void        start(const std::function<void()> &handler);
		void        stop(std::error_code &ec);
		void        stop();

	private:
		std::function<void()>	startHandler_ = []() {};
	};





	inline Idle::Idle()
	{
		handle_.data = this;
	}

	inline void Idle::init(uv::Loop &loop, std::error_code &ec)
	{
		const auto status = uv_idle_init(loop.value(), &handle_);

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
		startHandler_ = handler;
		auto status = uv_idle_start(&handle_, [](uv_idle_t *handle) {
			auto &idle = *reinterpret_cast<uv::Idle *>(handle->data);
			idle.startHandler_();
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
		const auto status = uv_idle_stop(&handle_);

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
