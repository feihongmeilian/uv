#ifndef UV_FILE_STREAM_EVENT_HPP
#define UV_FILE_STREAM_EVENT_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	class FileStreamEvent : public Noncopyable
	{
	public:
		explicit		FileStreamEvent(uv::Loop &loop);

		void			start(const std::string &path, unsigned int flags,
						std::function<void(const std::string &filename, int events, const Error &error)> handler, uv::Error &err);
		void			start(const std::string &path, unsigned int flags,
						std::function<void(const std::string &filename, int events, const Error &error)> handler);
		void			stop(uv::Error &err);
		void			stop();
		void			getpath(char *buffer, size_t &size, uv::Error &err);
		void			getpath(char *buffer, size_t &size);

	private:
		uv_fs_event_t	m_handle;
		std::function<void(const std::string &filename, int events, const Error &error)>	m_callbackHandler
			= [](const std::string &filename, int events, const Error &error) {};
	};





	inline FileStreamEvent::FileStreamEvent(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_fs_event_init(loop.m_loop_ptr, &m_handle);
	}

	inline void FileStreamEvent::start(const std::string &path, unsigned int flags,
		std::function<void(const std::string &filename, int events, const Error &error)> handler, uv::Error &err)
	{
		m_callbackHandler = handler;
		err.m_error = uv_fs_event_start(&m_handle,
			[](uv_fs_event_t* handle, const char *filename, int events, int status) {
			auto &fe = *reinterpret_cast<uv::FileStreamEvent *>(handle->data);
			fe.m_callbackHandler(filename, events, Error(status));
		}, path.c_str(), flags);
	}

	inline void FileStreamEvent::start(const std::string &path, unsigned int flags,
		std::function<void(const std::string&filename, int events, const Error&error)> handler)
	{
		uv::Error err;

		start(path, flags, handler, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void FileStreamEvent::stop(uv::Error &err)
	{
		err.m_error = uv_fs_event_stop(&m_handle);
	}

	inline void FileStreamEvent::stop()
	{
		uv::Error err;

		stop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void FileStreamEvent::getpath(char *buffer, size_t &size, uv::Error &err)
	{
		err.m_error = uv_fs_event_getpath(&m_handle, buffer, &size);
	}

	inline void FileStreamEvent::getpath(char *buffer, size_t &size)
	{
		uv::Error err;

		getpath(buffer, size, err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
