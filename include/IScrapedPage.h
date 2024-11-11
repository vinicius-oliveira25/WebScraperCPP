#pragma once
#include <memory>
#include <vector>
#include <string>
#include <format>

namespace WebScraper
{
    struct FontTextData
    {
        FontTextData(std::string _fontSize, std::string _text) :
            fontSize(_fontSize), text(_text)
        {}
        std::string fontSize;
        std::string text;

        std::string Serialize()
        {
            return std::format("fontSize: {}. text: {}\n", fontSize, text);
        }
    };

    struct KeywordData
    {
        KeywordData() :
            occurrences(0)
        {}

        int occurrences;
        std::vector<std::pair<int,int>> position;
    };

    class IScrapedPage
    {
    public:
        virtual ~IScrapedPage() = default;

        virtual std::string GetUrl() = 0;
        virtual std::string GetBodyText() = 0;
        virtual std::vector<FontTextData> GetExtractedText() = 0;
        virtual KeywordData GetKeyword(std::string keyword) = 0;
    };
    using IScrapedPagePtr = std::shared_ptr<IScrapedPage>;
}