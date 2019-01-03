#ifndef UV_PIPE_HPP
#define UV_PIPE_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Connect.hpp"

#if ( defined(WIN32) || defined(WIN64) )
#include "Stream.hpp"
#else
#include "FileStreamHandle.hpp"
#endif

namespace uv
{
#if ( defined(WIN32) || defined(WIN64) )
	class Pipe : public Stream<uv_pipe_t>
#else
	class Pipe : public FileStreamHandle<uv_pipe_t>
#endif
	{
	public:
		Pipe();

		void        init(uv::Loop &loop, int ipc, std::error_code &ec);
		void        init(uv::Loop &loop, int ipc);
		void        open(uv_file file, std::error_code &ec);
		void        open(uv_file file);
		void        bind(const std::string &name, std::error_code &ec);
		void        bind(const std::string &name);
		void        connect(const std::string &name, const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec);
		void        connect(const std::string &name, const std::function<void(const std::error_code &ec)> &handler);
		void        getsockname(char *buffer, size_t &size, std::error_code &ecr) const;
		void        getsockname(char *buffer, size_t &size) const;
		void        getpeername(char *buffer, size_t &size, std::error_code &ec) const;
		void        getpeername(char *buffer, size_t &size) const;
		void        pendingInstances(int count);
		int         pendingCount();

		uv_handle_type	pendingType();

	private:
		std::function<void(const std::error_code &ec)>	connectHandler_
		            = [](const std::error_code &ec) {};
	};





	inline Pipe::Pipe()
	{
		handle_.data = this;
	}

	inline void Pipe::init(uv::Loop &loop, int ipc, std::error_code &ec)
	{
		auto status = uv_pipe_init(loop.value(), &handle_, ipc);

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
		auto status = uv_pipe_open(&handle_, file);

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
		auto status = uv_pipe_bind(&handle_, name.c_str());

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

		connectHandler_ = handler;
		uv_pipe_connect(&connect->handle_, &handle_, name.c_str(),
			[](uv_connect_t *req, int status) {

			std::shared_ptr<Connect> connect(reinterpret_cast<Connect *>(req->data));

			auto &pipe = *reinterpret_cast<uv::Pipe *>(req->handle->data);
			pipe.connectHandler_(makeErrorCode(status));
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
		auto status = uv_pipe_getsockname(&handle_, buffer, &size);

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
		auto status =  uv_pipe_getpeername(&handle_, buffer, &size);

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
		uv_pipe_pending_instances(&handle_, count);
	}

	inline int Pipe::pendingCount()
	{
		return uv_pipe_pending_count(&handle_);
	}

	inline uv_handle_type Pipe::pendingType()
	{
		return uv_pipe_pending_type(&handle_);
	}
}


#endif
