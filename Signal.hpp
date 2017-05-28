#ifndef UV_SIGNAL_HPP
#define UV_SIGNAL_HPP

#include <functional>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
    class Signal : public Noncopyable
    {
    public:
		inline explicit	Signal(uv::Loop &);

    public:
		inline int		start(std::function<void (uv::Signal &, int)> handler, int sigNum);
		inline int		stop();

    private:
        uv_signal_t		m_handle;

	private:
        std::function<void (uv::Signal &, int)>	m_startHandler = [](uv::Signal &, int) {};
    };





	Signal::Signal(uv::Loop &loop)
	{
		uv_signal_init(&loop.m_loop, &m_handle);
		m_handle.data = this;
	}

	int Signal::start(std::function<void(uv::Signal &, int)> handler, int sigNum)
	{
		m_startHandler = handler;
		return uv_signal_start(&m_handle, [](uv_signal_t *handle, int num)
		{
			auto &signal = *reinterpret_cast<uv::Signal *>(handle->data);
			signal.m_startHandler(signal, num);
		}, sigNum);
	}

	int Signal::stop()
	{
		return uv_signal_stop(&m_handle);
	}
}

#endif //UVNET_SIGNAL_H
