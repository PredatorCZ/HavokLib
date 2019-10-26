void TestAllosaurVariables(const TSTRING testingPath)
{
	for (auto &v : versions)
		for (auto &t : compiledFlags)
		{
			TSTRING fleName = testingPath;
			fleName.append(v);
			fleName.append(_T("allosaur"));
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
            const int kjsdavnrnaga = rcont->GetNumVariants();
			const hkxEnvironment *env = rcont->GetVariant(1);
            //for (auto g : *env)
              //  printer << g.name << "\n" << g.value >> 1;


			delete hdr;
		}
}