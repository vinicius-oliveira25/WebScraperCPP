#pragma once
#include "lexbor/html/html.h"
#include "IScrapedPage.h"

namespace WebScraper
{
    class ScrapedPage : public IScrapedPage
    {
    public:
        ScrapedPage() = default;
        ScrapedPage(std::string bodyText);

        std::string GetBodyText() final;
    private:
        std::string m_bodyText;
        
        void SerializeDOMTree(lxb_dom_node_t* node);
    };
    using ScrapedPagePtr = std::shared_ptr<ScrapedPage>;
}