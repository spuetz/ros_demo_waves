#include <ros/ros.h>
#include <ros/node_handle.h>
#include <ros/publisher.h>
#include <std_msgs/Float64.h>
#include <oscillation/OscillationConfig.h>
#include <dynamic_reconfigure/server.h>


oscillation::OscillationConfig cfg;

void dyn_conf_callback(oscillation::OscillationConfig &config, uint32_t)
{
  cfg = config;
}



int main(int argc, char** argv)
{
  ros::init(argc, argv, "oscillation_node");

  ros::NodeHandle private_nh("~");
  ros::NodeHandle nh;

  ros::Publisher osc_pub = nh.advertise<std_msgs::Float64>("oscillation", 5);

  dynamic_reconfigure::Server<oscillation::OscillationConfig> dyn_cfg_srv;
  dynamic_reconfigure::Server<oscillation::OscillationConfig>::CallbackType f;
  f = boost::bind(&dyn_conf_callback, _1, _2);
  dyn_cfg_srv.setCallback(f);

  ros::Rate r(100);

  while(ros::ok())
  {
    const double t = ros::Time::now().toSec();
    std_msgs::Float64 value;
    value.data = cfg.amplitude * sin(2 * M_PI * cfg.frequency * t + cfg.zero_phase_angle);
    osc_pub.publish(value);
    ros::spinOnce();
    r.sleep();
  }
  return EXIT_SUCCESS;

}