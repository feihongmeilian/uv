#ifndef UV_TCP_HPP
#define UV_TCP_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Connect.hpp"
#include "FileStreamHandle.hpp"

namespace uv
{
	class Tcp : public FileStreamHandle<uv_tcp_t>
	{
	public:
		Tcp();

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        init(uv::Loop &loop, unsigned int flags, std::error_code &ec);
		void        init(uv::Loop &loop, unsigned int flags);

		void        open(uv_os_sock_t sock, std::error_code &ec);
		void        open(uv_os_sock_t sock);
		void        nodelay(int enable, std::error_code &ec);
		void        nodelay(int enable);
		void        keepalive(int enable, unsigned int delay, std::error_code &ec);
		void        keepalive(int enable, unsigned int delay);
		void        simultaneousAccepts(int enable, std::error_code &ec);
		void        simultaneousAccepts(int enable);
		void        bind(const std::string &ip, int port, unsigned int flags, std::error_code &ec);
		void        bind(const std::string &ip, int port, unsigned int flags);
		void        getsockname(struct sockaddr &name, int &namelen, std::error_code &ec) const;
		void        getsockname(struct sockaddr &name, int &namelenr) const;
		void        getpeername(struct sockaddr &name, int &namelen, std::error_code &ec) const;
		void        getpeername(struct sockaddr &name, int &namelen) const;
		void        connect(const std::string &ip, int port,
						const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec);
		void        connect(const std::string &ip, int port,
						const std::function<void(const std::error_code &ec)> &handler);

	private:
		std::function<void(const std::error_code &ec)>	connectHandler_ = [](const std::error_code &ec) {};
	};





	inline Tcp::Tcp()
	{
		handle_.data = this;
	}

	inline void Tcp::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_tcp_init(loop.value(), &handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Tcp::init(uv::Loop &loop, unsigned int flags, std::error_code &ec)
	{
		auto status = uv_tcp_init_ex(loop.value(), &handle_, flags);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::init(uv::Loop &loop, unsigned int flags)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Tcp::open(uv_os_sock_t sock, std::error_code &ec)
	{
		auto status = uv_tcp_open(&handle_, sock);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::open(uv_os_sock_t sock)
	{
		std::error_code ec;

		open(sock, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Tcp::nodelay(int enable, std::error_code &ec)
	{
		auto status = uv_tcp_nodelay(&handle_, enable);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::nodelay(int enable)
	{
		std::error_code ec;

		nodelay(enable, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Tcp::keepalive(int enable, unsigned int delay, std::error_code &ec)
	{
		auto status = uv_tcp_keepalive(&handle_, enable, delay);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::keepalive(int enable, unsigned int delay)
	{
		std::error_code ec;

		keepalive(enable, delay, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Tcp::simultaneousAccepts(int enable, std::error_code &ec)
	{
		auto status = uv_tcp_simultaneous_accepts(&handle_, enable);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::simultaneousAccepts(int enable)
	{
		std::error_code ec;

		simultaneousAccepts(enable, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
	
	inline void Tcp::bind(const std::string &ip, int port, unsigned int flags, std::error_code &ec)
	{
		struct sockaddr *p_addr;

		struct sockaddr_in addr4;	
		auto status = uv_ip4_addr(ip.c_str(), port, &addr4);
		if (status) {
			ec = makeErrorCode(status);
			struct sockaddr_in6 addr6;
			auto status = uv_ip6_addr(ip.c_str(), port, &addr6);
			if (status) {
				return;
				
			}
			p_addr = reinterpret_cast<sockaddr *>(&addr6);
		}
		else {
			p_addr = reinterpret_cast<sockaddr *>(&addr4);
		}

		status = uv_tcp_bind(&handle_, p_addr, flags);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::bind(const std::string &ip, int port, unsigned int flags)
	{
		std::error_code ec;

		bind(ip, port, flags, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
	
	inline void Tcp::getsockname(struct sockaddr &name, int &namelen, std::error_code &ec) const
	{
		auto status = uv_tcp_getsockname(&handle_, &name, &namelen);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::getsockname(sockaddr &name, int &namelen) const
	{
		std::error_code ec;

		getsockname(name, namelen, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
	
	inline void Tcp::getpeername(struct sockaddr &name, int &namelen, std::error_code &ec) const
	{
		auto status = uv_tcp_getpeername(&handle_, &name, &namelen);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::getpeername(sockaddr &name, int &namelen) const
	{
		std::error_code ec;

		getpeername(name, namelen, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
	
	inline void Tcp::connect(const std::string &ip, int port,
		const std::function<void(const std::error_code &ec)> &handler, std::error_code &ec)
	{
		connectHandler_ = handler;

		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		struct sockaddr *p_addr;

		struct sockaddr_in addr4;
		auto status = uv_ip4_addr(ip.c_str(), port, &addr4);
		if (status != 0) {
			ec = makeErrorCode(status);
			struct sockaddr_in6 addr6;
			auto status = uv_ip6_addr(ip.c_str(), port, &addr6);
			if (status) {
				return;

			}
			p_addr = reinterpret_cast<sockaddr *>(&addr6);
		}
		else {
			p_addr = reinterpret_cast<sockaddr *>(&addr4);
		}

		status = uv_tcp_connect(&connect->handle_, &handle_, p_addr,
			[](uv_connect_t *req, int status) {
            std::unique_ptr<Connect> connect(reinterpret_cast<Connect *>(req->data));
			
            auto &tcp = *reinterpret_cast<uv::Tcp *>(req->handle->data);
			tcp.connectHandler_(makeErrorCode(status));
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tcp::connect(const std::string &ip, int port,
		const std::function<void(const std::error_code &ec)> &handler)
	{
		std::error_code ec;

		connect(ip, port, handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

}


#endif
