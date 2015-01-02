#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <mutex>

//
// If you would like to make a class to be a singleton,
// you should always the Singleton<> wrapper to get the
// ONE AND ONLY ONE instance of that class. To do this,
// this wrapper requires that the object is defaultly 
// constructible.
//
// This Singleton<MyClass> warpper will guarentee that:
// If you use its GetInstance() method, you will always
// get the same ONE AND ONLY ONE instance of MyClass.
//
// Please note that: this Singleton<> wrapper works
// perfectly in Multi-threaded environments.
//


template<class ObjType>
class SingletonWrapper
{
public:
	virtual ~SingletonWrapper() {}
	static ObjType *GetInstance();
private:
	static ObjType *m_instance;
	// Mutex to protect multi threaded construction of ObjType
	static std::mutex m_mutex;
};

template<class ObjType>
ObjType *SingletonWrapper<ObjType>::GetInstance()
{
	// this lock only locks once when building the only ObjType instance.
	// When running in multi-threaded environments, multiple threads may
	// call GetInstance simultaneously, causing multiple ObjType instance
	// to be created. It will not lock anymore after the initial instantiation.
	if (m_instance == nullptr)
	{
		std::lock_guard<std::mutex> lk(m_mutex);

		if (m_instance == nullptr)
		{
			static ObjType obj;	// Create ObjType on stack, ObjType must be default constructible
			m_instance = &obj;
		}
	}
	return m_instance;
}

// instantiation of internal static members
template< class ObjType > ObjType *SingletonWrapper<ObjType>::m_instance = nullptr;
template< class ObjType > std::mutex SingletonWrapper<ObjType>::m_mutex;

#endif
