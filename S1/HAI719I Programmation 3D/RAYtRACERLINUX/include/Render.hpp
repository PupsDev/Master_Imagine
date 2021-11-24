#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <time.h>


using namespace std;

class Render
{
    private:

        std::vector<std::vector <Vec3>> image;
        Option *option;
        Camera *camera;
        Raytracer * raytracer;
        Scene * scene;
        atomic<int> jobsDone{ 0 };
        atomic<int> nextJob{ 0 };
        Vec3 origin;
    public:
        Render(Option *optionImage, Camera *camera, Scene * scene)
        {
            
            this->camera = camera;
    
            this->scene = scene;
            
            this->setConfig(optionImage);
            cout<<option->depth<<endl;
            this->raytracer = new Raytracer(scene, option->depth);

        }
        Ray computePixelRay(Vec3 pixelClip, Vec3 origin)
        {
            Vec3 rayDirection = pixelClip-origin;

            rayDirection.normalize();
            return Ray(origin,rayDirection);

        }
        void drawRealtime(Vec3 origin)
        {
            
            float factor = this->camera->GetScale() * this->option->imageAspectRatio;
            float pixelX, pixelY;
            float pixelZ = -camera->GetNearClip();
  
            int threadNumbers = 1;
            int sample = 2;

            for(int i =0 ; i< option->height ; i+=sample)
            {
                pixelY = (1 - 2 * (i + 0.5) / (float)option->height) * this->camera->GetScale();

                for(int j = 0 ; j < option->width ; j+=sample)
                {
                    pixelX = (2 * (j + 0.5) / (float)option->width - 1) * factor;
                    
                    Vec3 pixelClip(pixelX,pixelY,pixelZ);

                    Ray ray = this->computePixelRay(pixelClip, origin);
                    
                    Vec3 color = raytracer->Raytrace(ray);
                    this->image[i][j]=color;
                           
                }
                
            }  
        }
        void draw(Vec3 origin)
        {

            float factor = this->camera->GetScale() * this->option->imageAspectRatio;
            float pixelX, pixelY;
            float pixelZ = -camera->GetNearClip();
  
            int threadNumbers = 1;
            int sample = 1;

            for(int i =0 ; i< option->height ; i+=sample)
            {
                pixelY = (1 - 2 * (i + 0.5) / (float)option->height) * this->camera->GetScale();

                for(int j = 0 ; j < option->width ; j+=sample)
                {
                    pixelX = (2 * (j + 0.5) / (float)option->width - 1) * factor;
                    
                    Vec3 pixelClip(pixelX,pixelY,pixelZ);

                    Ray ray = this->computePixelRay(pixelClip, origin);
                    
                    //Vec3 color = raytracer->Raytrace(ray);
                    //this->image[i][j]=color;

                    //Ray ray = this->computePixelRay(pixelClip, this->origin);
                    //Vec3 pixelClip(pixelX,pixelY,pixelZ);
                    Vec3 color=Vec3(0.,0.,0.);
                    int n =10;
                    for(int s =0;s<n;s++)
                    {

                        float spx = pixelX + (float)(rand()/ (float)(RAND_MAX + 1.0))/(float)option->width;
                        float spy= pixelY+ (float)(rand() / (float)(RAND_MAX + 1.0))/(float)option->height;
                        pixelClip=Vec3(spx,spy,pixelZ);
                        Ray ray = this->computePixelRay(pixelClip, origin);
                        color +=  raytracer->Raytrace(ray)/(float)n;
                    }
                    //Vec3 color = raytracer->Raytrace(ray);
                    this->image[i][j]=color;
                           
                }
                
            }  
        }
        void draw2()
        {
            do
            {

                float factor = this->camera->GetScale() * this->option->imageAspectRatio;
                float pixelX, pixelY;
                float pixelZ = -camera->GetNearClip();
    
                int threadNumbers = 1;
                int sample = 1;

                int i=this->nextJob++;
                pixelY = (1 - 2 * (i + 0.5) / (float)option->height) * this->camera->GetScale();

                for(int j = 0 ; j < option->width ; j+=sample)
                {
                    pixelX = (2 * (j + 0.5) / (float)option->width - 1) * factor;
                    
                    Vec3 pixelClip(pixelX,pixelY,pixelZ);

                    //Ray ray = this->computePixelRay(pixelClip, this->origin);
                    Vec3 color=Vec3(0.,0.,0.);
                    int n =25;
                    for(int s =0;s<n;s++)
                    {

                        float spx = pixelX + (float)(rand()/ (float)(RAND_MAX + 1.0))/(float)option->width;
                        float spy= pixelY+ (float)(rand() / (float)(RAND_MAX + 1.0))/(float)option->height;
                        pixelClip=Vec3(spx,spy,pixelZ);
                        Ray ray = this->computePixelRay(pixelClip, this->origin);
                        color +=  raytracer->Raytrace(ray)/(float)n;
                    }
                    //Vec3 color = raytracer->Raytrace(ray);
                    this->image[i][j]=color;
                            
                }
                
            	++this->jobsDone;
            if (this->jobsDone % 10 == 0)
            {
                
                std::cout << this->jobsDone << "\\" << option->height << " jobs finished." << std::endl;
            }
            } while (this->nextJob < option->height);
        }
        void drawThread(Vec3 Origin)
        {
                this->origin = Origin;
            	unsigned int numThreads = thread::hardware_concurrency();
                vector<thread> workers;

                unsigned int nextThreadIndex = 0;
                while (nextThreadIndex < numThreads && nextThreadIndex < (unsigned int)option->height)
                {
                    workers.emplace_back(thread(&Render::draw2, this));
                    ++nextThreadIndex;
                }

                numThreads = (int)workers.size();
                std::cout << numThreads << " workers launched to complete " << option->height << " jobs." << endl;

                for (auto& worker : workers)
                {
                    worker.join();
                }

        }
        float clip(float n, float lower, float upper) {
        if(n < lower)return lower;
        if(n>upper)return upper;
        return n;
            
        }
        std::vector<std::vector <Vec3>> getRender()
        {
            return image;
        }
        void saveImage(string filename)
        {
            string data = "P3\n"+ to_string((int)option->width) + " " + to_string((int)option->height) +"\n255\n";
            ofstream myfile;
            myfile.open (filename);

            for(int i =0 ; i< option->height ; i++)
            {
                for(int j = 0 ; j < option->width ; j++)
                {
                    Vec3 colorPixel = image[i][j];
                    
                    int r = int(this->clip(colorPixel[0]*255,0,255));
                    int g = int(this->clip(colorPixel[1]*255,0,255));
                    int b = int(this->clip(colorPixel[2]*255,0,255));

                    data+=  to_string(r)+" "+
                            to_string(g) +" "+
                            to_string(b)+"\n"; 
                }
                data+="\n";
            }
            myfile << data <<endl;
            
            myfile.close();
        }
        void SetCamera(Ray * ray)
        {
            this->camera->SetRay(ray);
        }
        void setConfig(Option *optionImage)
        {          
            this->option = optionImage;
            this->image = std::vector<std::vector <Vec3>>(option->height);
            for (int i = 0 ; i < option->height ; i++) {
                image[i].resize(option->width);
            }
            
        }
};
