#! /usr/bin/env ruby
require 'orocos'
Orocos.initialize

#Orocos.load_typekit 'phins_ixsea'

Orocos.run 'phins_ixsea::Task' => 'phins', 'phins_ixsea::IOTask' => ['gpsin', 'gpsout']  do
#Orocos.run 'phins_ixsea_test' do
    Orocos.logger.level = Logger::DEBUG
    Orocos.log_all
    puts "deployed phins_ixsea::Task"

    phins = Orocos::TaskContext.get 'phins'
    gps2phins = Orocos::TaskContext.get 'gpsout'
    gpsin = Orocos::TaskContext.get 'gpsin'
    
    gps2phins.io_master2slave.connect_to gpsin.io_read_listener
      
    Orocos.apply_conf_file(phins, 'phins_ixsea::Task.yml')
    phins.extended_status = true;
    phins.configure
    phins.start
    
    Orocos.apply_conf_file(gps2phins, 'phins_ixsea::IOTask.yml')
    gps2phins.configure
    gps2phins.start
    
    gpsin.io_port = 'udpserver://9111'
    gpsin.io_read_timeout = Time.at(0)
    gpsin.configure
    gpsin.start

    STDIN.getc
    
end
