#ifndef CPU_FILTER_H
#define CPU_FILTER_H

#include <QImage>

struct CpuFilter final
{
	CpuFilter();
	~CpuFilter();

	QImage operator()(const QImage& img, std::size_t dim, std::vector<double>& vec);

private:



};

#endif // CPU_FILTER_H