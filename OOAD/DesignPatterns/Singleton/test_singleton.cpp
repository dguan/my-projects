#include <iostream>
#include <string>

#include "singleton.h"

class MyClass
{
public:
	MyClass() {};

	void Initialize(const std::string& name) { m_name = name; };
	std::string GetName(void) const { return m_name; };
	void SetName(const std::string& nm) { m_name = nm; };
private:
	std::string m_name;
	
	MyClass(const MyClass&) = delete;
	MyClass& operator = (const MyClass &) = delete;
};

int main()
{
	// The following 2 lines still work, but this is not
	// the way MyClass is supposed to work.
	MyClass mc0;
	MyClass *pmc0 = new MyClass();
	
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
	SingletonWrapper<MyClass> singleton_MyClass;
	MyClass *pmc1 = singleton_MyClass.GetInstance();
	pmc1->Initialize("12345");
	
	MyClass *pmc2 = SingletonWrapper<MyClass>::GetInstance();
	pmc1->Initialize("abcde");
	
	MyClass *pmc3 = SingletonWrapper<MyClass>::GetInstance();
	pmc2->SetName("UVWXYZ");
	
	std::cout << pmc1 << " --- " << pmc2 << " --- " << pmc3 << std::endl;
	std::cout << pmc1->GetName() << " --- " << pmc2->GetName() << " --- " << pmc3->GetName() << std::endl;
	
	
	delete pmc0;
	return 0;
}
