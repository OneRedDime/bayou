bin_PROGRAMS = bayou

AM_CXXFLAGS = "-std=c++0x"

bayou_SOURCES = Animation.cpp Body.cpp GameObject.cpp Menu.cpp Vector3.cpp \
	Arena.cpp Button.cpp Keyboard.cpp Mesh.cpp Assets.cpp Character.cpp \
	main.cpp Mouse.cpp Barrier.cpp Environment.cpp Manager.cpp \
	TitleMenu.cpp Client.cpp Server.cpp communication.cpp

bayou_LDADD = -Lusr/local/lib -lallegro_acodec \
	-lallegro_audio -lallegro_color -lallegro_dialog -lallegro_image \
	-lallegro_main -lallegro_memfile -lallegro_physfs -lallegro_primitives \
	-lallegro_ttf -lallegro_font -lallegro

CLEANFILES = bayou *.o
