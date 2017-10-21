#include<iostream>
#include<string>
#include<uv>

int main()
{
	uv::Loop loop;

	uv::Tcp tcp(loop);

	try {
		tcp.connect("127.0.0.1", 13, [&tcp](const uv::Error &error) {
			if (error) {
				std::cout << "链接失败" << std::endl;
			}
			else {
				try {
					tcp.readStart([](char *buf, ssize_t len) {
						if (len > 0)
							std::cout << std::string(buf, len) << std::endl;
					});
				}
				catch (const uv::Exception &ex) {
					std::cout << ex.what() << std::endl;
				}
			}
		});

		loop.run();
	}
	catch (const uv::Exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}