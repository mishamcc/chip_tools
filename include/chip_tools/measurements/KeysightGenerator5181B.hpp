#ifndef CHIP_TOOLS_AGILENT_GENERATOR_HPP_
#define CHIP_TOOLS_AGILENT_GENERATOR_HPP_

#include <chip_tools/basic/ChipTypes.hpp>
#include <chip_tools/basic/ErrorCodes.hpp>
#include <map>

namespace chip_tools
{

namespace measurements
{

template <class Control>
class KeysightGenerator5181B : public Control
{
public:
   enum FreqUnit {Hz, KHz, MHz, GHz};
   enum LevelUnit {mV, dBm};
   explicit KeysightGenerator5181B(const std::string &name);
   ~KeysightGenerator5181B() {}

   chip_tools::errcode::errcode_t power(bool state);
   chip_tools::errcode::errcode_t set_frequency(double value, FreqUnit unit = MHz);
   chip_tools::errcode::errcode_t set_level(double value, LevelUnit unit = dBm);

private:
   std::map<FreqUnit, const char *> freq_unit_to_char_;
   std::map<LevelUnit, const char *> level_unit_to_char_;
}; // class AgilentGenerator

template <class Control>
KeysightGenerator5181B<Control>::KeysightGenerator5181B(const std::string &name):
   Control(name)
{
   freq_unit_to_char_[KeysightGenerator5181B::Hz] = "Hz";
   freq_unit_to_char_[KeysightGenerator5181B::KHz] = "KHz";
   freq_unit_to_char_[KeysightGenerator5181B::MHz] = "MHz";
   freq_unit_to_char_[KeysightGenerator5181B::GHz] = "GHz";

   level_unit_to_char_[KeysightGenerator5181B::mV] = "mV";
   level_unit_to_char_[KeysightGenerator5181B::dBm] = "dBm";
}

template <class Control>
chip_tools::errcode::errcode_t KeysightGenerator5181B<Control>::power(bool state)
{
   std::ostringstream stream_command;
   stream_command << ":OUTP " << (state ? "ON" : "OFF") << "\n";
   return Control::write_data(stream_command.str());
}

template <class Control>
chip_tools::errcode::errcode_t KeysightGenerator5181B<Control>::set_frequency(double value, FreqUnit unit)
{
   std::ostringstream stream_command;
   stream_command << ":FREQ " << value << " " << freq_unit_to_char_[unit] << "\n";
   return Control::write_data(stream_command.str());
}

template <class Control>
chip_tools::errcode::errcode_t KeysightGenerator5181B<Control>::set_level(double value, LevelUnit unit)
{
   std::ostringstream stream_command;
   stream_command << ":POW:AMPL " << value << " " << level_unit_to_char_[unit] << "\n";
   return Control::write_data(stream_command.str());
}

} // namespace measurements

} // namespace chip_tools

#endif // #ifndef MCOLOR19_AGILENT_GENERATOR_HPP_
