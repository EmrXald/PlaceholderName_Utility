#include "FileWatcher.h"
#include "DL_Assert.h"
#include "DL_Debug.h"
#include "Macro.h"

CommonUtilities::FileWatcher* CommonUtilities::FileWatcher::ourInstance = nullptr;

CommonUtilities::FileWatcher::FileWatcher()
{

}


CommonUtilities::FileWatcher::~FileWatcher()
{

}

void CommonUtilities::FileWatcher::CreateInstance()
{
	DL_ASSERT(ourInstance == nullptr && "Filewatcher instance has already created. call createInstance only once.\n");
	ourInstance = new FileWatcher();
}

void CommonUtilities::FileWatcher::DestroyInstance()
{
	DL_ASSERT(ourInstance == nullptr && "Filewatcher instance has already created. call createInstance only once.\n");
	SAFE_DELETE(ourInstance);
}

CommonUtilities::FileWatcher * CommonUtilities::FileWatcher::GetInstance()
{
	DL_ASSERT(ourInstance != nullptr && "Filewatcher instance has not been created. call CreateInstance upon startup to create a Filewatcher.\n");
	return ourInstance;
}

void CommonUtilities::FileWatcher::Register(const std::string & aFilePath, const std::function<void()>& aCallbackFunction)
{
	myRegisteredWatchers.push_back(FileCallbackPair(aFilePath, aCallbackFunction));
}

void CommonUtilities::FileWatcher::Update()
{
	for (unsigned int i = 0; i < myRegisteredWatchers.size(); ++i)
	{
		myRegisteredWatchers[i].UpdateFileTime();
	}
}

void CommonUtilities::FileWatcher::InvokeCallbacks()
{
	for (unsigned int i = 0; i< myRegisteredWatchers.size(); ++i)
	{
		myRegisteredWatchers[i].InvokeCallback();
	}
}
