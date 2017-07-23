#ifndef UV_STREAM_HPP
#define UV_STREAM_HPP

#include <functional>
#include <algorithm>
#include <vector>

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
		explicit inline	Stream(size_t buf_size = 64*1024);

		inline int		listen(std::function<void(uv::Stream<T> &)> handler, int backlog=SOMAXCONN);
		inline int		accept(uv::Stream<T> &client);
		inline int		readStart(std::function<void(uv::Stream<T> &, char *data, int len)> handler);
		inline void		read(size_t bytes);
		inline int		readStop();
		inline int		write(char *p, ssize_t len);
		inline int		shutdown(std::function<void(uv::Stream<T> &)> handler);
		inline int		isReadable();
		inline int		isWritable();
		inline int		setBlocking(int blocking=true);
		

		inline ssize_t	length() const;
		inline char		*data();
		inline void		use();

	private:
		inline int		write();

	protected:
		using Handle<T>::m_handle;
		using Handle<T>::m_closeHandler;
	private:
		uv::Write		m_write;
		uv::Buffer		m_sendbuf;
		std::vector<char>	m_recvbuf;
		char				*m_head;
		char				*m_tail;
		size_t			m_needBytes	= 0;
	private:
		std::function<void(uv::Stream<T> &)> m_listenHandler	= [](uv::Stream<T> &) {};
		std::function<void(uv::Stream<T> &)> m_shutdownHandler= [](uv::Stream<T> &) {};
		std::function<void(uv::Stream<T> &, char *data, int len)> m_readHandler = [](uv::Stream<T> &, char *data, int len) {};
	};






	template<typename T>
	Stream<T>::Stream(size_t buf_size)
		: m_recvbuf(buf_size)
	{
		m_head = m_recvbuf.data();
		m_tail = m_recvbuf.data();
	}


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
	void Stream<T>::use()
	{
		if (m_tail - m_head < m_needBytes) return;
		char *head = m_head;
		m_head += m_needBytes;

		m_readHandler(*this, head, m_needBytes);

		if (m_head == m_recvbuf.data()) return;

		if (m_head == m_tail)	{
			m_head = m_tail = m_recvbuf.data();
			return;
		}

		memmove(m_recvbuf.data(), m_head, m_tail - m_head);
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
	int Stream<T>::readStart(std::function<void(uv::Stream<T> &, char *data, int len)> handler)
	{
		m_readHandler = handler;
		return uv_read_start(reinterpret_cast<uv_stream_t *>(&m_handle),
			[](uv_handle_t* handle, size_t suggested_size, uv_buf_t *buff)
		{
			auto &stream = *reinterpret_cast<Stream<T> *>(handle->data);
			buff->base = stream.m_tail;
			buff->len = stream.m_recvbuf.size() - (stream.m_tail - stream.m_head);
		},
			[](uv_stream_t *handle, ssize_t nread, const uv_buf_t *buff)
		{
			auto &stream = *reinterpret_cast<Stream<T> *>(handle->data);

			if (nread < 0) {
				//stream.m_closeHandler(stream);
				return;
			}				
			if (nread == 0) return;

			stream.m_tail += nread;

			stream.use();
		});
	}

	template<typename T>
	inline void Stream<T>::read(size_t bytes)
	{
		m_needBytes = bytes;
		use();
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
