### class `TimeProvider`

Provides an abstraction for time.

#### Methods

##### `virtual DateTimeOffset GetUtcNow() const = 0`

Gets a `DateTimeOffset` value that is set to the current Coordinated Universal Time (UTC) date and time.

**Usage:**
```cpp
auto provider = TimeProvider::GetSystem();
DateTimeOffset now = provider->GetUtcNow();
```

##### `virtual DateTimeOffset GetLocalNow() const = 0`

Gets a `DateTimeOffset` value that is set to the current local date and time.

**Usage:**
```cpp
DateTimeOffset local = provider->GetLocalNow();
```

##### `virtual int64_t GetTimestamp() const = 0`

Gets the current high-resolution timestamp.

**Usage:**
```cpp
int64_t start = provider->GetTimestamp();
```

##### `virtual int64_t GetTimestampFrequency() const = 0`

Gets the frequency of the timestamp counter.

**Usage:**
```cpp
int64_t freq = provider->GetTimestampFrequency();
```

##### `TimeSpan GetElapsedTime(int64_t startingTimestamp) const`

Gets the elapsed time since the specified starting timestamp.

**Usage:**
```cpp
int64_t start = provider->GetTimestamp();
// ...
TimeSpan elapsed = provider->GetElapsedTime(start);
```

##### `static std::shared_ptr<TimeProvider> GetSystem()`

Gets the system's default `TimeProvider` instance.

**Usage:**
```cpp
auto provider = TimeProvider::GetSystem();
```

