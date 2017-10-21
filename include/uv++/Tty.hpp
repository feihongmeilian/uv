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
					Tty(uv::Loop &loop, uv_file fd, int readable);
		void			setMode(uv_tty_mode_t mode, uv::Error &er);
		void			setMode(uv_tty_mode_t mode);
		void			getWinsize(int &width, int &height, uv::Error &er);
		void			getWinsize(int &width, int &height);

		static void	resetMode(uv::Error &er);
		static void	resetMode();
	};





	inline Tty::Tty(uv::Loop &loop, uv_file fd, int readable)
	{
		m_handle.data = this;
		uv_tty_init(loop.m_loop_ptr, &m_handle, fd, readable);
	}

	inline void Tty::setMode(uv_tty_mode_t mode, uv::Error &er)
	{
		er.m_error = uv_tty_set_mode(&m_handle, mode);
	}

	inline void Tty::setMode(uv_tty_mode_t mode)
	{
		uv::Error er;
		setMode(mode, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}

	inline void Tty::resetMode(uv::Error &er)
	{
		er.m_error = uv_tty_reset_mode();
	}

	inline void Tty::resetMode()
	{
		uv::Error er;
		resetMode(er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}

	inline void Tty::getWinsize(int &width, int &height, uv::Error &er)
	{
		er.m_error = uv_tty_get_winsize(&m_handle, &width, &height);
	}

	inline void Tty::getWinsize(int & width, int & height)
	{
		uv::Error er;
		getWinsize(width, height, er);

		if (er)
		{
			throw uv::Exception(er);
		}
	}
}


#endif
