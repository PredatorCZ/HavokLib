#include "hk_rootlevelcontainer.hpp"
#include <structmember.h>
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

static PyMemberDef hkNamedVariantPyMembers[] = {
    {"name", T_OBJECT_EX, offsetof(hkNamedVariantPy, name), 0, "Variant Name"},
    {"class_name", T_OBJECT_EX, offsetof(hkNamedVariantPy, className), 0,
     "Variant Class Name"},
    {"object", T_OBJECT_EX, offsetof(hkNamedVariantPy, object), 0,
     "Variant Object"},
    {NULL},
};

PyTypeObject *hkNamedVariantPy::GetType() {
  static PyTypeObject hkNamedVariantPyType{
      .tp_name = "hkNamedVariant",
      .tp_basicsize = sizeof(hkNamedVariantPy),
      .tp_flags = Py_TPFLAGS_DEFAULT,
      .tp_doc = "Havok Named Variant",
      .tp_members = (PyMemberDef *)hkNamedVariantPyMembers,
      .tp_init = (initproc)hkNamedVariantPy::Init,
      .tp_new = hkNamedVariantPy::New,
  };

  return &hkNamedVariantPyType;
}

PyObject *hkNamedVariantPy::New(PyTypeObject *type, PyObject *, PyObject *) {
  hkNamedVariantPy *self = (hkNamedVariantPy *)type->tp_alloc(type, 0);

  if (self) {
    self->name = PyUnicode_FromString("");
    if (!self->name) {
      Py_DECREF(self);
      return nullptr;
    }

    self->className = PyUnicode_FromString("");
    if (!self->className) {
      Py_DECREF(self);
      return nullptr;
    }

    self->object = Py_None;
    Py_INCREF(Py_None);
  }

  return (PyObject *)self;
}

int hkNamedVariantPy::Init(hkNamedVariantPy *self, PyObject *args,
                           PyObject *kwds) {
  static char *kwList[] = {"name", "class_name", "object", nullptr};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|UUO", kwList, &self->name,
                                   &self->className, &self->object)) {
    return -1;
  }

  return 0;
}

static PyMappingMethods hkRootLevelContainerPyMappingMethods{
    (lenfunc)hkRootLevelContainerPy::Len,
    (binaryfunc)hkRootLevelContainerPy::Subscript,
    0,
};

PyTypeObject *hkRootLevelContainerPy::GetType() {
  static PyTypeObject hkRootLevelContainerPyType{
      .tp_name = "hkRootLevelContainer",
      .tp_basicsize = sizeof(hkRootLevelContainerPy),
      .tp_as_mapping = &hkRootLevelContainerPyMappingMethods,
      .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
      .tp_doc = "Havok Root Level Container",
      .tp_iter = (getiterfunc)hkRootLevelContainerPy::Iter,
      .tp_iternext = (iternextfunc)hkRootLevelContainerPy::IterNext,
  };

  return &hkRootLevelContainerPyType;
}

PyObject *hkRootLevelContainerPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkRootLevelContainerPy *>(
      PyType_GenericAlloc(GetType(), 0));
  cted->hkClass = checked_deref_cast<const hkRootLevelContainer>(cls);

  return reinterpret_cast<PyObject *>(cted);
}

size_t hkRootLevelContainerPy::Len(hkRootLevelContainerPy *self) {
  return self->hkClass->Size();
}

PyObject *hkRootLevelContainerPy::Subscript(hkRootLevelContainerPy *self,
                                            PyObject *index) {
  const auto id = PyLong_AsSsize_t(index);
  return self->SubscriptRaw(id);
}

PyObject *hkRootLevelContainerPy::SubscriptRaw(size_t index) {
  auto cVar = hkClass->At(index);
  hkNamedVariantPy *hkVar = reinterpret_cast<hkNamedVariantPy *>(
      PyType_GenericAlloc(hkNamedVariantPy::GetType(), 0));
  hkVar->name = PyUnicode_FromStringAndSize(cVar.name.data(), cVar.name.size());
  hkVar->className =
      PyUnicode_FromStringAndSize(cVar.className.data(), cVar.className.size());

  JenHash classHash(cVar.className);

  auto foundCt = hkClasses.find(classHash);

  if (foundCt != hkClasses.end()) {
    hkVar->object = foundCt->second(cVar.pointer);
  }

  return reinterpret_cast<PyObject *>(hkVar);
}

PyObject *hkRootLevelContainerPy::Iter(hkRootLevelContainerPy *self) {
  self->iterPos = 0;
  return reinterpret_cast<PyObject *>(self);
}

PyObject *hkRootLevelContainerPy::IterNext(hkRootLevelContainerPy *self) {
  if (self->iterPos < self->Len(self)) {
    return self->SubscriptRaw(self->iterPos++);
  } else {
    PyErr_SetNone(PyExc_StopIteration);
    return nullptr;
  }
}

void hkRootLevelContainerPy::InitType(PyObject *module) {
  PyAddTypes<hkRootLevelContainerPy, hkNamedVariantPy>(module);
}
