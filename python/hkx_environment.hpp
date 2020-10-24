#pragma once
#include "hk_core.hpp"
#include "hklib/hkx_environment.hpp"

struct hkxEnvironmentPy {
  PyObject_HEAD;
  typedef const hkxEnvironment value_type;
  value_type *hkClass;
  size_t iterPos;

  static void InitType(PyObject *module);
  static PyObject *Alloc(const IhkVirtualClass *cls);

  static size_t Len(hkxEnvironmentPy *self);
  static PyObject *Subscript(hkxEnvironmentPy *self, PyObject *index);
  PyObject *SubscriptRaw(size_t index);
  static PyObject *Iter(hkxEnvironmentPy *self);
  static PyObject *IterNext(hkxEnvironmentPy *self);
};
