require 'orocos'
include Orocos

## Initialize orocos ##
Orocos.initialize

## Execute the task 'message_producer::Task' ##
Orocos.run 'phins_ixsea::Task' => 'phins_ixsea' do
  
  Orocos.logger.level = Logger::DEBUG
  Orocos.log_all

  ## Get the task context##
  phins = TaskContext.get 'phins_ixsea'
    
  phins.configure
  ## Start the tasks 
  phins.start
  
  Orocos.watch(phins) 

  while true
  #    if msg = reader.read_new
       #   puts "#{msg.time} #{msg.content}"
   #   end
  
      sleep 0.1
  end
end