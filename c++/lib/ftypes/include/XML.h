#pragma once
#include <iostream>
#include <rapidxml.hpp>

namespace storage
{
    using XMLDoc = rapidxml::xml_document<>;
    using XMLNode = rapidxml::xml_node<>;
    using XMLAttribute = rapidxml::xml_attribute<>;

    class XML
    {
       public:
        XML() = default;
        inline XML(const XML& xml)
        {
            if (xml.mParsed) {
                // TODO ineffecient af
                this->parse(std::string(mInternal.begin(), mInternal.end()));
            }
        }

        ~XML() = default;

        class Node
        {
            XMLNode* mInternal = nullptr;

           public:
            Node() = default;
            inline Node(XMLNode* node) : mInternal(node), Exists(!!node)
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

            const std::string Name;
            const std::string Value;
            const bool Exists = false;

            class Attribute
            {
                XMLAttribute* mInternal;

               public:
                inline Attribute(XMLAttribute* attr) : mInternal(attr), Exists(!!attr)
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

                const std::string Name;
                const std::string Value;
                const bool Exists;

                inline operator int()
                {
                    return std::stoi(Value);
                }

                inline operator std::string()
                {
                    return Value;
                }
            };

            /* For children */
            void forEach(std::function<void(Node)> callback);

            /* For attributes */
            void forEach(std::function<void(Attribute)> callback);

            inline Attribute findFirst(std::string name)
            {
                return Exists ? mInternal->first_attribute(name.c_str()) : Attribute(nullptr);
            }
        };

        inline bool parse(std::string data)
        {
            mInternal.resize(data.length() + 1);
            std::fill(mInternal.begin(), mInternal.end(), '\0');
            std::copy(data.begin(), data.end(), mInternal.begin());

            try {
                mDoc.parse<0>(mInternal.data());
                return mValid = true;
            } catch (const std::runtime_error& e) {
                Error("XML runtime error: ", e.what());
                std::cout << "XML runtime error: " << e.what() << '\n';
            } catch (const rapidxml::parse_error& e) {
                Error("XML parse error: ", e.what());
                std::cout << "XML parse error: " << e.what() << '\n';
            } catch (const std::exception& e) {
                Error("XML exception: ", e.what());
            } catch (...) {
                Error("An unknown XML error occurred.");
            }

            return mValid = false;
        }

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

        /* Recursive */
        static inline void forEachChild(storage::XMLNode* node, std::function<void(storage::XML::Node)> callback)
        {
            callback(node);
            if (node->first_node()) {
                forEachChild(node->first_node(), callback);
            }
            if (node->parent() && node->next_sibling()) {
                forEachChild(node->next_sibling(), callback);
            }
        }
    };
}  // namespace storage
