#include "Path.h"
#include <filesystem>
namespace fs = std::filesystem;
namespace DotNetDupe
{
	namespace System
	{
		namespace IO
		{
			String Path::ChangeExtension(const String &filePath, const String &extension)
			{
				if (filePath.GetLength() <= 0 || extension.GetLength() <= 0)
					throw ArgumentException(_T("Invalid argument"));
				if (filePath.GetLength() <= extension.GetLength())
					throw ArgumentException(_T("Invalid argument"));
				auto strTempFilePath = filePath;
				if (filePath[0] == _T('.'))
					strTempFilePath = filePath.Remove(0);

				fs::path path = strTempFilePath.GetRawString();
				path.replace_extension(extension.GetRawString());
				return path.c_str();
			}

			String Path::Combine(const std::initializer_list<String> paths)
			{
				auto indexedList = _init_list_with_indexer(paths);
				fs::path rootPath = indexedList[0].GetRawString();
				if (!rootPath.has_root_directory())
					return indexedList[0];
				String strCombinedPath(_T(""));
				for (auto path : paths)
				{
					strCombinedPath.Append(path);
					strCombinedPath.Append(fs::path::preferred_separator);
				}
				return strCombinedPath.Remove(strCombinedPath.GetLength() - 1);
			}
			bool Path::EndsInDirectorySeparator(const String &filePath)
			{
				fs::path path = filePath.GetRawString();
				if (!path.has_root_path())
					return false;
				return filePath.EndsWith(fs::path::preferred_separator, false);
			}
			bool Path::Exists(const String &filePath)
			{
				return fs::exists(filePath.GetRawString());
			}
			String Path::GetDirectoryName(const String &filePath)
			{
				if (filePath.IsEmpty())
					return _T("");

				fs::path path = filePath.GetRawString();
				return path.parent_path().c_str();
			}
			String Path::GetFileName(const String &filePath)
			{
				if (filePath.IsEmpty())
					return _T("");
				fs::path path = filePath.GetRawString();
				return path.filename().c_str();
			}
			String Path::GetExtension(const String &filePath)
			{
				if (filePath.IsEmpty())
					return _T("");

				fs::path path = filePath.GetRawString();
				return path.extension().c_str() + 1;
			}
			String Path::GetFileNameWithoutExtension(const String &filePath)
			{
				if (filePath.IsEmpty())
					return _T("");

				fs::path path = filePath.GetRawString();
				return path.stem().c_str();
			}
			String Path::GetFullPath(const String &path)
			{
				if (path.IsEmpty())
					return _T("");

				return fs::absolute(path.GetRawString()).c_str();
			}
			std::vector<TCHAR> Path::GetInvalidFileNameChars()
			{
				static const std::vector<TCHAR> invalidChars = {
					'"', '<', '>', '|', '\0',
					'\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
					'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
					':', '*', '?', '\\', '/'};
				return invalidChars;
			}
			std::vector<TCHAR> Path::GetInvalidPathChars()
			{
				static const std::vector<TCHAR> invalidChars = {
					'|', '\0',
					'\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
					'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F'};
				return invalidChars;
			}
		}
	}
}