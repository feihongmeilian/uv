#ifndef UV_BUFFER_HPP
#define UV_BUFFER_HPP

#include <tuple>

#include <uv.h>

#include "Noncopyable.hpp"

namespace uv
{
	//专供Stream<T>使用的buffer
	class Buffer : public Noncopyable
	{
	public:
		inline explicit	Buffer(uint16_t length = 50);
		inline ~Buffer();

		/*********************************************
		*将一块new的字节数组添加到缓存
		**********************************************/
		inline int		add(char *p, ssize_t len);
		inline void		used();
		inline std::tuple<uv_buf_t *, uint16_t> get();

	private:
		uint16_t			m_length;
		uint16_t			m_writeIndex;
		uint16_t			m_writeLength;
		uv_buf_t			*m_buf[2];
	};





	Buffer::Buffer(uint16_t length)
	{
		m_buf[0]		= new (std::nothrow) uv_buf_t[length];
		m_buf[1]		= new (std::nothrow) uv_buf_t[length];
		m_length		= length;
		m_writeIndex	= 0;
		m_writeLength	= 0;
	}

	Buffer::~Buffer()
	{
		used();
		delete []m_buf[0];
		delete []m_buf[1];
	}

	int Buffer::add(char *p, ssize_t len)
	{
		if (m_writeIndex == m_length) return ENOBUFS;

		m_buf[0][m_writeIndex].base = p;
		m_buf[0][m_writeIndex].len = len;
		++m_writeIndex;

		return 0;
	}


	std::tuple<uv_buf_t *, uint16_t> Buffer::get()
	{
		if (m_writeIndex == 0 || m_writeLength) return std::make_tuple(nullptr, 0);

		auto buf = m_buf[0];
		m_buf[0] = m_buf[1];
		m_buf[1] = buf;
		m_writeLength = m_writeIndex;
		m_writeIndex = 0;
		return std::make_tuple(m_buf[1], m_writeLength);
	}

	void Buffer::used()
	{
		if (m_writeLength == 0) return;
		for (uint16_t i=0; i<m_writeLength; ++i) {
			free(m_buf[1][i].base);
			m_buf[1][i].base = nullptr;
		}
		m_writeLength = 0;
	}
}

#endif