#include <stdafx.h>
#include "XML.h"

namespace storage
{
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
