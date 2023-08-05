/*  Havok Format Unit Tests for hkaSkeleton
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
#include "HavokApi.hpp"
#include <fstream>
#include <map>
#include <random>

static const std::string_view names[]{
    "allosaur_run_S32_V16_B255_D1",
    "allosaur_run_S40_V16_B25_D3",
    "allosaur_run_S48_V8_B255_D2",
    "allosaur_run_D12_8_3",
    "allosaur_run",
};

static const std::string_view compiledFlags[] = {
    "[4001].hkx", "[4011].hkx", "[4101].hkx", "[4111].hkx",
    "[8001].hkx", "[8011].hkx", "[8101].hkx", "[8111].hkx",
};

static const std::string_view versions[] = {
    "550/",    "660/",    "710/",    "2010_2/", "2011_1/",
    "2011_2/", "2012_2/", "2013_1/", "2014_1/",
};

int test_sample() {
  std::ofstream logFile("../samples.csv");
  std::string vars[]{"RX", "RY", "RZ", "RW", "TX",
                     "TY", "TZ", "SX", "SY", "SZ"};

  for (const auto &n : names) {
    for (const auto &t : vars) {
      logFile << n << '_' << t << ';';
    }
  }
  logFile << std::endl;

  float times[100]{};
  float curTime = 0.f;
  std::random_device rd;
  std::uniform_real_distribution<float> rnd(0.01f, 0.04f);

  for (size_t i = 0; i < 100; i++) {
    curTime += rnd(rd);
    times[i] = curTime;
  }

  for (const auto &name : names) {
    for (auto &v : versions) {
      if (v == "2011_1/" && name == names[3]) {
        break;
      }
      for (auto &p : compiledFlags) {
        std::string curFile(v);
        curFile.append(name);
        curFile.append(p);

        std::unique_ptr<IhkPackFile> curHk(IhkPackFile::Create(curFile));

        const auto hkAnim = checked_deref_cast<const hkaAnimation>(
            curHk->GetClasses("hkaAnimation")[0]);
        const auto hkTracks = hkAnim->Tracks();
        Vector4A16::SetEpsilon(0.35f);
        const size_t numTracks = hkTracks->Size();

        for (size_t ti = 0; ti < numTracks; ti++) {
          const auto curve1 = hkTracks->At(ti);
          for (auto t : times) {
            uni::RTSValue val0, val1;
            // curve0->GetValue(val0, t);
            curve1->GetValue(val1, t);
            logFile << val1.rotation.X << ';' << val1.rotation.Y << ';'
                    << val1.rotation.Z << ';' << val1.rotation.W << ';'
                    << val1.translation.X << ';' << val1.translation.Y << ';'
                    << val1.translation.Z << ';' << val1.scale.X << ';'
                    << val1.scale.Y << ';' << val1.scale.Z << ';' << std::endl;
            // TEST_EQUAL(val1.rotation, val0.rotation);
            // TEST_EQUAL(val1.translation, val0.translation);
            // TEST_EQUAL(val1.scale, val0.scale);
          }
        }
      }
    }
  }

  return 0;
}

int test_sample_old() {
  int overallResult = 0;
  std::string names[]{
      "allosaur_run_S32_V16_B255_D1",
      "allosaur_run_S40_V16_B25_D3",
      "allosaur_run_S48_V8_B255_D2",
      "allosaur_run_D12_8_3",
      "allosaur_run",
  };

  std::ofstream logFile("../samples.csv");
  std::string vars[]{"RX", "RY", "RZ", "RW", "TX",
                     "TY", "TZ", "SX", "SY", "SZ"};

  for (const auto &n : names) {
    for (const auto &t : vars) {
      logFile << n << '_' << t << ';';
    }
  }
  logFile << std::endl;

  /*std::string uFile = versions[0];
  uFile.append("allosaur_run");
  uFile.append(compiledFlags[0]);*/
  size_t numFiles = 0, numTotalTracks = 0;

  /* std::unique_ptr<IhkPackFile> interFile(IhkPackFile::Create(uFile));
   const auto interAnim = dynamic_cast<const hkaAnimation *>(
       interFile->GetClasses("hkaAnimation")[0]);
   const auto interTracks = interAnim->Tracks();
   const size_t numTracks = interTracks->Size();*/
  float times[100]{};
  float curTime = 0.f;
  std::random_device rd;
  std::uniform_real_distribution<float> rnd(0.01f, 0.04f);

  for (size_t i = 0; i < 100; i++) {
    curTime += rnd(rd);
    times[i] = curTime;
  }

  for (const auto &name : names) {
    for (auto &v : versions) {
      if (v == "2011_1/" && name == names[3]) {
        break;
      }
      for (auto &p : compiledFlags) {
        std::string curFile(v);
        curFile.append(name);
        curFile.append(p);
        numFiles++;

        std::unique_ptr<IhkPackFile> curHk(IhkPackFile::Create(curFile));

        const auto hkAnim = checked_deref_cast<const hkaAnimation>(
            curHk->GetClasses("hkaAnimation")[0]);
        const auto hkTracks = hkAnim->Tracks();
        Vector4A16::SetEpsilon(0.35f);
        const size_t numTracks = hkTracks->Size();

        overallResult |= [&]() {
          for (size_t ti = 0; ti < numTracks; ti++) {
            // const auto curve0 = interTracks->At(ti);
            const auto curve1 = hkTracks->At(ti);
            numTotalTracks++;
            // TEST_EQUAL(curve0->BoneIndex(), curve1->BoneIndex());

            for (auto t : times) {
              uni::RTSValue val0, val1;
              // curve0->GetValue(val0, t);
              curve1->GetValue(val1, t);
              logFile << val1.rotation.X << ';' << val1.rotation.Y << ';'
                      << val1.rotation.Z << ';' << val1.rotation.W << ';'
                      << val1.translation.X << ';' << val1.translation.Y << ';'
                      << val1.translation.Z << ';' << val1.scale.X << ';'
                      << val1.scale.Y << ';' << val1.scale.Z << ';'
                      << std::endl;
              // TEST_EQUAL(val1.rotation, val0.rotation);
              // TEST_EQUAL(val1.translation, val0.translation);
              // TEST_EQUAL(val1.scale, val0.scale);
            }
          }

          return 0;
        }();
      }
    }
  }
  printline("Sample tested " << numFiles << " files & " << numTotalTracks
                             << " tracks.");
  return overallResult;
}
