#pragma once
#include <memory>
#include <vector>
#include <string>
#include <format>

namespace WebScraper
{
    struct FontTextData
    {
        FontTextData(std::string fontSize, std::string text) :
            fontSize(fontSize), text(text)
        {}
        std::string fontSize;
        std::string text;

        std::string Serialize()
        {
            return std::format("fontSize: {}. text: {}", fontSize, text);
        }
    };

    class IScrapedPage
    {
    public:
        virtual ~IScrapedPage() = default;

        virtual std::string GetBodyText() = 0;
        virtual std::vector<FontTextData> GetExtractedText() = 0;
    };
    using IScrapedPagePtr = std::shared_ptr<IScrapedPage>;
}