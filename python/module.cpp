
#include "datas/macroLoop.hpp"
#include "datas/master_printer.hpp"
#include "project.h"

#include "hk_core.hpp"
#include "hk_packfile.hpp"
#include "hk_rootlevelcontainer.hpp"
#include "hka_animationcontainer.hpp"
#include "hka_skeleton.hpp"
#include "hkx_environment.hpp"

#include "uni/python/pyskeleton.hpp"

#include <unordered_map>

extern "C" ES_EXPORT_FN(void) inithavokpy() {
  printer.AddPrinterFunction(
      reinterpret_cast<MasterPrinterThread::print_func>(printf));
  PyObject *m = Py_InitModule3(
      "havokpy", nullptr,
      "Module for working with Havok formats. Version " havokpy_VERSION);

  if (!m)
    return;

  IHavokPy::InitType(m);
  hkRootLevelContainerPy::InitType(m);
  hkxEnvironmentPy::InitType(m);
  hkaSkeletonPy::InitType(m);
  hkaAnimationContainerPy::InitType(m);
}

#define _REG_PYCLASS(item)                                                     \
  {item::value_type::GetHash(),                                                \
   [](const IhkVirtualClass *cls) { return item::Alloc(cls); }},

extern const HKPYRegistry hkClasses = {
    StaticFor(_REG_PYCLASS, hkRootLevelContainerPy, hkxEnvironmentPy,
              hkaSkeletonPy, hkaAnimationContainerPy)};
