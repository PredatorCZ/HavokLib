/*  Havok Format Unit Tests for Wavelet Animation
    Copyright(C) 2025 Lukas Cone

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
#include "internal/hka_waveletanimation.hpp"
#include "test_common.inl"

int test_wavelet(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);
  auto wavelet =
      static_cast<const hkaWaveletCompressedAnimationInternalInterface *>(
          *static_cast<const hkaAnimationInternalInterface *>(
              checked_deref_cast<const hkaAnimation>(hkNode)));
  TEST_CHECK(wavelet);

  TEST_EQUAL(wavelet->GetAnimationType(), HK_WAVELET_COMPRESSED_ANIMATION);

  for (auto b : nde) {
    std::string_view paramName = b.attribute("name").as_string();

    if (paramName == "numberOfPoses") {
      TEST_EQUAL(b.text().as_int(), wavelet->GetNumOfPoses());
    } else if (paramName == "blockSize") {
      TEST_EQUAL(b.text().as_int(), wavelet->GetBlockSize());
    } else if (paramName == "qFormat") {
      for (auto q : b.first_child()) {
        std::string_view qParamName = q.attribute("name").as_string();

        if (qParamName == "bitWidthIdx") {
          TEST_EQUAL(q.text().as_int(), wavelet->GetBitWidthOffset());
        } else if (qParamName == "preserved") {
          TEST_EQUAL(q.text().as_int(), wavelet->GetNumPreserved());
        } else if (qParamName == "offsetIdx") {
          TEST_EQUAL(q.text().as_int(), wavelet->GetOffsetsOffset());
        } else if (qParamName == "scaleIdx") {
          TEST_EQUAL(q.text().as_int(), wavelet->GetScalesOffset());
        } else if (qParamName == "numD") {
          TEST_EQUAL(q.text().as_int(), wavelet->GetNumDynamicTracks());
        }
      }
    } else if (paramName == "quantizedDataIdx") {
      TEST_EQUAL(b.text().as_int(), wavelet->GetQuantizedDataOffset());
    } else if (paramName == "staticMaskIdx") {
      TEST_EQUAL(b.text().as_int(), wavelet->GetStaticMaskOffset());
    } else if (paramName == "staticDOFsIdx") {
      TEST_EQUAL(b.text().as_int(), wavelet->GetStaticDataOffset());
    }
  }

  auto xmData = nde.find_child_by_attribute("name", "dataBuffer");
  TEST_NOT_CHECK(xmData.empty());
  const size_t dataSize = xmData.attribute("numelements").as_int();
  std::string_view xmDataData = xmData.text().as_string();
  xmDataData = es::SkipStartWhitespace(xmDataData, true);
  const auto dtData = reinterpret_cast<const uint8 *>(wavelet->GetData());

  for (size_t i = 0; i < dataSize; i++) {
    TEST_NOT_CHECK(GetInt(dtData[i], xmDataData));
  }

  return 0;
}

int test_wavelet(std::string name) {
  pugi::xml_document doc;
  const auto xmName = "../resources_wavelet/" + name + ".xml";
  doc.load_file(xmName.c_str());

  int overallResult = 0;

  for (auto &v : versions) {
    if (!v.compare(0, 3, "201")) {
      if (v[3] != '0') {
        break;
      }
    }
    for (auto &p : compiledFlags) {
      std::string curFile("../resources_wavelet/");
      curFile.append(v);
      curFile.append(name);
      curFile.append(p);
      printline("Testing file: " << curFile);

      std::unique_ptr<IhkPackFile> curHk(IhkPackFile::Create(curFile));

      TEST_CHECK(curHk);

      auto rootNode = doc.first_child().first_child();

      for (auto &c : rootNode) {
        std::string_view childName = c.attribute("class").as_string();

        if (childName == "hkRootLevelContainer") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_rootcontainer(c, *allClasses.begin());
        } else if (childName == "hkaWaveletCompressedAnimation" ||
                   childName == "hkaWaveletSkeletalAnimation") {
          auto allClasses = curHk->GetClasses("hkaWaveletCompressedAnimation");
          if (allClasses.empty()) {
            allClasses = curHk->GetClasses("hkaWaveletSkeletalAnimation");
          }
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animation(c, *allClasses.begin());
          overallResult |= test_wavelet(c, *allClasses.begin());
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

int test_wavelet() {
  int overallResult = 0;
  std::string names[]{
      "allosaur_run_W12_8_3",
      "allosaur_run_W12_128_3",
  };

  for (auto &n : names) {
    overallResult |= test_wavelet(n);
  }
  return overallResult;
}
