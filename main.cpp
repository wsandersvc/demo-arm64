// Windows compatibility
#ifdef _WIN32
#define _USE_MATH_DEFINES  // Enable M_PI, M_E constants
#define _CRT_SECURE_NO_WARNINGS  // Allow ctime usage
#endif

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <cstring>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include "verademo_vulns.h"

// Simulated sensitive data
const char* DATABASE_PASSWORD = "MyS3cr3tP@ssw0rd!";
const char* API_KEY = "sk_live_1234567890abcdefghijklmnop";
const char* DATABASE_CONNECTION = "Server=prod-db.internal.corp;User=admin;Password=MyS3cr3tP@ssw0rd!;";
const char* SSH_PRIVATE_KEY = "-----BEGIN RSA PRIVATE KEY-----\nMIIEowIBAAKCAQEA1234567890...\n-----END RSA PRIVATE KEY-----";

// CWE-209: Error message containing sensitive information
void authenticate_user(const char* username, const char* password) {
    // VULNERABLE: Exposes database password in exception message
    if (std::string(password) != "correct_password") {
        std::string error = "Authentication failed for user: " + std::string(username) + 
                          ". Database connection: " + DATABASE_CONNECTION + 
                          " returned error code 401";
        throw std::runtime_error(error);
    }
}

// CWE-200: Information Exposure - exposing system paths and configurations
void print_system_info() {
    std::cout << "\n=== SYSTEM INFORMATION ===\n";
    // VULNERABLE: Exposing internal system paths
    std::cout << "Database Host: prod-db.internal.corp\n";
    std::cout << "Database Port: 5432\n";
    std::cout << "Config Path: /etc/app/config/production.xml\n";
    std::cout << "Private Key Path: /home/admin/.ssh/id_rsa\n";
    std::cout << "Admin Username: root_admin\n";
    std::cout << "Internal API Endpoint: https://internal-api.corp.local:8443/admin\n";
}

// CWE-201: Insertion of Sensitive Information Into Sent Data
std::string generate_http_response(const char* username) {
    std::stringstream response;
    response << "HTTP/1.1 200 OK\n";
    response << "Content-Type: application/json\n";
    response << "X-Internal-User: " << username << "\n";
    // VULNERABLE: Including sensitive data in HTTP headers
    response << "X-Database-Password: " << DATABASE_PASSWORD << "\n";
    response << "X-API-Key: " << API_KEY << "\n";
    response << "X-Session-Token: sessiontoken_" << username << "_" << DATABASE_PASSWORD << "\n";
    response << "\n";
    response << "{\"user\":\"" << username << "\",\"api_key\":\"" << API_KEY << "\"}\n";
    return response.str();
}

// CWE-215: Insertion of Sensitive Information Into Debugging Code
void debug_login_attempt(const char* username, const char* password, bool success) {
    #ifdef _DEBUG
    // VULNERABLE: Debug output containing passwords
    std::cout << "[DEBUG] Login attempt:\n";
    std::cout << "  Username: " << username << "\n";
    std::cout << "  Password: " << password << "\n";
    std::cout << "  Success: " << (success ? "true" : "false") << "\n";
    std::cout << "  Database Password: " << DATABASE_PASSWORD << "\n";
    std::cout << "  API Key: " << API_KEY << "\n";
    #endif
    
    // VULNERABLE: Writing sensitive data to debug log file
    std::ofstream debuglog("debug.log", std::ios::app);
    if (debuglog.is_open()) {
        debuglog << "[DEBUG] Credentials used - User: " << username 
                 << " | Pass: " << password 
                 << " | DB_PASS: " << DATABASE_PASSWORD << "\n";
        debuglog.close();
    }
}

// CWE-209: Detailed exception with internal information
void connect_to_database() {
    // VULNERABLE: Exception exposes connection string with credentials
    try {
        bool connection_failed = true;
        if (connection_failed) {
            std::stringstream error;
            error << "Database connection failed!\n";
            error << "Connection string: " << DATABASE_CONNECTION << "\n";
            error << "Host: prod-db.internal.corp\n";
            error << "Username: admin\n";
            error << "Password: " << DATABASE_PASSWORD << "\n";
            error << "Stack trace: connect_to_database() at main.cpp:89\n";
            throw std::runtime_error(error.str());
        }
    } catch (const std::exception& e) {
        // VULNERABLE: Re-throwing with even more sensitive info
        std::cerr << "CRITICAL ERROR: " << e.what() << "\n";
        std::cerr << "API Key for recovery: " << API_KEY << "\n";
        std::cerr << "SSH Key: " << SSH_PRIVATE_KEY << "\n";
    }
}

// CWE-200: Information disclosure through verbose error messages
void process_payment(const char* credit_card, const char* cvv) {
    std::cout << "\n=== Processing Payment ===\n";
    // VULNERABLE: Displaying full credit card details
    std::cout << "Credit Card: " << credit_card << "\n";
    std::cout << "CVV: " << cvv << "\n";
    std::cout << "Processing with API Key: " << API_KEY << "\n";
    
    // VULNERABLE: Logging sensitive financial data
    std::ofstream paymentlog("payments.log", std::ios::app);
    if (paymentlog.is_open()) {
        paymentlog << "CC: " << credit_card << " | CVV: " << cvv 
                   << " | API: " << API_KEY << "\n";
        paymentlog.close();
    }
}

// CWE-117: Vulnerable logging function that doesn't sanitize input
void log_user_action(const char* username, const char* action) {
    std::ofstream logfile("application.log", std::ios::app);
    if (logfile.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt)-1] = '\0'; // Remove newline
        
        // VULNERABLE: Direct logging without sanitization
        logfile << dt << " - User: " << username << " - Action: " << action << std::endl;
        logfile.close();
    }
}

// CWE-117: Vulnerable error logging
void log_error(const std::string& error_message) {
    std::ofstream logfile("error.log", std::ios::app);
    if (logfile.is_open()) {
        // VULNERABLE: User-controlled data in log without sanitization
        logfile << "[ERROR] " << error_message << std::endl;
        logfile.close();
    }
}

// CWE-117: Vulnerable authentication logging
void log_authentication_attempt(const char* username, bool success) {
    std::ofstream logfile("auth.log", std::ios::app);
    if (logfile.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt)-1] = '\0';
        
        // VULNERABLE: Username not sanitized, could inject newlines
        logfile << dt << " - Authentication " << (success ? "SUCCESS" : "FAILED") 
                << " for user: " << username << std::endl;
        logfile.close();
    }
}

unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

float dot_product(const float* a, const float* b, size_t size) {
    float sum = 0.0f;
    
    for (size_t i = 0; i < size; i++) {
        sum += a[i] * b[i];
    }
    
    return sum;
}

void matrix_multiply(const std::vector<std::vector<double>>& a,
                    const std::vector<std::vector<double>>& b,
                    std::vector<std::vector<double>>& result) {
    size_t rows = a.size();
    size_t cols = b[0].size();
    size_t inner = b.size();
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            result[i][j] = 0;
            for (size_t k = 0; k < inner; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    std::cout << "ARM64 Sample Project - Security Scanner Test\n";
    std::cout << "==============================================\n\n";
    
    // CWE-117: Simulate user input that could contain log injection
    char username[256];
    char password[256];
    std::cout << "Enter username: ";
    std::cin.getline(username, 256);
    std::cout << "Enter password: ";
    std::cin.getline(password, 256);
    
    // CWE-215: Debug output with sensitive information
    debug_login_attempt(username, password, false);
    
    // VULNERABLE: Log user action without sanitization
    log_user_action(username, "Application started");
    
    // CWE-200: Exposing system information
    print_system_info();
    
    // CWE-201: Generating HTTP response with sensitive data
    std::cout << "\n=== GENERATING HTTP RESPONSE ===\n";
    std::string http_response = generate_http_response(username);
    std::cout << http_response << "\n";
    
    // CWE-209: Exception with sensitive information
    std::cout << "\n=== ATTEMPTING DATABASE CONNECTION ===\n";
    try {
        connect_to_database();
    } catch (const std::exception& e) {
        // Error already printed in connect_to_database()
    }
    
    // CWE-209: Authentication error with sensitive details
    std::cout << "\n=== TESTING AUTHENTICATION ===\n";
    try {
        authenticate_user(username, password);
    } catch (const std::exception& e) {
        std::cerr << "Authentication Error: " << e.what() << "\n";
    }
    
    // CWE-200: Processing payment with exposed data
    process_payment("4532-1234-5678-9010", "123");
    
    // VULNERABLE: Simulate authentication attempt with user input
    log_authentication_attempt(username, true);
    
    std::cout << "\n=== COMPUTATION TESTS ===\n";
    
    // Test factorial computation
    std::cout << "Computing factorials:\n";
    for (int i = 1; i <= 10; i++) {
        std::cout << "  " << i << "! = " << factorial(i) << "\n";
    }
    std::cout << "\n";
    
    // VULNERABLE: Log calculation with potential user input
    std::string calc_log = "User " + std::string(username) + " performed factorial calculations";
    log_error(calc_log);
    
    // Test dot product
    std::cout << "Dot product test:\n";
    float vec_a[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
    float vec_b[] = {8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    float dot = dot_product(vec_a, vec_b, 8);
    std::cout << "  Result: " << dot << "\n\n";
    
    // Test matrix multiplication
    std::cout << "Matrix multiplication test:\n";
    std::vector<std::vector<double>> mat_a = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::vector<double>> mat_b = {{7, 8}, {9, 10}, {11, 12}};
    std::vector<std::vector<double>> mat_result(2, std::vector<double>(2));
    
    matrix_multiply(mat_a, mat_b, mat_result);
    
    std::cout << "  Matrix A (2x3) * Matrix B (3x2) = Result (2x2):\n";
    for (const auto& row : mat_result) {
        std::cout << "  [ ";
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
    
    // Compute some mathematical operations
    std::cout << "Mathematical operations:\n";
    double angle = M_PI / 4.0;
    std::cout << "  sin(π/4) = " << std::sin(angle) << "\n";
    std::cout << "  cos(π/4) = " << std::cos(angle) << "\n";
    std::cout << "  sqrt(2) = " << std::sqrt(2.0) << "\n";
    std::cout << "  log(e) = " << std::log(M_E) << "\n";
    
    // ========================================================================
    // VERADEMO VULNERABILITIES (Ported from verademo-java)
    // ========================================================================
    
    std::cout << "\n\n=== VERADEMO VULNERABILITIES ===\n";
    std::cout << "Testing vulnerabilities ported from verademo-java...\n\n";
    
    // CWE-89: SQL Injection - Login (from UserController.java)
    std::cout << "--- CWE-89: SQL Injection in Login ---\n";
    bool login_result = UserController::login_vulnerable(username, password);
    std::cout << "Login result: " << (login_result ? "Success" : "Failed") << "\n";
    
    // CWE-89: SQL Injection - Password Hint (from UserController.java)
    std::cout << "\n--- CWE-89: SQL Injection in Password Hint ---\n";
    std::string hint = UserController::get_password_hint_vulnerable(username);
    std::cout << "Password hint: " << hint << "\n";
    
    // CWE-89: SQL Injection - User Registration (from UserController.java)
    std::cout << "\n--- CWE-89: SQL Injection in Registration ---\n";
    UserController::register_user_vulnerable(username, password, "John Doe", "jdoe");
    
    // CWE-89: SQL Injection - User History (from UserController.java)
    std::cout << "\n--- CWE-89: SQL Injection in User History ---\n";
    std::vector<std::string> history = UserController::get_user_history_vulnerable(username);
    
    // CWE-89 + CWE-564: SQL Injection in ORDER BY (from BlabController.java)
    std::cout << "\n--- CWE-564: SQL Injection in ORDER BY ---\n";
    char sort_column[256];
    std::cout << "Enter sort column (e.g., 'username', 'blab_name'): ";
    std::cin.getline(sort_column, 256);
    BlabController::get_blabbers_vulnerable(username, sort_column);
    
    // CWE-78: OS Command Injection - Ping (from ToolsController.java)
    std::cout << "\n--- CWE-78: OS Command Injection (Ping) ---\n";
    char host_to_ping[256];
    std::cout << "Enter host to ping: ";
    std::cin.getline(host_to_ping, 256);
    std::string ping_result = ToolsController::ping_vulnerable(host_to_ping);
    std::cout << "Ping result:\n" << ping_result << "\n";
    
    // CWE-327: Weak Cryptography - MD5 (from UserController.java)
    std::cout << "\n--- CWE-327: Weak Cryptography (MD5) ---\n";
    std::string weak_hash = CryptoUtils::md5_hash_vulnerable(password);
    std::cout << "MD5 hash of password: " << weak_hash << "\n";
    
    // CWE-73: Path Traversal (from UserController.java)
    std::cout << "\n--- CWE-73: Path Traversal ---\n";
    char image_filename[256];
    std::cout << "Enter image filename: ";
    std::cin.getline(image_filename, 256);
    std::string file_content = FileController::download_profile_image_vulnerable(image_filename);
    std::cout << "File access result: " << file_content.substr(0, 100) << "...\n";
    
    // CWE-611: XML External Entity (XXE)
    std::cout << "\n--- CWE-611: XML External Entity (XXE) ---\n";
    std::string xml_data = "<?xml version=\"1.0\"?><!DOCTYPE foo [<!ENTITY xxe SYSTEM \"file:///etc/passwd\">]><data>&xxe;</data>";
    std::string xml_result = XMLParser::parse_xml_vulnerable(xml_data);
    std::cout << "XML parsing result: " << xml_result << "\n";
    
    std::cout << "\n=== END VERADEMO VULNERABILITIES ===\n\n";
    
    // VULNERABLE: Log completion with user data
    log_user_action(username, "Application completed successfully");
    
    return 0;
}
