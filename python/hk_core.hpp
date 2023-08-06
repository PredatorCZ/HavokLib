#pragma once
#include "python/pythonex.hpp"
#include "spike/crypto/jenkinshash.hpp"
#include <map>

struct IhkVirtualClass;

typedef std::map<JenHash, PyObject *(*)(const IhkVirtualClass *)> HKPYRegistry;
extern const HKPYRegistry hkClasses;
