#ifndef CHIP_TOOLS_VISADEVICE_HPP_
#define CHIP_TOOLS_VISADEVICE_HPP_

#include <chip_tools/basic/Device.hpp>
#include <thread>
#include <chrono>
#include <visa.h>

namespace chip_tools
{

namespace visa
{

template<const std::string &name,
         class... Successors>
class VisaDevice : public Device<name, VisaDevice<name, Successors...>, Successors...>
{
public:
   explicit VisaDevice(std::ostream &output = std::cout) :
      Device<name, VisaDevice<name, Successors...>, Successors...>(output) {}

   Errcode initialize(boost::property_tree::ptree *node);
   Errcode connect();
   Errcode disconnect();
   Errcode reset();

protected:
   ViSession get_session() const {return visa_device_;}

   Errcode write_data(const std::string &data, size_t *actually_send = nullptr);
   Errcode read_data(std::string &data);

private:

   ViSession default_rm_;
   ViSession visa_device_;
   std::string connect_command_;
   unsigned int delay_;
   unsigned int timeout_ = 2000;
}; // class VisaDevice : public Device

template<const std::string &name,
         class... Successors>
Errcode VisaDevice<name, Successors...>::initialize(boost::property_tree::ptree *node)
{
   boost::optional<std::string> connect_command = node->get_optional<std::string>("Connect");
   if (connect_command)
   {
      connect_command_ = connect_command.get();
      boost::optional<unsigned int> delay = node->get_optional<unsigned int>("Delay");
      if (delay)
         delay_ = delay.get();

      const int default_timeout = 2000;
      timeout_ = node->get<unsigned int>("Timeout", default_timeout);
      return Errcode::success;
   }
   constexpr auto result = Errcode::wrong_file_format;
   this->fatal(err_to_str(result));
   return result;
}

template<const std::string &name,
         class... Successors>
Errcode VisaDevice<name, Successors...>::connect()
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   if (!this->is_using())
      return Errcode::success;
   if (viOpenDefaultRM(&default_rm_) == VI_SUCCESS)
   {
      constexpr size_t max_command_length = 1024;
      char command[max_command_length];
      strcpy(command, connect_command_.c_str());
      if (viOpen(default_rm_, command, VI_NULL, VI_NULL, &visa_device_) == VI_SUCCESS)
      {
         if (viSetAttribute(get_session(), VI_ATTR_TMO_VALUE, timeout_) != VI_SUCCESS)
            return Errcode::device_io_failed;
         return reset();
      }
      this->fatal(print_error<Errcode::couldnt_connect>());
   }
   else
      this->fatal("failed open default resource manager");
   return Errcode::couldnt_connect;
}

template<const std::string &name,
         class... Successors>
Errcode VisaDevice<name, Successors...>::disconnect()
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   if (!this->is_using())
      return Errcode::success;

   if (viClose(visa_device_) == VI_SUCCESS)
      if (viClose(default_rm_) == VI_SUCCESS)
         return Errcode::success;

   this->fatal(err_to_str(Errcode::device_io_failed));
   return Errcode::device_io_failed;
}

template<const std::string &name,
         class... Successors>
Errcode VisaDevice<name, Successors...>::reset()
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   if (!this->is_using())
      return Errcode::success;

   if (viPrintf(visa_device_, (ViString)"*RST\n") == VI_SUCCESS)
      return Errcode::success;

   this->fatal(err_to_str(Errcode::device_io_failed));
   return Errcode::device_io_failed;
}

template<const std::string &name,
         class... Successors>
Errcode VisaDevice<name, Successors...>::write_data(const std::string &data, size_t *actually_send)
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   if (this->is_using())
      return Errcode::success;

   constexpr size_t MAX_STRING_LENGTH = 64;
   char const_char_to_char_command[MAX_STRING_LENGTH];

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable:4996)
#endif // #ifdef _MSC_VER

   strcpy(const_char_to_char_command, data.c_str());

#ifdef _MSC_VER
#  pragma warning(pop)
#endif // #ifdef _MSC_VER

   if (viPrintf(get_session(), const_char_to_char_command,
      data.length()) != VI_SUCCESS)
   {
      this->fatal(print_error<Errcode::device_io_failed>());
      if (actually_send)
         *actually_send = 0;
      return Errcode::device_io_failed;
   }
   if (actually_send)
      *actually_send = data.size();
   return Errcode::success;
}

template<const std::string &name,
         class... Successors>
Errcode VisaDevice<name, Successors...>::read_data(std::string &data)
{
#ifdef CHIP_TOOLS_OFFLINE
   data = "";
   return Errcode::success;
#endif
   if (!this->is_using())
      return Errcode::success;

   constexpr size_t MAX_LENGTH = 1024;
   char ret[MAX_LENGTH];
   std::this_thread::sleep_for(std::chrono::milliseconds(delay_));
   char format[] = "%s";
   if (viScanf(get_session(), format, ret) == VI_SUCCESS)
   {
      std::copy(ret, ret + strlen(ret), std::back_inserter(data));
      return Errcode::success;
   }
   else
   {
      this->fatal(print_error<Errcode::device_io_failed>());
      return Errcode::device_io_failed;
   }
}

} // namespace visa

} // namespace chip_tools

#endif // CHIP_TOOLS_VISADEVICE_HPP_
