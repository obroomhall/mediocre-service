#ifndef _MEDIOCRE_API_H_
#define _MEDIOCRE_API_H_

#include <libmediocre/api/api.pb.h>
#ifndef MEDIOCRE_API
#define MEDIOCRE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

using namespace mediocre::api;

MEDIOCRE_API void print_line(UniqueThing ut);

#ifdef __cplusplus
}
#endif

#endif // _MEDIOCRE_API_H_