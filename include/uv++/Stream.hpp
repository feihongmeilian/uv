#ifndef UV_STREAM_HPP
#define UV_STREAM_HPP

#include <functional>
#include <algorithm>
#include <memory>

#include <uv.h>

#include "Error.hpp"
#include "Write.hpp"
#include "Handle.hpp"
#include "Shutdown.hpp"

namespace uv
{
	template<typename T>
	class Stream : public Handle<T>
	{
	public:
		void			listen(std::function<void(const Error &error)> handler, int backlog, uv::Error &err);
		void			listen(std::function<void(const Error &error)> handler, int backlog = SOMAXCONN);
		void			accept(uv::Stream<T> &client, uv::Error &err);
		void			accept(uv::Stream<T> &client);
		void			readStart(std::function<void(char *data, ssize_t len)> handler, uv::Error &err);
		void			readStart(std::function<void(char *data, ssize_t len)> handler);
		void			readStop(uv::Error &err);
		void			readStop();
		void			write(const char *p, ssize_t len, uv::Error &err);
		void			write(const char *p, ssize_t len);
		void			onWrite(std::function<void(const Error &error)>	 handler);
		void			shutdown(std::function<void(const Error &error)> handler, uv::Error &err);
		void			shutdown(std::function<void(const Error &error)> handler);
		bool			isReadable();
		bool			isWritable();
		void			setBlocking(bool blocking, uv::Error &err);
		void			setBlocking(bool blocking = true);
	protected:
		using Handle<T>::m_handle;
		using Handle<T>::m_closeHandler;

	private:
		std::function<void(const Error &error)>	m_listenHandler	= [](const Error &error) {};
		std::function<void(const Error &error)>	m_shutdownHandler	= [](const Error &error) {};
		std::function<void(const Error &error)>	m_writeHandler	= [](const Error &error) {};
		std::function<void(char *data, ssize_t len)>m_readHandler	= [](char *data, ssize_t len) {};
		
	};





	template<typename T>
	inline void Stream<T>::listen(std::function<void(const Error &error)> handler, int backlog, uv::Error &err)
	{
		m_listenHandler = handler;
		err.m_error = uv_listen(reinterpret_cast<uv_stream_t *>(&m_handle), backlog, [](uv_stream_t *st, int status) {
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(st->data);
			stream.m_listenHandler(Error(status));
		});
	}

	template<typename T>
	inline void Stream<T>::listen(std::function<void(const Error&error)> handler, int backlog)
	{
		uv::Error err;

		listen(handler, backlog, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	template<typename T>
	inline void Stream<T>::accept(uv::Stream<T> &client, uv::Error &err)
	{
		err.m_error = uv_accept(reinterpret_cast<uv_stream_t *>(&m_handle),
			reinterpret_cast<uv_stream_t *>(&(client.m_handle)));
	}

	template<typename T>
	inline void Stream<T>::accept(uv::Stream<T> &client)
	{
		uv::Error err;

		accept(client, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	template<typename T>
	inline void Stream<T>::readStart(std::function<void(char *data, ssize_t len)> handler, uv::Error &err)
	{
		m_readHandler = handler;
		err.m_error = uv_read_start(reinterpret_cast<uv_stream_t *>(&m_handle),
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
	inline void Stream<T>::readStart(std::function<void(char *data, ssize_t len)> handler)
	{
		uv::Error err;

		readStart(handler, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	template<typename T>
	inline void Stream<T>::readStop(uv::Error &err)
	{
		err.m_error = uv_read_stop(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	inline void Stream<T>::readStop()
	{
		uv::Error err;

		readStop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	//传入的p指针必须是通过new的
	template<typename T>
	inline void Stream<T>::write(const char *p, ssize_t len, uv::Error &err)
	{
		auto writeHandler = new (std::nothrow) uv::Write(p, len);
		if (writeHandler == nullptr) return ENOMEM;

		err.m_error = uv_write(&writeHandler->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			&writeHandler->m_buf, 1, [](uv_write_t *req, int status) {
			std::shared_ptr<uv::Write> writeHandler(reinterpret_cast<uv::Write *>(req->data));
			std::shared_ptr<char> bytes(writeHandler->m_buf.base, std::default_delete<char[]>());

			auto &stream = *reinterpret_cast<Stream<T> *>(req->handle->data);
			stream.m_writeHandler(Error(status));
		});
	}

	template<typename T>
	inline void Stream<T>::write(const char *p, ssize_t len)
	{
		uv::Error err;

		write(p, len, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	template<typename T>
	inline void Stream<T>::onWrite(std::function<void(const Error &error)> handler)
	{
		m_writeHandler = handler;
	}

	template<typename T>
	inline void Stream<T>::shutdown(std::function<void(const Error &error)> handler, uv::Error &err)
	{
		m_shutdownHandler = handler;

		auto req = new (std::nothrow) Shutdown;
		if (req == nullptr) return ENOMEM;

		err.m_error = uv_shutdown(&req->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_shutdown_t *req, int status) {
			std::shared_ptr<uv::Shutdown> shutdown(reinterpret_cast<Shutdown *>(req->data));
			
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(req->handle->data);
			stream.m_shutdownHandler(Error(status));
		});
	}

	template<typename T>
	inline void Stream<T>::shutdown(std::function<void(const Error&error)> handler)
	{
		uv::Error err;

		shutdown(handler, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	template<typename T>
	inline bool Stream<T>::isReadable()
	{
		return uv_is_readable(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	inline bool Stream<T>::isWritable()
	{
		return uv_is_writable(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	inline void Stream<T>::setBlocking(bool blocking, uv::Error &err)
	{
		err.m_error = uv_stream_set_blocking(reinterpret_cast<uv_stream_t *>(&m_handle), blocking);
	}
	template<typename T>
	inline void Stream<T>::setBlocking(bool blocking)
	{
		uv::Error err;

		setBlocking(blocking, err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
