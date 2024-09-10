#include<iostream>
#include<string>
#include<RESPData.h>

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

using namespace RedisReImp;

RESPSolver::RESPData sendMessage(std::string serverAdress, int port) {

    // 创建套接字
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string receivedStr;
    char buffer[1025] = {0};
    buffer[1024] = '\0';

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // 将 IP 地址转换为二进制形式
    if (inet_pton(AF_INET, serverAdress.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    const char *hello = "*1\r\n$7\r\nCOMMAND\r\n";
    send(sock, hello, strlen(hello), 0);
    std::cout << "SENT: " << hello << std::endl;

    // 接收服务器的响应
    
    


    int resultStatus = 0;
    int loopcount = 0;
    int valReadMax = 0;
    RESPSolver::RESPData data;
    while (true)
    {   
        loopcount ++;
        int valread = read(sock, buffer, 1024);
        std::cout << "RECEIVED Data: " << buffer << std::endl;
        std::cout << "RECEIVED: " << valread << "Bytes" << std::endl;
        receivedStr += buffer;
        if (valReadMax < valread) {
            valReadMax = valread;
        }
        data = RESPSolver::RESPData::checkData(receivedStr, 0, &resultStatus);
        if (data.isValidDataType()) {
            std::cout << "All Data Loaded\n"; 
            
            break;
        } else {
            std::cout << valReadMax << std::endl;
            std::cout << "Loop count " << loopcount << std::endl;  
            std::cout << "StrSize " << receivedStr.size() << std::endl;  
            std::cout << "Data not all loaded - wait for new data\n";  
        }
    }

    
    
    // 关闭套接字
    close(sock);
    // success
    return data;
}

int main() {
    std::string address = "192.168.1.103";
    std::string portStr = "6379";

    // std::cout << "Input official Redis Server Adress: ";
    // std::cin >> address;

    // std::cout << "Input Port: ";
    // std::cin >> portStr;
    int port = 6379;
    if (portStr.size() > 0) {
        port = std::stoi(portStr);
    }

    std::cout << "\nUse " + address + ":" + std::to_string(port) + "\n";
    auto result = sendMessage(address, port);

    std::ofstream file("./commands.resp");
    if (file.is_open()) {
        file << *result.parseRESPString();
        file.close();
    }

    
    


    return 1;
}