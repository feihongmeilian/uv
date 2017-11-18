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
		Pipe();

		void			init(uv::Loop &loop, int ipc, std::error_code &ec);
		void			init(uv::Loop &loop, int ipc);
		void			open(uv_file file, std::error_code &ec);
		void			open(uv_file file);
		void			bind(const std::string &name, std::error_code &ec);
		void			bind(const std::string &name);
		void			connect(const std::string &name, const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec);
		void			connect(const std::string &name, const std::function<void(const std::error_code &ec)> &handler);
		void			getsockname(char *buffer, size_t &size, std::error_code &ecr) const;
		void			getsockname(char *buffer, size_t &size) const;
		void			getpeername(char *buffer, size_t &size, std::error_code &ec) const;
		void			getpeername(char *buffer, size_t &size) const;
		void			pendingInstances(int count);
		int				pendingCount();

		uv_handle_type	pendingType();

	private:
		std::function<void(const std::error_code &ec)>	m_connectHandler = [](const std::error_code &ec) {};
	};





	inline Pipe::Pipe()
	{
		m_handle.data = this;
	}

	inline void Pipe::init(uv::Loop &loop, int ipc, std::error_code &ec)
	{
		auto status = uv_pipe_init(loop.value(), &m_handle, ipc);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Pipe::init(uv::Loop &loop, int ipc)
	{
		std::error_code ec;

		init(loop, ipc, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Pipe::open(uv_file file, std::error_code &ec)
	{
		auto status = uv_pipe_open(&m_handle, file);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Pipe::open(uv_file file)
	{
		std::error_code ec;

		open(file, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Pipe::bind(const std::string &name, std::error_code &ec)
	{
		auto status = uv_pipe_bind(&m_handle, name.c_str());

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Pipe::bind(const std::string &name)
	{
		std::error_code ec;

		bind(name, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
	
	inline void Pipe::connect(const std::string &name, const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec)
	{
		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		m_connectHandler = handler;
		uv_pipe_connect(&connect->m_handle, &m_handle, name.c_str(),
			[](uv_connect_t *req, int status) {

			std::shared_ptr<Connect> connect(reinterpret_cast<Connect *>(req->data));

			auto &pipe = *reinterpret_cast<uv::Pipe *>(req->handle->data);
			pipe.m_connectHandler(makeErrorCode(status));
		});
	}

	inline void Pipe::connect(const std::string &name, const std::function<void(const std::error_code &ec)> &handler)
	{
		std::error_code ec;

		connect(name, handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Pipe::getsockname(char *buffer, size_t &size, std::error_code &ec) const
	{
		auto status = uv_pipe_getsockname(&m_handle, buffer, &size);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Pipe::getsockname(char *buffer, size_t &size) const
	{
		std::error_code ec;

		getsockname(buffer, size, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Pipe::getpeername(char *buffer, size_t &size, std::error_code &ec) const
	{
		auto status =  uv_pipe_getpeername(&m_handle, buffer, &size);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Pipe::getpeername(char *buffer, size_t &size) const
	{
		std::error_code ec;

		getpeername(buffer, size, ec);
		if (ec) {
			throw uv::Exception(ec);
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
