#include <cspec.hpp>
#include "file.hpp"

Eval(File)
{
    Describe("class File", [] {
        Context("constructor", [] {
            It("sets the filename as the param", [] {
                io::File f("somefile.txt");
                Expect(f.filename()).toEqual("somefile.txt");
            });
        });

        Context("exists()", [] {
            Context("file does not exist", [] {
                It("returns false", [] {
                    io::File f("somefile.txt");
                    Expect(f.exists()).toEqual(false);
                });
            });

            Context("file is a directory", [] {
                It("returns false", [] {
                    io::File f("/");
                    Expect(f.exists()).toEqual(false);
                });
            });

            Context("file does exist", [] {
                It("returns true", [] {
                    io::File f(ARGV[0]);  // name of running program
                    Expect(f.exists()).toEqual(true);
                });
            });
        });

        Context("read()", [] {
            It("creates a file, reads it, and deletes it", [] {
                io::File f(ENV["MEMTMP"] + "/file.spec.txt");
                f.assign("test data");
                f.write();

                io::File actual(ENV["MEMTMP"] + "/file.spec.txt");

                Expect(actual.exists()).toEqual(true);
                Expect(actual.read()).toEqual(true);
                Expect(actual.data()).toEqual("test data");
                Expect(actual.remove()).toEqual(true);
                Expect(actual.exists()).toEqual(false);
            });
        });
    });
}

