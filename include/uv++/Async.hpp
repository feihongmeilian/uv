#ifndef UV_ASYNC_HPP
#define UV_ASYNC_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Handle.hpp"

namespace uv
{
	class Async : public Handle<uv_async_t>
	{
	public:
		Async();

		void        init(uv::Loop &loop, const std::function<void()> &handler, std::error_code &ec);
		void        init(uv::Loop &loop, const std::function<void()> &handler);
		void        send(std::error_code &ec);
		void        send();

	private:
		std::function<void()> callbackHandler_ = []() {};
	};





	inline Async::Async()
	{
		handle_.data = this;
	}

	inline void Async::init(uv::Loop &loop, const std::function<void()> &handler, std::error_code & ec)
	{
		callbackHandler_ = handler;
		auto status = uv_async_init(loop.value(), &handle_, [](uv_async_t *a) {
			auto &async = *reinterpret_cast<uv::Async *>(a->data);
			async.callbackHandler_();
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Async::init(uv::Loop &loop, const std::function<void()> &handler)
	{
		std::error_code ec;

		init(loop, handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Async::send(std::error_code &ec)
	{
		const auto status = uv_async_send(&handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Async::send()
	{
		std::error_code ec;

		send(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
