#include <iostream>
#include <string>
#include "WebScraper.h"

int main()
{
    WebScraper::WebScraper webScraper;

    webScraper.ScrapeURLs({"https://g1.globo.com/mundo/noticia/2024/11/01/enchente-na-espanha-moradores-ficam-sem-comida-mortos.ghtml"});
    auto scrapedPage = webScraper.GetScrapedDataFromPage("https://g1.globo.com/mundo/noticia/2024/11/01/enchente-na-espanha-moradores-ficam-sem-comida-mortos.ghtml");
    // std::cout << scrapedPage->GetBodyText() << std::endl;
    return 0;
}