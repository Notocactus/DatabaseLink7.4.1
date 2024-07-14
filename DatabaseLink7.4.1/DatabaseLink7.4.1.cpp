#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include <string>
#include <iostream>
#include <pqxx/pqxx>
#include "Database.h"

int main()
{

    try
    {
        Database db = Database();

        db.Connect();

        db.CreateTable("CREATE TABLE IF NOT EXISTS users (id UUID PRIMARY KEY DEFAULT uuid_generate_v4(), username varchar(80), password varchar(80), email varchar(80)); ");

        std::vector<std::string> cols = {"username", "password", "email"};
        std::vector<std::string> values = {"Funnyguy", "lol", "OOooo@gmail.com"};
        db.Insert("users", cols, values);
        values = {"Jellyfish", "blob", "blib"};
        db.Insert("users", cols, values);

        //pqxx::result response1 = worker.exec("CREATE TABLE IF NOT EXISTS users (Id int, Username varchar(80), Password varchar(80), Email varchar(80));");

        std::vector<std::pair<std::string, std::string>> opts = std::vector<std::pair<std::string, std::string>>();
        std::vector<std::pair<std::string, std::string>> argss = std::vector<std::pair<std::string, std::string>>();
        argss.emplace_back("email", "blipblop");
        opts.emplace_back("username", "Jellyfish");
        db.Update("users", argss, opts);

        cols = {};
        opts = {};
        pqxx::result response = db.Select("users", cols, opts);

        for (size_t i = 0; i < response.size(); i++)
        {
            std::cout << "Id: " << response[i][0] << " Username: " << response[i][1] << " Password: " << response[i][2] << " Email: " << response[i][3] << std::endl;
        }

        opts.emplace_back("username", "Jellyfish");
        db.Delete("users", opts);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    system("pause");
}