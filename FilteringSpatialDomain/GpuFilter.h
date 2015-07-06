#ifndef GPU_FILTER_H
#define GPU_FILTER_H

#include <QImage>

struct GpuFilter final
{
	GpuFilter();
	~GpuFilter();

	QImage operator()(const QImage& img, std::size_t dim, std::vector<double>& vec);

private:



};

#endif // GPU_FILTER_H