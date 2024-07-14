#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include <string>
#include <iostream>
#include <pqxx/pqxx>
#include <vector>

class Database
{
	pqxx::connection* connectionObject;

public:

	Database() {}

	void Connect(std::string host = "localhost", std::string port = "5433", std::string dbname = "test", 
		std::string user = "postgres", std::string password = "postgres") 
	{
		std::string connectionString = "host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password;
		connectionObject = new pqxx::connection(connectionString.c_str());
	}

	void CreateTable(std::string query) {
		pqxx::work worker(*connectionObject);
		pqxx::result response = worker.exec(query.c_str());
		worker.commit();
	}

	void CreateTable(std::string name, std::vector<std::pair<std::string, std::string>>& cols) {
		pqxx::work worker(*connectionObject);
		std::string query = "CREATE TABLE " + name + " (";
		for (int i = 0; i < cols.size() - 1; ++i) {
			query += cols[i].first + " " + cols[i].second + ", ";
		}
		query += cols[cols.size() - 1].first + " " + cols[cols.size() - 1].second + ")";
		pqxx::result response = worker.exec(query.c_str());
		worker.commit();
	}

	void Insert(std::string name, std::vector<std::string>& cols, std::vector<std::string>& values) {
		pqxx::work worker(*connectionObject);
		std::string query = "INSERT INTO " + name + " (";
		if (cols.size() != 0) {
			for (int i = 0; i < cols.size() - 1; ++i) {
				query += cols[i] + ", ";
			}
			query += cols[cols.size() - 1];
		}
		query += ") VALUES (";
		if (values.size() != 0) {
			for (int i = 0; i < values.size() - 1; ++i) {
				query += "'" + values[i] + "'" + ", ";
			}
			query += "'" + values[values.size() - 1] + "'";
		}		
		query += "); ";
		pqxx::result response = worker.exec(query.c_str());
		worker.commit();
	}

	pqxx::result Select(std::string name, std::vector<std::string>& cols, std::vector<std::pair<std::string, std::string>>& opts) {
		pqxx::work worker(*connectionObject);

		std::string columns = "";
		if (cols.size() == 0) {
			columns += "*";
		}
		else {
			for (int i = 0; i < cols.size() - 1; ++i) {
				columns += cols[i] + ", ";
			}
			columns += cols[cols.size() - 1];
		}
		std::string query = "SELECT " + columns + " FROM " + name;
	
		if (opts.size() != 0) {
			query += " WHERE ";
			for (int i = 0; i < opts.size() - 1; ++i) {
				query += opts[i].first + " = '" + opts[i].second + "' AND ";
			}
			query += opts[opts.size() - 1].first + " = '" + opts[opts.size() - 1].second + "'";
		}
		pqxx::result response = worker.exec(query.c_str());
		return response; 
	}

	void Delete(std::string name, std::vector<std::pair<std::string, std::string>>& opts) {
		pqxx::work worker(*connectionObject);
		std::string query = "DELETE FROM " + name;
		if (opts.size() != 0) {
			query += " WHERE ";
			for (int i = 0; i < opts.size() - 1; ++i) {
				query += opts[i].first + " = '" + opts[i].second + "' AND ";
			}
			query += opts[opts.size() - 1].first + " = '" + opts[opts.size() - 1].second + "'";
		}
		pqxx::result response = worker.exec(query.c_str());
		worker.commit();
	}

	void Update(std::string name, std::vector<std::pair<std::string, std::string>>& args, 
		std::vector<std::pair<std::string, std::string>>& opts) {
		pqxx::work worker(*connectionObject);
		std::string query = "UPDATE " + name + " SET ";
		for (int i = 0; i < args.size() - 1; ++i) {
			query += args[i].first + " = '" + args[i].second + "', "; 
		}
		query += args[args.size() - 1].first + " = '" + args[args.size() - 1].second + "'";
		if (opts.size() != 0) {
			query += " WHERE ";
			for (int i = 0; i < opts.size() - 1; ++i) {
				query += opts[i].first + " = '" + opts[i].second + "' AND ";
			}
			query += opts[opts.size() - 1].first + " = '" + opts[opts.size() - 1].second + "'";
		}
		pqxx::result response = worker.exec(query.c_str());
		worker.commit();
	}
};

