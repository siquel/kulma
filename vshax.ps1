param (
    [Parameter(Mandatory=$true)][string]$toolset
 )

Get-ChildItem .build/projects/$toolset -Filter *.vcxproj |
Foreach-Object {
	$content = Get-Content $_.FullName 
	$content -replace '<WarningLevel>Level3', '<WarningLevel>Level4' | Out-File -encoding UTF8 $_.FullName
}

(gc .build/projects/$toolset/unit_test.vcxproj) -replace '<ExceptionHandling>false', '<ExceptionHandling>Sync' | Out-File -encoding UTF8 .build/projects/$toolset/unit_test.vcxproj
(gc .build/projects/$toolset/unit_test.vcxproj) -replace '<RuntimeTypeInfo>false', '<RuntimeTypeInfo>true' | Out-File -encoding UTF8 .build/projects/$toolset/unit_test.vcxproj