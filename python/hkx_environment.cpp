#include "hkx_environment.hpp"

static PyMappingMethods hkxEnvironmentPyMappingMethods[] = {
    (lenfunc)hkxEnvironmentPy::Len,
    (binaryfunc)hkxEnvironmentPy::Subscript,
    0,
};

static PyTypeObject hkxEnvironmentPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)               /* init macro */
    "hkxEnvironment",                            /* tp_name */
    sizeof(hkxEnvironmentPy),                    /* tp_basicsize */
    0,                                           /* tp_itemsize */
    0,                                           /* tp_dealloc */
    0,                                           /* tp_print */
    0,                                           /* tp_getattr */
    0,                                           /* tp_setattr */
    0,                                           /* tp_compare */
    0,                                           /* tp_repr */
    0,                                           /* tp_as_number */
    0,                                           /* tp_as_sequence */
    hkxEnvironmentPyMappingMethods,              /* tp_as_mapping */
    0,                                           /* tp_hash */
    0,                                           /* tp_call */
    0,                                           /* tp_str */
    0,                                           /* tp_getattro */
    0,                                           /* tp_setattro */
    0,                                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT, /* tp_flags */
    "Havok Environment",                         /* tp_doc */
    0,                                           /* tp_traverse */
    0,                                           /* tp_clear */
    0,                                           /* tp_richcompare */
    0,                                           /* tp_weaklistoffset */
    (getiterfunc)hkxEnvironmentPy::Iter,         /* tp_iter */
    (iternextfunc)hkxEnvironmentPy::IterNext,    /* tp_iternext */
    0,                                           /* tp_methods */
    0,                                           /* tp_members */
    0,                                           /* tp_getset */
    0,                                           /* tp_base */
    0,                                           /* tp_dict */
    0,                                           /* tp_descr_get */
    0,                                           /* tp_descr_set */
    0,                                           /* tp_dictoffset */
    0,                                           /* tp_init */
    0,                                           /* tp_alloc */
    0,                                           /* tp_new */
};

void hkxEnvironmentPy::InitType(PyObject *module) {
  if (PyType_Ready(&hkxEnvironmentPyType) < 0)
    return;
  Py_INCREF(&hkxEnvironmentPyType);
  PyModule_AddObject(module, hkxEnvironmentPyType.tp_name,
                     (PyObject *)&hkxEnvironmentPyType);
}

PyObject *hkxEnvironmentPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkxEnvironmentPy *>(
      PyType_GenericAlloc(&hkxEnvironmentPyType, 0));
  cted->hkClass = dynamic_cast<const hkxEnvironment *>(cls);

  return reinterpret_cast<PyObject *>(cted);
}

size_t hkxEnvironmentPy::Len(hkxEnvironmentPy *self) {
  return self->hkClass->Size();
}

PyObject *hkxEnvironmentPy::Subscript(hkxEnvironmentPy *self, PyObject *index) {
  const auto id = PyInt_AsSsize_t(index);
  return self->SubscriptRaw(id);
}

PyObject *hkxEnvironmentPy::SubscriptRaw(size_t index) {
  auto cVar = hkClass->At(index);
  auto rtVal = PyDict_New();
  PyDict_SetItem(
      rtVal, PyString_FromStringAndSize(cVar.name.data(), cVar.name.size()),
      PyString_FromStringAndSize(cVar.value.data(), cVar.value.size()));

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