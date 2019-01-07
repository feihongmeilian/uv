#ifndef UV_DYNAMIC_LIBRARY_HPP
#define UV_DYNAMIC_LIBRARY_HPP

#include <string>
#include <system_error>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"

namespace uv
{
	class DynamicLibrary
	{
	public:
		void			open(const std::string &filename, std::error_code &ec);
		void			open(const std::string &filename);
		void			sym(const std::string &name, void** ptr, std::error_code &ec);
		void			sym(const std::string &name, void** ptr);
		void			close();

	private:
		uv_lib_t		lib_;
	};





	inline void DynamicLibrary::open(const std::string &filename, std::error_code &ec)
	{
		auto status = uv_dlopen(filename.c_str(), &lib_);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void DynamicLibrary::open(const std::string &filename)
	{
		std::error_code ec;

		open(filename, ec);
		if (ec) {
			throw uv::Exception(uv_dlerror(&lib_));
		}
	}

	inline void DynamicLibrary::sym(const std::string &name, void **ptr, std::error_code &ec)
	{
		auto status = uv_dlsym(&lib_, name.c_str(), ptr);

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void DynamicLibrary::sym(const std::string &name, void **ptr)
	{
		std::error_code ec;

		sym(name, ptr, ec);
		if (ec) {
			throw uv::Exception(uv_dlerror(&lib_));
		}
	}

	inline void DynamicLibrary::close()
	{
		uv_dlclose(&lib_);
	}



}
#endif
