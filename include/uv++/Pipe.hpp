#ifndef UV_PIPE_HPP
#define UV_PIPE_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Connect.hpp"
#include "Stream.hpp"

namespace uv
{
	class Pipe : public Stream<uv_pipe_t>
	{
	public:
					Pipe(uv::Loop &loop, int ipc);

		void			open(uv_file file, uv::Error &er);
		void			open(uv_file file);
		void			bind(const std::string &name, uv::Error &er);
		void			bind(const std::string &name);
		void			connect(const std::string &name, std::function<void(const Error &error)> handler, uv::Error &er);
		void			connect(const std::string &name, std::function<void(const Error &error)> handler);
		void			getsockname(char *buffer, size_t &size, uv::Error &er) const;
		void			getsockname(char *buffer, size_t &size) const;
		void			getpeername(char *buffer, size_t &size, uv::Error &er) const;
		void			getpeername(char *buffer, size_t &size) const;
		void			pendingInstances(int count);
		int			pendingCount();

		uv_handle_type	pendingType();

	private:
		std::function<void(const Error &error)>	m_connectHandler = [](const Error &error) {};
	};





	inline Pipe::Pipe(uv::Loop &loop, int ipc)
	{
		m_handle.data = this;
		uv_pipe_init(loop.m_loop_ptr, &m_handle, ipc);
	}

	inline void Pipe::open(uv_file file, uv::Error &er)
	{
		er.m_error = uv_pipe_open(&m_handle, file);
	}

	inline void Pipe::open(uv_file file)
	{
		uv::Error er;

		open(er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Pipe::bind(const std::string &name, uv::Error &er)
	{
		er.m_error = uv_pipe_bind(&m_handle, name.c_str());
	}

	inline void Pipe::bind(const std::string &name)
	{
		uv::Error er;

		bind(name, er);
		if (er) {
			throw uv::Exception(er);
		}
	}
	
	inline void Pipe::connect(const std::string &name, std::function<void(const Error &error)> handler, uv::Error &er)
	{
		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr) {
			er.m_error = ENOMEM;
			return;
		}

		m_connectHandler = handler;
		uv_pipe_connect(&connect->m_handle, &m_handle, name.c_str(),
			[](uv_connect_t *req, int status) {

			std::shared_ptr<Connect> connect(reinterpret_cast<Connect *>(req->data));

			auto &pipe = *reinterpret_cast<uv::Pipe *>(req->handle->data);
			pipe.m_connectHandler(Error(status));
		});
	}

	inline void Pipe::connect(const std::string &name, std::function<void(const Error&error)> handler)
	{
		uv::Error er;

		connect(name, handler, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Pipe::getsockname(char *buffer, size_t &size, uv::Error &er) const
	{
		er.m_error = uv_pipe_getsockname(&m_handle, buffer, &size);
	}

	inline void Pipe::getsockname(char *buffer, size_t &size) const
	{
		uv::Error er;

		getsockname(buffer, size, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Pipe::getpeername(char *buffer, size_t &size, uv::Error &er) const
	{
		er.m_error =  uv_pipe_getpeername(&m_handle, buffer, &size);
	}

	inline void Pipe::getpeername(char *buffer, size_t &size) const
	{
		uv::Error er;

		getpeername(buffer, size, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Pipe::pendingInstances(int count)
	{
		uv_pipe_pending_instances(&m_handle, count);
	}

	inline int Pipe::pendingCount()
	{
		return uv_pipe_pending_count(&m_handle);
	}

	inline uv_handle_type Pipe::pendingType()
	{
		return uv_pipe_pending_type(&m_handle);
	}
}


#endif
