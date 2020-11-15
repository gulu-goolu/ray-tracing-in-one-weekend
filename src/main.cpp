#include "util.h"

int main(int argc, char* argv[]) {
  Bitmap bitmap(1024, 1024);
  for (int i = 0; i < bitmap.rows; ++i) {
    for (int j = 0; j < bitmap.cols; ++j) {
      int x_grid = i / 32, y_grid = j / 32;
      if ((x_grid + y_grid) % 2) {
        bitmap[i][j] = Vec3(1);
      } else {
        bitmap[i][j] = Vec3(0);
      }
    }
  }
  save_as_png(bitmap, "board.png");
}