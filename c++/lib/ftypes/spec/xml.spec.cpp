#include <cspec.hpp>
#include "xml.hpp"

Eval(XML)
{
    Describe("class XML", [] {
        Context("parse()", [] {
            Context("invalid format", [] {
                const char* invalid_xml = R"(<?xml version="1.0" encoding="utf8"?>)"
                                          "<first>"
                                          "<second>"
                                          "</second>"
                                          "</first";  // unclosed tag
                It("returns false", [&] {
                    xml::XML doc;
                    Expect(doc.parse(invalid_xml)).toEqual(false);
                    Expect(doc.err()).toEqual("XML parse error: expected >");
                });
            });
            Context("valid format", [] {
                const char* valid_xml = R"(<?xml version="1.0" encoding="utf8"?>)"
                                        "<first>"
                                        "<second>"
                                        "</second>"
                                        "</first>";
                It("returns true", [&] {
                    xml::XML doc;
                    Expect(doc.parse(valid_xml)).toEqual(true);
                    Expect(doc.err()).toEqual("");
                });
            });
        });

        Context("forEach()", [] {
            It("loops over the tags in the right order", [] {
                const char* data = R"(<?xml version="1.0" encoding="utf8"?>)"
                                   "<first>"
                                   "<second>"
                                   "<third>"
                                   "</third>"
                                   "</second>"
                                   "<fourth>"
                                   "</fourth>"
                                   "</first>";
                xml::XML doc;
                doc.parse(data);

                unsigned int i = 0;
                std::vector<const char*> tags = { "first", "second", "third", "fourth" };
                doc.forEach([&](xml::Node node) -> void { Expect(node.Name).toEqual(tags[i++]); });
                Expect(i).toEqual(tags.size());
            });
        });

        const char* data = R"(<?xml version="1.0" encoding="utf8"?>)"
                           "<root>"
                           "<inner>"
                           R"(<this value="first">)"
                           "</this>"
                           R"(<this value="second">)"
                           "</this>"
                           "</inner>"
                           R"(<this value="third">)"
                           "</this>"
                           "</root>";

        Context("findFirstNode()", [&] {
            It("returns a node", [&] {
                xml::XML doc;
                Expect(doc.parse(data)).toEqual(true);

                auto root = doc.findFirstNode("root");
                Expect(root.Exists).toEqual(true);

                auto inner = root.findFirstNode("inner");
                Expect(inner.Exists).toEqual(true);

                auto found = inner.findFirstNode("this");
                Expect(found.Exists).toEqual(true, "could not find this");
                auto value = found.findFirstAttr("value");
                Expect(value.Value).toEqual("first");

                auto notfound = doc.findFirstNode("not-exist");
                Expect(notfound.Exists).toEqual(false, "found not-exists");
            });
        });

        Context("findLastNode()", [&] {
            It("returns a node", [&] {
                xml::XML doc;
                Expect(doc.parse(data)).toEqual(true);

                auto root = doc.findLastNode("root");
                Expect(root.Exists).toEqual(true);

                auto inner = root.findLastNode("inner");
                Expect(inner.Exists).toEqual(true);

                auto found = inner.findLastNode("this");
                Expect(found.Exists).toEqual(true, "could not find this");
                auto value = found.findFirstAttr("value");
                Expect(value.Value).toEqual("second");

                auto notfound = doc.findLastNode("not-exist");
                Expect(notfound.Exists).toEqual(false, "found not-exists");
            });
        });
    });
}

Eval(Node)
{
    Describe("class Node", [] {
        const char* data = R"(<?xml version="1.0" encoding="utf8"?>)"
                           "<found>"
                           "</found>";
        Context("Exists", [&] {
            Context("Does not exist", [&] {
                It("returns false", [&] {
                    xml::XML doc;
                    doc.parse(data);
                    auto node = doc.findFirstNode("notfound");
                    Expect(node.Exists).toEqual(false);
                });
            });

            Context("Does exist", [&] {
                It("returns true", [&] {
                    xml::XML doc;
                    doc.parse(data);
                    auto node = doc.findFirstNode("found");
                    Expect(node.Name).toEqual("found");
                    Expect(node.Exists).toEqual(true);
                });
            });
        });
    });
}

