#include <ros/ros.h>
#include <mobot_pub_des_state/path.h>
#include <iostream>
#include <string>
#include <nav_msgs/Path.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <object_grabber/object_grabberAction.h>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <xform_utils/xform_utils.h>
#include <object_manipulation_properties/object_ID_codes.h>
using namespace std;
bool g_got_callback= false;
actionlib::SimpleActionClient<object_grabber::object_grabberAction> *g_object_grabber_ac_ptr;
geometry_msgs::PoseStamped getSwipePose(){
    //pose that swipes the object
    geometry_msgs::PoseStamped swipe;
    swipe.header.frame_id = "torso";
    swipe.pose.position.x = 0.65;
    swipe.pose.position.y = -0.35;
    swipe.pose.position.z = -0.3;
    swipe.pose.orientation.x = 0.707;
    swipe.pose.orientation.y = 0.707;
    swipe.pose.orientation.z = 0;
    swipe.pose.orientation.w = 0;
    swipe.header.stamp = ros::Time::now();
    
    return swipe;
}
int g_object_grabber_return_code;
void objectGrabberDoneCb(const actionlib::SimpleClientGoalState& state,
        const object_grabber::object_grabberResultConstPtr& result) {
    ROS_INFO(" objectGrabberDoneCb: server responded with state [%s]", state.toString().c_str());
    g_object_grabber_return_code = result->return_code;
    ROS_INFO("got result output = %d; ", g_object_grabber_return_code);
    g_got_callback=true; //flag that action server has called back
}
void move_to_waiting_pose() {
        ROS_INFO("sending command to move to waiting pose");
        g_got_callback=false; //reset callback-done flag
        object_grabber::object_grabberGoal object_grabber_goal;
        object_grabber_goal.action_code = object_grabber::object_grabberGoal::MOVE_TO_WAITING_POSE;
        g_object_grabber_ac_ptr->sendGoal(object_grabber_goal, &objectGrabberDoneCb);
}
int main(int argc, char** argv) {
    ros::init(argc, argv, "baxter_swipe_client");
    ros::NodeHandle nh;
    //geometry_msgs::PoseStamped object_standard_poseStamped;
    geometry_msgs::PoseStamped object_swipe_poseStamped = getSwipePose();
    
    //specify object pick-up and drop-off frames using simple test fnc
    //more generally, pick-up comes from perception and drop-off comes from task
    //set_example_object_frames(object_standard_poseStamped, object_swipe_poseStamped);
    //instantiate an action client of object_grabber_action_service:
    //change this
    actionlib::SimpleActionClient<object_grabber::object_grabberAction> object_grabber_ac("object_grabber_action_service", true);
    g_object_grabber_ac_ptr = &object_grabber_ac; // make available to fncs
    ROS_INFO("waiting for server: ");
    bool server_exists = false;
    while ((!server_exists)&&(ros::ok())) {
        server_exists = object_grabber_ac.waitForServer(ros::Duration(0.5)); // 
        ros::spinOnce();
        ros::Duration(0.5).sleep();
        ROS_INFO("retrying...");
    }
    ROS_INFO("connected to mobot_pub_des_state action server"); // if here, then we connected to the server; 

    //move to waiting pose
    move_to_waiting_pose();
    while(!g_got_callback) {
        ROS_INFO("waiting on move...");
        ros::Duration(0.5).sleep(); //could do something useful
    }

    //if not waiting then should move -- this should only be done when the robot is at the end of the hallway
    if(g_got_callback){
        //move to swipe mode kappa
        ROS_INFO("time to hit object...");
    }

    return 0;
}
