#pragma once

//#define VERIFYFEATURESETDLL_EXPORTS
#ifdef VERIFYFEATURESETDLL_EXPORTS
#define VERIFYFEATURESETDLL_API __declspec(dllexport)
#else
#define VERIFYFEATURESETDLL_API __declspec(dllimport)
#endif
