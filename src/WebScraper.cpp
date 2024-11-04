#include <algorithm>
#include <chrono>
#include <iostream>
#include <format>
#include <utility>

#include "beauty/beauty.hpp"
#include "WebScraper.h"
#include "WebScraperException.h"

namespace WebScraper
{  
    void WebScraper::ScrapeURLs(std::vector<std::string_view> urls)
    {
        beauty::client client;
        WaitAsyncs();
        std::for_each(urls.begin(), urls.end(), [this, &client](std::string_view url){
            m_scrapedPages[url] = std::make_pair<std::future<void>, IScrapedPagePtr>(std::async(std::launch::async, [this, url, &client](){
                auto [ec, response] = client.get(std::string(url));
                if(!ec)
                {
                    if(response.status() == boost::beast::http::status::ok)
                    {
                        std::cout << std::format("Scrapped {}", url) << std::endl;
                        m_scrapedPages[url].second = std::make_shared<ScrapedPage>(response.body());
                    }
                    else
                    {
                        std::cerr << std::format("Non OK status from response: {}({})", std::string(response.reason()), static_cast<int>(response.status())) << std::endl;
                    }
                }
                else
                {
                    std::cerr << std::format("Erro when sending request to: {} error: {}", url, ec.message()) << std::endl;
                }
            }), nullptr);
        });
    }
   
    IScrapedPagePtr WebScraper::GetScrapedDataFromPage(std::string_view url)
    {
        if(auto search = m_scrapedPages.find(url); search != m_scrapedPages.end())
        {
            if(search->second.second == nullptr)
            {
                search->second.first.get();
            }
            return search->second.second;
        }
        return nullptr;
    }


    void WebScraper::WaitAsyncs()
    {
        std::for_each(m_scrapedPages.begin(), m_scrapedPages.end(), [](auto& thread){
            using namespace std::chrono_literals;
            if(!thread.second.first.valid()) return;
            auto ret = thread.second.first.wait_for(10ms);
            if(ret == std::future_status::ready)
            {
                thread.second.first.get();
            }
            else if(ret == std::future_status::timeout)
            {
            }
        });
        m_scrapedPages.clear();
    }

}