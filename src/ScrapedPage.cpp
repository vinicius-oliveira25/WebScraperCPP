#include <iostream>
#include <format>
#include <regex>
#include <algorithm>

#include "ScrapedPage.h"
#include "WebScraperException.h"

namespace WebScraper
{

    ScrapedPage::ScrapedPage(std::string url, std::string bodyText) :
        m_url(url), 
        m_bodyText(bodyText)
    {
        lxb_html_document_t *document = nullptr;
        try
        {
            document = lxb_html_document_create();
            if(document == nullptr)
            {
                throw(WebScraperException("Failed to create HTML document"));
            }

            auto status = lxb_html_document_parse(document, reinterpret_cast<unsigned char*>(const_cast<char *>(bodyText.c_str())), bodyText.size());
            if(status != LXB_STATUS_OK)
            {
                throw(WebScraperException(std::format("Failed to parse HTML document with status: {}", status)));
            }
            
            if(auto* body = lxb_html_document_body_element(document); body != nullptr)
            {
                ExtractPageTextElements(lxb_dom_interface_node(body));
            }
            
            lxb_html_document_destroy(document);
        }
        catch(const WebScraperException& e)
        {
            if(document != nullptr) lxb_html_document_destroy(document);
            std::cerr << e.what() << std::endl;
        }
    }

    std::string ScrapedPage::GetUrl()
    {
        return m_url;
    }

    std::string ScrapedPage::GetBodyText()
    {
        return m_bodyText;
    }

    std::vector<FontTextData> ScrapedPage::GetExtractedText()
    {
        return m_extractedText;
    }
    
    KeywordData ScrapedPage::GetKeyword(std::string keyword)
    {
        KeywordData keywordData;
        for(auto index = 0; index < m_extractedText.size(); ++index)
        {
            auto pos = m_extractedText[index].text.find(keyword, 0);
            while(pos != std::string::npos)
            {
                keywordData.occurrences++;
                keywordData.position.emplace_back(index, pos);
                pos = m_extractedText[index].text.find(keyword, pos + 1);
            }
        };
        return keywordData;
    }

    void ScrapedPage::ExtractPageTextElements(lxb_dom_node_t* node)
    {
        std::string fontSize = "default";
        auto verifyWhiteSpace = [](std::string str)
        {
            return std::all_of(str.begin(), str.end(), 
                [](auto ch) { 
                    return std::isspace(ch); 
                });
        };

        if(node->type == LXB_DOM_NODE_TYPE_TEXT)
        {
            auto* t = lxb_dom_node_text_content(node, nullptr);
            if(t != nullptr)
            {
                std::string tStr = reinterpret_cast<char *>(const_cast<lxb_char_t *>(t));

                if(tStr.size() != 0 && !verifyWhiteSpace(tStr))
                {
                    GetFontSize(node->parent, fontSize);
                    m_extractedText.push_back({fontSize, tStr});
                }
            }
        }
        auto* child = lxb_dom_node_first_child(node);
        while(child != nullptr)
        {
            ExtractPageTextElements(child);
            child = lxb_dom_node_next(child);
        }
    }

    void ScrapedPage::GetFontSize(lxb_dom_node_t* node, std::string& fontSize)
    {
        while(node)
        {
            if(node->type == LXB_DOM_NODE_TYPE_ELEMENT)
            {
                auto* elem = lxb_dom_interface_element(node);
                auto* attr = lxb_dom_element_get_attribute(elem, reinterpret_cast<lxb_char_t *>(const_cast<char *>("style")), 5, nullptr);
                if(attr)
                {
                    std::string style(reinterpret_cast<char *>(const_cast<lxb_char_t *>(attr)));
                    std::regex fontSizeRegex("font-size:\\s*([\\d\\.]+\\w+%?);?");
                    std::smatch match;
                    if(std::regex_search(style, match, fontSizeRegex))
                    {
                        fontSize = match[1].str();
                        return;
                    }
                }
            }
            node = lxb_dom_node_parent(node);
        }
    }


}