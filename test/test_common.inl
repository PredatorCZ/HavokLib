/*  Havok Format Unit Tests for common classes
    Copyright(C) 2020 Lukas Cone

    This program is free software : you can redistribute it and / or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include "HavokXMLApi.hpp"
#include "datas/unit_testing.hpp"

int test_rootcontainer(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto rlCont = dynamic_cast<const hkRootLevelContainer *>(hkNode);

  TEST_CHECK(rlCont);

  const auto xmNamedVariants = nde.first_child().children();
  auto xmVariant = xmNamedVariants.begin();

  TEST_EQUAL(rlCont->Size(), std::distance(xmVariant, xmNamedVariants.end()));

  for (auto &v : *rlCont) {
    for (auto &b : *xmVariant) {
      es::string_view paramName = b.attribute("name").as_string();

      if (paramName == "className") {
        auto xmText = b.text().as_string();
        TEST_EQUAL(v.className, xmText);
      } else if (paramName == "name") {
        es::string_view xmlText = b.text().as_string();

        if (xmlText == "hkaAnimationContainer") {
          if (v.name != "Merged Animation Container") {
            TEST_EQUAL(v.name, xmlText);
          }
        } else {
          TEST_EQUAL(v.name, xmlText)
        }
      }
    }

    xmVariant++;
  }

  return 0;
}

int test_animationcontainer(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto aniCont = dynamic_cast<const hkaAnimationContainer *>(hkNode);

  TEST_CHECK(aniCont);

  for (auto &c : nde) {
    es::string_view paramName = c.attribute("name").as_string();
    size_t numElements = c.attribute("numelements").as_uint();

    if (paramName == "skeletons") {
      TEST_EQUAL(aniCont->GetNumSkeletons(), numElements);
    } else if (paramName == "animations") {
      TEST_EQUAL(aniCont->GetNumAnimations(), numElements);
    } else if (paramName == "bindings") {
      TEST_EQUAL(aniCont->GetNumBindings(), numElements);
    } else if (paramName == "attachments") {
      TEST_EQUAL(aniCont->GetNumAttachments(), numElements);
    } else if (paramName == "skins") {
      TEST_EQUAL(aniCont->GetNumSkins(), numElements);
    }
  }

  return 0;
}

int GetNumber(float hkNum, es::string_view &sw) {
  char *strEnd = nullptr;
  auto xmNum = std::strtof(sw.begin(), &strEnd);

  TEST_NOT_EQUAL(strEnd, sw.begin());

  TEST_EQUAL(hkNum, xmNum);

  sw = es::SkipStartWhitespace<es::string_view>({strEnd, sw.end()}, true);

  return 0;
};

int GetInt(float hkNum, es::string_view &sw) {
  char *strEnd = nullptr;
  auto xmNum = std::strtol(sw.begin(), &strEnd, 10);

  TEST_NOT_EQUAL(strEnd, sw.begin());

  TEST_EQUAL(hkNum, xmNum);

  sw = es::SkipStartWhitespace<es::string_view>({strEnd, sw.end()}, true);

  return 0;
};

int test_animation(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);
  auto anim = dynamic_cast<const hkaAnimation *>(hkNode);
  TEST_CHECK(anim);

  auto xmAnimType = nde.find_child_by_attribute("name", "type");
  TEST_NOT_CHECK(xmAnimType.empty());
  es::string_view xmAnimTypeData = xmAnimType.text().as_string();
  TEST_EQUAL(xmAnimTypeData, anim->GetAnimationTypeName());

  auto xmDuration = nde.find_child_by_attribute("name", "duration");
  TEST_NOT_CHECK(xmDuration.empty());
  TEST_EQUAL(xmDuration.text().as_float(), anim->Duration());

  auto xmNumTM = nde.find_child_by_attribute("name", "numberOfTransformTracks");
  TEST_NOT_CHECK(xmNumTM.empty());
  TEST_EQUAL(xmNumTM.text().as_int(), anim->GetNumOfTransformTracks());

  auto xmNumFl = nde.find_child_by_attribute("name", "numberOfFloatTracks");

  if (!xmNumFl.empty()) {
    TEST_EQUAL(xmNumFl.text().as_int(), anim->GetNumOfFloatTracks());
  }

  auto xmExM = nde.find_child_by_attribute("name", "extractedMotion");
  TEST_NOT_CHECK(xmExM.empty());

  if (es::string_view("null") == xmExM.text().as_string()) {
    TEST_CHECK(!anim->GetExtractedMotion());
  } else {
    TEST_CHECK(anim->GetExtractedMotion());
  }

  auto xmAnnots = nde.find_child_by_attribute("name", "annotationTracks");

  TEST_NOT_CHECK(xmAnnots.empty());

  const auto cVersion =
      dynamic_cast<const hkVirtualClass *>(hkNode)->rule.version;

  es::string_view xmBoneLinks = xmAnnots.text().as_string();

  xmBoneLinks = es::SkipStartWhitespace(xmBoneLinks, true);

  auto xmParent = nde.parent();

  for (auto a : anim->Annotations()) {
    auto fndEnd = xmBoneLinks.find_first_of("\n ");
    const auto swNPOS = xmBoneLinks.npos;

    TEST_NOT_EQUAL(fndEnd, swNPOS);

    const std::string subName(xmBoneLinks.begin(), fndEnd);

    auto xmBone = xmParent.find_child_by_attribute("name", subName.c_str());

    TEST_NOT_CHECK(xmBone.empty());

    auto xmBoneName = xmBone.find_child_by_attribute("name", "name");

    TEST_NOT_CHECK(xmBoneName.empty());

    TEST_EQUAL(a->GetName(), xmBoneName.text().as_string());

    xmBoneLinks.remove_prefix(fndEnd);
    xmBoneLinks = es::SkipStartWhitespace(xmBoneLinks, true);
  }

  return 0;
}
