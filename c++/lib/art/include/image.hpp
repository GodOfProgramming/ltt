#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <FreeImage.h>

namespace art
{
    enum class ImageFormat
    {
        PNG = FIF_PNG,
        JPEG = FIF_JPEG,
        GIF = FIF_GIF
    };

    enum class ImageType
    {
        Byte,
        Pixel
    };

    struct Pixel
    {
        unsigned char data[4];

        unsigned char& r = data[0];
        unsigned char& g = data[1];
        unsigned char& b = data[2];
        unsigned char& a = data[3];

        void blend(Pixel& other);
        Pixel& operator=(const Pixel& other);
    };

    class Image
    {
       public:
        // do not use
        Image() = default;

        /*FreeImage handles this*/
        Image(std::string filename);

        /*In terms of how many pixels high and wide*/
        Image(Pixel* data, size_t width, size_t rows);

        /*In terms of how many bytes high and wide*/
        Image(const unsigned char* data, size_t width, size_t rows);

        /*Create blank image of specified width and height, in bytes*/
        Image(size_t width, size_t rows);

        bool load();
        bool load(std::string filename);

        unsigned char* getData();

        /* number of colums, in pixels */
        unsigned int lengthInPixels();
        /* number of columns, in bytes */
        unsigned int lengthInBytes();
        /* number of rows */
        unsigned int rows();

        /* length in pixels */
        unsigned long lengthOfDataInPixels();
        /* length in bytes */
        unsigned long lengthOfDataInBytes();

        void fill(unsigned char color);
        void fill(unsigned char r, unsigned char g, unsigned char b);

        // other is written to this
        void writeBytes(Image& other, size_t x, size_t y);
        void writePixels(Image& other, size_t x, size_t y);
        void blend(Image& other, size_t x, size_t y);

        void save(ImageFormat format = ImageFormat::PNG);
        void saveAs(std::string filename, ImageFormat format = ImageFormat::PNG);

        std::string err();

       private:
        std::string mFilename;
        std::vector<unsigned char> mData;
        const Pixel* mPixels;
        unsigned int mByteWidth;
        unsigned int mRows;
        std::string mErr;

        bool loadFromFile();
    };

    inline Image::Image(std::string filename)
    {
        load(filename);
    }

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

    inline bool Image::load(std::string filename)
    {
        mFilename = filename;
        return load();
    }

    inline bool Image::load()
    {
        return loadFromFile();
    }

    inline bool Image::loadFromFile()
    {
        /* Raw Pointers
         * fImage
         * temp
         */

        FREE_IMAGE_FORMAT format;
        FIBITMAP* fImage;

        auto filename = mFilename.c_str();
        format = FreeImage_GetFileType(filename);
        fImage = FreeImage_Load(format, filename);

        if (!fImage) {
            std::stringstream ss;
            ss << "Failed to load image: " << filename;
            mErr = ss.str();
            return false;
        }

        if (FreeImage_GetBPP(fImage) != 32) {
            FIBITMAP* temp = fImage;
            fImage = FreeImage_ConvertTo32Bits(temp);
            FreeImage_Unload(temp);
        }

        auto bits = FreeImage_GetBits(fImage);

        mByteWidth = FreeImage_GetWidth(fImage) * 4;
        mRows = FreeImage_GetHeight(fImage);

        size_t offset = mRows * mByteWidth;

        mData.resize(offset);
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

    inline void Image::save(ImageFormat format)
    {
        saveAs(mFilename, format);
    }

    inline void Image::saveAs(std::string filename, ImageFormat format)  // does not work
    {
        unsigned int pixelWidth = lengthInPixels();
        FIBITMAP* bitmap = FreeImage_Allocate(pixelWidth, mRows, 32);
        mPixels = (const Pixel*)mData.data();

        for (unsigned long r = 0; r < mRows; r++) {
            for (unsigned long c = 0; c < pixelWidth; c++) {
                Pixel pix = mPixels[r * pixelWidth + c + 0];
                RGBQUAD quad = { pix.r, pix.g, pix.b, pix.a };
                FreeImage_SetPixelColor(bitmap, c, r, &quad);
            }
        }

        FreeImage_Save((FREE_IMAGE_FORMAT)format, bitmap, filename.c_str());
        FreeImage_Unload(bitmap);
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

        auto thisData =
            (unsigned char*)mData.data();  // no need for cast, in case I change it to a void* later for some unforseen reason
        auto otherData = (unsigned char*)other.mData.data();

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

    inline Pixel::operator=(const Pixel& other)
    {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        this->a = other.a;
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
