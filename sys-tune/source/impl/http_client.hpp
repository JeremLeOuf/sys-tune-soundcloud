#pragma once

#include <switch.h>
#include <memory>
#include <string>

namespace tune::impl {

class HttpClient {
public:
    struct Response {
        long status_code = 0;
        std::string body;
        std::string content_type;
    };

    static Result Initialize();
    static void Exit();

    static Result Get(const std::string& url, Response& response);
    static Result Post(const std::string& url, const std::string& data, Response& response);

private:
    static bool s_initialized;
};

} // namespace tune::impl
