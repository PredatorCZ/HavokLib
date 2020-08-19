#pragma once
#include "hk_core.hpp"

struct hkNamedVariantPy {
  PyObject_HEAD;

  PyObject *name;
  PyObject *className;
  PyObject *object;

  static PyObject *New(PyTypeObject *type, PyObject *args, PyObject *kwds);
  static int Init(hkNamedVariantPy *self, PyObject *args, PyObject *kwds);
  static void Dealloc(hkNamedVariantPy *self);

  static PyTypeObject *GetType();
};

struct hkRootLevelContainerPy {
  PyObject_HEAD;
  typedef const hkRootLevelContainer value_type;
  value_type *hkClass;
  size_t iterPos;

  static void InitType(PyObject *module);
  static PyObject *Alloc(const IhkVirtualClass *cls);

  static size_t Len(hkRootLevelContainerPy *self);
  static PyObject *Subscript(hkRootLevelContainerPy *self, PyObject *index);
  PyObject *SubscriptRaw(size_t index);
  static PyObject *Iter(hkRootLevelContainerPy *self);
  static PyObject *IterNext(hkRootLevelContainerPy *self);

  static PyTypeObject *GetType();
};