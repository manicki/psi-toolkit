hostname = '0.0.0.0'
port = '3000'
threads = '1'
current_dir = File.dirname(__FILE__)
website = File.expand_path(current_dir + '/../website/')

$source_dir = File.expand_path(current_dir + '/../../prj/')
$server_address = "http://#{hostname}:#{port}/"
$server_localhost = "http://localhost:#{port}/"
$arguments = "--address #{hostname} --port #{port} --threads #{threads} --root #{website}"
