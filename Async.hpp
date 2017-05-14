#ifndef UV_ASYNC_HPP
#define UV_ASYNC_HPP

#include <functional>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class Async : public Noncopyable
	{
	public:
		inline Async(uv::Loop &loop, std::function<void(uv::Async &)> handler);
		inline int		send();

	private:
		uv_async_t		m_handle;
		std::function<void(uv::Async &)> m_callbackHandler = nullptr;
	};





	Async::Async(uv::Loop &loop, std::function<void(uv::Async &)> handler)
	{
		m_handle.data = this;
		uv_async_init(&loop.m_loop, &m_handle, [](uv_async_t *a) {
			auto &async = *reinterpret_cast<uv::Async *>(a->data);
			async.m_callbackHandler(async);
		});
	}

	int Async::send()
	{
		return uv_async_send(&m_handle);
	}
}


#endif
