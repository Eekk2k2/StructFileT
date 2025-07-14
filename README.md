```cpp
// Define a struct containing only trivially copyable fields
struct MyData { int x; float y; char name[32]; };

// Then create a new StructFile<T> with a filename
StructFile<MyData> mappedFile("data.struct");

// Writing is the same as modifying the struct
mappedFile.data->x = 42;
mappedFile.data->y = 38;

mappedFile.Flush(); // Ensures data is written

// Same with reading
char[32] name;
memcpy(&name, mappedFile.data->name, 32);
```
