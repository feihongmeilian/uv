#ifndef UV_FILE_STREAM_EVENT_HPP
#define UV_FILE_STREAM_EVENT_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class FileStreamEvent : public Noncopyable
	{
	public:
		inline explicit	FileStreamEvent(uv::Loop &loop);

		inline int		start(const std::string &path, unsigned int flags,
							std::function<void(const std::string &filename, int events, const Error &error)> handler);
		inline int		stop();
		inline int		getpath(char *buffer, size_t &size);

	private:
		uv_fs_event_t		m_handle;
		std::function<void(const std::string &filename, int events, const Error &error)>	m_callbackHandler
			= [](const std::string &filename, int events, const Error &error) {};
	};





	FileStreamEvent::FileStreamEvent(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_fs_event_init(loop.m_loop_ptr, &m_handle);
		m_callbackHandler = []( const std::string &filename, int events, int status) {};
	}

	int FileStreamEvent::start(const std::string &path, unsigned int flags,
		std::function<void(const std::string &filename, int events, const Error &error)> handler)
	{
		m_callbackHandler = handler;
		return uv_fs_event_start(&m_handle,
			[](uv_fs_event_t* handle, const char *filename, int events, int status) {
			auto &fe = *reinterpret_cast<uv::FileStreamEvent *>(handle->data);
			fe.m_callbackHandler(filename, events, Error(status));
		}, path.c_str(), flags);
	}

	int FileStreamEvent::stop()
	{
		return uv_fs_event_stop(&m_handle);
	}

	int FileStreamEvent::getpath(char *buffer, size_t &size)
	{
		return uv_fs_event_getpath(&m_handle, buffer, &size);
	}
}


#endif
