#ifndef VERADEMO_VULNS_H
#define VERADEMO_VULNS_H

#include <string>
#include <vector>

// Database simulation structure
struct User {
    std::string username;
    std::string password_hash;
    std::string real_name;
    std::string blab_name;
    std::string created_at;
};

struct Blab {
    int id;
    std::string username;
    std::string content;
    std::string timestamp;
    int comment_count;
};

// CWE-89: SQL Injection vulnerabilities
class UserController {
public:
    // CWE-89: SQL Injection in login (UserController.java line 138)
    static bool login_vulnerable(const std::string& username, const std::string& password);
    
    // CWE-89: SQL Injection in password hint (UserController.java line 239)
    static std::string get_password_hint_vulnerable(const std::string& username);
    
    // CWE-89: SQL Injection in registration (UserController.java line 332)
    static bool register_user_vulnerable(const std::string& username, 
                                         const std::string& password,
                                         const std::string& real_name,
                                         const std::string& blab_name);
    
    // CWE-89: SQL Injection in profile (UserController.java line 437)
    static std::vector<std::string> get_user_history_vulnerable(const std::string& username);
};

// CWE-78: OS Command Injection vulnerabilities
class ToolsController {
public:
    // CWE-78: Command Injection in ping (ToolsController.java line 39)
    static std::string ping_vulnerable(const std::string& host);
    
    // CWE-78: Command Injection in fortune (ToolsController.java line 58)
    static std::string fortune_vulnerable(const std::string& fortune_file);
};

// CWE-611: XML External Entity (XXE) vulnerability
class XMLParser {
public:
    // CWE-611: XXE vulnerability
    static std::string parse_xml_vulnerable(const std::string& xml_content);
};

// CWE-564: SQL Injection via ORDER BY clause
class BlabController {
public:
    // CWE-89 + CWE-564: SQL Injection in ORDER BY (BlabController.java line 335)
    static std::vector<User> get_blabbers_vulnerable(const std::string& username, 
                                                      const std::string& sort_column);
};

// CWE-327: Use of weak cryptographic algorithm (MD5)
class CryptoUtils {
public:
    // CWE-327: Weak crypto - MD5 for password hashing (UserController.java line 1095)
    static std::string md5_hash_vulnerable(const std::string& input);
};

// CWE-73: External Control of File Name or Path
class FileController {
public:
    // CWE-73: Path traversal in profile image download
    static std::string download_profile_image_vulnerable(const std::string& image_name);
};

// Helper function to simulate SQL execution
std::string execute_sql_query(const std::string& query);

// Helper function to simulate command execution
std::string execute_command(const std::string& command);

#endif // VERADEMO_VULNS_H
