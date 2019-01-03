#ifndef UV_FILE_STREAM_EVENT_HPP
#define UV_FILE_STREAM_EVENT_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Handle.hpp"

namespace uv
{
	class FileStreamEvent : public Handle<uv_fs_event_t>
	{
	public:
		FileStreamEvent();

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        start(const std::string &path, unsigned int flags,
						const std::function<void(const std::string &filename, int events, const std::error_code &ec)> &handler, std::error_code &ec);
		void        start(const std::string &path, unsigned int flags,
						const std::function<void(const std::string &filename, int events, const std::error_code &ec)> &handler);
		void        stop(std::error_code &ec);
		void        stop();
		void        getpath(char *buffer, size_t &size, std::error_code &ecr);
		void        getpath(char *buffer, size_t &size);

	private:
		std::function<void(const std::string &filename, int events, const std::error_code &ec)>	callbackHandler_
			= [](const std::string &filename, int events, const std::error_code &ec) {};
	};





	inline FileStreamEvent::FileStreamEvent()
	{
		handle_.data = this;
	}

	inline void FileStreamEvent::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_fs_event_init(loop.value(), &handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamEvent::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void FileStreamEvent::start(const std::string &path, unsigned int flags,
		const std::function<void(const std::string &filename, int events, const std::error_code &ec)> &handler, std::error_code &ec)
	{
		callbackHandler_ = handler;
		auto status = uv_fs_event_start(&handle_,
			[](uv_fs_event_t* handle, const char *filename, int events, int status) {
			auto &fe = *reinterpret_cast<uv::FileStreamEvent *>(handle->data);
			fe.callbackHandler_(filename, events, makeErrorCode(status));
		}, path.c_str(), flags);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamEvent::start(const std::string &path, unsigned int flags,
		const std::function<void(const std::string&filename, int events, const std::error_code &ec)> &handler)
	{
		std::error_code ec;

		start(path, flags, handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void FileStreamEvent::stop(std::error_code &ec)
	{
		auto status = uv_fs_event_stop(&handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamEvent::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void FileStreamEvent::getpath(char *buffer, size_t &size, std::error_code &ec)
	{
		auto status = uv_fs_event_getpath(&handle_, buffer, &size);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamEvent::getpath(char *buffer, size_t &size)
	{
		std::error_code ec;

		getpath(buffer, size, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
