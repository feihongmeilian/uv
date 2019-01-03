#ifndef UV_POLL_HPP
#define UV_POLL_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "FileHandle.hpp"

namespace uv
{
	class Poll : public FileHandle<uv_poll_t>
	{
	public:
		Poll();

		void        init(uv::Loop &loop, int fd, std::error_code &ec);
		void        init(uv::Loop &loop, int fd);
		void        initSocket(uv::Loop &loop, uv_os_sock_t socket, std::error_code &ec);
		void        initSocket(uv::Loop &loop, uv_os_sock_t socket);
		void        start(std::error_code &ec);
		void        start();
		void        stop(std::error_code &ec);
		void        stop();

		void        onReadable(const std::function<void(const std::error_code &ec)> &handler);
		void        onWritable(const std::function<void(const std::error_code &ec)> &handler);
		void        onDisconnect(const std::function<void(const std::error_code &ec)> &handler);
		void        onPrioritized(const std::function<void(const std::error_code &ec)> &handler);

	private:
		int         events_ = 0;
		std::function<void(const std::error_code &ec)>	readableHandler_ = [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	writableHandler_ = [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	disconnectHandler_ = [](const std::error_code &ec) {};
		std::function<void(const std::error_code &ec)>	prioritizedHandler_ = [](const std::error_code &ec) {};
	};





	inline Poll::Poll()
	{
		handle_.data = this;
	}
	
	inline void Poll::init(uv::Loop &loop, int fd, std::error_code &ec)
	{
		auto status = uv_poll_init(loop.value(), &handle_, fd);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::init(uv::Loop &loop, int fd)
	{
		std::error_code ec;

		init(loop, fd, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::initSocket(uv::Loop &loop, uv_os_sock_t socket, std::error_code &ec)
	{
		auto status = uv_poll_init_socket(loop.value(), &handle_, socket);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::initSocket(uv::Loop &loop, uv_os_sock_t socket)
	{
		std::error_code ec;

		initSocket(loop, socket, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::start(std::error_code &ec)
	{
		auto status = uv_poll_start(&handle_, events_, [](uv_poll_t* handle, int status, int events) {
			auto &poll = *reinterpret_cast<uv::Poll *>(handle->data);
			if (events & UV_READABLE) {
				poll.readableHandler_(makeErrorCode(status));
			}
			if (events & UV_WRITABLE) {
				poll.writableHandler_(makeErrorCode(status));
			}
			if (events & UV_DISCONNECT) {
				poll.disconnectHandler_(makeErrorCode(status));
			}
			if (events & UV_PRIORITIZED) {
				poll.prioritizedHandler_(makeErrorCode(status));
			}
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::start()
	{
		std::error_code ec;

		start(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::stop(std::error_code &ec)
	{
		auto status = uv_poll_stop(&handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Poll::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Poll::onReadable(const std::function<void(const std::error_code &ec)> &handler)
	{
		events_ += UV_READABLE;
		readableHandler_ = handler;
	}

	inline void Poll::onWritable(const std::function<void(const std::error_code &ec)> &handler)
	{
		events_ += UV_WRITABLE;
		writableHandler_ = handler;
	}

	inline void Poll::onDisconnect(const std::function<void(const std::error_code &ec)> &handler)
	{
		events_ += UV_DISCONNECT;
		disconnectHandler_ = handler;
	}

	inline void Poll::onPrioritized(const std::function<void(const std::error_code &ec)> &handler)
	{
		events_ += UV_PRIORITIZED;
		prioritizedHandler_ = handler;
	}
}


#endif
