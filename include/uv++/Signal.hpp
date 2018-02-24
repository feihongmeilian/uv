#ifndef UV_SIGNAL_HPP
#define UV_SIGNAL_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Handle.hpp"

namespace uv
{
    class Signal : public Handle<uv_signal_t>
    {
    public:
		Signal();

		void			init(uv::Loop &loop, std::error_code &ec);
		void			init(uv::Loop &loop);
		void			start(const std::function<void(int signum)> &handler, int sigNum, std::error_code &ec);
		void			start(const std::function<void(int signum)> &handler, int sigNum);
		void			stop(std::error_code &ec);
		void			stop();

	private:
        std::function<void(int signum)>	m_startHandler = [](int signum) {};
    };





	inline Signal::Signal()
	{
		m_handle.data = this;
	}

	inline void Signal::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_signal_init(loop.value(), &m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Signal::init(uv::Loop &loop)
	{
		std::error_code ec;

		init(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Signal::start(const std::function<void(int signum)> &handler, int sigNum, std::error_code &ec)
	{
		m_startHandler = handler;
		auto status = uv_signal_start(&m_handle, [](uv_signal_t *handle, int num) {
			auto &signal = *reinterpret_cast<uv::Signal *>(handle->data);
			signal.m_startHandler(num);
		}, sigNum);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Signal::start(const std::function<void(int signum)> &handler, int sigNum)
	{
		std::error_code ec;

		start(handler, sigNum, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Signal::stop(std::error_code &ec)
	{
		auto status = uv_signal_stop(&m_handle);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Signal::stop()
	{
		std::error_code ec;

		stop(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}

#endif
