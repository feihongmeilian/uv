#ifndef UV_FILE_HANDLE_HPP
#define UV_FILE_HANDLE_HPP

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Handle.hpp"

namespace uv
{
	template<typename T>
	class FileHandle : public Handle<T>
	{
	public:
		void			fileno(uv_os_fd_t &fd, std::error_code &ec);
		void			fileno(uv_os_fd_t &fd);

    protected:
        using Handle<T>::handle_;
	};




	template<typename T>
	inline void FileHandle<T>::fileno(uv_os_fd_t &fd, std::error_code &ec)
	{
		auto status = uv_fileno(reinterpret_cast<uv_handle_t *>(&handle_), &fd);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	template<typename T>
	inline void FileHandle<T>::fileno(uv_os_fd_t &fd)
	{
		std::error_code ec;

		fileno(fd, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}
#endif
