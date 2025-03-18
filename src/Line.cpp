#include "Line.h"

#include <algorithm>

#define L_MIDPOINT

void lineEquation(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image) 
{
	if (p1.x > p2.x) 
	{
		std::swap(p1, p2);
	}
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	if (std::abs(dy) > std::abs(dx)) // 判断主步进方向，同时避免分母为0
	{
		if (p1.y > p2.y)
		{
			std::swap(p1, p2);
		}
		// 斜率绝对值大于 1，以 y 为步长
		if (p1.y > p2.y) {
			std::swap(p1, p2);
		}
		float k = dx / dy;
		float b = p1.x - k * p1.y;
		for (int y = p1.y; y < p2.y; ++y) 
		{
			int x = static_cast<int>(k * y + b);
			if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height()) //防止超出画布范围
			{
				image.set(x, y, color);
			}
		}
	}
	else if (std::abs(dy) < std::abs(dx)) 
	{
		if (p1.x > p2.x)
		{
			std::swap(p1, p2);
		}
		// 斜率绝对值小于等于 1，以 x 为步长
		float k = dy / dx;
		float b = p1.y - k * p1.x;
		for (int x = p1.x; x < p2.x; ++x)
		{
			int y = static_cast<int>(k * x + b);
			if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height()) 
			{
				image.set(x, y, color);
			}
		}
	}
	else
	{
		if (p1.x >= 0 && p1.x < image.get_width() && p1.y >= 0 && p1.y < image.get_height())
		{
			image.set(p1.x, p1.y, color);
		}
	}
}

void lineDDA(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	if (dx == 0)
	{
		if (dy == 0)
		{
			image.set((int)p1.x, (int)p1.y, color);
		}
		float x = p1.x;
		if (p1.y > p2.y)
		{
			std::swap(p1, p2);
		}
		for (int y = p1.y; y < p2.y; ++y)
		{
			if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
			{
				image.set((int)x, y, color);
			}
		}
		return;
	}

	float k = dy / dx;
	
	if (std::abs(dx) > std::abs(dy))
	{
		if (p1.x > p2.x)
		{
			std::swap(p1, p2);
		}
		float y = p1.y;
		for (int x = p1.x; x < p2.x; ++x)
		{
			if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
			{
				image.set(x, (int)y, color);
			}
			y += k;
		}
	}
	else
	{
		if (p1.y > p2.y)
		{
			std::swap(p1, p2);
		}
		float x = p1.x;
		for (int y = p1.y; y < p2.y; ++y)
		{
			if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
			{
				image.set((int)x, y, color);
			}
			x += 1.0f / k;
		}
	}
}

void lineMidpoint(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image) 
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	
	if (dx == 0)
	{
		if (dy == 0)
		{
			image.set((int)p1.x, (int)p1.y, color);
			return;
		}
		float x = p1.x;
		if (p1.y > p2.y)
		{
			std::swap(p1, p2);
		}
		for (int y = p1.y; y < p2.y; ++y)
		{
			if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
			{
				image.set((int)x, y, color);
			}
		}
		return;
	}
	
	if (std::abs(dx) >= std::abs(dy))
	{
		if (p1.x > p2.x)
		{
			std::swap(p1, p2);
		}
		float k = dy / dx;
		k = abs(k);
		int x = (int)p1.x;
		int y = (int)p1.y;

		float d = 0.5f - k;

		for (; x <= p2.x; ++x)
		{
			if (d <= 0)
			{
				if (p1.y < p2.y)
					++y;
				else
					--y;
				d = d + 1.0f - k;
			}
			else
			{
				d = d - k;
			}
			image.set(x, y, color);
		}
	}
	else
	{
		if (p1.y > p2.y)
		{
			std::swap(p1, p2);
		}
		float m = dx / dy;
		m = abs(m);
		int x = (int)p1.x;
		int y = (int)p1.y;

		float d = m - 0.5f;
		for (; y <= p2.y; ++y)
		{
			if (d <= 0)
			{
				if (p1.x < p2.x)
					++x;
				else
					--x;
				d = d + m;
			}
			else
			{
				d = d - 1.0f + m;
			}
			image.set(x, y, color);
		}
	}
}

void lineMidpointA(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image)
{
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	if (dx == 0)
	{
		if (dy == 0)
		{
			image.set((int)p1.x, (int)p1.y, color);
			return;
		}
		float x = p1.x;
		if (p1.y > p2.y)
		{
			std::swap(p1, p2);
		}
		for (int y = p1.y; y < p2.y; ++y)
		{
			if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
			{
				image.set((int)x, y, color);
			}
		}
		return;
	}

	bool steep = std::abs(dx) < std::abs(dy);
	if (steep)
	{
		std::swap(p1.x, p2.x);
		std::swap(p1.y, p2.y);
		std::swap(dx, dy);
	}
	if (p1.x > p2.x)
	{
		std::swap(p1, p2);
	}
	int x = p1.x;
	int y = p1.y;

	int e = dx - 2 * dy;

	int d = 2 * dy - dx;
	for (; x <= p2.x; ++x)
	{
		int drawX = steep ? y : x;
		int drawY = steep ? x : y;

		image.set(drawX, drawY, color);

		if (d < 0)
		{
			y += (p1.y < p2.y) ? 1 : -1;
			d += 2 * (dx - dy);
		}
		else
		{
			d -= 2 * dy;
		}
	}

}

void lineBresenham(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image)
{
	
}

void lineBresenhamA(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image)
{
	
}

void drawLine(glm::vec2 p1, glm::vec2 p2, TGAColor color, LineAlgorithm a, TGAImage& image)
{
	switch (a)
	{
	case Equation:lineEquation(p1, p2, color, image);
		break;
	case DDA:lineDDA(p1, p2, color, image);
		break;
	case Midpoint:lineMidpoint(p1, p2, color, image);
		break;
	case MidpointA:lineMidpointA(p1, p2, color, image);
		break;
	case Bresenham:lineBresenham(p1, p2, color, image);
		break;
	case BresenhamA:lineBresenhamA(p1, p2, color, image);
		break;
	default:
		break;
	}
}


void line(glm::vec2 p1, glm::vec2 p2, TGAImage& image)
{
#ifdef L_EQUATION
	drawLine(p1, p2, WHITE, LineAlgorithm::Equation, image);
#endif 
#ifdef L_DDA
	drawLine(p1, p2, WHITE, LineAlgorithm::DDA, image);
#endif
#ifdef L_MIDPOINT
	drawLine(p1, p2, WHITE, LineAlgorithm::Midpoint, image);
#endif
#ifdef L_MIDPOINTA
	drawLine(p1, p2, WHITE, LineAlgorithm::MidpointA, image);
#endif
#ifdef L_BRESENHAM
	drawLine(p1, p2, WHITE, LineAlgorithm::Bresenham, image);
#endif
#ifdef L_BRESENHAMA
	drawLine(p1, p2, WHITE, LineAlgorithm::BresenhamA, image);
#endif
}

void line(glm::vec2 p1, glm::vec2 p2, TGAColor color,TGAImage& image)
{
#ifdef L_EQUATION
	drawLine(p1, p2, color, LineAlgorithm::Equation, image);
#endif 
#ifdef L_DDA
	drawLine(p1, p2, color, LineAlgorithm::DDA, image);
#endif
#ifdef L_MIDPOINT
	drawLine(p1, p2, color, LineAlgorithm::Midpoint, image);
#endif
#ifdef L_MIDPOINTA
	drawLine(p1, p2, color, LineAlgorithm::MidpointA, image);
#endif
#ifdef L_BRESENHAM
	drawLine(p1, p2, color, LineAlgorithm::Bresenham, image);
#endif
#ifdef L_BRESENHAMA
	drawLine(p1, p2, color, LineAlgorithm::BresenhamA, image);
#endif
}