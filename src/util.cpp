#include "util.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void save_as_png(const Bitmap& bitmap, const char* filename) {
  uint8_t* buf = new uint8_t[bitmap.cols * bitmap.rows * 3];

  const auto convert = [](float t) {
    if (t <= 0) return 0;
    if (t >= 1) return 255;
    return static_cast<int>(t * 255.99);
  };

  for (int row = 0; row < bitmap.rows; ++row) {
    for (int col = 0; col < bitmap.cols; ++col) {
      const auto& color = bitmap[row][col];

      const int offset = (row * bitmap.cols + col) * 3;
      uint8_t* ptr = buf + offset;

      ptr[0] = convert(color.x);
      ptr[1] = convert(color.y);
      ptr[2] = convert(color.z);
    }
  }

  stbi_write_png(filename, bitmap.cols, bitmap.rows, 3, buf, 0);

  delete[] buf;
}
