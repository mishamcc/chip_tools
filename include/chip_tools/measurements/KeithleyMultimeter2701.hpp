#ifndef CHIP_TOOLS_KEITHLEYMULTIMETER2701_HPP_
#define CHIP_TOOLS_KEITHLEYMULTIMETER2701_HPP_

#include <chip_tools/basic/Device.hpp>

namespace chip_tools
{

namespace measurements
{

template <class Control>
class KeithleyMultimeter2701 : public Control
{
public:
   KeithleyMultimeter2701(const std::string &name);
   ~KeithleyMultimeter2701() {}

   errcode::errcode_t raw_reset() override;

   chip_tools::errcode::errcode_t measure(const std::string &function, double &value);
   chip_tools::errcode::errcode_t read(double &value);
   chip_tools::errcode::errcode_t set_dcv_filter(bool on);
   chip_tools::errcode::errcode_t set_dcv_filter_count(int count);
   chip_tools::errcode::errcode_t set_function(const std::string &function);
   chip_tools::errcode::errcode_t set_dcv_resolution(double value);
   chip_tools::errcode::errcode_t set_dcv_range(double value);
   chip_tools::errcode::errcode_t set_dcv_npl(double value);
}; // class KeithleyMultimeter2701

template <class Control>
KeithleyMultimeter2701<Control>::KeithleyMultimeter2701(const std::string &name):
    Control(name) {}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::raw_reset()
{
    return Control::write_data("*RST\n");
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::measure(const std::string &function, double &value)
{
   std::ostringstream command;
   command << ":MEAS:" << function <<"?\n";
   return_if_error(Control::write_data(command.str()));

   std::string reply;
   return_if_error(Control::read_data(reply));

   value = to_number<double>(reply, chip_tools::dec);
   return errcode::success;
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::read(double &value)
{
   return_if_error(Control::write_data("READ?\n"));

   std::string reply;
   return_if_error(Control::read_data(reply));

   value = to_number<double>(reply, chip_tools::dec);
   return errcode::success;
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::set_dcv_filter(bool on)
{
   std::string command = on ? ":VOLT:DC:AVER:STAT ON\n" : ":VOLT:DC:AVER:STAT OFF\n";
   return Control::write_data(command);
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::set_dcv_filter_count(int count)
{
    std::ostringstream command;
    command << ":VOLT:DC:AVER:COUN " << count <<"\n";
    return Control::write_data(command.str());
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::set_function(const std::string &function)
{
   std::ostringstream command;
   command << ":FUNC \'" << function <<"\'\n";
   return Control::write_data(command.str());
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::set_dcv_resolution(double value)
{
   std::ostringstream command;
   command << ":VOLT:DC:DIG " << value <<"\n";
   return Control::write_data(command.str());
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::set_dcv_range(double value)
{
   std::ostringstream command;
   command << ":VOLT:DC:RANG " << value <<"\n";
   return Control::write_data(command.str());
}

template <class Control>
errcode::errcode_t KeithleyMultimeter2701<Control>::set_dcv_npl(double value)
{
   std::ostringstream command;
   command << ":VOLT:DC:NPLC " << value <<"\n";
   return Control::write_data(command.str());
}

} // namespace measurements

} // namespace chip_tools

#endif // CHIP_TOOLS_KEITHLEYMULTIMETER2701_HPP_
