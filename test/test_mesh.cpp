/*  Havok Format Unit Tests for hkxMesh
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

#include "hklib/hkx_mesh.hpp"
#include "test.hpp"

int test_indexbuffer(pugi::xml_node nde, const hkxIndexBuffer *buffer) {
  TEST_CHECK(buffer);

  auto xmIndexType = nde.find_child_by_attribute("name", "indexType");

  TEST_NOT_CHECK(xmIndexType.empty());
  TEST_EQUAL(buffer->GetIndexType(), IndexType::TRI_LIST);

  auto xmIndices16 = nde.find_child_by_attribute("name", "indices16");

  TEST_NOT_CHECK(xmIndices16.empty());
  TEST_EQUAL(buffer->NumIndices(),
             xmIndices16.attribute("numelements").as_int() *
                 !buffer->Is32Bit());

  auto xmIndices32 = nde.find_child_by_attribute("name", "indices32");

  TEST_NOT_CHECK(xmIndices32.empty());
  TEST_EQUAL(buffer->NumIndices(),
             xmIndices32.attribute("numelements").as_int() * buffer->Is32Bit());

  auto xmVertexBaseOffset =
      nde.find_child_by_attribute("name", "vertexBaseOffset");

  TEST_NOT_CHECK(xmVertexBaseOffset.empty());
  TEST_EQUAL(xmVertexBaseOffset.text().as_uint(), buffer->VerrtexBaseOffset());

  auto xmLength = nde.find_child_by_attribute("name", "length");

  TEST_NOT_CHECK(xmLength.empty());
  TEST_EQUAL(xmLength.text().as_uint(), buffer->Length());

  return 0;
}

int test_vertexbuffer(pugi::xml_node , const hkxVertexBuffer *buffer)
{
  TEST_CHECK(buffer);
  TEST_EQUAL(buffer->NumVertices(), 3072);

  return 0;
}

int test_section(pugi::xml_node nde, const hkxMeshSection *section) {
  TEST_CHECK(section);

  auto xmVertexBuffer = nde.find_child_by_attribute("name", "vertexBuffer");

  TEST_NOT_CHECK(xmVertexBuffer.empty());

  int overallResult = test_vertexbuffer(xmVertexBuffer, section->VertexBuffer().get());

  auto xmIndexBuffersNode = nde.find_child_by_attribute("name", "indexBuffers");

  TEST_NOT_CHECK(xmIndexBuffersNode.empty());

  TEST_EQUAL(section->GetNumIndexBuffers(),
             xmIndexBuffersNode.attribute("numelements").as_int());

  std::string_view xmIndexBuffers = xmIndexBuffersNode.text().as_string();

  xmIndexBuffers = es::SkipStartWhitespace(xmIndexBuffers, true);

  auto xmParent = nde.parent();

  for (auto ib : section->IndexBuffers()) {
    pugi::xml_node xmIndexBuffer;
    TEST_NOT_CHECK(GetLink(xmIndexBuffer, xmParent, xmIndexBuffers));

    overallResult |= test_indexbuffer(xmIndexBuffer, *ib);
  }

  return overallResult;
}

int test_mesh(pugi::xml_node nde, const IhkVirtualClass *hkNode) {
  TEST_CHECK(hkNode);

  auto mesh = safe_deref_cast<const hkxMesh>(hkNode);

  TEST_CHECK(mesh);

  auto xmSectionsNode = nde.find_child_by_attribute("name", "sections");

  TEST_NOT_CHECK(xmSectionsNode.empty());

  TEST_EQUAL(mesh->GetNumSections(),
             xmSectionsNode.attribute("numelements").as_int());

  std::string_view xmSections = xmSectionsNode.text().as_string();

  xmSections = es::SkipStartWhitespace(xmSections, true);

  auto xmParent = nde.parent();

  int overallResult = 0;

  for (auto s : mesh->Sections()) {
    pugi::xml_node xmSection;
    TEST_NOT_CHECK(GetLink(xmSection, xmParent, xmSections));

    overallResult |= test_section(xmSection, *s);
  }

  auto xmUserChannelInfos =
      nde.find_child_by_attribute("name", "userChannelInfos");

  TEST_NOT_CHECK(xmUserChannelInfos.empty());

  TEST_EQUAL(mesh->GetNumUserChannelInfos(),
             xmUserChannelInfos.attribute("numelements").as_int());

  return overallResult;
}
