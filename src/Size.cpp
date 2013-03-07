
#include "Size.h"

#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;

namespace alg = boost::algorithm;


const std::string Size::ResizeMode::FIT = "fit";
const std::string Size::ResizeMode::STRETCH = "stretch";
const std::string Size::ResizeMode::PAD = "pad";
const std::string Size::ResizeMode::FILL_CROP = "crop";


//-----------------------------------------------------------------------------
Size::Size()
	:m_mode(ResizeMode::FIT)
	,m_background("#ffffff")
{

}

//-----------------------------------------------------------------------------
Size::Size(const string &spec)
	:m_mode(ResizeMode::FIT)
	,m_background("#ffffff")
{
	vector<string> params;
	alg::split(params, spec, alg::is_any_of(","));

	for (int i = 0; i < params.size(); i++)
	{
		vector<string> key_value;
		alg::split(key_value, params[i], alg::is_any_of(":"));

		if (key_value.size() != 2)
		{
			continue;
		}

		string key = key_value[0];
		string value = key_value[1];

		if (key == "a")
		{
			m_alias = value;
		}
		else if (key == "m")
		{
			m_mode = value;
		}
		else if (key == "b")
		{
			m_background = value;
		}
		else if (key == "u")
		{
			m_use_previous = value == "true";
		}
		else if (key == "s")
		{
			if (m_alias.empty())
			{
				m_alias = value;
			}
		
			vector<string> size;
			alg::split(size, value, alg::is_any_of("x"));

			if (size.size() == 2)
			{
				m_width = atoi(size[0].c_str());
				m_height = atoi(size[1].c_str());
			}
		}
	}
}

//-----------------------------------------------------------------------------
Size::Size(const Size &other)
{
	copy(other);
}

//-----------------------------------------------------------------------------
Size &Size::operator =(const Size &other)
{
	copy(other);
	return *this;
}

//-----------------------------------------------------------------------------
Size::~Size()
{

}

//-----------------------------------------------------------------------------
bool Size::isValid() const
{
	return m_height > 0 && m_width > 0;
}

//-----------------------------------------------------------------------------
int Size::width() const
{
	return m_width;
}

//-----------------------------------------------------------------------------
int Size::height() const
{
	return m_height;
}

//-----------------------------------------------------------------------------
const string Size::alias() const
{
	return m_alias;
}

//-----------------------------------------------------------------------------
const string Size::mode() const
{
	return m_mode;
}

//-----------------------------------------------------------------------------
const string Size::background() const
{
	return m_background;
}

//-----------------------------------------------------------------------------
bool Size::usePrevious() const
{
	return m_use_previous;
}

//-----------------------------------------------------------------------------
void Size::copy(const Size &other)
{
	m_width = other.width();
	m_height = other.height();
	m_mode = other.mode();
	m_alias = other.alias();
	m_background = other.background();
}

//-----------------------------------------------------------------------------
istream &operator >>(istream &input, Size &size)
{
	string spec;
	input >> spec;

	size.copy(Size(spec));
	return input;
}

//-----------------------------------------------------------------------------
ostream &operator <<(ostream &output, const Size &size)
{
	return output << 
			std::string("a:") << size.alias() << "," <<	
			std::string("m:") << size.mode() << "," <<
			std::string("b:") << size.background() << "," <<
			std::string("s:") << size.width() << "," << size.height();
}
