
#ifndef _SIZE_H
#define _SIZE_H 

#include <string>

/**
 * Stores image resize parameters
 */
class Size
{
public:
	/**
	 * Resize mode
	 */
	class ResizeMode
	{
	public:
		// Uses the width and height as the maximum values for the resize, 
		// preserving aspect ratio.
		static const std::string FIT;

		// Ignores aspect ratio, resizing the image to the exact width and height 
		// specified. Note that this may result in a stretched or distorted image.
		static const std::string STRETCH;

		// Scales the image to fit, and then fills the remaining 
		// width and height with the specified background color.
		static const std::string PAD;

		// Given a rectangle of the specified width and height, 
		// the image will be resized such that the rectangle fits completely 
		// inside the resulting image, with one of the image dimensions the same
		// as that of the provided rectangle. Scaling is performed using the
		// larger scale value; in other words, the larger of dw/w and dh/h is used 
		// to scale both dimensions. This means that the dimension of the 
		// provided rectangle closest to the actual dimension will be resized match 
		// the rectangle dimension.

		// The resulting image is then cropped and horizontally/vertically centered 
		// to fill the rectangle, removing any overlaying parts.
		static const std::string FILL_CROP;
	};

public:
	/**
	 * Initialize with default params:
	 * size: 0x0
	 * alias: ""
	 * mode: FIT
	 */
	Size();

	/**
	 * Constructor
	 * @param spec String specification. Specification format:
	 *		[a:<alias>,][m:fit|stretch|pad|crop,][b:<bgcolor>,][u:true|false,]s:<width>x<height>
	 *
	 *		Where: a - alias, m - resize mode, b - background, 
	 *				u - use previous size as source, s - size 
	 */
	Size(const std::string &spec);

	/**
	 * Copy constructor
	 */
	Size(const Size &other);

	/**
	 * Copy operator
	 */
	Size &operator =(const Size &other);

	/**
	 * Destructor
	 */
	virtual ~Size();

public:
	/**
	 * Is valid
	 */
	bool isValid() const;

	/**
	 * Get width
	 */
	int width() const;

	/**
	 * Get height
	 */
	int height() const;	

	/**
	 * Get string alias to identify
	 */
	const std::string alias() const;

	/**
	 * Resize mode
	 */
	const std::string mode() const;

	/**
	 * Background for pad resize mode
	 */
	const std::string background() const;

	/**
	 * Use previous size as source
	 */
	bool usePrevious() const;

private:
	// Copy values from another instance
	void copy(const Size &other);


	// Read from istream
	friend std::istream &operator >>(std::istream &input, Size &size);

	// Write to ostream
	friend std::ostream &operator <<(std::ostream &output, const Size &size);

private:
	// Width
	int m_width;

	// Height
	int m_height;

	// Resize mode
	std::string m_mode;

	// Alias
	std::string m_alias;

	// Background
	std::string m_background;

	// Use previous size as source
	bool m_use_previous;
};

#endif
