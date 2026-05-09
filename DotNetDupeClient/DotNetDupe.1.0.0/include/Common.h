#pragma once
#ifdef DOTNETDUPE_EXPORTS
#define DOTNETDUPE_API __declspec(dllexport)
#else
#define DOTNETDUPE_API __declspec (dllimport)
#endif