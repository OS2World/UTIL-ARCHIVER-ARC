The enclosed files should be sufficient for bringing up ARC on a Sys V R3
system. As Jon mentions, Doug Gwyn's directory routines are needed for
Sys V R2. The enclosed copy of scandir is new, as far as I can tell, and
I've removed the (unneeded) ftw.? files. Also added a rename() routine,
courtesy of Janet Walz. (And an addition from Rich Salz.)

[see comp.sources.unix, volume 9, for gwyn-dir-lib...]

Thanks again to Jon Zeeff, Janet Walz, and Rich Salz for their help.
  -- Howard Chu
	hyc@umix.cc.umich.edu
	{uunet,rutgers}!umix!hyc
