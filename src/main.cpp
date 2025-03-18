#include "tgaimage.h"

#include "Line.h"
const int width = 100;
const int height = 100;

int main()
{
	TGAImage image(width, height, TGAImage::RGB);
	line(glm::vec2(width / 2, height / 2), glm::vec2(width, height / 2),    TGAColor(255, 0,    0,   255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(width, 0.75 * height), TGAColor(255, 128,  0,   255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(width, height), WHITE, image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0.75 * width, height), TGAColor(255, 255,  0,   255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(width / 2, height),    TGAColor(128, 255,  0,   255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0.25 * width, height), TGAColor(0,   255,  0,   255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0, height), WHITE, image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0, 0.75 * height),     TGAColor(0,   255,  128, 255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(width / 2, 0),         TGAColor(0,   255,  255, 255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0, 0.25 * height),     TGAColor(0,   128,  255, 255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0, 0), WHITE, image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0.25 * width, 0),      TGAColor(64,  64,   255, 255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0, height / 2),        TGAColor(128, 0,    255, 255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(0.75 * width, 0),      TGAColor(255, 0,    255, 255), image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(width, 0), WHITE, image);
	line(glm::vec2(width / 2, height / 2), glm::vec2(width, 0.25 * height), TGAColor(255, 0,    128, 255), image);

	//line(glm::vec2(1, 3), glm::vec2(1, 3), WHITE, image);

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}
