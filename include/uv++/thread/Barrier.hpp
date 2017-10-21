#ifndef UV_BARRIER_HPP
#define UV_BARRIER_HPP

#include <uv.h>

#include "../Noncopyable.hpp"

namespace uv
{
	class Barrier : public Noncopyable
	{
	public:
		explicit		Barrier(unsigned int count);
		~Barrier();

		int			wait();

	private:
		uv_barrier_t	m_handle;
	};





	inline Barrier::Barrier(unsigned int count)
	{
		uv_barrier_init(&m_handle, count);
	}

	inline Barrier::~Barrier()
	{
		uv_barrier_destroy(&m_handle);
	}

	inline int Barrier::wait()
	{
		return uv_barrier_wait(&m_handle);
	}
}


#endif
