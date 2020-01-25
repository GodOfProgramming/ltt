#pragma once
#include <vector>
#include <sstream>
#include <rapidxml.hpp>
#include "node.h"
#include "attribute.h"

namespace xml
{
    using XMLDoc = rapidxml::xml_document<>;

    class XML
    {
       public:
        XML() = default;
        ~XML() = default;

        /* Copy Constructor */
        inline XML(const XML& xml)
        {
            if (xml.mParsed) {
                // TODO ineffecient af
                this->parse(std::string(mInternal.begin(), mInternal.end()));
            }
        }

        inline bool parse(std::string data)
        {
            mInternal.resize(data.length() + 1);
            std::fill(mInternal.begin(), mInternal.end(), '\0');
            std::copy(data.begin(), data.end(), mInternal.begin());
            std::stringstream error_stream;

            try {
                mDoc.parse<0>(mInternal.data());
                return mValid = true;
            } catch (const std::runtime_error& e) {
                error_stream << "XML runtime error: " << e.what();
            } catch (const rapidxml::parse_error& e) {
                error_stream << "XML parse error: " << e.what();
            } catch (const std::exception& e) {
                error_stream << "XML exception: " << e.what();
            } catch (...) {
                error_stream << "An unknown XML error occurred.";
            }

            mErr = error_stream.str();

            return mValid = false;
        }

        inline Node root()
        {
            return mDoc.first_node();
        }

        inline void forEach(std::function<void(Node)> callback)
        {
            Node::ForEachChild(mDoc.first_node(), callback);
        }

        inline std::string err()
        {
            return mErr;
        }

       private:
        XMLDoc mDoc;
        std::vector<char> mInternal;
        bool mParsed = false;
        bool mValid = false;

        std::string mErr;
    };
}  // namespace xml
