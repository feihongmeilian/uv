#ifndef UV_UDP_HPP
#define UV_UDP_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "FileStreamHandle.hpp"
#include "UdpSend.hpp"

namespace uv
{
	class Udp : public FileStreamHandle<uv_udp_t>
	{
	public:
		Udp();

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        init(uv::Loop &loop, unsigned int flags, std::error_code &ec);
		void        init(uv::Loop &loop, unsigned int flags);
		void        open(uv_os_sock_t sock, std::error_code &ec);
		void        open(uv_os_sock_t sock);
		void        bind(const struct sockaddr &addr, unsigned int flags, std::error_code &ec);
		void        bind(const struct sockaddr &addr, unsigned int flags);
		void        getsockname(struct sockaddr &name, int &namelen, std::error_code &ec) const;
		void        getsockname(struct sockaddr &name, int &namelen) const;
		void        setMembership(const std::string &multicast_addr, const std::string &interface_addr,
			            uv_membership membership, std::error_code &ec);
		void        setMembership(const std::string &multicast_addr,
			            const std::string &interface_addr, uv_membership membership);
		void        setMulticastLoop(int on, std::error_code &ec);
		void        setMulticastLoop(int on);
		void        setMulticastTtl(int ttl, std::error_code &ec);
		void        setMulticastTtl(int ttl);
		void        setMulticastInterface(const std::string &interface_addr, std::error_code &ec);
		void        setMulticastInterface(const std::string &interface_addr);
		void        setBroadcast(int on, std::error_code &ec);
		void        setBroadcast(int on);
		void        setTtl(int ttl, std::error_code &ec);
		void        setTtl(int ttl);

		void        recvStop(std::error_code &ec);
		void        recvStop();

	private:
		uv::UdpSend	udpSend_;
	};





	inline Udp::Udp()
	{
		handle_.data = this;
	}


	inline void Udp::init(uv::Loop &loop, std::error_code &ec)
	{
		const auto status = uv_udp_init(loop.value(), &handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::init(uv::Loop &loop, unsigned int flags, std::error_code &ec)
	{
		const auto status = uv_udp_init_ex(loop.value(), &handle_, flags);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::init(uv::Loop &loop, unsigned int flags)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::open(uv_os_sock_t sock, std::error_code &ec)
	{
		const auto status = uv_udp_open(&handle_, sock);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::open(uv_os_sock_t sock)
	{
		std::error_code ec;

		open(sock, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::bind(const struct sockaddr &addr, unsigned int flags, std::error_code &ec)
	{
		const auto status = uv_udp_bind(&handle_, &addr, flags);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::bind(const sockaddr &addr, unsigned int flags)
	{
		std::error_code ec;

		bind(addr, flags, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::getsockname(struct sockaddr &name, int &namelen, std::error_code &ec) const
	{
		const auto status = uv_udp_getsockname(&handle_, &name, &namelen);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::getsockname(sockaddr &name, int &namelen) const
	{
		std::error_code ec;

		getsockname(name, namelen, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::setMembership(const std::string &multicast_addr,
		const std::string &interface_addr, uv_membership membership, std::error_code &ec)
	{
		const auto status = uv_udp_set_membership(&handle_, multicast_addr.c_str(),
				interface_addr.c_str(), membership);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::setMembership(const std::string &multicast_addr,
		const std::string &interface_addr, uv_membership membership)
	{
		std::error_code ec;

		setMembership(multicast_addr, interface_addr, membership, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::setMulticastLoop(int on, std::error_code &ec)
	{
		const auto status = uv_udp_set_multicast_loop(&handle_, on);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::setMulticastLoop(int on)
	{
		std::error_code ec;

		setMulticastLoop(on, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::setMulticastTtl(int ttl, std::error_code &ec)
	{
		const auto status = uv_udp_set_multicast_ttl(&handle_, ttl);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::setMulticastTtl(int ttl)
	{
		std::error_code ec;

		setMulticastTtl(ttl, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::setMulticastInterface(const std::string &interface_addr, std::error_code &ec)
	{
		const auto status = uv_udp_set_multicast_interface(&handle_, interface_addr.c_str());

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::setMulticastInterface(const std::string &interface_addr)
	{
		std::error_code ec;

		setMulticastInterface(interface_addr, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::setBroadcast(int on, std::error_code &ec)
	{
		const auto status = uv_udp_set_broadcast(&handle_, on);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::setBroadcast(int on)
	{
		std::error_code ec;

		setBroadcast(on, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::setTtl(int ttl, std::error_code &ec)
	{
		const auto status = uv_udp_set_ttl(&handle_, ttl);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::setTtl(int ttl)
	{
		std::error_code ec;

		setTtl(ttl, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Udp::recvStop(std::error_code &ec)
	{
		const auto status = uv_udp_recv_stop(&handle_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Udp::recvStop()
	{
		std::error_code ec;

		recvStop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
