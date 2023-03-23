/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2022, Bielefeld University
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Bielefeld University nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Authors: David P. Leins */

#pragma once

#include <math.h>

#include <mujoco_ros/common_types.h>

#include <sensor_msgs/CameraInfo.h>
#include <geometry_msgs/TransformStamped.h>

#include <camera_info_manager/camera_info_manager.h>

#include <mujoco_ros_msgs/SetBodyState.h>
#include <std_srvs/Empty.h>
#include <tf2_ros/transform_listener.h>
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

namespace MujocoSim {
namespace rendering {

class CameraStream
{
public:
	CameraStream(const uint8_t cam_id, const std::string cam_name, const int width, const int height,
	             const streamType stream_type, const bool use_segid, const float pub_freq,
	             image_transport::ImageTransport *it, ros::NodeHandlePtr parent_nh, const MujocoSim::mjModelPtr model,
	             MujocoSim::mjDataPtr data);

	~CameraStream()
	{
		if (&rgb_pub != nullptr) {
			rgb_pub.shutdown();
		}
		if (&depth_pub != nullptr) {
			depth_pub.shutdown();
		}
		if (&segment_pub != nullptr) {
			segment_pub.shutdown();
		}
	};

	uint8_t cam_id;
	std::string cam_name;
	int width, height;
	streamType stream_type = streamType::RGB;
	bool use_segid         = true;
	float pub_freq         = 15;
	ros::Time last_pub;
	ros::Publisher camera_info_pub_;
	image_transport::Publisher rgb_pub;
	image_transport::Publisher depth_pub;
	image_transport::Publisher segment_pub;

	// mjtNum cam_optical_quat[4] = {1.0, 0.0, 0.0, 0.0};
	// geometry_msgs::TransformStamped cam_transform;
	// ros::ServiceServer quatserver;
	// ros::Publisher pub_rel_tf;
	// ros::Publisher pub_rel;
	// ros::Publisher pub_cart;

	void publishCameraInfo();
	// bool setCamQuatCB(mujoco_ros_msgs::SetBodyState::Request &req, mujoco_ros_msgs::SetBodyState::Response &resp);

private:
	boost::shared_ptr<camera_info_manager::CameraInfoManager> camera_info_manager_;

	void publishCameraInfo(ros::Publisher camera_info_publisher);
	void publishCameraInfo(ros::Time &last_update_time);

	// bool getCamTransform(std_srvs::Empty::Request &req, std_srvs::Empty::Response &resp);
};

} // end namespace rendering
} // end namespace MujocoSim