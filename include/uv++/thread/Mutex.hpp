#ifndef UV_MUTEX_HPP
#define UV_MUTEX_HPP

#include <uv.h>

#include "../Noncopyable.hpp"

namespace uv
{
	class Cond;


	class Mutex : public Noncopyable
	{
	public:
		Mutex();
		~Mutex();

		void        lock();
		int         trylock();
		void        unlock();

	private:
		friend class Cond;
	private:
		uv_mutex_t  handle_;
	};





	inline Mutex::Mutex()
	{
		uv_mutex_init( &handle_);
	}

	inline Mutex::~Mutex()
	{
		uv_mutex_destroy(&handle_);
	}

	inline void Mutex::lock()
	{
		uv_mutex_lock(&handle_);
	}

	inline int Mutex::trylock()
	{
		return uv_mutex_trylock(&handle_);
	}

	inline void Mutex::unlock()
	{
		uv_mutex_unlock(&handle_);
	}
}


#endif
