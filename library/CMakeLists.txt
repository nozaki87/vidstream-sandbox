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
  src/stereo_eye_tracker.cpp
  src/face_tracker.cpp
  src/eye_tracker.cpp
  src/point_tracker.cpp
  src/utils.cpp
  # eye detection
  src/unhogpd/leye_unhog_model.c
  src/unhogpd/lmou_unhog_model.c
  src/unhogpd/lnos_unhog_model.c
  src/unhogpd/rceyeo_unhog_model.c
  src/unhogpd/reye_unhog_model.c
  src/unhogpd/rmou_unhog_model.c
  src/unhogpd/rnos_unhog_model.c
  src/unhogpd/unhog_operator.c
  src/unhogpd/unhogpd.c
  # calibration
  src/calib/calibfunctions.cpp
  src/calib/calibrator.cpp
  src/calib/stereo_calib.cpp
  )



include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/unhogpd"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/calib"
  )

add_definitions(-DFOOBAR)

add_library("eyetracker_static" ${srcs})
add_library("eyetracker" SHARED ${srcs})

