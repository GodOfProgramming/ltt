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
        Node(XMLNode* node);

        /* For children */
        void forEach(std::function<void(Node)> callback);

        /* For attributes */
        void forEach(std::function<void(Attribute)> callback);

        Node findFirstNode(std::string name);

        Node findLastNode(std::string name);

        inline Attribute findFirstAttr(std::string name);

        const std::string Name;
        const std::string Value;
        const bool Exists = false;

       private:
        friend class XML;

        static void ForEachChild(XMLNode* node, std::function<void(Node)> callback);
    };

    inline Node::Node(XMLNode* node) : mInternal(node), Exists(!!node)
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

    inline void Node::forEach(std::function<void(Node)> callback)
    {
        ForEachChild(mInternal->first_node(), callback);
    }

    inline void Node::forEach(std::function<void(Attribute)> callback)
    {
        for (auto attr = mInternal->first_attribute(); attr; attr = attr->next_attribute()) {
            callback(attr);
        }
    }

    inline Node Node::findFirstNode(std::string name)
    {
        if (Exists) {
            auto child = mInternal->first_node(name.c_str());
            if (child) {
                return Node(child);
            }
        }

        return Node(nullptr);
    }

    inline Node Node::findLastNode(std::string name)
    {
        if (Exists) {
            auto child = mInternal->last_node(name.c_str());
            if (child) {
                return Node(child);
            }
        }

        return Node(nullptr);
    }

    inline Attribute Node::findFirstAttr(std::string name)
    {
        return Exists ? mInternal->first_attribute(name.c_str()) : Attribute(nullptr);
    }

    inline void Node::ForEachChild(XMLNode* node, std::function<void(Node)> callback)
    {
        callback(node);
        if (node->first_node()) {
            ForEachChild(node->first_node(), callback);
        }
        if (node->parent() && node->next_sibling()) {
            ForEachChild(node->next_sibling(), callback);
        }
    }
}  // namespace xml
