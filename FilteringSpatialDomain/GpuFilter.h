#ifndef GPU_FILTER_H
#define GPU_FILTER_H

#include <QImage>
#include <cstddef>
#include <vector>
#include <string>
#include <iostream>

#define __CL_ENABLE_EXCEPTIONS
#include <CL\cl.hpp>

struct GpuFilter final
{
	GpuFilter();
	GpuFilter(const GpuFilter&) = delete;
	GpuFilter(GpuFilter&&) = delete;
	~GpuFilter();
	GpuFilter& operator=(const GpuFilter&) = delete;
	GpuFilter& operator=(GpuFilter&&) = delete;

	QImage operator()(const QImage& img, std::size_t dim, const std::vector<float>& filter);

private:

	std::string kernel;
	std::vector<cl_float4> inImg;
	std::vector<cl_float4> outImg;
	std::vector<cl_float> filter;

	cl::Context context;
	cl::CommandQueue queue;
	cl::Kernel kernel_setFilter;

	void prepareImg(const QImage& img);
	void prepareFilter(const std::vector<float>& filter);
	void prepareOpenCL();

};

#endif // GPU_FILTER_H