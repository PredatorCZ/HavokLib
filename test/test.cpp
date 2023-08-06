/*  Havok Format Unit Tests
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

#include "pugixml.hpp"
#include "spike/master_printer.hpp"

static const std::string_view compiledFlags[] = {
    "[4001].hkx", "[4011].hkx", "[4101].hkx", "[4111].hkx",
    "[8001].hkx", "[8011].hkx", "[8101].hkx", "[8111].hkx",
};

static const std::string_view versions[] = {
    "550/",    "660/",    "710/",    "2010_2/", "2011_1/",
    "2011_2/", "2012_2/", "2013_1/", "2014_1/",
};

#include "test_defaultmotion.inl"
#include "test_delta.inl"
#include "test_interleaved.inl"
#include "test_skeleton.inl"
#include "test_spline.inl"
#include "test_xml.inl"

#include "test_sample.inl"

int main() {
  es::print::AddPrinterFunction(es::Print);

  TEST_CASES(int resultVar, TEST_FUNC(test_skeleton),
             TEST_FUNC(test_defaultmotion), TEST_FUNC(test_spline),
             TEST_FUNC(test_interleaved), TEST_FUNC(test_delta),
             TEST_FUNC(test_sample), TEST_FUNC(test_xml_base));

  return resultVar;
}
