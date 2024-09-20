#include<iostream>
#include<RedisReImpGeneral.h>
#include<EpollServer.h>
#include<Executors.h>


using namespace RedisReImp;
int main() {

    //deleted std::cout << "Server starting !!!" << std::endl;
    const auto config = General::GlobalConfig::getInstance();
    //deleted std::cout << "CONFIG port: " << config.port << std::endl;
    NetCore::EpollServer serverObject;
    Server::CoreDataManager datamanager;
    serverObject.sBind();
    // init commands
    auto commandsMapper = Server::getCommandConfigs();
    Server::BaseExecutor baseExcutor;
    
    General::RuntimeCounter::reset();
    General::RuntimeCounter::setPrintLoopNum(10000);
    
    while (true)
    {
        serverObject.sEventLoop(
            [&datamanager, &baseExcutor](RESPSolver::RESPData inputData, std::string command) -> RESPSolver::RESPData {
                //deleted std::cout << "Received command: " << command << std::endl;
                
                auto excutor = Server::ExecutorsCluster::getExcutor(command);
                if (excutor == nullptr) {
                    //deleted std::cout << "Unknown command, use echo excutor -|" << command << "|" << std::endl;
                    return baseExcutor.execute(inputData, datamanager);
                }
                return excutor->execute(inputData, datamanager);
            }
        );
    }
    
    return 0;
}