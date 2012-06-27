require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class PipeSiteTest < Test::Unit::TestCase

    def setup
        @browser = Celerity::Browser.new(:browser => :firefox)
        @browser.webclient.setJavaScriptEnabled(true)
        @browser.goto $server_address
    end

    def teardown
        @browser.close
    end

    def test_if_pipe_is_not_empty
        assert !@browser.text_field(:name => 'pipe-text').value.empty?
    end

    def test_bookmarks_before_switching
        assert @browser.a(:id => 'bookmark-text').visible?
        assert @browser.a(:id=> 'bookmark-text').text.include?('file')

        assert @browser.div(:id => 'input-text').visible?
        assert !@browser.div(:id => 'input-file').visible?

        assert @browser.radio(:id => 'radio-input-text').set?
        assert !@browser.radio(:id => 'radio-input-file').set?
    end

    def test_bookmarks_after_switching
        switch_bookmark(:file)

        #assert @browser.a(:id => 'bookmark-file').visible?
        #assert @browser.a(:id => 'bookmark-file').text.include?('text')

        assert @browser.radio(:id => 'radio-input-file').set
        assert !@browser.radio(:id => 'radio-input-text').set?

        switch_bookmark(:text)

        assert @browser.a(:id => 'bookmark-text').visible?
        assert !@browser.div(:id => 'input-file').visible?
    end

    def test_if_submitting_of_an_input_text_works
        input  = @browser.text_field(:name => 'input-text')
        assert input.exists?
        assert !input.value.empty?

        txt = 'Marysia ma rysia'

        # set an input text
        assert_not_equal input.value, txt
        input.set txt
        assert input.value, txt

        # click a submit button
        btn = @browser.button(:name => 'pipe-submit')
        assert btn.exists?
        btn.click

        # check if pipe output contains expected words
        txt.split.each { |word| assert @browser.text.include? word }
    end

    def test_if_submitting_of_an_input_file_works
        switch_bookmark(:file)

        input_file = @browser.file_field(:name => 'input-file')
        assert input_file.exists?

        file_name = "file.txt"
        txt = 'Zosia nie ma nic'

        # create file to test
        File.open(file_name, "w") { |f| f.puts txt }

        input_file.set file_name

        assert input_file.value, txt
        assert_equal @browser.div(:id => 'input-file').text_field(:class => 'file').value, file_name

        submit

        # check if pipe output contains all words
        txt.split.each { |word| assert @browser.div(:id, 'output').text.include? word }

        File.delete file_name
    end

    def test_if_element_to_download_exists
        @browser.a(:class => 'download-output').exists?
    end

    def test_if_downloading_a_file_with_output_works
        set_input 'Marysia ma rysia'
        submit

        # compare pipe output and generated file content
        output = @browser.pre.text

        file = @browser.a(:class => 'download-output').download
        file_content = file.readlines.join
        file_content.gsub!(/(\n| )+/, ' ')
        file_content.strip!

        assert_equal output, file_content
    end

    def test_picture_output
        set_pipe 'gv-writer --format jpg'
        set_input 'Ala ma kota i psa.'
        submit

        output = @browser.div(:id => 'output')

        assert output.image.exists?
        assert output.image.src.end_with? 'jpg'

        assert output.link.exists?
        assert output.link.href.end_with? 'jpg'
    end

    private

    def set_pipe(txt)
        pipe = @browser.text_field(:name => 'pipe-text')
        pipe.set txt
        return pipe
    end

    def set_input(txt)
        input = @browser.text_field(:name => 'input-text')
        input.set txt
        return input
    end

    def submit
        btn = @browser.button(:name => 'pipe-submit')
        btn.click
    end

    def switch_bookmark(to)
        case to
        when :file
            @browser.a(:id => 'bookmark-text').click
            @browser.radio(:id => 'radio-input-text').set false
            @browser.radio(:id => 'radio-input-file').set
        when :text
            @browser.a(:id => 'bookmark-file').click
            @browser.radio(:id => 'radio-input-file').set false
            @browser.radio(:id => 'radio-input-text').set
        else
            raise "There is no bookmark called #{to}"
        end
    end

end
