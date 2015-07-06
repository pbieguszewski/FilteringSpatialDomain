#include "GpuFilter.h"

#include <qdebug.h>

GpuFilter::GpuFilter()
{
	kernel =
		"__kernel void setFilter(const __global float4* in, __global float4* out, __constant float* filter, int dim)\n"
		"{\n"
		"	int y = get_global_id(0);\n"
		"	int x = get_global_id(1);\n"
		"	int widht = get_global_size(0);\n"
		"	int height = get_global_size(1);\n"
		"	\n"
		"	if ((y > (int)(dim / 2) && y < widht - (int)(dim / 2)) && (x > (int)(dim / 2) && x < height - (int)(dim / 2)))\n"
		"	{\n"
		"		float4 sum = (float4) (0);\n"
		"		\n"
		"		for (int i = -(int)(dim / 2); i <= (int)(dim / 2); i++)\n"
		"			for (int j = -(int)(dim / 2); j <= (int)(dim / 2); j++)\n"
		"				;\n" //do this
		"		\n"
		"		outImg[y * widht + x] = sum;\n"
		"	}\n"
		"}\n";

	try { prepareOpenCL(); }
	catch (cl::Error& err) { throw err; }
}

GpuFilter::~GpuFilter() { ; }

QImage GpuFilter::operator()(const QImage& img, std::size_t dim, const std::vector<float>& filter)
{
	prepareImg(img);
	prepareFilter(filter);

	return QImage();
}

void GpuFilter::prepareImg(const QImage& img)
{
	for (auto i = 0; i < img.width(); i++)
		for (auto j = 0; j < img.height(); j++)
		{
			auto rgba = img.pixel(j, i);
			inImg.push_back(cl_float4{ { qRed(rgba), qGreen(rgba), qBlue(rgba), qAlpha(rgba) } });
		}
	outImg.resize(inImg.size());
}

void GpuFilter::prepareFilter(const std::vector<float>& filter)
{
	for (auto &i : filter)
		this->filter.push_back(cl_float(i));
}

void GpuFilter::prepareOpenCL()
{
	std::vector<cl::Platform> platform;

	cl::Platform::get(&platform);

	if (!platform.size())
		throw;

	std::vector<cl::Device> devices;

	platform[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

	if (!devices.size())
		throw;

	cl::Device gpuDevice = devices[0];

	context = cl::Context({ gpuDevice });

	cl::Program::Sources sources = { { kernel.c_str(), kernel.length() } };

	cl::Program program(context, sources);

	program.build({ gpuDevice });

	queue = cl::CommandQueue(context, gpuDevice);

	kernel_setFilter = cl::Kernel(program, "setFilter");
}

