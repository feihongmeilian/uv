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

		void			lock();
		int			trylock();
		void			unlock();

	private:
		friend class Cond;
	private:
		uv_mutex_t		m_handle;
	};





	inline Mutex::Mutex()
	{
		uv_mutex_init( &m_handle);
	}

	inline Mutex::~Mutex()
	{
		uv_mutex_destroy(&m_handle);
	}

	inline void Mutex::lock()
	{
		uv_mutex_lock(&m_handle);
	}

	inline int Mutex::trylock()
	{
		return uv_mutex_trylock(&m_handle);
	}

	inline void Mutex::unlock()
	{
		uv_mutex_unlock(&m_handle);
	}
}


#endif
