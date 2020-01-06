#pragma once
#include <string>
#include <experimental/filesystem>
#include <vector>
#include <functional>

namespace fs = std::experimental::filesystem;

namespace CommonUtilities
{
	class FileWatcher
	{
	public:
		
		static void CreateInstance();
		static void DestroyInstance();

		static FileWatcher* GetInstance();

		void Register(const std::string& aFilePath, const std::function<void()>& aCallbackFunction);

		void Update();
		void InvokeCallbacks();

	private:

		class FileCallbackPair
		{
		public:
			FileCallbackPair()
			{
				myFilePath = "";
			}

			FileCallbackPair(const std::string& aFilePath, const std::function<void()> aCallback)
			{
				myFilePath = aFilePath;
				myCallback = aCallback;

				myLastWriteTime = fs::last_write_time(myFilePath);
				myNewWriteTime = fs::last_write_time(myFilePath);
			}

			void UpdateFileTime()
			{
				myNewWriteTime = fs::last_write_time(myFilePath);
			}

			bool FileHasChanged()
			{
				return myNewWriteTime != myLastWriteTime;
			}

			void InvokeCallback()
			{
				if (FileHasChanged())
				{
					myLastWriteTime = myNewWriteTime;
					myCallback();
				}
			}

		private:		

			fs::file_time_type myLastWriteTime;
			fs::file_time_type myNewWriteTime;
			std::string myFilePath;
			std::function<void()> myCallback;		
		};

		FileWatcher();
		~FileWatcher();

		std::vector<FileCallbackPair> myRegisteredWatchers;
		static FileWatcher* ourInstance;

	};

}