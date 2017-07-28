#ifndef UV_COND_HPP
#define UV_COND_HPP

#include <uv.h>

#include "Mutex.hpp"
#include "uv/Noncopyable.hpp"

namespace uv
{
	class Cond : public Noncopyable
	{
	public:
		inline Cond();
		inline ~Cond();

		inline void		signal();
		inline void		broadcast();
		inline void		wait(uv::Mutex &mutex);
		inline int		timedwait(uv::Mutex &mutex, uint64_t timeout);
	private:
		uv_cond_t		m_handle;
	};





	Cond::Cond()
	{
		uv_cond_init(&m_handle);
	}

	Cond::~Cond()
	{
		uv_cond_destroy(&m_handle);
	}

	void Cond::signal()
	{
		uv_cond_signal(&m_handle);
	}

	void Cond::broadcast()
	{
		uv_cond_broadcast(&m_handle);
	}

	void Cond::wait(uv::Mutex &mutex)
	{
		uv_cond_wait(&m_handle, &mutex.m_handle);
	}

	int Cond::timedwait(uv::Mutex &mutex, uint64_t timeout)
	{
		return uv_cond_timedwait(&m_handle, &mutex.m_handle, timeout);
	}
}


#endif
