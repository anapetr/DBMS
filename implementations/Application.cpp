//
// Created by Ana Petrova on 19.07.22.
//

#include "../headers/Application.h"
#include "../headers/CommandParser.h"
#include "../headers/CommandExecutor.h"

void Application::execute() {

    //TODO - write menu
    std::string pathToDbFile = "path/to/db.bin";
    checkIfDbIsValid(pathToDbFile); //TODO  - set path

    CommandParser commandParser;
    CommandExecutor commandExecutor;

    try {
        std::ifstream inDb(pathToDbFile, std::ios::binary);
//        DataBase db(inDb); //TODO - create database
        inDb.close();

        while (true) {
            std::string command;
            std::getline(std::cin, command);

            //cp.clearCmd();
            //cp.setData(cmd); //TODO - error

//            catch (const invalid_argument& e)
//            {
//                cout << red << e.what() << reset << endl;
//                continue;
//            }

            commandExecutor.executeCommand(commandParser);

        }
    } catch (const std::exception e) {

    }

}

void Application::checkIfDbIsValid(const std::string &pathToDb) {
    std::ifstream in(pathToDb, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Couldn't open DB for reading. Creating new file directory for database..." << std::endl;

//        DataBase db(dbName, dbPath);
    }
    in.close();
}