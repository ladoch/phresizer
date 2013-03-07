
#ifndef _IMAGE_RESIZER_H
#define _IMAGE_RESIZER_H 

#include "Size.h"
#include "Config.h"

#include <string>

#include <boost/smart_ptr.hpp>


/**
 * Interface for image resizers. 
 * Image resizer is responsible for resizng image to multiple sizes.
 */
class ImageResizer
{
public:
	typedef boost::shared_ptr<ImageResizer> AutoPtr;

	/**
	 * Create implementation instance	 
	 */
	static AutoPtr create(const std::string &file, const Config &conf);

	/**
	 * Resize operation
	 * @param dest Destination path.
	 * @param size Resizing parameters.
	 */
	virtual bool resize(const std::string &dest, const Size &size) = 0;
};

#endif
