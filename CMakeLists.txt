CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )
CMAKE_POLICY(VERSION 3.0)
project(ConvertDisplacementFieldToITKTransform)

find_package(SlicerExecutionModel REQUIRED)
include( ${SlicerExecutionModel_USE_FILE} )
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})


set(Filter ConvertDisplacementFieldToITKTransform)

SEMMacroBuildCLI(
    NAME ${Filter}
    EXECUTABLE_ONLY
    TARGET_LIBRARIES ${ITK_LIBRARIES}
)
