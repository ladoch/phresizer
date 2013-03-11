
#ifndef _CONFIG_H
#define _CONFIG_H

#include "Size.h"
#include <string>
#include <vector>

#include <boost/program_options.hpp>


/**
 * Store app configuration
 */
class Config
{
public:
	/**
	 * Read configuration from command line
	 * @param argc Count of command line parameters
	 * @param argv Command line parameters	
	 */
	Config(int argc, char const *argv[]);

	/**
	 * Free configuratiom
	 */
	virtual ~Config();

public:
	/**
	 * Get command
	 */
	std::string command() const;

	/**
	 * Print configuration help
	 */
	void printHelp();

	/**
	 * Is configuration valid
	 */
	bool isValid() const;

	/**
	 * Print configuration errors
	 */
	void printErrors();

	/**
	 * Is verbose
	 */
	bool isVerbose() const;

	/**
	 * Is meta info output enabled 
	 */
	bool isMetaEnabled() const;

	/**
	 * Is contents output enabled 
	 */
	bool isContentsEnabled() const;

	/**
	 * Source path
     */
    std::string source() const;

    /**
     * Destination path
     */
    std::string dest() const;

    /**
     * Size definitions
     */ 
    std::vector<Size> sizes() const;

    /**
     * Source size
     */
    std::string sourceSize() const;

private:	
	Config(const Config &);
	
	// Validate parameters
	void validate();

private:
	// Configuration option names
	class Options
	{
	public:	
		static const char *HELP;
		static const char *VERSION;
		static const char *SOURCE;
		static const char *DEST;
		static const char *SIZE;
		static const char *CONF;
		static const char *VERBOSE;
		static const char *SRC_SIZE;
		static const char *META;
		static const char *CONTENTS;
	};

	// Command
	std::string m_command;

	// Configuration description
	boost::program_options::options_description m_config_description;

	// Configuration values
	boost::program_options::variables_map m_config_values;

	// Validation errors
	std::vector<std::string> m_errors;
};

#endif
