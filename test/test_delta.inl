/*  Havok Format Unit Tests for Delta Animation
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

int test_delta(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);
  auto delta =
      dynamic_cast<const hkaDeltaCompressedAnimationInternalInterface *>(
          hkNode);
  TEST_CHECK(delta);

  TEST_EQUAL(delta->GetAnimationType(), HK_DELTA_COMPRESSED_ANIMATION);

  for (auto b : nde) {
    es::string_view paramName = b.attribute("name").as_string();

    if (paramName == "numberOfPoses") {
      TEST_EQUAL(b.text().as_int(), delta->GetNumOfPoses());
    } else if (paramName == "blockSize") {
      TEST_EQUAL(b.text().as_int(), delta->GetBlockSize());
    } else if (paramName == "qFormat") {
      for (auto q : *nde.begin()) {
        es::string_view qParamName = b.attribute("name").as_string();

        if (qParamName == "bitWidthIdx") {
          TEST_EQUAL(b.text().as_int(), delta->GetBitWidthOffset());
        } else if (qParamName == "preserved") {
          TEST_EQUAL(b.text().as_int(), delta->GetNumPreserved());
        } else if (qParamName == "offsetIdx") {
          TEST_EQUAL(b.text().as_int(), delta->GetOffsetsOffset());
        } else if (qParamName == "scaleIdx") {
          TEST_EQUAL(b.text().as_int(), delta->GetScalesOffset());
        } else if (qParamName == "numD") {
          TEST_EQUAL(b.text().as_int(), delta->GetNumDynamicTracks());
        }
      }
    } else if (paramName == "quantizedDataIdx") {
      TEST_EQUAL(b.text().as_int(), delta->GetQuantizedDataOffset());
    } else if (paramName == "staticMaskIdx") {
      TEST_EQUAL(b.text().as_int(), delta->GetStaticMaskOffset());
    } else if (paramName == "staticDOFsIdx") {
      TEST_EQUAL(b.text().as_int(), delta->GetStaticDataOffset());
    }
  }

  auto xmData = nde.find_child_by_attribute("name", "dataBuffer");
  TEST_NOT_CHECK(xmData.empty());
  const size_t dataSize = xmData.attribute("numelements").as_int();
  es::string_view xmDataData = xmData.text().as_string();
  xmDataData = es::SkipStartWhitespace(xmDataData, true);
  const auto dtData = reinterpret_cast<const uint8 *>(delta->GetData());

  for (size_t i = 0; i < dataSize; i++) {
    TEST_NOT_CHECK(GetInt(dtData[i], xmDataData));
  }

  return 0;
}

int test_delta() {
  pugi::xml_document doc;
  doc.load_file("allosaur_run_D12_8_3.xml");

  int overallResult = 0;

  for (auto &v : versions) {
    if (!v.compare(0, 3, "201")) {
      if (v[3] != '0') {
        break;
      }
    }
    for (auto &p : compiledFlags) {
      std::string curFile(v);
      curFile.append("allosaur_run_D12_8_3");
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
        } else if (childName == "hkaDeltaCompressedSkeletalAnimation" ||
                   childName == "hkaDeltaCompressedAnimation") {
          auto allClasses =
              curHk->GetClasses("hkaDeltaCompressedSkeletalAnimation");
          if (allClasses.empty()) {
            allClasses = curHk->GetClasses("hkaDeltaCompressedAnimation");
          }
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animation(c, *allClasses.begin());
          overallResult |= test_delta(c, *allClasses.begin());
        } else if (childName == "hkaAnimationContainer") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animationcontainer(c, *allClasses.begin());
        }
      }
    }
  }

  return overallResult;
}
