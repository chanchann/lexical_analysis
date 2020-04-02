#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <iostream>

//创建单级目录
int main()
{
	std::string prefix = "./test1";
	if (access(prefix.c_str(), F_OK) == -1){//如果文件夹不存在
        std::cout << "Directory dosen't exist " << std::endl;
        //创建
        mkdir(prefix.c_str(), 0777);
    }else{
        std::cout << "This directory exists" << std::endl;
    }
					
}