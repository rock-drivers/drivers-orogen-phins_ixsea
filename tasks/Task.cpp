/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <phins_ixsea/Driver.hpp>
#include <rtt/extras/FileDescriptorActivity.hpp>



using namespace phins_ixsea;

Task::Task(std::string const& name)
    : TaskBase(name),
      mDriver(0)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine),
      mDriver(0)
{
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

 bool Task::configureHook()
 {
     delete mDriver;
     mDriver = new phins_ixsea::Driver();

     if (!_io_port.get().empty()) {
         mDriver->openURI(_io_port.get());
     }

     mDriver->setParser(_protocol.get());

     setDriver(mDriver);

     mOrigin = _origin.get();

     if (! TaskBase::configureHook())
         return false;
     return true;
 }


void Task::processIO()
{
    try {
        mDriver->read();

        if (mDriver->hasUpdate(phins_ixsea::UPD_UTMPOS | phins_ixsea::UPD_HPR, true))  {
            base::samples::RigidBodyState rbs = mDriver->relativePose(mOrigin);
            _pose_samples.write(rbs);
            //        std::cout << "New sample: " << (rbs.getYaw() * 180 / M_PI) << " "
            //                << (rbs.getPitch() * 180 / M_PI) << " "
            //                << (rbs.getRoll() * 180 / M_PI) << std::endl;
        }
        if (mDriver->hasUpdate(phins_ixsea::UPD_POSITION), true) {
            _geo_pos_samples.write(mDriver->geoPose());
        }
        if (mDriver->hasUpdate(UPD_STATUS), true) {
            //        std::cout << std::hex << std::setw(10) << mDriver->phinsExtStatus().status_lsb << std::setw(10)
            //                            << mDriver->phinsExtStatus().status_msb << std::setw(10)
            //                            << mDriver->phinsExtStatus().algo_status_lsb << std::setw(10)
            //                            << mDriver->phinsExtStatus().algo_status_msb << std::setw(10)
            //                            << mDriver->phinsExtStatus().user_status << std::endl;

            _phins_status.write(mDriver->phinsStatus());
            if (_extended_status.get()) {
                _phins_extended_status.write(mDriver->phinsExtStatus());
            }
            processStatus(mDriver->phinsExtStatus());
        }
    } catch (std::runtime_error& e) {
        // first ignore and decide later what to do
    }
}


void Task::processStatus(const PhinsExtStatus& status)
{
    switch(mDriver->navigationMode()) {
    case COARSE_ALIGN_MODE:
        if (state() != COARSE_ALIGN)
            state(COARSE_ALIGN);
        break;
    case FINE_ALIGN_MODE:
        if (state() != FINE_ALIGN)
            state(FINE_ALIGN);
        break;
    case NAVIGATION_MODE:
        if (state() != NAVIGATION)
            state(NAVIGATION);
        break;
    case FAILURE_MODE:
        if (state() != DEVICE_ERROR)
            exception(DEVICE_ERROR);
        break;
    default:
        break;
    }
}

void Task::initializeOrigin()
{
    mOrigin = mDriver->utmPose().position;
    _origin.set(mOrigin);
}


 // bool Task::startHook()
// {
//     if (! TaskBase::startHook())
//         return false;
//     return true;
// }
 void Task::updateHook()
 {
     TaskBase::updateHook();
     if (mDriver->getFileDescriptor() != Driver::INVALID_FD)
     {
         RTT::extras::FileDescriptorActivity* fd_activity =
             getActivity<RTT::extras::FileDescriptorActivity>();
         if (fd_activity)
         {
             if (fd_activity->hasTimeout())
                 return exception(IO_TIMEOUT);
         }
     }

     base::Time time = base::Time::now();
     if ((time - mLastStatus) > _io_status_interval.get()) {
         _io_status.write(mDriver->getStatus());
         mLastStatus = time;
     }
 }
// void Task::errorHook()
// {
//     TaskBase::errorHook();
// }
// void Task::stopHook()
// {
//     TaskBase::stopHook();
// }
// void Task::cleanupHook()
// {
//     TaskBase::cleanupHook();
// }

