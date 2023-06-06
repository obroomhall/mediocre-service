#ifndef _MEDIOCRE_API_H_
#define _MEDIOCRE_API_H_

#include <libmediocre/test/test.pb.h>
#ifndef MEDIOCRE_API
#define MEDIOCRE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

using namespace mediocre::test;

MEDIOCRE_API void print_line(Something ut);

#ifdef __cplusplus
}
#endif

#endif // _MEDIOCRE_API_H_