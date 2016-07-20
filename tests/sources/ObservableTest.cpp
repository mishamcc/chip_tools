#include <chip_tools/basic/ObservableParameter.hpp>

using namespace chip_tools;

#define BOOST_TEST_MODULE chip_tools_unit_test
#include <boost/test/included/unit_test.hpp>
#include <boost/property_tree/xml_parser.hpp>

BOOST_AUTO_TEST_SUITE(ObservableTest)

std::string par1_name("TestParameter");
ObservableParameter<int, par1_name> par1;
int num_of_callings = 0;
int test_value = 0;

void handler1(int value)
{
   num_of_callings++;
   test_value = value;
   std::cout << "----------------------\nhandler1 has been called " <<
      num_of_callings << " times\n";
   std::cout << "test value is " << value << "\n";
}

BOOST_AUTO_TEST_CASE(ObservableTest)
{
   par1.updated.connect(handler1);

   int num_of_calls = 10;

   for (auto i = 0; i < num_of_calls; ++i)
   {
      par1 = i * i;
      BOOST_CHECK_EQUAL(num_of_callings, i + 1);
      BOOST_CHECK_EQUAL(test_value, i * i);
   }
}

BOOST_AUTO_TEST_SUITE_END()
