#ifndef UV_CONNECT_HPP
#define UV_CONNECT_HPP

#include <uv.h>

#include "Req.hpp"

namespace uv
{
	class Tcp;
	class Pipe;

	class Connect : public Req<uv_connect_t>
	{
	public:
		inline Connect();

	private:
		friend class Tcp;
		friend class Pipe;
	};





	Connect::Connect()
	{
		m_handle.data = this;
	}
}


#endif
