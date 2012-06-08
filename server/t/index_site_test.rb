require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class IndexSiteTest < Test::Unit::TestCase

    def setup
        @browser = Celerity::Browser.new
        @browser.goto $server_address
    end

    def teardown
        @browser.close
    end

    def test_if_github_ribbon_exists
        assert @browser.a(:id, 'github-ribbon').exists?
    end

    def test_psi_toolkit_text
        assert @browser.text.include?('PSI-Toolkit')
    end

    def test_if_promotion_section_exists
        assert @browser.div(:id, 'promotion').exists?
    end

    def text_if_submit_hides_promotion_section
        assert @browser.div(:id, 'promotion').exists?
        @browser.button(:name => 'pipe-submit').click
        assert !@browser.div(:id, 'promotion').exists?
    end

end
