#include "Line.h"
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor white = TGAColor(255,255,255,255);

int main()
{
    TGAImage image(512, 512, TGAImage::RGB);
    line1(0, 0, 100, 100, image, red);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}