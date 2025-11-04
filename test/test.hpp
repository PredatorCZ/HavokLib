#pragma once
#include "havok_xml.hpp"
#include "pugixml.hpp"
#include "spike/master_printer.hpp"
#include "spike/util/unit_testing.hpp"

static const std::string_view compiledFlags[] = {
    "[4001].hkx", "[4011].hkx", "[4101].hkx", "[4111].hkx",
    "[8001].hkx", "[8011].hkx", "[8101].hkx", "[8111].hkx",
};

static const std::string_view versions[] = {
    "550/",    "660/",    "710/",    "2010_2/", "2011_1/",
    "2011_2/", "2012_2/", "2013_1/", "2014_1/",
};

static inline int GetNumber(float hkNum, std::string_view &sw) {
  char *strEnd = nullptr;
  auto xmNum = std::strtof(sw.data(), &strEnd);

  const size_t processed =
      std::distance(sw.data(), const_cast<const char *>(strEnd));

  TEST_NOT_EQUAL(processed, 0);

  TEST_EQUAL(hkNum, xmNum);
  sw.remove_prefix(processed);

  sw = es::SkipStartWhitespace(sw, true);

  return 0;
};

static inline int GetInt(float hkNum, std::string_view &sw) {
  char *strEnd = nullptr;
  auto xmNum = std::strtol(sw.data(), &strEnd, 10);

  const size_t processed =
      std::distance(sw.data(), const_cast<const char *>(strEnd));

  TEST_NOT_EQUAL(processed, 0);

  TEST_EQUAL(hkNum, xmNum);
  sw.remove_prefix(processed);

  sw = es::SkipStartWhitespace(sw, true);

  return 0;
};

static inline int GetLink(pugi::xml_node &out, pugi::xml_node &root,
                          std::string_view &sw) {
  auto fndEnd = sw.find_first_of("\n ");
  //const auto swNPOS = sw.npos;

  //TEST_NOT_EQUAL(fndEnd, swNPOS);

  const std::string subName(sw.substr(0, fndEnd));

  TEST_NOT_CHECK(subName.empty());

  out = root.find_child_by_attribute("name", subName.c_str());

  TEST_NOT_CHECK(out.empty());

  sw.remove_prefix(fndEnd);
  sw = es::SkipStartWhitespace(sw, true);

  return 0;
}

int test_sample();
int test_environment(pugi::xml_node nde, IhkVirtualClass *hkNode);
int test_skeleton(pugi::xml_node nde, IhkVirtualClass *hkNode);
int test_skeleton();
int test_rootcontainer(pugi::xml_node nde, IhkVirtualClass *hkNode);
int test_animationcontainer(pugi::xml_node nde, IhkVirtualClass *hkNode);
int test_animationbinding(pugi::xml_node nde, IhkVirtualClass *hkNode);
int test_animation(pugi::xml_node nde, IhkVirtualClass *hkNode);
int test_delta();
int test_interleaved();
int test_spline();
int test_wavelet();
int test_xml_base();
int test_defaultmotion();
int test_mesh(pugi::xml_node nde, const IhkVirtualClass *hkNode);
int test_scene();
