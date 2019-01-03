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

		void        init(uv::Loop &loop, std::error_code &ec);
		void        init(uv::Loop &loop);
		void        start(const std::function<void(int signum)> &handler, int sigNum, std::error_code &ec);
		void        start(const std::function<void(int signum)> &handler, int sigNum);
		void        stop(std::error_code &ec);
		void        stop();

	private:
        std::function<void(int signum)>	startHandler_ = [](int signum) {};
    };





	inline Signal::Signal()
	{
		handle_.data = this;
	}

	inline void Signal::init(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_signal_init(loop.value(), &handle_);

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
		startHandler_ = handler;
		auto status = uv_signal_start(&handle_, [](uv_signal_t *handle, int num) {
			auto &signal = *reinterpret_cast<uv::Signal *>(handle->data);
			signal.startHandler_(num);
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
		auto status = uv_signal_stop(&handle_);

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
