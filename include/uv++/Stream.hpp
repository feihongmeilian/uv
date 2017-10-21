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
		void			listen(std::function<void(const Error &error)> handler, int backlog, uv::Error &er);
		void			listen(std::function<void(const Error &error)> handler, int backlog = SOMAXCONN);
		void			accept(uv::Stream<T> &client, uv::Error &er);
		void			accept(uv::Stream<T> &client);
		void			readStart(std::function<void(char *data, ssize_t len)> handler, uv::Error &er);
		void			readStart(std::function<void(char *data, ssize_t len)> handler);
		void			readStop(uv::Error &er);
		void			readStop();
		void			write(const char *p, ssize_t len, uv::Error &er);
		void			write(const char *p, ssize_t len);
		void			onWrite(std::function<void(const Error &error)>	 handler);
		void			shutdown(std::function<void(const Error &error)> handler, uv::Error &er);
		void			shutdown(std::function<void(const Error &error)> handler);
		bool			isReadable();
		bool			isWritable();
		void			setBlocking(bool blocking, uv::Error &er);
		void			setBlocking(bool blocking = true);
	protected:
		using Handle<T>::m_handle;
		using Handle<T>::m_closeHandler;

	private:
		std::function<void(const Error &error)>	m_listenHandler	= [](const Error &error) {};
		std::function<void(const Error &error)>	m_shutdownHandler	= [](const Error &error) {};
		std::function<void(const Error &error)>	m_writeHandler	= [](const Error &errors) {};
		std::function<void(char *data, ssize_t len)>m_readHandler	= [](char *data, ssize_t len) {};
		
	};





	template<typename T>
	inline void Stream<T>::listen(std::function<void(const Error &error)> handler, int backlog, uv::Error &er)
	{
		m_listenHandler = handler;
		er.m_error = uv_listen(reinterpret_cast<uv_stream_t *>(&m_handle), backlog, [](uv_stream_t *st, int status) {
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(st->data);
			stream.m_listenHandler(Error(status));
		});
	}

	template<typename T>
	inline void Stream<T>::listen(std::function<void(const Error&error)> handler, int backlog)
	{
		uv::Error er;

		listen(handler, backlog, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	template<typename T>
	inline void Stream<T>::accept(uv::Stream<T> &client, uv::Error &er)
	{
		er.m_error = uv_accept(reinterpret_cast<uv_stream_t *>(&m_handle),
			reinterpret_cast<uv_stream_t *>(&(client.m_handle)));
	}

	template<typename T>
	inline void Stream<T>::accept(uv::Stream<T> &client)
	{
		uv::Error er;

		accept(client, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	template<typename T>
	inline void Stream<T>::readStart(std::function<void(char *data, ssize_t len)> handler, uv::Error &er)
	{
		m_readHandler = handler;
		er.m_error = uv_read_start(reinterpret_cast<uv_stream_t *>(&m_handle),
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
		uv::Error er;

		readStart(handler, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	template<typename T>
	inline void Stream<T>::readStop(uv::Error &er)
	{
		er.m_error = uv_read_stop(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	inline void Stream<T>::readStop()
	{
		uv::Error er;

		readStop(er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	//传入的p指针必须是通过new的
	template<typename T>
	inline void Stream<T>::write(const char *p, ssize_t len, uv::Error &er)
	{
		auto writeHandler = new (std::nothrow) uv::Write(p, len);
		if (writeHandler == nullptr) return ENOMEM;

		er.m_error = uv_write(&writeHandler->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
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
		uv::Error er;

		write(p, len, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	template<typename T>
	inline void Stream<T>::onWrite(std::function<void(const Error &error)> handler)
	{
		m_writeHandler = handler;
	}

	template<typename T>
	inline void Stream<T>::shutdown(std::function<void(const Error &error)> handler, uv::Error &er)
	{
		m_shutdownHandler = handler;

		auto req = new (std::nothrow) Shutdown;
		if (req == nullptr) return ENOMEM;

		er.m_error = uv_shutdown(&req->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_shutdown_t *req, int status) {

			std::shared_ptr<uv::Shutdown> shutdown(reinterpret_cast<Shutdown *>(req->data));
			
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(req->handle->data);
			stream.m_shutdownHandler(Error(status));
		});
	}

	template<typename T>
	inline void Stream<T>::shutdown(std::function<void(const Error&error)> handler)
	{
		uv::Error er;

		shutdown(handler, er);
		if (er) {
			throw uv::Exception(er);
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
	inline void Stream<T>::setBlocking(bool blocking, uv::Error &er)
	{
		er.m_error = uv_stream_set_blocking(reinterpret_cast<uv_stream_t *>(&m_handle), blocking);
	}
	template<typename T>
	inline void Stream<T>::setBlocking(bool blocking)
	{
		uv::Error er;

		setBlocking(blocking, er);
		if (er) {
			throw uv::Exception(er);
		}
	}
}


#endif
