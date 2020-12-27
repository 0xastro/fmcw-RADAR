# invoke SourceDir generated makefile for dss.pe674
dss.pe674: .libraries,dss.pe674
.libraries,dss.pe674: package/cfg/dss_pe674.xdl
	$(MAKE) -f C:\Users\astro\workspace_v9_2\srr_18xx_dss/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\astro\workspace_v9_2\srr_18xx_dss/src/makefile.libs clean

