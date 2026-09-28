---
name: dotnetdupe-webapp-patterns
description: Best practices and patterns for building web APIs, WebSockets, and static servers using DotNetDupe WebAppCore.
---

# DotNetDupe WebAppCore Patterns Skill

## Overview
This skill guides the construction of microservices, embedded servers, and web applications using `DotNetDupe::WebAppCore`.

## Core Components
- `WebApplicationBuilder`: Service collection and container composition.
- `WebApplication`: Route mapping and HTTP pipeline dispatch.
- `ControllerBase`: Base class for REST API controllers.
- `WebAppServer`: Multi-threaded HTTP/1.1 and WebSocket server.

## Standard Controller Setup
```cpp
#include "WebAppCore/Controllers/ControllerBase.h"
#include "System/Text/Json/JsonSerializer.h"

class ItemController : public DotNetDupe::WebAppCore::Controllers::ControllerBase {
public:
    ItemController() = default;
    
    DotNetDupe::System::String GetItems() {
        return "[{\"id\":1,\"name\":\"Sample\"}]";
    }
};
```
