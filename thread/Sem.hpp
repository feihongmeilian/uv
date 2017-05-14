#ifndef UV_SEM_HPP
#define UV_SEM_HPP

#include <uv.h>

#include "uv/Noncopyable.hpp"

namespace uv
{
	class Sem : public Noncopyable
	{
	public:
		inline explicit Sem(unsigned int value);
		inline ~Sem();

		inline void		post();
		inline void		wait();
		inline int		trywait();
	private:
		uv_sem_t			m_handle;
	};





	Sem::Sem(unsigned int value)
	{
		uv_sem_init(&m_handle, value);
	}

	Sem::~Sem()
	{
		uv_sem_destroy(&m_handle);
	}

	void Sem::post()
	{
		uv_sem_post(&m_handle);
	}

	void Sem::wait()
	{
		uv_sem_wait(&m_handle);
	}

	int Sem::trywait()
	{
		return uv_sem_trywait(&m_handle);
	}
}


#endif
