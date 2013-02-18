#! /usr/bin/env ruby
require 'orocos'
Orocos.initialize

#Orocos.load_typekit 'phins_ixsea'

Orocos.run 'phins_ixsea::Task' => 'phins' do
    Orocos.logger.level = Logger::DEBUG
    Orocos.log_all
    puts "deployed phins_ixsea::Task"

    phins = Orocos::TaskContext.get 'phins'
    
    phins.io_port = 'udpserver://8113'
    
    Orocos.apply_conf_file(phins, 'phins_ixsea::Task.yml')
    phins.extended_status = true;
    phins.configure
    phins.start
    
    STDIN.getc
    
end
