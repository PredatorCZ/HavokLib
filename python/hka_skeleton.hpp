#pragma once
#include "hk_core.hpp"
#include "uni/python/pyskeleton.hpp"

struct hkaPartitionPy {
    PyObject_HEAD;
    
    PyObject *name;
    uint32 startBoneIndex;
    uint32 numBones;

    static PyObject *New(PyTypeObject *type, PyObject *args, PyObject *kwds);
    static int Init(hkaPartitionPy *self, PyObject *args, PyObject *kwds);
    static void Dealloc(hkaPartitionPy *self);
    static hkaPartitionPy *Create(const hkaPartition &tp);
    static PyTypeObject *GetType();
};

struct hkaSkeletonPy {
    UniPy::Skeleton base;
    typedef const hkaSkeleton value_type;
    value_type *hkClass;

    static void InitType(PyObject *module);
    static PyObject *Alloc(const IhkVirtualClass *cls);
    static PyTypeObject *GetType();
};