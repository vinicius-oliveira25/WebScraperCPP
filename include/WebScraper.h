#pragma once

#include <future>
#include <map>
#include "IWebScraper.h"
#include "ScrapedPage.h"

namespace WebScraper
{
    class WebScraper : public IWebScraper
    {
    public:
        explicit WebScraper() = default;
    
        void ScrapeURLs(std::vector<std::string_view> urls) final;
        IScrapedPagePtr GetScrapedDataFromPage(std::string_view url) final;
    private:
        std::map<std::string_view, std::pair<std::future<void>, IScrapedPagePtr>> m_scrapedPages;

        void WaitAsyncs();
    };
    using WebScraperPtr = std::shared_ptr<WebScraper>;
}