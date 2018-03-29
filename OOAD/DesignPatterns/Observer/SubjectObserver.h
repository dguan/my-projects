#ifndef SUBJECT_OBSERVER_HEADER_FILE
#define SUBJECT_OBSERVER_HEADER_FILE

#include <iostream>
#include <string>
#include <map>

using namespace std;

template<class DataType>
class IObserver;

template<class DataType> class ISubject
{
public:
	virtual ~ISubject() {}
	virtual void Attach(IObserver<DataType>* obs, const string& info) = 0;
	virtual void Detach(IObserver<DataType>* obs) = 0;
	virtual void Notify(const DataType& data) = 0;
	virtual const string GetName() const = 0;
};

template<class DataType> class IObserver
{
public:
	virtual ~IObserver() {}
	virtual void Update(ISubject<DataType>* sub, const DataType& data) = 0;
	virtual void Register(ISubject<DataType>* sub, const string& info) = 0;
	virtual void Unregister(ISubject<DataType>* sub) = 0;
	virtual const string GetName() const = 0;
};

template<class DataType> class Subject : public ISubject<DataType>
{
public:
	Subject(const string& name) : m_name(name) {}
	~Subject() {}
	void Attach(IObserver<DataType> *obs, const string& info);
	void Detach(IObserver<DataType> *obs);
	void Notify(const DataType& data);
	const string GetName() const { return m_name; }
private:
	map<IObserver<DataType>*, string> m_obs;
	string m_name;
};

template<class DataType> class Observer : public IObserver<DataType>
{
public:
	Observer(const string& name) : m_name(name) {}
	~Observer() {};
	void Update(ISubject<DataType> *sub, const DataType& data);
	void Register(ISubject<DataType> *sub, const string& info);
	void Unregister(ISubject<DataType> *sub);
	const string GetName() const { return m_name; }
private:
	map<ISubject<DataType>*, string> m_subs;
	string m_name;
};

template<class DataType> void Subject<DataType>::Attach(IObserver<DataType> *obs, const string& info)
{
	auto itr = m_obs.find(obs);
	if(itr==m_obs.end())
	{
		m_obs[obs] = info;
		obs->Register(this, info);
	}
	else
	{
		itr->second = info;
	}
}

template<class DataType> void Subject<DataType>::Detach(IObserver<DataType> *obs)
{
	auto itr = m_obs.find(obs);
	if(itr != m_obs.end())
	{
		cout << "Subject " << GetName() << "detached object " << obs->GetName() << endl;
		//obs->Unregister(this); // detach should have been initiated by an observer
		m_obs.erase(obs);
	}
}

template<class DataType> void Subject<DataType>::Notify(const DataType& data)
{
	for(auto& obs : m_obs)
	{
		obs.first->Update(this, data);
	}
}

template<class DataType> void Observer<DataType>::Update(ISubject<DataType>* sub, const DataType& data)
{
	cout << "Observer " << GetName() << " got notification from subject " << sub->GetName() << ": " << data << endl; 
}

template<class DataType> void Observer<DataType>::Register(ISubject<DataType> *sub, const string& info)
{
	auto itr = m_subs.find(sub);
	if(itr == m_subs.end())
	{
		cout << "Observer " << GetName() << " registered subject " << sub->GetName() << ", info: " << info << endl;
		m_subs[sub] = info;
	}
	else
	{
		cout << "Observer " << GetName() << " modified subject " << sub->GetName() << "'s info from " << itr->second << " to " << info << endl;
		itr->second = info; 
	}
}

template<class DataType> void Observer<DataType>::Unregister(ISubject<DataType> *sub)
{
	auto itr = m_subs.find(sub);
	if(itr != m_subs.end())
	{
		cout << "Observer " << GetName() << " unregistered subject " << sub->GetName() << endl;
		m_subs.erase(sub);
		sub->Detach(this);
	}
}

#endif

