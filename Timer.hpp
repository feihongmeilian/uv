#ifndef UV_TIMER_HPP
#define UV_TIMER_HPP

#include <functional>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
    class Timer : public Noncopyable
    {
    public:
		inline explicit	Timer(uv::Loop &);

    public:
		inline int		start(std::function<void (uv::Timer &)> handler, uint64_t timeout, uint64_t repeat);
		inline int		stop();
		inline int		again();
		inline void		setRepeat(uint64_t repeat);
		inline uint64_t	getRepeat() const;

    private:
        uv_timer_t		m_handle;

	private:
        std::function<void (uv::Timer &)>	m_startHandler = [](uv::Timer &) {};
    };





	Timer::Timer(uv::Loop &loop)
	{
		uv_timer_init(&loop.m_loop, &m_handle);
		m_handle.data = this;
	}

	int Timer::start(std::function<void(uv::Timer &)> handler, uint64_t timeout, uint64_t repeat)
	{
		m_startHandler = handler;
		return uv_timer_start(&m_handle, [](uv_timer_t *handle)
		{
			auto &timer = *reinterpret_cast<uv::Timer *>(handle->data);
			timer.m_startHandler(timer);
		}, timeout, repeat);
	}

	int Timer::stop()
	{
		return uv_timer_stop(&m_handle);
	}

	int Timer::again()
	{
		return uv_timer_again(&m_handle);
	}

	void Timer::setRepeat(uint64_t repeat)
	{
		uv_timer_set_repeat(&m_handle, repeat);
	}

	uint64_t Timer::getRepeat() const
	{
		return uv_timer_get_repeat(&m_handle);
	}
}

#endif //UVNET_TIMER_H
