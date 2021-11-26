#ifndef __IMAGE_PNM_H
#define __IMAGE_PNM_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

template<typename T> 
struct RGB {T r,g,b;};

template<typename T>
std::ostream& operator<<(std::ostream& os, const RGB<T>& pixel){
  return os<<pixel.r<<" "<<pixel.g<<" "<<pixel.b;
}
template<typename T>
std::istream& operator>>(std::istream& is, RGB<T>& pixel){
  is>> pixel.r>>pixel.g>>pixel.b; 
  return is;
}
  
template<typename T>
class Image{
 private:
  std::vector<T> _data; // vector of pixel value
  size_t _row,_col;     // row and column dimension
  std::string _type;    // a string encoding the image format ("P1"=PBM, "P2"=PGM, "P3"=PPM) 
  int range_value;
  
 public:
  
  
  Image(std::string type, size_t w=0, size_t h=0) : _type(type), _data(h*w), _row(h), _col(w) {}


  // method for accessing image width and height
  size_t width()  const {return _col;}
  size_t height() const {return _row;}

  // method to access individual pixel in the image
  T&       get(size_t i, size_t j)       {return _data[i*_col+j];}
  const T& get(size_t i, size_t j)const  {return _data[i*_col+j];}

  // method to get access to the underlying image as an array through a pointer
  T*       getData()       {
    return _data.data();
    range_value=3;
    _col=512;
    _row=512;
  }  
  const T* getData()const  {return _data.data();range_value=3;}


  // method to read the image from a file
  void read(const char * file){    
    std::ifstream is(file);
    std::string control;
    
    is>>control;
    if (control!=_type){
      std::cerr<<"file "<<file<<" does not have the correct format.... aborting"<<std::endl;
      return ;
    }
    is>>_col>>_row;
    _data.resize(_row*_col);
    std::cout<<"reading an image of size: "<<_col<<"x"<<_row<<std::endl;
     std::cout<<range_value<<std::endl;
    if (_type != "P1")
      is>>range_value;
    _data.resize(range_value*_row*_col);
    _col=range_value*_col;

    for (size_t i=0;i<_row;i++)
    for(size_t j=0;j<_col;j++)
        is>>_data[i*_col+j];


  }
  
  // method to write the image to a file
  void write(const char * file) {     
    std::ofstream os(file);
    std::string control;
    os<<_type<<std::endl;
    _col=1536;
    range_value=3;
    std::cout<<"hello"<<_col<<std::endl;
    os<<_col/range_value<<" "<<_row<<std::endl;
    if (_type != "P1")
      os<<255<<std::endl;  
        for (size_t i=0;i<_row;i++){
          for(size_t j=0;j<_col;j++){
    	         os<<_data[i*_col+j]<<" ";

          }
          os<<std::endl;
          
        }
        
    
  } 
};

#endif
