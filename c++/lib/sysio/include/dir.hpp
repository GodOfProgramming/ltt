#pragma once
#include "file.hpp"

namespace io
{
    class Dir
    {
       public:
        Dir(std::string dirname);

        /* Creates the directory if it does not already exist */
        static Dir Create(std::string path, bool* success);

	/* Returns the working directory */
	static std::string CurrentDir();

	/* Returns true if the entry is a directory */
        bool exists();

        /* Iterate over each file in the directory using the file name */
        void eachFile(std::function<void(std::string&)> eachfile);

        /* Iterate over each file in the directory after the files are encapsulated */
        void eachFile(std::function<void(File&)> eachfile);

       private:
        std::string mDirname;

        void fileLoop(std::function<void(std::string)> callback);
    };

    inline Dir::Dir(std::string dirname) : mDirname(dirname)
    {}

    inline Dir Dir::Create(std::string path, bool* success)
    {
        bool result = std::experimental::filesystem::create_directory(path);
        if (success) {
            *success = result;
        }
        return Dir(path);
    }

    inline std::string Dir::CurrentDir() {
	return std::experimental::filesystem::current_path();
    }

    inline bool Dir::exists()
    {
        return std::experimental::filesystem::is_directory(mDirname);
    }

    inline void Dir::eachFile(std::function<void(std::string&)> eachfile)
    {
        fileLoop([eachfile](std::string filename) -> void { eachfile(filename); });
    }

    inline void Dir::eachFile(std::function<void(File&)> eachfile)
    {
        fileLoop([eachfile](std::string filename) -> void {
            io::File file(filename);
            if (file.exists()) {
                eachfile(file);
            }
        });
    }

    inline void Dir::fileLoop(std::function<void(std::string)> callback)
    {
        if (exists()) {
            for (const auto& entry : std::experimental::filesystem::directory_iterator(mDirname)) {
                callback(entry.path());
            }
        }
    }
}  // namespace io
