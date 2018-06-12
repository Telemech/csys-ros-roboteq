/**
Software License Agreement (BSD)

\file      channel.h
\authors   Mike Purvis <mpurvis@clearpathrobotics.com>
\copyright Copyright (c) 2013, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
   disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef ROBOTEQ_CHANNEL
#define ROBOTEQ_CHANNEL

#include "ros/ros.h"

namespace roboteq_msgs {
  ROS_DECLARE_MESSAGE(Command);
  ROS_DECLARE_MESSAGE(Feedback);
}

namespace roboteq {

class Controller;

class Channel {
public:
  Channel(int channel_num, std::string ns, Controller* controller);
  void feedbackCallback(std::vector<std::string>);

protected:
  /**
   * @param x Velocity in RPM (after gearbox).
   * @return Encoder velocity in RPM.
   */
  double to_encoder_rpm(double x)
  {
    return x * gear_ratio_;
  }

  /**
   * @param x Encoder velocity in RPM.
   * @return Velocity in RPM (after gearbox).
   */
  double from_encoder_rpm(double x)
  {
    return x / gear_ratio_;
  }

  /**
   * Conversion of revolutions to encoder ticks. Note that this assumes a
   * quadrature encoder.
   *
   * @param x Angular position in revolutions.
   * @return Angular position in encoder ticks.
   */
  double to_encoder_ticks(double x)
  {
    return x * (gear_ratio_ * encoder_ppr_ * 4);
  }

  /**
   * Conversion of encoder ticks to revolutions. Note that this assumes a
   * quadrature encoder.
   *
   * @param x Angular position in encoder ticks.
   * @return Angular position in revolutions.
   */
  double from_encoder_ticks(double x)
  {
    return x / (gear_ratio_ * encoder_ppr_ * 4);
  }

  void cmdCallback(const roboteq_msgs::Command&);
  void timeoutCallback(const ros::TimerEvent&);

  ros::NodeHandle nh_;
  boost::shared_ptr<Controller> controller_;
  int channel_num_;
  int max_rpm_;
  int encoder_ppr_;
  float gear_ratio_;

  ros::Subscriber sub_cmd_;
  ros::Publisher pub_feedback_;
  ros::Timer timeout_timer_;

  ros::Time last_feedback_time_;
  uint8_t last_mode_;
};

}

#endif
