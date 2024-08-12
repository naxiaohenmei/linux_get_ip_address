#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_local_ip(char* ip, char* dev)
{

    int fd, intrface, retn = 0;
    struct ifreq buf[INET_ADDRSTRLEN];  //这个结构定义在/usr/include/net/if.h，用来配置和获取ip地址，掩码，MTU等接口信息的
    struct ifconf ifc;

    /*1 建立socket链接，利用ioctl来自动扫描可用网卡*/
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {

        ifc.ifc_len = sizeof(buf);

        // caddr_t,linux内核源码里定义的：typedef void *caddr_t；
        ifc.ifc_buf = (caddr_t)buf;

        if (!ioctl(fd, SIOCGIFCONF, (char*)&ifc))  /*2  这里获取到可用网卡清单，包括网卡ip地址，mac地址*/
        {
            intrface = ifc.ifc_len / sizeof(struct ifreq);  //计算出有效网卡的数量//  
            while (intrface-- > 0)
            {
                if (!(ioctl(fd, SIOCGIFADDR, (char*)&buf[intrface])))  /*3  遍历并索引指定网卡的地址*/
                {
                    if (strcmp(buf[intrface].ifr_ifrn.ifrn_name, dev) == 0)
                    {
                        ip = (inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                        printf("IP:%s\n", ip);
                    }
                }

            }
        }

        close(fd);

        return 0;
    }
}

int main(int agrc, char* agrv[])
{
    char ip[64];
    memset(ip, 0, sizeof(ip));
    get_local_ip(ip, "ens33");

    return 0;
}