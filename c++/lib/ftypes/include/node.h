#pragma once

namespace xml
{
    using XMLNode = rapidxml::xml_node<>;
    class Node
    {
        XMLNode* mInternal = nullptr;

       public:
        Node() = default;
        Node(XMLNode* node);

        const std::string Name;
        const std::string Value;
        const bool Exists = false;

        /* For children */
        void forEach(std::function<void(Node)> callback);

        /* For attributes */
        void forEach(std::function<void(Attribute)> callback);

        inline Attribute findFirst(std::string name)
        {
            return Exists ? mInternal->first_attribute(name.c_str()) : Attribute(nullptr);
        }
    };

}  // namespace xml

