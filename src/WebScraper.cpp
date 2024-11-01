#include <algorithm>
#include <chrono>
#include <iostream>
#include <format>

#include "beauty/beauty.hpp"
#include "WebScraper.h"



namespace WebScraper
{  
    void WebScraper::ScrapeURLs(std::vector<std::string_view> urls)
    {
        beauty::client client;
        WaitAsyncs();
        std::for_each(urls.begin(), urls.end(), [this, &client](std::string_view url){
            m_scrapedPageThreads.push_back(std::async(std::launch::async, [this, url, &client](){
                auto [ec, response] = client.get(std::string(url));
                if(!ec)
                {
                    if(response.status() == boost::beast::http::status::ok)
                    {
                        std::cout << std::format("Scrapped {}", url) << std::endl;
                        m_scrapedPages[url] = ScrapedPage(response.body());
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
            }));
        });
    }
   
    IScrapedPagePtr WebScraper::GetScrapedDataFromPage(std::string_view url)
    {
        WaitAsyncs();
        if(auto search = m_scrapedPages.find(url); search != m_scrapedPages.end())
        {
            return std::make_shared<ScrapedPage>(search->second);
        }
        return nullptr;
    }


    void WebScraper::WaitAsyncs()
    {
        std::for_each(m_scrapedPageThreads.begin(), m_scrapedPageThreads.end(), [](auto& thread){
            using namespace std::chrono_literals;
            if(!thread.valid()) return;
            auto ret = thread.wait_for(100ms);
            if(ret == std::future_status::ready)
            {
                thread.get();
            }
            else if(ret == std::future_status::timeout)
            {
            }
        });
        m_scrapedPageThreads.clear();
    }

}