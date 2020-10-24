/*  Havok Format Unit Tests for Animated Reference Frame
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
#include "test_common.inl"
#include "internal/hka_animatedreferenceframe.hpp"

int test_defaultmotion(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);
  auto defan =
      dynamic_cast<const hkaAnimatedReferenceFrameInternalInterface *>(hkNode);
  TEST_CHECK(defan);

  TEST_EQUAL(defan->GetType(), hkaAnimatedReferenceFrameType::DEFAULT);

  auto xmUp = nde.find_child_by_attribute("name", "up");
  TEST_NOT_CHECK(xmUp.empty());
  es::string_view xmUpData = xmUp.text().as_string();

  xmUpData.remove_prefix(xmUpData.find_first_of('(') + 1);

  const auto hkUp = defan->GetUp();

  TEST_NOT_CHECK(GetNumber(hkUp.X, xmUpData));
  TEST_NOT_CHECK(GetNumber(hkUp.Y, xmUpData));
  TEST_NOT_CHECK(GetNumber(hkUp.Z, xmUpData));
  TEST_NOT_CHECK(GetNumber(hkUp.W, xmUpData));

  auto xmForward = nde.find_child_by_attribute("name", "forward");
  TEST_NOT_CHECK(xmForward.empty());
  es::string_view xmFwdData = xmForward.text().as_string();

  xmFwdData.remove_prefix(xmFwdData.find_first_of('(') + 1);

  const auto hkFwd = defan->GetForward();

  TEST_NOT_CHECK(GetNumber(hkFwd.X, xmFwdData));
  TEST_NOT_CHECK(GetNumber(hkFwd.Y, xmFwdData));
  TEST_NOT_CHECK(GetNumber(hkFwd.Z, xmFwdData));
  TEST_NOT_CHECK(GetNumber(hkFwd.W, xmFwdData));

  auto xmDuration = nde.find_child_by_attribute("name", "duration");
  TEST_NOT_CHECK(xmDuration.empty());
  TEST_EQUAL(defan->GetDuration(), xmDuration.text().as_float());

  auto xmData = nde.find_child_by_attribute("name", "referenceFrameSamples");
  TEST_NOT_CHECK(xmDuration.empty());
  const size_t numFrames = defan->GetNumFrames();
  TEST_EQUAL(numFrames, xmData.attribute("numelements").as_int());

  es::string_view xmFrames = xmData.text().as_string();

  for (size_t f = 0; f < numFrames; f++) {
    xmFrames.remove_prefix(xmFrames.find_first_of('(') + 1);
    auto hkFrame = defan->GetRefFrame(f);

    TEST_NOT_CHECK(GetNumber(hkFrame.X, xmFrames));
    TEST_NOT_CHECK(GetNumber(hkFrame.Y, xmFrames));
    TEST_NOT_CHECK(GetNumber(hkFrame.Z, xmFrames));
    TEST_NOT_CHECK(GetNumber(hkFrame.W, xmFrames));
  }

  return 0;
}

int test_defaultmotion() {
  pugi::xml_document doc;
  doc.load_file("animref.xml");

  int overallResult = 0;

  for (auto &v : versions) {
    for (auto &p : compiledFlags) {
      std::string curFile(v);
      curFile.append("animref");
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
        } else if (childName == "hkaDefaultAnimatedReferenceFrame") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_defaultmotion(c, *allClasses.begin());
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
