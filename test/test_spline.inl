/*  Havok Format Unit Tests for Spline Animation
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
#include "internal/hka_splineanimation.hpp"
#include "test_common.inl"

int test_spline(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);
  auto spline =
      checked_deref_cast<const hkaSplineCompressedAnimationInternalInterface>(
          static_cast<const hkaAnimationInternalInterface *>(
              checked_deref_cast<const hkaAnimation>(hkNode)));
  TEST_CHECK(spline);

  TEST_EQUAL(spline->GetAnimationType(), HK_SPLINE_COMPRESSED_ANIMATION);

  for (auto b : nde) {
    es::string_view paramName = b.attribute("name").as_string();

    if (paramName == "numFrames") {
      TEST_EQUAL(b.text().as_int(), spline->GetNumFrames());
    } else if (paramName == "numBlocks") {
      TEST_EQUAL(b.text().as_int(), spline->GetNumBlocks());
    } else if (paramName == "maxFramesPerBlock") {
      TEST_EQUAL(b.text().as_int(), spline->GetMaxFramesPerBlock());
    } else if (paramName == "blockDuration") {
      TEST_LT(b.text().as_float(), spline->GetBlockDuration() + 0.0001f);
      TEST_GT(b.text().as_float(), spline->GetBlockDuration() - 0.0001f);
    } else if (paramName == "blockInverseDuration") {
      TEST_LT(b.text().as_float(), spline->GetBlockInverseDuration() + 0.0001f);
      TEST_GT(b.text().as_float(), spline->GetBlockInverseDuration() - 0.0001f);
    } else if (paramName == "frameDuration") {
      TEST_LT(b.text().as_float(), spline->GetFrameDuration() + 0.0001f);
      TEST_GT(b.text().as_float(), spline->GetFrameDuration() - 0.0001f);
    } else if (paramName == "blockOffsets") {
      auto boffs = spline->GetBlockOffsets();
      TEST_EQUAL(b.attribute("numelements").as_int(), boffs.size());
      es::string_view xmDataData = b.text().as_string();
      xmDataData = es::SkipStartWhitespace(xmDataData, true);

      for (auto b : boffs) {
        TEST_NOT_CHECK(GetInt(b, xmDataData));
      }
    } else if (paramName == "floatBlockOffsets") {
      auto boffs = spline->GetFloatBlockOffsets();
      TEST_EQUAL(b.attribute("numelements").as_int(), boffs.size());
      es::string_view xmDataData = b.text().as_string();
      xmDataData = es::SkipStartWhitespace(xmDataData, true);

      for (auto b : boffs) {
        TEST_NOT_CHECK(GetInt(b, xmDataData));
      }
    } else if (paramName == "transformOffsets") {
      auto boffs = spline->GetTransformOffsets();
      TEST_EQUAL(b.attribute("numelements").as_int(), boffs.size());
      es::string_view xmDataData = b.text().as_string();
      xmDataData = es::SkipStartWhitespace(xmDataData, true);

      for (auto b : boffs) {
        TEST_NOT_CHECK(GetInt(b, xmDataData));
      }
    } else if (paramName == "floatOffsets") {
      auto boffs = spline->GetFloatOffsets();
      TEST_EQUAL(b.attribute("numelements").as_int(), boffs.size());
      es::string_view xmDataData = b.text().as_string();
      xmDataData = es::SkipStartWhitespace(xmDataData, true);

      for (auto b : boffs) {
        TEST_NOT_CHECK(GetInt(b, xmDataData));
      }
    }
  }

  auto xmData = nde.find_child_by_attribute("name", "data");
  TEST_NOT_CHECK(xmData.empty());
  const size_t dataSize = xmData.attribute("numelements").as_int();
  es::string_view xmDataData = xmData.text().as_string();
  xmDataData = es::SkipStartWhitespace(xmDataData, true);
  const auto dtData = reinterpret_cast<const uint8 *>(spline->GetData());

  for (size_t i = 0; i < dataSize; i++) {
    TEST_NOT_CHECK(GetInt(dtData[i], xmDataData));
  }

  return 0;
}

int test_spline(const std::string &name) {
  pugi::xml_document doc;
  int overallResult = 0;
  const auto xmName = name + ".xml";
  doc.load_file(xmName.c_str());

  for (auto &v : versions) {
    for (auto &p : compiledFlags) {
      std::string curFile(v);
      curFile.append(name);
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
        } else if (childName == "hkaSplineSkeletalAnimation" ||
                   childName == "hkaSplineCompressedAnimation") {
          auto allClasses = curHk->GetClasses("hkaSplineSkeletalAnimation");
          if (allClasses.empty()) {
            allClasses = curHk->GetClasses("hkaSplineCompressedAnimation");
          }
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animation(c, *allClasses.begin());
          overallResult |= test_spline(c, *allClasses.begin());
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

int test_spline() {
  int overallResult = 0;
  std::string names[]{"allosaur_run_S32_V16_B255_D1",
                      "allosaur_run_S40_V16_B25_D3",
                      "allosaur_run_S48_V8_B255_D2"};

  for (auto &n : names) {
    overallResult |= test_spline(n);
  }
  return overallResult;
}
