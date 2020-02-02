#pragma once
#include <string>
#include <sstream>
#include <functional>
#include <experimental/filesystem>
#include <fstream>

namespace io
{
    class File
    {
       public:
        File() = default;
        File(std::string filename);
        ~File() = default;

        /* Static method to read the file and return the data, success is set to the result of the operation */
        // No error message reported
        static std::string Read(std::string filename, bool* success = nullptr);

        /* Static method to write to file and return success */
        // No error message reported
        static bool Write(const std::string filename, const std::string& data);

        /* Read the file from the supplied filename */
        bool read(std::string filename);

        /* Read the file from the supplied filename from the constructor */
        inline bool read();

        /* Writes to the supplied filename */
        bool write(std::string filename);

        /* Writes to the file given from the constructor */
        bool write();

        /* True if the file exists and is a regular file */
        bool exists();

        /* Clears the file */
        void clear();

        /* Assigns the data to the files internal buffer, overwriting what was there */
        template <typename... Args>
        void assign(Args&&... args)
        {
            this->clear();
            this->append(args...);
        }

        /* Appends data to the file */
        template <typename... Args>
        void append(Args&&... args)
        {
            static_assert(sizeof...(args) > 0);
            ((mData << std::forward<Args>(args)), ...);
        }

        /* Removes the file, true if success */
        bool remove();

        std::string data();

        std::string filename();

       private:
        std::string mFilename;
        std::stringstream mData;

        std::string mErr;
    };

    inline std::string File::Read(std::string filename, bool* success)
    {
        File file(filename);
        if (file.exists() && file.read()) {
            if (success != nullptr) {
                *success = true;
            }
            return file.mData.str();
        }

        if (success != nullptr) {
            *success = false;
        }
        return std::string();
    }

    inline bool File::Write(const std::string filename, const std::string& data)
    {
        File file(filename);
        return file.write(data);
    }

    inline File::File(std::string filename) : mFilename(filename)
    {}

    inline bool File::read(std::string filename)
    {
        if (filename.empty()) {
            mErr = "no filename given";
            return false;
        }

        std::ifstream stream;

        stream.open(filename, std::ios::binary);

        if (!stream) {
            mErr = "could not open file for reading";
            return false;
        }

        this->clear();
        mData << stream.rdbuf();
        stream.close();

        return true;
    }

    inline bool File::read()
    {
        return read(mFilename);
    }

    inline bool File::write(std::string filename)
    {
        if (filename.empty()) {
            mErr = "no filename given";
            return false;
        }

        std::ofstream stream;

        stream.open(filename, std::ios::binary);

        if (!stream) {
            mErr = "could not open file for writing";
            return false;
        }

        auto output = mData.str();
        stream.write(output.c_str(), output.length());
        stream.close();

        return true;
    }

    inline bool File::write()
    {
        return write(mFilename);
    }

    inline bool File::exists()
    {
        return std::experimental::filesystem::is_regular_file(mFilename);
    }

    inline void File::clear()
    {
        mData.str(std::string());
    }

    inline bool File::remove()
    {
        return std::experimental::filesystem::remove(mFilename);
    }

    inline std::string File::data()
    {
        return mData.str();
    }

    inline std::string File::filename()
    {
        return mFilename;
    }
}  // namespace io
