if not exist q:\install.exe goto :badmap
if not exist q:\autorun.inf goto :badmap
if not exist q:\pm??70*.id goto :badmap
goto :goodmap
:badmap
echo Q: is not mapped correctly!
goto :end
:goodmap

call get pmwpflpl.txt
attrib -r q:\install\install.pfl
makepfl32 pmwpflpl.txt q:\install\install.pfl
attrib +r q:\install\install.pfl
rem more < q:\install\install.pfl

:end

