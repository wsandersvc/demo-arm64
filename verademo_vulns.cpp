#include "verademo_vulns.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <array>
#include <memory>

// Simple MD5 stub
namespace SimpleMD5 {
    std::string md5(const std::string& input) {
        unsigned int hash = 5381;
        for (char c : input) {
            hash = ((hash << 5) + hash) + c;
        }
        std::stringstream ss;
        ss << std::hex << hash;
        return ss.str();
    }
}

// CWE-89: SQL Injection in login
bool UserController::login_vulnerable(const std::string& username, const std::string& password) {
    std::cout << "[UserController::login_vulnerable] Processing login\n";
    std::string password_hash = CryptoUtils::md5_hash_vulnerable(password);
    std::string sql_query = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password_hash + "';";
    std::cout << "[DEBUG] SQL: " << sql_query << "\n";
    std::string result = execute_sql_query(sql_query);
    return !result.empty();
}

// CWE-89: SQL Injection in password hint
std::string UserController::get_password_hint_vulnerable(const std::string& username) {
    std::cout << "[UserController::get_password_hint_vulnerable]\n";
    std::string sql = "SELECT password_hint FROM users WHERE username = '" + username + "'";
    std::cout << "[DEBUG] SQL: " << sql << "\n";
    return execute_sql_query(sql);
}

// CWE-89: SQL Injection in registration
bool UserController::register_user_vulnerable(const std::string& username, const std::string& password,
                                               const std::string& real_name, const std::string& blab_name) {
    std::cout << "[UserController::register_user_vulnerable]\n";
    std::string password_hash = CryptoUtils::md5_hash_vulnerable(password);
    std::string sql = "INSERT INTO users VALUES('" + username + "','" + password_hash + "','" + real_name + "','" + blab_name + "')";
    std::cout << "[DEBUG] SQL: " << sql << "\n";
    execute_sql_query(sql);
    return true;
}

// CWE-89: SQL Injection in user history
std::vector<std::string> UserController::get_user_history_vulnerable(const std::string& username) {
    std::cout << "[UserController::get_user_history_vulnerable]\n";
    std::string sql = "SELECT event FROM users_history WHERE blabber=\"" + username + "\"";
    std::cout << "[DEBUG] SQL: " << sql << "\n";
    std::vector<std::string> events;
    events.push_back(execute_sql_query(sql));
    return events;
}

// CWE-89 + CWE-564: SQL Injection in ORDER BY
std::vector<User> BlabController::get_blabbers_vulnerable(const std::string& username, const std::string& sort_column) {
    std::cout << "[BlabController::get_blabbers_vulnerable]\n";
    std::string sql = "SELECT * FROM users WHERE username!='" + username + "' ORDER BY " + sort_column;
    std::cout << "[DEBUG] SQL: " << sql << "\n";
    execute_sql_query(sql);
    return std::vector<User>();
}

// CWE-78: Command Injection in ping
std::string ToolsController::ping_vulnerable(const std::string& host) {
    std::cout << "[ToolsController::ping_vulnerable] host=" << host << "\n";
    std::string command = "ping -c1 " + host;
    std::cout << "[DEBUG] CMD: " << command << "\n";
    return execute_command(command);
}

// CWE-78: Command Injection in fortune
std::string ToolsController::fortune_vulnerable(const std::string& fortune_file) {
    std::cout << "[ToolsController::fortune_vulnerable]\n";
    std::string cmd = "/usr/games/fortune " + fortune_file;
    std::cout << "[DEBUG] CMD: " << cmd << "\n";
    return execute_command(cmd);
}

// CWE-327: Weak MD5 hashing
std::string CryptoUtils::md5_hash_vulnerable(const std::string& input) {
    return SimpleMD5::md5(input);
}

// CWE-73: Path Traversal
std::string FileController::download_profile_image_vulnerable(const std::string& image_name) {
    std::cout << "[FileController::download_profile_image_vulnerable]\n";
    std::string path = "/var/www/resources/images/" + image_name;
    std::cout << "[DEBUG] PATH: " << path << "\n";
    std::ifstream file(path);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return "[ERROR] File not found";
}

// CWE-611: XXE
std::string XMLParser::parse_xml_vulnerable(const std::string& xml_content) {
    std::cout << "[XMLParser::parse_xml_vulnerable]\n";
    std::cout << "[DEBUG] XML: " << xml_content.substr(0, 100) << "\n";
    return "[XML parsed - XXE vulnerable]";
}

// Helper functions
std::string execute_sql_query(const std::string& query) {
    std::cout << "[SQL EXECUTION]\n";
    return (query.find("SELECT") != std::string::npos) ? "result_data" : "";
}

std::string execute_command(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) return "[ERROR] Command failed\n";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
