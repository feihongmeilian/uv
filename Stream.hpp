#ifndef UV_STREAM_HPP
#define UV_STREAM_HPP

#include <functional>
#include <algorithm>
#include <memory>

#include <uv.h>

#include "Write.hpp"
#include "Handle.hpp"
#include "Buffer.hpp"
#include "Shutdown.hpp"

namespace uv
{
	template<typename T>
	class Stream : public Handle<T>
	{
	public:
		inline int		listen(std::function<void()> handler, int backlog=SOMAXCONN);
		inline int		accept(uv::Stream<T> &client);
		inline int		readStart(std::function<void(char *data, ssize_t len)> handler);
		inline int		readStop();
		inline int		write(char *p, ssize_t len);
		inline int		shutdown(std::function<void()> handler);
		inline int		isReadable();
		inline int		isWritable();
		inline int		setBlocking(int blocking=true);

	private:
		inline int		write();

	protected:
		using Handle<T>::m_handle;
		using Handle<T>::m_closeHandler;
	private:
		uv::Write		m_write;
		uv::Buffer		m_sendbuf;
	private:
		std::function<void()> m_listenHandler	= []() {};
		std::function<void()> m_shutdownHandler= []() {};
		std::function<void(char *data, ssize_t len)> m_readHandler = [](char *data, ssize_t len) {};
	};







	template<typename T>
	int Stream<T>::listen(std::function<void()> handler,int backlog)
	{
		m_listenHandler = handler;
		return uv_listen(reinterpret_cast<uv_stream_t *>(&m_handle), backlog, [](uv_stream_t *st, int status) {
			if (status) return;
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(st->data);
			stream.m_listenHandler();
		});
	}

	template<typename T>
	int Stream<T>::accept(uv::Stream<T> &client)
	{
		return uv_accept(reinterpret_cast<uv_stream_t *>(&m_handle),
			reinterpret_cast<uv_stream_t *>(&(client.m_handle)));
	}

	template<typename T>
	int Stream<T>::readStart(std::function<void(char *data, ssize_t len)> handler)
	{
		m_readHandler = handler;
		return uv_read_start(reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_handle_t* handle, size_t suggested_size, uv_buf_t *buff)
		{
			buff->base = new char[suggested_size];
			buff->len = suggested_size;
		},
			[](uv_stream_t *handle, ssize_t nread, const uv_buf_t *buff)
		{
			std::shared_ptr<char> bytes(buff->base, std::default_delete<char[]>());
			auto &stream = *reinterpret_cast<Stream<T> *>(handle->data);

			if (nread < 0) {
				stream.m_readHandler(nullptr, nread);
			}				
			else {
				stream.m_readHandler(buff->base, nread);
			}
		});
	}

	template<typename T>
	int Stream<T>::readStop()
	{
		return uv_read_stop(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	int Stream<T>::write(char *p, ssize_t len)
	{
		auto status = m_sendbuf.add(p, len);
		if (status) return status;

		return this->write();
	}

	template<typename T>
	int Stream<T>::write()
	{
		auto buff = m_sendbuf.get();
		if (std::get<1>(buff) == 0) {
			return 0;
		}
		return uv_write(&m_write.m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			std::get<0>(buff), std::get<1>(buff), [](uv_write_t *req, int status) {

			if (status == 0) {
				auto &stream = *reinterpret_cast<Stream<T> *>(req->handle->data);

				stream.m_sendbuf.used();

				stream.write();
			}
		});
	}

	template<typename T>
	int Stream<T>::shutdown(std::function<void()> handler)
	{
		m_shutdownHandler = handler;

		auto req = new (std::nothrow) Shutdown;
		if (req == nullptr) return ENOMEM;

		return uv_shutdown(&req->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_shutdown_t *req, int status) {
			if (!status) {
				auto &stream = *reinterpret_cast<uv::Stream<T> *>(req->handle->data);
				stream.m_shutdownHandler();
			}
			delete reinterpret_cast<Shutdown *>(req->data);
		});
	}

	template<typename T>
	int Stream<T>::isReadable()
	{
		return uv_is_readable(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	int Stream<T>::isWritable()
	{
		return uv_is_writable(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	int Stream<T>::setBlocking(int blocking)
	{
		return uv_stream_set_blocking(reinterpret_cast<uv_stream_t *>(&m_handle), blocking);
	}
}


#endif
