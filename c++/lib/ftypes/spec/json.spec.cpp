#include "ftypes/json.hpp"
#include <cstdlib>

#include <cspec.hpp>

Eval(JSON)
{
  Describe("class JSON", [] {
    Context("parse()", [] {
      Context("invalid document", [] {
        const char* invalid_json = R"({ "foo": 0 )";  // no closing brace
        It("returns false", [&] {
          json::JSON doc;
          Expect(doc.parse(invalid_json)).toEqual(false);
        });
      });
      Context("valid document", [] {
        const char* valid_json = R"({ "foo": 0 })";
        It("returns true", [&] {
          json::JSON doc;
          Expect(doc.parse(valid_json)).toEqual(true);
        });
      });
    });

    Context("setting values", [] {
      Context("new document", [] {
        It("sets and gets", [] {
          json::JSON doc, other, array, arrayVal;

          arrayVal.set(1, "value");

          array.setArray();

          array.push("1", 2, "3", "value", arrayVal);

          doc.set("1", "root1");
          doc.set(2, "root2", "child");
          doc.set(json::Object(), "root3");
          doc.set(json::Array(), "root4");

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
        });
      });
    });

    Context("getting values", [] {
      return;
      It("gets the values", [] {
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

        json::JSON doc;
        doc.parse(data);

        int i = doc.get<int>("main", "int");
        float f = doc.get<float>("main", "float");
        bool b = doc.get<bool>("main", "bool");
        std::string s = doc.get<std::string>("main", "string");
				json::JSON object = doc.get<json::JSON>("main", "object");
        json::JSON array = doc.get<json::JSON>("main", "array");

        Expect(i).toEqual(1);
        Expect(f).toEqual(1.23f);
        Expect(b).toEqual(true);
        Expect(s).toEqual("string");
				Expect(object.get<int>("value")).toEqual(1);
        Expect(object.at<int>(2)).toEqual(3);
      });
    });
  });

  Describe("rapidjson tests", [] {
    It("compiles and works", [] {
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
