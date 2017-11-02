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

		void			open(uv_os_sock_t sock, uv::Error &err);
		void			open(uv_os_sock_t sock);
		void			bind(const struct sockaddr &addr, unsigned int flags, uv::Error &err);
		void			bind(const struct sockaddr &addr, unsigned int flags);
		void			getsockname(struct sockaddr &name, int &namelen, uv::Error &err) const;
		void			getsockname(struct sockaddr &name, int &namelen) const;
		void			setMembership(const std::string &multicast_addr, const std::string &interface_addr,
						uv_membership membership, uv::Error &err);
		void			setMembership(const std::string &multicast_addr,
						const std::string &interface_addr,	uv_membership membership);
		void			setMulticastLoop(int on, uv::Error &err);
		void			setMulticastLoop(int on);
		void			setMulticastTtl(int ttl, uv::Error &err);
		void			setMulticastTtl(int ttl);
		void			setMulticastInterface(const std::string &interface_addr, uv::Error &err);
		void			setMulticastInterface(const std::string &interface_addr);
		void			setBroadcast(int on, uv::Error &err);
		void			setBroadcast(int on);
		void			setTtl(int ttl, uv::Error &err);
		void			setTtl(int ttl);

		void			recvStop(uv::Error &err);
		void			recvStop();

	private:
		uv::UdpSend		m_udpSend;
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

	inline void Udp::open(uv_os_sock_t sock, uv::Error &err)
	{
		err.m_error = uv_udp_open(&m_handle, sock);
	}

	inline void Udp::open(uv_os_sock_t sock)
	{
		uv::Error err;

		open(sock, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::bind(const struct sockaddr &addr, unsigned int flags, uv::Error &err)
	{
		err.m_error = uv_udp_bind(&m_handle, &addr, flags);
	}

	inline void Udp::bind(const sockaddr &addr, unsigned int flags)
	{
		uv::Error err;

		bind(addr, flags, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::getsockname(struct sockaddr &name, int &namelen, uv::Error &err) const
	{
		err.m_error = uv_udp_getsockname(&m_handle, &name, &namelen);
	}

	inline void Udp::getsockname(sockaddr &name, int &namelen) const
	{
		uv::Error err;

		getsockname(name, namelen, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::setMembership(const std::string &multicast_addr,
		const std::string &interface_addr, uv_membership membership, uv::Error &err)
	{
		err.m_error = uv_udp_set_membership(&m_handle, multicast_addr.c_str(),
				interface_addr.c_str(), membership);
	}

	inline void Udp::setMembership(const std::string &multicast_addr, const std::string &interface_addr, uv_membership membership)
	{
		uv::Error err;

		setMembership(multicast_addr, interface_addr, membership, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::setMulticastLoop(int on, uv::Error &err)
	{
		err.m_error = uv_udp_set_multicast_loop(&m_handle, on);
	}

	inline void Udp::setMulticastLoop(int on)
	{
		uv::Error err;

		setMulticastLoop(on, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::setMulticastTtl(int ttl, uv::Error &err)
	{
		err.m_error = uv_udp_set_multicast_ttl(&m_handle, ttl);
	}

	inline void Udp::setMulticastTtl(int ttl)
	{
		uv::Error err;

		setMulticastTtl(ttl, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::setMulticastInterface(const std::string &interface_addr, uv::Error &err)
	{
		err.m_error = uv_udp_set_multicast_interface(&m_handle, interface_addr.c_str());
	}

	inline void Udp::setMulticastInterface(const std::string &interface_addr)
	{
		uv::Error err;

		setMulticastInterface(interface_addr, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::setBroadcast(int on, uv::Error &err)
	{
		err.m_error = uv_udp_set_broadcast(&m_handle, on);
	}

	inline void Udp::setBroadcast(int on)
	{
		uv::Error err;

		setBroadcast(on, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::setTtl(int ttl, uv::Error &err)
	{
		err.m_error = uv_udp_set_ttl(&m_handle, ttl);
	}

	inline void Udp::setTtl(int ttl)
	{
		uv::Error err;

		setTtl(ttl, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Udp::recvStop(uv::Error &err)
	{
		err.m_error = uv_udp_recv_stop(&m_handle);
	}

	inline void Udp::recvStop()
	{
		uv::Error err;

		recvStop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
