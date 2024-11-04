#include <iostream>
#include <string>
#include <vector>
#include <format>

#include "beauty/beauty.hpp"

#include "WebScraper.h"

int main()
{
    WebScraper::WebScraper webScraper;
    std::vector<std::string_view> wikipediaArticles = {
        "https://en.wikipedia.org/wiki/Quantum_mechanics",
        "https://en.wikipedia.org/wiki/Artificial_intelligence",
        "https://en.wikipedia.org/wiki/History_of_Earth",
        "https://en.wikipedia.org/wiki/Computer_science",
        "https://en.wikipedia.org/wiki/Evolutionary_biology",
        "https://en.wikipedia.org/wiki/C%2B%2B",
        "https://en.wikipedia.org/wiki/Space_exploration",
        "https://en.wikipedia.org/wiki/Machine_learning",
        "https://en.wikipedia.org/wiki/Ancient_Egypt",
        "https://en.wikipedia.org/wiki/Cryptography",
        "https://en.wikipedia.org/wiki/Microcontroller",
        "https://en.wikipedia.org/wiki/Neural_network",
        "https://en.wikipedia.org/wiki/Embedded_system",
        "https://en.wikipedia.org/wiki/Game_theory",
        "https://en.wikipedia.org/wiki/Control_system"
    };
    webScraper.ScrapeURLs(wikipediaArticles);
    auto scrapedPage = webScraper.GetScrapedDataFromPage(wikipediaArticles[1]);
    for(auto text : scrapedPage->GetExtractedText())
    {
        std::cout << text.Serialize() << std::endl;
    }
    beauty::signal({SIGINT, SIGTERM, SIGHUP}, [](int s){
        std::cout << std::format("Gracefully stopping with signal {}", s) << std::endl;
        beauty::stop();
    });
    beauty::wait();
    return 0;
}