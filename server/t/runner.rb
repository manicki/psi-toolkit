# Html Tests using jRuby and Celerity gem.
#
# Requirements:
# 	jruby
# 	celerity gem 		#=> jruby -S gem install celerity
# 	test-unit gem 		#=> jruby -S gem install test-unit
#	ci_reporter gem 	#=> jruby -S gem install ci_reporter
#

if ['--help', '-h', '-?'].include? ARGV[0]
    puts "usage: jruby runner.rb [host] [port]"
    exit
end

$host = ARGV[0]
$port = ARGV[1]

require 'config'

$exe = ""
$pid = ""
$tst = ""

at_exit { puts $tst }

if (ARGV[0].nil? && ARGV[1].nil?)
    at_exit { `kill #{$pid}` }

    t_server = Thread.new do
        puts "Starting psi-server..."
        $exe = `cd #{$source_dir}; ./server/psi-server #{$arguments}`
    end

    sleep 1

    $pid = `pidof psi-server #{$arguments.gsub('-', '')}`.split[0]
    puts "\nPsi Server process id = #{$pid}"
end

puts "working on: #{$server_address}"

t_test = Thread.new { $tst = `jruby -S rake ci:setup:testunit test[#{$host},#{$port}]` }

sleep(1) and print "." while (t_test.status)

exit
