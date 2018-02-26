#ifndef UV_UTILITY_HPP
#define UV_UTILITY_HPP

#include <string>

#if !(defined(WIN32) || defined(WIN64))
#define MAX_PATH PATH_MAX
#endif

#include <uv.h>

#include "Error.hpp"

namespace uv
{
	class Utility
	{
	public:
		static void	getExePath(std::string &path, std::error_code &ec);
		static void	getExePath(std::string &path);
		static void	getExeDirectory(std::string &path, std::error_code &ec);
		static void	getExeDirectory(std::string &path);
		static void	getCurrentWorkingDirectory(std::string &path, std::error_code& ec);
		static void	getCurrentWorkingDirectory(std::string &path);
		static void	getHomeDirectory(std::string &home, std::error_code &ec);
		static void	getHomeDirectory(std::string &home);
		static void	changeDirectory(const std::string &path, std::error_code &ec);
		static void	changeDirectory(const std::string &path);
	};


	



	inline void Utility::getExePath(std::string &path, std::error_code &ec)
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

	inline void Utility::getExePath(std::string &path)
	{
		std::error_code ec;

		getExePath(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::getExeDirectory(std::string &path, std::error_code &ec)
	{
		getExePath(path, ec);
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

	inline void Utility::getExeDirectory(std::string &path)
	{
		std::error_code ec;

		getExeDirectory(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::getCurrentWorkingDirectory(std::string &path, std::error_code &ec)
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

	inline void Utility::getCurrentWorkingDirectory(std::string &path)
	{
		std::error_code ec;

		getCurrentWorkingDirectory(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::changeDirectory(const std::string &path, std::error_code &ec)
	{
		auto status = uv_chdir(path.c_str());

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Utility::changeDirectory(const std::string &path)
	{
		std::error_code ec;

		changeDirectory(path, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

	inline void Utility::getHomeDirectory(std::string &home, std::error_code &ec)
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

	inline void Utility::getHomeDirectory(std::string &home)
	{
		std::error_code ec;

		getHomeDirectory(home, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}

}

#endif
