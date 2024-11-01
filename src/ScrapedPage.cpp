#include <iostream>
#include <format>
#include "ScrapedPage.h"
#include "WebScraperException.h"

namespace WebScraper
{
    ScrapedPage::ScrapedPage(std::string bodyText)
        : m_bodyText(bodyText)
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
            
            SerializeDOMTree(lxb_dom_interface_node(document));
            lxb_html_document_destroy(document);
        }
        catch(const WebScraperException& e)
        {
            if(document != nullptr) lxb_html_document_destroy(document);
            std::cerr << e.what() << std::endl;
        }
    }

    std::string ScrapedPage::GetBodyText()
    {
        return m_bodyText;
    }


    unsigned int SerializeDOM(const lxb_char_t *data, size_t len, void *ctx)
    {
        std::cout << std::format("{}", std::string(reinterpret_cast<char *>(const_cast<lxb_char_t *>(data)), len)) << std::flush;
        return LXB_STATUS_OK;
    }

    void ScrapedPage::SerializeDOMTree(lxb_dom_node_t* node)
    {
        if(node == nullptr) return;
        if(auto status = lxb_html_serialize_pretty_tree_cb(node, LXB_HTML_SERIALIZE_OPT_UNDEF, 0, SerializeDOM, nullptr); status != LXB_STATUS_OK)
        {
            throw(WebScraperException(std::format("Failed to serialize DOM tree with status: {}", status)));
        }
    }

}