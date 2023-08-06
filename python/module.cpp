
#include "spike/master_printer.hpp"

#include "hk_core.hpp"
#include "hk_packfile.hpp"
#include "hk_rootlevelcontainer.hpp"
#include "hka_animationcontainer.hpp"
#include "hka_skeleton.hpp"
#include "hkx_environment.hpp"

#include "python/uni/skeleton.hpp"

#include <unordered_map>

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

static PyModuleDef module{
  .m_name = "havokpy",
  .m_doc = "Module for working with Havok formats. Version " havokpy_VERSION,
  .m_size = -1,
};

extern "C" PyObject ES_EXPORT *PyInit_havokpy() {
  es::print::AddPrinterFunction(es::Print);
  PyObject *m = PyModule_Create(&module);

  if (!m)
    Py_RETURN_NONE;

  IHavokPy::InitType(m);
  hkRootLevelContainerPy::InitType(m);
  hkxEnvironmentPy::InitType(m);
  hkaSkeletonPy::InitType(m);
  hkaAnimationContainerPy::InitType(m);
  return m;
}

template <class C> auto GenPair() {
  return std::make_pair(
      C::value_type::GetHash(),
      [](const IhkVirtualClass *cls) { return C::Alloc(cls); });
}

template <class... C> auto GenPairs() { return HKPYRegistry{GenPair<C>()...}; }

const HKPYRegistry hkClasses =
    GenPairs<hkxEnvironmentPy, hkaSkeletonPy, hkaAnimationContainerPy,
             hkRootLevelContainerPy>();
