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
		inline Work(std::function<void()> wh, std::function<void(int)> ah);
		inline int		queue(uv::Loop &loop);

	private:
		std::function<void()>				m_workHandler		= []() {};
		std::function<void(int status)>	m_afterWorkHandler= [](int status) {};
	};





	Work::Work(std::function<void()> wh, std::function<void(int)> ah)
	{
		m_handle.data = this;
		m_workHandler = wh;
		m_afterWorkHandler = ah;
	}

	int Work::queue(uv::Loop &loop)
	{
		return uv_queue_work(loop.m_loop_ptr, &m_handle,
			[](uv_work_t *r) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_workHandler();
			}, 
			[](uv_work_t *r,int status) {
				auto &req = *reinterpret_cast<uv::Work *>(r->data);
				req.m_afterWorkHandler(status);
		});
	}
}


#endif
