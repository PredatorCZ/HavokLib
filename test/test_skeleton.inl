/*  Havok Format Unit Tests for hkaSkeleton
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

int test_environment(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto env = dynamic_cast<const hkxEnvironment *>(hkNode);

  TEST_CHECK(env);

  const auto xmVariables = nde.first_child().children();
  auto xmVar = xmVariables.begin();

  size_t numXmlVars = std::distance(xmVar, xmVariables.end());

  TEST_GT_EQ(env->Size(), numXmlVars);

  for (auto &v : *env) {
    if (v.name == "infoString" || v.name == "slot") {
      continue;
    }

    for (auto &b : *xmVar) {
      es::string_view paramName = b.attribute("name").as_string();
      auto xmText = b.text().as_string();

      if (paramName == "value") {
        TEST_EQUAL(v.value, xmText);
      } else if (paramName == "name") {
        TEST_EQUAL(v.name, xmText)
      }
    }

    xmVar++;
  }

  return 0;
}

int test_skeleton(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto skel = dynamic_cast<const hkaSkeleton *>(hkNode);

  TEST_CHECK(skel);

  auto xmNameNode = nde.find_child_by_attribute("name", "name");

  TEST_NOT_CHECK(xmNameNode.empty());

  TEST_EQUAL(skel->Name(), xmNameNode.text().as_string());

  auto xmParentIndices = nde.find_child_by_attribute("name", "parentIndices");

  TEST_NOT_CHECK(xmParentIndices.empty());

  TEST_EQUAL(skel->GetNumBones(),
             xmParentIndices.attribute("numelements").as_int());

  es::string_view xmParentIDs = xmParentIndices.text().as_string();

  xmParentIDs = es::SkipStartWhitespace(xmParentIDs, true);

  auto getNumber = [](float hkNum, es::string_view &sw) {
    char *strEnd = nullptr;
    auto xmNum = std::strtof(sw.begin(), &strEnd);

    TEST_NOT_EQUAL(strEnd, sw.begin());

    TEST_EQUAL(hkNum, xmNum);

    sw = es::SkipStartWhitespace<es::string_view>({strEnd, sw.end()}, true);

    return 0;
  };

  for (auto b : skel->BoneParentIDs()) {
    TEST_NOT_CHECK(getNumber(b, xmParentIDs));
  }

  auto xmBones = nde.find_child_by_attribute("name", "bones");

  TEST_NOT_CHECK(xmBones.empty());

  es::string_view xmBoneLinks = xmBones.text().as_string();

  xmBoneLinks = es::SkipStartWhitespace(xmBoneLinks, true);

  auto xmParent = nde.parent();

  for (auto b : skel->BoneNames()) {
    auto fndEnd = xmBoneLinks.find_first_of("\n ");
    const auto swNPOS = xmBoneLinks.npos;

    TEST_NOT_EQUAL(fndEnd, swNPOS);

    const std::string subName(xmBoneLinks.begin(), fndEnd);

    auto xmBone = xmParent.find_child_by_attribute("name", subName.c_str());

    TEST_NOT_CHECK(xmBone.empty());

    auto xmBoneName = xmBone.find_child_by_attribute("name", "name");

    TEST_NOT_CHECK(xmBoneName.empty());

    TEST_EQUAL(b, xmBoneName.text().as_string());

    xmBoneLinks.remove_prefix(fndEnd);
    xmBoneLinks = es::SkipStartWhitespace(xmBoneLinks, true);
  }

  auto xmRefPose = nde.find_child_by_attribute("name", "referencePose");

  TEST_NOT_CHECK(xmRefPose.empty());

  es::string_view xmRefPoses = xmRefPose.text().as_string();

  for (auto b : skel->BoneTransforms()) {
    xmRefPoses.remove_prefix(xmRefPoses.find_first_of('(') + 1);

    TEST_NOT_CHECK(getNumber(b.translation.X, xmRefPoses));
    TEST_NOT_CHECK(getNumber(b.translation.Y, xmRefPoses));
    TEST_NOT_CHECK(getNumber(b.translation.Z, xmRefPoses));

    xmRefPoses.remove_prefix(xmRefPoses.find_first_of('(') + 1);

    TEST_NOT_CHECK(getNumber(b.rotation.X, xmRefPoses));
    TEST_NOT_CHECK(getNumber(b.rotation.Y, xmRefPoses));
    TEST_NOT_CHECK(getNumber(b.rotation.Z, xmRefPoses));
    TEST_NOT_CHECK(getNumber(b.rotation.W, xmRefPoses));

    xmRefPoses.remove_prefix(xmRefPoses.find_first_of('(') + 1);

    TEST_NOT_CHECK(getNumber(b.scale.X, xmRefPoses));
    TEST_NOT_CHECK(getNumber(b.scale.Y, xmRefPoses));
    TEST_NOT_CHECK(getNumber(b.scale.Z, xmRefPoses));
  }

  return 0;
}

int test_skeleton() {
  pugi::xml_document doc;
  doc.load_file("allosaur.xml");

  auto rootNode = doc.first_child().first_child();
  int overallResult = 0;

  for (auto &v : versions) {
    for (auto &p : compiledFlags) {
      std::string curFile(v);
      curFile.append("allosaur");
      curFile.append(p);
      printline("Testing file: " << curFile);

      std::unique_ptr<IhkPackFile> curHk(IhkPackFile::Create(curFile));

      TEST_CHECK(curHk);

      for (auto &c : rootNode) {
        es::string_view childName = c.attribute("class").as_string();

        if (childName == "hkRootLevelContainer") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_rootcontainer(c, *allClasses.begin());
        } else if (childName == "hkxEnvironment") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());

          if (v == "550/") {
            pugi::xml_document cDoc;
            auto curNode = cDoc.append_child("");
            xmlHavokFile xmHk;
            xmlEnvironment *env = xmHk.NewClass<xmlEnvironment>();
            env->storage.emplace_back("asset", "rc/550/");
            env->storage.emplace_back(
                "assetFolder",
                "R:\\GitHub\\Havok\\HavokLib\\_samples\\Bin\\rc/550/");
            env->storage.emplace_back(
                "assetPath",
                "R:\\GitHub\\Havok\\HavokLib\\_samples\\Bin\\rc/550/");
            env->storage.emplace_back("out", "allosaur");
            env->storage.emplace_back("outFolder", "R:/CurUser/Desktop/");
            env->storage.emplace_back("outPath",
                                      "R:/CurUser/Desktop/allosaur.hkx");
            env->storage.emplace_back("configuration", "HKX Preview");
            env->ToXML({&curNode, HK550});
            overallResult |= test_environment(curNode, *allClasses.begin());
          } else {
            overallResult |= test_environment(c, *allClasses.begin());
          }
        } else if (childName == "hkaSkeleton") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_skeleton(c, *allClasses.begin());
        }
      }
    }
  }

  return overallResult;
}
