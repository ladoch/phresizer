
#ifndef _IMAGE_RESIZER_GRAPHICS_MAGICK_H
#define _IMAGE_RESIZER_GRAPHICS_MAGICK_H 

#include "ImageResizer.h"

#include <Magick++.h>

/**
 * Image resizer implemented using GrphicsMagick
 */
class ImageResizerMagick
	:public ImageResizer
{
public:
	/**
	 * Create resizer
	 */
	ImageResizerMagick(const std::string &source);
	ImageResizerMagick(const std::string &source, const std::string &size);

	/**
	 * Destructor
	 */
	virtual ~ImageResizerMagick();

public:
	/**
	 * Resize operation
	 * @param dest Destination path.
	 * @param size Resizing parameters.
	 */
	virtual bool resize(const std::string &dest, const Size &size);

	/**
	 * Writes exif info to specified file
	 */
	virtual bool writeExif(const std::string &dest);

private:
	// Resize with FIT strategy
	bool fit(const Size &size);

	// Resize with STRETCH strategy
	bool stretch(const Size &size);

	// Resize with PAD strategy
	bool pad(const Size &size);

	// Resize with CROP strategy
	bool crop(const Size &size);

private:
	// Source
	Magick::Image m_source;

	// Previous resized image
	Magick::Image m_prev;
};

#endif
