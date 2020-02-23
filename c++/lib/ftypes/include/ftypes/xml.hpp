#pragma once
#include <vector>
#include <sstream>
#include <rapidxml-1.13/rapidxml.hpp>
#include "node.hpp"
#include "attribute.hpp"

namespace xml
{
    using XMLDoc = rapidxml::xml_document<>;

    class XML
    {
       public:
        XML() = default;
        ~XML() = default;

        /* Copy Constructor */
        XML(const XML& xml);

        bool parse(std::string data);

        Node root();

        void forEach(std::function<void(Node)> callback);

        Node findFirstNode(std::string name);

        Node findLastNode(std::string name);

        std::string err();

       private:
        XMLDoc mDoc;
        std::vector<char> mInternal;
        bool mParsed = false;
        bool mValid = false;

        std::string mErr;
    };

    inline XML::XML(const XML& xml)
    {
        if (xml.mParsed) {
            // TODO ineffecient af
            this->parse(std::string(mInternal.begin(), mInternal.end()));
        }
    }

    inline bool XML::parse(std::string data)
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

    inline Node XML::root()
    {
        return mDoc.first_node();
    }

    inline void XML::forEach(std::function<void(Node)> callback)
    {
        Node::ForEachChild(mDoc.first_node(), callback);
    }

    inline Node XML::findFirstNode(std::string name)
    {
        auto result = mDoc.first_node(name.c_str());
        return result ? result : Node(nullptr);
    }

    inline Node XML::findLastNode(std::string name)
    {
        auto result = mDoc.last_node(name.c_str());
        return result ? Node(result) : Node(nullptr);
    }

    inline std::string XML::err()
    {
        return mErr;
    }
}  // namespace xml
