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
		Async(uv::Loop &loop, std::function<void()> handler);
		void			send(uv::Error &er);
		void			send();

	private:
		uv_async_t	m_handle;
		std::function<void()> m_callbackHandler = []() {};
	};





	inline Async::Async(uv::Loop &loop, std::function<void()> handler)
	{
		m_handle.data = this;
        m_callbackHandler = handler;
		uv_async_init(loop.m_loop_ptr, &m_handle, [](uv_async_t *a) {
			auto &async = *reinterpret_cast<uv::Async *>(a->data);
			async.m_callbackHandler();
		});
	}

	inline void Async::send(uv::Error &er)
	{
		er.m_error = uv_async_send(&m_handle);
	}

	inline void Async::send()
	{
		uv::Error er;
		send(er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}
}


#endif
