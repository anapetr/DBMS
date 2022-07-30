#ifndef DBMS_COMMANDEXECUTOR_H
#define DBMS_COMMANDEXECUTOR_H

#include "./CommandParser.h"
#include "./DataBase.h"
#include "../utils/DBUtils.h"

//TODO - we dont need command parser
class CommandExecutor {

public:
    void executeCommand(const CommandParser& commandParser, DataBase& dataBase);

private:
    void createTable(const CommandParser& commandParser, DataBase& dataBase);
    void dropTable(const CommandParser& commandParser, DataBase& dataBase);
    void listTables(const DataBase& dataBase);
    void tableInfo(const CommandParser& commandParser, const DataBase& dataBase);
    void insert(const CommandParser& commandParser, DataBase& dataBase);
    void select(const CommandParser& commandParser, DataBase& dataBase);
    void remove(const CommandParser& commandParser, DataBase& dataBase);
    void save(DataBase& dataBase);
};


#endif //DBMS_COMMANDEXECUTOR_H
