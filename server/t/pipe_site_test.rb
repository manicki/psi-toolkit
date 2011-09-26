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

    def test_pipe
        assert !@browser.text_field(:name => 'pipe-text').value.empty?
    end

    def test_bookmarks_before_switching
        assert @browser.div(:id, 'input-text').visible?
        assert !@browser.div(:id, 'input-file').visible?

        active_li_class = @browser.a(:class, 'input-text').parent.attribute_value(:class)
        assert active_li_class.include?('active')

        inactive_li_class = @browser.a(:class, 'input-file').parent.attribute_value(:class)
        assert !inactive_li_class.include?('active')
    end

    def test_bookmarks_after_switching
        @browser.a(:class => 'input-file').click

        assert !@browser.div(:id, 'input-text').visible?
        assert @browser.div(:id, 'input-file').visible?

        inactive_li_class = @browser.a(:class, 'input-text').parent.attribute_value(:class)
        assert !inactive_li_class.include?('active')

        active_li_class = @browser.a(:class, 'input-file').parent.attribute_value(:class)
        assert active_li_class.include?('active')
    end

    def test_input_text
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

    def test_input_file
        #TODO

        input_file = @browser.file_field(:name => 'input-file')
        assert input_file.exists?
    end

end
