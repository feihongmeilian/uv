#ifndef UV_REQ_HPP
#define UV_REQ_HPP

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	template<typename T>
	class Req : public Noncopyable
	{
	public:
		void		cancel(std::error_code &ec);
		void		cancel();

	protected:
		T			m_handle;
	};




	template<typename T>
	inline void Req<T>::cancel(std::error_code &ec)
	{
		auto status = uv_cancel(reinterpret_cast<uv_req_t *>(&m_handle));

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void Req<T>::cancel()
	{
		std::error_code ec;

		cancel(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
