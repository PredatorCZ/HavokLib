#include "hka_skeleton.hpp"

static PyMember hkaPartitionPyMembers[] = {
    {"name", T_OBJECT_EX, offsetof(hkaPartitionPy, name), 0, "Partition name."},
    {"start_bone_index", T_ULONG, offsetof(hkaPartitionPy, startBoneIndex), 0,
     "Partition starts from bone index."},
    {"num_bones", T_ULONG, offsetof(hkaPartitionPy, numBones), 0,
     "Number bones within parition."},
    {NULL},
};

static PyTypeObject hkaPartitionPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)        /* init macro */
    "hkaPartition",                       /* tp_name */
    sizeof(hkaPartitionPy),               /* tp_basicsize */
    0,                                    /* tp_itemsize */
    (destructor)hkaPartitionPy::Dealloc,  /* tp_dealloc */
    0,                                    /* tp_print */
    0,                                    /* tp_getattr */
    0,                                    /* tp_setattr */
    0,                                    /* tp_compare */
    0,                                    /* tp_repr */
    0,                                    /* tp_as_number */
    0,                                    /* tp_as_sequence */
    0,                                    /* tp_as_mapping */
    0,                                    /* tp_hash */
    0,                                    /* tp_call */
    0,                                    /* tp_str */
    0,                                    /* tp_getattro */
    0,                                    /* tp_setattro */
    0,                                    /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                   /* tp_flags */
    "Havok Skeleton Partition",           /* tp_doc */
    0,                                    /* tp_traverse */
    0,                                    /* tp_clear */
    0,                                    /* tp_richcompare */
    0,                                    /* tp_weaklistoffset */
    0,                                    /* tp_iter */
    0,                                    /* tp_iternext */
    0,                                    /* tp_methods */
    (PyMemberDef *)hkaPartitionPyMembers, /* tp_members */
    0,                                    /* tp_getset */
    0,                                    /* tp_base */
    0,                                    /* tp_dict */
    0,                                    /* tp_descr_get */
    0,                                    /* tp_descr_set */
    0,                                    /* tp_dictoffset */
    (initproc)hkaPartitionPy::Init,       /* tp_init */
    0,                                    /* tp_alloc */
    hkaPartitionPy::New,                  /* tp_new */
};

PyTypeObject *hkaPartitionPy::GetType() { return &hkaPartitionPyType; }

PyObject *hkaPartitionPy::New(PyTypeObject *type, PyObject *, PyObject *) {
  hkaPartitionPy *self = (hkaPartitionPy *)type->tp_alloc(type, 0);

  if (self) {
    self->name = PyString_FromString("");
    if (!self->name) {
      Py_DECREF(self);
      return nullptr;
    }
  }

  return (PyObject *)self;
}

int hkaPartitionPy::Init(hkaPartitionPy *self, PyObject *args, PyObject *kwds) {
  PyObject *name = nullptr;

  static char *kwList[] = {"name", "start_bone_index", "num_bones", nullptr};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Skk", kwList, &name,
                                   &self->startBoneIndex, &self->numBones)) {
    return -1;
  }

  if (name) {
    Py_XDECREF(self->name);
    Py_INCREF(name);
    self->name = name;
  }

  return 0;
}

void hkaPartitionPy::Dealloc(hkaPartitionPy *self) {
  Py_XDECREF(self->name);
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject *>(self));
}

hkaPartitionPy *hkaPartitionPy::Create(const hkaPartition &tp) {
  hkaPartitionPy *self = (hkaPartitionPy *)GetType()->tp_alloc(GetType(), 0);
  self->name = PyString_FromStringAndSize(tp.name.data(), tp.name.size());
  self->startBoneIndex = tp.startBoneIndex;
  self->numBones = tp.numBones;

  return self;
}

static PyTypeObject hkaSkeletonPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)            /* init macro */
    "hkaSkeleton",                            /* tp_name */
    sizeof(hkaSkeletonPy),                    /* tp_basicsize */
    0,                                        /* tp_itemsize */
    0,                                        /* tp_dealloc */
    0,                                        /* tp_print */
    0,                                        /* tp_getattr */
    0,                                        /* tp_setattr */
    0,                                        /* tp_compare */
    0,                                        /* tp_repr */
    0,                                        /* tp_as_number */
    0,                                        /* tp_as_sequence */
    0,                                        /* tp_as_mapping */
    0,                                        /* tp_hash */
    0,                                        /* tp_call */
    0,                                        /* tp_str */
    0,                                        /* tp_getattro */
    0,                                        /* tp_setattro */
    0,                                        /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "Havok Animation Skeleton",               /* tp_doc */
    0,                                        /* tp_traverse */
    0,                                        /* tp_clear */
    0,                                        /* tp_richcompare */
    0,                                        /* tp_weaklistoffset */
    0,                                        /* tp_iter */
    0,                                        /* tp_iternext */
    0,                                        /* tp_methods */
    0,                                        /* tp_members */
    0,                                        /* tp_getset */
    0,                                        /* tp_base */
    0,                                        /* tp_dict */
    0,                                        /* tp_descr_get */
    0,                                        /* tp_descr_set */
    0,                                        /* tp_dictoffset */
    0,                                        /* tp_init */
    0,                                        /* tp_alloc */
    0,                                        /* tp_new */
};

PyTypeObject *hkaSkeletonPy::GetType() { return &hkaSkeletonPyType; }

PyObject *hkaSkeletonPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkaSkeletonPy *>(
      PyType_GenericAlloc(&hkaSkeletonPyType, 0));
  cted->hkClass = dynamic_cast<const hkaSkeleton *>(cls);
  cted->base.item = {dynamic_cast<const uni::Skeleton *>(cls), false};

  return reinterpret_cast<PyObject *>(cted);
}

void hkaSkeletonPy::InitType(PyObject *module) {
  hkaSkeletonPyType.tp_base = UniPy::Skeleton::GetType();

  UniPy::Skeleton::InitType(module);
  PyAddType<hkaSkeletonPy>(module);
  PyAddType<hkaPartitionPy>(module);
}