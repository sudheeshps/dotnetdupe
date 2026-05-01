### interface `IDisposable`

Provides a mechanism for releasing unmanaged resources.

#### Methods

##### `virtual ~IDisposable() = default`

Destructor for the `IDisposable` interface.

**Usage:**
```cpp
IDisposable* disposable = getSomeResource();
delete disposable;
```

##### `virtual void Dispose() = 0`

Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.

**Usage:**
```cpp
disposable->Dispose();
```
