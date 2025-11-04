/*  Havok Format Unit Tests for hkxScene
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

#include "hklib/hkx_scene.hpp"
#include "test.hpp"

int test_material(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto material = checked_deref_cast<const hkxMaterial>(hkNode);
  TEST_CHECK(material);

  auto xmNameNode = nde.find_child_by_attribute("name", "name");

  TEST_NOT_CHECK(xmNameNode.empty());

  TEST_EQUAL(material->Name(), xmNameNode.text().as_string());

  auto xmStages = nde.find_child_by_attribute("name", "stages");

  TEST_NOT_CHECK(xmStages.empty());

  TEST_EQUAL(material->GetNumStages(),
             xmStages.attribute("numelements").as_int());

  // auto xmStageChild = xmStages.child("hkObject");

  for (hkxMaterialTextureStage s : material->Stages()) {
    // auto children = xmStageChild.children();
    TEST_CHECK(s.texture);
  }

  auto xmDiffuseColor = nde.find_child_by_attribute("name", "diffuseColor");
  TEST_NOT_CHECK(xmDiffuseColor.empty());
  std::string_view xmDiffuseColorData = xmDiffuseColor.text().as_string();

  xmDiffuseColorData.remove_prefix(xmDiffuseColorData.find_first_of('(') + 1);

  const auto hkDiffuseColor = material->DiffuseColor();

  TEST_NOT_CHECK(GetNumber(hkDiffuseColor.X, xmDiffuseColorData));
  TEST_NOT_CHECK(GetNumber(hkDiffuseColor.Y, xmDiffuseColorData));
  TEST_NOT_CHECK(GetNumber(hkDiffuseColor.Z, xmDiffuseColorData));
  TEST_NOT_CHECK(GetNumber(hkDiffuseColor.W, xmDiffuseColorData));

  auto xmambientColor = nde.find_child_by_attribute("name", "ambientColor");
  TEST_NOT_CHECK(xmambientColor.empty());
  std::string_view xmambientColorData = xmambientColor.text().as_string();

  xmambientColorData.remove_prefix(xmambientColorData.find_first_of('(') + 1);

  const auto hkambientColor = material->AmbientColor();

  TEST_NOT_CHECK(GetNumber(hkambientColor.X, xmambientColorData));
  TEST_NOT_CHECK(GetNumber(hkambientColor.Y, xmambientColorData));
  TEST_NOT_CHECK(GetNumber(hkambientColor.Z, xmambientColorData));
  TEST_NOT_CHECK(GetNumber(hkambientColor.W, xmambientColorData));

  auto xmspecularColor = nde.find_child_by_attribute("name", "specularColor");
  TEST_NOT_CHECK(xmspecularColor.empty());
  std::string_view xmspecularColorData = xmspecularColor.text().as_string();

  xmspecularColorData.remove_prefix(xmspecularColorData.find_first_of('(') + 1);

  const auto hkspecularColor = material->SpecularColor();

  TEST_NOT_CHECK(GetNumber(hkspecularColor.X, xmspecularColorData));
  TEST_NOT_CHECK(GetNumber(hkspecularColor.Y, xmspecularColorData));
  TEST_NOT_CHECK(GetNumber(hkspecularColor.Z, xmspecularColorData));
  TEST_NOT_CHECK(GetNumber(hkspecularColor.W, xmspecularColorData));

  auto xmemissiveColor = nde.find_child_by_attribute("name", "emissiveColor");
  TEST_NOT_CHECK(xmemissiveColor.empty());
  std::string_view xmemissiveColorData = xmemissiveColor.text().as_string();

  xmemissiveColorData.remove_prefix(xmemissiveColorData.find_first_of('(') + 1);

  const auto hkemissiveColor = material->EmissiveColor();

  TEST_NOT_CHECK(GetNumber(hkemissiveColor.X, xmemissiveColorData));
  TEST_NOT_CHECK(GetNumber(hkemissiveColor.Y, xmemissiveColorData));
  TEST_NOT_CHECK(GetNumber(hkemissiveColor.Z, xmemissiveColorData));
  TEST_NOT_CHECK(GetNumber(hkemissiveColor.W, xmemissiveColorData));

  return 0;
}

int test_node(pugi::xml_node root, pugi::xml_node nde,
              const IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto node = checked_deref_cast<const hkxNode>(hkNode);

  TEST_CHECK(node);

  auto xmNameNode = nde.find_child_by_attribute("name", "name");

  TEST_NOT_CHECK(xmNameNode.empty());

  if (node->Name().empty()) {
    TEST_EQUAL(std::string_view("␀"), xmNameNode.text().as_string());
  } else {
    TEST_EQUAL(node->Name(), xmNameNode.text().as_string());
  }

  auto xmChildrenNode = nde.find_child_by_attribute("name", "children");

  TEST_NOT_CHECK(xmChildrenNode.empty());

  TEST_EQUAL(node->GetNumChildren(),
             xmChildrenNode.attribute("numelements").as_int());

  std::string_view xmChildren = xmChildrenNode.text().as_string();

  xmChildren = es::SkipStartWhitespace(xmChildren, true);

  int overallResult = 0;

  for (auto ib : node->Children()) {
    pugi::xml_node xmChild;
    TEST_NOT_CHECK(GetLink(xmChild, root, xmChildren));

    overallResult |= test_node(root, xmChild, ib.get());
  }

  auto xmSelectedNode = nde.find_child_by_attribute("name", "selected");

  TEST_NOT_CHECK(xmSelectedNode.empty());

  TEST_EQUAL(node->Selected(), xmSelectedNode.text().as_bool());

  auto xmKeyFrames = nde.find_child_by_attribute("name", "keyFrames");

  TEST_NOT_CHECK(xmKeyFrames.empty());

  TEST_EQUAL(node->KeyFrames().size(),
             xmKeyFrames.attribute("numelements").as_int());

  auto xmAnnotations = nde.find_child_by_attribute("name", "annotations");

  TEST_NOT_CHECK(xmAnnotations.empty());

  TEST_EQUAL(node->GetNumAnnotations(),
             xmAnnotations.attribute("numelements").as_int());

  auto xmAnnotationsItem = xmAnnotations.first_child();

  for (auto a : node->Annotations()) {
    auto xmTime = xmAnnotationsItem.find_child_by_attribute("name", "time");

    TEST_NOT_CHECK(xmTime.empty());

    TEST_EQUAL(a.time, xmTime.text().as_float());

    auto xmDescription =
        xmAnnotationsItem.find_child_by_attribute("name", "description");

    TEST_NOT_CHECK(xmDescription.empty());

    TEST_EQUAL(a.description, xmDescription.text().as_string());

    xmAnnotationsItem = xmAnnotationsItem.next_sibling();
  }

  return overallResult;
}

int test_camera(pugi::xml_node nde, const hkxCamera *camera) {
  TEST_CHECK(camera);

  {
    auto xmFrom = nde.find_child_by_attribute("name", "from");
    TEST_NOT_CHECK(xmFrom.empty());
    std::string_view xmFromData = xmFrom.text().as_string();

    xmFromData.remove_prefix(xmFromData.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(camera->from.X, xmFromData));
    TEST_NOT_CHECK(GetNumber(camera->from.Y, xmFromData));
    TEST_NOT_CHECK(GetNumber(camera->from.Z, xmFromData));
    TEST_NOT_CHECK(GetNumber(camera->from.W, xmFromData));
  }

  {
    auto xmFocus = nde.find_child_by_attribute("name", "focus");
    TEST_NOT_CHECK(xmFocus.empty());
    std::string_view xmFocusData = xmFocus.text().as_string();

    xmFocusData.remove_prefix(xmFocusData.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(camera->focus.X, xmFocusData));
    TEST_NOT_CHECK(GetNumber(camera->focus.Y, xmFocusData));
    TEST_NOT_CHECK(GetNumber(camera->focus.Z, xmFocusData));
    TEST_NOT_CHECK(GetNumber(camera->focus.W, xmFocusData));
  }

  {
    auto xmUp = nde.find_child_by_attribute("name", "up");
    TEST_NOT_CHECK(xmUp.empty());
    std::string_view xmUpData = xmUp.text().as_string();

    xmUpData.remove_prefix(xmUpData.find_first_of('(') + 1);

    TEST_NOT_CHECK(GetNumber(camera->up.X, xmUpData));
    TEST_NOT_CHECK(GetNumber(camera->up.Y, xmUpData));
    TEST_NOT_CHECK(GetNumber(camera->up.Z, xmUpData));
    TEST_NOT_CHECK(GetNumber(camera->up.W, xmUpData));
  }

  auto xmFov = nde.find_child_by_attribute("name", "fov");
  TEST_NOT_CHECK(xmFov.empty());
  TEST_EQUAL(xmFov.text().as_float(), camera->fov);

  auto xmFar = nde.find_child_by_attribute("name", "far");
  TEST_NOT_CHECK(xmFar.empty());
  TEST_EQUAL(xmFar.text().as_float(), camera->far);

  auto xmNear = nde.find_child_by_attribute("name", "near");
  TEST_NOT_CHECK(xmNear.empty());
  TEST_EQUAL(xmNear.text().as_float(), camera->near);

  auto xmLeftHanded = nde.find_child_by_attribute("name", "leftHanded");
  TEST_NOT_CHECK(xmLeftHanded.empty());
  TEST_EQUAL(xmLeftHanded.text().as_bool(), camera->leftHanded);

  return 0;
}

int test_scene(pugi::xml_node nde, IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto scene = checked_deref_cast<const hkxScene>(hkNode);

  TEST_CHECK(scene);

  auto xmRootNode = nde.find_child_by_attribute("name", "rootNode");

  TEST_NOT_CHECK(xmRootNode.empty());

  std::string_view xmNode = xmRootNode.text().as_string();

  xmNode = es::SkipStartWhitespace(xmNode, true);
  auto xmParent = nde.parent();
  pugi::xml_node xmChild;
  TEST_NOT_CHECK(GetLink(xmChild, xmParent, xmNode));

  int overallResult = test_node(xmParent, xmChild, scene->RootNode().get());

  for (auto &c : nde) {
    std::string_view paramName = c.attribute("name").as_string();
    size_t numElements = c.attribute("numelements").as_uint();

    if (paramName == "selectionSets") {
      TEST_EQUAL(scene->GetNumSelectionSets(), numElements);
      if (numElements) {
        TEST_CHECK(scene->GetSelectionSet(0));
      }
    } else if (paramName == "cameras") {
      TEST_EQUAL(scene->GetNumCameras(), numElements);
      std::string_view xmCameras = c.text().as_string();

      xmCameras = es::SkipStartWhitespace(xmCameras, true);
      for (auto cam : scene->Cameras()) {
        pugi::xml_node xmChild;
        TEST_NOT_CHECK(GetLink(xmChild, xmParent, xmCameras));
        TEST_NOT_CHECK(test_camera(xmChild, cam));
      }
    } else if (paramName == "lights") {
      TEST_EQUAL(scene->GetNumLights(), numElements);
      if (numElements) {
        TEST_CHECK(scene->GetLight(0));
      }
    } else if (paramName == "meshes") {
      TEST_EQUAL(scene->GetNumMeshes(), numElements);
      if (numElements) {
        TEST_CHECK(scene->GetMesh(0));
      }
    } else if (paramName == "materials") {
      TEST_EQUAL(scene->GetNumMaterials(), numElements);
      if (numElements) {
        TEST_CHECK(scene->GetMaterial(0));
      }
    } else if (paramName == "inplaceTextures") {
      TEST_EQUAL(scene->GetNumInplaceTextures(), numElements);
      if (numElements) {
        TEST_CHECK(scene->GetInplaceTexture(0));
      }
    } else if (paramName == "externalTextures") {
      TEST_EQUAL(scene->GetNumExternalTextures(), numElements);
      if (numElements) {
        TEST_CHECK(scene->GetExternalTexture(0));
      }
    } else if (paramName == "skinBindings") {
      TEST_EQUAL(scene->GetNumSkinBindings(), numElements);
      if (numElements) {
        TEST_CHECK(scene->GetSkinBinding(0));
      }
    }
  }

  return overallResult;
}

int test_scene() {
  pugi::xml_document doc;
  doc.load_file("../resources_mesh/teapot.xml");

  int overallResult = 0;

  for (auto &v : versions) {
    if (v.starts_with("201")) {
      if (v[3] == '0') {
        doc.load_file("teapot2010.xml");
      } else if (v[3] == '2') {
        doc.load_file("teapot2012.xml");
      } else if (v[3] == '4') {
        doc.load_file("teapot2014.xml");
      } else if (v.starts_with("2011_1")) {
        continue;
      }
    } else if (v.starts_with("660")) {
      doc.load_file("teapot660.xml");
    } else if (v.starts_with("710")) {
      doc.load_file("teapot710.xml");
    }

    for (auto &p : compiledFlags) {
      std::string curFile("../resources_mesh/");
      curFile.append(v);
      curFile.append("teapot");
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
        } else if (childName == "hkxScene") {
          auto allClasses = curHk->GetClasses("hkxScene");
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_scene(c, *allClasses.begin());
        } else if (childName == "hkaAnimationContainer") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_animationcontainer(c, *allClasses.begin());
        } else if (childName == "hkxMesh") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_mesh(c, *allClasses.begin());
        } else if (childName == "hkxMaterial") {
          auto allClasses = curHk->GetClasses(childName);
          TEST_NOT_CHECK(allClasses.empty());
          overallResult |= test_material(c, *allClasses.begin());
        }
      }
    }
  }

  return overallResult;
}
