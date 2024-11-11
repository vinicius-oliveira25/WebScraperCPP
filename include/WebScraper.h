#pragma once

#include <future>
#include <map>
#include <string_view>
#include <vector>
#include <queue>
#include <mutex>
#include <algorithm>
#include <atomic>

#include "IWebScraper.h"
#include "ScrapedPage.h"

namespace WebScraper
{
    class WebScraper : public IWebScraper
    {
    public:
        explicit WebScraper(std::shared_ptr<std::atomic_bool> killSignal, unsigned int threadQty = 1);
    
        void ScrapeURLs(std::vector<std::string_view> urls) final;
        std::string GetPageData(std::string_view url, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(3000)) final;
        UpperKeywordData GetKeywordData(std::string keyword) final;
    private:
        mutable std::mutex m_mutex;
        std::shared_ptr<std::atomic_bool> m_killSignal;
        std::map<std::string, IScrapedPagePtr> m_scrapedPages;
        std::vector<std::pair<std::shared_ptr<std::future<void>>, ThreadSafeQueueString>> m_workerThreads;

        IScrapedPagePtr GetScrapedDataFromPage(std::string_view url);
        void ScrapURL(const std::string_view& url);
        void WaitAllQueues();
    };
    using WebScraperPtr = std::shared_ptr<WebScraper>;
}