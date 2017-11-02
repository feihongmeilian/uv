#ifndef UV_LOOP_HPP
#define UV_LOOP_HPP

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
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
		explicit		Loop(bool is_default = false);
						~Loop();

		void			run(uv::Error &err);
		void			run();
        void			runOnce(uv::Error &err);
		void			runOnce();
        void			runNowait(uv::Error &err);
		void			runNowait();
		void			stop();
		void			fork(uv::Error &err);
		void			fork();
		uint64_t		now() const;
		void			updateTime();
		bool			alive() const;
		int				backendFd() const;
		int				backendTimeout() const;
		void			printAllHandles(FILE *stream);
		void			printActiveHandles(FILE *stream);
		uv_loop_t		&operator()();

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
        uv_loop_t	m_loop;	//not use
        uv_loop_t	*m_loop_ptr;
    };





	inline Loop::Loop(bool is_default)
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

	inline Loop::~Loop()
	{
		uv_loop_close(m_loop_ptr);
	}

	inline void Loop::run(uv::Error &err)
	{
		err.m_error = uv_run(m_loop_ptr, UV_RUN_DEFAULT);
	}

	inline void Loop::run()
	{
		uv::Error err;

		run(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Loop::runOnce(uv::Error &err)
    {
		err.m_error = uv_run(m_loop_ptr, UV_RUN_ONCE);
    }

	inline void Loop::runOnce()
	{
		uv::Error err;

		runOnce(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Loop::runNowait(uv::Error &err)
    {
		err.m_error = uv_run(m_loop_ptr, UV_RUN_NOWAIT);
    }

	inline void Loop::runNowait()
	{
		uv::Error err;

		runNowait(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Loop::stop()
	{
		uv_stop(m_loop_ptr);
	}

	inline void Loop::fork(uv::Error &err)
	{
		err.m_error = uv_loop_fork(m_loop_ptr);
	}

	inline void Loop::fork()
	{
		uv::Error err;

		fork(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline uint64_t Loop::now() const
	{
		return uv_now(m_loop_ptr);
	}

	inline void Loop::updateTime()
	{
		uv_update_time(m_loop_ptr);
	}

	inline bool Loop::alive() const
	{
		return uv_loop_alive(m_loop_ptr);
	}

	inline int Loop::backendFd() const
	{
		return uv_backend_fd(m_loop_ptr);
	}

	inline int Loop::backendTimeout() const
	{
		return uv_backend_timeout(m_loop_ptr);
	}

	inline void Loop::printAllHandles(FILE *stream)
	{
		uv_print_all_handles(m_loop_ptr, stream);
	}

	inline void Loop::printActiveHandles(FILE *stream)
	{
		uv_print_active_handles(m_loop_ptr, stream);
	}

	inline uv_loop_t &Loop::operator()()
	{
		return *m_loop_ptr;
	}
}

#endif
