#ifndef UV_HANDLE_HPP
#define UV_HANDLE_HPP

#include <functional>

#include <uv.h>
#include "Noncopyable.hpp"

namespace uv
{
	template<typename T>
	class Handle : public Noncopyable
	{
	public:
		inline int		isActive();
		inline int		isClosing();
		inline void		ref();
		inline void		unref();
		inline int		hasRef();
		inline int		sendBufferSize(int &value);
		inline int		recvBufferSize(int &value);
		inline int		fileno(uv_os_fd_t &fd);
		inline void		close(std::function<void(uv::Handle<T> &)> handler);
	protected:
		T				m_handle;

		std::function<void(uv::Handle<T> &)> m_closeHandler = nullptr;
	};
	




	template<typename T>
	int Handle<T>::isActive()
	{
		return uv_is_active(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	int Handle<T>::isClosing()
	{
		return uv_is_closing(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	void Handle<T>::ref()
	{
		uv_ref(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	void Handle<T>::unref()
	{
		uv_unref(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	int Handle<T>::hasRef()
	{
		return uv_has_ref(reinterpret_cast<uv_handle_t *>(&m_handle));
	}


	template<typename T>
	int Handle<T>::sendBufferSize(int &value)
	{
		return uv_send_buffer_size(reinterpret_cast<uv_handle_t *>(&m_handle), &value);
	}

	template<typename T>
	int Handle<T>::recvBufferSize(int &value)
	{
		return uv_recv_buffer_size(reinterpret_cast<uv_handle_t *>(&m_handle), &value);
	}

	template<typename T>
	int Handle<T>::fileno(uv_os_fd_t &fd)
	{
		return uv_fileno(reinterpret_cast<uv_handle_t *>(&m_handle), &fd);
	}

	template<typename T>
	void Handle<T>::close(std::function<void(uv::Handle<T> &)> handler)
	{
		m_closeHandler = handler;
		uv_close(reinterpret_cast<uv_handle_t *>(&m_handle), [](uv_handle_t *h) {
			auto &handle = *reinterpret_cast<uv::Handle<T> *>(h->data);
			if(handle.m_closeHandler != nullptr)
				handle.m_closeHandler(handle);
		});
	}
}


#endif