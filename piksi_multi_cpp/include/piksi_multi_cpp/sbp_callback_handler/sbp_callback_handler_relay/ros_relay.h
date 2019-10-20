#ifndef PIKSI_MULTI_CPP_SBP_CALLBACK_HANDLER_SBP_CALLBACK_HANDLER_RELAY_ROS_RELAY_H_
#define PIKSI_MULTI_CPP_SBP_CALLBACK_HANDLER_SBP_CALLBACK_HANDLER_RELAY_ROS_RELAY_H_

#include <libsbp_ros_msgs/ros_conversion.h>
#include <optional>
#include "piksi_multi_cpp/sbp_callback_handler/sbp_callback_handler_relay/ros_time_handler.h"
#include "piksi_multi_cpp/sbp_callback_handler/sbp_callback_handler_relay/sbp_callback_handler_relay.h"

namespace piksi_multi_cpp {

// A base relay to publish ROS messages which have a header.
template <class SbpMsgType, class RosMsgType>
class RosRelay : public SBPCallbackHandlerRelay<SbpMsgType, RosMsgType> {
 public:
  inline RosRelay(const ros::NodeHandle& nh, const uint16_t sbp_msg_type,
                  const std::shared_ptr<sbp_state_t>& state,
                  const std::string& topic,
                  const RosTimeHandler::Ptr& ros_time_handler,
                  const std::string& frame_id)
      : SBPCallbackHandlerRelay<SbpMsgType, RosMsgType>(nh, sbp_msg_type, state,
                                                        "ros/" + topic),
        ros_time_handler_(ros_time_handler),
        frame_id_(frame_id) {}

 private:
  // Implement this method to convert the message to the desired output type.
  virtual void convertSbpMsgToRosMsg(const SbpMsgType& sbp_msg,
                                     const uint8_t len,
                                     RosMsgType* ros_msg) = 0;

  inline RosMsgType convertSbpToRos(const SbpMsgType& sbp_msg,
                                    const uint8_t len) override {
    RosMsgType ros_msg;

    // Assign time stamp.
    if (ros_time_handler_.get())
      ros_msg.header.stamp = ros_time_handler_->lookupTime(sbp_msg.tow);
    else {
      ROS_ERROR(
          "No time handler set. Using ros::Time::now() to stamp navigation "
          "data.");
      ros_msg.header.stamp = ros::Time::now();
    }

    // Set frame id.
    ros_msg.header.frame_id = frame_id_;

    // Manual conversion.
    convertSbpMsgToRosMsg(sbp_msg, len, &ros_msg);
    return ros_msg;
  }

  // void utcTimeCallback(const msg_utc_time_t& msg);
  //
  // std::optional<SBPLambdaCallbackHandler<msg_utc_time_t>> utc_time_listener_;
  // std::optional<std::pair<uint32_t tow, ros::Time>> utc_time_;

  RosTimeHandler::Ptr ros_time_handler_;
  std::string frame_id_;
};
}  // namespace piksi_multi_cpp

#endif  // PIKSI_MULTI_CPP_SBP_CALLBACK_HANDLER_SBP_CALLBACK_HANDLER_RELAY_ROS_RELAY_H_
