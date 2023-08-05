#include "hk_packfile.hpp"
#include "datas/python/reflected_enum.hpp"
#include "hk_core.hpp"
#include "hklib/hk_packfile.hpp"
#include "pugixml.hpp"
#include <sstream>
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

using hkToolsetPy = ReflectedEnumPy<hkToolset>;

static PyGetSetDef IHavokPyGetSets[] = {
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

PyTypeObject *IHavokPy::GetType() {
  static PyTypeObject IHavokPyType{
    .tp_name = "hkPackfile",
    .tp_basicsize = sizeof(IHavokPy),
    .tp_dealloc = (destructor)IHavokPy::Dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Havok resource",
    .tp_methods = IHavokPyMethods,
    .tp_getset = (PyGetSetDef *)IHavokPyGetSets,
    .tp_new = IHavokPy::New,
  };
  return &IHavokPyType;
}

void IHavokPy::Dealloc(IHavokPy *self) {
  auto t0 = std::move(self->file);
  Py_TYPE(self)->tp_free(self);
}

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

PyObject *IHavokPy::GetVersion(IHavokPy *self, void *) {
  return PyLong_FromLong(self->file->GetToolset());
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
    std::string_view fileName = fileNameRaw;
    self->file->ToXML(fileName, toolsetVersion);
    Py_RETURN_NONE;
  } else {
    pugi::xml_document doc;
    self->file->ToXML(doc, toolsetVersion);
    std::stringstream ss;
    doc.save(ss);
    std::string str = ss.str();

    return PyUnicode_FromStringAndSize(str.data(), str.size());
  }
}

void IHavokPy::InitType(PyObject *module) {
  PyAddTypes<IHavokPy, hkToolsetPy>(module);
}
