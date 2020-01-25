#include <stdafx.h>
#include "XML.h"

namespace
{
    /* Recursive */
    void forEachChild(storage::XMLNode* node, std::function<void(storage::XML::Node)> callback)
    {
        callback(node);
        if (node->first_node()) {
            forEachChild(node->first_node(), callback);
        }
        if (node->parent() && node->next_sibling()) {
            forEachChild(node->next_sibling(), callback);
        }
    }

}  // namespace

namespace storage
{
    XML::Node::Node(XMLNode* node) : mInternal(node), Exists(!!node)
    {
        if (node) {
            auto& name = const_cast<std::string&>(Name);
            if (node->name_size()) {
                name.assign(node->name(), node->name() + node->name_size());
            }

            auto& value = const_cast<std::string&>(Value);
            if (node->value_size()) {
                value.assign(node->value(), node->value() + node->value_size());
            }

            auto& exists = const_cast<bool&>(Exists);
            exists = true;
        }
    }

    XML::Node::Attribute::Attribute(XMLAttribute* attr) : mInternal(attr), Exists(!!attr)
    {
        if (attr) {
            auto& name = const_cast<std::string&>(Name);
            if (attr->name_size()) {
                name.assign(attr->name(), attr->name() + attr->name_size());
            }

            auto& value = const_cast<std::string&>(Value);
            if (attr->value_size()) {
                value.assign(attr->value(), attr->value() + attr->value_size());
            }

            auto& exists = const_cast<bool&>(Exists);
            exists = true;
        }
    }

    bool XML::parse(io::File& file)
    {
        if (!file.wasRead() && file.exists()) {
            if (!file.read()) {
                return mParsed = false;
            }
        }

        return parse(file.data());
    }

    bool XML::parse(std::string data)
    {
        mInternal.resize(data.length() + 1);
        std::fill(mInternal.begin(), mInternal.end(), '\0');
        std::copy(data.begin(), data.end(), mInternal.begin());

        try {
            mDoc.parse<0>(mInternal.data());
            return mValid = true;
        } catch (const std::runtime_error& e) {
            Error("XML runtime error: ", e.what());
        } catch (const rapidxml::parse_error& e) {
            Error("XML parse error: ", e.what());
        } catch (const std::exception& e) {
            Error("XML exception: ", e.what());
        } catch (...) {
            Error("An unknown XML error occurred.");
        }

        return mValid = false;
    }

    void XML::forEach(std::function<void(XML::Node)> callback)
    {
        forEachChild(mDoc.first_node(), callback);
    }

    void XML::Node::forEach(std::function<void(XML::Node)> callback)
    {
        forEachChild(mInternal->first_node(), callback);
    }

    void XML::Node::forEach(std::function<void(Attribute)> callback)
    {
        for (auto attr = mInternal->first_attribute(); attr; attr = attr->next_attribute()) {
            callback(attr);
        }
    }
}  // namespace storage
