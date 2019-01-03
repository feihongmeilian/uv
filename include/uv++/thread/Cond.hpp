#ifndef UV_COND_HPP
#define UV_COND_HPP

#include <uv.h>

#include "Mutex.hpp"
#include "../Noncopyable.hpp"

namespace uv
{
	class Cond : public Noncopyable
	{
	public:
		Cond();
		~Cond();

		void        signal();
		void        broadcast();
		void        wait(uv::Mutex &mutex);
		int         timedwait(uv::Mutex &mutex, uint64_t timeout);
	private:
		uv_cond_t   handle_;
	};





	inline Cond::Cond()
	{
		uv_cond_init(&handle_);
	}

	inline Cond::~Cond()
	{
		uv_cond_destroy(&handle_);
	}

	inline void Cond::signal()
	{
		uv_cond_signal(&handle_);
	}

	inline void Cond::broadcast()
	{
		uv_cond_broadcast(&handle_);
	}

	inline void Cond::wait(uv::Mutex &mutex)
	{
		uv_cond_wait(&handle_, &mutex.handle_);
	}

	inline int Cond::timedwait(uv::Mutex &mutex, uint64_t timeout)
	{
		return uv_cond_timedwait(&handle_, &mutex.handle_, timeout);
	}
}


#endif
