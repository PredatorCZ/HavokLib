#include "hk_rootlevelcontainer.hpp"

static PyMember hkNamedVariantPyMembers[] = {
    {"name", T_OBJECT_EX, offsetof(hkNamedVariantPy, name), 0, "Variant Name"},
    {"class_name", T_OBJECT_EX, offsetof(hkNamedVariantPy, className), 0,
     "Variant Class Name"},
    {"object", T_OBJECT_EX, offsetof(hkNamedVariantPy, object), 0,
     "Variant Object"},
    {NULL},
};

static PyTypeObject hkNamedVariantPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)          /* init macro */
    "hkNamedVariant",                       /* tp_name */
    sizeof(hkNamedVariantPy),               /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor)hkNamedVariantPy::Dealloc,  /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                     /* tp_flags */
    "Havok Named Variant",                  /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    0,                                      /* tp_methods */
    (PyMemberDef *)hkNamedVariantPyMembers, /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)hkNamedVariantPy::Init,       /* tp_init */
    0,                                      /* tp_alloc */
    hkNamedVariantPy::New,                  /* tp_new */
};

PyTypeObject *hkNamedVariantPy::GetType() { return &hkNamedVariantPyType; }

PyObject *hkNamedVariantPy::New(PyTypeObject *type, PyObject *, PyObject *) {
  hkNamedVariantPy *self = (hkNamedVariantPy *)type->tp_alloc(type, 0);

  if (self) {
    self->name = PyString_FromString("");
    if (!self->name) {
      Py_DECREF(self);
      return nullptr;
    }

    self->className = PyString_FromString("");
    if (!self->className) {
      Py_DECREF(self);
      return nullptr;
    }

    self->object = Py_None;
  }

  return (PyObject *)self;
}

int hkNamedVariantPy::Init(hkNamedVariantPy *self, PyObject *args,
                           PyObject *kwds) {
  PyObject *name = nullptr, *className = nullptr, *object = nullptr;

  static char *kwList[] = {"name", "class_name", "object", nullptr};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|SSO", kwList, &name,
                                   &className, &object)) {
    return -1;
  }

  if (name) {
    Py_XDECREF(self->name);
    Py_INCREF(name);
    self->name = name;
  }

  if (className) {
    Py_XDECREF(self->className);
    Py_INCREF(className);
    self->className = className;
  }

  if (object) {
    Py_XDECREF(self->object);
    Py_INCREF(object);
    self->object = object;
  }

  return 0;
}

void hkNamedVariantPy::Dealloc(hkNamedVariantPy *self) {
  Py_XDECREF(self->name);
  Py_XDECREF(self->className);
  Py_XDECREF(self->object);
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject *>(self));
}

static PyMappingMethods hkRootLevelContainerPyMappingMethods[] = {
    (lenfunc)hkRootLevelContainerPy::Len,
    (binaryfunc)hkRootLevelContainerPy::Subscript,
    0,
};

static PyTypeObject hkRootLevelContainerPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)                  /* init macro */
    "hkRootLevelContainer",                         /* tp_name */
    sizeof(hkRootLevelContainerPy),                 /* tp_basicsize */
    0,                                              /* tp_itemsize */
    0,                                              /* tp_dealloc */
    0,                                              /* tp_print */
    0,                                              /* tp_getattr */
    0,                                              /* tp_setattr */
    0,                                              /* tp_compare */
    0,                                              /* tp_repr */
    0,                                              /* tp_as_number */
    0,                                              /* tp_as_sequence */
    hkRootLevelContainerPyMappingMethods,           /* tp_as_mapping */
    0,                                              /* tp_hash */
    0,                                              /* tp_call */
    0,                                              /* tp_str */
    0,                                              /* tp_getattro */
    0,                                              /* tp_setattro */
    0,                                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,    /* tp_flags */
    "Havok Root Level Container",                   /* tp_doc */
    0,                                              /* tp_traverse */
    0,                                              /* tp_clear */
    0,                                              /* tp_richcompare */
    0,                                              /* tp_weaklistoffset */
    (getiterfunc)hkRootLevelContainerPy::Iter,      /* tp_iter */
    (iternextfunc)hkRootLevelContainerPy::IterNext, /* tp_iternext */
    0,                                              /* tp_methods */
    0,                                              /* tp_members */
    0,                                              /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    0,                                              /* tp_init */
    0,                                              /* tp_alloc */
    0,                                              /* tp_new */
};

PyTypeObject *hkRootLevelContainerPy::GetType() {
  return &hkRootLevelContainerPyType;
}

PyObject *hkRootLevelContainerPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkRootLevelContainerPy *>(
      PyType_GenericAlloc(&hkRootLevelContainerPyType, 0));
  cted->hkClass = dynamic_cast<const hkRootLevelContainer *>(cls);

  return reinterpret_cast<PyObject *>(cted);
}

size_t hkRootLevelContainerPy::Len(hkRootLevelContainerPy *self) {
  return self->hkClass->Size();
}

PyObject *hkRootLevelContainerPy::Subscript(hkRootLevelContainerPy *self,
                                            PyObject *index) {
  const auto id = PyInt_AsSsize_t(index);
  return self->SubscriptRaw(id);
}

PyObject *hkRootLevelContainerPy::SubscriptRaw(size_t index) {
  auto cVar = hkClass->At(index);
  hkNamedVariantPy *hkVar = reinterpret_cast<hkNamedVariantPy *>(
      PyType_GenericAlloc(&hkNamedVariantPyType, 0));
  hkVar->name = PyString_FromStringAndSize(cVar.name.data(), cVar.name.size());
  hkVar->className =
      PyString_FromStringAndSize(cVar.className.data(), cVar.className.size());

  JenHash classHash = JenkinsHash(cVar.className);

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
  PyAddType<hkRootLevelContainerPy>(module);
  PyAddType<hkNamedVariantPy>(module);
}
