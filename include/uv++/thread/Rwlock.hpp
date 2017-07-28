#ifndef UV_RWLOCK_HPP
#define UV_RWLOCK_HPP

#include <uv.h>

#include "uv/Noncopyable.hpp"

namespace uv
{
	class Rwlock : public Noncopyable
	{
	public:
		inline Rwlock();
		inline ~Rwlock();

		inline void		rdlock();
		inline int		tryrdlock();
		inline void		rdunlock();
		inline void		wrlock();
		inline int		trywrlock();
		inline void		wrunlock();
	private:
		uv_rwlock_t		m_handle;
	};





	Rwlock::Rwlock()
	{
		uv_rwlock_init(&m_handle);
	}

	Rwlock::~Rwlock()
	{
		uv_rwlock_destroy(&m_handle);
	}

	void Rwlock::rdlock()
	{
		uv_rwlock_rdlock(&m_handle);
	}
	
	int Rwlock::tryrdlock()
	{
		return uv_rwlock_tryrdlock(&m_handle);
	}

	void Rwlock::rdunlock()
	{
		uv_rwlock_rdunlock(&m_handle);
	}

	void Rwlock::wrlock()
	{
		uv_rwlock_wrlock(&m_handle);
	}

	int Rwlock::trywrlock()
	{
		return uv_rwlock_trywrlock(&m_handle);
	}

	void Rwlock::wrunlock()
	{
		uv_rwlock_wrunlock(&m_handle);
	}
}


#endif
