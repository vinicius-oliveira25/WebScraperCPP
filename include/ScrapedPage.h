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
        std::vector<FontTextData> GetExtractedText() final;
    private:
        std::string m_bodyText;
        std::vector<FontTextData> m_extractedText;
        
        void ExtractPageTextElements(lxb_dom_node_t* node);
        void GetFontSize(lxb_dom_node_t* node, std::string& fontSize);
    };
    using ScrapedPagePtr = std::shared_ptr<ScrapedPage>;
}