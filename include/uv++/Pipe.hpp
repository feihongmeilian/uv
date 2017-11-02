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

		void			open(uv_file file, uv::Error &err);
		void			open(uv_file file);
		void			bind(const std::string &name, uv::Error &err);
		void			bind(const std::string &name);
		void			connect(const std::string &name, std::function<void(const Error &error)> handler, uv::Error &err);
		void			connect(const std::string &name, std::function<void(const Error &error)> handler);
		void			getsockname(char *buffer, size_t &size, uv::Error &err) const;
		void			getsockname(char *buffer, size_t &size) const;
		void			getpeername(char *buffer, size_t &size, uv::Error &err) const;
		void			getpeername(char *buffer, size_t &size) const;
		void			pendingInstances(int count);
		int				pendingCount();

		uv_handle_type	pendingType();

	private:
		std::function<void(const Error &error)>	m_connectHandler = [](const Error &error) {};
	};





	inline Pipe::Pipe(uv::Loop &loop, int ipc)
	{
		m_handle.data = this;
		uv_pipe_init(loop.m_loop_ptr, &m_handle, ipc);
	}

	inline void Pipe::open(uv_file file, uv::Error &err)
	{
		err.m_error = uv_pipe_open(&m_handle, file);
	}

	inline void Pipe::open(uv_file file)
	{
		uv::Error err;

		open(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Pipe::bind(const std::string &name, uv::Error &err)
	{
		err.m_error = uv_pipe_bind(&m_handle, name.c_str());
	}

	inline void Pipe::bind(const std::string &name)
	{
		uv::Error err;

		bind(name, err);
		if (err) {
			throw uv::Exception(err);
		}
	}
	
	inline void Pipe::connect(const std::string &name, std::function<void(const Error &error)> handler, uv::Error &err)
	{
		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr) {
			err.m_error = ENOMEM;
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
		uv::Error err;

		connect(name, handler, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Pipe::getsockname(char *buffer, size_t &size, uv::Error &err) const
	{
		err.m_error = uv_pipe_getsockname(&m_handle, buffer, &size);
	}

	inline void Pipe::getsockname(char *buffer, size_t &size) const
	{
		uv::Error err;

		getsockname(buffer, size, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Pipe::getpeername(char *buffer, size_t &size, uv::Error &err) const
	{
		err.m_error =  uv_pipe_getpeername(&m_handle, buffer, &size);
	}

	inline void Pipe::getpeername(char *buffer, size_t &size) const
	{
		uv::Error err;

		getpeername(buffer, size, err);
		if (err) {
			throw uv::Exception(err);
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
