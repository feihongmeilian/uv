#ifndef UV_GET_ADDR_INFO_HPP
#define UV_GET_ADDR_INFO_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Error.hpp"
#include "Loop.hpp"
#include "Req.hpp"

namespace uv
{
	class GetAddrInfo : public Req<uv_getaddrinfo_t>
	{
	public:
		inline explicit	GetAddrInfo(uv::Loop &loop);
		inline int		get(const std::string &node, const std::string &service, const struct addrinfo &hints,
							std::function<void(const Error &error, struct addrinfo *res)> handler);
		
		
		inline static void	freeaddrinfo(struct addrinfo *ai);
	private:
		uv::Loop			&m_loop;
		std::function<void(const Error &error, struct addrinfo *res)> m_callbackHandler;
	};





	GetAddrInfo::GetAddrInfo(uv::Loop &loop)
		: m_loop(loop)
	{
		m_callbackHandler = [](const Error &error, struct addrinfo *res) {};
		m_handle.data = this;
	}

	int	GetAddrInfo::get(const std::string &node, const std::string &service, const struct addrinfo &hints,
		std::function<void(const Error &error, struct addrinfo *res)> handler)
	{
		m_callbackHandler = handler;
		return uv_getaddrinfo(m_loop.m_loop_ptr, &m_handle, [](uv_getaddrinfo_t* req, int status, struct addrinfo *res) {
			auto &addr = *reinterpret_cast<uv::GetAddrInfo *>(req->data);
			addr.m_callbackHandler(Error(status), res);
		}, node.c_str(), service.c_str(), &hints);
	}

	void GetAddrInfo::freeaddrinfo(struct addrinfo *ai)
	{
		uv_freeaddrinfo(ai);
	}
}


#endif
