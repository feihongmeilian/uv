#ifndef UV_PIPE_HPP
#define UV_PIPE_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Loop.hpp"
#include "Connect.hpp"
#include "Stream.hpp"

namespace uv
{
	class Pipe : public Stream<uv_pipe_t>
	{
	public:
		inline			Pipe(uv::Loop &loop, int ipc);

		inline int		open(uv_file file);
		inline int		bind(const std::string &name);
		inline int		connect(const std::string &name, std::function<void(const Error &error)> handler);
		inline int		getsockname(char *buffer, size_t &size) const;
		inline int		getpeername(char *buffer, size_t &size) const;
		inline void		pendingInstances(int count);
		inline int		pendingCount();

		inline uv_handle_type	pendingType();

	private:
		std::function<void(const Error &error)>		m_connectHandler = [](const Error &error) {};
	};





	Pipe::Pipe(uv::Loop &loop, int ipc)
	{
		m_handle.data = this;
		uv_pipe_init(loop.m_loop_ptr, &m_handle, ipc);
	}

	int Pipe::open(uv_file file)
	{
		return uv_pipe_open(&m_handle, file);
	}

	int Pipe::bind(const std::string &name)
	{
		return uv_pipe_bind(&m_handle, name.c_str());
	}
	
	int Pipe::connect(const std::string &name, std::function<void(const Error &error)> handler)
	{
		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr) return ENOMEM;

		m_connectHandler = handler;
		uv_pipe_connect(&connect->m_handle, &m_handle, name.c_str(),
			[](uv_connect_t *req, int status) {

			std::shared_ptr<Connect> connect(reinterpret_cast<Connect *>(req->data));

			auto &pipe = *reinterpret_cast<uv::Pipe *>(req->handle->data);
			pipe.m_connectHandler(Error(status));
		});

		return 0;
	}

	int Pipe::getsockname(char *buffer, size_t &size) const
	{
		return uv_pipe_getsockname(&m_handle, buffer, &size);
	}

	int Pipe::getpeername(char *buffer, size_t &size) const
	{
		return uv_pipe_getpeername(&m_handle, buffer, &size);
	}

	void Pipe::pendingInstances(int count)
	{
		uv_pipe_pending_instances(&m_handle, count);
	}

	int Pipe::pendingCount()
	{
		return uv_pipe_pending_count(&m_handle);
	}

	uv_handle_type Pipe::pendingType()
	{
		return uv_pipe_pending_type(&m_handle);
	}
}


#endif
