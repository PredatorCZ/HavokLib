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
#include "test_common.inl"

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

  for (auto b : skel->BoneParentIDs()) {
    TEST_NOT_CHECK(GetNumber(b, xmParentIDs));
  }

  auto xmBones = nde.find_child_by_attribute("name", "bones");

  TEST_NOT_CHECK(xmBones.empty());

  const auto cVersion =
      dynamic_cast<const hkVirtualClass *>(hkNode)->rule.version;

  if (cVersion < HK2010_1) {
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
  } else {
    auto xmBone = xmBones.begin();

    for (auto b : skel->BoneNames()) {
      auto xmBoneName = xmBone->find_child_by_attribute("name", "name");

      TEST_NOT_CHECK(xmBoneName.empty());

      TEST_EQUAL(b, xmBoneName.text().as_string());

      xmBone++;
    }
  }

  auto xmRefPose = nde.find_child_by_attribute("name", "referencePose");

  TEST_NOT_CHECK(xmRefPose.empty());

  es::string_view xmRefPoses = xmRefPose.text().as_string();

  for (auto b : skel->BoneTransforms()) {
    xmRefPoses.remove_prefix(xmRefPoses.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(b->translation.X, xmRefPoses));
    TEST_NOT_CHECK(GetNumber(b->translation.Y, xmRefPoses));
    TEST_NOT_CHECK(GetNumber(b->translation.Z, xmRefPoses));

    xmRefPoses.remove_prefix(xmRefPoses.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(b->rotation.X, xmRefPoses));
    TEST_NOT_CHECK(GetNumber(b->rotation.Y, xmRefPoses));
    TEST_NOT_CHECK(GetNumber(b->rotation.Z, xmRefPoses));
    TEST_NOT_CHECK(GetNumber(b->rotation.W, xmRefPoses));

    xmRefPoses.remove_prefix(xmRefPoses.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(b->scale.X, xmRefPoses));
    TEST_NOT_CHECK(GetNumber(b->scale.Y, xmRefPoses));
    TEST_NOT_CHECK(GetNumber(b->scale.Z, xmRefPoses));
  }

  auto xmFloatSlots = nde.find_child_by_attribute("name", "floatSlots");

  if (!xmFloatSlots.empty()) {
    TEST_EQUAL(skel->GetNumFloatSlots(),
               xmFloatSlots.attribute("numelements").as_int());

    auto xmflstBegin = xmFloatSlots.begin();

    for (auto f : skel->FloatSlots()) {
      TEST_EQUAL(f, xmflstBegin->text().as_string());
      xmflstBegin++;
    }
  }

  auto xmRefFloats = nde.find_child_by_attribute("name", "referenceFloats");

  if (!xmRefFloats.empty()) {
    TEST_EQUAL(skel->GetNumReferenceFloats(),
               xmRefFloats.attribute("numelements").as_int());

    es::string_view xmRefFloatsText = xmRefFloats.text().as_string();
    xmRefFloatsText = es::SkipStartWhitespace(xmRefFloatsText, true);

    for (auto f : skel->ReferenceFloats()) {
      TEST_NOT_CHECK(GetNumber(f, xmRefFloatsText));
    }
  }

  auto xmPartitions = nde.find_child_by_attribute("name", "partitions");

  if (!xmPartitions.empty()) {
    TEST_EQUAL(skel->GetNumPartitions(),
               xmPartitions.attribute("numelements").as_int());
    auto xmPartBegin = xmPartitions.begin();

    for (auto p : skel->Partitions()) {
      TEST_EQUAL(p.name, xmPartBegin->find_child_by_attribute("name", "name")
                             .text()
                             .as_string());
      TEST_EQUAL(p.startBoneIndex,
                 xmPartBegin->find_child_by_attribute("name", "startBoneIndex")
                     .text()
                     .as_int());
      TEST_EQUAL(p.numBones,
                 xmPartBegin->find_child_by_attribute("name", "numBones")
                     .text()
                     .as_int());

      xmPartBegin++;
    }
  }

  return 0;
}

int test_skeleton() {
  pugi::xml_document doc;
  doc.load_file("allosaur550.xml");

  int overallResult = 0;

  for (auto &v : versions) {
    if (!v.compare(0, 3, "201")) {
      if (v[3] == '2') {
        doc.load_file("allosaur2012.xml");
      } else {
        doc.load_file("allosaur2010.xml");
      }
    }

    for (auto &p : compiledFlags) {
      std::string curFile(v);
      curFile.append("allosaur");
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
        } else if (childName == "hkxEnvironment") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());

          if (v == "550/") {
            pugi::xml_document cDoc;
            auto curNode = cDoc.append_child("");
            xmlHavokFile xmHk;
            xmlEnvironment *env = xmHk.NewClass<xmlEnvironment>();
            env->storage.emplace_back("asset", "resources/550/");
            env->storage.emplace_back(
                "assetFolder",
                "r:\\GitHub\\Havok\\HavokLib\\test\\resources/550/");
            env->storage.emplace_back(
                "assetPath",
                "r:\\GitHub\\Havok\\HavokLib\\test\\resources/550/");
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
