#pragma once
#include <rapidxml.hpp>

namespace xml
{
    using XMLAttribute = rapidxml::xml_attribute<>;

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

}  // namespace xml
