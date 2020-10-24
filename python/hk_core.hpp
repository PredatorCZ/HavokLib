#pragma once
#include "datas/pythonex.hpp"
#include "datas/jenkinshash.hpp"
#include <unordered_map>

struct IhkVirtualClass;

typedef std::unordered_map<JenHash, PyObject *(*)(const IhkVirtualClass *)>
    HKPYRegistry;
extern const HKPYRegistry hkClasses;
