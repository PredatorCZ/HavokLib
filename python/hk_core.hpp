#pragma once
#include "datas/pythonex.hpp"
#include "datas/jenkinshash.hpp"
#include <map>

struct IhkVirtualClass;

typedef std::map<JenHash, PyObject *(*)(const IhkVirtualClass *)>
    HKPYRegistry;
extern const HKPYRegistry hkClasses;
