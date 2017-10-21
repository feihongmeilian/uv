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
		void			cancel(uv::Error &er);
		void			cancel();

	protected:
		T			m_handle;
	};




	template<typename T>
	inline void Req<T>::cancel(uv::Error &er)
	{
		er.m_error = uv_cancel(reinterpret_cast<uv_req_t *>(&m_handle));
	}

	template<typename T>
	inline void Req<T>::cancel()
	{
		uv::Error er;

		cancel(er);
		if (er) {
			throw uv::Exception(er);
		}
	}
}


#endif
