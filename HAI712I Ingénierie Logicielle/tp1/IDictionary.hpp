#ifndef DEF_IDICTIONARY
#define DEF_IDICTIONARY

#include <stdexcept>

template <typename Object> 
class IDictionary 
{  
	protected :
		Object* key;
		Object* value;
		int currentSize;

	public :

		virtual Object getValue(Object _key)
		{
			int i = this->indexOf(_key);
			if ( i != -1)
				return value[i];
			else
				throw std::logic_error("Cette clé n'est pas présente");
		}
		
		virtual void putValue(Object _key, Object _value)
		{	
			int index;
			if(!this->hasKey(_key))
			{
				index = this->newIndexOf(_key);
				value[index]=_value;
				key[index] =_key;

			}
		}
		
		virtual bool hasKey(Object _key)
		{
			bool keyIsPresent = false;
			for(int i=0; i<this->currentSize && !keyIsPresent ;i++)
				if(key[i]==_key)
					keyIsPresent = true;
			return keyIsPresent;

		}
		
		virtual bool isEmpty()
		{
			return currentSize == 0;
		}

		virtual int indexOf(Object _key)=0;
	    virtual int newIndexOf(Object _key)=0;
	    virtual int size()=0;
};


#endif