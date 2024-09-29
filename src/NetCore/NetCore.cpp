
#include<iostream>
#include<EpollServer.h>


// set a fd to nonblocking mode
void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl()");
        return;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl()");
    }
}

void set_nodelay(int fd) {
    int flag = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(flag)) < 0) {
        perror("setsockopt");
    }
}

RedisReImp::NetCore::EpollServer::EpollServer() {
    // set vector length
    events.resize(epollEventsLen);
    // set configs
    auto config = General::GlobalConfig::getInstance();
    listenPort = config.port;
    listenIP = config.listenAdress;
    epollEventsLen = config.epollEventsLen;
    
    ioWorkerThreadNum = config.ioWorkerThreadNum;

    // if ioWorkerThreadNum > 0, enable multi-thread workers
    if (ioWorkerThreadNum > 0) {
        ioWorkerThreads = std::make_shared<General::ThreadPool>(ioWorkerThreadNum);
        readReadyEvents = std::make_shared<std::deque< std::shared_ptr< General::BaseEvent> >>();
        writeReadyEvents = std::make_shared<std::deque< std::shared_ptr< General::BaseEvent> >>();

    }

}

RedisReImp::NetCore::EpollServer::~EpollServer() {
}


void RedisReImp::NetCore::EpollServer::sBind() {

    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD == -1) {
        perror("socket()");
    }

    // set SO_REUSEADDR
    int enableAdressReuse = 1;
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &enableAdressReuse, sizeof(enableAdressReuse)) == -1) {
        perror("setsockopt()");
    }

    // bind server port
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, listenIP.c_str(), &addr.sin_addr);
    addr.sin_port = htons(listenPort);


    if (bind(serverFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind()");
    }

    set_nonblocking(serverFD);
    //listen on target port
     if (listen(serverFD, SOMAXCONN) < 0) {
        perror("listen()");
    }


    epollFD = epoll_create1(0);
    if (epollFD == -1) {
        perror("epoll_create1()");
    }
    
    //deleted std::cout << "epollFD: "<< epollFD << std::endl;
    //deleted std::cout << "serverFD: "<< serverFD << std::endl;
    // register server sock to epoll
    memset(&event, 0, sizeof(event));
    event.data.fd = serverFD;
    event.events = EPOLLIN | EPOLLET;

    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, serverFD, &event) == -1) {
        perror("epoll_ctl()");
    }

}






// True: next loop exists
// False: loop should exit
bool RedisReImp::NetCore::EpollServer::sEventLoop( 
    std::function< RedisReImp::RESPSolver::RESPData (RedisReImp::RESPSolver::RESPData, std::string )> eventProcessFunc
) {
    // single thread process
    if (ioWorkerThreadNum == 0) {
        General::RuntimeCounter::loopStart();

        General::RuntimeCounter::moduleStart("Epoll wait");
        int nevents = epoll_wait(epollFD, events.data(), epollEventsLen, -1);
        General::RuntimeCounter::moduleStart("Epoll end");

        

    
    if (nevents == -1) {
            perror("epoll_wait()");
            return 1;
        }
    
        //deleted std::cout << "nevents: "<< nevents << std::endl;
        for (int i = 0; i < nevents; i++) {
            General::RuntimeCounter::moduleStart("BeforeStages");
            
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))) {
                
            // 1100   获取第0位的状态
            // 1100 & 0001 =

            int clientFD = events[i].data.fd;
                if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                    eventMapper.erase(clientFD);
                    close(events[i].data.fd);
                    continue;
                }
                
            } else if (events[i].data.fd == serverFD) {
                // server socket; call accept as many times as we can - bacause we are using non blocking mode
                while (true) {
                    struct sockaddr in_addr;
                    socklen_t in_addr_len = sizeof(in_addr);
                    int client = accept(serverFD, &in_addr, &in_addr_len);
    
                //deleted std::cout << "eFD: "<< client << std::endl;
                    if (client == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // we processed all of the connections
                            break;
                        } else {
                            perror("accept()");
                        }
                    } else {
                        printf("accepted new connection on fd %d\n", client);
                        set_nonblocking(client);
                        // disable nagle algorithm
                        set_nodelay(client);
    
                    event.data.fd = client;
                            event.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLHUP;
                        if (epoll_ctl(epollFD, EPOLL_CTL_ADD, client, &event) == -1) {
                            perror("epoll_ctl()");
                        }
                    }
                }
            } else {
                // client socket; Create an corresponding Event
                int clientFD = events[i].data.fd;

                //deleted std::cout << "eFD: "<< events[i].data.fd << std::endl;
                //deleted std::cout << "sFD: "<< serverFD << std::endl;
                std::shared_ptr<RedisReImp::General::BaseEvent> targetEvent;
    
            if (eventMapper.find(clientFD) == eventMapper.end()){
                    targetEvent = std::make_shared<RedisReImp::General::BaseEvent>(clientFD);
                    targetEvent->registerProcessFunction(eventProcessFunc);
    
                eventMapper.emplace(
                        std::pair<int, std::shared_ptr<RedisReImp::General::BaseEvent>>(
                            clientFD, targetEvent)
                    );
                } else {
                    targetEvent = eventMapper[clientFD];
                    // if end event: enter next stage
                    if (targetEvent->eventFinished()) {
                        targetEvent->setToFirstStage();
                        // reset client
                        // targetEvent->setClientFD(clientFD);
                    }
                }
                    
                
                while (true)
                {
                    General::RuntimeCounter::moduleStart("EStage-" + std::to_string(targetEvent->getStageNow()) );
                    bool loopContinue = targetEvent->triggerEvent();
                    General::RuntimeCounter::moduleEnd();
                    if (!loopContinue) {
                        break;
                    }
                }



            }
        }

        General::RuntimeCounter::loopEnd();

        return true;

    }
    else { // Multithread case
        General::RuntimeCounter::loopStart();
        // step 0 epoll wait;
        General::RuntimeCounter::moduleStart("Epoll wait");
        int nevents = epoll_wait(epollFD, events.data(), epollEventsLen, 1);
        General::RuntimeCounter::moduleStart("Epoll end");
        
        int clientEvent = 0;
        if (nevents == -1) {
            perror("epoll_wait()");
            return 1;
        }

        // allocate events to Threads
        std::vector<std::function<void()>> readTasks;
        
        for (int i = 0; i < nevents; i++) {
            auto targetEvent = events[i];
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))) {
                int clientFD = events[i].data.fd;
                if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                    eventMapper.erase(clientFD);
                    close(events[i].data.fd);
                    continue;
                }
                
            } else if (events[i].data.fd == serverFD){
                // server socket; call accept as many times as we can - bacause we are using non blocking mode
                while (true) {
                    struct sockaddr in_addr;
                    socklen_t in_addr_len = sizeof(in_addr);
                    int client = accept(serverFD, &in_addr, &in_addr_len);
                    //deleted std::cout << "eFD: "<< client << std::endl;
                    if (client == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // we processed all of the connections
                            break;
                        } else {
                            perror("accept()");
                        }
                    } else {
                        printf("accepted new connection on fd %d\n", client);
                        set_nonblocking(client);
                        // disable nagle algorithm
                        set_nodelay(client);
                        event.data.fd = client;
                        
                        event.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLHUP;
                        if (epoll_ctl(epollFD, EPOLL_CTL_ADD, client, &event) == -1) {
                            perror("epoll_ctl()");
                        }
                    }
                }
            } else {
                // allocate to io worker threads
                int clientFD = events[i].data.fd;
                std::shared_ptr<RedisReImp::General::BaseEvent> targetEvent;
                if (eventMapper.find(clientFD) == eventMapper.end()){
                    targetEvent = std::make_shared<RedisReImp::General::BaseEvent>(clientFD);
                    targetEvent->registerProcessFunction(eventProcessFunc);
                    eventMapper.emplace(
                    std::pair<int, std::shared_ptr<RedisReImp::General::BaseEvent>>(
                        clientFD, targetEvent)
                    );
                } else {
                    targetEvent = eventMapper[clientFD];
                    if (targetEvent->eventFinished()) {
                        targetEvent->setToFirstStage();
                    }
                }
                
        // step 1 read and preprocess procedure 
                // General::RuntimeCounter::moduleStart("Read");
                
                readTasks.emplace_back(
                    // capture target event
                    // epoll must be in edge-triggered mode
                    [targetEvent, this] () {
                        // Excute read
                        while (true)
                        {
                            bool loopContinue = targetEvent->triggerEvent();
                            if (!loopContinue || targetEvent->getStageNow() > 2) {
                                break;
                            }
                        }
                        //  to process stage
                        if (targetEvent->getStageNow() > 2) {
                            // push events to queue
                            {
                                std::unique_lock<std::mutex> lock(readReadyMutex);
                                readReadyEvents->emplace_back(targetEvent);
                            }
                        }
                    }
                );
                


            }
        }

        if (readTasks.size() > 0) {
            ioWorkerThreads->enqueue( 
                readTasks
            );
        }
        
        General::RuntimeCounter::moduleStart("Core");
        // while (readReadyEvents->size() == 0)
        // {
        //     std::cout << "No Task, --- nevents " << nevents<<" \n";
        // }

        // step 2 excute core logic
        // get all events
        std::shared_ptr<std::deque< std::shared_ptr< General::BaseEvent> >> readReadyEventsNow = nullptr;
        {
            std::unique_lock<std::mutex> lock(readReadyMutex);
            // get read events
            readReadyEventsNow = readReadyEvents;
            // reset read events
            readReadyEvents = std::make_shared<std::deque< std::shared_ptr< General::BaseEvent> >>();
        }
        

        
        // process events
        
        
        for (auto targetEvent: *readReadyEventsNow) {
            while (true)
            {
                bool loopContinue = targetEvent->triggerEvent();
                if (!loopContinue || targetEvent->getStageNow() > 3) {
                    break;
                }
            }
        }
        General::RuntimeCounter::moduleEnd();
        

        
        // step 3 write procedure
        // std::cout << (*readReadyEventsNow).size() << std::endl;
        std::vector<std::function<void()>> tasks;
        for (auto targetEvent: *readReadyEventsNow) {
            tasks.emplace_back(
                [targetEvent] () {
                    // write loop
                    while (true)
                    {
                        bool loopContinue = targetEvent->triggerEvent();
                        if (!loopContinue) {
                            break;
                        }
                    }
                }
            );
        }
        ioWorkerThreads->enqueue( 
            tasks
        );

        General::RuntimeCounter::loopEnd();

    }
    return true;
}