/** @file Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include <base/logging.h>
#include <phins_ixsea/Driver.hpp>
#include <phins_ixsea/Error.hpp>

#include "Task.hpp"

using namespace phins_ixsea;
using namespace oro_marum;

Task::Task(std::string const& name)
    : TaskBase(name),
      mpDriver(0)
{

}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine),
      mpDriver(0)
{
}

Task::~Task()
{
	delete mpDriver;
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.
bool Task::configureHook()
 {
	 try{
		 LOG_DEBUG("configure hook");
		 delete mpDriver;

		 Config configuration = _config.get();
		 mpDriver = new Driver(configuration);
    	 mpDriver-> open(configuration.port);

		 if (! TaskBase::configureHook())
			 return false;
	 }
	 catch(std::runtime_error &e){
		 LOG_DEBUG("exception %s", e.what());
	 }
     return true;
 }

bool Task::startHook()
 {
	try{
	LOG_DEBUG("Start Hook");
	if (! TaskBase::startHook())
         return false;
     return true;
	}catch(std::runtime_error &e){
		LOG_DEBUG("exception %s", e.what());

		return false;
	}
 }

void Task::updateHook() {

   try{

	//  switch(state()){
	//  	  case RUNNING:{
	//	  break;
	//  	  }
	//  }
		TaskBase::updateHook();
		mpDriver-> read(2000);
		_utmwgs.write(mpDriver->getUtmwgs());
		_rbs.write(mpDriver->getRBS());
	}
	catch(std::runtime_error &e){
		_log_message.write(LogMessage(e)); // wird als output port in den task nspector geschrieben
	}
 }

void Task::errorHook()
 {
	 LOG_DEBUG("Error Hook");
     TaskBase::errorHook();

 }

void Task::stopHook()
 {
	LOG_DEBUG("Stop Hook");
    TaskBase::stopHook();
    mpDriver->resetPhins();
    recover(); // sets Task in pre operational state

 }

void Task::cleanupHook()
 {
     TaskBase::cleanupHook();
     delete mpDriver;
 }

