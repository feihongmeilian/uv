#ifndef UV_TTY_HPP
#define UV_TTY_HPP

#include <uv.h>

#include "Loop.hpp"
#include "Stream.hpp"

namespace uv
{
	class Tty : public Stream<uv_tty_t>
	{
	public:
		inline			Tty(uv::Loop &loop, uv_file fd, int readable);
		inline int		setMode(uv_tty_mode_t mode);
		inline int		getWinsize(int &width, int &height);

		static inline int	resetMode();
	};





	Tty::Tty(uv::Loop &loop, uv_file fd, int readable)
	{
		m_handle.data = this;
		uv_tty_init(&loop.m_loop, &m_handle, fd, readable);
	}

	int	Tty::setMode(uv_tty_mode_t mode)
	{
		return uv_tty_set_mode(&m_handle, mode);
	}

	int	Tty::resetMode()
	{
		return uv_tty_reset_mode();
	}

	int	Tty::getWinsize(int &width, int &height)
	{
		return uv_tty_get_winsize(&m_handle, &width, &height);
	}
}


#endif
