#include <stdafx.h>
#include "JSON.h"
#include <util/logging/Logger.h>
#include <util/structures/Dimention.h>
#include <util/structures/Point.h>
#include <util/structures/Range.h>
#include <util/structures/Ratio.h>

namespace storage
{
    JSON::JSON()
    {
        mDoc.SetObject();  // default document to type object
    }

    bool JSON::parse(const std::string& data)
    {
        rapidjson::ParseResult result = mDoc.Parse(data.c_str());

        if (!result && mDoc.HasParseError()) {
            auto error = mDoc.GetParseError();
            Log("Document parse error: ", error);
            return false;
        }

        return true;
    }

    std::string JSON::toString()
    {
        rapidjson::StringBuffer buff;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
        mDoc.Accept(writer);

        return buff.GetString();
    }

    std::string JSON::toPrettyString()
    {
        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> pwriter(buff);
        mDoc.Accept(pwriter);

        return buff.GetString();
    }

    /* Setters */

    template <>
    void JSON::setValue(rapidjson::Value* member, std::string str)
    {
        member->SetString(rapidjson::StringRef(str.c_str()), mDoc.GetAllocator());
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, const char* str)
    {
        member->SetString(rapidjson::StringRef(str), mDoc.GetAllocator());
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, int i)
    {
        member->SetInt(i);
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, unsigned int i)
    {
        member->SetUint(i);
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, bool b)
    {
        member->SetBool(b);
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, float f)
    {
        member->SetFloat(f);
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, rapidjson::Value* value)
    {
        member->SetObject();
        *member = rapidjson::Value(*value, mDoc.GetAllocator());
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, structures::Dimention<int> value)
    {
        member->SetObject();

        if (member->HasMember("width")) {
            (*member)["width"].SetInt(value.Width);
        } else {
            member->AddMember("width", value.Width, mDoc.GetAllocator());
        }

        if (member->HasMember("height")) {
            (*member)["height"].SetInt(value.Height);
        } else {
            member->AddMember("height", value.Height, mDoc.GetAllocator());
        }
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, structures::Point<int> value)
    {
        member->SetObject();

        if (member->HasMember("x")) {
            (*member)["x"].SetInt(value.x);
        } else {
            member->AddMember("x", value.x, mDoc.GetAllocator());
        }

        if (member->HasMember("y")) {
            (*member)["y"].SetInt(value.y);
        } else {
            member->AddMember("y", value.y, mDoc.GetAllocator());
        }
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, structures::Range<float> value)
    {
        member->SetObject();

        if (member->HasMember("start")) {
            (*member)["start"].SetFloat(value.Begin);
        } else {
            member->AddMember("start", value.Begin, mDoc.GetAllocator());
        }

        if (member->HasMember("end")) {
            (*member)["end"].SetFloat(value.End);
        } else {
            member->AddMember("end", value.End, mDoc.GetAllocator());
        }
    }

    template <>
    void JSON::setValue(rapidjson::Value* member, structures::Ratio<int> value)
    {
        member->SetObject();

        if (member->HasMember("numerator")) {
            (*member)["numerator"].SetInt(value.Top);
        } else {
            member->AddMember("numerator", value.Top, mDoc.GetAllocator());
        }

        if (member->HasMember("denominator")) {
            (*member)["denominator"].SetInt(value.Bottom);
        } else {
            member->AddMember("denominator", value.Bottom, mDoc.GetAllocator());
        }
    }

    /* Getters */

    template <>
    rapidjson::Value* JSON::getValue(rapidjson::Value* member)
    {
        return member;
    }

    template <>
    std::string JSON::getValue(rapidjson::Value* member)
    {
        return member && member->IsString() ? std::string(member->GetString()) : std::string();
    }

    template <>
    const char* JSON::getValue(rapidjson::Value* member)
    {
        return member && member->IsString() ? member->GetString() : "";
    }

    template <>
    int JSON::getValue(rapidjson::Value* member)
    {
        return member && member->IsInt() ? member->GetInt() : 0;
    }

    template <>
    unsigned int JSON::getValue(rapidjson::Value* member)
    {
        return member && member->IsUint() ? member->GetUint() : 0;
    }

    template <>
    bool JSON::getValue(rapidjson::Value* member)
    {
        return member && member->IsBool() ? member->GetBool() : false;
    }

    template <>
    float JSON::getValue(rapidjson::Value* member)
    {
        return member && member->IsFloat() ? member->GetFloat() : 0.0f;
    }

    template <>
    structures::Dimention<int> JSON::getValue(rapidjson::Value* member)
    {
        structures::Dimention<int> retval = { 0, 0 };

        if (member->GetType() == rapidjson::Type::kObjectType) {
            if (member->HasMember("width")) {
                retval.Width = (*member)["width"].GetInt();
            }

            if (member->HasMember("height")) {
                retval.Height = (*member)["height"].GetInt();
            }
        }

        return retval;
    }

    template <>
    structures::Point<int> JSON::getValue(rapidjson::Value* member)
    {
        structures::Point<int> retval = { 0, 0 };

        if (member->GetType() == rapidjson::Type::kObjectType) {
            if (member->HasMember("x")) {
                retval.x = (*member)["x"].GetInt();
            }

            if (member->HasMember("y")) {
                retval.y = (*member)["y"].GetInt();
            }
        }

        return retval;
    }

    template <>
    structures::Range<float> JSON::getValue(rapidjson::Value* member)
    {
        structures::Range<float> retval = { 0.0f, 0.0f };

        if (member->GetType() == rapidjson::Type::kObjectType) {
            if (member->HasMember("start")) {
                retval.Begin = (*member)["start"].GetFloat();
            }

            if (member->HasMember("end")) {
                retval.End = (*member)["end"].GetFloat();
            }
        }

        return retval;
    }

    template <>
    structures::Ratio<int> JSON::getValue(rapidjson::Value* member)
    {
        structures::Ratio<int> retval = { 0, 0 };

        if (member->GetType() == rapidjson::Type::kObjectType) {
            if (member->HasMember("numerator")) {
                retval.Top = (*member)["numerator"].GetInt();
            }

            if (member->HasMember("denominator")) {
                retval.Bottom = (*member)["denominator"].GetInt();
            }
        }

        return retval;
    }
}  // namespace storage