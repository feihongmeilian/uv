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
						std::function<void(const Error &error, struct addrinfo *res)> handler, uv::Error &err);
		void			get(const std::string &node, const std::string &service, const struct addrinfo &hints,
						std::function<void(const Error &error, struct addrinfo *res)> handler);
		
		static void	freeaddrinfo(struct addrinfo *ai);
	private:
		uv::Loop		&m_loop;
		std::function<void(const Error &error, struct addrinfo *res)> m_callbackHandler
			= [] (const Error &, struct addrinfo *) {};
	};





	inline GetAddrInfo::GetAddrInfo(uv::Loop &loop)
		: m_loop(loop)
	{
		m_handle.data = this;
	}

	inline void GetAddrInfo::get(const std::string &node, const std::string &service, const struct addrinfo &hints,
		std::function<void(const Error &error, struct addrinfo *res)> handler, uv::Error &err)
	{
		m_callbackHandler = handler;
		err.m_error = uv_getaddrinfo(m_loop.m_loop_ptr, &m_handle, [](uv_getaddrinfo_t* req, int status, struct addrinfo *res) {
			auto &addr = *reinterpret_cast<uv::GetAddrInfo *>(req->data);
			addr.m_callbackHandler(Error(status), res);
		}, node.c_str(), service.c_str(), &hints);
	}

	inline void GetAddrInfo::get(const std::string &node, const std::string &service, const addrinfo &hints,
		std::function<void(const Error&error, struct addrinfo*res)> handler)
	{
		uv::Error err;

		get(node, service, hints, handler, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void GetAddrInfo::freeaddrinfo(struct addrinfo *ai)
	{
		uv_freeaddrinfo(ai);
	}
}


#endif
