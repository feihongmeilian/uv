#include<iostream>
#include<uv>

int main()
{
	uv::Loop loop;
	uv::Signal signal;

	try {
		loop.init();
		signal.init(loop);

		signal.start([](int signum) {
			std::cout << "Ctrl+C中断,信号量值为:" << signum << std::endl;
		}, SIGINT);

		loop.run();
	}
	catch (const uv::Exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}