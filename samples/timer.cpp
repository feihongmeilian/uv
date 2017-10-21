#include <iostream>
#include <uv>

int main()
{
	uv::Loop loop;
	uv::Timer timer(loop);

	try {
		timer.start([]() {
			std::cout << "Hello,World" << std::endl;
		}, 1000, 1000);

		loop.run();
	}
	catch (const uv::Exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

