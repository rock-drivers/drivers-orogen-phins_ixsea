/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include <phins_ixsea/IODriver.hpp>
#include "IOTask.hpp"


using namespace phins_ixsea;

IOTask::IOTask(std::string const& name)
    : IOTaskBase(name),
      mDriver(0)
{
}

IOTask::IOTask(std::string const& name, RTT::ExecutionEngine* engine)
    : IOTaskBase(name, engine),
      mDriver(0)
{
}

IOTask::~IOTask()
{
    delete mDriver;
}

bool IOTask::configureHook()
{
    delete mDriver;
    mDriver = new phins_ixsea::IODriver();

    if (!_io_port.get().empty()) {
        mDriver->openURI(_io_port.get());
    }

    setDriver(mDriver);

    if (! iodrivers_base::Task::configureHook())
        return false;
    return true;
}


bool IOTask::startHook()
{
    if (! iodrivers_base::Task::startHook())
        return false;
    return true;
}

void IOTask::updateHook()
{
    iodrivers_base::RawPacket   packet;
    if (_io_master2slave.read(packet) == RTT::NewData) {
        mDriver->writePacket(packet.data.data(), packet.data.size());
    }
    
    iodrivers_base::Task::updateHook();

    base::Time time = base::Time::now();
    if ((time - mLastStatus) > _io_status_interval.get()) {
        _io_status.write(mDriver->getStatus());
        mLastStatus = time;
    }
}



void IOTask::errorHook()
{
    iodrivers_base::Task::errorHook();
}



void IOTask::stopHook()
{
    iodrivers_base::Task::stopHook();
}



void IOTask::cleanupHook()
{
    iodrivers_base::Task::cleanupHook();
}

void IOTask::processIO()
{
    uint8_t buffer[2048];

    int size = mDriver->readPacket(buffer, 2048);

    if (size) {
        iodrivers_base::RawPacket   packet;
        packet.time = base::Time::now();
        packet.data.assign(buffer, buffer + size);
        _io_slave2master.write(packet);
    }
}

