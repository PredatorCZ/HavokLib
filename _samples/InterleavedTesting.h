#pragma once

void TestBinding(const hkaAnimationContainer *aniCont)
{
	const hkaAnimationBinding *binds = aniCont->GetBinding(0);

	MyAssert(binds->GetBlendHint() != BlendHint::NORMAL);
	MyAssert(binds->GetNumTransformTrackToBoneIndices() != 93);
	MyAssert(binds->GetNumFloatTrackToFloatSlotIndices() != 0);

	for (int cTID = 0; cTID < binds->GetNumTransformTrackToBoneIndices(); cTID++)
	{
		MyAssertLoop(binds->GetTransformTrackToBoneIndex(cTID) != cTID, cTID);
	}
}

void TestAllosaurInterleaved(const std::wstring testingPath)
{
	for (auto &v : versions)
		for (auto &t : compiledFlags)
		{
			std::wstring fleName = testingPath;
			fleName.append(v);
			fleName.append(L"allosaur_run");
			fleName.append(t);

			printline("testing: ", << fleName);
			IhkPackFile * hdr = IhkPackFile::Create(fleName.c_str());

			if (!hdr)
			{
				printerror("testing failed!");
				continue;
			}

			hkRootLevelContainer *rcont = hdr->GetRootLevelContainer();

			if (!rcont)
			{
				printerror("Couldn't find hkRootLevelContainer!");
				continue;
			}

			const hkaAnimationContainer *aniCont = rcont->GetVariant(0);

			TestBinding(aniCont);

			const hkaAnimation *anim = aniCont->GetAnimation(0);
			
			MyAssert(!FLTCMP(anim->GetDuration(), 3.33354211));
			MyAssert(anim->GetNumAnnotations() != 93);

			for (int cAnnot = 0; cAnnot < anim->GetNumAnnotations(); cAnnot++)
			{
				MyAssertLoop(strcmp(anim->GetAnnotation(cAnnot).get()->GetName(), boneNames[cAnnot]), cAnnot);
			}

			const hkaInterleavedAnimationInternalInterface *iterAnim = dynamic_cast<const hkaInterleavedAnimationInternalInterface *>(anim);

			MyAssert(iterAnim->GetNumTransforms() != 9393);
			MyAssert(iterAnim->GetNumFloats() != 0);

			delete hdr;
		}
}

void TestAllosaurDelta(const std::wstring testingPath)
{
	for (auto &v : versions_delta)
		for (auto &t : compiledFlags)
		{
			std::wstring fleName = testingPath;
			fleName.append(v);
			fleName.append(L"allosaur_run_D12_8_3");
			fleName.append(t);

			printline("testing: ", << fleName);
			IhkPackFile * hdr = IhkPackFile::Create(fleName.c_str());

			if (!hdr)
			{
				printerror("testing failed!");
				continue;
			}

			hkRootLevelContainer *rcont = hdr->GetRootLevelContainer();

			if (!rcont)
			{
				printerror("Couldn't find hkRootLevelContainer!");
				continue;
			}

			const hkaAnimationContainer *aniCont = rcont->GetVariant(0);

			TestBinding(aniCont);

			const hkaAnimation *anim = aniCont->GetAnimation(0);

			MyAssert(!FLTCMP(anim->GetDuration(), 3.33354211));
			MyAssert(anim->GetNumAnnotations() != 93);

			for (int cAnnot = 0; cAnnot < anim->GetNumAnnotations(); cAnnot++)
			{
				MyAssertLoop(strcmp(anim->GetAnnotation(cAnnot).get()->GetName(), boneNames[cAnnot]), cAnnot);
			}

			const hkaDeltaCompressedAnimationInternalInterface *iterAnim = dynamic_cast<const hkaDeltaCompressedAnimationInternalInterface *>(anim);

			MyAssert(iterAnim->GetNumOfPoses() != 101);
			MyAssert(iterAnim->GetBlockSize() != 8);
			MyAssert(iterAnim->GetQuantizedDataOffset() != 3824);
			MyAssert(iterAnim->GetStaticMaskOffset() != 4);
			MyAssert(iterAnim->GetStaticDataOffset() != 192);
			MyAssert(iterAnim->GetNumDynamicTracks() != 210);
			MyAssert(iterAnim->GetOffsetsOffset() != 1932);
			MyAssert(iterAnim->GetBitWidthOffset() != 3612);
			MyAssert(iterAnim->GetScalesOffset() != 2772);
			MyAssert(iterAnim->GetNumPreserved() != 1);

			delete hdr;
		}
}

void TestAllosaurSpline1(const std::wstring testingPath)
{
	for (auto &v : versions_delta)
		for (auto &t : compiledFlags)
		{
			std::wstring fleName = testingPath;
			fleName.append(v);
			fleName.append(L"allosaur_run_S32_V16_B255_D1");
			fleName.append(t);

			printline("testing: ", << fleName);
			IhkPackFile *hdr = IhkPackFile::Create(fleName.c_str());

			if (!hdr)
			{
				printerror("testing failed!");
				continue;
			}

			hkRootLevelContainer *rcont = hdr->GetRootLevelContainer();

			if (!rcont)
			{
				printerror("Couldn't find hkRootLevelContainer!");
				continue;
			}

			const hkaAnimationContainer *aniCont = rcont->GetVariant(0);

			TestBinding(aniCont);

			const hkaAnimation *anim = aniCont->GetAnimation(0);

			MyAssert(!FLTCMP(anim->GetDuration(), 3.33354211));
			MyAssert(anim->GetNumAnnotations() != 93);

			for (int cAnnot = 0; cAnnot < anim->GetNumAnnotations(); cAnnot++)
			{
				MyAssertLoop(strcmp(anim->GetAnnotation(cAnnot).get()->GetName(), boneNames[cAnnot]), cAnnot);
			}

			// Add some further testing here?

			delete hdr;
		}
}

void TestAllosaurSpline2(const std::wstring testingPath)
{
	for (auto &v : versions_delta)
		for (auto &t : compiledFlags)
		{
			std::wstring fleName = testingPath;
			fleName.append(v);
			fleName.append(L"allosaur_run_S40_V16_B25_D3");
			fleName.append(t);

			printline("testing: ", << fleName);
			IhkPackFile *hdr = IhkPackFile::Create(fleName.c_str());

			if (!hdr)
			{
				printerror("testing failed!");
				continue;
			}

			hkRootLevelContainer *rcont = hdr->GetRootLevelContainer();

			if (!rcont)
			{
				printerror("Couldn't find hkRootLevelContainer!");
				continue;
			}

			const hkaAnimationContainer *aniCont = rcont->GetVariant(0);

			TestBinding(aniCont);

			const hkaAnimation *anim = aniCont->GetAnimation(0);

			MyAssert(!FLTCMP(anim->GetDuration(), 3.33354211));
			MyAssert(anim->GetNumAnnotations() != 93);

			for (int cAnnot = 0; cAnnot < anim->GetNumAnnotations(); cAnnot++)
			{
				MyAssertLoop(strcmp(anim->GetAnnotation(cAnnot).get()->GetName(), boneNames[cAnnot]), cAnnot);
			}

			// Add some further testing here?

			delete hdr;
		}
}

void TestAllosaurSpline3(const std::wstring testingPath)
{
	for (auto &v : versions_delta)
		for (auto &t : compiledFlags)
		{
			std::wstring fleName = testingPath;
			fleName.append(v);
			fleName.append(L"allosaur_run_S48_V8_B255_D2");
			fleName.append(t);

			printline("testing: ", << fleName);
			IhkPackFile *hdr = IhkPackFile::Create(fleName.c_str());

			if (!hdr)
			{
				printerror("testing failed!");
				continue;
			}

			hkRootLevelContainer *rcont = hdr->GetRootLevelContainer();

			if (!rcont)
			{
				printerror("Couldn't find hkRootLevelContainer!");
				continue;
			}

			const hkaAnimationContainer *aniCont = rcont->GetVariant(0);

			TestBinding(aniCont);

			const hkaAnimation *anim = aniCont->GetAnimation(0);

			MyAssert(!FLTCMP(anim->GetDuration(), 3.33354211));
			MyAssert(anim->GetNumAnnotations() != 93);

			for (int cAnnot = 0; cAnnot < anim->GetNumAnnotations(); cAnnot++)
			{
				MyAssertLoop(strcmp(anim->GetAnnotation(cAnnot).get()->GetName(), boneNames[cAnnot]), cAnnot);
			}

			// Add some further testing here?

			delete hdr;
		}
}