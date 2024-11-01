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
        std::map<std::string_view, ScrapedPage> m_scrapedPages;
        std::vector<std::future<void>> m_scrapedPageThreads;

        void WaitAsyncs();
    };
    using WebScraperPtr = std::shared_ptr<WebScraper>;
}