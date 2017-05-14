#ifndef UV_MUTEX_HPP
#define UV_MUTEX_HPP

#include <uv.h>

#include "uv/Noncopyable.hpp"

namespace uv
{
	class Cond;


	class Mutex : public Noncopyable
	{
	public:
		inline Mutex();
		inline ~Mutex();

		inline void		lock();
		inline int		trylock();
		inline void		unlock();

	private:
		friend class Cond;
	private:
		uv_mutex_t		m_handle;
	};





	Mutex::Mutex()
	{
		uv_mutex_init( &m_handle);
	}

	Mutex::~Mutex()
	{
		uv_mutex_destroy(&m_handle);
	}

	void Mutex::lock()
	{
		uv_mutex_lock(&m_handle);
	}

	int Mutex::trylock()
	{
		return uv_mutex_trylock(&m_handle);
	}

	void Mutex::unlock()
	{
		uv_mutex_unlock(&m_handle);
	}
}


#endif
