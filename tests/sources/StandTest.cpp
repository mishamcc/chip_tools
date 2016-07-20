#define CHIP_TOOLS_OFFLINE
#ifdef TESTIO_VIA_VISA
#   include <chip_tools/visa/VisaDevice.hpp>
#else
#   include <chip_tools/inet/InetDevice.hpp>
#endif // TESTIO_VIA_VISA

#include <chip_tools/measurements/KeithleyMultimeter2010.hpp>
#include <chip_tools/measurements/KeithleySourceMeter2602.hpp>

using namespace chip_tools;

#define BOOST_TEST_MODULE chip_tools_unit_test
#include <boost/test/included/unit_test.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace chip_tools::measurements;

BOOST_AUTO_TEST_SUITE(StandTest)

std::string mult_name("Multimeter");
std::string src_name("SourceMeter");

#ifdef TESTIO_VIA_VISA
   using namespace chip_tools::visa;
   using Multimeter = KeithleyMultimeter2010<mult_name, VisaDevice<mult_name>>;
   using SourceMeter = KeithleySourceMeter2602<src_name, VisaDevice<src_name>>;
#else
   using namespace chip_tools::inet;
   using Multimeter = KeithleyMultimeter2010<mult_name, InetDevice<mult_name>>;
   using SourceMeter = KeithleySourceMeter2602<src_name, InetDevice<src_name>>;
#endif // #ifdef TESTIO_VIA_VISA

std::string stand_name("Stand");
class Stand : public Device<stand_name, Stand, Multimeter, SourceMeter>
{
public:
   explicit Stand(std::ostream &output = std::cout):
      Device<stand_name, Stand, Multimeter, SourceMeter>(output) {}
}; // class Stand

BOOST_AUTO_TEST_CASE(StandTest)
{
   Stand stand;

   std::ifstream config_file("config/StandTest.xml");
   BOOST_CHECK(config_file.is_open());
   boost::property_tree::ptree config_tree;
   BOOST_REQUIRE_NO_THROW(boost::property_tree::xml_parser::read_xml(config_file, config_tree));

   BOOST_CHECK(stand.initialize_all(&config_tree) == Errcode::success);
   BOOST_CHECK(stand.set_voltage(SourceMeter::C_A, .0) == Errcode::success);

   double voltage = .0;
   double check_val = .3;
   stand.Multimeter::set_default_value(check_val);
   BOOST_CHECK(stand.read(voltage) == Errcode::success);
   BOOST_CHECK_EQUAL(voltage, stand.Multimeter::get_default_value());
}

BOOST_AUTO_TEST_SUITE_END() // CompositeTest
