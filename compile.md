# Change .cpp into .dll
cl /LD uareu4500.cpp <br>
/Feuareu4500.dll <br>
/I "C:\Program Files (x86)\Windows <br> Kits\10\Include\10.0.26100.0\um" <br>
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared" <br>
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt"<br>
/I "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\atlmfc\include" <br>
/I "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\include" <br>
/I "include"<br>
/link <br>
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64" <br>
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64" <br>
/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\atlmfc\lib\x64" <br>
/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\lib\x64" <br>
/LIBPATH:"lib" 
atls.lib dpfpdd.lib dpfj.lib libcpmt.lib libucrt.lib uuid.lib

# DLL Created Result
Microsoft (R) C/C++ Optimizing Compiler Version 19.43.34808 for x64 <br>
Copyright (C) Microsoft Corporation.  All rights reserved.

uareu4500.cpp <br>
C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\include\__msvc_ostream.hpp(551): warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc <br>
C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\include\__msvc_ostream.hpp(551): <br>note: the template instantiation context (the oldest one first) is <br>
uareu4500.cpp(14): <br>note: see reference to class template instantiation 'std::basic_ofstream<char,std::char_traits<char>>' being compiled <br>   
C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\include\fstream(177): note: see reference to class template instantiation 'std::basic_ostream<char,std::char_traits<char>>' being compiled <br>
C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\include\__msvc_ostream.hpp(543): <br> note: while compiling class template member function 'std::basic_ostream<char,std::char_traits<char>> &std::basic_ostream<char,std::char_traits<char>>::write(const _Elem *,std::streamsize)' <br>
        with
        [
            _Elem=char
        ]
uareu4500.cpp(29): note: see the first reference to 'std::basic_ostream<char,std::char_traits<char>>::write' in 'SaveBitmap'
Microsoft (R) Incremental Linker Version 14.43.34808.0
Copyright (C) Microsoft Corporation.  All rights reserved.

/dll
/implib:uareu4500.lib
/out:uareu4500.dll
"/LIBPATH:C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64"
"/LIBPATH:C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64"
"/LIBPATH:C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\atlmfc\lib\x64"
"/LIBPATH:C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.43.34808\lib\x64" 
/LIBPATH:lib
atls.lib
dpfpdd.lib
dpfj.lib
libcpmt.lib
libucrt.lib
uuid.lib
uareu4500.obj
   Creating library uareu4500.lib and object uareu4500.exp


Installing to app.exe
pyinstaller --onefile -w
--add-binary "uareu4500.dll;." 
--add-data "asset/favicon.ico;asset" 
--add-data "asset/logo.png;asset" 
app.py
command: pyinstaller --onefile -w --add-binary "uareu4500.dll;." --add-data "asset/favicon.ico;asset" --add-data "asset/logo.png;asset" app.py

App builded result:
765 INFO: PyInstaller: 6.12.0, contrib hooks: 2025.1
766 INFO: Python: 3.12.8
805 INFO: Platform: Windows-11-10.0.22631-SP0
805 INFO: Python environment: C:\Users\sumito\AppData\Local\Programs\Python\Python312
948 INFO: wrote D:\FingerPrint\app.spec
1014 INFO: Module search paths (PYTHONPATH):
['C:\\Users\\sumito\\AppData\\Local\\Programs\\Python\\Python312\\Scripts\\pyinstaller.exe',
 'C:\\Users\\sumito\\AppData\\Local\\Programs\\Python\\Python312\\python312.zip',
 'C:\\Users\\sumito\\AppData\\Local\\Programs\\Python\\Python312\\DLLs',
 'C:\\Users\\sumito\\AppData\\Local\\Programs\\Python\\Python312\\Lib',
 'C:\\Users\\sumito\\AppData\\Local\\Programs\\Python\\Python312',
 'C:\\Users\\sumito\\AppData\\Roaming\\Python\\Python312\\site-packages',
 'C:\\Users\\sumito\\AppData\\Roaming\\Python\\Python312\\site-packages\\win32',
 'C:\\Users\\sumito\\AppData\\Roaming\\Python\\Python312\\site-packages\\win32\\lib',
 'C:\\Users\\sumito\\AppData\\Roaming\\Python\\Python312\\site-packages\\Pythonwin',
 'C:\\Users\\sumito\\AppData\\Local\\Programs\\Python\\Python312\\Lib\\site-packages',
 'D:\\FingerPrint']
pygame 2.5.2 (SDL 2.28.3, Python 3.12.8)
Hello from the pygame community. https://www.pygame.org/contribute.html
4015 INFO: Appending 'binaries' from .spec
4069 INFO: Appending 'datas' from .spec
4070 INFO: checking Analysis
4608 INFO: checking PYZ
4747 INFO: checking PKG
4857 INFO: Bootloader C:\Users\sumito\AppData\Local\Programs\Python\Python312\Lib\site-packages\PyInstaller\bootloader\Windows-64bit-intel\runw.exe
4858 INFO: checking EXE
4946 INFO: Build complete! The results are available in: D:\FingerPrint\dist

