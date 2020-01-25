#pragma once
#include <vector>
#include <rapidxml.h>
#include "node.h"
#include "attribute.h"

namespace xml
{
    using XMLDoc = rapidxml::xml_document<>;

    class XML
    {
       public:
        XML() = default;
        XML(const XML& xml)
        {
            if (xml.mParsed) {
                // TODO ineffecient af
                this->parse(std::string(mInternal.begin(), mInternal.end()));
            }
        }
        ~XML() = default;

        bool parse(io::File& file);
        bool parse(std::string data);

        inline Node root()
        {
            return mDoc.first_node();
        }

        void forEach(std::function<void(Node)> callback);

       private:
        XMLDoc mDoc;
        std::vector<char> mInternal;
        bool mParsed = false;
        bool mValid = false;
    };
}  // namespace xml
