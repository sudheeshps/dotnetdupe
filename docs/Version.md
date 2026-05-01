### class `Version`

Represents the version number of an assembly, operating system, or the common language runtime.

#### Methods

##### `Version(int major, int minor, int build, int revision)`

Initializes a new instance of the `Version` class with the specified major, minor, build, and revision numbers.

**Parameters:**
- `major`: The major version number.
- `minor`: The minor version number.
- `build`: The build number.
- `revision`: The revision number.

**Usage:**
```cpp
Version v(1, 2, 3, 4);
```

##### `Version(int major, int minor, int build)`

Initializes a new instance of the `Version` class with the specified major, minor, and build numbers.

**Parameters:**
- `major`: The major version number.
- `minor`: The minor version number.
- `build`: The build number.

**Usage:**
```cpp
Version v(1, 2, 3);
```

##### `Version(int major, int minor)`

Initializes a new instance of the `Version` class with the specified major and minor numbers.

**Parameters:**
- `major`: The major version number.
- `minor`: The minor version number.

**Usage:**
```cpp
Version v(1, 2);
```

##### `Version()`

Initializes a new instance of the `Version` class.

**Usage:**
```cpp
Version v;
```

##### `int GetMajor() const`

Gets the value of the major component of the version number.

**Returns:**
- The major version number.

**Usage:**
```cpp
int major = v.GetMajor();
```

##### `int GetMinor() const`

Gets the value of the minor component of the version number.

**Returns:**
- The minor version number.

**Usage:**
```cpp
int minor = v.GetMinor();
```

##### `int GetBuild() const`

Gets the value of the build component of the version number.

**Returns:**
- The build number, or -1 if the build number is undefined.

**Usage:**
```cpp
int build = v.GetBuild();
```

##### `int GetRevision() const`

Gets the value of the revision component of the version number.

**Returns:**
- The revision number, or -1 if the revision number is undefined.

**Usage:**
```cpp
int revision = v.GetRevision();
```

##### `String ToString() const`

Converts the value of the current `Version` object to its equivalent `String` representation.

**Returns:**
- The `String` representation of the values of the major, minor, build, and revision components.

**Usage:**
```cpp
String s = v.ToString(); // "1.2.3.4"
```

##### `bool operator==(const Version& other) const`

Determines whether two specified `Version` objects are equal.

**Parameters:**
- `other`: The `Version` object to compare with the current object.

**Returns:**
- `true` if every component of the current `Version` object matches the corresponding component of the `other` parameter; otherwise, `false`.

**Usage:**
```cpp
Version v1(1, 0), v2(1, 0);
if (v1 == v2) { /* ... */ }
```
