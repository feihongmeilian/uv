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

		void        rdlock();
		int         tryrdlock();
		void        rdunlock();
		void        wrlock();
		int         trywrlock();
		void        wrunlock();
	private:
		uv_rwlock_t	handle_;
	};





	inline Rwlock::Rwlock()
	{
		uv_rwlock_init(&handle_);
	}

	inline Rwlock::~Rwlock()
	{
		uv_rwlock_destroy(&handle_);
	}

	inline void Rwlock::rdlock()
	{
		uv_rwlock_rdlock(&handle_);
	}
	
	inline int Rwlock::tryrdlock()
	{
		return uv_rwlock_tryrdlock(&handle_);
	}

	inline void Rwlock::rdunlock()
	{
		uv_rwlock_rdunlock(&handle_);
	}

	inline void Rwlock::wrlock()
	{
		uv_rwlock_wrlock(&handle_);
	}

	inline int Rwlock::trywrlock()
	{
		return uv_rwlock_trywrlock(&handle_);
	}

	inline void Rwlock::wrunlock()
	{
		uv_rwlock_wrunlock(&handle_);
	}
}


#endif
