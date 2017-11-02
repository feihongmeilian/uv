#ifndef UV_TIMER_HPP
#define UV_TIMER_HPP

#include <functional>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
    class Timer : public Noncopyable
    {
    public:
		explicit		Timer(uv::Loop &);

    public:
		void			start(std::function<void ()> handler, uint64_t timeout, uint64_t repeat, uv::Error &err);
		void			start(std::function<void()> handler, uint64_t timeout, uint64_t repeat);
		void			stop(uv::Error &err);
		void			stop();
		void			again(uv::Error &err);
		void			again();
		void			setRepeat(uint64_t repeat);
		uint64_t		getRepeat() const;

    private:
        uv_timer_t		m_handle;

	private:
        std::function<void ()>	m_startHandler = []() {};
    };





	inline Timer::Timer(uv::Loop &loop)
	{
		uv_timer_init(loop.m_loop_ptr, &m_handle);
		m_handle.data = this;
	}

	inline void Timer::start(std::function<void()> handler, uint64_t timeout, uint64_t repeat, uv::Error &err)
	{
		m_startHandler = handler;
		err.m_error = uv_timer_start(&m_handle, [](uv_timer_t *handle) {
			auto &timer = *reinterpret_cast<uv::Timer *>(handle->data);
			timer.m_startHandler();
		}, timeout, repeat);
	}

	inline void Timer::start(std::function<void()> handler, uint64_t timeout, uint64_t repeat)
	{
		uv::Error err;

		start(handler, timeout, repeat, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Timer::stop(uv::Error &err)
	{
		err.m_error = uv_timer_stop(&m_handle);
	}

	inline void Timer::stop()
	{
		uv::Error err;

		stop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Timer::again(uv::Error &err)
	{
		err.m_error = uv_timer_again(&m_handle);
	}

	inline void Timer::again()
	{
		uv::Error err;

		again(err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Timer::setRepeat(uint64_t repeat)
	{
		uv_timer_set_repeat(&m_handle, repeat);
	}

	inline uint64_t Timer::getRepeat() const
	{
		return uv_timer_get_repeat(&m_handle);
	}
}

#endif
