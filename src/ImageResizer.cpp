
#include "ImageResizer.h"
#include "ImageResizerMagick.h"


using namespace std;

//-----------------------------------------------------------------------------
ImageResizer::AutoPtr ImageResizer::create(const string &file, const Config &conf)
{
	ImageResizer *resizer;
	if (conf.sourceSize().empty())
	{
		resizer = new ImageResizerMagick(file);
	}
	else
	{
		resizer = new ImageResizerMagick(file, conf.sourceSize());
	}

	return ImageResizer::AutoPtr(resizer);
}
