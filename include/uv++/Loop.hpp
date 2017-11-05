#ifndef UV_LOOP_HPP
#define UV_LOOP_HPP

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Noncopyable.hpp"

namespace uv
{
    class Loop : public Noncopyable
    {
    public:
		Loop() = default;
		~Loop();

		void			init(bool is_default, std::error_code &ec);
		void			init(bool is_default = false);
		void			run(std::error_code &ec);
		void			run();
        void			runOnce(std::error_code &ec);
		void			runOnce();
        void			runNowait(std::error_code &ec);
		void			runNowait();
		void			stop();
		void			fork(std::error_code &ec);
		void			fork();
		uint64_t		now() const;
		void			updateTime();
		bool			alive() const;
		int				backendFd() const;
		int				backendTimeout() const;
		void			printAllHandles(FILE *stream);
		void			printActiveHandles(FILE *stream);
		uv_loop_t		&operator()();
		uv_loop_t		*value();

    private:
        uv_loop_t	m_loop;	//not use
        uv_loop_t	*m_loop_ptr;
    };





	inline Loop::~Loop()
	{
		uv_loop_close(m_loop_ptr);
	}

	inline void Loop::init(bool is_default, std::error_code & ec)
	{
		if (is_default) {
			m_loop_ptr = uv_default_loop();
		}
		else {
			m_loop_ptr = &m_loop;
			auto status = uv_loop_init(m_loop_ptr);
			if (status != 0) {
				ec = makeErrorCode(status);
				return;
			}
		}

		if (m_loop_ptr == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		m_loop_ptr->data = this;
	}

	inline void Loop::init(bool is_default)
	{
		std::error_code ec;

		init(is_default, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Loop::run(std::error_code &ec)
	{
		auto status = uv_run(m_loop_ptr, UV_RUN_DEFAULT);
		
		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Loop::run()
	{
		std::error_code ec;

		run(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Loop::runOnce(std::error_code &ec)
    {
		auto status = uv_run(m_loop_ptr, UV_RUN_ONCE);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
    }

	inline void Loop::runOnce()
	{
		std::error_code ec;

		runOnce(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Loop::runNowait(std::error_code &ec)
    {
		auto status = uv_run(m_loop_ptr, UV_RUN_NOWAIT);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
    }

	inline void Loop::runNowait()
	{
		std::error_code ec;

		runNowait(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Loop::stop()
	{
		uv_stop(m_loop_ptr);
	}

	inline void Loop::fork(std::error_code &ec)
	{
		auto status = uv_loop_fork(m_loop_ptr);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Loop::fork()
	{
		std::error_code ec;

		fork(ec);
		if (ec) {
			throw uv::Exception(ec);
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
	inline uv_loop_t * Loop::value()
	{
		return m_loop_ptr;
	}
}

#endif
