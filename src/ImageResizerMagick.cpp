
#include "ImageResizerMagick.h"

#include <fstream>

#include <magick/api.h>

#include <boost/algorithm/string.hpp>

using namespace std;

namespace alg = boost::algorithm;


// Helper class for automatic GM initialization
class GraphicsMagickInitializer
{
public:
	GraphicsMagickInitializer()
	{
		Magick::InitializeMagick(NULL);
	}

	~GraphicsMagickInitializer() {}
};

// Initilize GM
static GraphicsMagickInitializer gm_init;


//-----------------------------------------------------------------------------
ImageResizerMagick::ImageResizerMagick(const string &source)
	:m_source(source)
{
	m_prev = m_source;
}

//-----------------------------------------------------------------------------
ImageResizerMagick::ImageResizerMagick(const string &source, const string &size)
	:m_source(size, source)
{
	m_prev = m_source;
}

//-----------------------------------------------------------------------------
ImageResizerMagick::~ImageResizerMagick()
{

}

//-----------------------------------------------------------------------------
bool ImageResizerMagick::resize(const std::string &dest, const Size &size)
{
	if (!size.usePrevious())
	{
		m_prev = m_source;
	}

	bool result;
	if (size.mode() == Size::ResizeMode::FIT)
	{
		result = fit(size);
	}
	else if (size.mode() == Size::ResizeMode::STRETCH)
	{
		result = stretch(size);
	}
	else if (size.mode() == Size::ResizeMode::PAD)
	{
		result = pad(size);
	}
	else if (size.mode() == Size::ResizeMode::FILL_CROP)
	{
		result = crop(size);
	}
	else 
	{
		result = false;
	}

	if (result)
	{
		m_prev.strip();
		m_prev.write(dest);
	}

	return result;
}

//-----------------------------------------------------------------------------
bool ImageResizerMagick::writeExif(const std::string &dest)
{
	try 
	{
		ofstream outs(dest.c_str(), ios::out);

		string exif = m_source.attribute("EXIF:*");

		vector<string> exifValues;
		alg::split(exifValues, exif, alg::is_any_of("\n"));

		for (int i = 0; i < exifValues.size(); ++i)
		{
			if (!alg::starts_with(exifValues[i], "MakerNote="))
			{
				outs << exifValues[i] << "\n";
			}
		}

		outs.flush();

		return true;
	}
	catch (...)
	{
		return false;
	}

}

//-----------------------------------------------------------------------------
bool ImageResizerMagick::fit(const Size &size)
{
	Magick::Geometry geometry(size.width(), size.height());
	geometry.aspect(false);
	geometry.greater(true);

	if (!geometry.isValid())
	{
		return false;
	}

	m_prev.scale(geometry);
	return true;
}

//-----------------------------------------------------------------------------
bool ImageResizerMagick::stretch(const Size &size)
{
	Magick::Geometry geometry(size.width(), size.height());
	geometry.aspect(true);
	geometry.greater(true);

	if (!geometry.isValid())
	{
		return false;
	}

	m_prev.scale(geometry);
	return true;
}

//-----------------------------------------------------------------------------
bool ImageResizerMagick::pad(const Size &size)
{
	Magick::Geometry geometry(size.width(), size.height());
	geometry.aspect(false);
	geometry.greater(true);

	if (!geometry.isValid())
	{
		return false;
	}

	m_prev.scale(geometry);

	Magick::Image background(geometry, Magick::Color(size.background()));	
	background.composite(m_prev, Magick::CenterGravity, Magick::CopyCompositeOp);

	m_prev = background;
	return true;
}

//-----------------------------------------------------------------------------
bool ImageResizerMagick::crop(const Size &size)
{
	Magick::Geometry sourceSize = m_prev.size();
	Magick::Geometry cropSize(size.width(), size.height());
	Magick::Geometry destSize;

    double dx = (double)size.width() / sourceSize.width();
    double dy = (double)size.height() / sourceSize.height();

	if (dx > dy)
	{
		destSize.width(size.width());
		destSize.height(sourceSize.height() * dx);
		cropSize.yOff((destSize.height() - size.height()) / 2);		
	}
	else
	{
		destSize.height(size.height());
		destSize.width(sourceSize.width() * dy);
		cropSize.xOff((destSize.width() - size.width()) / 2);
	}

	m_prev.scale(destSize);
	m_prev.crop(cropSize);	

	return true;
}
