#ifndef UV_HANDLE_HPP
#define UV_HANDLE_HPP

#include <functional>

#include <uv.h>

#include "Loop.hpp"
#include "Noncopyable.hpp"

namespace uv
{
	template<typename T>
	class Handle : public Noncopyable
	{
	public:
		bool			isActive();
		bool			isClosing();
		void			ref();
		void			unref();
		bool			hasRef();
		uv::Loop		&loop();
		void			close(const std::function<void()> &handler);

	protected:
		T			m_handle;

		std::function<void()> m_closeHandler = []() {};
	};
	




	template<typename T>
	inline bool Handle<T>::isActive()
	{
		return uv_is_active(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	inline bool Handle<T>::isClosing()
	{
		return uv_is_closing(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	inline void Handle<T>::ref()
	{
		uv_ref(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	inline void Handle<T>::unref()
	{
		uv_unref(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	inline bool Handle<T>::hasRef()
	{
		return uv_has_ref(reinterpret_cast<uv_handle_t *>(&m_handle));
	}

	template<typename T>
	inline uv::Loop &Handle<T>::loop()
	{
		return *reinterpret_cast<uv::Loop *>(
			reinterpret_cast<uv_handle_t *>(&m_handle)->loop->data);
	}

	template<typename T>
	inline void Handle<T>::close(const std::function<void()> &handler)
	{
		m_closeHandler = handler;
		uv_close(reinterpret_cast<uv_handle_t *>(&m_handle), [](uv_handle_t *h) {
			auto &handle = *reinterpret_cast<uv::Handle<T> *>(h->data);
			handle.m_closeHandler();
		});
	}
}


#endif