* Модифицировать Makefile для nmopengl-nmc4, nmopengl-x86-x64
Добавить в src_proc1/common/textureTriangle.cpp макроопределение:
```
#define TEXTURE_ENABLED
```

или вызывать из nmOpenGL_fork.
```
sed -i -e 's\-std=c++11\-std=c++11 -DTEXTURE_ENABLED\g' make/nmopengl-nmc4/Makefile
sed -i -e 's\DEBUG"\DEBUG", "TEXTURE_ENABLED"\g' make/nmopengl-x86-x64/premake5.lua
```

* Пересобрать nmopengl-nmc4 и nmopengl-x86-x64
