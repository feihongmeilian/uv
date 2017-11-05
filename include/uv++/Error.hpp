#ifndef UV_ERROR_HPP
#define UV_ERROR_HPP

#include <string>
#include <system_error>

namespace uv
{
	class Category : public std::error_category
	{
	public:
		Category() = default;

		char const		*name() const noexcept {
			return "uv";
		}

		std::string		message(int value) const {
			return uv_strerror(value);
		}
	};

	inline const std::error_category& getCategory() {
		static Category instance;
		return instance;
	}

	inline std::error_code makeErrorCode(int e) {
		return std::error_code(e, getCategory());
	}
}


#endif
