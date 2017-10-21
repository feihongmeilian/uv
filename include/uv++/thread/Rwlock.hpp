#ifndef UV_RWLOCK_HPP
#define UV_RWLOCK_HPP

#include <uv.h>

#include "../Noncopyable.hpp"

namespace uv
{
	class Rwlock : public Noncopyable
	{
	public:
		Rwlock();
		~Rwlock();

		void			rdlock();
		int			tryrdlock();
		void			rdunlock();
		void			wrlock();
		int			trywrlock();
		void			wrunlock();
	private:
		uv_rwlock_t	m_handle;
	};





	inline Rwlock::Rwlock()
	{
		uv_rwlock_init(&m_handle);
	}

	inline Rwlock::~Rwlock()
	{
		uv_rwlock_destroy(&m_handle);
	}

	inline void Rwlock::rdlock()
	{
		uv_rwlock_rdlock(&m_handle);
	}
	
	inline int Rwlock::tryrdlock()
	{
		return uv_rwlock_tryrdlock(&m_handle);
	}

	inline void Rwlock::rdunlock()
	{
		uv_rwlock_rdunlock(&m_handle);
	}

	inline void Rwlock::wrlock()
	{
		uv_rwlock_wrlock(&m_handle);
	}

	inline int Rwlock::trywrlock()
	{
		return uv_rwlock_trywrlock(&m_handle);
	}

	inline void Rwlock::wrunlock()
	{
		uv_rwlock_wrunlock(&m_handle);
	}
}


#endif
