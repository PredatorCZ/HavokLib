/*  Havok Format Unit Tests for Interleaved Animation
    Copyright(C) 2020-2022 Lukas Cone

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
#include "test_common.inl"

int test_interleaved(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);
  auto inter =
      checked_deref_cast<const hkaInterleavedAnimationInternalInterface>(
          static_cast<const hkaAnimationInternalInterface *>(
              checked_deref_cast<const hkaAnimation>(hkNode)));
  TEST_CHECK(inter);

  TEST_EQUAL(inter->GetAnimationType(), HK_INTERLEAVED_ANIMATION);

  auto xmTMs = nde.find_child_by_attribute("name", "transforms");
  TEST_NOT_CHECK(xmTMs.empty());
  const size_t numTransforms = inter->GetNumTransforms();
  TEST_EQUAL(numTransforms, xmTMs.attribute("numelements").as_int());
  es::string_view xmTMData = xmTMs.text().as_string();

  for (size_t t = 0; t < numTransforms; t++) {
    auto kVal = inter->GetTransform(t);

    xmTMData.remove_prefix(xmTMData.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(kVal->translation.X, xmTMData));
    TEST_NOT_CHECK(GetNumber(kVal->translation.Y, xmTMData));
    TEST_NOT_CHECK(GetNumber(kVal->translation.Z, xmTMData));

    xmTMData.remove_prefix(xmTMData.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(kVal->rotation.X, xmTMData));
    TEST_NOT_CHECK(GetNumber(kVal->rotation.Y, xmTMData));
    TEST_NOT_CHECK(GetNumber(kVal->rotation.Z, xmTMData));
    TEST_NOT_CHECK(GetNumber(kVal->rotation.W, xmTMData));

    xmTMData.remove_prefix(xmTMData.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(kVal->scale.X, xmTMData));
    TEST_NOT_CHECK(GetNumber(kVal->scale.Y, xmTMData));
    TEST_NOT_CHECK(GetNumber(kVal->scale.Z, xmTMData));
  }

  return 0;
}

int test_interleaved() {
  pugi::xml_document doc;
  doc.load_file("allosaur_run.xml");

  int overallResult = 0;

  for (auto &v : versions) {
    for (auto &p : compiledFlags) {
      std::string curFile(v);
      curFile.append("allosaur_run");
      curFile.append(p);
      printline("Testing file: " << curFile);

      std::unique_ptr<IhkPackFile> curHk(IhkPackFile::Create(curFile));

      TEST_CHECK(curHk);

      auto rootNode = doc.first_child().first_child();

      for (auto &c : rootNode) {
        es::string_view childName = c.attribute("class").as_string();

        if (childName == "hkRootLevelContainer") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_rootcontainer(c, *allClasses.begin());
        } else if (childName == "hkaInterleavedUncompressedAnimation" ||
                   childName == "hkaInterleavedSkeletalAnimation") {
          auto allClasses =
              curHk->GetClasses("hkaInterleavedUncompressedAnimation");
          if (allClasses.empty()) {
            allClasses = curHk->GetClasses("hkaInterleavedSkeletalAnimation");
          }
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animation(c, *allClasses.begin());
          overallResult |= test_interleaved(c, *allClasses.begin());
        } else if (childName == "hkaAnimationContainer") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animationcontainer(c, *allClasses.begin());
        } else if (childName == "hkaAnimationBinding") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animationbinding(c, *allClasses.begin());
        }
      }
    }
  }

  return overallResult;
}
