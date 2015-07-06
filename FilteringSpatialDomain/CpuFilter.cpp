#include "CpuFilter.h"

CpuFilter::CpuFilter() { ; }

CpuFilter::~CpuFilter() { ; }

QImage CpuFilter::operator()(const QImage& img, std::size_t dim, const std::vector<float>& filter)
{
	int width = img.width() - static_cast<int>(dim / 2);
	int height = img.height() - static_cast<int>(dim / 2);
	QImage outImg(width, height, QImage::Format_RGB32);

	float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f, sumA = 0.0f;

	for (int y = static_cast<int>(dim / 2); y < width; y++)
	{
		for (int x = static_cast<int>(dim / 2); x < height; x++, sumR = 0, sumG = 0, sumB = 0, sumA = 0)
		{
			for (int i = -static_cast<int>(dim / 2); i <= static_cast<int>(dim / 2); i++)
			{
				for (int j = -static_cast<int>(dim / 2); j <= static_cast<int>(dim / 2); j++)
				{
					sumR += filter[((j + static_cast<int>(dim / 2)) * dim) + (i + static_cast<int>(dim / 2))] * qRed(img.pixel(x - i, y - j));
					sumG += filter[((j + static_cast<int>(dim / 2)) * dim) + (i + static_cast<int>(dim / 2))] * qGreen(img.pixel(x - i, y - j));
					sumB += filter[((j + static_cast<int>(dim / 2)) * dim) + (i + static_cast<int>(dim / 2))] * qBlue(img.pixel(x - i, y - j));
					sumA += filter[((j + static_cast<int>(dim / 2)) * dim) + (i + static_cast<int>(dim / 2))] * qAlpha(img.pixel(x - i, y - j));
				}
			}
			outImg.setPixel(x, y, qRgba(sumR, sumG, sumB, sumA));
		}
	}
	return outImg;
}