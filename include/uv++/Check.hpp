#ifndef UV_CHECK_HPP
#define UV_CHECK_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Handle.hpp"

namespace uv
{
	class Check : public Handle<uv_check_t>
	{
	public:
		Check();

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        start(const std::function<void()> &handler, std::error_code &ec);
		void        start(const std::function<void()> &handler);
		void        stop(std::error_code &ec);
		void        stop();

	private:
		std::function<void()>	startHandler_ = []() {};
	};





	inline Check::Check()
	{
		handle_.data = this;
	}

	inline void Check::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_check_init(loop.value(), &handle_);

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

	inline void Check::start(const std::function<void()> &handler, std::error_code &ec)
	{
		startHandler_ = handler;
		auto status = uv_check_start(&handle_, [](uv_check_t *handle) {
			auto &check = *reinterpret_cast<uv::Check *>(handle->data);
			check.startHandler_();
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Check::start(const std::function<void()> &handler)
	{
		std::error_code ec;

		start(handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Check::stop(std::error_code &ec)
	{
		auto status = uv_check_stop(&handle_);

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
