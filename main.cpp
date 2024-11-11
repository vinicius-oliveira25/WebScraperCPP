#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <atomic>
#include <chrono>
#include <algorithm>

#include "beauty/beauty.hpp"

#include "WebScraper.h"

int Helper(int argc, char *argv[])
{
    if(argc == 3)
    {
        auto argument = std::string(argv[1]);
        auto value = std::string(argv[2]);
        auto isDigit = std::all_of(value.begin(), value.end(), [](auto ch){
            return std::isdigit(ch);
        });

        if((argument == "-t" || argument == "--thread") && isDigit)
        {
            return std::stoi(value);
        }
    }

    std::cerr << "[ERROR] Argument missing: " << std::endl;
    std::cerr << "      -t / --threads <num> (Determining how many worker threads should function)" << std::endl;
    std::cerr << "ie: WebScraper -th 3" << std::endl;
    std::exit(0);
}

int main(int argc , char *argv[])
{
    auto threadsNum = Helper(argc, argv);
    auto killSignal = std::make_shared<std::atomic_bool>(false);
    WebScraper::WebScraper webScraper(killSignal, threadsNum);
    std::vector<std::string_view> wikipediaArticles = {
        "https://en.wikipedia.org/wiki/The_Eternaut",
        "https://en.wikipedia.org/wiki/El_Eternauta:_segunda_parte",
        "https://en.wikipedia.org/wiki/Quantum_mechanics"
    };
    webScraper.ScrapeURLs(wikipediaArticles);

#ifdef BENCHMARK
    auto startTime = std::chrono::steady_clock::now();
#endif 
    std::cout << webScraper.GetPageData(wikipediaArticles[0]) << std::endl;

#ifdef BENCHMARK
    auto endTime = std::chrono::steady_clock::now();
    std::cout << "Milliseconds since: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
#endif

    auto keyword = webScraper.GetKeywordData("Eternaut ");
    std::cout << keyword.Serialize() << std::endl;

    beauty::signal({SIGINT, SIGTERM, SIGHUP}, [&killSignal](int s){
        std::cout << std::format("Gracefully stopping with signal {}", s) << std::endl;
        killSignal->store(true);
        beauty::stop();
    });
    beauty::wait();
    return 0;
}