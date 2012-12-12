
#! /usr/bin/env ruby

# starting the gui:
# "ruby scripts/runviz.rb run --gui" require 'vizkit'
# 

require 'orocos'

include Orocos

Orocos.initialize


Orocos::CORBA.call_timeout= 200


#Orocos.load_typekit_for "/base/samples/RigidBodyState"
Orocos.load_typekit "base"
#ENV['ORO_LOGLEVEL'] = '1'

gui = false
optparse = OptionParser.new do |opt|
    opt.banner = "run [--gui] DEVICE"
    opt.on '--gui', "starts the Rock task inspection widget on the running task" do
        gui = true
    end
end
dev_id = *optparse.parse(ARGV)
if !dev_id
    puts optparse
    exit 1
end##
if gui
    require 'vizkit'
end



## Execute the task 'message_producer::Task' ##
Orocos.run 'phins_ixsea::Task' => 'phins_ixsea' do
  
  # log files
  #Orocos.logger.level = Logger::DEBUG
  #Orocos.log_all

  ## Get the task context##
  phins = TaskContext.get 'phins_ixsea'
 # phins.io_read_timeout = Time.at(2);
  
 
 Orocos.apply_conf_file(phins, 'phins_ixsea_config.yml', ['phinsStandard'])
 #Orocos.apply_conf_file(phins, 'phins_ixsea_config.yml', ['halliburtonSAS'])
    
  phins.configure
  phins.start
  
  
  if gui

    task_inspector = Vizkit.default_loader.task_inspector
    task_inspector.config(phins)
    task_inspector.show
    
    # lade die gui
    guiviz = Vizkit.load 'scripts/simple.ui'
    puts "loaded simple.ui"

 
 
    
    phins.rbs.connect_to do |data,_|
          puts "xxxxxxxxxxXXXXXX**********************************************************"
          guiviz.pos_x.setText(data.position[0].to_s())
          guiviz.pos_y.setText(data.position[1].to_s())
          guiviz.pos_z.setText(data.position[2].to_s())
            
         # guiviz.heading.setText(data.getYaw().to_s())
          guiviz.roll.setText(data.position[1].to_s())
          guiviz.pitch.setText(data.position[2].to_s())
            
          guiviz.vel_east.setText(data.velocity[0].to_s())
          guiviz.vel_north.setText(data.velocity[1].to_s())
          guiviz.vel_up.setText(data.velocity[2].to_s())
            
          guiviz.time.setText(data.time.to_s())    

           # data.getYaw().to_s() 
     end
     
     
     phins.utmwgs.connect_to do |data,_|
       
          guiviz.latitude.setText(data.latitudeZone.to_s())
          guiviz.longitude.setText(data.longitudeZone.to_s())
          guiviz.north_pos.setText(data.northPosition.to_s())
          guiviz.east_pos.setText(data.eastPosition.to_s())
          guiviz.altitude.setText(data.altitude.to_s())
      
     end
     
     guiviz.show
     Vizkit.exec
end
  
 # Orocos.watch(phins) do
 #   
 # end
 # else 
    #while true
   
    #    sleep 0.5
    #  end
 # end
end