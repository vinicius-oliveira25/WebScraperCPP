#pragma once
#include <memory>
#include <vector>
#include <string_view>
#include <sstream>
#include <chrono>

#include "ScrapedPage.h"

namespace WebScraper
{
    struct UpperKeywordData
    {
        UpperKeywordData() : 
            totalOccurences(0)
        {}

        std::string Serialize()
        {
            std::stringstream retString;
            retString << "Total occurrences: " << totalOccurences << std::endl;
            for(auto index = 0; index < keywordDataPerURL.size(); ++index)
            {
                retString << "In Url: " << keywordDataPerURL[index].first << std::endl;
                retString << "  Occurrences: " << keywordDataPerURL[index].second.occurrences << std::endl;
                retString << "  Positions: " << std::endl;
                for(auto lower_index = 0; lower_index < keywordDataPerURL[index].second.position.size(); ++lower_index)
                {
                    retString << "      [line: " << keywordDataPerURL[index].second.position[lower_index].first << " offset: " 
                        << keywordDataPerURL[index].second.position[lower_index].second << "]" << std::endl;
                } 
            }
            return retString.str();
        }
        int totalOccurences;
        std::vector<std::pair<std::string, KeywordData>> keywordDataPerURL;
    };

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

    class IWebScraper
    {
    public:
        virtual ~IWebScraper() = default;

        virtual void ScrapeURLs(std::vector<std::string_view> urls) = 0;
        virtual std::string GetPageData(std::string_view url, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(3000)) = 0;
        virtual UpperKeywordData GetKeywordData(std::string keyword) = 0;
    };
    using IWebScraperPtr = std::shared_ptr<IWebScraper>;
}