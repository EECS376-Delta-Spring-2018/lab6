# lab6

# here are the jinx commands to run
roscd exmpl_models/glennan_2nd_flr/; rosrun map_server map_server glennan_2nd_flr_model_map.yaml
rosrun amcl amcl 
rosrun mobot_pub_des_state mobot_pub_des_state
rosrun lin_steering lin_steering_wrt_amcl_and_odom drifty_odom:=odom
rosrun mobot_pub_des_state glennan_pub_des_state_path_client

# here are the baxter commands to run
roslaunch baxter_launch_files baxter_object_grabber_nodes.launch
rosrun object_grabber example_object_grabber_action_client 
