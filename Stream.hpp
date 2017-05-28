#ifndef UV_STREAM_HPP
#define UV_STREAM_HPP

#include <functional>
#include <algorithm>
#include <array>

#include <uv.h>

#include "Write.hpp"
#include "Handle.hpp"
#include "Buffer.hpp"
#include "Shutdown.hpp"

namespace uv
{
	template<typename T>
	class Stream : public Handle<T>
	{
	public:
		inline int		listen(std::function<void(uv::Stream<T> &)> handler, int backlog=SOMAXCONN);
		inline int		accept(uv::Stream<T> &client);
		inline int		readStart(std::function<void(uv::Stream<T> &)> handler);
		inline int		readStop();
		inline int		write(char *p, ssize_t len);
		inline int		shutdown(std::function<void(uv::Stream<T> &)> handler);
		inline int		isReadable();
		inline int		isWritable();
		inline int		setBlocking(int blocking=true);
		
		inline ssize_t	length() const;
		inline char		*data();
		inline void		use(ssize_t len);
	private:
		inline int		write();
		inline void		arrangeRecvBuf();

	protected:
		using Handle<T>::m_handle;
		using Handle<T>::m_closeHandler;
	private:
		uv::Write		m_write;
		uv::Buffer		m_sendbuf;

		static const ssize_t			PAGE_SIZE = 64 * 1024;
		std::array<char, PAGE_SIZE>	m_recvbuf;
		ssize_t			m_recvBytes	= 0;
		ssize_t			m_useBytes	= 0;
	private:
		std::function<void(uv::Stream<T> &)> m_listenHandler	= [](uv::Stream<T> &) {};
		std::function<void(uv::Stream<T> &)> m_shutdownHandler= [](uv::Stream<T> &) {};
		std::function<void(uv::Stream<T> &)> m_readHandler	= [](uv::Stream<T> &) {};
	};






	template<typename T>
	ssize_t Stream<T>::length() const
	{
		return m_recvBytes - m_useBytes;
	}

	template<typename T>
	char *Stream<T>::data()
	{
		return &m_recvbuf[m_useBytes];
	}

	template<typename T>
	void Stream<T>::use(ssize_t len)
	{
		m_useBytes += len;
	}

	template<typename T>
	int Stream<T>::listen(std::function<void(uv::Stream<T> &)> handler,int backlog)
	{
		m_listenHandler = handler;
		return uv_listen(reinterpret_cast<uv_stream_t *>(&m_handle), backlog, [](uv_stream_t *st, int status) {
			if (status) return;
			auto &stream = *reinterpret_cast<uv::Stream<T> *>(st->data);
			stream.m_listenHandler(stream);
		});
	}

	template<typename T>
	int Stream<T>::accept(uv::Stream<T> &client)
	{
		return uv_accept(reinterpret_cast<uv_stream_t *>(&m_handle),
			reinterpret_cast<uv_stream_t *>(&(client.m_handle)));
	}

	template<typename T>
	int Stream<T>::readStart(std::function<void(uv::Stream<T> &)> handler)
	{
		m_readHandler = handler;

		return uv_read_start(reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_handle_t* handle, size_t suggested_size, uv_buf_t *buff)
		{
			auto &stream = *reinterpret_cast<Stream<T> *>(handle->data);
			buff->base = stream.m_recvbuf.data() + stream.m_recvBytes;
			buff->len = stream.PAGE_SIZE - stream.m_recvBytes;
		},
			[](uv_stream_t *handle, ssize_t nread, const uv_buf_t *buff)
		{
			auto &stream = *reinterpret_cast<Stream<T> *>(handle->data);

			if (nread < 0) {
				//stream.m_closeHandler(stream);
				return;
			}				
			if (nread <= 0) return;

			stream.m_recvBytes += nread;
			stream.m_readHandler(stream);
			stream.arrangeRecvBuf();
		});
	}

	template<typename T>
	int Stream<T>::readStop()
	{
		return uv_read_stop(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	int Stream<T>::write(char *p, ssize_t len)
	{
		auto status = m_sendbuf.add(p, len);
		if (status) return status;

		return this->write();
	}

	template<typename T>
	int Stream<T>::write()
	{
		auto buff = m_sendbuf.get();
		if (std::get<1>(buff) == 0) {
			return 0;
		}
		return uv_write(&m_write.m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			std::get<0>(buff), std::get<1>(buff), [](uv_write_t *req, int status) {

			if (status == 0) {
				auto &stream = *reinterpret_cast<Stream<T> *>(req->handle->data);

				stream.m_sendbuf.used();

				stream.write();
			}
		});
	}

	template<typename T>
	void Stream<T>::arrangeRecvBuf()
	{
		if (m_useBytes == 0) return;
		if (m_useBytes == m_recvBytes)
		{
			m_useBytes = 0;
			m_recvBytes = 0;
		}
		std::reverse_copy(std::begin(m_recvbuf)+m_useBytes, std::begin(m_recvbuf) + m_recvBytes, std::begin(m_recvbuf));
	}

	template<typename T>
	int Stream<T>::shutdown(std::function<void(uv::Stream<T> &)> handler)
	{
		m_shutdownHandler = handler;

		auto req = new (std::nothrow) Shutdown;
		if (req == nullptr) return ENOMEM;

		return uv_shutdown(&req->m_handle, reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_shutdown_t *req, int status) {
			if (!status) {
				auto &stream = *reinterpret_cast<uv::Stream<T> *>(req->handle->data);
				stream.m_shutdownHandler(stream);
			}
			delete reinterpret_cast<Shutdown *>(req->data);
		});
	}

	template<typename T>
	int Stream<T>::isReadable()
	{
		return uv_is_readable(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	int Stream<T>::isWritable()
	{
		return uv_is_writable(reinterpret_cast<uv_stream_t *>(&m_handle));
	}

	template<typename T>
	int Stream<T>::setBlocking(int blocking)
	{
		return uv_stream_set_blocking(reinterpret_cast<uv_stream_t *>(&m_handle), blocking);
	}
}


#endif
