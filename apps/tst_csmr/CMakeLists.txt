project(CFE_TST_CSMR_APP C)

# Create the app module
add_cfe_app(tst_csmr_app fsw/src/tst_csmr_app.c)



# Add table
add_cfe_tables(tst_csmr_app fsw/tables/tst_csmr_app_tbl.c)

target_include_directories(tst_csmr_app PUBLIC
  fsw/mission_inc
  fsw/platform_inc
)

add_cfe_app_dependency(tst_csmr_app tst_prod_app)


# If UT is enabled, then add the tests from the subdirectory
# Note that this is an app, and therefore does not provide
# stub functions, as other entities would not typically make
# direct function calls into this application.
if (ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)
