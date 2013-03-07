
#include <string>
#include <sys/time.h>

#include <boost/filesystem.hpp>

#include <GraphicsMagick/Magick++.h>

#include "Config.h"
#include "ImageResizer.h"
#include "Version.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

// Paths vector type definition
typedef vector<fs::path> path_vector;           

double utcms()
{
	timeval tim;
    gettimeofday(&tim, NULL);

    return tim.tv_sec * 1000.0 + (tim.tv_usec / 1000.0);
}

/**
 * Entry point
 */
int main(int argc, char const *argv[])
{
	// Read configuration
	Config conf(argc, argv);

	// Show help by request
	if (conf.command() == "help") 
	{
	    conf.printHelp();
	    return 1;
	}

    if (conf.command() == "version")
    {
        cout << VERSION << "\n";
        return 0;
    }

	if (!conf.isValid())
	{
		conf.printErrors();
		return -1;
	}

	if (conf.isVerbose())
	{
		cout << "Config params:\n";
		cout << "source = " << conf.source() << "\n";
		cout << "dest = " << conf.dest() << "\n";
		cout << "src-size = " << conf.sourceSize() << "\n";

		for (int i = 0; i < conf.sizes().size(); ++i)
		{
			cout << "size[" << i << "] = " << conf.sizes()[i] << "\n";
		}
	}
		
    path_vector files;                                			

    // Copy paths of files form input directory to 'files' vector and then sort
    copy(fs::directory_iterator(conf.source()), fs::directory_iterator(), back_inserter(files));
    sort(files.begin(), files.end());

    vector<Size> sizes = conf.sizes();
    for (int i = 0; i < sizes.size(); ++i)
    {
    	// Make output path
		fs::path out_dir = fs::path(conf.dest()) / sizes[i].alias();
		fs::create_directories(out_dir);    	
    }

    double start = utcms();

	// Iterate throught input directory files
    for (path_vector::const_iterator it(files.begin()); it != files.end(); ++it)
    {
    	string file_path = fs::absolute(*it).native();

    	// Skip directories
    	if (!fs::is_regular_file(file_path))
    	{
    		continue;
    	}

    	if (conf.isVerbose())
    	{
    		cout << "Process " << file_path << " file\n";      	
    	}
    	
    	try
    	{
            // Create resizer
    		ImageResizer::AutoPtr resizer = ImageResizer::create(file_path, conf);

            for (int i = 0; i < sizes.size(); ++i)
            {
                fs::path out_path = fs::path(conf.dest()) / sizes[i].alias() / it->filename();
                string dest = fs::absolute(out_path).native();

                // Resize
                resizer->resize(dest, sizes[i]);
            }
    	}
    	catch(std::runtime_error &ex) 
    	{
    		if (conf.isVerbose()) 
    		{
    			cout << "Exception: " << ex.what() << "\n";    			
    		}

    		return -1;
    	}    	
    }

    double end = utcms();

    if (conf.isVerbose())
    {
    	cout << "Time spent: " << (int)(end - start) << " ms\n";
    }
	
	return 0;
}
