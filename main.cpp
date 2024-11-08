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

        if((argument == "-th" || argument == "--thread") && isDigit)
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
        "https://br.linkedin.com/company/pureai-official",
        "https://stackoverflow.com/questions/68867455/how-to-get-certificate-chain-to-show-on-google-chrome",
        "https://en.wikipedia.org/wiki/Quantum_mechanics",
        "https://en.wikipedia.org/wiki/Artificial_intelligence",
        "https://en.wikipedia.org/wiki/History_of_Earth",
        "https://en.wikipedia.org/wiki/Computer_science",
        "https://en.wikipedia.org/wiki/Evolutionary_biology",
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

#ifdef BENCHMARK
    auto startTime = std::chrono::steady_clock::now();
#endif 
    std::cout << webScraper.GetPageData(wikipediaArticles[3], std::chrono::milliseconds(3000)) << std::endl;
    std::cout << webScraper.GetPageData(wikipediaArticles[14], std::chrono::milliseconds(3000)) << std::endl;
    std::cout << webScraper.GetPageData(wikipediaArticles[11], std::chrono::milliseconds(3000)) << std::endl;
    std::cout << webScraper.GetPageData(wikipediaArticles[8], std::chrono::milliseconds(3000)) << std::endl;
    std::cout << webScraper.GetPageData(wikipediaArticles[13], std::chrono::milliseconds(3000)) << std::endl;
    std::cout << webScraper.GetPageData(wikipediaArticles[6], std::chrono::milliseconds(3000)) << std::endl;

#ifdef BENCHMARK
    auto endTime = std::chrono::steady_clock::now();
    std::cout << "Milliseconds since: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
#endif

    beauty::signal({SIGINT, SIGTERM, SIGHUP}, [&killSignal](int s){
        std::cout << std::format("Gracefully stopping with signal {}", s) << std::endl;
        killSignal->store(true);
        beauty::stop();
    });
    beauty::wait();
    return 0;
}