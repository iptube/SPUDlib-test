#
#
# Sample programs


add_executable ( testing testing.c )
target_link_libraries ( testing spud cn-cbor pthread )

set_target_properties(testing PROPERTIES INSTALL_RPATH "${install_dir}/lib")
set_target_properties(testing PROPERTIES BUILD_WITH_INSTALL_RPATH ON)

add_definitions(-DUSE_CBOR_CONTEXT)
