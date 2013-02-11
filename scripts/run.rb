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
    
    phins.configure
    phins.start
    
    
    STDIN.getc
    
end
