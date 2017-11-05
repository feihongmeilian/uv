#ifndef UV_GET_ADDR_INFO_HPP
#define UV_GET_ADDR_INFO_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Req.hpp"

namespace uv
{
	class GetAddrInfo : public Req<uv_getaddrinfo_t>
	{
	public:
		explicit		GetAddrInfo(uv::Loop &loop);
		void			get(const std::string &node, const std::string &service, const struct addrinfo &hints,
						std::function<void(const std::error_code &ec, struct addrinfo *res)> handler, std::error_code &ec);
		void			get(const std::string &node, const std::string &service, const struct addrinfo &hints,
						std::function<void(const std::error_code &ec, struct addrinfo *res)> handler);
		
		static void		freeaddrinfo(struct addrinfo *ai);
	private:
		uv::Loop		&m_loop;
		std::function<void(const std::error_code &ec, struct addrinfo *res)> m_callbackHandler
			= [] (const std::error_code &ec,struct addrinfo *) {};
	};





	inline GetAddrInfo::GetAddrInfo(uv::Loop &loop)
		: m_loop(loop)
	{
		m_handle.data = this;
	}

	inline void GetAddrInfo::get(const std::string &node, const std::string &service, const struct addrinfo &hints,
		std::function<void(const std::error_code &ecr, struct addrinfo *res)> handler, std::error_code &ec)
	{
		m_callbackHandler = handler;
		auto status = uv_getaddrinfo(m_loop.value(), &m_handle, [](uv_getaddrinfo_t* req, int status, struct addrinfo *res) {
			auto &addr = *reinterpret_cast<uv::GetAddrInfo *>(req->data);
			addr.m_callbackHandler(makeErrorCode(status), res);
		}, node.c_str(), service.c_str(), &hints);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void GetAddrInfo::get(const std::string &node, const std::string &service, const addrinfo &hints,
		std::function<void(const std::error_code &ec, struct addrinfo *res)> handler)
	{
		std::error_code ec;

		get(node, service, hints, handler, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void GetAddrInfo::freeaddrinfo(struct addrinfo *ai)
	{
		uv_freeaddrinfo(ai);
	}
}


#endif
