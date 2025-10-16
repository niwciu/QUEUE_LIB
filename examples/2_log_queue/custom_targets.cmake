# PROJECT CUSTOM TARGETS FILE
#  here you can define custom targets for the project so all team member can use it in the same way
#  some example of custo targets are shown bello those are targets for:
# 		1. Running example


#TARGETS FOR RUNNING UNIT TESTS
message(STATUS "To run example, you can use predefine target: \r\n\trun,")
add_custom_target(run log_queue_example)

