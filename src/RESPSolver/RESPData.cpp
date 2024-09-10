#include<RESPData.h>
#include<iostream>
#include <string>
namespace RedisReImp::RESPSolver  {

inline bool _isValidDataType(char dt) {
    if (dt=='+' || dt=='-' || dt==':' || dt=='$' || dt=='*')
        return true;
    return false;
}

inline int _searchNextPr(std::string& strin, int offsetNow) {
    int offsetNew = offsetNow;
    int strinSize = strin.size();
    while (offsetNew < strinSize && strin[offsetNew] != '\r')
    {
        offsetNew += 1;
    }
    if (offsetNew == strinSize) {
        return -1;
    } else {
        return offsetNew - offsetNow;
    }
    
}

bool RESPData::isValidDataType() {
    if (
        dataType == ':' ||
        dataType == '+' ||
        dataType == '-' ||
        dataType == '$' ||
        dataType == '*'
    ) {
        return true;
    }
    return false;
}

std::shared_ptr<std::string> RESPData::parseRESPString() {
    std::shared_ptr<std::string> resultStr = std::make_shared<std::string>();
    // push dataType
    *resultStr += dataType;

    


    if (dataType == '+' || dataType == '-') {
        *resultStr += *strData + "\r\n";
    }

    if (dataType == ':') {
        *resultStr += std::to_string(intData) + "\r\n";
    }


    if (dataType == '$') {
        *resultStr += std::to_string((*strData).size()) + "\r\n" + *strData + "\r\n";
    }

    if (dataType == '*') {
        int objectNum = commandDatas->size();
        
    

        *resultStr += std::to_string(objectNum) + "\r\n";
        for (size_t i = 0; i < objectNum; i++)
        {   
            *resultStr += *(commandDatas->at(i).parseRESPString());
        }
    }

    
    return resultStr;
};


RESPData RESPData::checkData(std::string & strDataIn, int offset, int * resultStatus, int* nextPartOffset) {
    // default case
    if (resultStatus != nullptr)
        * resultStatus = 0;
    // dataType
    char dataType = strDataIn[offset];

    // //deleted std::cout << "DataType&Offset " << dataType << " " << offset << std::endl;
    if (!_isValidDataType(dataType)) {
        if (resultStatus != nullptr)
            *resultStatus = -1;
        return RESPData();
    }

    int offsetSubContent = offset + 1;
    int offsetPr = 0;
    
    // //deleted std::cout << "result(dataType) " << dataType << std::endl;
    RESPData result(dataType);

    offsetPr = _searchNextPr(strDataIn, offsetSubContent);
    if (offsetPr == -1) {
        // data not complete
        if (resultStatus != nullptr)
            *resultStatus = 1;
        return RESPData();
    }

    // //deleted std::cout << "offsetPr " << offsetPr << std::endl;
    
    

    // For different data cases
    if (dataType == '+' || dataType == '-') {
        result.strData = std::make_shared<std::string>(strDataIn.substr(offsetSubContent, offsetPr) );
        if (nextPartOffset != nullptr) {
            *nextPartOffset = offsetSubContent + offsetPr + 2;
        }
    }

    if (dataType == ':') {
        std::string intstr = strDataIn.substr(offsetSubContent, offsetPr);

        
        

        result.intData = std::stoi(intstr);
        if (nextPartOffset != nullptr) {
            *nextPartOffset = offsetSubContent + offsetPr + 2;
        }



    }
    
    if (dataType == '$') {
        std::string lengthStr = strDataIn.substr(offsetSubContent, offsetPr);
        // //deleted std::cout << "lengthStr " << lengthStr << std::endl;
        int strLen = std::stoi(lengthStr);
        // //deleted std::cout << "strLen " << strLen << std::endl;
        // in case of null str
        if (strLen == -1) {
            result.strData = nullptr;
            return result;
        }
        if ( offsetSubContent + offsetPr + 2 + strLen >= strDataIn.size()) {
            // data not complete
            if (resultStatus != nullptr)
                *resultStatus = 1;
            return RESPData();
        }
        result.strData = std::make_shared<std::string>(
            strDataIn.substr(offsetSubContent + offsetPr + 2,strLen)
        );

        // //deleted std::cout << "result.strData " << *result.strData << std::endl;

        if (nextPartOffset != nullptr) {
            *nextPartOffset = offsetSubContent + offsetPr + strLen + 4;
        }
    }

    if (dataType == '*')  {
        std::string lengthStr = strDataIn.substr(offsetSubContent, offsetPr);
        int objectNum = std::stoi(lengthStr);
        offsetSubContent = offsetPr + offsetSubContent + 2;

        for (size_t i = 0; i < objectNum; i++)
        {   
            // //deleted std::cout << "Load Array content " << i << std::endl;
            // //deleted std::cout << "offsetSubContent " << offsetSubContent << std::endl;

            int npOffset;
            RESPData subData = checkData(strDataIn, offsetSubContent, resultStatus, &npOffset);
            // //deleted std::cout << "subData Type" << subData.getDataType() << std::endl;

            // err case
            // //deleted std::cout << "IS VALID" << subData.isValidDataType() << " OFFSET " << offset << std::endl;
            if (!subData.isValidDataType()) {
                return RESPData();
            }
            
            // success
            result.setDataVecEmplace(subData);

            
            // move to next
            offsetSubContent = npOffset;
            
        }

        if (nextPartOffset != nullptr) {
            *nextPartOffset = offsetSubContent;
        }

    }

    return result;

};

std::shared_ptr<std::string>  RESPData::getDataStr() {
    return strData;
};


const std::shared_ptr<std::vector<RESPData>> RESPData::getDataVec() {
    return commandDatas;
};

int RESPData::getDataInt() {
    return intData;
};

char RESPData::getDataType() {
    return dataType;
};  


RESPData& RESPData::getByIndex(int index) {
    return commandDatas->at(index);
};

int RESPData::getDataVecLen() {
    return commandDatas->size();
};


void RESPData::setDataVec(std::shared_ptr< std::vector<RESPData> > datain) {
    commandDatas = datain;
};

void RESPData::setDataVecEmplace(RESPData & datain) {
    if (commandDatas == nullptr) {
        commandDatas = std::make_shared< std::vector<RESPData> >(std::vector<RESPData>(0));
    }
    
    commandDatas->emplace_back(datain);
};

void RESPData::setDataStr(std::shared_ptr< std::string >  datain) {
    strData = datain;
};

void RESPData::setDataStr(const std::string  datain) {
    strData = std::make_shared<std::string>(datain);
};

void RESPData::setDataType(char dtype) {
    dataType = dtype;
};

void RESPData::setDataInt(int datain) {
    intData = datain;
};

RESPData::RESPData(const RESPData& copysrc) {
    dataType = copysrc.dataType;
    commandDatas = copysrc.commandDatas;
    strData = copysrc.strData;
    intData = copysrc.intData;
};

RESPData::RESPData(char dtype) {
    dataType = dtype;
};

RESPData::RESPData() {
    dataType = '\0';
};


RESPData::~RESPData() {
    ;// do nothing 
};




}








