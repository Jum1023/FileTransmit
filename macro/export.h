#ifndef _EXPORT_H_
#define _EXPORT_H_


#if defined(_WIN32)
# define SHAREDOBJECT_EXPORT __declspec(dllexport)
#else
# define SHAREOBJECT_EXPORT
#endif

#endif