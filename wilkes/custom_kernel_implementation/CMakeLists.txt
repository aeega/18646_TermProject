# This defines a library which will end up on disk as
# custom_kernel_implementation.lib or custom_kernel_implementation.dll
# it also creates a cmake target called wilkes::custom_kernel_implementation
# It will install itself in the subdirectory wilkes/custom_kernel_implementation
# The dependencies listed in the DEPENDENCIES section are transitive, anything
# that includes this library as a dependency will get those dependencies
# added.
wilkes_define_library(custom_kernel_implementation
  USE_OPENMP
  HEADERS
    ced_stages345.h
    conv.h
    conv2.h
    conv_kernel_2_stages.h
    get_img_rc.h
    globals.h
  SOURCES
    ced_stages345.c
    conv.c
    conv2.c
    conv_kernel_2_stages.c
    get_img_rc.c
    globals.c
)

wilkes_define_executable(conv_kernel_2_stages
  SOURCES
    main.c
  DEPENDENCIES
    wilkes::custom_kernel_implementation
)

wilkes_define_executable(dummy
  SOURCES
    dummy.c
  DEPENDENCIES
    wilkes::custom_kernel_implementation
)

wilkes_define_executable(image_read
  SOURCES 
    image_read.cpp
  DEPENDENCIES
    opencv::opencv
)

wilkes_define_executable(image_write
  SOURCES 
    image_write.cpp
  DEPENDENCIES
    opencv::opencv
)

