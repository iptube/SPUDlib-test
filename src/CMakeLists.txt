#
#
# Sample programs


add_executable ( testing testing.c )
target_link_libraries ( testing spud cn-cbor pthread )

add_definitions(-DUSE_CBOR_CONTEXT)
