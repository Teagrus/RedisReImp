#include <BaseEvent.h>
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
#include<iostream>
#include<memory>

#include<map>

std::map<std::string, std::string> contents;

RedisReImp::General::BaseEvent::BaseEvent(){
    // readDataChar = new char[bufferLen];
};

RedisReImp::General::BaseEvent::BaseEvent(int clientFD){
    setClientFD(clientFD);
};

RedisReImp::General::BaseEvent::~BaseEvent() {
    // free memory
    if (readDataRaw != nullptr) { 
        delete [] readDataRaw;
    }
}

int RedisReImp::General::BaseEvent::eRead() {
    // default for linux
    if (readDataRaw == nullptr) {
        readDataRaw = new char [bufferLen + 1];
    }
    
    while (true)
    {
        ssize_t nbytes = read(clientFD, readDataRaw+readDataLen, sizeof(char) * bufferLen-readDataLen);
        //deleted std::cout << "Read nbytes: "<< nbytes << std::endl;
        if (nbytes == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // printf("finished reading data from client\n");
                readDataRaw[readDataLen] = '\0';
                readDataString += readDataRaw;
                //deleted std::cout << "Received Data: "<< readDataString << std::endl;
                // check whether all datas are received
                int resolveStatus;
                int nextPartOffset;
                //deleted std::cout << "CCCCHeckedData: \n" <<  readDataString << std::endl;
                readData = RESPSolver::RESPData::checkData(readDataString, 0, &resolveStatus, &nextPartOffset);

                // //deleted std::cout << "Str Size " << readDataString.size() << "nextPartOffset "<< nextPartOffset;
                if (resolveStatus == 0) {
                    //deleted std::cout << "Loaded Data: "<< *readData.parseRESPString() << std::endl;

                    // reset ReadData
                    readDataString = readDataString.substr(nextPartOffset, -1);
                    readDataLen = 0;

                    stageNow += 1;
                    return 1;
                } else {
                    //deleted std::cout << "Loaded Err, Err type: "<<resolveStatus << std::endl;
                    if (resolveStatus < 0) {
                        // err! close socket
                        close(clientFD);
                        stageNow = 6;
                        return 1;
                    }
                }
                break;
            } else {
                //deleted std::cout << "ClientFD: "<< clientFD << std::endl;
                perror("read()");
                return -1;
            }
        } else if (nbytes == 0) {
            // all datas has been read, free raw memory
            // copy data
            readDataRaw[readDataLen] = '\0';
            readDataString += readDataRaw;
            if (readDataRaw != nullptr) {
                delete [] readDataRaw;
                readDataRaw = nullptr;
            }

            int resolveStatus;
            readData = RESPSolver::RESPData::checkData(readDataString, 0, &resolveStatus);
            if (resolveStatus == 0) {
                // successfully load the data
                //deleted std::cout << "Loaded Data: "<< *readData.getDataStr() << std::endl;
                stageNow += 1;
                return 1;
            } else {
                //deleted std::cout << "Loaded Err, Err type: "<<resolveStatus << std::endl;
                if (resolveStatus < 0) {
                    // err! close socket
                    close(clientFD);
                    stageNow = 6;
                    return 1;
                }
            }

        } else {
            // add readDataLen 
            readDataLen += nbytes;
            //deleted std::cout << "ReadDataRaw - readDataLen: "<< readDataLen << std::endl;
            if (readDataLen > bufferLen / 2) {
                readDataRaw[readDataLen] = '\0';
                readDataString += readDataRaw;
                // reset read Data Len
                readDataLen = 0;
            }
        }

    }

    return 0;
}


int RedisReImp::General::BaseEvent::eReadDataParse() {
    stageNow+=1;
    return 1;
}

int RedisReImp::General::BaseEvent::eDataProcess () {

    // For Registered function
    
    responseData = registeredProcessFunction(readData, *(readData.getByIndex(0).getDataStr()));

    // auto command = readData.getDataVec()->at(0).getDataStr();

    stageNow+=1;
    return 1;
}


    


void RedisReImp::General::BaseEvent::registerProcessFunction(
    std::function< RedisReImp::RESPSolver::RESPData (RedisReImp::RESPSolver::RESPData, std::string)> & func) {
    registeredProcessFunction = func;
}

int RedisReImp::General::BaseEvent::eWriteDataParse () {
    // responseDataString = readDataString;
    responseDataString = *responseData.parseRESPString();
    stageNow += 1;
    return 1;
}


int RedisReImp::General::BaseEvent::eWrite() {
    // for linux system
    // write response Data string to buffer
    int nwrite;

    nwrite = write(clientFD, responseDataString.c_str()+bytesWritten, responseDataString.size()-bytesWritten);
    if (nwrite == -1)
    {
        
        perror("Write error:");
        // close(clientFD);
    }

    bytesWritten += nwrite;
    //deleted std::cout << "NWRITE" << nwrite << std::endl;
    //deleted std::cout << "responseDataString.size() " << responseDataString.size() << std::endl;
    if (bytesWritten < responseDataString.size()) {
        return 0;
    } else {
        // close file descriptor
        // Do not close file decriptor by server
        // close(clientFD);
        stageNow += 1;

        // process end
        return 1;
    }
}

void RedisReImp::General::BaseEvent::setToFirstStage() {
    // reset
    bytesWritten = 0;
    stageNow = 1;
}

bool RedisReImp::General::BaseEvent::triggerEvent() {
    int tempResult = 0;
    
    //deleted std::cout << "EventTriggered, Stage:" << stageNow <<std::endl;
    switch (stageNow)
    {
    case 1:
        tempResult = eRead();
        if (tempResult != 1){
            return false; // wait for following data
        }
            
        break;
    case 2:
        eReadDataParse();
        break;
    case 3:
        eDataProcess();
        break;
    case 4:
        eWriteDataParse();
        break;
    case 5:
        eWrite();
        break;
    case 6:
        return false; // everything is done    
    
    default:
        break;
    }
    return true;
}

int RedisReImp::General::BaseEvent::getStageNow() {
    return stageNow;
}


void RedisReImp::General::BaseEvent::setClientFD(int fd) {
    clientFD = fd;
}


bool RedisReImp::General::BaseEvent::eventFinished() {
    if (stageNow == 6){
        return true;
    } else {
        return false;
    }
}
