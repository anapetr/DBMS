#include "../headers/Application.h"

const size_t WIDTH = 60;

void Application::menu() {
    std::cout << "The following commands are supported:" << std::endl;

    std::cout << std::setw(WIDTH) << std::left << "CreateTable <table_name> (<column_name>:<column_type>, <column_name>:<column_type>,...) Index ON <column_name>";
    std::cout << "Creates Database Table with <object_name> and the specified columns."<< std::endl;

    std::cout << std::setw(WIDTH) << std::left << "DropTable <table_name>";
    std::cout << "Drops table with <table_name> from the database" << std::endl;

    std::cout << std::setw(WIDTH) << std::left << "ListTables";
    std::cout << "Returns all of the table names in the database." << std::endl;

    std::cout << std::setw(WIDTH) << std::left << "TableInfo <table_name>";
    std::cout << "Returns information about table with <table_name> - size of the table in bytes, scheme, indexed columns and number of records."<< std::endl;

    std::cout << std::setw(WIDTH) << std::left << "Select <column_names> FROM <table_name> WHERE <condition_1> OR|AND <condition_2> ORDER BY <column_name> DISTINCT";
    std::cout << "Returns the specified <column_names> according to the given conditions." << std::endl;

    std::cout << std::setw(WIDTH) << std::left << "Remove FROM <table_name> WHERE <condition_1> OR|AND <condition_2> ..";
    std::cout << "Remove one or more records from table with <table_name>." <<std::endl;

    std::cout << std::setw(WIDTH) << std::left << "Insert INTO <table_name> {(<value_1>, <value_2>...), (<value_1>, <value_2>...)..}";
    std::cout << "Inserts one or more records in table with <table_name>.\n";

    std::cout << std::setw(WIDTH) << std::left << "Exit";
    std::cout << "Exits program.\n";
}

void Application::execute() {

    menu();

    //if there is no valid DB, create one
    checkIfDbIsValid(constants.pathToDb);

    //read commands
    CommandParser commandParser;
    CommandExecutor commandExecutor;

    try {
        std::ifstream in(constants.pathToDb, std::ios::binary);
        DataBase dataBase(in);
        in.close();

        while (true) {
            std::string command;
            std::getline(std::cin, command);

            commandParser.loadAndExecuteCommand(command);

            //cp.clearCmd();
            //cp.setData(cmd); //TODO - error

//            catch (const invalid_argument& e)
//            {
//                cout << red << e.what() << reset << endl;
//                continue;
//            }

            commandExecutor.executeCommand(commandParser, dataBase);

        }
    } catch (const std::exception e) {

    }

}

void Application::checkIfDbIsValid(const std::string &pathToDb) {
    std::ifstream in(pathToDb, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Couldn't open DB for reading. Creating new file directory for database..." << std::endl;

//        DataBase dataBase(dbName, dbPath);
    }


    in.close();
}