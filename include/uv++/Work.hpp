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
		Work(std::function<void()> wh, std::function<void(const Error &error)> ah);
		void			queue(uv::Loop &loop, uv::Error &err);
		void			queue(uv::Loop &loop);

	private:
		std::function<void()>		m_workHandler	= []() {};
		std::function<void(const Error &error)>	m_afterWorkHandler = [](const Error &error) {};
	};





	inline Work::Work(std::function<void()> wh, std::function<void(const Error &error)> ah)
	{
		m_handle.data = this;
		m_workHandler = wh;
		m_afterWorkHandler = ah;
	}

	inline void Work::queue(uv::Loop &loop, uv::Error &err)
	{
		err.m_error = uv_queue_work(loop.m_loop_ptr, &m_handle,
			[](uv_work_t *r) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_workHandler();
			}, 
			[](uv_work_t *r,int status) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_afterWorkHandler(Error(status));
		});
	}

	inline void Work::queue(uv::Loop &loop)
	{
		uv::Error err;

		queue(loop, err);
		if (err) {
			throw uv::Exception(err);
		}
	}
}


#endif
