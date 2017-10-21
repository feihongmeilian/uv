#ifndef UV_SEM_HPP
#define UV_SEM_HPP

#include <uv.h>

#include "../Noncopyable.hpp"

namespace uv
{
	class Sem : public Noncopyable
	{
	public:
		explicit		Sem(unsigned int value);
		~Sem();

		void			post();
		void			wait();
		int			trywait();
	private:
		uv_sem_t		m_handle;
	};





	inline Sem::Sem(unsigned int value)
	{
		uv_sem_init(&m_handle, value);
	}

	inline Sem::~Sem()
	{
		uv_sem_destroy(&m_handle);
	}

	inline void Sem::post()
	{
		uv_sem_post(&m_handle);
	}

	inline void Sem::wait()
	{
		uv_sem_wait(&m_handle);
	}

	inline int Sem::trywait()
	{
		return uv_sem_trywait(&m_handle);
	}
}


#endif
