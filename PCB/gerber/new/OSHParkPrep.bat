del *.ger
del *.xln
rename *.gtl "Top Layer.ger"
rename *.gbl "Bottom Layer.ger"
rename *.gts "Top Solder Mask.ger"
rename *.gbs "Bottom Solder Mask.ger"
rename *.gto "Top Silk Screen.ger"
rename *.gbo "Bottom Silk Screen.ger"
rename *.gbr "Board Outline.ger"
rename *.drl "Drills.xln"
7z a -tzip gerber.zip *.ger *.xln
pause