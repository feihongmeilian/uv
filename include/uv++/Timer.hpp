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
		Timer();

		void			init(uv::Loop &loop, std::error_code &ec);
		void			init(uv::Loop &loop);
		void			start(std::function<void ()> handler, uint64_t timeout, uint64_t repeat, std::error_code &ec);
		void			start(std::function<void()> handler, uint64_t timeout, uint64_t repeat);
		void			stop(std::error_code &ec);
		void			stop();
		void			again(std::error_code &ec);
		void			again();
		void			setRepeat(uint64_t repeat);
		uint64_t		getRepeat() const;

    private:
        uv_timer_t		m_handle;

	private:
        std::function<void ()>	m_startHandler = []() {};
    };





	inline Timer::Timer()
	{
		m_handle.data = this;
	}

	inline void Timer::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_timer_init(loop.value(), &m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Timer::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Timer::start(std::function<void()> handler, uint64_t timeout, uint64_t repeat, std::error_code &ec)
	{
		m_startHandler = handler;
		auto status = uv_timer_start(&m_handle, [](uv_timer_t *handle) {
			auto &timer = *reinterpret_cast<uv::Timer *>(handle->data);
			timer.m_startHandler();
		}, timeout, repeat);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Timer::start(std::function<void()> handler, uint64_t timeout, uint64_t repeat)
	{
		std::error_code ec;

		start(handler, timeout, repeat, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Timer::stop(std::error_code &ec)
	{
		auto status = uv_timer_stop(&m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Timer::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Timer::again(std::error_code &ec)
	{
		auto status = uv_timer_again(&m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Timer::again()
	{
		std::error_code ec;

		again(ec);
		if (ec) {
			throw uv::Exception(ec);
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
