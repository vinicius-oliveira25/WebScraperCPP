#pragma once
#include <exception>
#include <string>

namespace WebScraper
{
    class WebScraperException : public std::exception
    {
    public:
        explicit WebScraperException(std::string message) : m_message(message) {}
        const char* what() const noexcept override { return m_message.c_str(); }
    private:
        std::string m_message;
    };
}