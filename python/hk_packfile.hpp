
#pragma once
#include "HavokApi.hpp"
#include <Python.h>

struct IHavokPy {
  PyObject_HEAD;
  std::unique_ptr<IhkPackFile> file;

  static void Dealloc(IHavokPy *self);
  static PyObject *New(PyTypeObject *type, PyObject *args, PyObject *);

  static void InitType(PyObject *module);
  static PyTypeObject *GetType();

  static PyObject *GetVersion(IHavokPy *self);
  static PyObject *GetClasses(IHavokPy *self, PyObject *args);
  static PyObject *ToXML(IHavokPy *self, PyObject *args);
};
