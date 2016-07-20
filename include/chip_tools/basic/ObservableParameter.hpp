#ifndef CHIP_TOOLS_OBSERVABLE_PARAMETER_HPP_
#define CHIP_TOOLS_OBSERVABLE_PARAMETER_HPP_

#include <chip_tools/basic/Composite.hpp>
#include <boost/signals2.hpp>

namespace chip_tools
{

template<typename T,
         const std::string &name>
class ObservableParameter : public Composite<name, ObservableParameter<T, name>>
{
public:
   using ValueType = T;
   using SignalType = boost::signals2::signal<void(T)>;
   using ParentType = Composite<name, ObservableParameter<T, name>>;

   explicit ObservableParameter(std::ostream &output = std::cout) :
      ParentType(output) {}

   std::string get_units() const {return units_;}
   boost::signals2::signal<void(T)> updated;
   boost::signals2::signal<void()> cleared;

   ParentType & operator=(T value)
   {
      updated(value_ = value);
      return *this;
   }  

   operator T() const {return value_;}
   void reset()
   {
      value_ = T();
      cleared();
   }

   Errcode initialize(boost::property_tree::ptree *node)
   {
      units_ = node->get<std::string>("<xmlattr>.Units", "");
      return Errcode::success;
   }

private:
   std::string units_;
   T value_;
}; // class ObservableParameter


template<typename T,
         const std::string &name>
class CheckableParameter : public ObservableParameter<T, name>
{
public:
   using ViolationSignalType = boost::signals2::signal<void(std::size_t)>;
   using ParentType = ObservableParameter<T, name>;

   explicit CheckableParameter(std::ostream &output = std::cout) :
      ParentType(output),
      min_(), max_(), valid_(true), num_of_violations_(0) {}

   ParentType & operator=(T value)
   {
      valid_ = check(value);
      ParentType::operator=(value);
      return *this;
   }

   T min() const {return min_;}
   T max() const {return max_;}

   bool valid() const {return valid_;}

   void reset()
   {
      ParentType::reset();
      valid_ = true;
      num_of_violations_ = 0;
   }

   ViolationSignalType violation;

   Errcode initialize(boost::property_tree::ptree *node)
   {
      ParentType::initialize(node);
      max_ = node->get<T>("<xmlattr>.max", T());
      min_ = node->get<T>("<xmlattr>.min", T());
      return Errcode::success;
   }

private:
   bool check(T value)
   {
      if ((value < min_) || (value > max_))
      {
         violation(++num_of_violations_);
         return false;
      }
      else
         return true;
   }

   T min_;
   T max_;
   bool valid_;
   std::size_t num_of_violations_;

}; // class CheckableParameter

template<const std::string &name>
class ErrorsCounter : public ObservableParameter<std::size_t, name>
{
public:
    using ViolationSignalType = boost::signals2::signal<void(std::size_t)>;
    using ParentType = ObservableParameter<std::size_t, name>;

    explicit ErrorsCounter(std::ostream &output = std::cout) :
       ParentType(output),
       num_of_violations_(0) {}

    ParentType & operator=(std::size_t value) override
    {
       ParentType::operator=(value);
       if (value != 0)
       {
          num_of_violations_ += value;
          violation(num_of_violations_);
       }
       return *this;
    }

    void reset()
    {
       ParentType::reset();
       num_of_violations_ = 0;
    }

    bool valid() const
    {
       return *this == 0;
    }

    ViolationSignalType violation;
private:
    std::size_t num_of_violations_;
}; // class ErrorsCounter

} // namespace chip_tools

#endif // #ifndef CHIP_TOOLS_OBSERVABLE_PARAMETER_HPP_
