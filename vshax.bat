powershell -Command "(gc .build/projects/%1/unit_test.vcxproj) -replace '<ExceptionHandling>false', '<ExceptionHandling>Sync' | Out-File -encoding UTF8 .build/projects/%1/unit_test.vcxproj"
powershell -Command "(gc .build/projects/%1/unit_test.vcxproj) -replace '<RuntimeTypeInfo>false', '<RuntimeTypeInfo>true' | Out-File -encoding UTF8 .build/projects/%1/unit_test.vcxproj"