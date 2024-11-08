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
    template<typename Type>
    class ThreadSafeQueue
    {
    public:
        ThreadSafeQueue() = default;
        ThreadSafeQueue(const ThreadSafeQueue& other)
        {
            this->m_queue = other.m_queue;
        }

        std::optional<Type> pop()
        {
            std::lock_guard lock(m_mutex);
            if(m_queue.empty())
            {
                return std::nullopt;
            }
            auto value = m_queue.front();
            m_queue.pop();
            return value;
        }

        void push(const Type& value)
        {
            std::lock_guard lock(m_mutex);
            m_queue.push(value);
        }

        std::size_t size() const
        {
            std::lock_guard lock(m_mutex);
            return m_queue.size();
        }

        bool empty() const
        {
            return (size() == 0) ? true : false;
        }

    private:
        mutable std::mutex m_mutex;
        std::queue<Type> m_queue;
    };
    using ThreadSafeQueueString = ThreadSafeQueue<std::string>;

    class WebScraper : public IWebScraper
    {
    public:
        explicit WebScraper(std::shared_ptr<std::atomic_bool> killSignal, unsigned int threadQty = 1);
    
        void ScrapeURLs(std::vector<std::string_view> urls) final;
        std::string GetPageData(std::string_view url, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(100)) final;
    private:
        mutable std::mutex m_mutex;
        std::shared_ptr<std::atomic_bool> m_killSignal;
        std::map<std::string_view, IScrapedPagePtr> m_scrapedPages;
        std::vector<std::pair<std::shared_ptr<std::future<void>>, ThreadSafeQueueString>> m_workerThreads;

        IScrapedPagePtr GetScrapedDataFromPage(std::string_view url);
        void ScrapURL(const std::string_view& url);
        void GetAllThreads();
    };
    using WebScraperPtr = std::shared_ptr<WebScraper>;
}