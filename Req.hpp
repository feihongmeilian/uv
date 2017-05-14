#ifndef UV_REQ_HPP
#define UV_REQ_HPP

#include <uv.h>

#include "Noncopyable.hpp"

namespace uv
{
	template<typename T>
	class Req : public Noncopyable
	{
	public:
		inline int		cancel();

	protected:
		T				m_handle;
	};




	template<typename T>
	int Req<T>::cancel()
	{
		return uv_cancel(reinterpret_cast<uv_req_t *>(&m_handle));
	}
}


#endif
