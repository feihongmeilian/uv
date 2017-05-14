#ifndef UV_FILE_STREAM_POLL_HPP
#define UV_FILE_STREAM_POLL_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class FileStreamPoll : public Noncopyable
	{
	public:
		inline explicit	FileStreamPoll(uv::Loop &loop);

		inline int		start(const std::string &path, unsigned int interval, std::function<void(uv::FileStreamPoll &, int status)> handler);
		inline int		stop();
		inline int		getpath(char *buffer, size_t &size);

		//just use in callback function
		inline const uv_stat_t		*getPrevStat() const;
		inline const uv_stat_t		*getCurrStat() const;
	private:
		uv_fs_poll_t		m_handle;
		const uv_stat_t	*m_prevStat;
		const uv_stat_t	*m_currStat;
		std::function<void(uv::FileStreamPoll &, int status)>	m_callbackHandler = nullptr;
	};





	FileStreamPoll::FileStreamPoll(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_fs_poll_init(&loop.m_loop, &m_handle);
	}

	int FileStreamPoll::start(const std::string &path, unsigned int interval, std::function<void(uv::FileStreamPoll &, int status)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_poll_start(&m_handle,
			[](uv_fs_poll_t* handle, int status, const uv_stat_t *prev, const uv_stat_t *curr) {
			auto &fp = *reinterpret_cast<uv::FileStreamPoll *>(handle->data);
			fp.m_prevStat = prev;
			fp.m_currStat = curr;
			fp.m_callbackHandler(fp, status);
		}, path.c_str(), interval);
	}

	int FileStreamPoll::stop()
	{
		return uv_fs_poll_stop(&m_handle);
	}

	int FileStreamPoll::getpath(char *buffer, size_t &size)
	{
		return uv_fs_poll_getpath(&m_handle, buffer, &size);
	}

	const uv_stat_t *FileStreamPoll::getPrevStat() const
	{
		return m_prevStat;
	}

	const uv_stat_t *FileStreamPoll::getCurrStat() const
	{
		return m_currStat;
	}
}


#endif
