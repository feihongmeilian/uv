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
		inline Loop();
		inline ~Loop();

		inline void		run(uv_run_mode mode = UV_RUN_DEFAULT);
		inline void		stop();
		inline uint64_t	now() const;
		inline void		updateTime();
		inline int		alive() const;
		inline uv_os_fd_t	backendFd() const;
		inline int		backendTimeout() const;
		inline void		printAllHandles(FILE *stream);
		inline void		printActiveHandles(FILE *stream);

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
        uv_loop_t		m_loop;
    };





	Loop::Loop()
	{
		uv_loop_init(&m_loop);
		m_loop.data = this;
	}

	Loop::~Loop()
	{
		uv_loop_close(&m_loop);
	}

	void Loop::run(uv_run_mode mode)
	{
		uv_run(&m_loop, mode);
	}

	void Loop::stop()
	{
		uv_stop(&m_loop);
	}

	uint64_t Loop::now() const
	{
		return uv_now(&m_loop);
	}

	void Loop::updateTime()
	{
		uv_update_time(&m_loop);
	}

	int Loop::alive() const
	{
		return uv_loop_alive(&m_loop);
	}

	uv_os_fd_t Loop::backendFd() const
	{
		return uv_backend_fd(&m_loop);
	}

	int Loop::backendTimeout() const
	{
		return uv_backend_timeout(&m_loop);
	}

	void Loop::printAllHandles(FILE *stream)
	{
		uv_print_all_handles(&m_loop, stream);
	}

	void Loop::printActiveHandles(FILE *stream)
	{
		uv_print_active_handles(&m_loop, stream);
	}
}

#endif //UVNET_LOOP_H
