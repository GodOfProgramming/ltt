#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <FreeImage.h>

namespace art
{
    enum class ImageFormat
    {
        Unknown = FIF_UNKNOWN,
        PNG = FIF_PNG,
        JPEG = FIF_JPEG,
        GIF = FIF_GIF,
        BMP = FIF_BMP
    };

    struct Pixel
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;

        void blend(Pixel& other);
        Pixel& operator=(const Pixel& other);
    };

    class Image
    {
       public:
        Image() = default;

        /* Load a image using filename */
        Image(std::string filename);

        /* Create a blank image with width and length in pixels */
        Image(Pixel* data, size_t width, size_t rows);

        /* Create a blank image with width and length in bytes */
        Image(const unsigned char* data, size_t width, size_t rows);

        /* Create blank image of specified width and height, in bytes */
        Image(size_t width, size_t rows);

        /* Copy the other image to this */
        void copy(const Image& other);

        /* Loads the file using the supplied file name from the constructor */
        bool load();

        /* Sets the filename to the supplied one and loads */
        bool load(std::string filename);

        unsigned char* getData();

        /* Number of colums, in pixels */
        unsigned int lengthInPixels();
        /* Number of columns, in bytes */
        unsigned int lengthInBytes();
        /* Number of rows */
        unsigned int rows();

        /* length in pixels */
        unsigned long lengthOfDataInPixels();
        /* length in bytes */
        unsigned long lengthOfDataInBytes();

        void fill(unsigned char color);
        void fill(unsigned char r, unsigned char g, unsigned char b);

        // Write to this image using data, at offset x & y in bytes
        void writeBytes(Image& data, size_t x, size_t y);

        // Write to this image using data, at offset x & y in pixels
        void writePixels(Image& data, size_t x, size_t y);

        // Blend this image using data, at offset x & y in pixels
        void blend(Image& data, size_t x, size_t y);

        bool save();
        bool save(std::string filename);
        bool save(ImageFormat format);
        bool save(std::string filename, ImageFormat format);

        std::string err();

       private:
        std::string mFilename;
        ImageFormat mFormat;
        std::vector<uint8_t> mData;
        unsigned int mByteWidth;
        unsigned int mRows;
        std::string mErr;

        bool loadFromFile();
    };

    inline Image::Image(std::string filename) : mFilename(filename)
    {}

    inline Image::Image(size_t width, size_t rows)
    {
        this->mRows = rows;
        this->mByteWidth = width * 4;

        mData.resize(rows * width * 4);
    }

    inline Image::Image(Pixel* data, size_t width, size_t rows)
    {
        this->mRows = rows * 4;
        this->mByteWidth = width * 4;

        this->mData.resize(this->mRows * this->mByteWidth);

        size_t offset = this->mRows * this->mByteWidth;
        this->mData.insert(this->mData.begin(), (unsigned char*)data, (unsigned char*)data + offset);
    }

    inline Image::Image(const unsigned char* data, size_t width, size_t rows)
    {
        this->mRows = rows;
        this->mByteWidth = width;

        this->mData.resize(this->mRows * this->mByteWidth);

        size_t offset = this->mRows * this->mByteWidth;
        this->mData.insert(this->mData.begin(), data, data + offset);
    }

    inline void Image::copy(const Image& other)
    {
        this->mFilename = other.mFilename;
        this->mFormat = other.mFormat;
        this->mData.assign(other.mData.begin(), other.mData.end());
        this->mByteWidth = other.mByteWidth;
        this->mRows = other.mRows;
        // don't copy the error msg if there is one
    }

    inline bool Image::load()
    {
        return loadFromFile();
    }

    inline bool Image::load(std::string filename)
    {
        mFilename = filename;
        return load();
    }

    inline bool Image::loadFromFile()
    {
        /* Raw Pointers
         * fImage
         * temp
         */

        FIBITMAP* fImage;

        auto filename = mFilename.c_str();
        mFormat = (ImageFormat)FreeImage_GetFileType(filename);

        if (mFormat == ImageFormat::Unknown) {
            mErr = "unknown image type";
            return false;
        }

        fImage = FreeImage_Load((FREE_IMAGE_FORMAT)mFormat, filename);

        if (!fImage) {
            std::stringstream ss;
            ss << "Failed to load image: " << filename;
            mErr = ss.str();
            return false;
        }

        if (FreeImage_GetBPP(fImage) != 32) {
            FIBITMAP* temp = fImage;
            fImage = FreeImage_ConvertTo32Bits(temp);

            if (!fImage) {
                mErr = "unable to convert to 32 bits per pixel";
                FreeImage_Unload(temp);
                return false;
            }

            FreeImage_Unload(temp);
        }

        auto bits = FreeImage_GetBits(fImage);

        mByteWidth = FreeImage_GetWidth(fImage) * 4;
        mRows = FreeImage_GetHeight(fImage);

        size_t offset = mRows * mByteWidth;

        mData.insert(mData.begin(), bits, bits + offset);

        FreeImage_Unload(fImage);

        return true;
    }

    inline unsigned char* Image::getData()
    {
        return mData.data();
    }

    inline unsigned long Image::lengthOfDataInBytes()
    {
        return mData.size();
    }

    inline unsigned long Image::lengthOfDataInPixels()
    {
        return mData.size() / 4;
    }

    inline unsigned int Image::lengthInBytes()
    {
        return mByteWidth;
    }

    inline unsigned int Image::lengthInPixels()
    {
        return mByteWidth / 4;
    }

    inline unsigned int Image::rows()
    {
        return mRows;
    }

    inline bool Image::save()
    {
        return save(mFilename, mFormat);
    }

    inline bool Image::save(std::string filename)
    {
        auto fname = filename.c_str();
        return save(filename, (ImageFormat)FreeImage_GetFileType(fname));
    }

    inline bool Image::save(ImageFormat format)
    {
        return save(mFilename, format);
    }

    inline bool Image::save(std::string filename, ImageFormat format)  // does not work
    {
        if (format == ImageFormat::Unknown) {
            mErr = "unknown file type";
            return false;
        }

        unsigned int pixelWidth = lengthInPixels();
        FIBITMAP* bitmap = FreeImage_Allocate(pixelWidth, mRows, 32);

        if (!bitmap) {
            mErr = "unable to allocate temporary buffer to save";
            return false;
        }

        const Pixel* pixels = (const Pixel*)mData.data();
        for (unsigned long r = 0; r < mRows; r++) {
            for (unsigned long c = 0; c < pixelWidth; c++) {
                Pixel pix = pixels[r * pixelWidth + c];
                RGBQUAD quad = { pix.r, pix.g, pix.b, pix.a };
                if (!FreeImage_SetPixelColor(bitmap, c, r, &quad)) {
                    std::stringstream ss;
                    ss << "unable to set pixel color at coord at (" << c << ", " << r << ')';
                    mErr = ss.str();
                    FreeImage_Unload(bitmap);
                    return false;
                }
            }
        }

        auto res = FreeImage_Save((FREE_IMAGE_FORMAT)format, bitmap, filename.c_str());

        if (!res) {
            mErr = "unable to save image";
        }

        FreeImage_Unload(bitmap);

        return res;
    }

    inline void Image::fill(unsigned char color)
    {
        for (unsigned long i = 0; i < mData.size(); i += 4) {
            mData[i + 0] = color;
            mData[i + 1] = color;
            mData[i + 2] = color;
        }
    }

    inline void Image::fill(unsigned char r, unsigned char g, unsigned char b)
    {
        for (unsigned long i = 0; i < mData.size(); i += 4) {
            mData[i + 0] = r;
            mData[i + 1] = g;
            mData[i + 2] = b;
        }
    }

    inline void Image::writeBytes(Image& other, size_t x, size_t y)
    {
        auto thisWidth = this->lengthInBytes();
        auto thisRows = this->rows();
        auto otherWidth = other.lengthInBytes();
        auto otherRows = other.rows();

        auto thisData = mData.data();
        auto otherData = other.mData.data();

        for (size_t h = 0; h < otherRows; h++) {
            auto hLoc = h + y;

            if (hLoc >= thisRows) {
                break;  // can't write to what doesn't exist
            }

            for (size_t w = 0; w < otherWidth; w++) {
                auto wLoc = w + x;

                if (wLoc >= thisWidth) {
                    break;
                }

                thisData[thisWidth * hLoc + wLoc] = otherData[otherWidth * h + w];
            }
        }
    }

    void Image::writePixels(Image& other, size_t x, size_t y)
    {
        auto thisWidth = this->lengthInPixels();
        auto thisRows = this->rows();
        auto otherWidth = other.lengthInPixels();
        auto otherRows = other.rows();

        auto thisData = (Pixel*)mData.data();
        auto otherData = (Pixel*)other.mData.data();

        for (size_t h = 0; h < otherRows; h++) {
            auto hLoc = h + y;

            if (hLoc >= thisRows) {
                break;  // can't write to what doesn't exist
            }

            for (size_t w = 0; w < otherWidth; w++) {
                auto wLoc = w + x;

                if (wLoc >= thisWidth) {
                    break;
                }

                thisData[thisWidth * hLoc + wLoc] = otherData[otherWidth * h + w];
            }
        }
    }

    inline void Image::blend(Image& other, size_t x, size_t y)
    {
        auto thisWidth = this->lengthInPixels();
        auto thisRows = this->rows();
        auto otherWidth = other.lengthInPixels();
        auto otherRows = other.rows();

        auto thisData = (Pixel*)mData.data();
        auto otherData = (Pixel*)other.mData.data();

        for (size_t h = 0; h < otherRows; h++) {
            auto hLoc = h + y;

            if (hLoc >= thisRows) {
                break;  // can't write to what doesn't exist
            }

            for (size_t w = 0; w < otherWidth; w++) {
                auto wLoc = w + x;

                if (wLoc >= thisWidth) {
                    break;
                }

                Pixel& dp = thisData[thisWidth * hLoc + wLoc];
                Pixel& sp = otherData[otherWidth * h + w];

                dp.blend(sp);
            }
        }
    }

    inline std::string Image::err()
    {
        return mErr;
    }

    inline Pixel& Pixel::operator=(const Pixel& other)
    {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        this->a = other.a;

        return *this;
    }

    inline void Pixel::blend(Pixel& other)
    {
        Pixel& dp = *this;
        Pixel& sp = other;
        float sr, sg, sb, sa, dr, dg, db;

        sr = sp.r / 255.0;
        sg = sp.g / 255.0;
        sb = sp.b / 255.0;
        sa = sp.a / 255.0;

        dr = dp.r / 255.0;
        dg = dp.g / 255.0;
        db = dp.b / 255.0;

        float rmod, gmod, bmod;

        rmod = sr * sa + dr * (1 - sa);
        gmod = sg * sa + dg * (1 - sa);
        bmod = sb * sa + db * (1 - sa);

        rmod = std::min(rmod, 1.0f);
        gmod = std::min(gmod, 1.0f);
        bmod = std::min(bmod, 1.0f);

        dp.r = 255 * rmod;
        dp.g = 255 * gmod;
        dp.b = 255 * bmod;
    }
}  // namespace art
