#ifndef UV_SIGNAL_HPP
#define UV_SIGNAL_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
    class Signal : public Noncopyable
    {
    public:
		explicit		Signal(uv::Loop &);

    public:
		void			start(std::function<void(int signum)> handler, int sigNum, uv::Error &err);
		void			start(std::function<void(int signum)> handler, int sigNum);
		void			stop(uv::Error &err);
		void			stop();

    private:
        uv_signal_t		m_handle;

	private:
        std::function<void(int signum)>	m_startHandler = [](int signum) {};
    };





	inline Signal::Signal(uv::Loop &loop)
	{
		uv_signal_init(loop.m_loop_ptr, &m_handle);
		m_handle.data = this;
	}

	inline void Signal::start(std::function<void(int signum)> handler, int sigNum, uv::Error &err)
	{
		m_startHandler = handler;
		err.m_error = uv_signal_start(&m_handle, [](uv_signal_t *handle, int num) {
			auto &signal = *reinterpret_cast<uv::Signal *>(handle->data);
			signal.m_startHandler(num);
		}, sigNum);
	}

	inline void Signal::start(std::function<void(int signum)> handler, int sigNum)
	{
		uv::Error err;

		start(handler, sigNum, err);
		if (err) {
			throw uv::Exception(err);
		}
	}

	inline void Signal::stop(uv::Error &err)
	{
		err.m_error = uv_signal_stop(&m_handle);
	}

	inline void Signal::stop()
	{
		uv::Error err;

		stop(err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}

#endif
