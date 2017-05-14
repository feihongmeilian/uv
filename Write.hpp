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
		inline Write();

	private:
		template<typename T> friend class Stream;
	};





	Write::Write()
	{
		m_handle.data = this;
	}
}


#endif
