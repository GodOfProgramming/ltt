#include <spec.hpp>
#include "json.hpp"

BeginSpec(JSON);
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
                json::JSON doc;

                doc.set(1, "root1");
                doc.set(2, "root2", "child");
		doc.set(json::Object(), "root3");
		doc.set(json::Array(), "root4");

                auto out = doc.toString();
                Expect(out).toEqual(R"({"root1":1,"root2":{"child":2},"root3":{},"root4":[]})");

                int root1 = doc.get<int>("root1");
                int root2_child = doc.get<int>("root2", "child");

                Expect(root1).toEqual(1);
                Expect(root2_child).toEqual(2);
            });
        });
    });

    Context("getting values", [] {
        It("gets the values", [] {
            auto data = R"({
	    "main": {
	    "int": 1,
	    "float": 1.23,
	    "bool": true,
	    "string": "string"
	    }
	    })";

	    json::JSON doc;
	    doc.parse(data);

	    int i = doc.get<int>("main", "int");
	    float f = doc.get<float>("main", "float");
	    bool b = doc.get<bool>("main", "bool");
	    std::string s = doc.get<std::string>("main", "string");

	    Expect(i).toEqual(1);
	    Expect(f).toEqual(1.23f);
	    Expect(b).toEqual(true);
	    Expect(s).toEqual("string");
        });
    });
});
EndSpec();
