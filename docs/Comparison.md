# STL vs DotNetDupe Comparison ⚖️

This document provides a side-by-side comparison of common development tasks using the C++ Standard Template Library (STL) versus the **DotNetDupe** library. DotNetDupe is designed to be intuitive, discoverable, concise, and developer-friendly.

---

## 1. String Manipulation 🧵

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Lowercase** | `std::transform(s.begin(), s.end(), s.begin(), ::tolower);` | `s = s.ToLower();` |
| **Substring** | `s.substr(0, 5);` | `s.Substring(0, 5);` |
| **Split** | Manual `std::stringstream` / `find` loop | `Array<String> parts = s.Split(',');` |
| **Join** | Manual loop with delimiter check | `String s = String::Join(", ", list);` |
| **Starts / Ends With** | `s.rfind(prefix, 0) == 0;` | `s.StartsWith(prefix);` |
| **Trim** | Manual `erase` and `find_first_not_of` | `s = s.Trim();` |

### Example

**STL:**
```cpp
#include <string>
#include <algorithm>
#include <vector>

std::string s = "  Hello World, C++  ";
// Trim & Lowercase
s.erase(0, s.find_first_not_of(" \t"));
s.erase(s.find_last_not_of(" \t") + 1);
std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
```

**DotNetDupe:**
```cpp
#include "System/String.h"
using namespace DotNetDupe::System;

String s = "  Hello World, C++  ";
String cleaned = s.Trim().ToLower();
Array<String> parts = cleaned.Split(',');
```

---

## 2. Lists & Search 🔍

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <vector>` | `#include "System/Collections/Generic/List.h"` |
| **Contains Check** | `std::find(v.begin(), v.end(), 3) != v.end();` | `list.Contains(3);` |
| **Add / Append** | `v.push_back(42);` | `list.Add(42);` |
| **Element Count** | `v.size();` | `list.GetCount();` |

### Example

**STL:**
```cpp
#include <vector>
#include <algorithm>

std::vector<int> v = { 1, 2, 3, 4, 5 };
bool exists = std::find(v.begin(), v.end(), 3) != v.end();
```

**DotNetDupe:**
```cpp
#include "System/Collections/Generic/List.h"
using namespace DotNetDupe::System::Collections::Generic;

List<int> list = { 1, 2, 3, 4, 5 };
bool exists = list.Contains(3);
```

---

## 3. Dictionary (Hash Map) 📖

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Header** | `#include <unordered_map>` | `#include "System/Collections/Generic/Dictionary.h"` |
| **Safe Lookup** | `auto it = m.find(k); if (it != m.end()) ...` | `if (dict.TryGetValue(k, outVal)) ...` |
| **Key Existence** | `m.count(k) > 0` | `dict.ContainsKey(k)` |

### Example

**STL:**
```cpp
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> m;
m["apple"] = 5;

auto it = m.find("apple");
if (it != m.end()) {
    int val = it->second;
}
```

**DotNetDupe:**
```cpp
#include "System/Collections/Generic/Dictionary.h"
#include "System/String.h"
using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;

Dictionary<String, int> dict;
dict.Add("apple", 5);

int val = 0;
if (dict.TryGetValue("apple", val)) {
    // val == 5
}
```

---

## 4. Path Handling 📂

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Combine Paths** | `p /= "sub";` | `Path::Combine("dir", "file.txt");` |
| **Change Extension** | `p.replace_extension(".bak");` | `Path::ChangeExtension(p, ".bak");` |
| **Get Directory** | `p.parent_path().string();` | `Path::GetDirectoryName(p);` |

### Example

**STL:**
```cpp
#include <filesystem>
std::filesystem::path p = "C:\\Projects";
p /= "app.log";
p.replace_extension(".bak");
std::string pathStr = p.string();
```

**DotNetDupe:**
```cpp
#include "System/IO/Path.h"
using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

String p = Path::Combine("C:\\Projects", "app.log");
String backup = Path::ChangeExtension(p, ".bak");
```

---

## 5. Console & Colored Output 🖥️

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Colored Output** | Manual ANSI escape codes `"\033[1;32m"` | `Console::SetForegroundColor(ConsoleColor::Green);` |
| **Window Title** | Platform-specific OS API calls | `Console::SetTitle("Application Name");` |
| **Screen Clear** | `std::system("cls")` / platform calls | `Console::Clear();` |

### Example

**STL:**
```cpp
#include <iostream>
// ANSI escape codes (Windows console support varies)
std::cout << "\033[1;32mOperation Succeeded!\033[0m" << std::endl;
```

**DotNetDupe:**
```cpp
#include "System/Console.h"
using namespace DotNetDupe::System;

Console::SetForegroundColor(ConsoleColor::Green);
Console::WriteLine("Operation Succeeded!");
Console::ResetColor();
```

---

## 6. Buffer Operations 📦

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Block Copy** | `std::memcpy(dest, src, count);` | `Buffer::BlockCopy(src, 0, dest, 0, count);` |
| **Byte Length** | `sizeof(arr)` / `size * sizeof(T)` | `Buffer::ByteLength(arr);` |

### Example

**DotNetDupe:**
```cpp
#include "System/Buffer.h"
#include "System/Array.h"
using namespace DotNetDupe::System;

Array<int> src = { 10, 20, 30, 40 };
Array<int> dest(4);
Buffer::BlockCopy(src, 0, dest, 0, src.GetLength() * sizeof(int));
```

---

## 7. Version Comparison 🔢

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Version Class** | No standard class (manual parsing required) | `Version v("1.2.3.4");` |
| **Comparison** | Custom comparison operators | `if (v1 < v2) ...` |

### Example

**DotNetDupe:**
```cpp
#include "System/Version.h"
using namespace DotNetDupe::System;

Version current("1.2.0.0");
Version required("1.3.0.0");

if (current < required) {
    // Prompt update
}
```

---

## 8. File I/O & Streams 💾

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **File Read All Text** | `std::ifstream` with `stringstream` | `File::ReadAllText("config.json");` |
| **File Write All Text** | `std::ofstream << text;` | `File::WriteAllText("log.txt", text);` |
| **File Stream** | `std::fstream` | `FileStream fs("data.bin", FileMode::Open);` |

### Example

**STL:**
```cpp
#include <fstream>
#include <sstream>
#include <string>

std::ifstream file("config.json");
std::stringstream buffer;
buffer << file.rdbuf();
std::string json = buffer.str();
```

**DotNetDupe:**
```cpp
#include "System/IO/File.h"
using namespace DotNetDupe::System;
using namespace DotNetDupe::System::IO;

String json = File::ReadAllText("config.json");
```

---

## 9. String Building 🏗️

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Class** | `std::stringstream` | `StringBuilder` |
| **Method Chaining** | `ss << a << b;` | `sb.Append(a).Append(b).AppendLine();` |

### Example

**DotNetDupe:**
```cpp
#include "System/Text/StringBuilder.h"
using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Text;

StringBuilder sb;
sb.Append("Server: ").Append("127.0.0.1").AppendLine();
sb.Append("Port: ").Append(8080);
String config = sb.ToString();
```

---

## 10. High-Resolution Timing ⏱️

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Timer** | `std::chrono::high_resolution_clock` | `Stopwatch` |
| **Elapsed Milliseconds** | `duration_cast<milliseconds>(end - start).count()` | `sw.ElapsedMilliseconds()` |

### Example

**STL:**
```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
// perform task
auto end = std::chrono::high_resolution_clock::now();
auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
```

**DotNetDupe:**
```cpp
#include "System/Diagnostics/Stopwatch.h"
using namespace DotNetDupe::System::Diagnostics;

Stopwatch sw = Stopwatch::StartNew();
// perform task
long long ms = sw.ElapsedMilliseconds();
```

---

## 11. Time Intervals (TimeSpan) ⏳

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Creation** | `std::chrono::hours(1) + std::chrono::minutes(30)` | `TimeSpan::FromHours(1) + TimeSpan::FromMinutes(30)` |
| **Formatting** | Cumbersome formatting in C++17 | `ts.ToString()` |

### Example

**DotNetDupe:**
```cpp
#include "System/TimeSpan.h"
using namespace DotNetDupe::System;

TimeSpan timeout = TimeSpan::FromSeconds(30);
TimeSpan total = TimeSpan::FromHours(1) + TimeSpan::FromMinutes(45);
```

---

## 12. Random Numbers 🎲

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Setup** | `std::random_device` + `std::mt19937` + `std::uniform_int_distribution` | `Random rnd;` |
| **Next Integer** | `dist(gen);` | `rnd.Next(1, 101);` |

### Example

**STL:**
```cpp
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(1, 100);
int value = dis(gen);
```

**DotNetDupe:**
```cpp
#include "System/Random.h"
using namespace DotNetDupe::System;

Random rnd;
int value = rnd.Next(1, 101);
```

---

## 13. Smart Pointers 🧠

| Feature | Standard C++ (STL) | DotNetDupe |
| :--- | :--- | :--- |
| **Unique Ownership** | `std::unique_ptr<T>` | `SmartPointer<T>` |
| **Shared Ownership** | `std::shared_ptr<T>` | `SmartPointer<T>::NewShared(...)` |

### Example

**DotNetDupe:**
```cpp
#include "System/SmartPointer.h"
using namespace DotNetDupe::System;

// Unique ownership (moves only)
SmartPointer<MyClass> pUnique = SmartPointer<MyClass>::New(arg1);

// Shared ownership (reference counted)
SmartPointer<MyClass> pShared = SmartPointer<MyClass>::NewShared(arg1);
SmartPointer<MyClass> pCopy = pShared; // Shared reference count incremented
```

---

## Summary

DotNetDupe eliminates boilerplate, reduces accidental errors, and unifies C++ development under intuitive .NET BCL patterns.
