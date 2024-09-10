#include <iostream>
#include <EpollServer.h>


using namespace RedisReImp;

int main() {
    // test config
    
    const auto config = General::GlobalConfig::getInstance();
    //deleted std::cout << "CONFIG port: " << config.port << std::endl;
    NetCore::EpollServer serverObject;
    serverObject.sBind();
    while (true)
    {
        serverObject.sEventLoop();
    }
    
    return 0; 
}



















// constexpr int MAX_EVENTS = 20;
// constexpr int MAX_CLIENTS = 30;
// constexpr int PORT = 33400;
// constexpr int MS_PER_TASK = 0;

// // Function to handle client connections in a separate thread
// void handleClient(int clientFd) {
//     char buffer[1024];
//     while (true) {
//         int bytesRead = read(clientFd, buffer, sizeof(buffer));
//         if (bytesRead <= 0) {
//             break;
//         }

//         write(clientFd, buffer, bytesRead);
//     }
//     // procees

    
    
//     close(clientFd);
// }



// int main(){
//     // Create thread Pool to accept messages
//     General::ThreadPool ioThreadPool(80);


//     // create server socket descriptors and epolls
//     int serverFd, epollFd;
//     size_t connectCount = 0;
//     struct sockaddr_in serverAddress;
//     struct epoll_event event, events[MAX_EVENTS];

//     // Create socket
//     serverFd = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverFd == -1) {
//         std::cerr << "Failed to create socket." << std::endl;
//         return 1;
//     }

//     // Bind socket to address and port
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = INADDR_ANY;
//     serverAddress.sin_port = htons(PORT);
//     if (bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
//         std::cerr << "Failed to bind socket." << std::endl;
//         close(serverFd);
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(serverFd, MAX_CLIENTS) == -1) {
//         std::cerr << "Failed to listen." << std::endl;
//         close(serverFd);
//         return 1;
//     }

//     // Create epoll instance
//     epollFd = epoll_create1(0);
//     if (epollFd == -1) {
//         std::cerr << "Failed to create epoll instance." << std::endl;
//         close(serverFd);
//         return 1;
//     }

//     // Add server socket to epoll
//     event.events = EPOLLIN;
//     event.data.fd = serverFd;
//     if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &event) == -1) {
//         std::cerr << "Failed to add server socket to epoll instance." << std::endl;
//         close(serverFd);
//         close(epollFd);
//         return 1;
//     }

//     //deleted std::cout << "Server started. Listening on port " << PORT << std::endl;

//     while (true) {
//         int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
//         if (numEvents == -1) {
//             std::cerr << "Failed to wait for events." << std::endl;
//             break;
//         }

//         for (int i = 0; i < numEvents; ++i) {
//             if (events[i].data.fd == serverFd) {
//                 // Accept new client connection
//                 struct sockaddr_in clientAddress;
//                 socklen_t clientAddressLength = sizeof(clientAddress);
//                 int clientFd = accept(serverFd, (struct sockaddr*)&clientAddress, &clientAddressLength);
//                 if (clientFd == -1) {
//                     std::cerr << "Failed to accept client connection." << std::endl;
//                     continue;
//                 }

//                 // Add client socket to epoll
//                 event.events = EPOLLIN;
//                 event.data.fd = clientFd;
//                 if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
//                     std::cerr << "Failed to add client socket to epoll instance." << std::endl;
//                     close(clientFd);
//                     continue;
//                 }

//             } 
//             else {
//                 // Handle client data
//                 // //deleted std::cout << "Add Client task to thread pool" << std::endl;
//                 int clientFd = events[i].data.fd;
//                 // ioThreadPool.enqueue([clientFd]{handleClient(clientFd); });
//                 handleClient(clientFd);
                
//             }

//         }
//     }

    
//     close(serverFd);
//     close(epollFd);
//     return 0;
    
// }