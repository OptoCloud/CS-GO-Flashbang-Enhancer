#ifndef UTILS_H
#define UTILS_H

// Function to access a function in a VMT
template <typename Fn>
constexpr Fn GetVirtualFunction(void* pClassBase, int nFunctionIndex)
{
    return (Fn)((PDWORD) * (PDWORD*)pClassBase)[nFunctionIndex];
}

#endif // UTILS_H
