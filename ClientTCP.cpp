#define _WINSOCK_DEPRECATED_NO_WARNINGS 
// 客户端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

#define PORT 1234 // 服务器端口
#define ROWS 3 // 二维数组的行数
#define COLS 3 // 二维数组的列数

typedef struct route_node {               //路由节点；
    char destinnation;    //目的节点；
    int distance;         //最短开销；
    char out;              //经过节点；
    char pass[10];
    int num;


}route_node;

typedef struct route_table {
    char route_name;          //路由表的名字；
    route_node items[20];     //路由表的项目；
}route_table;

int main() {
    // 初始化socket库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    /*这里我们使用了WSAStartup函数来初始化socket库，这是Windows下使用socket编程必须做的第一步。
    WSAStartup函数需要两个参数，第一个是一个WORD类型的值，表示请求的socket库的版本号，这里我们使用MAKEWORD宏来生成一个值，表示请求2.2版本的socket库。
    第二个参数是一个指向WSADATA结构体的指针，这个结构体用来存储socket库的信息，如版本号，描述等。
    WSAStartup函数返回一个整数值，如果为0，表示初始化成功，如果不为0，表示初始化失败。
    所以我们用一个if语句来判断WSAStartup函数的返回值是否为0，如果不为0，就说明初始化失败了，然后就打印出错误信息，
    WSAGetLastError函数可以返回最近发生的错误代码*/

    // 创建一个socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    /*这里我们使用了socket函数来创建一个socket，并检查是否创建成功。socket是一种通信的端点，可以用来在不同的进程或机器之间发送和接收数据。
    socket有三个参数，第一个是地址族，AF_INET表示使用IPv4协议，第二个是类型，SOCK_STREAM表示使用可靠的字节流服务，第三个是协议，0表示使用默认的协议，即TCP协议。
   socket函数返回一个SOCKET类型的值，如果创建成功，就是一个有效的socket描述符，如果创建失败，就是INVALID_SOCKET。
   所以我们用一个if语句来判断socket函数的返回值是否等于INVALID_SOCKET，如果是，就说明socket创建失败了，然后就打印出错误信息，*/


    // 设置服务器地址和端口
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 本地回环地址

    /*
   这里我们使用了一个结构体sockaddr_in来表示服务器的地址和端口，这个结构体有四个成员，
   sin_family表示地址族，sin_port表示端口号，sin_addr表示IP地址，sin_zero是一个填充字节，用于对齐。
   我们使用memset函数来将这个结构体的所有字节都初始化为0，然后给其中的三个成员赋值。
   sin_family赋值为AF_INET，表示使用IPv4协议，sin_port赋值为htons(PORT)，表示使用我们之前定义的宏PORT作为端口号，
   htons函数可以将主机字节序转换为网络字节序，因为不同的机器可能有不同的字节序，所以在网络通信时需要统一字节序。
   sin_addr.s_addr赋值为inet_addr(“127.0.0.1”)，表示使用本地回环地址作为IP地址，
   inet_addr函数可以将点分十进制的字符串形式的IP地址转换为无符号长整型的数值形式。
   */




    // 连接到服务器
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("connect failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /* 这里我们使用了connect函数来连接到服务器，并检查是否连接成功。
    connect函数有三个参数，第一个是我们之前创建的socket描述符，第二个是一个指向sockaddr结构体的指针，表示要连接的服务器的地址和端口，
    第三个是这个结构体的大小。注意这里我们需要将sockaddr_in结构体强制转换为sockaddr结构体，因为connect函数只接受sockaddr类型的参数。
    connect函数返回一个整数值，如果为0，表示连接成功，如果不为0，表示连接失败。
    所以我们用一个if语句来判断connect函数的返回值是否等于SOCKET_ERROR，如果是，就说明连接失败了，然后就打印出错误信息，*/

    // 定义一个二维数组并初始化
    int array[ROWS][COLS] = { {0, 2, 3}, {2, 0, 7}, {3, 7, 0} };

    // 发送二维数组到服务器
    if (send(sock, (char*)array, sizeof(array), 0) == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Sent array to server.\n");

    /*这里我们使用了send函数来发送二维数组到服务器，并检查是否发送成功。
    send函数有四个参数，第一个是我们之前创建的socket描述符，第二个是一个指向要发送数据的缓冲区的指针，第三个是要发送数据的大小，
    第四个是一些标志位，这里我们使用0表示默认的行为。注意这里我们需要将int类型的二维数组强制转换为char类型的指针，因为send函数只接受char类型的指针作为参数。
    send函数返回一个整数值，表示实际发送的字节数，如果为SOCKET_ERROR，表示发送失败。
    所以我们用一个if语句来判断send函数的返回值是否等于SOCKET_ERROR，如果是，就说明发送失败了，然后就打印出错误信息*/

    // 接收服务器返回的二维数组
    int recv_array[ROWS][COLS];
    if (recv(sock, (char*)recv_array, sizeof(recv_array), 0) == SOCKET_ERROR) {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    struct route_table table[ROWS];
    if (recv(sock, (char*)table, sizeof(table), 0) == SOCKET_ERROR) {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Received array from server.\n");


    // 打印接收到的二维数组
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", recv_array[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < ROWS; i++) {
        printf("%c的路由表:\n", table[i].route_name);
        printf("目的节点       最短路径         开销    \n");
        for (int j = 0; j < COLS; j++) {
            printf("%c\t\t", table[i].items[j].destinnation);
            for (int k = 0; k < table[i].items[j].num; k++) {
                printf("%c", table[i].items[j].pass[k]);
            }

            printf("\t\t%d\n", table[i].items[j].distance);
        }
    }

    // 关闭socket
    closesocket(sock);
    WSACleanup();
    return 0;
}
