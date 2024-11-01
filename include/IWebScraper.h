#pragma once
#include <memory>
#include <vector>
#include <string_view>
#include "ScrapedPage.h"

namespace WebScraper
{
    class IWebScraper
    {
    public:
        virtual ~IWebScraper() = default;

        virtual void ScrapeURLs(std::vector<std::string_view> urls) = 0;
        virtual IScrapedPagePtr GetScrapedDataFromPage(std::string_view url) = 0;
    };
    using IWebScraperPtr = std::shared_ptr<IWebScraper>;
}