#ifndef CHIP_TOOLS_SERIAL_DEVICE_HPP_
#define CHIP_TOOLS_SERIAL_DEVICE_HPP_

#include <chip_tools/basic/Device.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

namespace chip_tools
{

namespace serial
{

template<const std::string &name,
         class... Successors>
class SerialDevice : public Device<name, SerialDevice<name, Successors...>, Successors...>
{
public:
   explicit SerialDevice(std::ostream &output = std::cout) :
      Device<name, SerialDevice<name, Successors...>, Successors...>(output)
   {
      memset(&tio_, 0, sizeof(tio_));
   }

   Errcode initialize(const boost::property_tree::ptree &node);
   Errcode connect();
   Errcode disconnect();
   Errcode write_data(const std::string &data, size_t *actually_send = nullptr);
   Errcode read_data(std::string &data);

private:
   std::string tty_;
   int tty_fd_;
   struct termios tio_;

   enum {max_string_size = 8192};
   char retval_[max_string_size];
   boost::chrono::milliseconds delay_;
}; // class SerialDevice

template<const std::string &name,
         class... Successors>
Errcode SerialDevice<name, Successors...>::initialize(const boost::property_tree::ptree &node)
{
   tty_ = node.get<std::string>("tty", "tty0");
   delay_ = boost::chrono::milliseconds(node.get<unsigned int>("Delay", 300));
   return Errcode::success;
}

template<const std::string &name,
         class... Successors>
Errcode SerialDevice<name, Successors...>::connect()
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   tio_.c_iflag = 0;
   tio_.c_oflag = 0;
   tio_.c_cflag = CS8|CREAD|CLOCAL;
   tio_.c_lflag = 0;
   tio_.c_cc[VMIN] = 1;
   tio_.c_cc[VTIME] = 5;

   tty_fd_ = open(tty_.c_str(), O_RDWR);
   if (tty_fd_ > 0)
   {
      cfsetospeed(&tio_, B19200);
      boost::this_thread::sleep_for(delay_);
      cfsetispeed(&tio_, B19200);
      boost::this_thread::sleep_for(delay_);
      tcsetattr(tty_fd_, TCSANOW, &tio_);
      return Errcode::success;
   }
   return Errcode::couldnt_connect;
}

template<const std::string &name,
         class... Successors>
Errcode SerialDevice<name, Successors...>::disconnect()
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   if (tty_fd_ > 0)
      close(tty_fd_);
   return Errcode::success;
}

template<const std::string &name,
         class... Successors>
Errcode SerialDevice<name, Successors...>::write_data(const std::string &data, size_t *actually_send = nullptr)
{
#ifdef CHIP_TOOLS_OFFLINE
   if (actually_send)
      *actually_send = data.size();

   return Errcode::success;
#endif
   auto bytes_send = write(tty_fd_, data.c_str(), data.size());
   boost::this_thread::sleep_for(delay_);
   if (bytes_send > 0)
   {
      if (actually_send)
         *actually_send = bytes_send;

      return Errcode::success;
   }
   if (actually_send)
      *actually_send = 0;

   return Errcode::device_io_failed;
}

template<const std::string &name,
         class... Successors>
Errcode SerialDevice<name, Successors...>::read_data(std::string &data)
{
#ifdef CHIP_TOOLS_OFFLINE
   data = "";
   return Errcode::success;
#endif
   data.clear();
   auto bytes_read = read(tty_fd_, retval_, max_string_size);
   boost::this_thread::sleep_for(delay_);
   if (bytes_read > 0)
   {
      std::copy(retval_, retval_ + bytes_read, std::back_inserter(data));
      return Errcode::success;
   }
   return Errcode::device_io_failed;
}

} // namespace serial

} // namespace chip_tools

#endif // CHIP_TOOLS_SERIAL_DEVICE_HPP_
