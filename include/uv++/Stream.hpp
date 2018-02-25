#ifndef UV_STREAM_HPP
#define UV_STREAM_HPP

#include <functional>
#include <memory>

#include <uv.h>

#include "Error.hpp"
#include "Write.hpp"
#include "Shutdown.hpp"
#include "FileHandle.hpp"

namespace uv
{
	template<typename T>
	class Stream : public FileHandle<T>
	{
	public:
		void			listen(const std::function<void(const std::error_code &ec)> &handler, int backlog, std::error_code &ec);
		void			listen(const std::function<void(const std::error_code &ec)> &handler, int backlog = SOMAXCONN);
		void			accept(uv::Stream<T> &client, std::error_code &ec);
		void			accept(uv::Stream<T> &client);
		void			readStart(const std::function<void(char *data, ssize_t len)> &handler, std::error_code &ec);
		void			readStart(const std::function<void(char *data, ssize_t len)> &handler);
		void			readStop(std::error_code &ec);
		void			readStop();
		void			write(const char *p, ssize_t len, std::error_code &ec);
		//传入的p指针必须是通过new的
		void			write(const char *p, ssize_t len);
		void			onWrite(const std::function<void(const std::error_code &ec)> &handler);
		void			shutdown(const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec);
		void			shutdown(const std::function<void(const std::error_code &ec)> &handler);
		bool			isReadable();
		bool			isWritable();
		void			setBlocking(bool blocking, std::error_code &ec);
		void			setBlocking(bool blocking = true);

	private:
		std::function<void(const std::error_code &ec)>	m_listenHandler	= [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	m_shutdownHandler	= [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	m_writeHandler	= [](const std::error_code &ec) {};
		std::function<void(char *data, ssize_t len)>	m_readHandler	= [](char *data, ssize_t len) {};
		
	};





	template<typename T>
	inline void Stream<T>::listen(const std::function<void(const std::error_code &ec)> &handler, int backlog, std::error_code &ec)
	{
		m_listenHandler = handler;
		auto status = uv_listen(reinterpret_cast<uv_stream_t *>(&m_handle), backlog, [](uv_stream_t *st, int status) {
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(st->data);
			stream.m_listenHandler(makeErrorCode(status));
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void Stream<T>::listen(const std::function<void(const std::error_code &ec)> &handler, int backlog)
	{
		std::error_code ec;

		listen(handler, backlog, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	template<typename T>
	inline void Stream<T>::accept(uv::Stream<T> &client, std::error_code &ec)
	{
		auto status = uv_accept(reinterpret_cast<uv_stream_t *>(&m_handle),
			reinterpret_cast<uv_stream_t *>(&(client.m_handle)));

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void Stream<T>::accept(uv::Stream<T> &client)
	{
		std::error_code ec;

		accept(client, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	template<typename T>
	inline void Stream<T>::readStart(const std::function<void(char *data, ssize_t len)> &handler, std::error_code &ec)
	{
		m_readHandler = handler;
		auto status = uv_read_start(reinterpret_cast<uv_stream_t *>(&m_handle),
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

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void Stream<T>::readStart(const std::function<void(char *data, ssize_t len)> &handler)
	{
		std::error_code ec;

		readStart(handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	template<typename T>
	inline void Stream<T>::readStop(std::error_code &ec)
	{
		auto status = uv_read_stop(reinterpret_cast<uv_stream_t *>(&m_handle));

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void Stream<T>::readStop()
	{
		std::error_code ec;

		readStop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	template<typename T>
	inline void Stream<T>::write(const char *p, ssize_t len, std::error_code &ec)
	{
		auto writeHandler = new (std::nothrow) uv::Write(p, len);
		if (writeHandler == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		auto status = uv_write(&writeHandler->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			&writeHandler->m_buf, 1, [](uv_write_t *req, int status) {
			std::shared_ptr<uv::Write> writeHandler(reinterpret_cast<uv::Write *>(req->data));
			std::shared_ptr<char> bytes(writeHandler->m_buf.base, std::default_delete<char[]>());

			auto &stream = *reinterpret_cast<Stream<T> *>(req->handle->data);
			stream.m_writeHandler(makeErrorCode(status));
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void Stream<T>::write(const char *p, ssize_t len)
	{
		std::error_code ec;

		write(p, len, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	template<typename T>
	inline void Stream<T>::onWrite(const std::function<void(const std::error_code &ec)> &handler)
	{
		m_writeHandler = handler;
	}

	template<typename T>
	inline void Stream<T>::shutdown(const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec)
	{
		m_shutdownHandler = handler;

		auto req = new (std::nothrow) Shutdown;
		if (req == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		auto status = uv_shutdown(&req->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_shutdown_t *req, int status) {
			std::shared_ptr<uv::Shutdown> shutdown(reinterpret_cast<Shutdown *>(req->data));
			
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(req->handle->data);
			stream.m_shutdownHandler(makeErrorCode(status));
		});


		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void Stream<T>::shutdown(const std::function<void(const std::error_code &ec)> &handler)
	{
		std::error_code ec;

		shutdown(handler, ec);
		if (ec) {
			throw uv::Exception(ec);
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
	inline void Stream<T>::setBlocking(bool blocking, std::error_code &ec)
	{
		auto status = uv_stream_set_blocking(reinterpret_cast<uv_stream_t *>(&m_handle), blocking);


		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}
	template<typename T>
	inline void Stream<T>::setBlocking(bool blocking)
	{
		std::error_code ec;

		setBlocking(blocking, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

}


#endif
