###########################################################
#
# BLINKER_APP mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the BLINKER_APP configuration
set(BLINKER_APP_MISSION_CONFIG_FILE_LIST
  blinker_app_fcncodes.h
  blinker_app_interface_cfg.h
  blinker_app_mission_cfg.h
  blinker_app_perfids.h
  blinker_app_msg.h
  blinker_app_msgdefs.h
  blinker_app_msgstruct.h
  blinker_app_tbl.h
  blinker_app_tbldefs.h
  blinker_app_tblstruct.h
  blinker_app_topicids.h
)

if (CFE_EDS_ENABLED_BUILD)

  # In an EDS-based build, these files come generated from the EDS tool
  set(BLINKER_APP_CFGFILE_SRC_blinker_app_interface_cfg "blinker_app_eds_designparameters.h")
  set(BLINKER_APP_CFGFILE_SRC_blinker_app_tbldefs       "blinker_app_eds_typedefs.h")
  set(BLINKER_APP_CFGFILE_SRC_blinker_app_tblstruct     "blinker_app_eds_typedefs.h")
  set(BLINKER_APP_CFGFILE_SRC_blinker_app_msgdefs       "blinker_app_eds_typedefs.h")
  set(BLINKER_APP_CFGFILE_SRC_blinker_app_msgstruct     "blinker_app_eds_typedefs.h")
  set(BLINKER_APP_CFGFILE_SRC_blinker_app_fcncodes      "blinker_app_eds_cc.h")

endif(CFE_EDS_ENABLED_BUILD)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(BLINKER_APP_CFGFILE ${BLINKER_APP_MISSION_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${BLINKER_APP_CFGFILE}" NAME_WE)
  if (DEFINED BLINKER_APP_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${BLINKER_APP_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${BLINKER_APP_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${BLINKER_APP_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
