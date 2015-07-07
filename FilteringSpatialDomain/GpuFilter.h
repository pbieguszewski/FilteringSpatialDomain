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
	std::vector<cl_float3> inImg;
	std::vector<cl_float3> outImg;
	std::vector<cl_float> filter;

	cl::Context context;
	cl::CommandQueue queue;
	cl::Kernel kernel_setFilter;

	void prepareImgIn(const QImage& img, std::size_t dim);
	void prepareFilter(const std::vector<float>& filter);
	void prepareOpenCL();
	void setFilter(int width, int height, std::size_t dim);
	QImage prepareImgOut(int width, int height, std::size_t dim);

};

#endif // GPU_FILTER_H