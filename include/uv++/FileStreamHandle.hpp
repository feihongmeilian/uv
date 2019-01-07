#ifndef UV_FILE_STREAhandle__HPP
#define UV_FILE_STREAhandle__HPP

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Stream.hpp"

namespace uv
{
	template<typename T>
	class FileStreamHandle : public Stream<T>
	{
	public:
		void			sendBufferSize(int &value, std::error_code &ec);
		void			sendBufferSize(int &value);
		void			recvBufferSize(int &value, std::error_code &ec);
		void			recvBufferSize(int &value);

    protected:
        using Handle<T>::handle_;
	};




	template<typename T>
	inline void FileStreamHandle<T>::sendBufferSize(int &value, std::error_code &ec)
	{
		auto status = uv_send_buffer_size(reinterpret_cast<uv_handle_t *>(&handle_), &value);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void FileStreamHandle<T>::sendBufferSize(int &value)
	{
		std::error_code ec;

		sendBufferSize(value, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	template<typename T>
	inline void FileStreamHandle<T>::recvBufferSize(int &value, std::error_code &ec)
	{
		auto status = uv_recv_buffer_size(reinterpret_cast<uv_handle_t *>(&handle_), &value);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void FileStreamHandle<T>::recvBufferSize(int &value)
	{
		std::error_code ec;

		recvBufferSize(value, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

}
#endif
