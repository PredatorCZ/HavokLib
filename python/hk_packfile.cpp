#include "hk_packfile.hpp"
#include "datas/python/reflected_enum.hpp"
#include "hk_core.hpp"
#include "hklib/hk_packfile.hpp"
#include "pugixml.hpp"
#include <sstream>

using hkToolsetPy = ReflectedEnumPy<hkToolset>;

static PyGetSet IHavokPyGetSets[] = {
    {"version", (getter)IHavokPy::GetVersion, nullptr,
     "Get version of loaded resource."},
    {NULL} /* Sentinel */
};

static PyMethodDef IHavokPyMethods[] = {
    {"classes", (PyCFunction)IHavokPy::GetClasses, METH_VARARGS,
     "Find classes by name."},
    {"to_xml", (PyCFunction)IHavokPy::ToXML, METH_VARARGS,
     "Export to XML format."},
    {NULL} /* Sentinel */
};

static PyTypeObject IHavokPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)  /* init macro */
    "hkPackfile",                   /* tp_name */
    sizeof(IHavokPy),               /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)IHavokPy::Dealloc,  /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_compare */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,             /* tp_flags */
    "Havok resource",               /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    IHavokPyMethods,                /* tp_methods */
    0,                              /* tp_members */
    (PyGetSetDef *)IHavokPyGetSets, /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    0,                              /* tp_init */
    0,                              /* tp_alloc */
    IHavokPy::New,                  /* tp_new */
};

PyTypeObject *IHavokPy::GetType() { return &IHavokPyType; }

void IHavokPy::Dealloc(IHavokPy *self) { auto t0 = std::move(self->file); }

PyObject *IHavokPy::New(PyTypeObject *type, PyObject *args, PyObject *) {
  IHavokPy *self = reinterpret_cast<IHavokPy *>(type->tp_alloc(type, 0));

  if (self) {
    const char *fileNameRaw = nullptr;
    if (!PyArg_ParseTuple(args, "s", &fileNameRaw)) {
      return nullptr;
    }

    std::string fileName(fileNameRaw);

    self->file = std::unique_ptr<IhkPackFile>(IhkPackFile::Create(fileName));
  }

  return reinterpret_cast<PyObject *>(self);
}

PyObject *IHavokPy::GetVersion(IHavokPy *self) {
  return PyInt_FromLong(self->file->GetToolset());
}

PyObject *IHavokPy::GetClasses(IHavokPy *self, PyObject *args) {
  const char *classNameRaw = nullptr;
  if (!PyArg_ParseTuple(args, "s", &classNameRaw)) {
    return nullptr;
  }

  JenHash classHash(classNameRaw);

  auto foundItems = self->file->GetClasses(classHash);
  PyObject *rtlist = PyList_New(0);

  for (auto &c : foundItems) {
    auto foundCt = hkClasses.find(classHash);

    if (foundCt == hkClasses.end()) {
      continue;
    }

    PyList_Append(rtlist, foundCt->second(c));
  }

  return rtlist;
}

PyObject *IHavokPy::ToXML(IHavokPy *self, PyObject *args) {
  const char *fileNameRaw = nullptr;
  hkToolset toolsetVersion;

  if (!PyArg_ParseTuple(args, "I|s", &toolsetVersion, &fileNameRaw)) {
    return nullptr;
  }

  if (fileNameRaw) {
    es::string_view fileName = fileNameRaw;
    self->file->ToXML(fileName, toolsetVersion);
    return Py_None;
  } else {
    pugi::xml_document doc;
    self->file->ToXML(doc, toolsetVersion);
    std::stringstream ss;
    doc.save(ss);
    std::string str = ss.str();

    return PyString_FromString(str.data());
  }
}

void IHavokPy::InitType(PyObject *module) {
  PyAddType<IHavokPy>(module);
  PyAddType<hkToolsetPy>(module);
}
