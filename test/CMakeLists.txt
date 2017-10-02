
FILE(GLOB TESTS test*)

SET(EXT_LIB
	ws2_32
	WeSocket
)

FOREACH(TEST ${TESTS})
     add_subdirectory (${TEST})
ENDFOREACH(TEST)