#ifndef UV_UTILITY_HPP
#define UV_UTILITY_HPP

#include <string>

#include <uv.h>

#include "Error.hpp"

namespace uv
{
	class Utility
	{
	public:
		static void	GetExePath(std::string &path, std::error_code &ec);
		static void	GetExePath(std::string &path);
		static void	GetExeDirectory(std::string &path, std::error_code &ec);
		static void	GetExeDirectory(std::string &path);
		static void	GetCurrentWorkingDirectory(std::string &path, std::error_code& ec);
		static void	GetCurrentWorkingDirectory(std::string &path);
		static void	GetHomeDirectory(std::string &home, std::error_code &ec);
		static void	GetHomeDirectory(std::string &home);
		static void	ChangeDirectory(const std::string &path, std::error_code &ec);
		static void	ChangeDirectory(const std::string &path);
	};


	



	inline void Utility::GetExePath(std::string &path, std::error_code &ec)
	{
		char buffer[MAX_PATH];
		size_t size = sizeof(buffer);

		auto status = uv_exepath(buffer, &size);
		path.clear();
		path += buffer;
		
		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Utility::GetExePath(std::string &path)
	{
		std::error_code ec;

		GetExePath(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::GetExeDirectory(std::string &path, std::error_code &ec)
	{
		GetExePath(path, ec);
		if (ec) return;
		
		auto index = path.rfind('/');
		if (index == std::string::npos)
		{
			index = path.rfind('\\');
			if (index == std::string::npos)
			{
				return;
			}
		}

		path.erase(index);
	}

	inline void Utility::GetExeDirectory(std::string &path)
	{
		std::error_code ec;

		GetExeDirectory(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::GetCurrentWorkingDirectory(std::string &path, std::error_code &ec)
	{
		char buffer[MAX_PATH];
		size_t size = sizeof(buffer);

		auto status = uv_cwd(buffer, &size);
		path.clear();
		path += buffer;

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Utility::GetCurrentWorkingDirectory(std::string &path)
	{
		std::error_code ec;

		GetCurrentWorkingDirectory(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::ChangeDirectory(const std::string &path, std::error_code &ec)
	{
		auto status = uv_chdir(path.c_str());

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Utility::ChangeDirectory(const std::string &path)
	{
		std::error_code ec;

		ChangeDirectory(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::GetHomeDirectory(std::string &home, std::error_code &ec)
	{
		char buffer[MAX_PATH];
		size_t size = sizeof(buffer);

		auto status = uv_os_homedir(buffer, &size);
		home.clear();
		home += buffer;

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Utility::GetHomeDirectory(std::string &home)
	{
		std::error_code ec;

		GetHomeDirectory(home, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

}

#endif
