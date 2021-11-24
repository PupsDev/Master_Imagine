
#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <time.h>
#define M_PI 3.14159265358979323846
using namespace std;

void SetWindow(int Width, int Height) 
{ 
    _COORD coord; 
    coord.X = Width; 
    coord.Y = Height; 

    _SMALL_RECT Rect; 
    Rect.Top = 0; 
    Rect.Left = 0; 
    Rect.Bottom = Height - 1; 
    Rect.Right = Width - 1; 

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
} 


struct Option
{
    float width = 640.0;
    float height = 360.0;

    float nearClip = 5.;
    float farClip = 100.;
    float angleCamera = M_PI/6;
    float focal = 60.;
    float sensorW = 36;
    float fov = 2*atan(sensorW/(2*focal));
    //float angleCamera = fov * (M_PI/180);


    float clipPlaneWidth = nearClip*10*tan(angleCamera);
    float clipPlaneHeight = clipPlaneWidth * 9/16;

};



class Render
{
    private:
        Scene scene;
        Option option;
    public:
        Render(Scene scene, Option option):scene(scene),option(option){}

 

        string Raytrace(Vec3 origin)
        {

           float fov = 150.;
           float width = 320.;
           float height=130.;
           float scale = tan(deg2rad(fov * 0.5)); 
           float imageAspectRatio = width / (float)height;
           int depth = 5;
           HDC hdc = GetDC(GetConsoleWindow());
            SetWindow(width,height);
            float pixelX, pixelY;
            float pixelZ = -option.nearClip;
             string test = "P3\n"+ to_string((int)width) + " " + to_string((int)height) +"\n255\n";
            int threadNumbers = 1;
            for(int i =0 ; i< height ; i+=threadNumbers)
            {
                pixelY = (1 - 2 * (i + 0.5) / (float)height) * scale;

                for(int j = 0 ; j < width ; j++)
                {
                    pixelX = (2 * (j + 0.5) / (float)width - 1) * imageAspectRatio * scale;
                    
                    Vec3 pixelClip(pixelX,pixelY,pixelZ);

                    Vec3 rayDirection = pixelClip-origin;
                    rayDirection.normalize();
                    Ray ray(origin,rayDirection);

                    Vec3 color = trace(ray,depth);
                    
                    test+=  to_string(int(clip(color[0]*255,0,255)))+" "+
                            to_string(int(clip(color[1]*255,0,255))) +" "+
                            to_string(int(clip(color[2]*255,0,255)))+"\n";
                            
                    int red = int(clip(color[0]*255,0,255));
                    int green =int(clip(color[1]*255,0,255));
                    int blue =int(clip(color[2]*255,0,255));
                    SetPixel(hdc, j, i, RGB(red, green, blue));

                }
                test+="\n";
            }  
            return test;
        }

};
