#pragma once
#include <memory>
#include <vector>
#include <string>

namespace WebScraper
{
    class IScrapedPage
    {
    public:
        virtual ~IScrapedPage() = default;

        virtual std::string GetBodyText() = 0;
    };
    using IScrapedPagePtr = std::shared_ptr<IScrapedPage>;
}