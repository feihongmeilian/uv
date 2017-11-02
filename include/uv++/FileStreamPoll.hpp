#ifndef UV_FILE_STREAM_POLL_HPP
#define UV_FILE_STREAM_POLL_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class FileStreamPoll : public Noncopyable
	{
	public:
		explicit		FileStreamPoll(uv::Loop &loop);

		void			start(const std::string &path, unsigned int interval, 
						std::function<void(const Error &error)> handler, uv::Error &err);
		void			start(const std::string &path, unsigned int interval,
						std::function<void(const Error &error)> handler);
		void			stop(uv::Error &err);
		void			stop();
		void			getpath(char *buffer, size_t &size, uv::Error &err);
		void			getpath(char *buffer, size_t &size);

		//just use in callback function
		const uv_stat_t	*getPrevStat() const;
		const uv_stat_t	*getCurrStat() const;
	private:
		uv_fs_poll_t	m_handle;
		const uv_stat_t	*m_prevStat;
		const uv_stat_t	*m_currStat;
		std::function<void(const Error &error)>	m_callbackHandler = [](const Error &error) {};
	};





	inline FileStreamPoll::FileStreamPoll(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_fs_poll_init(loop.m_loop_ptr, &m_handle);
	}

	inline void FileStreamPoll::start(const std::string &path, unsigned int interval,
		std::function<void(const Error &error)> handler, uv::Error &err)
	{
		m_callbackHandler = handler;
		err.m_error = uv_fs_poll_start(&m_handle,
			[](uv_fs_poll_t* handle, int status, const uv_stat_t *prev, const uv_stat_t *curr) {
			auto &fp = *reinterpret_cast<uv::FileStreamPoll *>(handle->data);
			fp.m_prevStat = prev;
			fp.m_currStat = curr;
			fp.m_callbackHandler(Error(status));
		}, path.c_str(), interval);
	}

	inline void FileStreamPoll::start(const std::string &path, unsigned int interval,
		std::function<void(const Error&error)> handler)
	{
		uv::Error err;

		start(path, interval, handler, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void FileStreamPoll::stop(uv::Error &err)
	{
		err.m_error = uv_fs_poll_stop(&m_handle);
	}

	inline void FileStreamPoll::stop()
	{
		uv::Error err;

		stop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void FileStreamPoll::getpath(char *buffer, size_t &size, uv::Error &err)
	{
		err.m_error = uv_fs_poll_getpath(&m_handle, buffer, &size);
	}

	inline void FileStreamPoll::getpath(char *buffer, size_t &size)
	{
		uv::Error err;

		getpath(buffer, size, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline const uv_stat_t *FileStreamPoll::getPrevStat() const
	{
		return m_prevStat;
	}

	inline const uv_stat_t *FileStreamPoll::getCurrStat() const
	{
		return m_currStat;
	}
}


#endif
