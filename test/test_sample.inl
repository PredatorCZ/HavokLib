/*  Havok Format Unit Tests for Animation Sampling
    Copyright(C) 2020-2023 Lukas Cone

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
#include "hklib/hk_packfile.hpp"
#include "hklib/hka_animation.hpp"
#include "spike/util/unit_testing.hpp"
#include <random>

int test_sample() {
  static const std::string_view names[]{
      "allosaur_run_D12_8_3",
      "allosaur_run_S32_V16_B255_D1",
      "allosaur_run_S40_V16_B25_D3",
      "allosaur_run_S48_V8_B255_D2",
  };

  std::unique_ptr<IhkPackFile> interFile(
      IhkPackFile::Create("550/allosaur_run[4101].hkx"));
  auto interAnim = checked_deref_cast<const hkaAnimation>(
      interFile->GetClasses("hkaAnimation")[0]);
  auto interTracks = interAnim->Tracks();
  auto numTracks = interTracks->Size();
  int overallResult = 0;

  std::random_device rd;
  std::uniform_real_distribution<float> rm(0.01, 0.04);
  std::vector<float> times(100);
  float curTime = rm(rd);
  Vector4A16::SetEpsilon(0.35);
  size_t totalNumTracks = 0, numFiles = 0;

  for (size_t i = 0; i < 100; i++) {
    times[i] = curTime;
    curTime += rm(rd);
  }

  for (auto &n : names) {
    for (auto &v : versions) {
      if (n == "allosaur_run_D12_8_3" && v == "2011_1/") {
        break;
      }
      for (auto &r : compiledFlags) {
        std::string curFile(v);
        curFile.append(n);
        curFile.append(r);

        std::unique_ptr<IhkPackFile> animFile(IhkPackFile::Create(curFile));
        const hkaAnimation *hkAnim = checked_deref_cast<const hkaAnimation>(
            animFile->GetClasses("hkaAnimation")[0]);
        auto animTracks = hkAnim->Tracks();
        numFiles++;

        for (size_t i = 0; i < numTracks; i++) {
          auto cTrack = animTracks->At(i);
          auto iTrack = interTracks->At(i);
          totalNumTracks++;

          for (auto f : times) {
            hkQTransform var0, var1;

            cTrack->GetValue(var0, f);
            iTrack->GetValue(var1, f);

            overallResult |= [&]() {
              TEST_EQUAL(var0.translation, var1.translation);
              TEST_EQUAL(var0.rotation, var1.rotation);
              TEST_EQUAL(var0.scale, var1.scale);

              return 0;
            }();
          }
        }
      }
    }
  }

  printline("Sample test finished for " << numFiles << " files with "
                                        << totalNumTracks << " tracks.");

  return overallResult;
}
