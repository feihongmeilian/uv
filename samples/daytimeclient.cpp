/*************************************************************************
	> File Name: daytimeclient.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年07月19日 星期三 13时08分37秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<uv>

int main()
{
    uv::Loop loop;

    uv::Tcp tcp(loop);
    tcp.connect("127.0.0.1",13, [&tcp](int status){
        if (status) {
            std::cout << "链接失败" << std::endl;
        }
        else {
            tcp.readStart([](char *buf, ssize_t len){
                if (len > 0)
                    std::cout << std::string(buf, len) << std::endl;
            });
        }
    });

    loop.run();
}
