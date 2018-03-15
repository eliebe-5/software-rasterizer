#--------------------------------------------------------------------------
# matlib project
#--------------------------------------------------------------------------

PROJECT(matlib)
FILE(GLOB matlib_headers code/*.h)

SET(files_matlib ${matlib_headers} ${matlib_sources})
SOURCE_GROUP("matlib" FILES ${files_matlib})

ADD_EXECUTABLE(matlib ${files_matlib})
TARGET_LINK_LIBRARIES(matlib core render)
ADD_DEPENDENCIES(matlib core render)