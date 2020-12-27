# invoke SourceDir generated makefile for mss.per4f
mss.per4f: .libraries,mss.per4f
.libraries,mss.per4f: package/cfg/mss_per4f.xdl
	$(MAKE) -f C:\Users\astro\workspace_v9_2\srr_18xx_mss/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\astro\workspace_v9_2\srr_18xx_mss/src/makefile.libs clean

