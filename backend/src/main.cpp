#include "crow.h"

#include <cstdlib>
#include <fstream>
#include <mutex>
#include <regex>
#include <string>

namespace {
std::mutex file_mutex;
const std::regex email_pattern(R"(^[^\s@]+@[^\s@]+\.[^\s@]+$)");

std::string csv_escape(std::string value) {
    std::string escaped;
    for (char character : value) {
        if (character == '"') escaped += "\"\"";
        else if (character != '\r' && character != '\n') escaped += character;
        else escaped += ' ';
    }
    return '"' + escaped + '"';
}

void apply_cors(crow::response& response) {
    const char* configured_origin = std::getenv("ALLOWED_ORIGIN");
    response.add_header("Access-Control-Allow-Origin", configured_origin ? configured_origin : "*");
    response.add_header("Access-Control-Allow-Headers", "Content-Type");
    response.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
}
}  // namespace

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/health")([] {
        return crow::json::wvalue{{"status", "ok"}, {"service", "jovias-beadary-api"}};
    });

    CROW_ROUTE(app, "/api/inquiries").methods(crow::HTTPMethod::OPTIONS)(
        [](const crow::request&) {
            crow::response response(204);
            apply_cors(response);
            return response;
        });

    CROW_ROUTE(app, "/api/inquiries").methods(crow::HTTPMethod::POST)(
        [](const crow::request& request) {
            crow::response response;
            apply_cors(response);

            const auto body = crow::json::load(request.body);
            if (!body || !body.has("name") || !body.has("email") ||
                !body.has("interest") || !body.has("message")) {
                response.code = 400;
                response.body = R"({"error":"All fields are required."})";
                return response;
            }

            const std::string name = body["name"].s();
            const std::string email = body["email"].s();
            const std::string interest = body["interest"].s();
            const std::string message = body["message"].s();

            if (name.empty() || name.size() > 100 || message.empty() ||
                message.size() > 2000 || !std::regex_match(email, email_pattern)) {
                response.code = 422;
                response.body = R"({"error":"Please provide valid inquiry details."})";
                return response;
            }

            {
                std::lock_guard<std::mutex> lock(file_mutex);
                std::ofstream file("inquiries.csv", std::ios::app);
                if (!file) {
                    response.code = 500;
                    response.body = R"({"error":"Unable to save the inquiry."})";
                    return response;
                }
                file << csv_escape(name) << ',' << csv_escape(email) << ','
                     << csv_escape(interest) << ',' << csv_escape(message) << '\n';
            }

            response.code = 201;
            response.set_header("Content-Type", "application/json");
            response.body = R"({"message":"Inquiry received. Thank you!"})";
            return response;
        });

    const char* port_value = std::getenv("PORT");
    const uint16_t port = port_value ? static_cast<uint16_t>(std::stoi(port_value)) : 8080;
    app.port(port).multithreaded().run();
}
