#pragma once
#include "HavokApi.hpp"
#include "datas/pythonex.hpp"
#include <unordered_map>

typedef std::unordered_map<JenHash, PyObject *(*)(const IhkVirtualClass *)>
    HKPYRegistry;
extern const HKPYRegistry hkClasses;

