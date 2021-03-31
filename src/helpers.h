#ifndef HELPERS_H
#define HELPERS_H

#include <cstddef>

namespace Helpers {
//template <typename Type>
//constexpr size_t PtrSize(Type ptr) {
//    return sizeof(ptr)/sizeof(*ptr);
//}

template <typename Type>
void PtrDeepCopy(Type* from, Type* to, size_t size) {
    for (unsigned index = 0; index < size; index++) {
        to[index] = from[index];
    }
}
}

#endif // HELPERS_H
