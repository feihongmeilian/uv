#ifndef UV_BARRIER_HPP
#define UV_BARRIER_HPP

#include <uv.h>

#include "../Noncopyable.hpp"

namespace uv
{
	class Barrier : public Noncopyable
	{
	public:
		explicit    Barrier(unsigned int count);
		~Barrier();

		int         wait();

	private:
		uv_barrier_t	handle_;
	};





	inline Barrier::Barrier(unsigned int count)
	{
		uv_barrier_init(&handle_, count);
	}

	inline Barrier::~Barrier()
	{
		uv_barrier_destroy(&handle_);
	}

	inline int Barrier::wait()
	{
		return uv_barrier_wait(&handle_);
	}
}


#endif
