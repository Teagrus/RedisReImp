#pragma once

#include<RedisReImpGeneral.h>
#include<BaseEvent.h>
#include<string>
#include<vector>
#include<map>
#include<memory>

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

namespace RedisReImp::NetCore {
// ------------------


class EpollServer{
    private:
        std::string listenIP = "0.0.0.0";
        int listenPort = 33400;
        int listenQueueLen = 10;
        int epollEventsLen = 100;
        int fdSize = 1000;

        int serverFD = -1;
        int epollFD = -1;

        epoll_event event;
        std::vector<epoll_event> events;
        std::map<int, std::shared_ptr<RedisReImp::General::BaseEvent>> eventMapper;


    public:
        EpollServer();
        ~EpollServer();

        // void initialize();
        void sBind();
        bool sEventLoop(std::function< RedisReImp::RESPSolver::RESPData (RedisReImp::RESPSolver::RESPData, std::string)>);

};




// ------------------
}