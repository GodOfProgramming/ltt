#pragma once
#include <algorithm>

namespace art {
  struct Pixel
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    void blend(Pixel& other);
  };

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
}

