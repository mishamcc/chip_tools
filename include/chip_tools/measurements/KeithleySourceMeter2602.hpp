#ifndef CHIP_TOOLS_KEITHLEYSOURCEMETER2602_HPP_
#define CHIP_TOOLS_KEITHLEYSOURCEMETER2602_HPP_

#include <chip_tools/basic/Device.hpp>

namespace chip_tools
{

namespace measurements
{

template <const std::string &name,
          class Control>
class KeithleySourceMeter2602 : public Control
{
public:
   enum Channel {C_A, C_B};
   enum SenseMode {SM_LOCAL, SM_REMOTE};
   
   explicit KeithleySourceMeter2602(std::ostream &output = std::cout);
   
   Errcode set_channel_state(Channel channel, bool state);
   Errcode set_sense_mode(Channel channel, SenseMode mode);

   Errcode set_voltage(Channel channel, double voltage);
   Errcode set_current_limit(Channel channel, double current_limit);

   Errcode get_current(Channel channel, double &current);
   Errcode get_voltage(Channel channel, double &voltage);
   
#ifdef CHIP_TOOLS_OFFLINE
   void set_default_value(double value) {default_val = value;}
   double get_default_value() {return default_val;}
   double default_val = .1;
#endif
}; // class KeithleySourceMeter2602

template <const std::string &name,
          class Control>
KeithleySourceMeter2602<name, Control>::KeithleySourceMeter2602(std::ostream &output):
   Control(output) {}
   
template <const std::string &name,
          class Control>
Errcode KeithleySourceMeter2602<name, Control>::set_channel_state(Channel channel, bool state)
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   std::string command = (channel == C_A) ?
      (state ? "smua.source.output = smua.OUTPUT_ON\n" :
         "smua.source.output = smua.OUTPUT_OFF\n") :
      (state ? "smub.source.output = smub.OUTPUT_ON\n" :
         "smub.source.output = smub.OUTPUT_OFF\n");
   return Control::write_data(command);   
}

template <const std::string &name,
          class Control>
Errcode KeithleySourceMeter2602<name, Control>::set_sense_mode(Channel channel, SenseMode mode)
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   std::string command = (channel == C_A) ?
      (mode == SM_LOCAL ? "smua.sense = smua.SENSE_LOCAL\n" :
         "smua.sense = smua.SENSE_REMOTE\n") :
      (mode == SM_REMOTE ? "smub.sense = smub.SENSE_LOCAL\n" :
         "smub.sense = smub.SENSE_REMOTE\n");
   return Control::write_data(command);
}

template <const std::string &name,
          class Control>
Errcode KeithleySourceMeter2602<name, Control>::set_voltage(Channel channel, double voltage)
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   std::string command = (channel == C_A) ? "smua.source.levelv = " :
      "smub.source.levelv = ";
   command += std::to_string(voltage) + "\n";
   return Control::write_data(command);
}

template <const std::string &name,
          class Control>
Errcode KeithleySourceMeter2602<name, Control>::set_current_limit(Channel channel, double current_limit)
{
#ifdef CHIP_TOOLS_OFFLINE
   return Errcode::success;
#endif
   std::string command = (channel == C_A) ? "smua.source.limiti = " :
      "smub.source.limiti = ";
   command += std::string(current_limit) + "\n";
   return Control::write_data(command);
}

template <const std::string &name,
          class Control>
Errcode KeithleySourceMeter2602<name, Control>::get_current(Channel channel, double &current)
{
#ifdef CHIP_TOOLS_OFFLINE
   current = default_val;
   return Errcode::success;
#endif
   std::string command = (channel == C_A) ? "print(smua.measure.i())\n" :
      "print(smub.measure.i())\n";   
   return_if_error(Control::write_data(command));
   std::string reply;
   return_if_error(Control::read_data(reply));

   current = std::stod(reply);
   return Errcode::success;
}

template <const std::string &name,
          class Control>
Errcode KeithleySourceMeter2602<name, Control>::get_voltage(Channel channel, double &voltage)
{
#ifdef CHIP_TOOLS_OFFLINE
   voltage = default_val;
   return Errcode::success;
#endif
   std::string command = (channel == C_A) ? "print(smua.measure.v())\n" :
      "print(smub.measure.v())\n";
   return_if_error(Control::write_data(command));   
   std::string reply;
   return_if_error(Control::read_data(reply));

   voltage = std::stod(reply);
   return Errcode::success;
}

} // namespace measurements

} // namespace chip_tools

#endif // CHIP_TOOLS_KEITHLEYSOURCEMETER2602_HPP_
