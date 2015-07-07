#include "CpuFilter.h"

CpuFilter::CpuFilter() { ; }

CpuFilter::~CpuFilter() { ; }

QImage CpuFilter::operator()(const QImage& img, std::size_t dim, const std::vector<float>& filter)
{
	int halfDim = static_cast<int>(dim / 2);
	int width = img.width() - halfDim;
	int height = img.height() - halfDim;
	QImage outImg(width, height, QImage::Format_RGB32);

	float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

	for (int y = halfDim; y < width; y++)
	{
		for (int x = halfDim; x < height; x++, sumR = 0, sumG = 0, sumB = 0)
		{
			for (int i = -halfDim; i <= halfDim; i++)
			{
				for (int j = -halfDim; j <= halfDim; j++)
				{
					sumR += filter[((j + halfDim) * dim) + (i + halfDim)] * qRed(img.pixel(x - i, y - j));
					sumG += filter[((j + halfDim) * dim) + (i + halfDim)] * qGreen(img.pixel(x - i, y - j));
					sumB += filter[((j + halfDim) * dim) + (i + halfDim)] * qBlue(img.pixel(x - i, y - j));
				}
			}
			outImg.setPixel(x, y, qRgb(sumR, sumG, sumB));
		}
	}

	return outImg;
}