#include <iostream>

#include <stdio.h>
#include <Winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

#define PORT     90000

int main()
{
	//[1]初始化套接字
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 )
	{
		cout << "初始化套接字库失败！" << endl;
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2)
	{
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
		return -1;
	}

	//[2]创建套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);

	//[3]绑定
	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) < 0)
	{
		cout << "设置绑定失败！" << endl;
		return -1;
	}
	
	//[4]监听
	if (listen(sockSrv,10) < 0)
	{
		cout << "设置监听状态失败！" << endl;
		return -1;
	}

	//[5]接收客户端连接
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	while (true)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		char sendBuf[128];
		sprintf_s(sendBuf, "Welcome %s to connct success!", inet_ntoa(addrClient.sin_addr));
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		char recvBuf[128];
		recv(sockConn, recvBuf, 128, 0);
		cout << "Receive Message:" << recvBuf << endl;
		closesocket(sockConn);
	}
	WSACleanup();
	return 0;
}