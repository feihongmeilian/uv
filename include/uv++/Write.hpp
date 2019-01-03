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
		Write(const char *p, ssize_t len);

	private:
		template<typename T> friend class Stream;
	private:
		uv_buf_t		buf_;
	};





	inline Write::Write(const char *p, ssize_t len)
	{
		buf_.base = const_cast<char*>(p);
		buf_.len = static_cast<size_t>(len);
		handle_.data = this;
	}
}


#endif
