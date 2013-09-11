project(eyetracker)

cmake_minimum_required(VERSION 2.6)

# Generating Rules for Dictionary as Binary.
set(BINODIR ${PROJECT_BINARY_DIR}/CMakeFiles/mstereo.dir/src)
set(BINIDIR ${CMAKE_CURRENT_SOURCE_DIR}/src/dic)
# set(BINOBJ ${BINODIR}/smiledic.o) # (obsolete)
MACRO(ADD_BINOBJ_TARGET _file)
  STRING(REPLACE .bin .o output_name ${_file})
  set(BINOBJ ${BINOBJ} ${BINODIR}/${output_name})
  ADD_CUSTOM_COMMAND(
	OUTPUT ${BINODIR}/${output_name}
    COMMAND cd ${BINIDIR} && ${CMAKE_OBJCOPY} -I binary ${OBJCOPY_TERGET} ${_file} ${BINODIR}/${output_name}
    DEPENDS ${BINIDIR}/${_file}
    COMMENT "Generating ${BINODIR}/${output_name} from ${_file}"
	)
ENDMACRO(ADD_BINOBJ_TARGET)
#ADD_BINOBJ_TARGET(foobar.bin)

# Sources
set(srcs 
  # main
  src/utils.cpp
  )



include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  )

add_definitions(-DFOOBAR)

add_library("eyetracker_static" ${srcs})
add_library("eyetracker" SHARED ${srcs})

