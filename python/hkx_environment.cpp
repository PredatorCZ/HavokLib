#include "hkx_environment.hpp"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

static PyMappingMethods hkxEnvironmentPyMappingMethods{
    (lenfunc)hkxEnvironmentPy::Len,
    (binaryfunc)hkxEnvironmentPy::Subscript,
    0,
};

PyTypeObject *hkxEnvironmentPy::GetType() {
  static PyTypeObject hkxEnvironmentPyType = {
      .tp_name = "hkxEnvironment",
      .tp_basicsize = sizeof(hkxEnvironmentPy),
      .tp_as_mapping = &hkxEnvironmentPyMappingMethods,
      .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
      .tp_doc = "Havok Environment",
      .tp_iter = (getiterfunc)hkxEnvironmentPy::Iter,
      .tp_iternext = (iternextfunc)hkxEnvironmentPy::IterNext,
  };

  return &hkxEnvironmentPyType;
}

void hkxEnvironmentPy::InitType(PyObject *module) {
  PyAddType<hkxEnvironmentPy>(module);
}

PyObject *hkxEnvironmentPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkxEnvironmentPy *>(
      PyType_GenericAlloc(hkxEnvironmentPy::GetType(), 0));
  cted->hkClass = checked_deref_cast<const hkxEnvironment>(cls);

  return reinterpret_cast<PyObject *>(cted);
}

size_t hkxEnvironmentPy::Len(hkxEnvironmentPy *self) {
  return self->hkClass->Size();
}

PyObject *hkxEnvironmentPy::Subscript(hkxEnvironmentPy *self, PyObject *index) {
  const auto id = PyLong_AsSize_t(index);
  return self->SubscriptRaw(id);
}

PyObject *hkxEnvironmentPy::SubscriptRaw(size_t index) {
  auto cVar = hkClass->At(index);
  auto rtVal = PyDict_New();
  PyDict_SetItem(
      rtVal, PyUnicode_FromStringAndSize(cVar.name.data(), cVar.name.size()),
      PyUnicode_FromStringAndSize(cVar.value.data(), cVar.value.size()));

  return rtVal;
}

PyObject *hkxEnvironmentPy::Iter(hkxEnvironmentPy *self) {
  self->iterPos = 0;
  return reinterpret_cast<PyObject *>(self);
}

PyObject *hkxEnvironmentPy::IterNext(hkxEnvironmentPy *self) {
  if (self->iterPos < self->Len(self)) {
    return self->SubscriptRaw(self->iterPos++);
  } else {
    PyErr_SetNone(PyExc_StopIteration);
    return nullptr;
  }
}
