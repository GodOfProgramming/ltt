#pragma once
#include <rapidxml.h>

namespace xml
{
    using XMLAttribute = rapidxml::xml_attribute<>;

    class Attribute
    {
        XMLAttribute* mInternal;

       public:
        Attribute(XMLAttribute* attribute);

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

}  // namespace xml
