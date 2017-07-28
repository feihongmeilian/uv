#ifndef UV_UDP_HPP
#define UV_UDP_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Loop.hpp"
#include "Stream.hpp"
#include "UdpSend.hpp"

namespace uv
{
	class Udp : public Stream<uv_udp_t>
	{
	public:
		inline explicit	Udp(uv::Loop &loop);
		inline			Udp(uv::Loop &loop, unsigned int flags);

		inline int		open(uv_os_sock_t sock);
		inline int		bind(const struct sockaddr &addr, unsigned int flags);
		inline int		getsockname(struct sockaddr &name, int &namelen) const;

		inline int		setMembership(const std::string &multicast_addr,
						const std::string &interface_addr, uv_membership membership);
		inline int		setMulticastLoop(int on);
		inline int		setMulticastTtl(int ttl);
		inline int		setMulticastInterface(const std::string &interface_addr);
		inline int		setBroadcast(int on);
		inline int		setTtl(int ttl);

		inline int		recvStop();

	private:
		uv::UdpSend		m_udpSend;
	};





	Udp::Udp(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_udp_init(loop.m_loop_ptr, &m_handle);
	}

	Udp::Udp(uv::Loop &loop, unsigned int flags)
	{
		m_handle.data = this;
		uv_udp_init_ex(loop.m_loop_ptr, &m_handle, flags);
	}

	int Udp::open(uv_os_sock_t sock)
	{
		return uv_udp_open(&m_handle, sock);
	}

	int Udp::bind(const struct sockaddr &addr, unsigned int flags)
	{
		return uv_udp_bind(&m_handle, &addr, flags);
	}

	int Udp::getsockname(struct sockaddr &name, int &namelen) const
	{
		return uv_udp_getsockname(&m_handle, &name, &namelen);
	}

	int Udp::setMembership(const std::string &multicast_addr,
		const std::string &interface_addr, uv_membership membership)
	{
		return uv_udp_set_membership(&m_handle, multicast_addr.c_str(), 
				interface_addr.c_str(), membership);
	}

	int Udp::setMulticastLoop(int on)
	{
		return uv_udp_set_multicast_loop(&m_handle, on);
	}

	int Udp::setMulticastTtl(int ttl)
	{
		return uv_udp_set_multicast_ttl(&m_handle, ttl);
	}

	int Udp::setMulticastInterface(const std::string &interface_addr)
	{
		return uv_udp_set_multicast_interface(&m_handle, interface_addr.c_str());
	}

	int Udp::setBroadcast(int on)
	{
		return uv_udp_set_broadcast(&m_handle, on);
	}

	int Udp::setTtl(int ttl)
	{
		return uv_udp_set_ttl(&m_handle, ttl);
	}

	int Udp::recvStop()
	{
		return uv_udp_recv_stop(&m_handle);
	}
}


#endif
