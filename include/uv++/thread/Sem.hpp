#ifndef UV_SEM_HPP
#define UV_SEM_HPP

#include <uv.h>

#include "../Noncopyable.hpp"

namespace uv
{
	class Sem : public Noncopyable
	{
	public:
		explicit    Sem(unsigned int value);
		~Sem();

		void        post();
		void        wait();
		int         trywait();
	private:
		uv_sem_t    handle_;
	};





	inline Sem::Sem(unsigned int value)
	{
		uv_sem_init(&handle_, value);
	}

	inline Sem::~Sem()
	{
		uv_sem_destroy(&handle_);
	}

	inline void Sem::post()
	{
		uv_sem_post(&handle_);
	}

	inline void Sem::wait()
	{
		uv_sem_wait(&handle_);
	}

	inline int Sem::trywait()
	{
		return uv_sem_trywait(&handle_);
	}
}


#endif
