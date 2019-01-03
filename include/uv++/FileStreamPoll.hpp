#ifndef UV_FILE_STREAM_POLL_HPP
#define UV_FILE_STREAM_POLL_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Handle.hpp"

namespace uv
{
	class FileStreamPoll : public Handle<uv_fs_poll_t>
	{
	public:
		FileStreamPoll();

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        start(const std::string &path, unsigned int interval, 
						const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec);
		void        start(const std::string &path, unsigned int interval,
						const std::function<void(const std::error_code &ec)> &handler);
		void        stop(std::error_code &ec);
		void        stop();
		void        getpath(char *buffer, size_t &size, std::error_code &ec);
		void        getpath(char *buffer, size_t &size);

		//just use in callback function
		const uv_stat_t	*getPrevStat() const;
		const uv_stat_t	*getCurrStat() const;

	private:
		const uv_stat_t	*prevStat_;
		const uv_stat_t	*currStat_;
		std::function<void(const std::error_code &ec)>	callbackHandler_
	                = [](const std::error_code &ec) {};
	};





	inline FileStreamPoll::FileStreamPoll()
	{
		handle_.data = this;
	}

	inline void FileStreamPoll::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_fs_poll_init(loop.value(), &handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamPoll::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void FileStreamPoll::start(const std::string &path, unsigned int interval,
		const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec)
	{
		callbackHandler_ = handler;
		auto status = uv_fs_poll_start(&handle_,
			[](uv_fs_poll_t* handle, int status, const uv_stat_t *prev, const uv_stat_t *curr) {
			auto &fp = *reinterpret_cast<uv::FileStreamPoll *>(handle->data);
			fp.prevStat_ = prev;
			fp.currStat_ = curr;
			fp.callbackHandler_(makeErrorCode(status));
		}, path.c_str(), interval);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamPoll::start(const std::string &path, unsigned int interval,
		const std::function<void(const std::error_code &ec)> &handler)
	{
		std::error_code ec;

		start(path, interval, handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void FileStreamPoll::stop(std::error_code &ec)
	{
		auto status = uv_fs_poll_stop(&handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamPoll::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void FileStreamPoll::getpath(char *buffer, size_t &size, std::error_code &ec)
	{
		auto status = uv_fs_poll_getpath(&handle_, buffer, &size);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void FileStreamPoll::getpath(char *buffer, size_t &size)
	{
		std::error_code ec;

		getpath(buffer, size, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline const uv_stat_t *FileStreamPoll::getPrevStat() const
	{
		return prevStat_;
	}

	inline const uv_stat_t *FileStreamPoll::getCurrStat() const
	{
		return currStat_;
	}
}


#endif
