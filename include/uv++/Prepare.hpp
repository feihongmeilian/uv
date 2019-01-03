#ifndef UV_PREPARE_HPP
#define UV_PREPARE_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Handle.hpp"

namespace uv
{
	class Prepare : public Handle<uv_prepare_t>
	{
	public:
		Prepare();

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        start(const std::function<void()> &handler, std::error_code &ec);
		void        start(const std::function<void()> &handler);
		void        stop(std::error_code &ec);
		void        stop();

	private:
		std::function<void()>	startHandler_ = []() {};
	};





	inline Prepare::Prepare()
	{
		handle_.data = this;
	}

	inline void Prepare::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_prepare_init(loop.value(), &handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Prepare::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Prepare::start(const std::function<void()> &handler, std::error_code &ec)
	{
		startHandler_ = handler;
		auto status = uv_prepare_start(&handle_, [](uv_prepare_t *handle) {
			auto &prepare = *reinterpret_cast<uv::Prepare *>(handle->data);
			prepare.startHandler_();
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Prepare::start(const std::function<void()> &handler)
	{
		std::error_code ec;

		start(handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Prepare::stop(std::error_code &ec)
	{
		auto status = uv_prepare_stop(&handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Prepare::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
