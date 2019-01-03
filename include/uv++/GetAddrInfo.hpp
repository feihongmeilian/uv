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
		explicit    GetAddrInfo(uv::Loop &loop);
		void        get(const std::string &node, const std::string &service, const struct addrinfo &hints,
						const std::function<void(const std::error_code &ec, struct addrinfo *res)> &handler, std::error_code &ec);
		void        get(const std::string &node, const std::string &service, const struct addrinfo &hints,
						const std::function<void(const std::error_code &ec, struct addrinfo *res)> &handler);
		
		static void freeaddrinfo(struct addrinfo *ai);
	private:
		uv::Loop    &loop_;
		std::function<void(const std::error_code &ec, struct addrinfo *res)> callbackHandler_
			= [] (const std::error_code &ec,struct addrinfo *) {};
	};





	inline GetAddrInfo::GetAddrInfo(uv::Loop &loop)
		: loop_(loop)
	{
		handle_.data = this;
	}

	inline void GetAddrInfo::get(const std::string &node, const std::string &service, const struct addrinfo &hints,
		const std::function<void(const std::error_code &ecr, struct addrinfo *res)> &handler, std::error_code &ec)
	{
		callbackHandler_ = handler;
		auto status = uv_getaddrinfo(loop_.value(), &handle_, [](uv_getaddrinfo_t* req, int status, struct addrinfo *res) {
			auto &addr = *reinterpret_cast<uv::GetAddrInfo *>(req->data);
			addr.callbackHandler_(makeErrorCode(status), res);
		}, node.c_str(), service.c_str(), &hints);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void GetAddrInfo::get(const std::string &node, const std::string &service, const addrinfo &hints,
		const std::function<void(const std::error_code &ec, struct addrinfo *res)> &handler)
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
