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

		void        init(bool is_default, std::error_code &ec);
		void        init(bool is_default = false);
		void        run(std::error_code &ec);
		void        run();
        void        runOnce(std::error_code &ec);
		void        runOnce();
        void        runNowait(std::error_code &ec);
		void        runNowait();
		void        stop();
		void        fork(std::error_code &ec);
		void        fork();
		uint64_t    now() const;
		void        updateTime();
		bool        alive() const;
		int         backendFd() const;
		int         backendTimeout() const;
		void        printAllHandles(FILE *stream);
		void        printActiveHandles(FILE *stream);
		uv_loop_t   &operator()();
		uv_loop_t   *value();

    private:
        uv_loop_t	loop_;	//not use
        uv_loop_t	*loop_ptr_;
    };





	inline Loop::~Loop()
	{
		uv_loop_close(loop_ptr_);
	}

	inline void Loop::init(bool is_default, std::error_code & ec)
	{
		if (is_default) {
			loop_ptr_ = uv_default_loop();
		}
		else {
			loop_ptr_ = &loop_;
			auto status = uv_loop_init(loop_ptr_);
			if (status != 0) {
				ec = makeErrorCode(status);
				return;
			}
		}

		if (loop_ptr_ == nullptr) {
			ec = makeErrorCode(ENOMEM);
			return;
		}

		loop_ptr_->data = this;
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
		auto status = uv_run(loop_ptr_, UV_RUN_DEFAULT);
		
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
		auto status = uv_run(loop_ptr_, UV_RUN_ONCE);

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
		auto status = uv_run(loop_ptr_, UV_RUN_NOWAIT);

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
		uv_stop(loop_ptr_);
	}

	inline void Loop::fork(std::error_code &ec)
	{
		auto status = uv_loop_fork(loop_ptr_);

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
		return uv_now(loop_ptr_);
	}

	inline void Loop::updateTime()
	{
		uv_update_time(loop_ptr_);
	}

	inline bool Loop::alive() const
	{
		return uv_loop_alive(loop_ptr_);
	}

	inline int Loop::backendFd() const
	{
		return uv_backend_fd(loop_ptr_);
	}

	inline int Loop::backendTimeout() const
	{
		return uv_backend_timeout(loop_ptr_);
	}

	inline void Loop::printAllHandles(FILE *stream)
	{
		uv_print_all_handles(loop_ptr_, stream);
	}

	inline void Loop::printActiveHandles(FILE *stream)
	{
		uv_print_active_handles(loop_ptr_, stream);
	}

	inline uv_loop_t &Loop::operator()()
	{
		return *loop_ptr_;
	}
	inline uv_loop_t * Loop::value()
	{
		return loop_ptr_;
	}
}

#endif
