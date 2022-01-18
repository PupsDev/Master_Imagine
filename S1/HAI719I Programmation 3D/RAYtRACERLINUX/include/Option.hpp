class Option
{


    public:
        float imageAspectRatio;
        
        float width;
        float height;
        int depth;
        bool realtime;
        bool saveImage;
        Option()
        {

            this->width = 1080.;
            this->height = 640.;
            this->imageAspectRatio = this->width / this->height;
            this->depth=1;
            this->realtime=false;
            this->saveImage=true;
        }
        Option(std::string* opt)
        {
            this->width = std::stof(opt[0]);
            this->height = std::stof(opt[1]);
            
            this->depth = std::stoi(opt[2]);
            if(std::stoi(opt[3]))
                this->realtime=true;
            else
            {
                this->realtime=false;
            }
            this->imageAspectRatio = this->width / this->height;
            if(std::stoi(opt[4]))
                this->saveImage=true;
            else
            {
                this->saveImage=false;
            }

        }


};