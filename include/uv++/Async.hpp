#ifndef UV_ASYNC_HPP
#define UV_ASYNC_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Async : public Noncopyable
	{
	public:
		Async();

		void			init(uv::Loop &loop, const std::function<void()> &handler, std::error_code &ec);
		void			init(uv::Loop &loop, const std::function<void()> &handler);
		void			send(std::error_code &ec);
		void			send();

	private:
		uv_async_t		m_handle;
		std::function<void()> m_callbackHandler = []() {};
	};





	inline Async::Async()
	{
		m_handle.data = this;
	}

	inline void Async::init(uv::Loop &loop, const std::function<void()> &handler, std::error_code & ec)
	{
		m_callbackHandler = handler;
		auto status = uv_async_init(loop.value(), &m_handle, [](uv_async_t *a) {
			auto &async = *reinterpret_cast<uv::Async *>(a->data);
			async.m_callbackHandler();
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
		auto status = uv_async_send(&m_handle);

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
