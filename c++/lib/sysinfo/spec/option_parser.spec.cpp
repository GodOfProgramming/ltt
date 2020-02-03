#include <cspec.hpp>
#include "option_parser.hpp"

Eval(OptionParser)
{
    Describe("parse()", [] {
        Context("there are no extra arguments", [] {
            It("returns an empty vector", [] {
                bool a_value = false;
                int argc = 2;
                const char* const argv[] = { "prgm name", "-a" };
                info::OptionParser parser(argc, argv, [&](info::Options& opts) {
                    opts.on("-a", "--arg", "test arg", [&](bool v) {
                        if (v) {
                            a_value = true;
                        } else {
                            a_value = false;
                        }
                    });

                    auto extra_args = parser.parse();

                    Expect(a_value).toEqual(true);
                    Expect(extra_args.size()).toEqual((unsigned int)0);
                });
            });
        });

        Context("there are extra arguments", [] {
            It("returns a vector containing the extra arguments", [] {
                bool a_value = false;
                int argc = 4;
                const char* const argv[] = { "prgm name", "extra-arg-2", "-a", "extra-arg-1" };
                info::OptionParser parser(argc, argv, [&](info::Options& opts) {
                    opts.on("-a", "--arg", "test arg", [&](bool v) {
                        if (v) {
                            a_value = true;
                        } else {
                            a_value = false;
                        }
                    });

                    auto extra_args = parser.parse();

                    Expect(a_value).toEqual(true);
                    Expect(extra_args.size()).toEqual((unsigned int)2);
                    Expect(extra_args[0]).toEqual("extra-arg-2");
                    Expect(extra_args[1]).toEqual("extra-arg-1");
                });
            });
        });
    });

    Describe("getArgValue()", [] {
        const int argc = 15;
        const char* const argv[] = {
            "prgm name",
            "--next-is-invalid-short",
            "-a",
            "--next-is-invalid-long-type-1",
            "--type1",
            "--next-is-invalid-long-type-2",
            "--type2=somevalue",
            "-e=invalid",
            "--equal-attachment=equal-value",
            "regular-argument",
            "-w",
            "whitespace-value-1",
            "--whitespace",
            "whitespace-value-2",
	    "valid value",
            "--last",
        };
        info::OptionParser parser(argc, argv, [](info::Options& opts) { (void)opts; });

        Context("next argument is not a value", [&] {
            Context("invalid value is a short arg", [&] {
                It("returns an empty string", [&] {
                    int i = 1;
                    Expect(parser.getArgValue(i)).toEqual(std::string());
                });
            });

            Context("invalid value is a type1 long arg", [&] {
                It("returns an empty string", [&] {
                    int i = 3;
                    Expect(parser.getArgValue(i)).toEqual(std::string());
                });
            });

            Context("invalid value is a type2 long arg", [&] {
                It("returns an empty string", [&] {
                    int i = 5;
                    Expect(parser.getArgValue(i)).toEqual(std::string());
                });
            });
        });

        Context("next argument is attached via '=' but there is also a argument afterwards", [&] {
            Context("short arg", [&] {
                It("returns an empty string", [&] {
                    int i = 7;
                    Expect(parser.getArgValue(i)).toEqual(std::string());
                });
            });
            Context("long arg", [&] {
                It("returns the value after '='", [&] {
                    int i = 8;
                    Expect(parser.getArgValue(i)).toEqual("equal-value");
                });
            });
        });

        Context("next argument is a valid value and separated by whitespace", [&] {
            Context("short arg", [&] {
                It("returns the value after the whitespace", [&] {
                    int i = 10;
                    Expect(parser.getArgValue(i)).toEqual("whitespace-value-1");
                });
            });
            Context("long arg", [&] {
                It("returns the value after the whitespace", [&] {
                    int i = 12;
                    Expect(parser.getArgValue(i)).toEqual("whitespace-value-2");
                });
            });
        });

        Context("at end of argument list", [&] {
            It("returns an empty string", [&] {
                int i = argc - 1;
                Expect(parser.getArgValue(i)).toEqual(std::string());
            });
        });
    });
}

Eval(Options)
{
    Describe("on()", [] {
        Context("adding a basic argument", [] {
            It("can find the short and long versions", [] {
                info::Options opts;
                bool arg_val = false;

                opts.on("-a", "--a", "test arg", [&](bool v) { arg_val = v; });

                Expect(arg_val).toEqual(false);

                auto short_opt = opts.find("-a");

                Expect(short_opt != nullptr).toEqual(true);
                Expect(short_opt->Desc).toEqual("test arg");
            });
        });
    });

    Describe("isAdvancedOpt()", [] {
        Context("-i, --invalid", [] {
            It("returns false with no new values", [] {
                std::string s_name = "-i", l_name = "--invalid", new_s_name, new_l_name, arg;
                info::Options opts;

                Expect(opts.isAdvancedOpt(s_name, l_name, new_s_name, new_l_name, arg)).toEqual(false);
                Expect(new_s_name).toEqual(std::string());
                Expect(new_l_name).toEqual(std::string());
                Expect(arg).toEqual(std::string());
            });
        });
        Context("-s [NUMBER], --short", [] {
            It("returns true with the correct new args", [] {
                std::string s_name = "-s [NUMBER]", l_name = "--short", new_s_name, new_l_name, arg;
                info::Options opts;

                Expect(opts.isAdvancedOpt(s_name, l_name, new_s_name, new_l_name, arg)).toEqual(true);
                Expect(new_s_name).toEqual("-s");
                Expect(new_l_name).toEqual(l_name);
                Expect(arg).toEqual("[NUMBER]");
            });
        });
        Context("-l , --long [NUMBER]", [] {
            It("returns true with the correct new args", [] {
                std::string s_name = "-l ", l_name = "--long [NUMBER]", new_s_name, new_l_name, arg;
                info::Options opts;

                Expect(opts.isAdvancedOpt(s_name, l_name, new_s_name, new_l_name, arg)).toEqual(true);
                Expect(new_s_name).toEqual(s_name);
                Expect(new_l_name).toEqual("--long");
                Expect(arg).toEqual("[NUMBER]");
            });
        });
    });

    Describe("shouldShowHelp()", [] {
        Context("help opts are not in argv", [] {
            It("returns false", [] {
                int argc = 3;
                const char* const argv[] = {
                    "program",
                    "--some-option",
                    "-a",
                };

                info::Options opts;

                Expect(opts.shouldShowHelp(argc, argv)).toEqual(false);
            });
        });

        Context("-h is present but overriden", [] {
            It("returns false", [] {
                int argc = 2;
                const char* const argv[] = {
                    "program",
                    "-h",
                };

                info::Options opts;

                opts.on("-h", "--hello", "stub -h", [](bool v) { (void)v; });

                Expect(opts.shouldShowHelp(argc, argv)).toEqual(false);
            });
        });

        Context("--help is present but overriden", [] {
            It("returns false", [] {
                int argc = 2;
                const char* const argv[] = {
                    "program",
                    "--help",
                };

                info::Options opts;

                opts.on("-x", "--help", "stub --help", [](bool v) { (void)v; });

                Expect(opts.shouldShowHelp(argc, argv)).toEqual(false);
            });
        });

        Context("-h is present and not overriden", [] {
            It("returns true", [] {
                int argc = 2;
                const char* const argv[] = {
                    "program",
                    "-h",
                };

                info::Options opts;

                Expect(opts.shouldShowHelp(argc, argv)).toEqual(true);
            });
        });

        Context("--help is present and not overriden", [] {
            It("returns true", [] {
                int argc = 2;
                const char* const argv[] = {
                    "program",
                    "--help",
                };

                info::Options opts;

                Expect(opts.shouldShowHelp(argc, argv)).toEqual(true);
            });
        });
    });
}

