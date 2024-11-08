#include <algorithm>
#include <chrono>
#include <iostream>
#include <format>
#include <iterator>
#include <utility>
#include <fstream>

#include "beauty/beauty.hpp"
#include "WebScraper.h"
#include "WebScraperException.h"

namespace WebScraper
{  
    WebScraper::WebScraper(std::shared_ptr<std::atomic_bool> killSignal, unsigned int threadQty) :
            m_killSignal(std::move(killSignal))
    {
        m_workerThreads.resize(threadQty);
        for(auto& elem : m_workerThreads)
        {
            elem.first = std::make_shared<std::future<void>>(std::async(std::launch::async, [this, &elem](){
                while(!m_killSignal->load())
                {
                    if(auto url = elem.second.pop())
                    {
                        ScrapURL(*url);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }));
        }

    }

    void WebScraper::ScrapeURLs(std::vector<std::string_view> urls)
    {
        for(auto index = 0; index < urls.size(); ++index)
        {
            m_scrapedPages[urls[index]] = nullptr;
            m_workerThreads.at(index % m_workerThreads.size()).second.push(static_cast<std::string>(urls[index]));
        }
    }
   
    std::string WebScraper::GetPageData(std::string_view url, const std::chrono::milliseconds& timeout)
    {
        std::string returnString;
        auto startTime = std::chrono::steady_clock::now();

        while((GetScrapedDataFromPage(url) == nullptr) && (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime) < timeout));
        auto page = GetScrapedDataFromPage(url);
        if(page == nullptr)
        {
            return "Empty";
        }

        for(auto text : page->GetExtractedText())
        {
            returnString += text.Serialize();
        }
        return returnString;
    }

    void WebScraper::ScrapURL(const std::string_view& url)
    {
        beauty::client client;
        beauty::request req;
        req.set("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36");
        req.method(beast::http::verb::get);
        
        auto [ec, response] = client.send_request(std::move(req), std::chrono::milliseconds(0), std::string(url));
        if(!ec)
        {
            if(response.status() == boost::beast::http::status::ok)
            {
                std::cout << std::format("Scrapped {}", url) << std::endl;
                m_scrapedPages[url] = std::make_shared<ScrapedPage>(response.body());
            }
            else
            {
                std::cerr << std::format("Non OK status {} from response: {}({})", url,std::string(response.reason()), static_cast<int>(response.status())) << std::endl;
            }
        }
        else
        {
            std::cerr << std::format("Erro when sending request to: {} error: {}", url, ec.message()) << std::endl;
        }
    }

    void WebScraper::GetAllThreads()
    {
        for(auto& thread : m_workerThreads)
        {
            while(!thread.second.empty());
        }
    }

    IScrapedPagePtr WebScraper::GetScrapedDataFromPage(std::string_view url)
    {
        return m_scrapedPages[url];
    }

}