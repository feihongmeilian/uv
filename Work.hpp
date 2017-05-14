#ifndef UV_WORK_HPP
#define UV_WORK_HPP

#include <functional>

#include <uv.h>

#include "Req.hpp"
#include "Loop.hpp"

namespace uv
{
	class Work : public Req<uv_work_t>
	{
	public:
		inline Work(std::function<void(Work &)> wh, std::function<void(uv::Work &, int)> ah);
		inline int		queue(uv::Loop &loop);

	private:
		std::function<void(uv::Work &)>			m_workHandler		= nullptr;
		std::function<void(uv::Work &, int)>	m_afterWorkHandler= nullptr;
	};





	Work::Work(std::function<void(uv::Work &)> wh, std::function<void(uv::Work &, int)> ah)
	{
		m_handle.data = this;
		m_workHandler = wh;
		m_afterWorkHandler = ah;
	}

	int Work::queue(uv::Loop &loop)
	{
		return uv_queue_work(&loop.m_loop, &m_handle,
			[](uv_work_t *r) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_workHandler(req);
			}, 
			[](uv_work_t *r,int status) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_afterWorkHandler(req, status);
		});
	}
}


#endif
