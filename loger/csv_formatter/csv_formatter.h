//
// Created by rotten on 17.01.23.
//

#include <iostream>
#include <ctime>

#ifndef DIRECTORYCHANGELOGGER_CSV_FORMATTER_H
#define DIRECTORYCHANGELOGGER_CSV_FORMATTER_H


class CSVFormatter {
public:
    std::string timeTToString(std::time_t time){
        char buf[100];
        std::string t;
        strftime(buf, 100, "%a %b %d %T %Y", localtime(&time));
        t.append(buf);
        return t;
    }
    std::string Serialize(std::string path, std::string msg, std::time_t time){
        std::string serialized;
        serialized.append(timeTToString(time) + ",");
        serialized.append(path + ",");
        serialized.append(msg);
        return serialized;
    }
};

#endif //DIRECTORYCHANGELOGGER_CSV_FORMATTER_H
