require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class HelpSiteTest < Test::Unit::TestCase

    def setup
        @browser = Celerity::Browser.new
        @browser.goto $server_address + '/help/documentation.html'
    end

    def teardown
        @browser.close
    end

    def test_if_help_container_exists
        assert @browser.div(:class, 'help-item').exists?
    end

    def test_if_each_processor_has_a_title_and_an_option_list
       titles = @browser.h2s
       assert !titles.empty?

       options = @browser.pres
       assert !options.empty?

       assert titles.size, options.size
    end

end
