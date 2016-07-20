#ifndef CHIP_TOOLS_KEITHLEYMULTIMETER2010_HPP_
#define CHIP_TOOLS_KEITHLEYMULTIMETER2010_HPP_

#include <chip_tools/basic/Device.hpp>

namespace chip_tools
{

namespace measurements
{

template <const std::string &name,
          class Control>
class KeithleyMultimeter2010 : public Control
{
public:
   explicit KeithleyMultimeter2010(std::ostream &output = std::cout);

   Errcode set_channel(unsigned char channel);
   Errcode measure(const std::string &function, double &value);
   Errcode measure(unsigned char channel, const std::string &function, double &value);

   Errcode read(double &value);
   Errcode set_dcv_filter(bool on);
   Errcode set_dcv_filter_count(int count);
   Errcode set_function(const std::string &function);
   Errcode set_dcv_resolution(double value);
   Errcode set_dcv_range(double value);
   Errcode set_dcv_npl(double value);

#ifdef CHIP_TOOLS_OFFLINE
   void set_default_value(double value) {default_val = value;}
   double get_default_value() {return default_val;}
   double default_val = .1;
#endif
}; // class KeithleyMultimeter2010

template <const std::string &name,
          class Control>
KeithleyMultimeter2010<name, Control>::KeithleyMultimeter2010(std::ostream &output):
   Control(output) {}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::set_channel(unsigned char channel)
{
   return Control::write_data(":ROUT:CLOS (@ " + std::to_string(channel) + ")\n");
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::measure(const std::string &function, double &value)
{
#ifdef CHIP_TOOLS_OFFLINE
   value = default_val;
   return Errcode::success;
#endif
   return_if_error(Control::write_data(":MEAS:" + function + "?\n"));

   std::string reply;
   return_if_error(Control::read_data(reply));
   value = std::stod(reply);
   return Errcode::success;
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::measure(unsigned char channel,
   const std::string &function, double &value)
{
   return_if_error(set_channel(channel));
   return measure(function, value);
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::read(double &value)
{
#ifdef CHIP_TOOLS_OFFLINE
   value = default_val;
   return Errcode::success;
#endif
   std::string reply;
   RETURN_IF_ERROR(Control::read_data(reply));

   value = std::stod(reply);
   return Errcode::success;
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::set_dcv_filter(bool on)
{
   std::string command = on ? ":VOLT:DC:AVER:STAT ON\n" : ":VOLT:DC:AVER:STAT OFF\n";
   return Control::write_data(command);
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::set_dcv_filter_count(int count)
{
   return Control::write_data(":VOLT:DC:AVER:COUN " + std::to_string(count) + "\n");
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::set_function(const std::string &function)
{
   return Control::write_data(":FUNC \'" + function + "\'\n");
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::set_dcv_resolution(double value)
{
   return Control::write_data(":VOLT:DC:DIG " + std::to_string(value) + "\n");
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::set_dcv_range(double value)
{
   return Control::write_data(":VOLT:DC:RANG " + std::to_string(value) + "\n");
}

template <const std::string &name,
          class Control>
Errcode KeithleyMultimeter2010<name, Control>::set_dcv_npl(double value)
{
   return Control::write_data(":VOLT:DC:NPLC " + std::to_string(value) + "\n");
}

} // namespace measurements

} // namespace chip_tools

#endif // CHIP_TOOLS_KEITHLEYMULTIMETER2010_HPP_
