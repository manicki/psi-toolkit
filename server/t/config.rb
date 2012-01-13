$host ||= 'localhost'
$port ||= '3000'
threads = '1'
current_dir = File.dirname(__FILE__)
website = File.expand_path(current_dir + '/../website/')

$source_dir = File.expand_path(current_dir + '/../../build/')
$server_address = "http://#{$host}:#{$port}/"
$arguments = "--address #{$host} --port #{$port} --threads #{threads} --root #{website}"
