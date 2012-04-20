require 'rubygems'
require 'celerity'
require 'test/unit'
require 'uri'

require 'config'

class JsonSiteTest < Test::Unit::TestCase

    def setup
        @browser = Celerity::Browser.new
        #@browser.goto $server_address + 'json.html'
    end

    def teardown
        @browser.close
    end

    def test_if_json_api_description_exists
        @browser.goto $server_address + 'api.html'
        assert @browser.div(:id, 'content').text.downcase.include? 'api for json'
    end

    def test_if_json_page_exists
        @browser.goto $server_address + 'json.html'
        assert @browser.text.include?('{') && @browser.text.include?('}')
    end

    def test_simple_json_request
        json_request('psi-writer', 'Ala ma kota')
        hash = browser_json_content_as_hash

        assert_equal hash['pipe'], 'psi-writer'
        assert hash['input'].kind_of?(String)
        assert_equal hash['input'], 'Ala ma kota'
        assert hash['output'].include? 'Ala_ma_kota'
        assert hash['error'].nil?
    end

    def test_empty_json_request
        json_request('', '')
        hash = browser_json_content_as_hash

        assert hash['pipe'].empty?
        assert hash['input'].empty?
        assert hash['output'].nil?
        assert hash['error'].include? 'some problems'
    end

    def test_json_request_with_json_simple_writer
        json_request('tokenize --lang pl ! json-simple-writer', 'Ala ma kota')
        hash = browser_json_content_as_hash

        assert hash['output'].kind_of?(Array)
        assert_equal hash['output'], ['Ala', 'ma', 'kota']
        assert hash['error'].nil?
    end

    private

    def json_request(pipe, input)
        query = "json.psis?pipe=" + URI.escape(pipe) + "&input=" + URI.escape(input)
        @browser.goto $server_address + query
    end

    def browser_json_content_as_hash
        eval(@browser.text.gsub(':', '=>').gsub('null', 'nil'))
    end

end
