#include <iostream>
#include <ThreadPool.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>

using namespace RedisReImp;


int runningTasksCount = 0;
int finishedTasks = 0;
int maxRunningTasks = 100;
long long latency = 0;
std::mutex runMutex;
std::condition_variable runningMaxCondition;

long long getMillisecondTimestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


int sendMessage() {

    // wait for max connection count
    {
        std::unique_lock<std::mutex> lock(runMutex);
        runningTasksCount += 1;
    }

    long long tStart = getMillisecondTimestamp();
    // 创建套接字
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(33400);

    // 将 IP 地址转换为二进制形式
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    // const char *hello = "+OK\r\n";
    // const char *hello = "*3\r\n$3\r\nSET\r\n$7\r\ntestkey\r\n$5\r\nhello\r\n";
    const char *hello = "*2\r\n$3\r\nGET\r\n$7\r\ntestkey\r\n";
    send(sock, hello, strlen(hello), 0);
    std::cout << "SENT: " << hello << std::endl;

    // 接收服务器的响应
    int valread = read(sock, buffer, 1024);

    std::cout << "RECEIVED: " << buffer << std::endl;
    // 关闭套接字
    close(sock);

    long long tConsumed = getMillisecondTimestamp() - tStart;
    // success

    {
        std::unique_lock<std::mutex> lock(runMutex);
        runningTasksCount -= 1;
        latency += tConsumed;
        runningMaxCondition.notify_one();
    }
    finishedTasks += 1;


    return 0;
}



int main(){
    General::ThreadPool ioThreadPool(5);
    int sendInterval = 0.1;
    int connectionCount = 0;

    std::time_t now = std::clock();
    sendMessage();
    // while (true)
    // {
    //     std::time_t elapsed = getMillisecondTimestamp() - now;
    //     if (elapsed > 1000) {
            
    //         std::cout << finishedTasks << " connections finished in last second!!" << std::endl;
    //         std::cout << "Average latency " << (latency / (float)finishedTasks) << " ms"  << std::endl << std::endl;
    //         finishedTasks = 0;
    //         latency = 0;

    //         now = getMillisecondTimestamp();
    //     }

    //     {
    //         // if success max wait for notify
    //         std::unique_lock<std::mutex> lock(runMutex);
    //         runningMaxCondition.wait(lock, [] {
    //             return runningTasksCount < maxRunningTasks;
    //         });
    //     }

    //     ioThreadPool.enqueue([]{sendMessage();});
    //     connectionCount++;
        

    // }
    
    
    return 0;
}