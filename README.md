# ChipTools library
## Brief
The library implements the main features which may be needed by developing some hardware-based (particulariy dsp) applications:

* convenient and transparent tree-like hierarchies of classes (compile-time composite pattern);
* devices configuration loading from xml-files;
* logging;
* notification system for parameters changing.
 
ChipTools library also contains basic tools for 

* Socket-, VISA-(Virtual Instrument Software Architecture), Serial-based devices;
* interacting with CypressFx2 microcontroller;
* digital signal processing (fft, tools for signal analyzing).

### System requirements

The library has been written in C/C++. The Boost library (at least 1.58) is required.  Other requirements depend on modules used by client app. 
The content of the library and system requirements are listed in the following section.

## Overview
* **basic** (header-only). The module provides common functionality for library users and for the rest modules:
   * error codes definition (Errno.hpp, ErrorCodes.hpp);
   * basic functionality for logging (Log.hpp);
   * compile-time composite pattern implementation (Composite.hpp, Boost is required);
   * common interface for devices classes (Device.hpp);
   * implementation of the observer pattern for parameters of a system must be observable (ObservableParameter.hpp, Boost is required);

* **inet** (header-only). Implements basic IO-operations for socket-based devices;

* **serial** (header-only). Implements basic IO-operations for serial-based devices (Unix only);

* **visa** (header-only). Implements basic IO-operations for VISA-based devices (VISA library is required);

* **measurements** (header-only). Provides common operations for some measurements devices. Among them are multimeters, sourcemeters, spectrum analyzers, generators. 
Generally, all of them support  VISA, but some could be implemented via inet, serial or other interfaces (it depends on interfaces supported by certain device);

* **ucf_tools**. Contains some specific operations with the CypressFx2 EZ-USB controller (libusb-1.0 is required);
* **qt** (header-only). qt-specific tools (saving/loading widget values, Qt5 required).

Some parts of the ChipTools are implemented in the header-only form and thus are ready for using without compilation. 
