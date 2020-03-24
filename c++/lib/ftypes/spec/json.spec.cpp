#include "ftypes/json.hpp"
#include <cstdlib>

#include <cspec/cspec.hpp>

Eval(JSON)
{
  Describe("class JSON", [] {
    auto data = R"({
					"main": {
						"int": 1,
						"float": 1.23,
						"bool": true,
						"string": "string",
						"object": {
							"value": 1
						},
            "array": [
              1, 2, 3
            ]
					}
				})";

    Context("parse()", [] {
      Context("invalid document", [] {
        const char* invalid_json = R"({ "foo": 0 )";  // no closing brace
        It("returns false", [&] {
          ftypes::JSON doc;
          Expect(doc.parse(invalid_json)).toEqual(false);
        });
      });
      Context("valid document", [] {
        const char* valid_json = R"({ "foo": 0 })";
        It("returns true", [&] {
          ftypes::JSON doc;
          Expect(doc.parse(valid_json)).toEqual(true);
        });
      });
    });

    Context("setting values", [] {
      Context("new document", [] {
        It("sets", [] {
          ftypes::JSON doc, other, array, arrayVal, integer;

          arrayVal.set(1, "value");

          array.setArray();

          array.push("1", 2, "3", "value", arrayVal);

          doc.set("1", "root1");
          doc.set(2, "root2", "child");
          doc.set(ftypes::JSON::Object(), "root3");
          doc.set(ftypes::JSON::Array(), "root4");

          other.set(1, "a_value");
          other.set(2, "b_value", "c_value");

          doc.set(other, "other");
          doc.set(array, "array");

          auto out = doc.toString();
          Expect(out).toEqual(
           R"({"root1":"1","root2":{"child":2},"root3":{},"root4":[],"other":{"a_value":1,"b_value":{"c_value":2}},"array":["1",2,"3","value",{"value":1}]})");

          const char* root1 = doc.get<const char*>("root1");
          int root2_child = doc.get<int>("root2", "child");

          Expect(root1).toEqual("1");
          Expect(root2_child).toEqual(2);

          integer.set(1);
          Expect(integer.get<int>()).toEqual(1);
        });
      });
    });

    Context("getting values", [&] {
      It("gets the values", [&] {
        ftypes::JSON doc;
        doc.parse(data);

        int i = doc.get<int>("main", "int");
        float f = doc.get<float>("main", "float");
        bool b = doc.get<bool>("main", "bool");
        std::string s = doc.get<std::string>("main", "string");
        ftypes::JSON object = doc.get<ftypes::JSON>("main", "object");
        ftypes::JSON array = doc.get<ftypes::JSON>("main", "array");

        Expect(i).toEqual(1);
        Expect(f).toEqual(1.23f);
        Expect(b).toEqual(true);
        Expect(s).toEqual("string");
        Expect(object.get<int>("value")).toEqual(1);

        int val = 0;
        array.foreach ([&val](rapidjson::Value& value) -> void {
          Expect(value.Get<int>()).toEqual(val++ + 1);
        });
        Expect(val).toEqual(3);
      });
    });

    Context("memberType()", [&] {
      It("returns the right type", [&] {
        ftypes::JSON doc;
        doc.parse(data);

        Expect(doc.memberIs(ftypes::JSON::Type::Number, "main", "int")).toEqual(true);
        Expect(doc.memberIs(ftypes::JSON::Type::Number, "main", "float")).toEqual(true);
        Expect(doc.memberIs(ftypes::JSON::Type::True, "main", "bool")).toEqual(true);
        Expect(doc.memberIs(ftypes::JSON::Type::String, "main", "string")).toEqual(true);
        Expect(doc.memberIs(ftypes::JSON::Type::Object, "main", "object")).toEqual(true);
        Expect(doc.memberIs(ftypes::JSON::Type::Array, "main", "array")).toEqual(true);

        ftypes::JSON object = doc.get<ftypes::JSON>("main", "object");
        Expect(object.memberIs(ftypes::JSON::Type::Number, "value")).toEqual(true);
        Expect(doc.memberIs(ftypes::JSON::Type::Object, "main", "object")).toEqual(true);

        ftypes::JSON array = doc.get<ftypes::JSON>("main", "array");
        Expect(doc.memberIs(ftypes::JSON::Type::Array, "main", "array")).toEqual(true);

        int val = 0;
        array.foreach ([&val](rapidjson::Value& value) -> void {
          Expect(value.GetType()).toEqual(rapidjson::Type::kNumberType);
          val++;
        });
        Expect(val).toEqual(3);

        ftypes::JSON null = doc.get<ftypes::JSON>("main", "doesnotexist");
        Expect(null.memberIs(ftypes::JSON::Type::Null)).toEqual(true);
      });
    });
  });

  Describe("rapidjson tests", [] {
    It("compiles", [] {
      rapidjson::Document d, x;
      d.SetObject();
      rapidjson::Value v;
      v.SetArray();
      rapidjson::Value a;
      a.Set(1);
      v.PushBack(a, x.GetAllocator());
      a.Swap(d);
    });
  });
}
