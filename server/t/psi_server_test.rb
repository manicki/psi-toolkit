require 'config'

class PsiServerTest < Test::Unit::TestCase

	def test_server_address
		assert_nothing_raised(NativeException) do
			browser = Celerity::Browser.new
			browser.goto $server_address
			browser.close
		end
	end

end
