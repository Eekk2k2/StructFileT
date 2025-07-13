#include <Windows.h>

template <typename T>
class StructFile {
public:
	StructFile(std::string path);
	StructFile(const StructFile& other) = delete;
	~StructFile();

	StructFile& operator=(const StructFile& other) = delete;

	void Flush();

	T* data;
private:
	HANDLE hFile, hFileMapping;
};

template<typename T>
inline StructFile<T>::StructFile(std::string path)
{
	std::wstring wpath = std::wstring(path.begin(), path.end());
	LPCTSTR wpathcstr = wpath.c_str();
	this->hFile = CreateFile(
		wpathcstr,
		GENERIC_READ | GENERIC_WRITE,
		0, NULL,
		OPEN_ALWAYS,
		0, 0
	);

	if (this->hFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create file handle: " << GetLastError() << std::endl;

		return;
	}

	this->hFileMapping = CreateFileMapping(
		this->hFile,
		NULL, PAGE_READWRITE, 0,
		sizeof(T), NULL
	);

	if (this->hFileMapping == nullptr) {
		std::cerr << "Failed to create file mapping: " << GetLastError() << std::endl;

		return;
	}

	this->data = reinterpret_cast<T*>(MapViewOfFile(
		this->hFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(T)
	));

	if (!this->data) {
		std::cout << "Could not create a file mapping view" << std::endl;

		return;
	}
}

template<typename T>
inline StructFile<T>::~StructFile()
{
	this->Flush();

	UnmapViewOfFile(this->data);

	if (this->hFileMapping) {
		CloseHandle(this->hFileMapping);
	}

	if (this->hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(this->hFile);
	}
}

template<typename T>
inline void StructFile<T>::Flush()
{
	FlushViewOfFile(this->data, sizeof(T));
	FlushFileBuffers(this->hFile);
}
