#include <spec.hpp>
#include "json.h"

BeginSpec(JSON);
Describe("class JSON", [] {
    Context("parse()", [] {
        Context("invalid document", [] {
            const char* invalid_json = "{ \"foo\": 0 ";  // no closing brace
            It("returns false", [&] {
                json::JSON doc;
                Expect(doc.parse(invalid_json)).toEqual(false);
            });
        });
        Context("valid document", [] {
            const char* valid_json = "{ \"foo\": 0 }";
            It("returns true", [&] {
                json::JSON doc;
                Expect(doc.parse(valid_json)).toEqual(true);
            });
        });
    });
});
EndSpec();
