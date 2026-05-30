# STL vs DotNetDupe Comparison ⚖️

This document provides a side-by-side comparison of common development tasks using the C++ Standard Template Library (STL) versus the **DotNetDupe** library. DotNetDupe is designed to be more intuitive, concise, and developer-friendly.

---

### 1. String Manipulation 🧵
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| **Lowercase & Substring**<br>`std::string s = "Hello World";`<br>`std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });`<br>`std::string sub = s.substr(0, 5);` | **Lowercase & Substring**<br>`String s = "Hello World";`<br>`String sub = s.ToLower().Substring(0, 5);` |
| **Split into List**<br>`std::vector<std::string> parts;`<br>`size_t pos = 0; while ((pos = s.find(',')) != npos) {`<br>`  parts.push_back(s.substr(0, pos)); s.erase(0, pos + 1); }`<br>`parts.push_back(s);` | **Split into List**<br>`Array<String> parts = s.Split(',');` |
| **Join List**<br>`std::string joined;`<br>`for (size_t i = 0; i < v.size(); ++i) {`<br>`  joined += v[i]; if (i < v.size() - 1) joined += ", "; }` | **Join List**<br>`String joined = String::Join(", ", list);` |
| **Starts / Ends With**<br>`bool b = (s.size() >= prefix.size() && `<br>`  s.compare(0, prefix.size(), prefix) == 0);` | **Starts / Ends With**<br>`bool b = s.StartsWith(prefix);` |
| **Trim Whitespace**<br>`s.erase(0, s.find_first_not_of(' '));`<br>`s.erase(s.find_last_not_of(' ') + 1);` | **Trim Whitespace**<br>`String trimmed = s.Trim();` |

### 2. Lists & Search 🔍
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `std::vector<int> v = {1, 2, 3, 4, 5};`<br>`bool exists = std::find(v.begin(), v.end(), 3) != v.end();` | `List<int> list = {1, 2, 3, 4, 5};`<br>`bool exists = list.Contains(3);` |

### 3. Dictionary (Hash Map) 📖
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `std::unordered_map<std::string, int> m;`<br>`auto it = m.find("key");`<br>`if (it != m.end()) { int val = it->second; }` | `Dictionary<String, int> dict;`<br>`int val;`<br>`if (dict.TryGetValue("key", val)) { ... }` |

### 4. Path Handling 📂
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `std::filesystem::path p = "dir";`<br>`p /= "file.txt";`<br>`p.replace_extension(".bak");`<br>`std::string s = p.string();` | `String p = Path::Combine({"dir", "file.txt"});`<br>`p = Path::ChangeExtension(p, ".bak");` |

### 5. Console & I/O 🖥️
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `// Manual color escape codes`<br>`std::cout << "\033[1;32mGreen\033[0m" << std::endl;`<br>`// No standard title or clear API` | `Console::SetForegroundColor(ConsoleColor::Green);`<br>`Console::WriteLine("Green");`<br>`Console::SetTitle("My App");`<br>`Console::Clear();` |

### 6. Buffer Management 📦
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `std::memcpy(dest, src, count * sizeof(T));`<br>`// or`<br>`std::copy(src, src + count, dest);` | `Buffer::BlockCopy(src, 0, dest, 0, count * sizeof(T));` |

### 7. Version Handling 🔢
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `// Manual parsing and comparison logic`<br>`struct Ver { int major, minor; ... };`<br>`bool operator<(const Ver& a, const Ver& b) { ... }` | `Version v1("1.2.3.4");`<br>`Version v2(1, 2, 4, 0);`<br>`if (v1 < v2) { ... }` |

### 8. File I/O (FileStream) 💾
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `std::fstream fs("data.bin", std::ios::in \| std::ios::out \| std::ios::binary);`<br>`fs.seekp(10, std::ios::beg);`<br>`fs.write(buffer, size);` | `FileStream fs("data.bin", FileMode::Open, FileAccess::ReadWrite);`<br>`fs.Seek(10, SeekOrigin::Begin);`<br>`fs.Write(buffer, 0, size);` |

### 9. Efficient String Building 🏗️
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `std::stringstream ss;`<br>`ss << "Items: " << 10 << ", Price: " << 19.99;`<br>`std::string s = ss.str();` | `StringBuilder sb;`<br>`sb.Append("Items: ").Append(10).Append(", Price: ").Append(19.99);`<br>`String s = sb.ToString();` |

### 10. High-Resolution Timing ⏱️
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `auto start = std::chrono::high_resolution_clock::now();`<br>`// ... op ...`<br>`auto end = std::chrono::high_resolution_clock::now();`<br>`auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();` | `Stopwatch sw = Stopwatch::StartNew();`<br>`// ... op ...`<br>`long long ms = sw.ElapsedMilliseconds();` |

### 11. Time Intervals (TimeSpan) ⏳
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `auto d1 = std::chrono::hours(1);`<br>`auto d2 = std::chrono::minutes(30);`<br>`auto total = d1 + d2;` | `TimeSpan ts = TimeSpan::FromHours(1) + TimeSpan::FromMinutes(30);` |

### 12. Random Number Generation 🎲
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `std::random_device rd; std::mt19937 gen(rd());`<br>`std::uniform_int_distribution<> dis(1, 100);`<br>`int val = dis(gen);` | `Random rnd;`<br>`int val = rnd.Next(1, 101);` |

### 13. URI Building 🌐
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| *(No standard equivalent; requires manual parsing)* | `UriBuilder ub("https", "github.com");`<br>`ub.SetPath("/google/gemini");`<br>`String url = ub.ToString();` |

### 14. Type Conversion 🔄
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `try { int i = std::stoi("12345"); }`<br>`catch (const std::invalid_argument& e) { ... }` | `try { int i = Convert::ToInt32("12345"); }`<br>`catch (const FormatException& e) { ... }` |

### 15. Environment & OS 💻
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `char* p = std::getenv("PATH");`<br>`// No standard OS version/platform API` | `String p = Environment::GetEnvironmentVariable("PATH");`<br>`OperatingSystem os = Environment::GetOperatingSystem();` |

### 16. GUID Generation 🆔
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| *(No standard equivalent)* | `Guid id = Guid::NewGuid();`<br>`String s = id.ToString();` |

### 17. Text Encoding (UTF-8) 🔠
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| `// Cumbersome (deprecated std::codecvt or WinAPI)`<br>`int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, ...);`<br>`std::string utf8(size, '\0');`<br>`WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &utf8[0], size, ...);` | `auto encoding = TextEncoding::GetUTF8();`<br>`auto bytes = encoding->GetBytes(str);`<br>`String s = encoding->GetString(bytes);` |

### 18. Smart Pointers 🧠
| Standard Template Library (STL) | DotNetDupe |
| :--- | :--- |
| **Unique Ownership**<br>`std::unique_ptr<int> p = std::make_unique<int>(10);`<br>`auto p2 = std::move(p);`<br><br>**Shared Ownership**<br>`std::shared_ptr<int> p = std::make_shared<int>(10);`<br>`auto p2 = p; // Increments ref count` | **Unified SmartPointer**<br>`// Unique by default`<br>`SmartPointer<int> p(new int(10));`<br>`auto p2 = std::move(p);`<br><br>`// Shared via flag`<br>`SmartPointer<int> p(new int(10), true);`<br>`auto p2 = p; // Increments ref count` |

---

## Conclusion

**DotNetDupe** provides a consistent, discoverable, and object-oriented API that hides the complexity of underlying system calls and the often verbose syntax of the C++ Standard Library.

