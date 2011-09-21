require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class PipeSiteTest < Test::Unit::TestCase

    def setup
        @browser = Celerity::Browser.new(:browser => :firefox)
        @browser.goto $server_localhost
    end

    def teardown
        @browser.close
    end

    def test_input_text
        assert !@browser.text_field(:name => 'pipe-text').value.empty?

        input  = @browser.text_field(:name => 'input-text')
        assert input.exists?
        assert !input.value.empty?

        txt = 'Marysia ma rysia'

        assert_not_equal input.value, txt
        input.set txt
        assert input.value, txt

        btn = @browser.button(:name => 'pipe-submit')
        assert btn.exists?
        btn.click

        txt.split(' ').each do |word|
            assert @browser.text.include? word
        end
    end

end
