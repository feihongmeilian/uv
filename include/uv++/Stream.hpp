#ifndef UV_STREAM_HPP
#define UV_STREAM_HPP

#include <functional>
#include <memory>
#include <cstring>
#include <sstream>

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
		void        listen(const std::function<void(const std::error_code &ec)> &handler, int backlog, std::error_code &ec);
		void        listen(const std::function<void(const std::error_code &ec)> &handler, int backlog = SOMAXCONN);
		void        accept(uv::Stream<T> &client, std::error_code &ec);
		void        accept(uv::Stream<T> &client);
		void        readStart(const std::function<void(char *data, ssize_t len)> &handler, std::error_code &ec);
		void        readStart(const std::function<void(char *data, ssize_t len)> &handler);
		void        readStop(std::error_code &ec);
		void        readStop();
		void        write(const char *p, ssize_t len, std::error_code &ec);
		void        write(const char *p, ssize_t len);
		void        write(std::stringstream &ss, std::error_code &ec);
		void        write(std::stringstream &ss);
		void        onWrite(const std::function<void(const std::error_code &ec)> &handler);
		void        shutdown(const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec);
		void        shutdown(const std::function<void(const std::error_code &ec)> &handler);
		bool        isReadable();
		bool        isWritable();
		void        setBlocking(bool blocking, std::error_code &ec);
		void        setBlocking(bool blocking = true);
	private:
		void        writeNewArrayAndDeleteIt(const char *p, ssize_t len, std::error_code &ec);

    protected:
        using Handle<T>::handle_;

	private:
		std::function<void(const std::error_code &ec)>	listenHandler_	= [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	shutdownHandler_	= [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	writeHandler_	= [](const std::error_code &ec) {};
		std::function<void(char *data, ssize_t len)>	readHandler_	= [](char *data, ssize_t len) {};
		
	};





	template<typename T>
	inline void Stream<T>::listen(const std::function<void(const std::error_code &ec)> &handler, int backlog, std::error_code &ec)
	{
		listenHandler_ = handler;
		auto status = uv_listen(reinterpret_cast<uv_stream_t *>(&handle_), backlog, [](uv_stream_t *st, int status) {
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(st->data);
			stream.listenHandler_(makeErrorCode(status));
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
		auto status = uv_accept(reinterpret_cast<uv_stream_t *>(&handle_),
			reinterpret_cast<uv_stream_t *>(&(client.handle_)));

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
		readHandler_ = handler;
		auto status = uv_read_start(reinterpret_cast<uv_stream_t *>(&handle_),
			[](uv_handle_t* handle, size_t suggested_size, uv_buf_t *buff)
		{
			buff->base = new char[suggested_size];
			buff->len = suggested_size;
		},
			[](uv_stream_t *handle, ssize_t nread, const uv_buf_t *buff)
		{
			std::unique_ptr<char[]> bytes(buff->base);
			auto &stream = *reinterpret_cast<Stream<T> *>(handle->data);

			if (nread < 0) {
				stream.readHandler_(nullptr, nread);
			}				
			else {
				stream.readHandler_(buff->base, nread);
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
		auto status = uv_read_stop(reinterpret_cast<uv_stream_t *>(&handle_));

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
		if (len == 0) return;

		auto sendbuf = new char[len];
        std::memcpy(sendbuf, p, len);
		writeNewArrayAndDeleteIt(sendbuf, len, ec);
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
	inline void Stream<T>::write(std::stringstream &ss, std::error_code &ec)
	{
		auto len = ss.str().length();
		if (len == 0) return;

		auto sendbuf = new char[len];
		ss.read(sendbuf, len);
		writeNewArrayAndDeleteIt(sendbuf, len, ec);
	}

	template<typename T>
	inline void Stream<T>::write(std::stringstream &ss)
	{
		std::error_code ec;

		write(ss, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	template<typename T>
	inline void Stream<T>::onWrite(const std::function<void(const std::error_code &ec)> &handler)
	{
		writeHandler_ = handler;
	}

	template<typename T>
	inline void Stream<T>::shutdown(const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec)
	{
		shutdownHandler_ = handler;

		auto req = new (std::nothrow) Shutdown;
		if (req == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		auto status = uv_shutdown(&req->handle_, reinterpret_cast<uv_stream_t *>(&handle_),
			[](uv_shutdown_t *req, int status) {
			std::unique_ptr<uv::Shutdown> shutdown(reinterpret_cast<Shutdown *>(req->data));
			
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(req->handle->data);
			stream.shutdownHandler_(makeErrorCode(status));
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
		return uv_is_readable(reinterpret_cast<uv_stream_t *>(&handle_));
	}

	template<typename T>
	inline bool Stream<T>::isWritable()
	{
		return uv_is_writable(reinterpret_cast<uv_stream_t *>(&handle_));
	}

	template<typename T>
	inline void Stream<T>::setBlocking(bool blocking, std::error_code &ec)
	{
		auto status = uv_stream_set_blocking(reinterpret_cast<uv_stream_t *>(&handle_), blocking);


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

	template<typename T>
	inline void Stream<T>::writeNewArrayAndDeleteIt(const char * p, ssize_t len, std::error_code & ec)
	{
		auto writeHandler = new (std::nothrow) uv::Write(p, len);
		if (writeHandler == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		auto status = uv_write(&writeHandler->handle_, reinterpret_cast<uv_stream_t *>(&handle_),
			&writeHandler->buf_, 1, [](uv_write_t *req, int status) {
			std::unique_ptr<uv::Write> writeHandler(reinterpret_cast<uv::Write *>(req->data));
			std::unique_ptr<char[]> bytes(writeHandler->buf_.base);

			auto &stream = *reinterpret_cast<Stream<T> *>(req->handle->data);
			stream.writeHandler_(makeErrorCode(status));
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

}


#endif
