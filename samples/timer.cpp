/*************************************************************************
	> File Name: timer.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年07月19日 星期三 11时36分34秒
 ************************************************************************/

#include<iostream>
#include<uv>
using namespace std;

int main()
{
    uv::Loop loop;

    uv::Timer timer(loop);
    timer.start([](){
        std::cout << "Hello, World" << std::endl;
    }, 1000, 1000);

    loop.run();
}
