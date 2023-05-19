#define _WINSOCK_DEPRECATED_NO_WARNINGS 
// �ͻ��˴���
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

#define PORT 1234 // �������˿�
#define ROWS 3 // ��ά���������
#define COLS 3 // ��ά���������

typedef struct route_node {               //·�ɽڵ㣻
    char destinnation;    //Ŀ�Ľڵ㣻
    int distance;         //��̿�����
    char out;              //�����ڵ㣻
    char pass[10];
    int num;


}route_node;

typedef struct route_table {
    char route_name;          //·�ɱ�����֣�
    route_node items[20];     //·�ɱ����Ŀ��
}route_table;

int main() {
    // ��ʼ��socket��
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    /*��������ʹ����WSAStartup��������ʼ��socket�⣬����Windows��ʹ��socket��̱������ĵ�һ����
    WSAStartup������Ҫ������������һ����һ��WORD���͵�ֵ����ʾ�����socket��İ汾�ţ���������ʹ��MAKEWORD��������һ��ֵ����ʾ����2.2�汾��socket�⡣
    �ڶ���������һ��ָ��WSADATA�ṹ���ָ�룬����ṹ�������洢socket�����Ϣ����汾�ţ������ȡ�
    WSAStartup��������һ������ֵ�����Ϊ0����ʾ��ʼ���ɹ��������Ϊ0����ʾ��ʼ��ʧ�ܡ�
    ����������һ��if������ж�WSAStartup�����ķ���ֵ�Ƿ�Ϊ0�������Ϊ0����˵����ʼ��ʧ���ˣ�Ȼ��ʹ�ӡ��������Ϣ��
    WSAGetLastError�������Է�����������Ĵ������*/

    // ����һ��socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    /*��������ʹ����socket����������һ��socket��������Ƿ񴴽��ɹ���socket��һ��ͨ�ŵĶ˵㣬���������ڲ�ͬ�Ľ��̻����֮�䷢�ͺͽ������ݡ�
    socket��������������һ���ǵ�ַ�壬AF_INET��ʾʹ��IPv4Э�飬�ڶ��������ͣ�SOCK_STREAM��ʾʹ�ÿɿ����ֽ������񣬵�������Э�飬0��ʾʹ��Ĭ�ϵ�Э�飬��TCPЭ�顣
   socket��������һ��SOCKET���͵�ֵ����������ɹ�������һ����Ч��socket���������������ʧ�ܣ�����INVALID_SOCKET��
   ����������һ��if������ж�socket�����ķ���ֵ�Ƿ����INVALID_SOCKET������ǣ���˵��socket����ʧ���ˣ�Ȼ��ʹ�ӡ��������Ϣ��*/


    // ���÷�������ַ�Ͷ˿�
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ���ػػ���ַ

    /*
   ��������ʹ����һ���ṹ��sockaddr_in����ʾ�������ĵ�ַ�Ͷ˿ڣ�����ṹ�����ĸ���Ա��
   sin_family��ʾ��ַ�壬sin_port��ʾ�˿ںţ�sin_addr��ʾIP��ַ��sin_zero��һ������ֽڣ����ڶ��롣
   ����ʹ��memset������������ṹ��������ֽڶ���ʼ��Ϊ0��Ȼ������е�������Ա��ֵ��
   sin_family��ֵΪAF_INET����ʾʹ��IPv4Э�飬sin_port��ֵΪhtons(PORT)����ʾʹ������֮ǰ����ĺ�PORT��Ϊ�˿ںţ�
   htons�������Խ������ֽ���ת��Ϊ�����ֽ�����Ϊ��ͬ�Ļ��������в�ͬ���ֽ�������������ͨ��ʱ��Ҫͳһ�ֽ���
   sin_addr.s_addr��ֵΪinet_addr(��127.0.0.1��)����ʾʹ�ñ��ػػ���ַ��ΪIP��ַ��
   inet_addr�������Խ����ʮ���Ƶ��ַ�����ʽ��IP��ַת��Ϊ�޷��ų����͵���ֵ��ʽ��
   */




    // ���ӵ�������
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("connect failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /* ��������ʹ����connect���������ӵ���������������Ƿ����ӳɹ���
    connect������������������һ��������֮ǰ������socket���������ڶ�����һ��ָ��sockaddr�ṹ���ָ�룬��ʾҪ���ӵķ������ĵ�ַ�Ͷ˿ڣ�
    ������������ṹ��Ĵ�С��ע������������Ҫ��sockaddr_in�ṹ��ǿ��ת��Ϊsockaddr�ṹ�壬��Ϊconnect����ֻ����sockaddr���͵Ĳ�����
    connect��������һ������ֵ�����Ϊ0����ʾ���ӳɹ��������Ϊ0����ʾ����ʧ�ܡ�
    ����������һ��if������ж�connect�����ķ���ֵ�Ƿ����SOCKET_ERROR������ǣ���˵������ʧ���ˣ�Ȼ��ʹ�ӡ��������Ϣ��*/

    // ����һ����ά���鲢��ʼ��
    int array[ROWS][COLS] = { {0, 2, 3}, {2, 0, 7}, {3, 7, 0} };

    // ���Ͷ�ά���鵽������
    if (send(sock, (char*)array, sizeof(array), 0) == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Sent array to server.\n");

    /*��������ʹ����send���������Ͷ�ά���鵽��������������Ƿ��ͳɹ���
    send�������ĸ���������һ��������֮ǰ������socket���������ڶ�����һ��ָ��Ҫ�������ݵĻ�������ָ�룬��������Ҫ�������ݵĴ�С��
    ���ĸ���һЩ��־λ����������ʹ��0��ʾĬ�ϵ���Ϊ��ע������������Ҫ��int���͵Ķ�ά����ǿ��ת��Ϊchar���͵�ָ�룬��Ϊsend����ֻ����char���͵�ָ����Ϊ������
    send��������һ������ֵ����ʾʵ�ʷ��͵��ֽ��������ΪSOCKET_ERROR����ʾ����ʧ�ܡ�
    ����������һ��if������ж�send�����ķ���ֵ�Ƿ����SOCKET_ERROR������ǣ���˵������ʧ���ˣ�Ȼ��ʹ�ӡ��������Ϣ*/

    // ���շ��������صĶ�ά����
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


    // ��ӡ���յ��Ķ�ά����
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", recv_array[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < ROWS; i++) {
        printf("%c��·�ɱ�:\n", table[i].route_name);
        printf("Ŀ�Ľڵ�       ���·��         ����    \n");
        for (int j = 0; j < COLS; j++) {
            printf("%c\t\t", table[i].items[j].destinnation);
            for (int k = 0; k < table[i].items[j].num; k++) {
                printf("%c", table[i].items[j].pass[k]);
            }

            printf("\t\t%d\n", table[i].items[j].distance);
        }
    }

    // �ر�socket
    closesocket(sock);
    WSACleanup();
    return 0;
}
