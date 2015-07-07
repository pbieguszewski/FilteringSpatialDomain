#include "GpuFilter.h"

GpuFilter::GpuFilter()
{
	kernel =
		"__kernel void setFilter(const __global float3* in, __global float3* out, __constant float* filter, int dim)"
		"{"
		"   int halfDim = (int) dim / 2.0f;"
		"	int y = get_global_id(0);"
		"	int x = get_global_id(1);"
		"	int widht = get_global_size(0) - halfDim;"
		"	int height = get_global_size(1) - halfDim;"
		"	"
		"   if ((y > halfDim && y < widht) && (x > halfDim && x < height))"
		"   {"
		"       float3 sum = (float3) 0.0;"
		"       "
		"       for (int i = -halfDim; i <= halfDim; i++)"
		"           for (int j = -halfDim; j <= halfDim; j++)"
		"               sum += filter[((j + halfDim) * dim) + (i + halfDim)] * in[((y - j) * (widht + halfDim)) + (x - i)];"
		"       "
		"       out[y * widht + x] = sum;"
		"   }"
		"}";

	try { prepareOpenCL(); }
	catch (cl::Error& err) { throw err; }
}

GpuFilter::~GpuFilter() { ; }

QImage GpuFilter::operator()(const QImage& img, std::size_t dim, const std::vector<float>& filter)
{
	prepareImgIn(img, dim);
	prepareFilter(filter);
	setFilter(img.width(), img.height(), dim);

	return prepareImgOut(img.width(), img.height(), dim);
}

void GpuFilter::prepareImgIn(const QImage& img, std::size_t dim)
{
	if (!inImg.empty())
		inImg.clear();

	int halfDim = static_cast<int>(dim / 2);

	for (auto i = 0; i < img.width(); i++)
		for (auto j = 0; j < img.height(); j++)
		{
			auto rgb = img.pixel(j, i);
			inImg.push_back(cl_float3{ { qRed(rgb), qGreen(rgb), qBlue(rgb) } });
		}
	outImg.resize((img.width() - halfDim) * (img.height() - halfDim));
}

void GpuFilter::prepareFilter(const std::vector<float>& filter)
{
	if (!this->filter.empty())
		this->filter.clear();

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

void GpuFilter::setFilter(int width, int height, std::size_t dim)
{
	cl_int tmp = dim;
	cl::Buffer inBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float3) * inImg.size());
	cl::Buffer outBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float3) * outImg.size());
	cl::Buffer filterBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * filter.size());

	queue.enqueueWriteBuffer(inBuffer, true, 0, sizeof(cl_float3) * inImg.size(), &inImg[0]);
	queue.enqueueWriteBuffer(filterBuffer, true, 0, sizeof(cl_float) * filter.size(), &filter[0]);

	kernel_setFilter.setArg(0, inBuffer);
	kernel_setFilter.setArg(1, outBuffer);
	kernel_setFilter.setArg(2, filterBuffer);
	kernel_setFilter.setArg(3, sizeof(tmp), &tmp);

	queue.enqueueNDRangeKernel(kernel_setFilter, cl::NullRange, cl::NDRange(width, height), cl::NullRange);
	queue.finish();

	queue.enqueueReadBuffer(outBuffer, true, 0, sizeof(cl_float3) * outImg.size(), &outImg.front());
}

QImage GpuFilter::prepareImgOut(int width, int height, std::size_t dim)
{
	int halfDim = static_cast<int>(dim / 2);

	QImage img(width - halfDim, height - halfDim, QImage::Format_RGB32);

	for (auto i = 0; i < img.width(); i++)
	{
		for (auto j = 0; j < img.height(); j++)
		{
			auto& tmp = outImg[i * img.width() + j];
			img.setPixel(j, i, qRgb(tmp.s[0], tmp.s[1], tmp.s[2]));
		}
	}

	return img;
}