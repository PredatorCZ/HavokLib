import sys
import os.path as pt
this_dir = pt.dirname(pt.abspath(__file__))
sys.path.append(this_dir + '/../build/python')

import havokpy
from xml.etree.ElementInclude import default_loader as load_xml
import re

xmToolset = havokpy.hkToolset()
assert(len(xmToolset) == 22)
assert(xmToolset[0] == 'HKUNKVER')
assert(xmToolset[1] == 'HK500')
assert(xmToolset[2] == 'HK510')
assert(xmToolset[3] == 'HK550')
assert(xmToolset[4] == 'HK600')
assert(xmToolset[5] == 'HK610')
assert(xmToolset[6] == 'HK650')
assert(xmToolset[7] == 'HK660')
assert(xmToolset[8] == 'HK700')
assert(xmToolset[9] == 'HK710')
assert(xmToolset[10] == 'HK2010_1')
assert(xmToolset[11] == 'HK2010_2')
assert(xmToolset[12] == 'HK2011_1')
assert(xmToolset[13] == 'HK2011_2')
assert(xmToolset[14] == 'HK2011_3')
assert(xmToolset[15] == 'HK2012_1')
assert(xmToolset[16] == 'HK2012_2')
assert(xmToolset[17] == 'HK2013')
assert(xmToolset[18] == 'HK2014')
assert(xmToolset[19] == 'HK2015')
assert(xmToolset[20] == 'HK2016')
assert(xmToolset[21] == 'HK2017')
assert(xmToolset.HKUNKVER == 0)
assert(xmToolset.HK500 == 1)
assert(xmToolset.HK510 == 2)
assert(xmToolset.HK550 == 3)
assert(xmToolset.HK600 == 4)
assert(xmToolset.HK610 == 5)
assert(xmToolset.HK650 == 6)
assert(xmToolset.HK660 == 7)
assert(xmToolset.HK700 == 8)
assert(xmToolset.HK710 == 9)
assert(xmToolset.HK2010_1 == 10)
assert(xmToolset.HK2010_2 == 11)
assert(xmToolset.HK2011_1 == 12)
assert(xmToolset.HK2011_2 == 13)
assert(xmToolset.HK2011_3 == 14)
assert(xmToolset.HK2012_1 == 15)
assert(xmToolset.HK2012_2 == 16)
assert(xmToolset.HK2013 == 17)
assert(xmToolset.HK2014 == 18)
assert(xmToolset.HK2015 == 19)
assert(xmToolset.HK2016 == 20)
assert(xmToolset.HK2017 == 21)

xmRoot = load_xml(this_dir + '/resources/allosaur2012.xml', 'xml')
assert(xmRoot != None)
xmData = xmRoot[0]

hinst = havokpy.hkPackfile(this_dir + "/resources/2012_2/allosaur[4001].hkx")
assert(hinst.version == xmToolset.HK2012_2)

def find_element_by_name(xmlNode, name):
    for c in xmlNode:
        if c.attrib['name'] == name:
            return c
    return None

for ob in xmData:
    xmClass = ob.attrib['class']
    fndClasses = hinst.classes(xmClass)

    if xmClass == 'hkRootLevelContainer':
        assert(len(fndClasses) == 1)
        curXML = 0
        xmVariants = ob[0]

        for vr in fndClasses[0]:
            assert(find_element_by_name(xmVariants[curXML], 'name').text == vr.name)
            assert(find_element_by_name(xmVariants[curXML], 'className').text == vr.class_name)
            curXML += 1
    elif xmClass == 'hkxEnvironment':
        assert(len(fndClasses) == 1)
        curXML = 0
        xmVars = ob[0]

        for vl in fndClasses[0]:
            k, v = next(iter(vl.items()))
            assert(find_element_by_name(xmVars[curXML], 'name').text == k)
            assert(find_element_by_name(xmVars[curXML], 'value').text == v)
            curXML += 1
    elif xmClass == 'hkaSkeleton':
        assert(len(fndClasses) == 1)
        hkSkel = fndClasses[0]
        assert(find_element_by_name(ob, 'name').text == hkSkel.name)
        xmPrentIds = find_element_by_name(ob, 'parentIndices')
        hkBones = hkSkel.bones
        bone0 = hkSkel.bones[0]
        assert(int(xmPrentIds.attrib['numelements']) == len(hkBones))
        xmParentIDs = xmPrentIds.text.split()
        xmTMs = find_element_by_name(ob, 'referencePose')
        xmTMs = re.split('[\(\)\n\t\s]+', xmTMs.text)[1:]
        xmBoneNames = find_element_by_name(ob, 'bones')
        curID = 0
        curTMID = 0

        for b in hkBones:
            cid = b.parent
            if cid == None:
                assert(int(xmParentIDs[curID]) == -1)
            else:
                assert(int(xmParentIDs[curID]) == cid.index)

            assert(b.index == curID)
            assert(b.tm_type == havokpy.uniBoneTMType().TMTYPE_RTS)
            ctm = b.transform

            for i0 in range(3):
                t = ctm[i0]
                rng = 3

                if i0 == 1:
                    rng = 4
                for i1 in range(rng):
                    v = t[i1]
                    assert(float(xmTMs[curTMID]) == round(v, 6))
                    curTMID += 1
            xmBoneName = xmBoneNames[curID]
            xmBoneName = find_element_by_name(xmBoneName, 'name').text
            assert(b.name == xmBoneName)
            curID += 1
    elif xmClass == "hkaAnimationContainer":
        assert(len(fndClasses) == 1)
        hkAniCont = fndClasses[0]

        for c in ob:
            if c.attrib['name'] == 'skeletons':
                cSkels = hkAniCont.skeletons()
                assert(int(c.attrib['numelements']) == len(cSkels))

nv0 = havokpy.hkNamedVariant('testName', 'testClass', None)
assert(nv0.class_name == 'testClass')
assert(nv0.name == 'testName')
assert(nv0.object == None)

nv1 = havokpy.hkNamedVariant(
    class_name = 'testClass',
    name = 'testName'
)

assert(nv1.class_name == 'testClass')
assert(nv1.name == 'testName')
assert(nv1.object == None)

skpart0 = havokpy.hkaPartition('part0', 0, 1)
assert(skpart0.name == 'part0')
assert(skpart0.start_bone_index == 0)
assert(skpart0.num_bones == 1)

skpart1 = havokpy.hkaPartition(
    name = 'part1',
    num_bones = 2
)

assert(skpart1.name == 'part1')
assert(skpart1.start_bone_index == 0)
assert(skpart1.num_bones == 2)
