#include <iostream>
#include <fstream>
#include <string>

class Configuration
{
    private:
    Option *option;


    public:
        Configuration(char * argv)
        {
            std::ifstream configFile;
            configFile.open (argv);
            int n = 100;
            std::string * lines = new std::string[n];
            int i = 0;
            std::cout << "reading configuration file..." << '\n';
            std::string linesM[6] = {"width=","height=","depth=","realtime=","saveImage="};
            
            if (configFile.is_open())
            {
                while ( getline (configFile,lines[i]) )
                {

                    std::cout << linesM[i]<<lines[i] << '\n';
                    i++;
                }
                configFile.close();
            }
            
            this->option = new Option(lines);


        }
        Configuration()
        {
            this->option = new Option();
            std::cout << "Default configuration" << '\n';
        }
        Option *GetOption() const {
            return this->option;
        }



};
