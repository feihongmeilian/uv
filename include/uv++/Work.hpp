#ifndef UV_WORK_HPP
#define UV_WORK_HPP

#include <functional>

#include <uv.h>

#include "Error.hpp"
#include "Exception.hpp"
#include "Req.hpp"
#include "Loop.hpp"

namespace uv
{
	class Work : public Req<uv_work_t>
	{
	public:
		Work(const std::function<void()> &wh, const std::function<void(const std::error_code &ec)> &ah);
		void			queue(uv::Loop &loop, std::error_code &ec);
		void			queue(uv::Loop &loop);

	private:
		std::function<void()>		m_workHandler	= []() {};
		std::function<void(const std::error_code &ec)>	m_afterWorkHandler = [](const std::error_code &ec) {};
	};





	inline Work::Work(const std::function<void()> &wh, const std::function<void(const std::error_code &ec)> &ah)
	{
		m_handle.data = this;
		m_workHandler = wh;
		m_afterWorkHandler = ah;
	}

	inline void Work::queue(uv::Loop &loop, std::error_code &ec)
	{
		auto status = uv_queue_work(loop.value(), &m_handle,
			[](uv_work_t *r) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_workHandler();
			}, 
			[](uv_work_t *r,int status) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_afterWorkHandler(makeErrorCode(status));
		});

		if (status != 0) {
			ec = makeErrorCode(status);
		}
	}

	inline void Work::queue(uv::Loop &loop)
	{
		std::error_code ec;

		queue(loop, ec);
		if (ec) {
			throw uv::Exception(ec);
		}
	}
}


#endif
