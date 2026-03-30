#include <iostream>
#include <winsock2.h>
#include <thread>
#include <atomic>
#include <algorithm>
#include <mutex>
#include <vector>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

atomic<bool> leave(false);
mutex mtx;


void start(int sPort, int ePort) {
    sockaddr_in address;
    ZeroMemory(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    u_long optval = 1;
    timeval timeout;

    fd_set fds;
    FD_ZERO(&fds);
    for (int port=sPort;port<ePort;port++) {
        if (leave)
            break;

        timeout.tv_sec = 0;
        timeout.tv_usec = 3000;
        FD_ZERO(&fds);
        address.sin_port = htons(port);

        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        ioctlsocket(sock, FIONBIO, &optval);

        if (sock == INVALID_SOCKET) {
            cout << "socket failed with error: " << WSAGetLastError() << std::endl;
            exit(1);
        }
        int result = connect(sock, (struct sockaddr*)&address, sizeof(address));
        if (result == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK && err != WSAEINPROGRESS && err != WSAEINVAL) {
                closesocket(sock);
                continue;
            }
        }
        FD_SET(sock, &fds);
        int sel = select(0, nullptr, &fds, nullptr, &timeout );
        if (sel > 0 && FD_ISSET(sock, &fds)) {
            int so_error = 0;
            int len = sizeof(so_error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len);

            if (so_error == 0) {
                lock_guard<mutex> lock(mtx);
                cout << port << " open\n";
            }

            //else cout << port << " connect failed with error: " << WSAGetLastError() << std::endl;

        }
        closesocket(sock);
    }
}


int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        exit(1);
    }

    int cPort = 1;
    vector<thread> threads;

    for (int i = 0; i < 51; i++) {
        threads.emplace_back(start,cPort,min(cPort+1285,65536));
        cPort += 1285;
    }



    string input = "";
    while (input != "exit") {
        cin >> input;
        if (input != "exit") {
            input = "";
            cin.clear();
            cin.ignore(1000,'\n');
        }
    }
    leave = true;

    for (auto& th : threads) {
        th.join();
    }

    WSACleanup();


    return 0;
}