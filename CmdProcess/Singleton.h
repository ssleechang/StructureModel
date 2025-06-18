// Singleton class
#pragma once
  
template <typename T>
class CSingleton
{
public:
  static T * InstancePtr()
  {
    if( _pInstance == NULL )
		_pInstance = new T;
    return _pInstance;
  };

  static T & Instance()
  {
    if(_pInstance == NULL)
		_pInstance = new T;
    return *_pInstance;
  };

  static void DestroyInstance()
  {
    delete _pInstance;
    _pInstance = NULL;
  };
  
private:
  static T * _pInstance;
};
  
template<typename T> T* CSingleton<T>::_pInstance = 0;