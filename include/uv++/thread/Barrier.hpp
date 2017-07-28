#ifndef UV_BARRIER_HPP
#define UV_BARRIER_HPP

#include <uv.h>

#include "uv/Noncopyable.hpp"

namespace uv
{
	class Barrier : public Noncopyable
	{
	public:
		inline explicit Barrier(unsigned int count);
		inline ~Barrier();

		inline int		wait();

	private:
		uv_barrier_t		m_handle;
	};





	Barrier::Barrier(unsigned int count)
	{
		uv_barrier_init(&m_handle, count);
	}

	Barrier::~Barrier()
	{
		uv_barrier_destroy(&m_handle);
	}

	int Barrier::wait()
	{
		return uv_barrier_wait(&m_handle);
	}
}


#endif
