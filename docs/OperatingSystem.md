### class `OperatingSystem`

Represents information about an operating system, such as the version and platform identifier.

#### Methods

##### `OperatingSystem(PlatformID platform, const Version& version)`

Initializes a new instance of the `OperatingSystem` class, using the specified platform identifier and version object.

**Parameters:**
- `platform`: One of the `PlatformID` values that identifies the operating system platform.
- `version`: A `Version` object that identifies the operating system version.

**Usage:**
```cpp
OperatingSystem os(PlatformID::Win32NT, Version(10, 0));
```

##### `PlatformID GetPlatform() const`

Gets a `PlatformID` enumeration value that identifies the operating system platform.

**Returns:**
- One of the `PlatformID` values.

**Usage:**
```cpp
PlatformID id = os.GetPlatform();
```

##### `const Version& GetVersion() const`

Gets a `Version` object that identifies the operating system.

**Returns:**
- A `Version` object.

**Usage:**
```cpp
const Version& v = os.GetVersion();
```

##### `String GetVersionString() const`

Gets the concatenated string representation of the platform identifier, version, and service pack of the current operating system.

**Returns:**
- The string representation.

**Usage:**
```cpp
String s = os.GetVersionString();
```

##### `String GetServicePack() const`

Gets the service pack version of the operating system.

**Returns:**
- The service pack version, if one is available.

**Usage:**
```cpp
String sp = os.GetServicePack();
```

##### `static bool IsWindows()`

Indicates whether the current operating system is a Windows operating system.

**Returns:**
- `true` if the platform is Windows; otherwise, `false`.

**Usage:**
```cpp
if (OperatingSystem::IsWindows()) { /* ... */ }
```

##### `static bool IsLinux()`

Indicates whether the current operating system is a Linux operating system.

**Returns:**
- `true` if the platform is Linux; otherwise, `false`.

**Usage:**
```cpp
if (OperatingSystem::IsLinux()) { /* ... */ }
```

##### `static bool IsMacOS()`

Indicates whether the current operating system is a macOS operating system.

**Returns:**
- `true` if the platform is macOS; otherwise, `false`.

**Usage:**
```cpp
if (OperatingSystem::IsMacOS()) { /* ... */ }
```
