
#include "Config.h"
#include "Size.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>


using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;


const char *Config::Options::HELP = "help";
const char *Config::Options::VERSION = "version";
const char *Config::Options::SOURCE = "source";
const char *Config::Options::DEST = "dest";
const char *Config::Options::SIZE = "size";
const char *Config::Options::CONF = "conf";
const char *Config::Options::VERBOSE = "verbose";
const char *Config::Options::SRC_SIZE = "src-size";
const char *Config::Options::META = "meta";
const char *Config::Options::CONTENTS = "contents";


//-----------------------------------------------------------------------------
Config::Config(int argc, char const*argv[])
	:m_config_description("Allowed options")
	,m_command("convert")
{
	typedef Config::Options Options;

	// Declare the supported options.
	m_config_description.add_options()
	    (Options::HELP, "produce help message")
	    (Options::VERSION, "print version")
	    (Options::SOURCE, po::value<string>(), "set source directory path")
	    (Options::DEST, po::value<string>(), "set destination directory")
	    (Options::SIZE, po::value<vector<Size> >()->multitoken(), 
	    	"add resize operation, format:\n [a:<alias>,][m:fit|stretch|pad|crop,][b:<bgcolor>,][u:true|false,]s:<width>x<height>")
	    (Options::CONF, po::value<string>(), "read configuration form specified file")
	    (Options::VERBOSE, "verbose output")
	    (Options::META, "extract meta information from files and store in separate file")
	    (Options::CONTENTS, "write output contents")
	    (Options::SRC_SIZE, po::value<string>(), "hint to open file at reduced size");

	po::store(po::parse_command_line(argc, argv, m_config_description), m_config_values);

	// Check for help command
	if (m_config_values.count(Options::HELP)) 
	{
	    m_command = "help";
	}
	else if (m_config_values.count(Options::VERSION))
	{
		m_command = "version";		
	}
	else
	{
		// Read config from file if specified
		if (m_config_values.count(Options::CONF)) 
		{
			string path = m_config_values[Options::CONF].as<string>();
			po::store(po::parse_config_file<char>(path.c_str(), m_config_description), m_config_values);
		}

		po::notify(m_config_values);    

		// Validate parameters
		validate();
	}
	
}

//-----------------------------------------------------------------------------	
Config::~Config()
{

}

//-----------------------------------------------------------------------------
string Config::command() const
{
	return m_command;
}

//-----------------------------------------------------------------------------
void Config::printHelp()
{
	cout << m_config_description << "\n";
}

//-----------------------------------------------------------------------------
bool Config::isValid() const
{
	return m_errors.size() == 0;
}

//-----------------------------------------------------------------------------
void Config::printErrors()
{
	if (isVerbose()) 
	{
		for (int i = 0; i < m_errors.size(); i++)
		{
			cout << m_errors[i] << "\n";
		}
	}
}

//-----------------------------------------------------------------------------
bool Config::isVerbose() const
{
	return m_config_values.count(Options::VERBOSE) > 0;
}

//-----------------------------------------------------------------------------
bool Config::isMetaEnabled() const
{
	return m_config_values.count(Options::META) > 0;
}

//-----------------------------------------------------------------------------
bool Config::isContentsEnabled() const
{
	return m_config_values.count(Options::CONTENTS) > 0;
}

//-----------------------------------------------------------------------------
string Config::source() const
{
	return m_config_values[Options::SOURCE].as<string>();
}

//-----------------------------------------------------------------------------
string Config::dest() const
{
	return m_config_values[Options::DEST].as<string>();
}

//-----------------------------------------------------------------------------
vector<Size> Config::sizes() const
{
	return m_config_values[Options::SIZE].as<vector<Size> >();
}

//-----------------------------------------------------------------------------
string Config::sourceSize() const
{
	return m_config_values.count(Options::SRC_SIZE) ?
				m_config_values[Options::SRC_SIZE].as<string>() : "";
}

//-----------------------------------------------------------------------------
void Config::validate()
{
	vector<string> required_params;
	required_params.push_back(Options::SOURCE);
	required_params.push_back(Options::DEST);
	required_params.push_back(Options::SIZE);

	// Check for presence of required parameters
	for (int i = 0; i < required_params.size(); i++)
	{
		string param_name = required_params[i];
		if (!m_config_values.count(param_name)) 
		{
			m_errors.push_back(std::string("--") + param_name + " is required parameter");
		}		
	}	

	if (!isValid())
	{
		return;
	}

	// Check source path
	if (!fs::exists(source()) || !fs::is_directory(source()))
	{
		m_errors.push_back(string("Directory ") + source() + " doesn't exists.");
	}

	// Check ability to create destinations directory

	if (!fs::exists(dest()))
	{
		fs::create_directories(dest());
	}

	if (!fs::exists(dest()) || !fs::is_directory(dest()))
	{
		m_errors.push_back(string("Can not create directory ") + dest());
	}
}
