/* Auto-generated by genmsg_cpp for file /home/xps/ros_pkgs/siue-ros-pkg/siue_utils/serial_port_node/srv/SerialWrite.srv */
#ifndef SERIAL_PORT_NODE_SERVICE_SERIALWRITE_H
#define SERIAL_PORT_NODE_SERVICE_SERIALWRITE_H
#include <string>
#include <vector>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/message.h"
#include "ros/time.h"

#include "ros/service_traits.h"




namespace serial_port_node
{
template <class ContainerAllocator>
struct SerialWriteRequest_ : public ros::Message
{
  typedef SerialWriteRequest_<ContainerAllocator> Type;

  SerialWriteRequest_()
  : data()
  , length(0)
  {
  }

  SerialWriteRequest_(const ContainerAllocator& _alloc)
  : data(_alloc)
  , length(0)
  {
  }

  typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _data_type;
  std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  data;

  typedef int16_t _length_type;
  int16_t length;


private:
  static const char* __s_getDataType_() { return "serial_port_node/SerialWriteRequest"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROSCPP_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "a9645de454fb3b94d33cdbf604ffc2ef"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getServerMD5Sum_() { return "9ddbb730a027ebe9b37465e267e69b16"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getServerMD5Sum() { return __s_getServerMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getServerMD5Sum() const { return __s_getServerMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "string data\n\
int16 length\n\
\n\
"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, data);
    ros::serialization::serialize(stream, length);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, data);
    ros::serialization::deserialize(stream, length);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(data);
    size += ros::serialization::serializationLength(length);
    return size;
  }

  typedef boost::shared_ptr< ::serial_port_node::SerialWriteRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::serial_port_node::SerialWriteRequest_<ContainerAllocator>  const> ConstPtr;
}; // struct SerialWriteRequest
typedef  ::serial_port_node::SerialWriteRequest_<std::allocator<void> > SerialWriteRequest;

typedef boost::shared_ptr< ::serial_port_node::SerialWriteRequest> SerialWriteRequestPtr;
typedef boost::shared_ptr< ::serial_port_node::SerialWriteRequest const> SerialWriteRequestConstPtr;


template <class ContainerAllocator>
struct SerialWriteResponse_ : public ros::Message
{
  typedef SerialWriteResponse_<ContainerAllocator> Type;

  SerialWriteResponse_()
  : bytesWritten(0)
  , status(false)
  {
  }

  SerialWriteResponse_(const ContainerAllocator& _alloc)
  : bytesWritten(0)
  , status(false)
  {
  }

  typedef int16_t _bytesWritten_type;
  int16_t bytesWritten;

  typedef uint8_t _status_type;
  uint8_t status;


private:
  static const char* __s_getDataType_() { return "serial_port_node/SerialWriteResponse"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROSCPP_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "f3674d18d7164751694db2909529a25f"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getServerMD5Sum_() { return "9ddbb730a027ebe9b37465e267e69b16"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getServerMD5Sum() { return __s_getServerMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getServerMD5Sum() const { return __s_getServerMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "int16 bytesWritten\n\
bool status\n\
\n\
\n\
"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, bytesWritten);
    ros::serialization::serialize(stream, status);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, bytesWritten);
    ros::serialization::deserialize(stream, status);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(bytesWritten);
    size += ros::serialization::serializationLength(status);
    return size;
  }

  typedef boost::shared_ptr< ::serial_port_node::SerialWriteResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::serial_port_node::SerialWriteResponse_<ContainerAllocator>  const> ConstPtr;
}; // struct SerialWriteResponse
typedef  ::serial_port_node::SerialWriteResponse_<std::allocator<void> > SerialWriteResponse;

typedef boost::shared_ptr< ::serial_port_node::SerialWriteResponse> SerialWriteResponsePtr;
typedef boost::shared_ptr< ::serial_port_node::SerialWriteResponse const> SerialWriteResponseConstPtr;

struct SerialWrite
{

typedef SerialWriteRequest Request;
typedef SerialWriteResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;
}; // struct SerialWrite
} // namespace serial_port_node

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator>
struct MD5Sum< ::serial_port_node::SerialWriteRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "a9645de454fb3b94d33cdbf604ffc2ef";
  }

  static const char* value(const  ::serial_port_node::SerialWriteRequest_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xa9645de454fb3b94ULL;
  static const uint64_t static_value2 = 0xd33cdbf604ffc2efULL;
};

template<class ContainerAllocator>
struct DataType< ::serial_port_node::SerialWriteRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "serial_port_node/SerialWriteRequest";
  }

  static const char* value(const  ::serial_port_node::SerialWriteRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::serial_port_node::SerialWriteRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "string data\n\
int16 length\n\
\n\
";
  }

  static const char* value(const  ::serial_port_node::SerialWriteRequest_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros


namespace ros
{
namespace message_traits
{
template<class ContainerAllocator>
struct MD5Sum< ::serial_port_node::SerialWriteResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "f3674d18d7164751694db2909529a25f";
  }

  static const char* value(const  ::serial_port_node::SerialWriteResponse_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xf3674d18d7164751ULL;
  static const uint64_t static_value2 = 0x694db2909529a25fULL;
};

template<class ContainerAllocator>
struct DataType< ::serial_port_node::SerialWriteResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "serial_port_node/SerialWriteResponse";
  }

  static const char* value(const  ::serial_port_node::SerialWriteResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::serial_port_node::SerialWriteResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "int16 bytesWritten\n\
bool status\n\
\n\
\n\
";
  }

  static const char* value(const  ::serial_port_node::SerialWriteResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::serial_port_node::SerialWriteResponse_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::serial_port_node::SerialWriteRequest_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.data);
    stream.next(m.length);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct SerialWriteRequest_
} // namespace serialization
} // namespace ros


namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::serial_port_node::SerialWriteResponse_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.bytesWritten);
    stream.next(m.status);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct SerialWriteResponse_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace service_traits
{
template<>
struct MD5Sum<serial_port_node::SerialWrite> {
  static const char* value() 
  {
    return "9ddbb730a027ebe9b37465e267e69b16";
  }

  static const char* value(const serial_port_node::SerialWrite&) { return value(); } 
};

template<>
struct DataType<serial_port_node::SerialWrite> {
  static const char* value() 
  {
    return "serial_port_node/SerialWrite";
  }

  static const char* value(const serial_port_node::SerialWrite&) { return value(); } 
};

template<class ContainerAllocator>
struct MD5Sum<serial_port_node::SerialWriteRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "9ddbb730a027ebe9b37465e267e69b16";
  }

  static const char* value(const serial_port_node::SerialWriteRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct DataType<serial_port_node::SerialWriteRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "serial_port_node/SerialWrite";
  }

  static const char* value(const serial_port_node::SerialWriteRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct MD5Sum<serial_port_node::SerialWriteResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "9ddbb730a027ebe9b37465e267e69b16";
  }

  static const char* value(const serial_port_node::SerialWriteResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct DataType<serial_port_node::SerialWriteResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "serial_port_node/SerialWrite";
  }

  static const char* value(const serial_port_node::SerialWriteResponse_<ContainerAllocator> &) { return value(); } 
};

} // namespace service_traits
} // namespace ros

#endif // SERIAL_PORT_NODE_SERVICE_SERIALWRITE_H

