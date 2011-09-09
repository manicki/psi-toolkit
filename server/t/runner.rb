# Html Tests using jRuby and Celerity gem.
#
# Requirements:
# 	jruby
# 	celerity gem 	#=> jruby -S gem install celerity
# 	test-unit gem 	#=> jruby -S gem install test-unit
#

require 'config'

$exe = ""
$pid = ""

at_exit do
	`kill #{$pid}`
	puts ""
	puts $tst
end

t_server = Thread.new do
	puts "Starting psi-server..."
	$exe = `#{$source_dir}/psi-server #{$arguments}`
end

sleep 0.5
$pid = `pidof psi-server #{$arguments}`
puts "\nPsi Server process id = #{$pid}"

$tst = ""
t_test = Thread.new do
	$tst = `jruby test_suite.rb`
end

while (t_test.status)
	sleep(1)
	print "."
end


exit
