#ifndef UV_LOOP_HPP
#define UV_LOOP_HPP

#include <uv.h>

#include "Noncopyable.hpp"

namespace uv
{
	class Pipe;
	class Tty;
	class Tcp;
	class Udp;
	class Check;
	class Timer;
	class Signal;
	class Prepare;
	class Poll;
	class Idle;
	class Async;
	class Work;
	class FileStream;
	class FileStreamPoll;
	class FileStreamEvent;
	class GetAddrInfo;


    class Loop : public Noncopyable
    {
    public:
		inline explicit	Loop(bool is_default = false);
		inline			~Loop();

		inline void		run();
        inline void		runOnce();
        inline void		runNowait();
		inline void		stop();
		inline uint64_t	now() const;
		inline void		updateTime();
		inline int		alive() const;
		inline int		backendFd() const;
		inline int		backendTimeout() const;
		inline void		printAllHandles(FILE *stream);
		inline void		printActiveHandles(FILE *stream);
		inline uv_loop_t	&operator()();

	private:
		friend class Pipe;
		friend class Tcp;
		friend class Udp;
		friend class Check;
		friend class Timer;
		friend class Signal;
		friend class Prepare;
		friend class Poll;
		friend class Idle;
		friend class Tty;
		friend class Async;
		friend class Work;
		friend class FileStream;
		friend class FileStreamPoll;
		friend class FileStreamEvent;
		friend class GetAddrInfo;

    private:
        uv_loop_t		m_loop;	//not use
        uv_loop_t		*m_loop_ptr;
    };





	Loop::Loop(bool is_default)
	{
        if (is_default) {
			m_loop_ptr = uv_default_loop();
        }
        else {
            m_loop_ptr = &m_loop;
		    uv_loop_init(m_loop_ptr);
        }
        m_loop_ptr->data = this;
    }

	Loop::~Loop()
	{
		uv_loop_close(m_loop_ptr);
	}

	void Loop::run()
	{
		uv_run(m_loop_ptr, UV_RUN_DEFAULT);
	}

    void Loop::runOnce()
    {
        uv_run(m_loop_ptr, UV_RUN_ONCE);
    }

    void Loop::runNowait()
    {
        uv_run(m_loop_ptr, UV_RUN_NOWAIT);
    }

	void Loop::stop()
	{
		uv_stop(m_loop_ptr);
	}

	uint64_t Loop::now() const
	{
		return uv_now(m_loop_ptr);
	}

	void Loop::updateTime()
	{
		uv_update_time(m_loop_ptr);
	}

	int Loop::alive() const
	{
		return uv_loop_alive(m_loop_ptr);
	}

	int Loop::backendFd() const
	{
		return uv_backend_fd(m_loop_ptr);
	}

	int Loop::backendTimeout() const
	{
		return uv_backend_timeout(m_loop_ptr);
	}

	void Loop::printAllHandles(FILE *stream)
	{
		uv_print_all_handles(m_loop_ptr, stream);
	}

	void Loop::printActiveHandles(FILE *stream)
	{
		uv_print_active_handles(m_loop_ptr, stream);
	}

	uv_loop_t &Loop::operator()()
	{
		return *m_loop_ptr;
	}
}

#endif
