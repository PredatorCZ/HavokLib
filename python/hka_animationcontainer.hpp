#pragma once
#include "hk_core.hpp"
#include "hklib/hka_animationcontainer.hpp"

struct hkaAniContSkeletonListPy {
    PyObject_HEAD;
    typedef const hkaAnimationContainer value_type;
    value_type *hkClass;
    size_t iterPos;

    static size_t Len(hkaAniContSkeletonListPy *self);
    static PyObject *Subscript(hkaAniContSkeletonListPy *self, PyObject *index);
    PyObject *SubscriptRaw(size_t index);
    static PyObject *Iter(hkaAniContSkeletonListPy *self);
    static PyObject *IterNext(hkaAniContSkeletonListPy *self);
};

struct hkaAnimationContainerPy {
    PyObject_HEAD;
    typedef const hkaAnimationContainer value_type;
    value_type *hkClass;
    size_t iterPos;

    static void InitType(PyObject *module);
    static PyObject *Alloc(const IhkVirtualClass *cls);

    static PyObject *Skeletons(hkaAnimationContainerPy *self);
};
