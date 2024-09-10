#pragma once
#include<RedisReImpGeneral.h>
#include<RESPData.h>
#include<CoreDataManager.h>

namespace RedisReImp::Server {


// interface for command executores
class ExecutorInterface {
public:
    // this returns which command this excutor corresponds
    virtual std::string  getCommandName() = 0;

    // this excutes target command, use RESPData descriptor as input and return value
    virtual RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager &)= 0;

    // this uses to get result status or errinfo
    virtual std::string getStatus()= 0; 

    // true: no err, false: fails. if now excuted, it is unknown for the return value of this function
    virtual bool isSuccess()= 0;
};

}
