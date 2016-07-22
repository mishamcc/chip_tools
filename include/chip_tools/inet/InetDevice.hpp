#ifndef CHIP_TOOLS_INET_DEVICE_HPP_
#define CHIP_TOOLS_INET_DEVICE_HPP_

#include <chip_tools/basic/Device.hpp>

#ifdef _WIN32
#   include <Winsock2.h>
#   include <Windows.h>
#   pragma comment(lib,"Ws2_32.lib")
#   define wsa_startup()                                 \
    if (true)                                            \
    {                                                    \
       WSADATA wsa_data;                                 \
       if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)   \
       {                                                 \
          this->fatal("Couldn't init socket");           \
          return Errcode::couldnt_connect;               \
       }                                                 \
    }                                                    \
    else                                                 \
       void(0)

#   define wsa_cleanup() WSACleanup()
#else
#   include <sys/socket.h>
#   include <sys/types.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   define wsa_startup()
#   define wsa_cleanup()
#endif

namespace chip_tools
{

namespace inet
{

template<const std::string &name,
         class... Successors>
class InetDevice : public Device<name, InetDevice<name, Successors...>, Successors...>
{
public:
   explicit InetDevice(std::ostream &output = std::cout) :
      Device<name, InetDevice<name, Successors...>, Successors...>(output) {}

   Errcode initialize(boost::property_tree::ptree *node);
   Errcode connect();
   Errcode disconnect();
   Errcode write_data(const std::string &data, size_t *actually_send = nullptr);
   Errcode read_data(std::string &data);

private:
   std::string ip_address_ = "127.0.0.1";
   unsigned short port_ = 3128;
   int socket_ = 0;

}; // class InetDevice

template<const std::string &name,
         class... Successors>
Errcode InetDevice<name, Successors...>::initialize(boost::property_tree::ptree *node)
{
   boost::optional<std::string> ip_address = node->get_optional<std::string>("Address");
   if (ip_address)
   {
      ip_address_ = ip_address.get();
      boost::optional<unsigned short> port = node->get_optional<unsigned short>("Port");
      if (port)
         port_ = port.get();
   }
   return Errcode::success;
}

template<const std::string &name,
         class... Successors>
Errcode InetDevice<name, Successors...>::connect()
{
   if (this->is_using())
   {
#ifdef CHIP_TOOLS_OFFLINE
      return Errcode::success;
#endif
      wsa_startup();
      sockaddr_in address;
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = inet_addr(ip_address_.c_str());
      address.sin_port = htons(port_);
      socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if (socket_ > 0)
      {
         if (::connect(socket_, (sockaddr *)&address, sizeof(sockaddr)) == 0)
            return Errcode::success;
      }
      this->fatal(print_error<Errcode::couldnt_connect>());
      return Errcode::couldnt_connect;
   }
   return Errcode::success;
}

template<const std::string &name,
         class... Successors>
Errcode InetDevice<name, Successors...>::disconnect()
{
   if (this->is_using())
   {
#ifdef CHIP_TOOLS_OFFLINE
      return Errcode::success;
#endif
#ifdef _WIN32
      closesocket(socket_);
#else
      close(socket_);
#endif
      wsa_cleanup();
   }
   return Errcode::success;
}

template<const std::string &name,
         class... Successors>
Errcode InetDevice<name, Successors...>::write_data(const std::string &data, size_t *actually_send)
{
   if (this->used())
   {
#ifdef CHIP_TOOLS_OFFLINE
      if (actually_send)
         *actually_send = data.size();

      return Errcode::success;
#endif
      assert(socket_ != 0);
      size_t sended = ::send(socket_, data.c_str(), data.size() + 1, 0);
      if (actually_send)
         *actually_send = sended;

      if (sended > 0)
         return Errcode::success;
      else
      {
         this->error(print_error<Errcode::device_io_failed>());
         return Errcode::device_io_failed;
      }
   }
   return Errcode::success;
}

template<const std::string &name,
         class... Successors>
Errcode InetDevice<name, Successors...>::read_data(std::string &data)
{
   if (this->used())
   {
#ifdef CHIP_TOOLS_OFFLINE
      return Errcode::success;
#endif
      assert(socket_ != 0);
      const size_t max_length = 1024;
      char recv_data[max_length];
      size_t reply_length = ::recv(socket_, recv_data, max_length, 0);
      if (reply_length > 0)
      {
         std::copy(recv_data, recv_data + reply_length, std::back_inserter(data));
         return Errcode::success;
      }
      this->error(print_error<Errcode::device_io_failed>());
      return Errcode::device_io_failed;
   }
   return Errcode::success;
}

} // namespace inet

} // namespace chip_tools

#endif // #ifndef MCOLOR19_INET_DEVICE_HPP_
