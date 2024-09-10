#include<iostream>
#include<RedisReImpGeneral.h>
#include<EpollServer.h>
#include<Executors.h>


using namespace RedisReImp;
int main() {

    std::cout << "Server starting !!!" << std::endl;
    const auto config = General::GlobalConfig::getInstance();
    std::cout << "CONFIG port: " << config.port << std::endl;
    NetCore::EpollServer serverObject;
    Server::CoreDataManager datamanager;
    serverObject.sBind();
    // init commands
    auto commandsMapper = Server::getCommandConfigs();
    Server::BaseExecutor baseExcutor;
    while (true)
    {
        serverObject.sEventLoop(
            [&datamanager, &baseExcutor](RESPSolver::RESPData inputData, std::string command) -> RESPSolver::RESPData {
                std::cout << "Received command: " << command << std::endl;
                
                auto excutor = Server::ExecutorsCluster::getExcutor(command);
                if (excutor == nullptr) {
                    std::cout << "Unknown command, use echo excutor -|" << command << "|" << std::endl;
                    return baseExcutor.execute(inputData, datamanager);
                }
                return excutor->execute(inputData, datamanager);
            }
        );
    }
    
    return 0;
}