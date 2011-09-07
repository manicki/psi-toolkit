require 'config'

=begin
exec = ""
t_server = Thread.new do
	exec = "jruby #{$source_dir}/psi-server #{$arguments}"
end

t_test = Thread.new do
	sleep 0.1
	require 'psi_server_test'
	require 'index_site_test'
end

puts exec
=end
