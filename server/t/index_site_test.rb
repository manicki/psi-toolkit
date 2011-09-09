require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class IndexSiteTest < Test::Unit::TestCase

	def setup
		@browser = Celerity::Browser.new
		@browser.goto $server_localhost
	end

	def teardown
		@browser.close
	end

	def test_psi_server_text
		assert(@browser.text.include? 'Psi Server')
	end

	def test_input_text
		assert(@browser.text_field(:name => 'input-text').exists?)

		txt = 'Ala ma kota';

		@browser.text_field(:name => 'input-text').set txt
		@browser.button(:name => 'input-text-submit').click

		assert(@browser.text.include? txt)
	end

end
