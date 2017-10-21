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

		void			signal();
		void			broadcast();
		void			wait(uv::Mutex &mutex);
		int			timedwait(uv::Mutex &mutex, uint64_t timeout);
	private:
		uv_cond_t	m_handle;
	};





	inline Cond::Cond()
	{
		uv_cond_init(&m_handle);
	}

	inline Cond::~Cond()
	{
		uv_cond_destroy(&m_handle);
	}

	inline void Cond::signal()
	{
		uv_cond_signal(&m_handle);
	}

	inline void Cond::broadcast()
	{
		uv_cond_broadcast(&m_handle);
	}

	inline void Cond::wait(uv::Mutex &mutex)
	{
		uv_cond_wait(&m_handle, &mutex.m_handle);
	}

	inline int Cond::timedwait(uv::Mutex &mutex, uint64_t timeout)
	{
		return uv_cond_timedwait(&m_handle, &mutex.m_handle, timeout);
	}
}


#endif
