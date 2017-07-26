#ifndef UV_WRITE_HPP
#define UV_WRITE_HPP

#include <uv.h>

#include "Req.hpp"

namespace uv
{
	template<typename T> class Stream;


	class Write : public Req<uv_write_t>
	{
	public:
		inline Write(const char *p, ssize_t len);

	private:
		template<typename T> friend class Stream;
	private:
		uv_buf_t		m_buf;
	};





	Write::Write(const char *p, ssize_t len)
	{
		m_buf.base = const_cast<char*>(p);
		m_buf.len = static_cast<size_t>(len);
		m_handle.data = this;
	}
}


#endif
