#pragma once
#include <memory>
#include <vector>
#include <string_view>
#include <chrono>

#include "ScrapedPage.h"

namespace WebScraper
{
    class IWebScraper
    {
    public:
        virtual ~IWebScraper() = default;

        virtual void ScrapeURLs(std::vector<std::string_view> urls) = 0;
        virtual std::string GetPageData(std::string_view url, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(100)) = 0;
    };
    using IWebScraperPtr = std::shared_ptr<IWebScraper>;
}