#include "hka_skeleton.hpp"
#include <structmember.h>
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

static PyMemberDef hkaPartitionPyMembers[] = {
    {"name", T_OBJECT_EX, offsetof(hkaPartitionPy, name), 0, "Partition name."},
    {"start_bone_index", T_UINT, offsetof(hkaPartitionPy, startBoneIndex), 0,
     "Partition starts from bone index."},
    {"num_bones", T_UINT, offsetof(hkaPartitionPy, numBones), 0,
     "Number bones within parition."},
    {NULL},
};

PyTypeObject *hkaPartitionPy::GetType() {
  static PyTypeObject hkaPartitionPyType{
      .tp_name = "hkaPartition",
      .tp_basicsize = sizeof(hkaPartitionPy),
      .tp_flags = Py_TPFLAGS_DEFAULT,
      .tp_doc = "Havok Skeleton Partition",
      .tp_members = (PyMemberDef *)hkaPartitionPyMembers,
      .tp_init = (initproc)hkaPartitionPy::Init,
      .tp_new = hkaPartitionPy::New,
  };

  return &hkaPartitionPyType;
}

PyObject *hkaPartitionPy::New(PyTypeObject *type, PyObject *, PyObject *) {
  hkaPartitionPy *self = (hkaPartitionPy *)type->tp_alloc(type, 0);

  if (self) {
    self->name = PyUnicode_FromString("");
    if (!self->name) {
      Py_DECREF(self);
      return nullptr;
    }
  }

  return (PyObject *)self;
}

int hkaPartitionPy::Init(hkaPartitionPy *self, PyObject *args, PyObject *kwds) {
  static char *kwList[] = {"name", "start_bone_index", "num_bones", nullptr};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|UII", kwList, &self->name,
                                   &self->startBoneIndex, &self->numBones)) {
    return -1;
  }

  return 0;
}

hkaPartitionPy *hkaPartitionPy::Create(const hkaPartition &tp) {
  hkaPartitionPy *self = (hkaPartitionPy *)GetType()->tp_alloc(GetType(), 0);
  self->name = PyUnicode_FromStringAndSize(tp.name.data(), tp.name.size());
  self->startBoneIndex = tp.startBoneIndex;
  self->numBones = tp.numBones;

  return self;
}

PyTypeObject *hkaSkeletonPy::GetType() {
  static PyTypeObject hkaSkeletonPyType{
      .tp_name = "hkaSkeleton",
      .tp_basicsize = sizeof(hkaSkeletonPy),
      .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
      .tp_doc = "Havok Animation Skeleton",
      .tp_base = UniPy::Skeleton::GetType(),
  };

  return &hkaSkeletonPyType;
}

PyObject *hkaSkeletonPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkaSkeletonPy *>(
      PyType_GenericAlloc(hkaSkeletonPy::GetType(), 0));
  cted->hkClass = checked_deref_cast<const hkaSkeleton>(cls);
  cted->base.item = {static_cast<const uni::Skeleton *>(cted->hkClass), false};

  return reinterpret_cast<PyObject *>(cted);
}

void hkaSkeletonPy::InitType(PyObject *module) {
  UniPy::Skeleton::InitType(module);
  PyAddTypes<hkaSkeletonPy, hkaPartitionPy>(module);
}
