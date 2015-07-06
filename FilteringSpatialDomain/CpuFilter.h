#ifndef CPU_FILTER_H
#define CPU_FILTER_H

#include <QImage>
#include <cstddef>
#include <vector>

struct CpuFilter final
{
	CpuFilter();
	CpuFilter(const CpuFilter&) = delete;
	CpuFilter(CpuFilter&&) = delete;
	~CpuFilter();
	CpuFilter& operator=(const CpuFilter&) = delete;
	CpuFilter& operator=(CpuFilter&&) = delete;

	QImage operator()(const QImage& img, std::size_t dim, const std::vector<float>& filter);

};

#endif // CPU_FILTER_H