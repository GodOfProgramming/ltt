#pragma once
#include <functional>
#include <rapidxml.hpp>
#include "attribute.h"

namespace xml
{
    using XMLNode = rapidxml::xml_node<>;
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

        /* For children */
        inline void forEach(std::function<void(Node)> callback)
        {
            ForEachChild(mInternal->first_node(), callback);
        }

        /* For attributes */
        inline void forEach(std::function<void(Attribute)> callback)
        {
            for (auto attr = mInternal->first_attribute(); attr; attr = attr->next_attribute()) {
                callback(attr);
            }
        }

        inline Node findFirstNode(std::string name) {
            if (Exists) {
                auto child = mInternal->first_node(name.c_str());
                if (child) {
                    return Node(child);
                }
            }

            return Node(nullptr);
        }

        inline Node findLastNode(std::string name) {
            if (Exists) {
                auto child = mInternal->last_node(name.c_str());
                if (child) {
                    return Node(child);
                }
            }

            return Node(nullptr);
        }

        inline Attribute findFirstAttr(std::string name)
        {
            return Exists ? mInternal->first_attribute(name.c_str()) : Attribute(nullptr);
        }

        const std::string Name;
        const std::string Value;
        const bool Exists = false;

       private:
        friend class XML;

        inline static void ForEachChild(XMLNode* node, std::function<void(Node)> callback)
        {
            callback(node);
            if (node->first_node()) {
                ForEachChild(node->first_node(), callback);
            }
            if (node->parent() && node->next_sibling()) {
                ForEachChild(node->next_sibling(), callback);
            }
        }
    };

}  // namespace xml
