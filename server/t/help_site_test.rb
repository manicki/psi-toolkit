require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class HelpSiteTest < Test::Unit::TestCase

    def setup
        @browser = Celerity::Browser.new
        @browser.goto $server_localhost + 'help.html'
    end

    def teardown
        @browser.close
    end

    def test_if_help_container_exists
        assert !@browser.div(:id, 'help-list').text.empty?
    end

    def test_if_each_processor_has_a_title_and_an_option_list
       titles = @browser.h3s
       assert !titles.empty?

       options = @browser.pres
       assert !options.empty?

       assert titles.size, options.size
    end

end
