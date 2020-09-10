#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <tuple>

#include <FreeImage.h>

#include "pixel.hpp"

#include <utility/error.hpp>

using utility::Error;

namespace art
{
  //const auto foo = 1;
  enum class ImageFormat
  {
    Unknown = FIF_UNKNOWN,
    PNG     = FIF_PNG,
    JPEG    = FIF_JPEG,
    GIF     = FIF_GIF,
    BMP     = FIF_BMP
  };

  class Image
  {
   public:
    Image() = default;

    /* Create a blank image with width and length in pixels */
    Image(Pixel* data, size_t width, size_t rows);

    /* Create a blank image with width and length in bytes */
    Image(const unsigned char* data, size_t width, size_t rows);

    /* Create blank image of specified width and height, in bytes */
    Image(size_t width, size_t rows);

    /* Copy the other image to this */
    void copy(const Image& other);

    /* Sets the filename to the supplied one and loads. Error will be a string */
    auto load(std::string filename) -> Error<std::string>;

    auto data() -> unsigned char*;

    /* Number of colums, in pixels */
    auto img_pixel_length() -> unsigned int;
    /* Number of columns, in bytes */
    auto img_byte_length() -> unsigned int;

    /* Length in pixels */
    auto data_pixel_length() -> unsigned long;
    /* Length in bytes */
    auto data_byte_length() -> unsigned long;

    /* Number of rows */
    auto rows() -> unsigned int;

    void fill(unsigned char color);
    void fill(unsigned char r, unsigned char g, unsigned char b);

    // Write to this image using data, at offset x & y in bytes
    void write_bytes(Image& data, size_t x, size_t y);

    // Write to this image using data, at offset x & y in pixels
    void write_pixels(Image& data, size_t x, size_t y);

    // Blend this image using data, at offset x & y in pixels
    void blend(Image& data, size_t x, size_t y);

    /* Saves using the format of the file name. Error will be a string */
    auto save(std::string filename) -> Error<std::string>;

    /* Saves using the format specified regardless of the file name. Error will be a string */
    auto save(std::string filename, ImageFormat format) -> Error<std::string>;

   private:
    ImageFormat          mFormat;
    std::vector<uint8_t> mData;
    unsigned int         mByteWidth;
    unsigned int         mRows;

    class ImageUnloader
    {
     public:
      ImageUnloader(FIBITMAP*& bitmap): mBitmap(bitmap){};
      ~ImageUnloader()
      {
        if (mBitmap != nullptr) {
          FreeImage_Unload(mBitmap);
          mBitmap = nullptr;
        }
      }

     private:
      FIBITMAP*& mBitmap;
    };
  };

  inline Image::Image(size_t width, size_t rows)
  {
    mRows      = rows;
    mByteWidth = width * 4;

    mData.resize(rows * width * 4);
  }

  inline Image::Image(Pixel* data, size_t width, size_t rows)
  {
    mRows      = rows * 4;
    mByteWidth = width * 4;

    mData.resize(mRows * mByteWidth);

    size_t offset = mRows * mByteWidth;
    mData.insert(mData.begin(), (unsigned char*)data, (unsigned char*)data + offset);
  }

  inline Image::Image(const unsigned char* data, size_t width, size_t rows)
  {
    mRows      = rows;
    mByteWidth = width;

    mData.resize(mRows * mByteWidth);

    size_t offset = mRows * mByteWidth;
    mData.insert(mData.begin(), data, data + offset);
  }

  inline void Image::copy(const Image& other)
  {
    mFormat = other.mFormat;
    mData.assign(other.mData.begin(), other.mData.end());
    mByteWidth = other.mByteWidth;
    mRows      = other.mRows;
    // don't copy the error msg if there is one
  }

  inline auto Image::load(std::string filename) -> Error<std::string>
  {
    mFormat = static_cast<ImageFormat>(FreeImage_GetFileType(filename.c_str()));

    if (mFormat == ImageFormat::Unknown) {
      return std::string("unknown image type");
    }

    FIBITMAP*     fImage = FreeImage_Load((FREE_IMAGE_FORMAT)mFormat, filename.c_str());
    ImageUnloader unloader(fImage);

    if (fImage == nullptr) {
      std::stringstream ss;
      ss << "failed to load image: " << filename;
      return ss.str();
    }

    if (FreeImage_GetBPP(fImage) != 32) {
      FIBITMAP*     temp = fImage;
      ImageUnloader temp_unloader(temp);

      fImage = FreeImage_ConvertTo32Bits(temp);

      if (!fImage) {
        return std::string("unable to convert to 32 bits per pixel");
      }
    }

    auto bits = FreeImage_GetBits(fImage);

    mByteWidth = FreeImage_GetWidth(fImage) * 4;
    mRows      = FreeImage_GetHeight(fImage);

    size_t offset = mRows * mByteWidth;

    mData.insert(mData.begin(), bits, bits + offset);

    return Error<std::string>::none();
  }

  inline auto Image::data() -> unsigned char*
  {
    return mData.data();
  }

  inline auto Image::data_byte_length() -> unsigned long
  {
    return mData.size();
  }

  inline auto Image::data_pixel_length() -> unsigned long
  {
    return mData.size() / 4;
  }

  inline auto Image::img_byte_length() -> unsigned int
  {
    return mByteWidth;
  }

  inline auto Image::img_pixel_length() -> unsigned int
  {
    return mByteWidth / 4;
  }

  inline auto Image::rows() -> unsigned int
  {
    return mRows;
  }

  inline auto Image::save(std::string filename) -> Error<std::string>
  {
    return save(filename, (ImageFormat)FreeImage_GetFileType(filename.c_str()));
  }

  // TODO does not work
  inline auto Image::save(std::string filename, ImageFormat format) -> Error<std::string>
  {
    if (format == ImageFormat::Unknown) {
      return std::string("unknown file type");
    }

    unsigned int  pixelWidth = img_pixel_length();
    FIBITMAP*     bitmap     = FreeImage_Allocate(pixelWidth, mRows, 32);
    ImageUnloader unloader(bitmap);

    if (bitmap == nullptr) {
      return std::string("unable to allocate temporary buffer to save");
    }

    const Pixel* pixels = reinterpret_cast<const Pixel*>(mData.data());
    for (unsigned long r = 0; r < mRows; r++) {
      for (unsigned long c = 0; c < pixelWidth; c++) {
        Pixel   pix  = pixels[r * pixelWidth + c];
        RGBQUAD quad = {pix.r, pix.g, pix.b, pix.a};
        if (!FreeImage_SetPixelColor(bitmap, c, r, &quad)) {
          std::stringstream ss;
          ss << "unable to set pixel color at coord at (" << c << ", " << r << ')';
          return ss.str();
        }
      }
    }

    auto res = FreeImage_Save((FREE_IMAGE_FORMAT)format, bitmap, filename.c_str());

    if (!res) {
      return std::string("unable to save image");
    }

    return Error<std::string>::none();
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

  inline void Image::write_bytes(Image& other, size_t x, size_t y)
  {
    auto thisWidth  = img_byte_length();
    auto thisRows   = rows();
    auto otherWidth = other.img_byte_length();
    auto otherRows  = other.rows();

    auto thisData  = mData.data();
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

  void Image::write_pixels(Image& other, size_t x, size_t y)
  {
    auto thisWidth  = img_pixel_length();
    auto thisRows   = rows();
    auto otherWidth = other.img_pixel_length();
    auto otherRows  = other.rows();

    auto thisData  = (Pixel*)mData.data();
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
    auto thisWidth  = img_pixel_length();
    auto thisRows   = rows();
    auto otherWidth = other.img_pixel_length();
    auto otherRows  = other.rows();

    auto thisData  = (Pixel*)mData.data();
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
}  // namespace art
