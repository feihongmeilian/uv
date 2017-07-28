#ifndef UV_TCP_HPP
#define UV_TCP_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Loop.hpp"
#include "Connect.hpp"
#include "Stream.hpp"

namespace uv
{
	class Tcp : public Stream<uv_tcp_t>
	{
	public:
		inline explicit	Tcp(uv::Loop &loop);
		inline			Tcp(uv::Loop &loop, unsigned int flags);

		inline int		open(uv_os_sock_t sock);
		inline int		nodelay(int enable);
		inline int		keepalive(int enable, unsigned int delay);
		inline int		simultaneousAccepts(int enable);
		inline int		bind(const std::string &ip, int port, unsigned int flags);
		inline int		getsockname(struct sockaddr &name, int &namelen) const;
		inline int		getpeername(struct sockaddr &name, int &namelen) const;
		inline int		connect(const std::string &ip, int port,
							std::function<void(const Error &error)> handler);

	private:
		std::function<void(const Error &error)>	m_connectHandler = [](const Error &error) {};
	};





	Tcp::Tcp(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_tcp_init(loop.m_loop_ptr, &m_handle);
	}

	Tcp::Tcp(uv::Loop &loop, unsigned int flags)
	{
		m_handle.data = this;
		uv_tcp_init_ex(loop.m_loop_ptr, &m_handle, flags);
	}

	int Tcp::open(uv_os_sock_t sock)
	{
		return uv_tcp_open(&m_handle, sock);
	}

	int Tcp::nodelay(int enable)
	{
		return uv_tcp_nodelay(&m_handle, enable);
	}

	int Tcp::keepalive(int enable, unsigned int delay)
	{
		return uv_tcp_keepalive(&m_handle, enable, delay);
	}

	int Tcp::simultaneousAccepts(int enable)
	{
		return uv_tcp_simultaneous_accepts(&m_handle, enable);
	}
	
	int Tcp::bind(const std::string &ip, int port, unsigned int flags)
	{
		struct sockaddr_in addr;
		auto status = uv_ip4_addr(ip.c_str(), port, &addr);
		if (status) return status;

		return uv_tcp_bind(&m_handle, reinterpret_cast<sockaddr *>(&addr), flags);
	}
	
	int Tcp::getsockname(struct sockaddr &name, int &namelen) const
	{
		return uv_tcp_getsockname(&m_handle, &name, &namelen);
	}
	
	int Tcp::getpeername(struct sockaddr &name, int &namelen) const
	{
		return uv_tcp_getpeername(&m_handle, &name, &namelen);
	}
	
	int Tcp::connect(const std::string &ip, int port, std::function<void(const Error &error)> handler)
	{
		m_connectHandler = handler;

		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr) return ENOMEM;

		struct sockaddr_in addr;
		auto status = uv_ip4_addr(ip.c_str(), port, &addr);
		if (status) return status;
		return uv_tcp_connect(&connect->m_handle, &m_handle, reinterpret_cast<sockaddr *>(&addr),
			[](uv_connect_t *req, int status) {

            std::shared_ptr<Connect> connect(reinterpret_cast<Connect *>(req->data));
			
            auto &tcp = *reinterpret_cast<uv::Tcp *>(req->handle->data);
			tcp.m_connectHandler(Error(status));
		});
	}
}


#endif
