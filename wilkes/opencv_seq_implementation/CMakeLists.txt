wilkes_define_library(opencv_seq_implementation
  HEADERS
    CannyEdgeDetector.h
  SOURCES
    CannyEdgeDetector.cpp
  DEPENDENCIES
    opencv::opencv
)

wilkes_define_executable(opencv_seq
  SOURCES
    main.cpp
  DEPENDENCIES
    wilkes::opencv_seq_implementation
)


