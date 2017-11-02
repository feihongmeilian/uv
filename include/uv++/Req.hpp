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
		void		cancel(uv::Error &err);
		void		cancel();

	protected:
		T			m_handle;
	};




	template<typename T>
	inline void Req<T>::cancel(uv::Error &err)
	{
		err.m_error = uv_cancel(reinterpret_cast<uv_req_t *>(&m_handle));
	}

	template<typename T>
	inline void Req<T>::cancel()
	{
		uv::Error err;

		cancel(err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
