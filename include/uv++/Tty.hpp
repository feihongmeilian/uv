#ifndef UV_TTY_HPP
#define UV_TTY_HPP

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Loop.hpp"
#include "Stream.hpp"

namespace uv
{
	class Tty : public Stream<uv_tty_t>
	{
	public:
		Tty();

		void		init(uv::Loop &loop, uv_file fd, int readable, std::error_code &ec);
		void		init(uv::Loop &loop, uv_file fd, int readable);
		void		setMode(uv_tty_mode_t mode, std::error_code &ec);
		void		setMode(uv_tty_mode_t mode);
		void		getWinsize(int &width, int &height, std::error_code &ec);
		void		getWinsize(int &width, int &height);

		static void	resetMode(std::error_code &ec);
		static void	resetMode();
	};





	inline Tty::Tty()
	{
		handle_.data = this;
	}


	inline void Tty::init(uv::Loop &loop, uv_file fd, int readable, std::error_code &ec)
	{
		const auto status = uv_tty_init(loop.value(), &handle_, fd, readable);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tty::init(uv::Loop &loop, uv_file fd, int readable)
	{
		std::error_code ec;

		init(loop, fd, readable, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
	inline void Tty::setMode(uv_tty_mode_t mode, std::error_code &ec)
	{
		const auto status = uv_tty_set_mode(&handle_, mode);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tty::setMode(uv_tty_mode_t mode)
	{
		std::error_code ec;

		setMode(mode, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Tty::resetMode(std::error_code &ec)
	{
		const auto status = uv_tty_reset_mode();

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tty::resetMode()
	{
		std::error_code ec;

		resetMode(ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Tty::getWinsize(int &width, int &height, std::error_code &ec)
	{
		const auto status = uv_tty_get_winsize(&handle_, &width, &height);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Tty::getWinsize(int & width, int & height)
	{
		std::error_code ec;

		getWinsize(width, height, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
