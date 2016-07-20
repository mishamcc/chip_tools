#ifndef CHIP_TOOLS_ANRITSU_SPECTRUM_ANALYZER_MS2668C_HPP_
#define CHIP_TOOLS_ANRITSU_SPECTRUM_ANALYZER_MS2668C_HPP_

#include <chip_tools/visa/VisaDevice.hpp>

namespace chip_tools
{

namespace visa
{

namespace anritsu
{

class SpectrumAnalyzerMC2668C : public VisaDevice
{
public:
   explicit SpectrumAnalyzerMC2668C(const std::string &name);
   ~SpectrumAnalyzerMC2668C();

   errcode::errcode_t set_central_frequency(double frequency);
   errcode::errcode_t set_frequency_span(double span);

   errcode::errcode_t trace_a_memory(size_t first_point, size_t last_point,
      std::vector<double> spectrum);

};

// class SpectrumAnalyzerMC2668C : public VisaDevice

} // namespace anritsu

} // namespace visa

} // namespace chip_tools

#endif // CHIP_TOOLS_ANRITSU_SPECTRUM_ANALYZER_MS2668C_HPP_
