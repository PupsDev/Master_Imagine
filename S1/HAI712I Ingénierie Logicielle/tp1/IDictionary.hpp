#ifndef DEF_IDICTIONARY
#define DEF_IDICTIONARY
#include <string>
class IDictionary 
{  
	protected :
		std::string* key;
		std::string* value;
		int currentSize;

	public :

		virtual std::string getValue(std::string _key);
		virtual std::string putValue(std::string _key, std::string _value);

		virtual bool hasKey(std::string _key);
		virtual bool isEmpty();

		virtual int indexOf(std::string _key)
	   {
	   		int currentKey = -1;
	   		for(int i = 0 ; i< currentSize; i++)
	   		{
	   			if(_key == key[i] )
	   				currentKey = i;
	   		} 
	   		return currentKey;
	   }
	   virtual int newIndexOf(std::string _key)
	   {
	   		int index =-1;
		   	if(indexOf(_key)==-1)
		   	{
		   		index = currentSize+1;
		   	}
		   	return index;
	   }
	   virtual int size()
	   {
	   		return currentSize;
	   }
};


#endif