#ifndef UV_UDP_HPP
#define UV_UDP_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Stream.hpp"
#include "UdpSend.hpp"

namespace uv
{
	class Udp : public Stream<uv_udp_t>
	{
	public:
		explicit		Udp(uv::Loop &loop);
					Udp(uv::Loop &loop, unsigned int flags);

		void			open(uv_os_sock_t sock, uv::Error &er);
		void			open(uv_os_sock_t sock);
		void			bind(const struct sockaddr &addr, unsigned int flags, uv::Error &er);
		void			bind(const struct sockaddr &addr, unsigned int flags);
		void			getsockname(struct sockaddr &name, int &namelen, uv::Error &er) const;
		void			getsockname(struct sockaddr &name, int &namelen) const;
		void			setMembership(const std::string &multicast_addr, const std::string &interface_addr,
						uv_membership membership, uv::Error &er);
		void			setMembership(const std::string &multicast_addr,
						const std::string &interface_addr,	uv_membership membership);
		void			setMulticastLoop(int on, uv::Error &er);
		void			setMulticastLoop(int on);
		void			setMulticastTtl(int ttl, uv::Error &er);
		void			setMulticastTtl(int ttl);
		void			setMulticastInterface(const std::string &interface_addr, uv::Error &er);
		void			setMulticastInterface(const std::string &interface_addr);
		void			setBroadcast(int on, uv::Error &er);
		void			setBroadcast(int on);
		void			setTtl(int ttl, uv::Error &er);
		void			setTtl(int ttl);

		void			recvStop(uv::Error &er);
		void			recvStop();

	private:
		uv::UdpSend	m_udpSend;
	};





	inline Udp::Udp(uv::Loop &loop)
	{
		m_handle.data = this;
		uv_udp_init(loop.m_loop_ptr, &m_handle);
	}

	inline Udp::Udp(uv::Loop &loop, unsigned int flags)
	{
		m_handle.data = this;
		uv_udp_init_ex(loop.m_loop_ptr, &m_handle, flags);
	}

	inline void Udp::open(uv_os_sock_t sock, uv::Error &er)
	{
		er.m_error = uv_udp_open(&m_handle, sock);
	}

	inline void Udp::open(uv_os_sock_t sock)
	{
		uv::Error er;

		open(sock, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::bind(const struct sockaddr &addr, unsigned int flags, uv::Error &er)
	{
		er.m_error = uv_udp_bind(&m_handle, &addr, flags);
	}

	inline void Udp::bind(const sockaddr &addr, unsigned int flags)
	{
		uv::Error er;

		bind(addr, flags, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::getsockname(struct sockaddr &name, int &namelen, uv::Error &er) const
	{
		er.m_error = uv_udp_getsockname(&m_handle, &name, &namelen);
	}

	inline void Udp::getsockname(sockaddr &name, int &namelen) const
	{
		uv::Error er;

		getsockname(name, namelen, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::setMembership(const std::string &multicast_addr,
		const std::string &interface_addr, uv_membership membership, uv::Error &er)
	{
		er.m_error = uv_udp_set_membership(&m_handle, multicast_addr.c_str(),
				interface_addr.c_str(), membership);
	}

	inline void Udp::setMembership(const std::string &multicast_addr, const std::string &interface_addr, uv_membership membership)
	{
		uv::Error er;

		setMembership(multicast_addr, interface_addr, membership, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::setMulticastLoop(int on, uv::Error &er)
	{
		er.m_error = uv_udp_set_multicast_loop(&m_handle, on);
	}

	inline void Udp::setMulticastLoop(int on)
	{
		uv::Error er;

		setMulticastLoop(on, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::setMulticastTtl(int ttl, uv::Error &er)
	{
		er.m_error = uv_udp_set_multicast_ttl(&m_handle, ttl);
	}

	inline void Udp::setMulticastTtl(int ttl)
	{
		uv::Error er;

		setMulticastTtl(ttl, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::setMulticastInterface(const std::string &interface_addr, uv::Error &er)
	{
		er.m_error = uv_udp_set_multicast_interface(&m_handle, interface_addr.c_str());
	}

	inline void Udp::setMulticastInterface(const std::string &interface_addr)
	{
		uv::Error er;

		setMulticastInterface(interface_addr, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::setBroadcast(int on, uv::Error &er)
	{
		er.m_error = uv_udp_set_broadcast(&m_handle, on);
	}

	inline void Udp::setBroadcast(int on)
	{
		uv::Error er;

		setBroadcast(on, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::setTtl(int ttl, uv::Error &er)
	{
		er.m_error = uv_udp_set_ttl(&m_handle, ttl);
	}

	inline void Udp::setTtl(int ttl)
	{
		uv::Error er;

		setTtl(ttl, er);
		if (er) {
			throw uv::Exception(er);
		}
	}

	inline void Udp::recvStop(uv::Error &er)
	{
		er.m_error = uv_udp_recv_stop(&m_handle);
	}

	inline void Udp::recvStop()
	{
		uv::Error er;

		recvStop(er);
		if (er) {
			throw uv::Exception(er);
		}
	}
}


#endif
