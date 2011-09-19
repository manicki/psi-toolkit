# Html Tests using jRuby and Celerity gem.
#
# Requirements:
# 	jruby
# 	celerity gem 		#=> jruby -S gem install celerity
# 	test-unit gem 		#=> jruby -S gem install test-unit
#	ci_reporter gem 	#=> jruby -S gem install ci_reporter
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
	$exe = `cd #{$source_dir}; ./psi-server #{$arguments}`
end

sleep 1
$pid = `pidof psi-server #{$arguments.gsub('-', '')}`
puts "\nPsi Server process id = #{$pid}"

$tst = ""
t_test = Thread.new do
	$tst = `jruby -S rake ci:setup:testunit test`
end

while (t_test.status)
	sleep(1)
	print "."
end


exit
