#ifndef UV_TCP_HPP
#define UV_TCP_HPP

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
	class Tcp : public Stream<uv_tcp_t>
	{
	public:
		explicit		Tcp(uv::Loop &loop);
					Tcp(uv::Loop &loop, unsigned int flags);

		void			open(uv_os_sock_t sock, uv::Error &er);
		void			open(uv_os_sock_t sock);
		void			nodelay(int enable, uv::Error &er);
		void			nodelay(int enable);
		void			keepalive(int enable, unsigned int delay, uv::Error &er);
		void			keepalive(int enable, unsigned int delay);
		void			simultaneousAccepts(int enable, uv::Error &er);
		void			simultaneousAccepts(int enable);
		void			bind(const std::string &ip, int port, unsigned int flags, uv::Error &er);
		void			bind(const std::string &ip, int port, unsigned int flags);
		void			getsockname(struct sockaddr &name, int &namelen, uv::Error &er) const;
		void			getsockname(struct sockaddr &name, int &namelenr) const;
		void			getpeername(struct sockaddr &name, int &namelen, uv::Error &er) const;
		void			getpeername(struct sockaddr &name, int &namelen) const;
		void			connect(const std::string &ip, int port,
						std::function<void(const Error &error)> handler, uv::Error &er);
		void			connect(const std::string &ip, int port,
						std::function<void(const Error &error)> handler);

	private:
		std::function<void(const Error &error)>	m_connectHandler = [](const Error &error) {};
	};





	inline Tcp::Tcp(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_tcp_init(loop.m_loop_ptr, &m_handle);
	}

	inline Tcp::Tcp(uv::Loop &loop, unsigned int flags)
	{
		m_handle.data = this;
		uv_tcp_init_ex(loop.m_loop_ptr, &m_handle, flags);
	}

	inline void Tcp::open(uv_os_sock_t sock, uv::Error &er)
	{
		er.m_error = uv_tcp_open(&m_handle, sock);
	}

	inline void Tcp::open(uv_os_sock_t sock)
	{
		uv::Error er;
		open(sock, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}

	inline void Tcp::nodelay(int enable, uv::Error &er)
	{
		er.m_error = uv_tcp_nodelay(&m_handle, enable);
	}

	inline void Tcp::nodelay(int enable)
	{
		uv::Error er;
		nodelay(enable, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}

	inline void Tcp::keepalive(int enable, unsigned int delay, uv::Error &er)
	{
		er.m_error = uv_tcp_keepalive(&m_handle, enable, delay);
	}

	inline void Tcp::keepalive(int enable, unsigned int delay)
	{
		uv::Error er;
		keepalive(enable, delay, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}

	inline void Tcp::simultaneousAccepts(int enable, uv::Error &er)
	{
		er.m_error = uv_tcp_simultaneous_accepts(&m_handle, enable);
	}

	inline void Tcp::simultaneousAccepts(int enable)
	{
		uv::Error er;
		simultaneousAccepts(enable, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}
	
	inline void Tcp::bind(const std::string &ip, int port, unsigned int flags, uv::Error &er)
	{
		struct sockaddr_in addr;
		er.m_error = uv_ip4_addr(ip.c_str(), port, &addr);
		if (er) return;

		er.m_error = uv_tcp_bind(&m_handle, reinterpret_cast<sockaddr *>(&addr), flags);
	}

	inline void Tcp::bind(const std::string &ip, int port, unsigned int flags)
	{
		uv::Error er;
		bind(ip, port, flags, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}
	
	inline void Tcp::getsockname(struct sockaddr &name, int &namelen, uv::Error &er) const
	{
		er.m_error = uv_tcp_getsockname(&m_handle, &name, &namelen);
	}

	inline void Tcp::getsockname(sockaddr &name, int &namelen) const
	{
		uv::Error er;
		getsockname(name, namelen, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}
	
	inline void Tcp::getpeername(struct sockaddr &name, int &namelen, uv::Error &er) const
	{
		er.m_error = uv_tcp_getpeername(&m_handle, &name, &namelen);
	}

	inline void Tcp::getpeername(sockaddr &name, int &namelen) const
	{
		uv::Error er;
		getpeername(name, namelen, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}
	
	inline void Tcp::connect(const std::string &ip, int port, std::function<void(const Error &error)> handler, uv::Error &er)
	{
		m_connectHandler = handler;

		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr)
		{
			er.m_error = ENOMEM;
			return;
		}

		struct sockaddr_in addr;
		er.m_error = uv_ip4_addr(ip.c_str(), port, &addr);
		if (er) return;

		er.m_error = uv_tcp_connect(&connect->m_handle, &m_handle, reinterpret_cast<sockaddr *>(&addr),
			[](uv_connect_t *req, int status) {

            std::shared_ptr<Connect> connect(reinterpret_cast<Connect *>(req->data));
			
            auto &tcp = *reinterpret_cast<uv::Tcp *>(req->handle->data);
			tcp.m_connectHandler(Error(status));
		});
	}

	inline void Tcp::connect(const std::string &ip, int port, std::function<void(const Error&error)> handler)
	{
		uv::Error er;
		connect(ip, port, handler, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}
}


#endif
