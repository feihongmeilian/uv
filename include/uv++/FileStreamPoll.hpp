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
						std::function<void(const Error &error)> handler, uv::Error &er);
		void			start(const std::string &path, unsigned int interval,
						std::function<void(const Error &error)> handler);
		void			stop(uv::Error &er);
		void			stop();
		void			getpath(char *buffer, size_t &size, uv::Error &er);
		void			getpath(char *buffer, size_t &size);

		//just use in callback function
		const uv_stat_t	*getPrevStat() const;
		const uv_stat_t	*getCurrStat() const;
	private:
		uv_fs_poll_t		m_handle;
		const uv_stat_t	*m_prevStat;
		const uv_stat_t	*m_currStat;
		std::function<void(const Error &error)>	m_callbackHandler = [](const Error &error) {};
	};





	inline FileStreamPoll::FileStreamPoll(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_fs_poll_init(loop.m_loop_ptr, &m_handle);
		m_callbackHandler = [](int status) {};
	}

	inline void FileStreamPoll::start(const std::string &path, unsigned int interval,
		std::function<void(const Error &error)> handler, uv::Error &er)
	{
		m_callbackHandler = handler;
		er.m_error = uv_fs_poll_start(&m_handle,
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
		uv::Error er;
		start(path, interval, handler, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}

	inline void FileStreamPoll::stop(uv::Error &er)
	{
		er.m_error = uv_fs_poll_stop(&m_handle);
	}

	inline void FileStreamPoll::stop()
	{
		uv::Error er;
		stop(er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}

	inline void FileStreamPoll::getpath(char *buffer, size_t &size, uv::Error &er)
	{
		er.m_error = uv_fs_poll_getpath(&m_handle, buffer, &size);
	}

	inline void FileStreamPoll::getpath(char *buffer, size_t &size)
	{
		uv::Error er;
		getpath(buffer, size, er);

		if (er)
		{
			throw uv::Exception(er);
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
