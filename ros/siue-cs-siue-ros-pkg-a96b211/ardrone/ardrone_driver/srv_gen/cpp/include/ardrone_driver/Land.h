/* Auto-generated by genmsg_cpp for file /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/srv/Land.srv */
#ifndef ARDRONE_DRIVER_SERVICE_LAND_H
#define ARDRONE_DRIVER_SERVICE_LAND_H
#include <string>
#include <vector>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/message.h"
#include "ros/time.h"

#include "ros/service_traits.h"




namespace ardrone_driver
{
template <class ContainerAllocator>
struct LandRequest_ : public ros::Message
{
  typedef LandRequest_<ContainerAllocator> Type;

  LandRequest_()
  {
  }

  LandRequest_(const ContainerAllocator& _alloc)
  {
  }


private:
  static const char* __s_getDataType_() { return "ardrone_driver/LandRequest"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROSCPP_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "d41d8cd98f00b204e9800998ecf8427e"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getServerMD5Sum_() { return "6ccdb752b565be6d250259ff3d0c7ea1"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getServerMD5Sum() { return __s_getServerMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getServerMD5Sum() const { return __s_getServerMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "\n\
"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    return size;
  }

  typedef boost::shared_ptr< ::ardrone_driver::LandRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::ardrone_driver::LandRequest_<ContainerAllocator>  const> ConstPtr;
}; // struct LandRequest
typedef  ::ardrone_driver::LandRequest_<std::allocator<void> > LandRequest;

typedef boost::shared_ptr< ::ardrone_driver::LandRequest> LandRequestPtr;
typedef boost::shared_ptr< ::ardrone_driver::LandRequest const> LandRequestConstPtr;


template <class ContainerAllocator>
struct LandResponse_ : public ros::Message
{
  typedef LandResponse_<ContainerAllocator> Type;

  LandResponse_()
  : successful(false)
  {
  }

  LandResponse_(const ContainerAllocator& _alloc)
  : successful(false)
  {
  }

  typedef uint8_t _successful_type;
  uint8_t successful;


private:
  static const char* __s_getDataType_() { return "ardrone_driver/LandResponse"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROSCPP_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "6ccdb752b565be6d250259ff3d0c7ea1"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getServerMD5Sum_() { return "6ccdb752b565be6d250259ff3d0c7ea1"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getServerMD5Sum() { return __s_getServerMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getServerMD5Sum() const { return __s_getServerMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "bool successful\n\
\n\
\n\
"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, successful);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, successful);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(successful);
    return size;
  }

  typedef boost::shared_ptr< ::ardrone_driver::LandResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::ardrone_driver::LandResponse_<ContainerAllocator>  const> ConstPtr;
}; // struct LandResponse
typedef  ::ardrone_driver::LandResponse_<std::allocator<void> > LandResponse;

typedef boost::shared_ptr< ::ardrone_driver::LandResponse> LandResponsePtr;
typedef boost::shared_ptr< ::ardrone_driver::LandResponse const> LandResponseConstPtr;

struct Land
{

typedef LandRequest Request;
typedef LandResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;
}; // struct Land
} // namespace ardrone_driver

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator>
struct MD5Sum< ::ardrone_driver::LandRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const  ::ardrone_driver::LandRequest_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::ardrone_driver::LandRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "ardrone_driver/LandRequest";
  }

  static const char* value(const  ::ardrone_driver::LandRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::ardrone_driver::LandRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "\n\
";
  }

  static const char* value(const  ::ardrone_driver::LandRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::ardrone_driver::LandRequest_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros


namespace ros
{
namespace message_traits
{
template<class ContainerAllocator>
struct MD5Sum< ::ardrone_driver::LandResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "6ccdb752b565be6d250259ff3d0c7ea1";
  }

  static const char* value(const  ::ardrone_driver::LandResponse_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x6ccdb752b565be6dULL;
  static const uint64_t static_value2 = 0x250259ff3d0c7ea1ULL;
};

template<class ContainerAllocator>
struct DataType< ::ardrone_driver::LandResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "ardrone_driver/LandResponse";
  }

  static const char* value(const  ::ardrone_driver::LandResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::ardrone_driver::LandResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "bool successful\n\
\n\
\n\
";
  }

  static const char* value(const  ::ardrone_driver::LandResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::ardrone_driver::LandResponse_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::ardrone_driver::LandRequest_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct LandRequest_
} // namespace serialization
} // namespace ros


namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::ardrone_driver::LandResponse_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.successful);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct LandResponse_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace service_traits
{
template<>
struct MD5Sum<ardrone_driver::Land> {
  static const char* value() 
  {
    return "6ccdb752b565be6d250259ff3d0c7ea1";
  }

  static const char* value(const ardrone_driver::Land&) { return value(); } 
};

template<>
struct DataType<ardrone_driver::Land> {
  static const char* value() 
  {
    return "ardrone_driver/Land";
  }

  static const char* value(const ardrone_driver::Land&) { return value(); } 
};

template<class ContainerAllocator>
struct MD5Sum<ardrone_driver::LandRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "6ccdb752b565be6d250259ff3d0c7ea1";
  }

  static const char* value(const ardrone_driver::LandRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct DataType<ardrone_driver::LandRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "ardrone_driver/Land";
  }

  static const char* value(const ardrone_driver::LandRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct MD5Sum<ardrone_driver::LandResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "6ccdb752b565be6d250259ff3d0c7ea1";
  }

  static const char* value(const ardrone_driver::LandResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct DataType<ardrone_driver::LandResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "ardrone_driver/Land";
  }

  static const char* value(const ardrone_driver::LandResponse_<ContainerAllocator> &) { return value(); } 
};

} // namespace service_traits
} // namespace ros

#endif // ARDRONE_DRIVER_SERVICE_LAND_H

