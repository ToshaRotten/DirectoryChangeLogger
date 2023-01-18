//
// Created by rotten on 17.01.23.
//
#include <string>

#ifndef DIRECTORYCHANGELOGGER_JSON_FORMATTER_H
#define DIRECTORYCHANGELOGGER_JSON_FORMATTER_H

std::string formatWithQuotes(std::string str){
    std::string withQuotes;
    withQuotes.append("\"");
    withQuotes.append(str);
    withQuotes.append("\"");
    return withQuotes;
}

class JSONFormatter {
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
        serialized.append("{");
        serialized.append("\"path\": ");
        serialized.append(formatWithQuotes(path)+ ",");
        serialized.append("\"msg\": ");
        serialized.append(formatWithQuotes(msg) + ",");
        serialized.append("\"time\": ");
        serialized.append(formatWithQuotes(timeTToString(time)));
        serialized.append("}");
        return serialized;
    }
};


#endif //DIRECTORYCHANGELOGGER_JSON_FORMATTER_H
