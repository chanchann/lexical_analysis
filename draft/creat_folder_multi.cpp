#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <iostream>

//创建多级目录
int main()
{
	std::string prefix = "./test1/test2/test.c";
    int len = prefix.length();
    char tmpDirPath[256] = { 0 };
    for(int i = 0 ; i < len; i++){
        tmpDirPath[i] = prefix[i];
        if(tmpDirPath[i] == '/'){
            if (access(tmpDirPath, F_OK) == -1){
                std::cout << "Directory dosen't exist " << std::endl;
                int ret = mkdir(tmpDirPath, 0777);
                if(ret == -1){
                    std::cout << " Create direcotory error " << std::endl;
                    return ret;
                }
            }
        }
    }
    return 0;
}
